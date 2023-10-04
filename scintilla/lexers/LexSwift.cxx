// This file is part of Notepad2.
// See License.txt for details about distribution and modification.
//! Lexer for Swift.

#include <cassert>
#include <cstring>

#include <string>
#include <string_view>
#include <vector>

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
#include "LexerUtils.h"

using namespace Lexilla;

namespace {

enum {
	SwiftLineStateMaskLineComment = 1,		// line comment
	SwiftLineStateMaskImport = (1 << 1),	// import
};

//KeywordIndex++Autogenerated -- start of section automatically generated
enum {
	KeywordIndex_Keyword = 0,
	KeywordIndex_Directive = 1,
	KeywordIndex_Attribute = 2,
	KeywordIndex_Class = 3,
	KeywordIndex_Struct = 4,
	KeywordIndex_Protocol = 5,
	KeywordIndex_Enumeration = 6,
};
//KeywordIndex--Autogenerated -- end of section automatically generated

enum class KeywordType {
	None = SCE_SWIFT_DEFAULT,
	Class = SCE_SWIFT_CLASS,
	Struct = SCE_SWIFT_STRUCT,
	Protocol = SCE_SWIFT_PROTOCOL,
	Enum = SCE_SWIFT_ENUM,
	Function = SCE_SWIFT_FUNCTION_DEFINITION,
	Label = SCE_SWIFT_LABEL,
};

static_assert(DefaultNestedStateBaseStyle + 1 == SCE_SWIFT_STRING);
static_assert(DefaultNestedStateBaseStyle + 2 == SCE_SWIFT_TRIPLE_STRING);
static_assert(DefaultNestedStateBaseStyle + 3 == SCE_SWIFT_STRING_ED);
static_assert(DefaultNestedStateBaseStyle + 4 == SCE_SWIFT_TRIPLE_STRING_ED);

constexpr bool IsEscapeSequence(int ch) noexcept {
	return AnyOf(ch, '0', '\\', 't', 'n', 'r', '"', '\'', 'u');
}

constexpr bool FollowExpression(int chPrevNonWhite, int stylePrevNonWhite) noexcept {
	return chPrevNonWhite == ')' || chPrevNonWhite == ']'
		|| (stylePrevNonWhite >= SCE_SWIFT_OPERATOR_PF && stylePrevNonWhite <= SCE_SWIFT_NUMBER)
		|| IsIdentifierCharEx(chPrevNonWhite);
}

constexpr bool IsRegexStart(int chPrevNonWhite, int stylePrevNonWhite) noexcept {
	return stylePrevNonWhite == SCE_SWIFT_WORD || !FollowExpression(chPrevNonWhite, stylePrevNonWhite);
}

enum class DelimiterCheck {
	Start,
	End,
	Escape,
};

enum class DelimiterResult {
	False,
	True,
	Regex,
};

DelimiterResult CheckSwiftStringDelimiter(LexAccessor &styler, Sci_PositionU pos, DelimiterCheck what, int &delimiterCount) noexcept {
	++pos; // first '#'
	int count = 1;
	char ch;
	while ((ch = styler[pos]) == '#') {
		++count;
		++pos;
	}
	if (what == DelimiterCheck::End) {
		return (count == delimiterCount) ? DelimiterResult::True : DelimiterResult::False;
	}

	if (what == DelimiterCheck::Start) {
		if (ch == '\"' || ch == '/') {
			delimiterCount = count;
			return (ch == '\"') ? DelimiterResult::True : DelimiterResult::Regex;
		}
	} else {
		if (count == delimiterCount && (ch == '(' || IsEscapeSequence(ch))) {
			return DelimiterResult::True;
		}
	}
	return DelimiterResult::False;
}

constexpr bool IsSpaceEquiv(int state) noexcept {
	return state <= SCE_SWIFT_TASKMARKER;
}

void ColouriseSwiftDoc(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, LexerWordList keywordLists, Accessor &styler) {
	int lineStateLineType = 0;
	int commentLevel = 0;	// nested block comment level

	KeywordType kwType = KeywordType::None;
	int chBeforeIdentifier = 0;

	int delimiterCount = 0;	// count of '#'
	std::vector<int> delimiters;	// nested extended string delimiters in string interpolation
	std::vector<int> nestedState;	// string interpolation "\()"

	int visibleChars = 0;
	int visibleCharsBefore = 0;
	int chBefore = 0;
	int chPrevNonWhite = 0;
	int stylePrevNonWhite = SCE_SWIFT_DEFAULT;
	bool insideRegexRange = false; // inside regex character range []

	StyleContext sc(startPos, lengthDoc, initStyle, styler);
	if (sc.currentLine > 0) {
		int lineState = styler.GetLineState(sc.currentLine - 1);
		/*
		2: lineStateLineType
		6: commentLevel
		8: delimiterCount
		3: nestedState count
		3*4: nestedState
		*/
		commentLevel = (lineState >> 2) & 0x3f;
		delimiterCount = (lineState >> 8) & 0xff;
		lineState >>= 16;
		if (lineState) {
			UnpackLineState(lineState, nestedState);
		}
		if (delimiterCount) {
			// TODO: backtrack to the line without string interpolation.
			delimiters.push_back(delimiterCount);
		}
	}
	if (startPos != 0 && IsSpaceEquiv(initStyle)) {
		// look back for better regex colouring
		LookbackNonWhite(styler, startPos, SCE_SWIFT_TASKMARKER, chPrevNonWhite, stylePrevNonWhite);
	}

	while (sc.More()) {
		switch (sc.state) {
		case SCE_SWIFT_OPERATOR:
		case SCE_SWIFT_OPERATOR2:
		case SCE_SWIFT_OPERATOR_PF:
			sc.SetState(SCE_SWIFT_DEFAULT);
			break;

		case SCE_SWIFT_NUMBER:
			if (!IsDecimalNumberEx(sc.chPrev, sc.ch, sc.chNext)) {
				sc.SetState(SCE_SWIFT_DEFAULT);
			}
			break;

		case SCE_SWIFT_IDENTIFIER:
		case SCE_SWIFT_IDENTIFIER_BT:
		case SCE_SWIFT_ATTRIBUTE:
		case SCE_SWIFT_DIRECTIVE:
		case SCE_SWIFT_VARIABLE:
			if (!IsIdentifierCharEx(sc.ch)) {
				if (sc.state == SCE_SWIFT_IDENTIFIER_BT) {
					if (sc.ch == '`') {
						sc.Forward();
					}
				} else if (sc.state == SCE_SWIFT_IDENTIFIER || sc.state == SCE_SWIFT_DIRECTIVE) {
					char s[128];
					sc.GetCurrent(s, sizeof(s));
					if (sc.state == SCE_SWIFT_DIRECTIVE) {
						if (!keywordLists[KeywordIndex_Directive].InListPrefixed(s + 1, '(')) {
							// required for code folding
							sc.ChangeState(SCE_SWIFT_DEFAULT);
						}
					} else if (keywordLists[KeywordIndex_Keyword].InList(s)) {
						sc.ChangeState(SCE_SWIFT_WORD);
						if (StrEqual(s, "import")) {
							if (visibleChars == sc.LengthCurrent()) {
								lineStateLineType = SwiftLineStateMaskImport;
							}
						} else if (StrEqualsAny(s, "class", "extension", "typealias", "as", "is")) {
							kwType = KeywordType::Class;
						} else if (StrEqual(s, "struct")) {
							kwType = KeywordType::Struct;
						} else if (StrEqual(s, "protocol")) {
							kwType = KeywordType::Protocol;
						} else if (StrEqual(s, "enum")) {
							kwType = KeywordType::Enum;
						} else if (StrEqual(s, "func")) {
							kwType = KeywordType::Function;
						} else if (StrEqualsAny(s, "break", "continue")) {
							kwType = KeywordType::Label;
						}
						if (kwType != KeywordType::None) {
							const int chNext = sc.GetLineNextChar();
							if (!IsIdentifierStartEx(chNext)) {
								kwType = KeywordType::None;
							}
						}
					} else if (keywordLists[KeywordIndex_Class].InList(s)) {
						sc.ChangeState(SCE_SWIFT_CLASS);
					} else if (keywordLists[KeywordIndex_Struct].InList(s)) {
						sc.ChangeState(SCE_SWIFT_STRUCT);
					} else if (keywordLists[KeywordIndex_Protocol].InList(s)) {
						sc.ChangeState(SCE_SWIFT_PROTOCOL);
					} else if (keywordLists[KeywordIndex_Enumeration].InList(s)) {
						sc.ChangeState(SCE_SWIFT_ENUM);
					}
				}
				if (sc.state == SCE_SWIFT_IDENTIFIER || sc.state == SCE_SWIFT_IDENTIFIER_BT) {
					if (sc.ch == ':') {
						if (IsJumpLabelPrevASI(chBefore)) {
							sc.ChangeState(SCE_SWIFT_LABEL);
						}
					} else if (sc.ch != '.') {
						if (kwType != KeywordType::None) {
							sc.ChangeState(static_cast<int>(kwType));
						} else {
							const int chNext = sc.GetDocNextChar(sc.ch == '?');
							if (chNext == '(') {
								sc.ChangeState(SCE_SWIFT_FUNCTION);
							} else if ((chBeforeIdentifier == '<' && (chNext == '>' || chNext == '<'))
								|| (chBeforeIdentifier == '[' && (sc.ch == ']' && AnyOf(sc.chNext, '(', ']')))) {
								// type<type>
								// type<type?>
								// type<type<type>>
								// [type]()
								// [[type]]()
								sc.ChangeState(SCE_SWIFT_CLASS);
							}
						}
					}
				}
				if (sc.state != SCE_SWIFT_WORD && sc.ch != '.') {
					kwType = KeywordType::None;
				}
				stylePrevNonWhite = sc.state;
				sc.SetState(SCE_SWIFT_DEFAULT);
			}
			break;

		case SCE_SWIFT_COMMENTLINE:
		case SCE_SWIFT_COMMENTLINEDOC:
			if (sc.atLineStart) {
				sc.SetState(SCE_SWIFT_DEFAULT);
			} else {
				HighlightTaskMarker(sc, visibleChars, visibleCharsBefore, SCE_SWIFT_TASKMARKER);
			}
			break;

		case SCE_SWIFT_COMMENTBLOCK:
		case SCE_SWIFT_COMMENTBLOCKDOC:
			if (sc.Match('*', '/')) {
				sc.Forward();
				--commentLevel;
				if (commentLevel == 0) {
					sc.ForwardSetState(SCE_SWIFT_DEFAULT);
				}
			} else if (sc.Match('/', '*')) {
				sc.Forward();
				++commentLevel;
			} else if (HighlightTaskMarker(sc, visibleChars, visibleCharsBefore, SCE_SWIFT_TASKMARKER)) {
				continue;
			}
			break;

		case SCE_SWIFT_STRING:
		case SCE_SWIFT_TRIPLE_STRING:
			if (sc.atLineStart && sc.state == SCE_SWIFT_STRING) {
				sc.SetState(SCE_SWIFT_DEFAULT);
			} else if (sc.ch == '\\') {
				if (sc.chNext == '(') {
					nestedState.push_back(sc.state);
					sc.SetState(SCE_SWIFT_OPERATOR2);
					sc.Forward();
				} else if (IsEscapeSequence(sc.chNext)) {
					const int state = sc.state;
					sc.SetState(SCE_SWIFT_ESCAPECHAR);
					sc.Forward();
					sc.ForwardSetState(state);
					continue;
				}
			} else if (sc.ch == '"' && (sc.state == SCE_SWIFT_STRING || sc.MatchNext('"', '"'))) {
				if (sc.state == SCE_SWIFT_TRIPLE_STRING) {
					sc.Advance(2);
				}
				sc.ForwardSetState(SCE_SWIFT_DEFAULT);
			}
			break;

		case SCE_SWIFT_STRING_ED:
		case SCE_SWIFT_TRIPLE_STRING_ED:
			if (sc.atLineStart && sc.state == SCE_SWIFT_STRING_ED) {
				sc.SetState(SCE_SWIFT_DEFAULT);
			} else if (sc.Match('\\', '#')) {
				const int state = sc.state;
				sc.SetState(SCE_SWIFT_ESCAPECHAR);
				sc.Forward();
				const DelimiterResult result = CheckSwiftStringDelimiter(styler, sc.currentPos, DelimiterCheck::Escape, delimiterCount);
				if (result != DelimiterResult::False) {
					sc.Advance(delimiterCount);
					if (sc.ch == '(') {
						nestedState.push_back(state);
						sc.SetState(SCE_SWIFT_OPERATOR2);
					}
				}
				if (sc.state == SCE_SWIFT_ESCAPECHAR){
					sc.ForwardSetState(state);
					continue;
				}
			} else if (sc.ch == '"' && ((sc.state == SCE_SWIFT_STRING_ED && sc.chNext == '#')
				|| (sc.state == SCE_SWIFT_TRIPLE_STRING_ED && sc.MatchNext('"', '"', '#')))) {
				const int offset = (sc.state == SCE_SWIFT_STRING_ED) ? 1 : 3;
				const DelimiterResult result = CheckSwiftStringDelimiter(styler, sc.currentPos + offset, DelimiterCheck::End, delimiterCount);
				if (result != DelimiterResult::False) {
					sc.Advance(delimiterCount + offset);
					sc.SetState(SCE_SWIFT_DEFAULT);
					delimiterCount = TryPopAndPeek(delimiters);
				} else {
					sc.Forward(offset);
				}
			}
			break;

		case SCE_SWIFT_REGEX:
		case SCE_SWIFT_REGEX_ED:
			if (sc.atLineStart && sc.state == SCE_SWIFT_REGEX) {
				sc.SetState(SCE_SWIFT_DEFAULT);
			} else if (sc.ch == '\\') {
				sc.Forward();
			} else if (sc.ch == '[' || sc.ch == ']') {
				insideRegexRange = sc.ch == '[';
			} else if (sc.ch == '/' && !insideRegexRange && (sc.state == SCE_SWIFT_REGEX || sc.chNext == '#')) {
				sc.Forward();
				if (sc.state == SCE_SWIFT_REGEX) {
					sc.SetState(SCE_SWIFT_DEFAULT);
				} else {
					const DelimiterResult result = CheckSwiftStringDelimiter(styler, sc.currentPos, DelimiterCheck::End, delimiterCount);
					if (result != DelimiterResult::False) {
						sc.Advance(delimiterCount);
						sc.SetState(SCE_SWIFT_DEFAULT);
						delimiterCount = TryPopAndPeek(delimiters);
					}
				}
			}
			break;
		}

		if (sc.state == SCE_SWIFT_DEFAULT) {
			if (sc.ch == '/') {
				if (sc.chNext == '/' || sc.chNext == '*') {
					visibleCharsBefore = visibleChars;
					const int chNext = sc.chNext;
					sc.SetState((chNext == '/') ? SCE_SWIFT_COMMENTLINE : SCE_SWIFT_COMMENTBLOCK);
					sc.Forward(2);
					if (sc.ch == ':' || sc.ch == '!' || (sc.ch == chNext && sc.chNext != chNext)) {
						sc.ChangeState((chNext == '/') ? SCE_SWIFT_COMMENTLINEDOC : SCE_SWIFT_COMMENTBLOCKDOC);
					}
					if (chNext == '/') {
						if (visibleChars == 0) {
							lineStateLineType = SwiftLineStateMaskLineComment;
						}
					} else {
						commentLevel = 1;
					}
					continue;
				}
				if (sc.chNext > ' ' && IsRegexStart(chPrevNonWhite, stylePrevNonWhite)) {
					insideRegexRange = false;
					sc.SetState(SCE_SWIFT_REGEX);
				} else {
					sc.SetState(SCE_SWIFT_OPERATOR);
				}
			}
			else if (sc.ch == '"') {
				if (sc.MatchNext('"', '"')) {
					sc.SetState(SCE_SWIFT_TRIPLE_STRING);
					sc.Advance(2);
				} else {
					sc.SetState(SCE_SWIFT_STRING);
				}
			} else if (IsNumberStartEx(sc.chPrev, sc.ch, sc.chNext)) {
				sc.SetState(SCE_SWIFT_NUMBER);
			} else if ((sc.ch == '@' || sc.ch == '`') && IsIdentifierStartEx(sc.chNext)) {
				chBefore = chPrevNonWhite;
				if (chPrevNonWhite != '.') {
					chBeforeIdentifier = chPrevNonWhite;
				}
				sc.SetState((sc.ch == '@') ? SCE_SWIFT_ATTRIBUTE : SCE_SWIFT_IDENTIFIER_BT);
			} else if (sc.ch == '$' && IsIdentifierCharEx(sc.chNext)) {
				sc.SetState(SCE_SWIFT_VARIABLE);
			} else if (sc.ch == '#') {
				if (IsIdentifierStartEx(sc.chNext)) {
					sc.SetState(SCE_SWIFT_DIRECTIVE);
				} else {
					int delimiter = 0;
					const DelimiterResult result = CheckSwiftStringDelimiter(styler, sc.currentPos, DelimiterCheck::Start, delimiter);
					if (result != DelimiterResult::False) {
						insideRegexRange = false;
						delimiterCount = delimiter;
						delimiters.push_back(delimiter);
						sc.SetState((result == DelimiterResult::Regex) ? SCE_SWIFT_REGEX_ED : SCE_SWIFT_STRING_ED);
						sc.Advance(delimiter);
						if (result != DelimiterResult::Regex && sc.Match('"', '"', '"')) {
							sc.ChangeState(SCE_SWIFT_TRIPLE_STRING_ED);
							sc.Advance(2);
						}
					}
				}
			} else if (IsIdentifierStartEx(sc.ch)) {
				chBefore = chPrevNonWhite;
				if (chPrevNonWhite != '.') {
					chBeforeIdentifier = chPrevNonWhite;
				}
				sc.SetState(SCE_SWIFT_IDENTIFIER);
			} else if (sc.ch == '+' || sc.ch == '-') {
				if (sc.ch == sc.chNext) {
					// highlight ++ and -- as different style to simplify regex detection.
					sc.SetState(SCE_SWIFT_OPERATOR_PF);
					sc.Forward();
				} else {
					sc.SetState(SCE_SWIFT_OPERATOR);
				}
			} else if (IsAGraphic(sc.ch)) {
				sc.SetState(SCE_SWIFT_OPERATOR);
				if (!nestedState.empty()) {
					if (sc.ch == '(') {
						nestedState.push_back(SCE_SWIFT_DEFAULT);
					} else if (sc.ch == ')') {
						const int outerState = TakeAndPop(nestedState);
						if (outerState != SCE_SWIFT_DEFAULT) {
							sc.ChangeState(SCE_SWIFT_OPERATOR2);
						}
						sc.ForwardSetState(outerState);
						continue;
					}
				}
			}
		}

		if (!isspacechar(sc.ch)) {
			visibleChars++;
			if (!IsSpaceEquiv(sc.state)) {
				chPrevNonWhite = sc.ch;
				stylePrevNonWhite = sc.state;
			}
		}
		if (sc.atLineEnd) {
			int lineState = (commentLevel << 2) | (delimiterCount << 8) | lineStateLineType;
			if (!nestedState.empty()) {
				lineState |= PackLineState(nestedState) << 16;
			}
			styler.SetLineState(sc.currentLine, lineState);
			lineStateLineType = 0;
			visibleChars = 0;
			visibleCharsBefore = 0;
			kwType = KeywordType::None;
		}
		sc.Forward();
	}

	sc.Complete();
}

struct FoldLineState {
	int lineComment;
	int packageImport;
	constexpr explicit FoldLineState(int lineState) noexcept:
		lineComment(lineState & SwiftLineStateMaskLineComment),
		packageImport((lineState >> 1) & 1) {
	}
};

constexpr bool IsMultilineStringStyle(int style) noexcept {
	return style == SCE_SWIFT_TRIPLE_STRING
		|| style == SCE_SWIFT_TRIPLE_STRING_ED
		|| style == SCE_SWIFT_REGEX_ED
		|| style == SCE_SWIFT_OPERATOR2
		|| style == SCE_SWIFT_ESCAPECHAR;
}

void FoldSwiftDoc(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, LexerWordList, Accessor &styler) {
	const Sci_PositionU endPos = startPos + lengthDoc;
	Sci_Line lineCurrent = styler.GetLine(startPos);
	FoldLineState foldPrev(0);
	int levelCurrent = SC_FOLDLEVELBASE;
	if (lineCurrent > 0) {
		levelCurrent = styler.LevelAt(lineCurrent - 1) >> 16;
		foldPrev = FoldLineState(styler.GetLineState(lineCurrent - 1));
		const Sci_PositionU bracePos = CheckBraceOnNextLine(styler, lineCurrent - 1, SCE_SWIFT_OPERATOR, SCE_SWIFT_TASKMARKER, SCE_SWIFT_DIRECTIVE);
		if (bracePos) {
			startPos = bracePos + 1; // skip the brace
		}
	}

	int levelNext = levelCurrent;
	FoldLineState foldCurrent(styler.GetLineState(lineCurrent));
	Sci_PositionU lineStartNext = styler.LineStart(lineCurrent + 1);
	lineStartNext = sci::min(lineStartNext, endPos);

	char chNext = styler[startPos];
	int styleNext = styler.StyleAt(startPos);
	int style = initStyle;
	int visibleChars = 0;

	while (startPos < endPos) {
		const char ch = chNext;
		const int stylePrev = style;
		style = styleNext;
		chNext = styler[++startPos];
		styleNext = styler.StyleAt(startPos);

		switch (style) {
		case SCE_SWIFT_COMMENTBLOCK:
		case SCE_SWIFT_COMMENTBLOCKDOC: {
			const int level = (ch == '/' && chNext == '*') ? 1 : ((ch == '*' && chNext == '/') ? -1 : 0);
			if (level != 0) {
				levelNext += level;
				startPos++;
				chNext = styler[startPos];
				styleNext = styler.StyleAt(startPos);
			}
		} break;

		case SCE_SWIFT_TRIPLE_STRING:
		case SCE_SWIFT_TRIPLE_STRING_ED:
			if (!IsMultilineStringStyle(stylePrev)) {
				levelNext++;
			} else if (!IsMultilineStringStyle(styleNext)) {
				levelNext--;
			}
			break;

		case SCE_SWIFT_OPERATOR:
			if (ch == '{' || ch == '[' || ch == '(') {
				levelNext++;
			} else if (ch == '}' || ch == ']' || ch == ')') {
				levelNext--;
			}
			break;

		case SCE_SWIFT_DIRECTIVE:
			if (ch == '#') {
				if (chNext == 'i' && styler[startPos + 1] == 'f') {
					levelNext++;
				} else if (chNext == 'e' && styler.Match(startPos, "endif")) {
					levelNext--;
				}
			}
			break;
		}

		if (visibleChars == 0 && !IsSpaceEquiv(style)) {
			++visibleChars;
		}
		if (startPos == lineStartNext) {
			const FoldLineState foldNext(styler.GetLineState(lineCurrent + 1));
			if (foldCurrent.lineComment) {
				levelNext += foldNext.lineComment - foldPrev.lineComment;
			} else if (foldCurrent.packageImport) {
				levelNext += foldNext.packageImport - foldPrev.packageImport;
			} else if (visibleChars) {
				const Sci_PositionU bracePos = CheckBraceOnNextLine(styler, lineCurrent, SCE_SWIFT_OPERATOR, SCE_SWIFT_TASKMARKER, SCE_SWIFT_DIRECTIVE);
				if (bracePos) {
					levelNext++;
					startPos = bracePos + 1; // skip the brace
					style = SCE_SWIFT_OPERATOR;
					chNext = styler[startPos];
					styleNext = styler.StyleAt(startPos);
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
			lineStartNext = sci::min(lineStartNext, endPos);
			levelCurrent = levelNext;
			foldPrev = foldCurrent;
			foldCurrent = foldNext;
			visibleChars = 0;
		}
	}
}

}

LexerModule lmSwift(SCLEX_SWIFT, ColouriseSwiftDoc, "swift", FoldSwiftDoc);
