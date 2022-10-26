// This file is part of Notepad2.
// See License.txt for details about distribution and modification.
//! Lexer VHDL

#include <cassert>
#include <cstring>

#include <string>
#include <string_view>

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

enum {
	VHDLLineStateMaskLineComment = 1,
	VHDLLineStateMaskDirective = 1 << 1,
	VHDLLineStateMaskAttribute = 1 << 2,
	VHDLLineStateMaskCodeFolding = 1 << 3,
};

//KeywordIndex++Autogenerated -- start of section automatically generated
enum {
	KeywordIndex_Keyword = 0,
	KeywordIndex_CodeFolding = 1,
	KeywordIndex_Type = 2,
	KeywordIndex_Function = 5,
	KeywordIndex_Constant = 6,
	KeywordIndex_Package = 7,
};
//KeywordIndex--Autogenerated -- end of section automatically generated

enum class KeywordType {
	None,
	End,
	Attribute = SCE_VHDL_ATTRIBUTE,
	Constant = SCE_VHDL_CONSTANT,
	Function = SCE_VHDL_FUNCTION,
	Package = SCE_VHDL_PACKAGE,
	Type = SCE_VHDL_TYPE,
};

constexpr bool IsSpaceEquiv(int state) noexcept {
	return state <= SCE_VHDL_TASKMARKER;
}

void ColouriseVHDLDoc(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, LexerWordList keywordLists, Accessor &styler) {
	int lineState = 0;
	int parenCount = 0;
	int visibleChars = 0;
	KeywordType kwType = KeywordType::None;
	int chBeforeIdentifier = 0;
	int chPrevNonWhite = 0;

	StyleContext sc(startPos, lengthDoc, initStyle, styler);
	if (sc.currentLine > 0) {
		lineState = styler.GetLineState(sc.currentLine - 1);
		/*
		8: lineState
		4: kwType
		20: parenCount
		*/
		parenCount = lineState >> 12;
		kwType = static_cast<KeywordType>((lineState >> 8) & 15);
		lineState &= VHDLLineStateMaskAttribute | VHDLLineStateMaskCodeFolding;
	}
	if (startPos != 0 && IsSpaceEquiv(initStyle)) {
		int stylePrevNonWhite = SCE_VHDL_DEFAULT;
		LookbackNonWhite(styler, startPos, SCE_VHDL_TASKMARKER, chPrevNonWhite, stylePrevNonWhite);
	}

	while (sc.More()) {
		switch (sc.state) {
		case SCE_VHDL_OPERATOR:
		case SCE_VHDL_OPERATOR2:
			sc.SetState(SCE_VHDL_DEFAULT);
			break;

		case SCE_VHDL_NUMBER:
			if (!(IsDecimalNumber(sc.chPrev, sc.ch, sc.chNext) || sc.ch == '#')) {
				sc.SetState(SCE_VHDL_DEFAULT);
			}
			break;

		case SCE_VHDL_ATTRIBUTE:
		case SCE_VHDL_DIRECTIVE:
		case SCE_VHDL_IDENTIFIER:
			if (!IsIdentifierCharEx(sc.ch)) {
				if (sc.state == SCE_VHDL_IDENTIFIER) {
					char s[128];
					sc.GetCurrentLowered(s, sizeof(s));
					const KeywordType prevWord = kwType;
					kwType = KeywordType::None;
					const bool demoted = parenCount != 0 || chBeforeIdentifier == '.'
						|| (lineState & (VHDLLineStateMaskDirective | VHDLLineStateMaskAttribute)) != 0;
					if (keywordLists[KeywordIndex_CodeFolding].InList(s)) {
						if (!demoted) {
							if (StrEqual(s, "end")) {
								kwType = KeywordType::End;
							} else if (StrEqual(s, "package")) {
								kwType = KeywordType::Package;
							} else if (StrEqualsAny(s, "function", "procedure")) {
								kwType = KeywordType::Function;
							}
							if (kwType == KeywordType::End) {
								lineState &= ~VHDLLineStateMaskCodeFolding;
							} else {
								lineState |= VHDLLineStateMaskCodeFolding;
							}
						}
						sc.ChangeState((demoted || prevWord == KeywordType::End) ? SCE_VHDL_KEYWORD_DEMOTED : SCE_VHDL_FOLDING_KEYWORD);
					} else if (keywordLists[KeywordIndex_Keyword].InList(s)) {
						if (!demoted) {
							if (StrEqual(s, "begin")) {
								lineState &= ~VHDLLineStateMaskCodeFolding;
							} else if (StrEqualsAny(s, "type", "subtype")) {
								kwType = KeywordType::Type;
							} else if (StrEqual(s, "attribute")) {
								kwType = KeywordType::Attribute;
								lineState |= VHDLLineStateMaskAttribute;
							} else if (StrEqual(s, "wait") || (prevWord == KeywordType::End && StrEqual(s, "postponed"))) {
								// wait for time_expression;
								// end postponed process;
								kwType = KeywordType::End;
							} else if (StrEqual(s, "constant")) {
								kwType = KeywordType::Constant;
							}
						} else if (parenCount != 0 && StrEqualsAny(s, "in", "out", "inout")) {
							kwType = KeywordType::Type;
						}
						sc.ChangeState(demoted ? SCE_VHDL_KEYWORD_DEMOTED : SCE_VHDL_KEYWORD);
					} else if (keywordLists[KeywordIndex_Type].InList(s)) {
						sc.ChangeState(SCE_VHDL_STDTYPE);
					} else if (keywordLists[KeywordIndex_Function].InListPrefixed(s, '(')) {
						sc.ChangeState(SCE_VHDL_STDFUNCTION);
					} else if (keywordLists[KeywordIndex_Constant].InList(s)) {
						sc.ChangeState(SCE_VHDL_CONSTANT);
					} else if (keywordLists[KeywordIndex_Package].InList(s)) {
						sc.ChangeState(SCE_VHDL_PACKAGE);
					} else if (prevWord > KeywordType::End) {
						sc.ChangeState(static_cast<int>(prevWord));
					} else if (sc.Match('\'', '(') || chBeforeIdentifier == ':') {
						sc.ChangeState(SCE_VHDL_TYPE);
					} else if (parenCount == 0 && sc.ch == ':' && sc.chNext != '=' && visibleChars == sc.LengthCurrent()) {
						sc.ChangeState(SCE_VHDL_LABEL);
					} else if (sc.GetLineNextChar() == '(') {
						sc.ChangeState(SCE_VHDL_FUNCTION);
					}
				}
				sc.SetState(sc.state != SCE_VHDL_DIRECTIVE && sc.Match('\'', '(') ? SCE_VHDL_OPERATOR : SCE_VHDL_DEFAULT);
			}
			break;

		case SCE_VHDL_ESCAPE_IDENTIFIER:
			if (sc.ch == '\\') {
				sc.Forward();
				if (sc.ch != '\\') {
					if (sc.Match('\'', '(')) {
						sc.ChangeState(SCE_VHDL_TYPE);
						sc.SetState(SCE_VHDL_OPERATOR);
					} else if (kwType > KeywordType::End) {
						sc.ChangeState(static_cast<int>(kwType));
					}
					kwType = KeywordType::None;
					if (sc.state != SCE_VHDL_OPERATOR) {
						sc.SetState(SCE_VHDL_DEFAULT);
					}
				}
			} else if (!IsGraphic(sc.ch)) {
				kwType = KeywordType::None;
				sc.SetState(SCE_VHDL_DEFAULT);
			}
			break;

		case SCE_VHDL_STRING:
			if (sc.atLineStart) {
				sc.SetState(SCE_VHDL_DEFAULT);
			} else if (sc.ch == '\"') {
				sc.Forward();
				if (sc.ch != '\"') {
					sc.SetState(SCE_VHDL_DEFAULT);
				}
			}
			break;

		case SCE_VHDL_CHARACTER:
			if (sc.atLineStart) {
				sc.SetState(SCE_VHDL_DEFAULT);
			} else if (sc.ch == '\'') {
				sc.ForwardSetState(SCE_VHDL_DEFAULT);
			}
			break;

		case SCE_VHDL_COMMENTLINE:
		case SCE_VHDL_COMMENTLINEDOC:
			if (sc.atLineStart) {
				sc.SetState(SCE_VHDL_DEFAULT);
			}
			break;

		case SCE_VHDL_COMMENTBLOCK:
		case SCE_VHDL_COMMENTBLOCKDOC:
			if (sc.Match('*', '/')) {
				sc.Forward();
				sc.ForwardSetState(SCE_VHDL_DEFAULT);
			}
			break;
		}

		if (sc.state == SCE_VHDL_DEFAULT) {
			if (sc.Match('-', '-') || sc.Match('/', '*')) {
				const int chNext = sc.GetRelative(2);
				if (sc.ch == '-') {
					if (visibleChars == 0) {
						lineState = VHDLLineStateMaskLineComment;
					}
					sc.SetState((chNext == '!') ? SCE_VHDL_COMMENTLINEDOC : SCE_VHDL_COMMENTLINE);
				} else {
					sc.SetState((chNext == '!' || chNext == '*') ? SCE_VHDL_COMMENTBLOCKDOC : SCE_VHDL_COMMENTBLOCK);
					sc.Forward();
				}
			} else if (sc.ch == '\'' && !IsEOLChar(sc.chNext)) {
				int state = SCE_VHDL_CHARACTER;
				const int after = sc.GetCharAfterNext();
				if (after != '\'') {
					state = IsIdentifierStartEx(sc.chNext) ? SCE_VHDL_ATTRIBUTE : SCE_VHDL_OPERATOR;
				}
				sc.SetState(state);
				if (state == SCE_VHDL_CHARACTER) {
					sc.Forward(); // to fix '''
				}
			} else if (sc.ch == '\"') {
				sc.SetState(SCE_VHDL_STRING);
			} else if (IsNumberStart(sc.ch, sc.chNext)) {
				sc.SetState(SCE_VHDL_NUMBER);
			} else if (sc.ch == '`' && IsIdentifierStart(sc.chNext)) {
				lineState |= VHDLLineStateMaskDirective;
				sc.SetState(SCE_VHDL_DIRECTIVE);
			} else if (IsIdentifierStartEx(sc.ch) || (sc.ch == '\\' && IsGraphic(sc.chNext))) {
				chBeforeIdentifier = chPrevNonWhite;
				sc.SetState((sc.ch == '\\') ? SCE_VHDL_ESCAPE_IDENTIFIER : SCE_VHDL_IDENTIFIER);
			} else if (IsAGraphic(sc.ch)) {
				kwType = KeywordType::None;
				bool paren = false;
				if (sc.ch == ';' && parenCount == 0) {
					lineState &= ~(VHDLLineStateMaskAttribute | VHDLLineStateMaskCodeFolding);
				} else if (sc.ch == '(' || sc.ch == '[' || sc.ch == '{') {
					paren = true;
					++parenCount;
				} else if (sc.ch == ')' || sc.ch == ']' || sc.ch == '}') {
					paren = true;
					--parenCount;
				}
				sc.SetState((paren || parenCount == 0) ? SCE_VHDL_OPERATOR : SCE_VHDL_OPERATOR2);
			}
		}

		if (!isspacechar(sc.ch)) {
			visibleChars++;
			if (!IsSpaceEquiv(sc.state)) {
				chPrevNonWhite = sc.ch;
			}
		}
		if (sc.atLineEnd) {
			styler.SetLineState(sc.currentLine, lineState | (static_cast<int>(kwType) << 8) | (parenCount << 12));
			lineState &= VHDLLineStateMaskAttribute | VHDLLineStateMaskCodeFolding;
			visibleChars = 0;
		}
		sc.Forward();
	}

	sc.Complete();
}

constexpr int GetLineCommentState(int lineState) noexcept {
	return lineState & VHDLLineStateMaskLineComment;
}

enum class CodeFolding {
	Normal,
	Is,
	When,
	For,
	Units,
};

bool FindCodeFolding(LexAccessor &styler, CodeFolding folding, Sci_PositionU startPos) noexcept {
	const Sci_PositionU endPos = styler.Length();
	int style = SCE_VHDL_DEFAULT;
	for (; startPos < endPos; startPos++) {
		const int stylePrev = style;
		style = styler.StyleAt(startPos);
		if (IsSpaceEquiv(style)) {
			continue;
		}
		char ch = styler[startPos];
		if (folding == CodeFolding::Units) {
			return ch != '<'; // units <>;
		}
		if (style == SCE_VHDL_OPERATOR) {
			if (ch == ';' && folding != CodeFolding::When) {
				return false;
			}
		} else if (style == SCE_VHDL_FOLDING_KEYWORD) {
			// for ... loop ... end loop
			return folding == CodeFolding::For && UnsafeLower(ch) == 'e' && styler.MatchIgnoreCase(startPos, "end");
		} else if (style == SCE_VHDL_KEYWORD && style != stylePrev) {
			ch = UnsafeLower(ch);
			switch (folding) {
			case CodeFolding::Is:
				if (ch == 'i' && UnsafeLower(styler.SafeGetCharAt(startPos + 1)) == 's') {
					for (Sci_PositionU pos = startPos + 2; pos < endPos; pos++) {
						const int styleNext = styler.StyleAt(pos);
						if (!IsSpaceEquiv(styleNext)) {
							// find `is`, but not `is new`
							return styleNext != SCE_VHDL_KEYWORD || !styler.MatchIgnoreCase(pos, "new");
						}
					}
				}
				break;

			case CodeFolding::When:
				if (ch == 'b' && styler.MatchIgnoreCase(startPos, "begin")) {
					return true;
				}
				break;

			case CodeFolding::For:
				if (ch == 'u' && styler.MatchIgnoreCase(startPos, "use")) {
					return false;
				}
				break;

			default:
				break;
			}

		}
	}
	return folding != CodeFolding::For;
}

void FoldVHDLDoc(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, LexerWordList, Accessor &styler) {
	if (startPos != 0) {
		BacktrackToStart(styler, VHDLLineStateMaskCodeFolding, startPos, lengthDoc, initStyle);
	}

	const Sci_PositionU endPos = startPos + lengthDoc;
	Sci_Line lineCurrent = styler.GetLine(startPos);
	int levelCurrent = SC_FOLDLEVELBASE;
	int lineCommentPrev = 0;
	if (lineCurrent > 0) {
		levelCurrent = styler.LevelAt(lineCurrent - 1) >> 16;
		lineCommentPrev = GetLineCommentState(styler.GetLineState(lineCurrent - 1));
	}

	int levelNext = levelCurrent;
	int lineCommentCurrent = GetLineCommentState(styler.GetLineState(lineCurrent));
	Sci_PositionU lineStartNext = styler.LineStart(lineCurrent + 1);
	Sci_PositionU lineEndPos = sci::min(lineStartNext, endPos) - 1;

	char buf[16]; // configuration
	constexpr int MaxFoldWordLength = sizeof(buf) - 1;
	int wordLen = 0;

	int styleNext = styler.StyleAt(startPos);
	int style = initStyle;

	for (Sci_PositionU i = startPos; i < endPos; i++) {
		const char ch = styler[i];
		const int stylePrev = style;
		style = styleNext;
		styleNext = styler.StyleAt(i + 1);

		switch (style) {
		case SCE_VHDL_COMMENTBLOCK:
		case SCE_VHDL_COMMENTBLOCKDOC:
			if (style != stylePrev) {
				levelNext++;
			} else if (style != styleNext) {
				levelNext--;
			}
			break;

		case SCE_VHDL_OPERATOR:
			if (ch == '{' || ch == '[' || ch == '(') {
				levelNext++;
			} else if (ch == '}' || ch == ']' || ch == ')') {
				levelNext--;
			}
			break;

		case SCE_VHDL_DIRECTIVE:
		case SCE_VHDL_FOLDING_KEYWORD:
			if (wordLen < MaxFoldWordLength) {
				buf[wordLen++] = UnsafeLower(ch);
			}
			if (styleNext != style) {
				buf[wordLen] = '\0';
				wordLen = 0;
				if (style == SCE_VHDL_DIRECTIVE) {
					if (StrEqual(buf, "`if")) {
						levelNext++;
					} else if (StrEqual(buf, "`end")) {
						levelNext--;
					}
				} else {
					if (StrEqual(buf, "end")) {
						levelNext--;
					} else {
						CodeFolding folding = CodeFolding::Normal;
						if (StrEqualsAny(buf, "context", "function", "package", "procedure", "view")) {
							folding = CodeFolding::Is;
						} else if (StrEqual(buf, "when")) {
							folding = CodeFolding::When;
						} else if (StrEqual(buf, "for")) {
							folding = CodeFolding::For;
						} else if (StrEqual(buf, "units")) {
							folding = CodeFolding::Units;
						}
						if (folding == CodeFolding::Normal || FindCodeFolding(styler, folding, i + 1)) {
							levelNext++;
						}
					}
				}
			}
			break;
		}

		if (i == lineEndPos) {
			const int lineCommentNext = GetLineCommentState(styler.GetLineState(lineCurrent + 1));
			if (lineCommentCurrent) {
				levelNext += lineCommentNext - lineCommentPrev;
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
			lineCommentPrev = lineCommentCurrent;
			lineCommentCurrent = lineCommentNext;
		}
	}
}

}

LexerModule lmVHDL(SCLEX_VHDL, ColouriseVHDLDoc, "vhdl", FoldVHDLDoc);
