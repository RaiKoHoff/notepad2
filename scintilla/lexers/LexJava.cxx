// This file is part of Notepad2.
// See License.txt for details about distribution and modification.
//! Lexer for Java, Android IDL, BeanShell.

#include <cassert>
#include <cstring>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "CharacterSet.h"
#include "LexerModule.h"

using namespace Scintilla;

namespace {

struct EscapeSequence {
	int outerState = SCE_JAVA_DEFAULT;
	int digitsLeft = 0;
	int numBase = 0;

	// highlight any character as escape sequence.
	bool resetEscapeState(int state, int chNext) noexcept {
		if (IsEOLChar(chNext)) {
			return false;
		}
		outerState = state;
		digitsLeft = 1;
		numBase = 16;
		if (chNext == 'u') {
			digitsLeft = 5;
		} else if (IsOctalDigit(chNext)) {
			digitsLeft = 3;
			numBase = 8;
		}
		return true;
	}
	bool atEscapeEnd(int ch) noexcept {
		--digitsLeft;
		return digitsLeft <= 0 || !IsADigit(ch, numBase);
	}
};

enum {
	JavaLineStateMaskLineComment = 1, // line comment
	JavaLineStateMaskImport = 1 << 1, // import
};

enum class DocTagState {
	None,
	At,				// @param x
	InlineAt,		// {@link package.class#member label}
	TagOpen,		// <tag>
	TagClose,		// </tag>
};

constexpr bool IsSpaceEquiv(int state) noexcept {
	return state <= SCE_JAVA_TASKMARKER;
}

void ColouriseJavaDoc(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, LexerWordList keywordLists, Accessor &styler) {
	int lineStateLineType = 0;

	int kwType = SCE_JAVA_DEFAULT;
	int chBeforeIdentifier = 0;
	int chBefore = 0;

	int visibleChars = 0;
	int visibleCharsBefore = 0;
	DocTagState docTagState = DocTagState::None;
	EscapeSequence escSeq;

	StyleContext sc(startPos, lengthDoc, initStyle, styler);
	if (startPos == 0 && sc.Match('#', '!')) {
		// Shell Shebang at beginning of file
		sc.SetState(SCE_JAVA_COMMENTLINE);
		sc.Forward();
		lineStateLineType = JavaLineStateMaskLineComment;
	}

	while (sc.More()) {
		switch (sc.state) {
		case SCE_JAVA_OPERATOR:
			sc.SetState(SCE_JAVA_DEFAULT);
			break;

		case SCE_JAVA_NUMBER:
			if (!IsDecimalNumberEx(sc.chPrev, sc.ch, sc.chNext)) {
				sc.SetState(SCE_JAVA_DEFAULT);
			}
			break;

		case SCE_JAVA_IDENTIFIER:
			if (!IsIdentifierCharEx(sc.ch)) {
				char s[128];
				sc.GetCurrent(s, sizeof(s));
				if (s[0] == '@') {
					if (strcmp(s + 1, "interface") == 0) {
						sc.ChangeState(SCE_JAVA_WORD);
						kwType = SCE_JAVA_ANNOTATION;
					} else {
						sc.ChangeState(SCE_JAVA_ANNOTATION);
						continue;
					}
				} else if (keywordLists[0]->InList(s)) {
					const bool demoted = (chBefore == '.' || chBefore == ':');
					sc.ChangeState(demoted ? SCE_JAVA_WORD_DEMOTED : SCE_JAVA_WORD);
					if (strcmp(s, "import") == 0) {
						if (visibleChars == sc.LengthCurrent()) {
							lineStateLineType = JavaLineStateMaskImport;
						}
					} else if ((!demoted && strcmp(s, "class") == 0) || EqualsAny(s, "extends", "instanceof")) {
						kwType = SCE_JAVA_CLASS;
					} else if (strcmp(s, "enum") == 0) {
						kwType = SCE_JAVA_ENUM;
					} else if (EqualsAny(s, "interface", "implements")) {
						kwType = SCE_JAVA_INTERFACE;
					} else if (EqualsAny(s, "break", "continue")) {
						kwType = SCE_JAVA_LABEL;
					}
					if (kwType != SCE_JAVA_DEFAULT) {
						const int chNext = sc.GetDocNextChar();
						if (!IsIdentifierStart(chNext)) {
							kwType = SCE_JAVA_DEFAULT;
						}
					}
				} else if (keywordLists[1]->InList(s)) {
					sc.ChangeState(SCE_JAVA_WORD2);
				} else if (keywordLists[2]->InList(s)) {
					sc.ChangeState(SCE_JAVA_DIRECTIVE);
				} else if (keywordLists[3]->InList(s)) {
					sc.ChangeState(SCE_JAVA_CLASS);
				} else if (keywordLists[4]->InList(s)) {
					sc.ChangeState(SCE_JAVA_INTERFACE);
				} else if (keywordLists[5]->InList(s)) {
					sc.ChangeState(SCE_JAVA_ENUM);
				} else if (keywordLists[6]->InList(s)) {
					sc.ChangeState(SCE_JAVA_CONSTANT);
				} else if (sc.ch != '.') {
					if (kwType != SCE_JAVA_DEFAULT) {
						sc.ChangeState(kwType);
					} else {
						const int chNext = sc.GetDocNextChar(sc.ch == ')');
						if (chNext == '(') {
							if (sc.ch == ')') {
								if (chBeforeIdentifier == '(') {
									// (type)(expression)
									sc.ChangeState(SCE_JAVA_CLASS);
								}
							} else {
								sc.ChangeState(SCE_JAVA_FUNCTION);
							}
						} else if (sc.Match('[', ']') || sc.Match(':', ':')
							|| (chBeforeIdentifier == '<' && (chNext == '>' || chNext == '<'))
							|| IsIdentifierStartEx(chNext)
							|| (chBeforeIdentifier == '(' && sc.ch == ')' && IsADigit(chNext))) {
							// type[]
							// type::method
							// type identifier
							// type<type>
							// type<type<type>>
							// (type)expression, (type)++identifier, (type)--identifier
							sc.ChangeState(SCE_JAVA_CLASS);
						}
					}
				}
				if (sc.state != SCE_JAVA_WORD && sc.ch != '.') {
					kwType = SCE_JAVA_DEFAULT;
				}
				sc.SetState(SCE_JAVA_DEFAULT);
			}
			break;

		case SCE_JAVA_ANNOTATION:
			if (sc.ch == '.' || sc.ch == '$') {
				sc.SetState(SCE_JAVA_OPERATOR);
				sc.ForwardSetState(SCE_JAVA_ANNOTATION);
				continue;
			}
			if (!IsIdentifierCharEx(sc.ch)) {
				sc.SetState(SCE_JAVA_DEFAULT);
			}
			break;

		case SCE_JAVA_COMMENTLINE:
		case SCE_JAVA_COMMENTLINEDOC:
			if (sc.atLineStart) {
				sc.SetState(SCE_JAVA_DEFAULT);
			} else {
				HighlightTaskMarker(sc, visibleChars, visibleCharsBefore, SCE_JAVA_TASKMARKER);
			}
			break;

		case SCE_JAVA_COMMENTBLOCK:
			if (sc.Match('*', '/')) {
				sc.Forward();
				sc.ForwardSetState(SCE_JAVA_DEFAULT);
			} else if (HighlightTaskMarker(sc, visibleChars, visibleCharsBefore, SCE_JAVA_TASKMARKER)) {
				continue;
			}
			break;

		case SCE_JAVA_COMMENTBLOCKDOC:
			switch (docTagState) {
			case DocTagState::At:
				docTagState = DocTagState::None;
				break;
			case DocTagState::InlineAt:
				if (sc.ch == '}') {
					docTagState = DocTagState::None;
					sc.SetState(SCE_JAVA_COMMENTTAGAT);
					sc.ForwardSetState(SCE_JAVA_COMMENTBLOCKDOC);
				}
				break;
			case DocTagState::TagOpen:
			case DocTagState::TagClose:
				if (sc.Match('/', '>') || sc.ch == '>') {
					docTagState = DocTagState::None;
					sc.SetState(SCE_JAVA_COMMENTTAGHTML);
					sc.Forward((sc.ch == '/') ? 2 : 1);
					sc.SetState(SCE_JAVA_COMMENTBLOCKDOC);
				}
				break;
			default:
				break;
			}
			if (sc.Match('*', '/')) {
				sc.Forward();
				sc.ForwardSetState(SCE_JAVA_DEFAULT);
			} else if (sc.ch == '@' && IsAlpha(sc.chNext) && IsCommentTagPrev(sc.chPrev)) {
				docTagState = DocTagState::At;
				sc.SetState(SCE_JAVA_COMMENTTAGAT);
			} else if (sc.Match('{', '@') && IsAlpha(sc.GetRelative(2))) {
				docTagState = DocTagState::InlineAt;
				sc.SetState(SCE_JAVA_COMMENTTAGAT);
				sc.Forward();
			} else if (sc.ch == '<') {
				if (IsAlpha(sc.chNext)) {
					docTagState = DocTagState::TagOpen;
					sc.SetState(SCE_JAVA_COMMENTTAGHTML);
				} else if (sc.chNext == '/' && IsAlpha(sc.GetRelative(2))) {
					docTagState = DocTagState::TagClose;
					sc.SetState(SCE_JAVA_COMMENTTAGHTML);
					sc.Forward();
				}
			} else if (HighlightTaskMarker(sc, visibleChars, visibleCharsBefore, SCE_JAVA_TASKMARKER)) {
				continue;
			}
			break;

		case SCE_JAVA_COMMENTTAGAT:
		case SCE_JAVA_COMMENTTAGHTML:
			if (!(IsIdentifierChar(sc.ch) || sc.ch == '-' || sc.ch == ':')) {
				sc.SetState(SCE_JAVA_COMMENTBLOCKDOC);
				continue;
			}
			break;

		case SCE_JAVA_CHARACTER:
			if (sc.atLineStart) {
				sc.SetState(SCE_JAVA_DEFAULT);
			} else if (sc.ch == '\\') {
				if (escSeq.resetEscapeState(sc.state, sc.chNext)) {
					sc.SetState(SCE_JAVA_ESCAPECHAR);
					sc.Forward();
				}
			} else if (sc.ch == '\'') {
				sc.ForwardSetState(SCE_JAVA_DEFAULT);
			}
			break;

		case SCE_JAVA_STRING:
		case SCE_JAVA_TRIPLE_STRING:
			if (sc.state == SCE_JAVA_STRING && sc.atLineStart) {
				sc.SetState(SCE_JAVA_DEFAULT);
			} else if (sc.ch == '\\') {
				if (escSeq.resetEscapeState(sc.state, sc.chNext)) {
					sc.SetState(SCE_JAVA_ESCAPECHAR);
					sc.Forward();
				}
			//} else if (sc.ch == '%') {
			//} else if (sc.ch == '{' && IsADigit(sc.chNext)) {
			} else if (sc.ch == '"' && (sc.state == SCE_JAVA_STRING || sc.Match('"', '"', '"'))) {
				if (sc.state == SCE_JAVA_TRIPLE_STRING) {
					sc.Forward(2);
				}
				sc.ForwardSetState(SCE_JAVA_DEFAULT);
			}
			break;

		case SCE_JAVA_ESCAPECHAR:
			if (escSeq.atEscapeEnd(sc.ch)) {
				sc.SetState(escSeq.outerState);
				continue;
			}
			break;
		}

		if (sc.state == SCE_JAVA_DEFAULT) {
			if (sc.Match('/', '/')) {
				const int chNext = sc.GetRelative(2);
				sc.SetState((chNext == '!' || chNext == '/') ? SCE_JAVA_COMMENTLINEDOC : SCE_JAVA_COMMENTLINE);
				if (visibleChars == 0) {
					lineStateLineType = JavaLineStateMaskLineComment;
				}
				visibleCharsBefore = visibleChars;
			} else if (sc.Match('/', '*')) {
				const int chNext = sc.GetRelative(2);
				sc.SetState((chNext == '*' || chNext == '!') ? SCE_JAVA_COMMENTBLOCKDOC : SCE_JAVA_COMMENTBLOCK);
				sc.Forward();
				visibleCharsBefore = visibleChars;
				docTagState = DocTagState::None;
			} else if (sc.Match('"', '"', '"')) {
				sc.SetState(SCE_JAVA_TRIPLE_STRING);
				sc.Forward(2);
			} else if (sc.ch == '\"') {
				sc.SetState(SCE_JAVA_STRING);
			} else if (sc.ch == '\'') {
				sc.SetState(SCE_JAVA_CHARACTER);
			} else if (IsNumberStart(sc.ch, sc.chNext)) {
				sc.SetState(SCE_JAVA_NUMBER);
			} else if (IsIdentifierStartEx(sc.ch) || sc.Match('@', 'i')) {
				chBefore = sc.chPrev;
				if (chBefore != '.') {
					chBeforeIdentifier = chBefore;
				}
				sc.SetState(SCE_JAVA_IDENTIFIER);
			} else if (sc.ch == '@' && IsIdentifierStartEx(sc.chNext)) {
				sc.SetState(SCE_JAVA_ANNOTATION);
			} else if (isoperator(sc.ch) || sc.ch == '$') {
				sc.SetState(SCE_JAVA_OPERATOR);
			}
		}

		if (!isspacechar(sc.ch)) {
			visibleChars++;
		}
		if (sc.atLineEnd) {
			styler.SetLineState(sc.currentLine, lineStateLineType);
			lineStateLineType = 0;
			visibleChars = 0;
			visibleCharsBefore = 0;
			docTagState = DocTagState::None;
			kwType = SCE_JAVA_DEFAULT;
		}
		sc.Forward();
	}

	sc.Complete();
}

struct FoldLineState {
	int lineComment;
	int packageImport;
	constexpr explicit FoldLineState(int lineState) noexcept:
		lineComment(lineState & JavaLineStateMaskLineComment),
		packageImport((lineState >> 1) & 1) {
	}
};

constexpr bool IsInnerStyle(int style) noexcept {
	return style == SCE_JAVA_ESCAPECHAR
		|| style == SCE_JAVA_COMMENTTAGAT || style == SCE_JAVA_COMMENTTAGHTML;
}

void FoldJavaDoc(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, LexerWordList, Accessor &styler) {
	const Sci_PositionU endPos = startPos + lengthDoc;
	Sci_Line lineCurrent = styler.GetLine(startPos);
	FoldLineState foldPrev(0);
	int levelCurrent = SC_FOLDLEVELBASE;
	bool detachedBrace = false;
	if (lineCurrent > 0) {
		levelCurrent = styler.LevelAt(lineCurrent - 1) >> 16;
		foldPrev = FoldLineState(styler.GetLineState(lineCurrent - 1));
		detachedBrace = HasDetachedBraceOnNextLine(styler, lineCurrent - 1, SCE_JAVA_OPERATOR, SCE_JAVA_TASKMARKER);
	}

	int levelNext = levelCurrent;
	FoldLineState foldCurrent(styler.GetLineState(lineCurrent));
	Sci_PositionU lineStartNext = styler.LineStart(lineCurrent + 1);
	Sci_PositionU lineEndPos = sci::min(lineStartNext, endPos) - 1;

	int styleNext = styler.StyleAt(startPos);
	int style = initStyle;
	int visibleChars = 0;

	for (Sci_PositionU i = startPos; i < endPos; i++) {
		const int stylePrev = style;
		style = styleNext;
		styleNext = styler.StyleAt(i + 1);

		switch (style) {
		case SCE_JAVA_COMMENTBLOCK:
		case SCE_JAVA_COMMENTBLOCKDOC:
		case SCE_JAVA_TRIPLE_STRING:
			if (style != stylePrev && !IsInnerStyle(stylePrev)) {
				levelNext++;
			} else if (style != styleNext && !IsInnerStyle(styleNext)) {
				levelNext--;
			}
			break;

		case SCE_JAVA_OPERATOR: {
			const char ch = styler[i];
			if (ch == '{' || ch == '[' || ch == '(') {
				if (!(ch == '{' && visibleChars == 0 && detachedBrace)) {
					levelNext++;
				}
			} else if (ch == '}' || ch == ']' || ch == ')') {
				levelNext--;
			}
		} break;
		}

		if (visibleChars == 0 && !IsSpaceEquiv(style)) {
			++visibleChars;
		}
		if (i == lineEndPos) {
			detachedBrace = false;
			const FoldLineState foldNext(styler.GetLineState(lineCurrent + 1));
			if (foldCurrent.lineComment) {
				levelNext += foldNext.lineComment - foldPrev.lineComment;
			} else if (foldCurrent.packageImport) {
				levelNext += foldNext.packageImport - foldPrev.packageImport;
			} else if (visibleChars) {
				detachedBrace = HasDetachedBraceOnNextLine(styler, lineCurrent, SCE_JAVA_OPERATOR, SCE_JAVA_TASKMARKER);
				if (detachedBrace) {
					levelNext++;
				}
			}

			const int levelUse = levelCurrent;
			int lev = levelUse | levelNext << 16;
			if (levelUse < levelNext) {
				lev |= SC_FOLDLEVELHEADERFLAG;
			}
			if (lev != styler.LevelAt(lineCurrent)) {
				styler.SetLevel(lineCurrent, lev);
			}

			lineCurrent++;
			lineStartNext = styler.LineStart(lineCurrent + 1);
			lineEndPos = sci::min(lineStartNext, endPos) - 1;
			levelCurrent = levelNext;
			foldPrev = foldCurrent;
			foldCurrent = foldNext;
			visibleChars = 0;
		}
	}
}

}

LexerModule lmJava(SCLEX_JAVA, ColouriseJavaDoc, "java", FoldJavaDoc);
