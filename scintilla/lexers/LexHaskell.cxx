// This file is part of Notepad2.
// See License.txt for details about distribution and modification.
//! Lexer for Haskell

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

// https://www.haskell.org/onlinereport/haskell2010/haskellch2.html#x7-200002.6
struct EscapeSequence {
	int outerState = SCE_HA_DEFAULT;
	int digitsLeft = 0;
	int numBase = 0;

	// highlight any character as escape sequence.
	void resetEscapeState(int state, int chNext) noexcept {
		outerState = state;
		digitsLeft = 0;
		numBase = 16;
		if (chNext == 'x') {
			digitsLeft = 7;
		} else if (chNext == 'o') {
			digitsLeft = 4;
			numBase = 8;
		} else if (IsADigit(chNext)) {
			digitsLeft = 7;
			numBase = 10;
		} else {
			digitsLeft = 1;
		}
	}
	bool atEscapeEnd(int ch) noexcept {
		--digitsLeft;
		return digitsLeft <= 0 || !IsADigitEx(ch, numBase);
	}
};

constexpr bool IsHaskellIdentifierChar(int ch) noexcept {
	return IsIdentifierCharEx(ch) || ch == '\'';
}

enum class KeywordType {
	None = SCE_HA_DEFAULT,
	Module = SCE_HA_MODULE,
	Class = SCE_HA_CLASS,
	Pragma = SCE_HA_PRAGMA,
};

enum {
	HaskellLineStatePragma = 1 << 6,
};

//KeywordIndex++Autogenerated -- start of section automatically generated
enum {
	KeywordIndex_Keyword = 0,
	KeywordIndex_Class = 1,
};
//KeywordIndex--Autogenerated -- end of section automatically generated

void ColouriseHaskellDoc(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, LexerWordList keywordLists, Accessor &styler) {
	int commentLevel = 0;
	int visibleChars = 0;
	int indentCount = 0;
	int lineState = 0;
	KeywordType kwType = KeywordType::None;
	bool lineContinuation = false;
	EscapeSequence escSeq;

	StyleContext sc(startPos, lengthDoc, initStyle, styler);
	if (sc.currentLine > 0) {
		lineState = styler.GetLineState(sc.currentLine - 1);
		commentLevel = (lineState >> 8) & 0xff;
		lineContinuation= (lineState & PyLineStateLineContinuation) != 0;
		lineState &= HaskellLineStatePragma;
	}

	while (sc.More()) {
		switch (sc.state) {
		case SCE_HA_OPERATOR:
		case SCE_HA_PRAGMA:
			sc.SetState(SCE_HA_DEFAULT);
			break;

		case SCE_HA_NUMBER:
			if (!IsDecimalNumberEx(sc.chPrev, sc.ch, sc.chNext)) {
				sc.SetState(SCE_HA_DEFAULT);
			}
			break;

		case SCE_HA_IDENTIFIER:
		case SCE_HA_PREPROCESSOR:
			if (!IsHaskellIdentifierChar(sc.ch)) {
				if (sc.state == SCE_HA_IDENTIFIER) {
					char s[128];
					sc.GetCurrent(s, sizeof(s));
					if (keywordLists[KeywordIndex_Keyword].InList(s)) {
						sc.ChangeState(SCE_HA_KEYWORD);
						kwType = KeywordType::None;
						if (StrEqualsAny(s, "class", "data", "type", "family", "instance")) {
							kwType = KeywordType::Class;
						} else if (StrEqualsAny(s, "module", "import", "qualified")) {
							kwType = KeywordType::Module;
						}
					} else if (keywordLists[KeywordIndex_Class].InList(s)) {
						sc.ChangeState(SCE_HA_CLASS);
					} else if (kwType != KeywordType::None) {
						sc.ChangeState(static_cast<int>(kwType));
					}
					if (sc.state != SCE_HA_KEYWORD) {
						kwType = KeywordType::None;
					}
				}
				sc.SetState(SCE_HA_DEFAULT);
			}
			break;

		case SCE_HA_CHARACTER:
		case SCE_HA_STRING:
			if (sc.atLineStart) {
				if (lineContinuation) {
					lineContinuation = false;
				} else {
					sc.SetState(SCE_HA_DEFAULT);
					break;
				}
			}
			if (sc.ch == '\\') {
				if (IsEOLChar(sc.chNext)) {
					lineContinuation = true;
					lineState |= PyLineStateMaskTripleQuote | PyLineStateLineContinuation;
				} else {
					escSeq.resetEscapeState(sc.state, sc.chNext);
					sc.SetState(SCE_HA_ESCAPECHAR);
					sc.Forward();
				}
			} else if (sc.ch == ((sc.state == SCE_HA_CHARACTER) ? '\'' : '\"')) {
				sc.ForwardSetState(SCE_HA_DEFAULT);
			}
			break;

		case SCE_HA_ESCAPECHAR:
			if (escSeq.atEscapeEnd(sc.ch)) {
				sc.SetState(escSeq.outerState);
				continue;
			}
			break;

		case SCE_HA_COMMENTLINE:
			if (sc.atLineStart) {
				sc.SetState(SCE_HA_DEFAULT);
			}
			break;

		case SCE_HA_COMMENTBLOCK:
			if (sc.atLineStart) {
				lineState |= PyLineStateMaskCommentLine;
			}
			if (sc.Match('{', '-')) {
				commentLevel++;
				sc.Forward();
			} else if (sc.Match('-', '}')) {
				sc.Forward();
				commentLevel--;
				if (commentLevel == 0) {
					sc.ForwardSetState(SCE_HA_DEFAULT);
					if ((lineState & PyLineStateMaskCommentLine) && sc.GetLineNextChar() != '\0') {
						lineState &= HaskellLineStatePragma;
					}
				}
			}
			break;
		}

		if (sc.state == SCE_HA_DEFAULT) {
			if (sc.Match('-', '-')) {
				if (visibleChars == 0) {
					lineState |= PyLineStateMaskCommentLine;
				}
				sc.SetState(SCE_HA_COMMENTLINE);
			} else if (sc.Match('{', '-')) {
				sc.SetState(SCE_HA_COMMENTBLOCK);
				sc.Forward();
				if (sc.chNext == '#') {
					kwType = KeywordType::Pragma;
					lineState = HaskellLineStatePragma;
					sc.ChangeState(SCE_HA_PRAGMA);
					sc.Forward();
				} else {
					commentLevel = 1;
					if (visibleChars == 0) {
						lineState |= PyLineStateMaskCommentLine;
					}
				}
			} else if (sc.ch == '\"') {
				sc.SetState(SCE_HA_STRING);
			} else if (sc.ch == '\'') {
				int state = SCE_HA_CHARACTER;
				if (IsEOLChar(sc.chNext)) {
					state = SCE_HA_OPERATOR;
				} else if (sc.chNext == '\'') {
					state = SCE_HA_IDENTIFIER;
				} else if (sc.chNext != '\\') {
					const int after = sc.GetCharAfterNext();
					if (after != '\'') {
						state = IsIdentifierStartEx(sc.chNext) ? SCE_HA_IDENTIFIER : SCE_HA_OPERATOR;
					}
				}
				sc.SetState(state);
			} else if (IsNumberStartEx(sc.chPrev, sc.ch, sc.chNext)) {
				sc.SetState(SCE_HA_NUMBER);
			} else if (IsIdentifierStartEx(sc.ch)) {
				sc.SetState(SCE_HA_IDENTIFIER);
			} else if (sc.ch == '#') {
				const bool pragma = (lineState & HaskellLineStatePragma) != 0;
				if (pragma && sc.MatchNext('-', '}')) {
					lineState = 0;
					sc.SetState(SCE_HA_PRAGMA);
					sc.Forward(2);
				} else if (!pragma && sc.atLineStart && IsLowerCase(sc.chNext)) {
					sc.SetState(SCE_HA_PREPROCESSOR);
				} else {
                    sc.SetState(SCE_HA_OPERATOR);
				}
			} else if (IsAGraphic(sc.ch)) {
				sc.SetState(SCE_HA_OPERATOR);
				if (visibleChars == 0 && (sc.ch == '}' || sc.ch == ']' || sc.ch == ')')) {
					lineState |= PyLineStateMaskCloseBrace;
				}
			}
		}

		if (visibleChars == 0) {
			if (sc.ch == ' ') {
				++indentCount;
			} else if (sc.ch == '\t') {
				indentCount = GetTabIndentCount(indentCount);
			} else if (!isspacechar(sc.ch)) {
				visibleChars++;
			}
		}
		if (sc.atLineEnd) {
			if (lineState & HaskellLineStatePragma) {
				lineState |= PyLineStateMaskTripleQuote;
			} else if (lineState == 0 && visibleChars == 0) {
				lineState = PyLineStateMaskEmptyLine;
			}
			lineState |= (indentCount << 16) | (commentLevel << 8);
			styler.SetLineState(sc.currentLine, lineState);
			lineState &= HaskellLineStatePragma;
			visibleChars = 0;
			indentCount = 0;
			kwType = KeywordType::None;
		}
		sc.Forward();
	}

	sc.Complete();
}

}

LexerModule lmHaskell(SCLEX_HASKELL, ColouriseHaskellDoc, "haskell", FoldPyDoc);
