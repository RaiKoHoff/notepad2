// This file is part of Notepad2.
// See License.txt for details about distribution and modification.
//! Lexer for GN build.

#include <cassert>
#include <cstring>

#include <string_view>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "CharacterSet.h"
#include "LexerModule.h"

using namespace Lexilla;

namespace {

struct EscapeSequence {
	int digitsLeft = 0;

	bool resetEscapeState(const StyleContext &sc) noexcept {
		if (sc.ch == '\\') {
			if (sc.chNext == '\\' || sc.chNext == '\"' || sc.chNext == '$') {
				digitsLeft = 1;
				return true;
			}
		} else if (sc.ch == '$' && sc.chNext == '0' && sc.GetRelative(2) == 'x') {
			digitsLeft = 3;
			return true;
		}
		return false;
	}
	bool atEscapeEnd(int ch) noexcept {
		--digitsLeft;
		return digitsLeft <= 0 || !IsHexDigit(ch);
	}
};

void ColouriseGNDoc(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, LexerWordList keywordLists, Accessor &styler) {
	int visibleChars = 0;
	int lineStateLineComment = 0;
	int expansionLevel = 0;	// for BracketExpansion
	EscapeSequence escSeq;

	StyleContext sc(startPos, lengthDoc, initStyle, styler);

	while (sc.More()) {
		switch (sc.state) {
		case SCE_GN_OPERATOR:
		case SCE_GN_OPERATOR2:
			sc.SetState(SCE_GN_DEFAULT);
			break;

		case SCE_GN_NUMBER:
			if (!IsDecimalNumber(sc.chPrev, sc.ch, sc.chNext)) {
				sc.SetState(SCE_GN_DEFAULT);
			}
			break;

		case SCE_GN_IDENTIFIER:
			if (!IsIdentifierChar(sc.ch)) {
				char s[128];
				sc.GetCurrent(s, sizeof(s));
				if (keywordLists[0]->InList(s)) {
					sc.ChangeState(SCE_GN_KEYWORD);
				} else if (sc.GetDocNextChar() == '(') {
					const bool builtin = keywordLists[1]->InListPrefixed(s, '(');
					sc.ChangeState(builtin ? SCE_GN_BUILTIN_FUNCTION : SCE_GN_FUNCTION);
				} else if (keywordLists[2]->InList(s)) {
					sc.ChangeState(SCE_GN_BUILTIN_VARIABLE);
				}
				sc.SetState(SCE_GN_DEFAULT);
			}
			break;

		case SCE_GN_STRING:
			if (escSeq.resetEscapeState(sc)) {
				sc.SetState(SCE_GN_ESCAPECHAR);
				sc.Forward((escSeq.digitsLeft == 1)? 1 : 2);
			} else if (sc.ch == ':' || sc.ch == '*') {
				sc.SetState(SCE_GN_OPERATOR2);
				sc.ForwardSetState(SCE_GN_STRING);
				continue;
			} else if (sc.ch == '$') {
				if (sc.chNext == '{') {
					expansionLevel = 1;
					sc.SetState(SCE_GN_OPERATOR2);
					sc.Forward();
				} else if (IsIdentifierStart(sc.chNext)) {
					sc.SetState(SCE_GN_VARIABLE);
				}
			} else if (sc.Match('{', '{')) {
				sc.SetState(SCE_GN_PLACEHOLDER);
				sc.Forward();
			} else if (sc.ch == '\"') {
				sc.ForwardSetState(SCE_GN_DEFAULT);
			} else if (sc.atLineStart) {
				sc.SetState(SCE_GN_DEFAULT);
			}
			break;

		case SCE_GN_VARIABLE:
			if (!IsIdentifierChar(sc.ch)) {
				sc.SetState(SCE_GN_STRING);
				continue;
			}
			break;

		case SCE_GN_PLACEHOLDER:
			if (sc.Match('}', '}')) {
				sc.Forward(2);
				sc.SetState(SCE_GN_STRING);
				continue;
			} else if (!(IsIdentifierChar(sc.ch) || sc.ch == '*')) {
				sc.ChangeState(SCE_GN_STRING);
				continue;
			}
			break;

		case SCE_GN_ESCAPECHAR:
			if (escSeq.atEscapeEnd(sc.ch)) {
				sc.SetState(SCE_GN_STRING);
				continue;
			}
			break;

		case SCE_GN_COMMENT:
			if (sc.atLineStart) {
				sc.SetState(SCE_GN_DEFAULT);
			}
			break;
		}

		if (sc.state == SCE_GN_DEFAULT) {
			if (sc.ch == '#') {
				sc.SetState(SCE_GN_COMMENT);
				if (visibleChars == 0) {
					lineStateLineComment = SimpleLineStateMaskLineComment;
				}
			} else if (sc.ch == '\"') {
				sc.SetState(SCE_GN_STRING);
			} else if (IsADigit(sc.ch)) {
				sc.SetState(SCE_GN_NUMBER);
			} else if (IsIdentifierStart(sc.ch)) {
				sc.SetState(SCE_GN_IDENTIFIER);
			} else if (isoperator(sc.ch)) {
				sc.SetState(expansionLevel ? SCE_GN_OPERATOR2 : SCE_GN_OPERATOR);
				if (expansionLevel) {
					if (sc.ch == '{') {
						++expansionLevel;
					} else if (sc.ch == '}') {
						--expansionLevel;
						if (expansionLevel == 0) {
							sc.ForwardSetState(SCE_GN_STRING);
							continue;
						}
					}
				}
			}
		}

		if (visibleChars == 0 && !isspacechar(sc.ch)) {
			++visibleChars;
		}
		if (sc.atLineEnd) {
			styler.SetLineState(sc.currentLine, lineStateLineComment);
			visibleChars = 0;
			lineStateLineComment = 0;
			expansionLevel = 0;
		}
		sc.Forward();
	}

	sc.Complete();
}

static_assert(SCE_GN_OPERATOR == SCE_SIMPLE_OPERATOR);
}

LexerModule lmGN(SCLEX_GN, ColouriseGNDoc, "gn", FoldSimpleDoc);
