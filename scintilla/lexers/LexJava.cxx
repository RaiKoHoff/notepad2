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
#include "StringUtils.h"
#include "LexerModule.h"

using namespace Lexilla;

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

// for java.util.Formatter
// https://docs.oracle.com/en/java/javase/15/docs/api/java.base/java/util/Formatter.html

constexpr bool IsFormatFlag(int ch) noexcept {
	return AnyOf(ch, ' ', '+', '-', '#', '.', '0', '<', '(', ',');
}

constexpr bool IsFormatSpecifier(int ch) noexcept {
	return AnyOf(ch, 'a', 'A',
					'b', 'B',
					'c', 'C',
					'd',
					'e', 'E',
					'f',
					'g', 'G',
					'h', 'H',
					'n',
					'o',
					's', 'S',
					'x', 'X');
}

constexpr bool IsDateTimeFormatSpecifier(int ch) noexcept {
	return AnyOf(ch, 'H', 'I', 'k', 'l', 'M', 'S', 'L', 'N', 'p', 'z', 'Z', 's', 'Q', // time
		'B', 'b', 'h', 'A', 'a', 'C', 'Y', 'y', 'j', 'm', 'd', 'e', // date
		'R', 'T', 'r', 'D', 'F', 'c'); // date/time
}

inline Sci_Position CheckFormatSpecifier(const StyleContext &sc, bool insideUrl) noexcept {
	if (sc.chNext == '%') {
		return 2;
	}
	if (insideUrl && IsHexDigit(sc.chNext)) {
		// percent encoded URL string
		return 0;
	}
	if (IsASpaceOrTab(sc.chNext) && IsADigit(sc.chPrev)) {
		// ignore word after percent: "5% x"
		return 0;
	}

	Sci_PositionU pos = sc.currentPos + 1;
	if (IsFormatFlag(sc.chNext)) {
		++pos;
	}
	while (pos < sc.lineStartNext) {
		const uint8_t ch = sc.styler[pos];
		if (IsFormatSpecifier(ch)) {
			return pos - sc.currentPos + 1;
		}
		if (ch == 't' || ch == 'T') {
			const uint8_t chNext = sc.styler.SafeGetCharAt(pos + 1);
			if (IsDateTimeFormatSpecifier(chNext)) {
				return pos - sc.currentPos + 2;
			}
			break;
		}
		if (ch == '$') {
			const uint8_t chNext = sc.styler.SafeGetCharAt(pos + 1);
			if (IsFormatFlag(chNext)) {
				++pos;
			}
		} else if (!(IsADigit(ch) || ch == '*' || ch == '.' || ch == ',')) {
			break;
		}
		++pos;
	}
	return 0;
}

inline Sci_Position CheckPlaceholder(const StyleContext &sc) noexcept {
	if (IsADigit(sc.chNext)) {
		// for java.text.MessageFormat, only simplest form: {num}
		// https://docs.oracle.com/en/java/javase/15/docs/api/java.base/java/text/MessageFormat.html
		Sci_PositionU pos = sc.currentPos + 2;
		while (pos < sc.lineStartNext) {
			const uint8_t ch = sc.styler[pos];
			if (ch == '}') {
				return pos - sc.currentPos + 1;
			}
			if (!IsADigit(ch)) {
				break;
			}
			++pos;
		}
	}
	return 0;
}

void ColouriseJavaDoc(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, LexerWordList keywordLists, Accessor &styler) {
	int lineStateLineType = 0;
	bool insideUrl = false;

	int kwType = SCE_JAVA_DEFAULT;
	int chBeforeIdentifier = 0;

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
					if (StrEqual(s, "@interface")) {
						sc.ChangeState(SCE_JAVA_WORD);
						kwType = SCE_JAVA_ANNOTATION;
					} else {
						sc.ChangeState(SCE_JAVA_ANNOTATION);
						continue;
					}
				} else if (keywordLists[0]->InList(s)) {
					sc.ChangeState(SCE_JAVA_WORD);
					if (StrEqual(s, "import")) {
						if (visibleChars == sc.LengthCurrent()) {
							lineStateLineType = JavaLineStateMaskImport;
						}
					} else if (StrEqualsAny(s, "class", "new", "extends", "instanceof", "throws")) {
						kwType = SCE_JAVA_CLASS;
					} else if (StrEqualsAny(s, "interface", "implements")) {
						kwType = SCE_JAVA_INTERFACE;
					} else if (StrEqual(s, "enum")) {
						kwType = SCE_JAVA_ENUM;
					} else if (StrEqual(s, "record")) {
						kwType = SCE_JAVA_RECORD;
					} else if (StrEqualsAny(s, "break", "continue")) {
						kwType = SCE_JAVA_LABEL;
					} else if (StrEqualsAny(s, "if", "while")) {
						// to avoid treating following code as type cast:
						// if (identifier) expression, while (identifier) expression
						kwType = SCE_JAVA_WORD;
					}
					if (kwType != SCE_JAVA_DEFAULT && kwType != SCE_JAVA_WORD) {
						const int chNext = sc.GetDocNextChar();
						if (!IsIdentifierStartEx(chNext)) {
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
				} else if (sc.ch == ':') {
					if (sc.chNext == ':') {
						// type::method
						sc.ChangeState(SCE_JAVA_CLASS);
					} else if (visibleChars == sc.LengthCurrent()) {
						sc.ChangeState(SCE_JAVA_LABEL);
					}
				} else if (sc.ch != '.') {
					if (kwType != SCE_JAVA_DEFAULT && kwType != SCE_JAVA_WORD) {
						sc.ChangeState(kwType);
					} else {
						const int chNext = sc.GetDocNextChar(sc.ch == ')');
						if (sc.ch == ')') {
							if (chBeforeIdentifier == '(' && (chNext == '(' || (kwType != SCE_JAVA_WORD && IsIdentifierCharEx(chNext)))) {
								// (type)(expression)
								// (type)expression, (type)++identifier, (type)--identifier
								sc.ChangeState(SCE_JAVA_CLASS);
							}
						} else if (chNext == '(') {
							sc.ChangeState(SCE_JAVA_FUNCTION);
						} else if (sc.Match('[', ']')
							|| (sc.ch == '<' && (sc.chNext == '>' || sc.chNext == '?'))
							|| (chBeforeIdentifier == '<' && (chNext == '>' || chNext == '<'))
							|| IsIdentifierStartEx(chNext)) {
							// type[] identifier
							// TODO: fix C/C++ style: type identifier[]
							// type<>, type<?>, type<? super T>
							// type<type>
							// type<type<type>>
							// type identifier
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
			} else if (sc.ch == '%') {
				const Sci_Position length = CheckFormatSpecifier(sc, insideUrl);
				if (length != 0) {
					const int state = sc.state;
					sc.SetState(SCE_JAVA_FORMAT_SPECIFIER);
					sc.Forward(length);
					sc.SetState(state);
					continue;
				}
			} else if (sc.ch == '{') {
				const Sci_Position length = CheckPlaceholder(sc);
				if (length != 0) {
					const int state = sc.state;
					sc.SetState(SCE_JAVA_PLACEHOLDER);
					sc.Forward(length);
					sc.SetState(state);
					continue;
				}
			} else if (sc.ch == '"' && (sc.state == SCE_JAVA_STRING || sc.MatchNext('"', '"'))) {
				if (sc.state == SCE_JAVA_TRIPLE_STRING) {
					sc.Forward(2);
				}
				sc.ForwardSetState(SCE_JAVA_DEFAULT);
			} else if (sc.Match(':', '/', '/') && IsLowerCase(sc.chPrev)) {
				insideUrl = true;
			} else if (insideUrl && IsInvalidUrlChar(sc.ch)) {
				insideUrl = false;
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
				visibleCharsBefore = visibleChars;
				sc.SetState(SCE_JAVA_COMMENTLINE);
				if (visibleChars == 0) {
					lineStateLineType = JavaLineStateMaskLineComment;
				}
			} else if (sc.Match('/', '*')) {
				visibleCharsBefore = visibleChars;
				docTagState = DocTagState::None;
				sc.SetState(SCE_JAVA_COMMENTBLOCK);
				sc.Forward(2);
				if (sc.ch == '*' && sc.chNext != '*') {
					sc.ChangeState(SCE_JAVA_COMMENTBLOCKDOC);
				}
				continue;
			} else if (sc.ch == '\"') {
				insideUrl = false;
				if (sc.MatchNext('"', '"')) {
					sc.SetState(SCE_JAVA_TRIPLE_STRING);
					sc.Forward(2);
				} else {
					sc.SetState(SCE_JAVA_STRING);
				}
			} else if (sc.ch == '\'') {
				sc.SetState(SCE_JAVA_CHARACTER);
			} else if (IsNumberStart(sc.ch, sc.chNext)) {
				sc.SetState(SCE_JAVA_NUMBER);
			} else if (IsIdentifierStartEx(sc.ch) || sc.Match('@', 'i')) {
				if (sc.chPrev != '.') {
					chBeforeIdentifier = sc.chPrev;
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
	return AnyOf(style, SCE_JAVA_ESCAPECHAR, SCE_JAVA_FORMAT_SPECIFIER, SCE_JAVA_PLACEHOLDER,
				SCE_JAVA_COMMENTTAGAT, SCE_JAVA_COMMENTTAGHTML, SCE_JAVA_TASKMARKER);
}

void FoldJavaDoc(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, LexerWordList, Accessor &styler) {
	const Sci_PositionU endPos = startPos + lengthDoc;
	Sci_Line lineCurrent = styler.GetLine(startPos);
	FoldLineState foldPrev(0);
	int levelCurrent = SC_FOLDLEVELBASE;
	if (lineCurrent > 0) {
		levelCurrent = styler.LevelAt(lineCurrent - 1) >> 16;
		foldPrev = FoldLineState(styler.GetLineState(lineCurrent - 1));
		const Sci_PositionU bracePos = CheckBraceOnNextLine(styler, lineCurrent - 1, SCE_JAVA_OPERATOR, SCE_JAVA_TASKMARKER);
		if (bracePos) {
			startPos = bracePos + 1; // skip the brace
		}
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
				levelNext++;
			} else if (ch == '}' || ch == ']' || ch == ')') {
				levelNext--;
			}
		} break;
		}

		if (visibleChars == 0 && !IsSpaceEquiv(style)) {
			++visibleChars;
		}
		if (i == lineEndPos) {
			const FoldLineState foldNext(styler.GetLineState(lineCurrent + 1));
			if (foldCurrent.lineComment) {
				levelNext += foldNext.lineComment - foldPrev.lineComment;
			} else if (foldCurrent.packageImport) {
				levelNext += foldNext.packageImport - foldPrev.packageImport;
			} else if (visibleChars) {
				const Sci_PositionU bracePos = CheckBraceOnNextLine(styler, lineCurrent, SCE_JAVA_OPERATOR, SCE_JAVA_TASKMARKER);
				if (bracePos) {
					levelNext++;
					i = bracePos; // skip the brace
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
