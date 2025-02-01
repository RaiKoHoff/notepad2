// This file is part of Notepad4.
// See License.txt for details about distribution and modification.
//! Lexer for OCaml

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

// https://v2.ocaml.org/releases/5.0/htmlman/lex.html#sss:character-literals
struct EscapeSequence {
	int outerState = SCE_OCAML_DEFAULT;
	int digitsLeft = 0;
	int numBase = 0;
	bool brace = false;

	// highlight any character as escape sequence.
	void resetEscapeState(int state, int chNext) noexcept {
		outerState = state;
		digitsLeft = 1;
		numBase = 16;
		brace = false;
		if (chNext == 'x') {
			digitsLeft = 3;
		} else if (chNext == 'u') {
			digitsLeft = 7;
		} else if (chNext == 'o') {
			digitsLeft = 4;
			numBase = 8;
		} else if (IsADigit(chNext)) {
			digitsLeft = 3;
			numBase = 10;
		}
	}
	bool atEscapeEnd(int ch) noexcept {
		--digitsLeft;
		return digitsLeft <= 0 || !IsADigitEx(ch, numBase);
	}
};

constexpr bool IsOCamlIdentifierChar(int ch) noexcept {
	return IsIdentifierChar(ch) || ch == '\'';
}

constexpr bool IsQuotedStringId(int ch) noexcept {
	return IsLowerCase(ch) || ch == '_';
}

// https://v2.ocaml.org/releases/5.0/api/Printf.html
constexpr bool IsFormatSpecifier(char ch) noexcept {
	return AnyOf(ch, 'a',
					'b',
					'c', 'C',
					'd',
					'e', 'E',
					'f', 'F',
					'g', 'G',
					'h', 'H',
					'i',
					'l', 'L',
					'n', 'N',
					'o',
					's', 'S',
					't',
					'u',
					'x', 'X');
}

inline Sci_Position CheckFormatSpecifier(const StyleContext &sc, LexAccessor &styler, bool insideUrl) noexcept {
	if (AnyOf(sc.chNext, '!', '%', '@', ',')) {
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
	char ch = styler[pos];
	// flags
	while (AnyOf(ch, '-', '+', ' ', '0', '#')) {
		ch = styler[++pos];
	}
	// [width]
	if (ch == '*') {
		ch = styler[++pos];
	} else {
		while (IsADigit(ch)) {
			ch = styler[++pos];
		}
	}
	// [.precision]
	if (ch == '.') {
		ch = styler[++pos];
		if (ch == '*') {
			ch = styler[++pos];
		} else {
			while (IsADigit(ch)) {
				ch = styler[++pos];
			}
		}
	}
	// [type]
	if (IsFormatSpecifier(ch)) {
		++pos;
		if (AnyOf(ch, 'l', 'L', 'n')) {
			ch = styler[pos];
			if (AnyOf(ch, 'd', 'i', 'u', 'x', 'X', 'o')) {
				++pos;
			}
		}
		return pos - sc.currentPos;
	}
	return 0;
}

//KeywordIndex++Autogenerated -- start of section automatically generated
enum {
	KeywordIndex_Keyword = 0,
	KeywordIndex_Type = 1,
	MaxKeywordSize = 16,
};
//KeywordIndex--Autogenerated -- end of section automatically generated

void ColouriseOCamlDoc(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, LexerWordList keywordLists, Accessor &styler) {
	int commentLevel = 0;
	int visibleChars = 0;
	int indentCount = 0;
	int lineState = 0;
	Sci_PositionU backPos = 0;
	bool insideUrl = false;
	bool lineContinuation = false;
	EscapeSequence escSeq;
	std::string quotedStringId;

	if (startPos != 0) {
		// backtrack to restore quoted-string-id
		BacktrackToStart(styler, PyLineStateStringInterpolation, startPos, lengthDoc, initStyle);
	}

	StyleContext sc(startPos, lengthDoc, initStyle, styler);
	if (sc.currentLine > 0) {
		lineState = styler.GetLineState(sc.currentLine - 1);
		commentLevel = (lineState >> 8) & 0xff;
		lineContinuation = (lineState & PyLineStateLineContinuation) != 0;
		lineState = 0;
	}

	while (sc.More()) {
		switch (sc.state) {
		case SCE_OCAML_OPERATOR:
			sc.SetState(SCE_OCAML_DEFAULT);
			break;

		case SCE_OCAML_NUMBER:
			if (!IsDecimalNumberEx(sc.chPrev, sc.ch, sc.chNext)) {
				sc.SetState(SCE_OCAML_DEFAULT);
			}
			break;

		case SCE_OCAML_IDENTIFIER:
		case SCE_OCAML_TAGNAME:
		case SCE_OCAML_ATTRIBUTE:
			if (!IsOCamlIdentifierChar(sc.ch)) {
				if (sc.state == SCE_OCAML_IDENTIFIER) {
					char s[MaxKeywordSize];
					sc.GetCurrent(s, sizeof(s));
					if (keywordLists[KeywordIndex_Keyword].InList(s)) {
						sc.ChangeState(SCE_OCAML_KEYWORD);
						if ((visibleChars == 3 && StrEqual(s, "end")) || (visibleChars == 4 && StrEqual(s, "done"))) {
							lineState |= PyLineStateMaskCloseBrace;
						}
					} else if (keywordLists[KeywordIndex_Type].InList(s)) {
						sc.ChangeState(SCE_OCAML_TYPE);
					}
				}
				sc.SetState(SCE_OCAML_DEFAULT);
			}
			break;

		case SCE_OCAML_CHARACTER:
		case SCE_OCAML_STRING:
		case SCE_OCAML_QUOTED_STRING:
			if (sc.atLineStart && sc.state != SCE_OCAML_QUOTED_STRING) {
				if (lineContinuation) {
					lineContinuation = false;
				} else {
					sc.SetState(SCE_OCAML_DEFAULT);
					break;
				}
			}
			if (sc.ch == '\\') {
				if (sc.state != SCE_OCAML_QUOTED_STRING) {
					if (IsEOLChar(sc.chNext)) {
						lineContinuation = true;
						lineState |= PyLineStateLineContinuation;
					} else {
						escSeq.resetEscapeState(sc.state, sc.chNext);
						sc.SetState(SCE_OCAML_ESCAPECHAR);
						sc.Forward();
						if (sc.Match('u', '{')) {
							escSeq.brace = true;
							sc.Forward();
						}
					}
				}
			} else if ((sc.ch == '\'' && sc.state == SCE_OCAML_CHARACTER) || (sc.ch == '\"' && sc.state == SCE_OCAML_STRING)) {
				sc.ForwardSetState(SCE_OCAML_DEFAULT);
			} else if (sc.ch == '|' && sc.state == SCE_OCAML_QUOTED_STRING) {
				if (quotedStringId.empty()) {
					if (sc.chNext == '}') {
						sc.Forward();
						sc.ForwardSetState(SCE_OCAML_DEFAULT);
					}
				} else if (styler.Match(sc.currentPos + 1, quotedStringId.c_str())) {
					sc.Advance(quotedStringId.length());
					sc.ForwardSetState(SCE_OCAML_DEFAULT);
				}
			} else if (sc.state != SCE_OCAML_CHARACTER) {
				if (sc.Match(':', '/', '/') && IsLowerCase(sc.chPrev)) {
					insideUrl = true;
				} else if (insideUrl && IsInvalidUrlChar(sc.ch)) {
					insideUrl = false;
				} else if (sc.ch == '%') {
					const Sci_Position length = CheckFormatSpecifier(sc, styler, insideUrl);
					if (length != 0) {
						const int state = sc.state;
						sc.SetState(SCE_OCAML_FORMAT_SPECIFIER);
						sc.Advance(length);
						sc.SetState(state);
						continue;
					}
				}
			}
			break;

		case SCE_OCAML_ESCAPECHAR:
			if (escSeq.atEscapeEnd(sc.ch)) {
				if (escSeq.brace && sc.ch == '}') {
					sc.Forward();
				}
				sc.SetState(escSeq.outerState);
				continue;
			}
			break;

		case SCE_OCAML_QUOTED_STRINGID:
			if (!IsQuotedStringId(sc.ch)) {
				if (sc.ch == '|') {
					quotedStringId = styler.GetRange(backPos + 1, sc.currentPos);
					quotedStringId.push_back('}');
					sc.ChangeState(SCE_OCAML_QUOTED_STRING);
				} else {
					(void)sc.BackTo(backPos);
					sc.SetState(SCE_OCAML_OPERATOR);
				}
			}
			break;

		case SCE_OCAML_COMMENT:
			if (sc.atLineStart) {
				lineState = PyLineStateMaskCommentLine;
			}
			if (sc.Match('(', '*')) {
				commentLevel++;
				sc.Forward();
			} else if (sc.Match('*', ')')) {
				sc.Forward();
				commentLevel--;
				if (commentLevel == 0) {
					sc.ForwardSetState(SCE_OCAML_DEFAULT);
					if (lineState == PyLineStateMaskCommentLine && sc.GetLineNextChar() != '\0') {
						lineState = 0;
					}
				}
			}
			break;
		}

		if (sc.state == SCE_OCAML_DEFAULT) {
			if (sc.Match('(', '*')) {
				if (visibleChars == 0) {
					lineState = PyLineStateMaskCommentLine;
				}
				commentLevel = 1;
				sc.SetState(SCE_OCAML_COMMENT);
				sc.Forward();
			} else if (sc.ch == '\"') {
				insideUrl = false;
				sc.SetState(SCE_OCAML_STRING);
			} else if (sc.ch == '\'') {
				int state = SCE_OCAML_CHARACTER;
				if (IsEOLChar(sc.chNext)) {
					state = SCE_OCAML_OPERATOR;
				} else if (sc.chNext != '\\') {
					const int after = sc.GetCharAfterNext();
					if (after != '\'') {
						state = IsIdentifierStart(sc.chNext) ? SCE_OCAML_IDENTIFIER : SCE_OCAML_OPERATOR;
					}
				}
				sc.SetState(state);
			} else if (sc.Match('{', '|')) {
				insideUrl = false;
				quotedStringId.clear();
				sc.SetState(SCE_OCAML_QUOTED_STRING);
				sc.Forward();
			} else if (sc.ch == '{' && IsQuotedStringId(sc.chNext)) {
				insideUrl = false;
				backPos = sc.currentPos;
				sc.SetState(SCE_OCAML_QUOTED_STRING);
				sc.ForwardSetState(SCE_OCAML_QUOTED_STRINGID);
			} else if (IsADigit(sc.ch) || (sc.ch == '#' && IsADigit(sc.chNext))) {
				sc.SetState(SCE_OCAML_NUMBER);
			} else if (sc.ch == '`' && IsUpperCase(sc.chNext)) {
				sc.SetState(SCE_OCAML_TAGNAME);
			} else if ((sc.ch == '@' || sc.ch == '%') && sc.chPrev == '[' && (sc.chNext == sc.ch || IsIdentifierStart(sc.chNext))) {
				sc.SetState(SCE_OCAML_ATTRIBUTE);
				sc.Forward();
				if (sc.ch == '@') {
					sc.Forward(); // floating-attribute
				}
			} else if (IsIdentifierStart(sc.ch)) {
				sc.SetState(SCE_OCAML_IDENTIFIER);
			} else if (IsAGraphic(sc.ch)) {
				sc.SetState(SCE_OCAML_OPERATOR);
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
			}
		}
		if (!isspacechar(sc.ch)) {
			visibleChars++;
		}
		if (sc.atLineEnd) {
			if (sc.state == SCE_OCAML_QUOTED_STRING) {
				lineState = PyLineStateMaskTripleQuote;
				if (!quotedStringId.empty()) {
					lineState |= PyLineStateStringInterpolation;
				}
			} else if (sc.state == SCE_OCAML_STRING) {
				lineState |= PyLineStateMaskTripleQuote;
			} else if (lineState == 0 && visibleChars == 0) {
				lineState = PyLineStateMaskEmptyLine;
			}
			lineState |= (indentCount << 16) | (commentLevel << 8);
			styler.SetLineState(sc.currentLine, lineState);
			lineState = 0;
			visibleChars = 0;
			indentCount = 0;
			insideUrl = false;
		}
		sc.Forward();
	}

	sc.Complete();
}

}

extern const LexerModule lmOCaml(SCLEX_OCAML, ColouriseOCamlDoc, "ocaml", FoldPyDoc);
