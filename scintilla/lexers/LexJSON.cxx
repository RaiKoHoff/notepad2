// This file is part of Notepad2.
// See License.txt for details about distribution and modification.
//! Lexer for JSON, JSON5.

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
#include "CharacterSet.h"
#include "LexerModule.h"

using namespace Lexilla;

namespace {

enum {
	JsonChar_None = 0,
	JsonChar_BraceOpen = 1,
	JsonChar_BraceClose = 2,
	JsonChar_WordStart = 3,
	JsonChar_Dot = 4,
	JsonChar_Slash = 5,
	JsonChar_Ignore = 6,

	JsonMask_Number = 1 << 3,
	JsonMask_Identifier = 1 << 4,
};

void ColouriseJSONDoc(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, LexerWordList keywordLists, Accessor &styler) {
	const bool fold = styler.GetPropertyBool("fold");

	int state = initStyle;
	uint8_t chNext = styler[startPos];
	styler.StartAt(startPos);
	styler.StartSegment(startPos);
	const Sci_PositionU endPos = startPos + lengthDoc;

	Sci_Line lineCurrent = styler.GetLine(startPos);
	int levelCurrent = SC_FOLDLEVELBASE;
	if (lineCurrent > 0) {
		levelCurrent = styler.LevelAt(lineCurrent - 1) >> 16;
	}
	int levelNext = levelCurrent;

	constexpr int MaxLexWordLength = 8; // Infinity
	char buf[MaxLexWordLength + 1];
	int wordLen = 0;

	// JSON5 line continuation
	bool lineContinuation = false;
	bool atLineStart = startPos == static_cast<Sci_PositionU>(styler.LineStart(lineCurrent));
	Sci_PositionU lineStartNext = styler.LineStart(lineCurrent + 1);
	Sci_PositionU lineEndPos = sci::min(lineStartNext, endPos) - 1;

	// scripts/GenerateCharTable.py
	static constexpr uint8_t kJsonCharClass[256] = {
//++Autogenerated -- start of section automatically generated
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 166, 0, 150, 0, 0, 198, 0, 0, 0, 46, 38, 46, 44, 5,
94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 38, 0, 0, 0, 0, 0,
0, 158, 158, 158, 158, 158, 158, 158, 158, 123, 158, 158, 158, 158, 123, 158,
158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 158, 33, 150, 34, 0, 158,
0, 158, 158, 158, 158, 158, 123, 158, 158, 158, 158, 158, 158, 158, 123, 158,
158, 158, 158, 158, 123, 158, 158, 158, 158, 158, 158, 33, 0, 34, 0, 0,
150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150,
//--Autogenerated -- end of section automatically generated
	};

	for (Sci_PositionU i = startPos; i < endPos; i++) {
		const uint8_t ch = chNext;
		chNext = styler.SafeGetCharAt(i + 1);

		switch (state) {
		case SCE_JSON_OPERATOR:
			styler.ColorTo(i, state);
			state = SCE_JSON_DEFAULT;
			break;

		case SCE_JSON_NUMBER:
			if (!(kJsonCharClass[ch] & JsonMask_Number)) {
				styler.ColorTo(i, state);
				state = SCE_JSON_DEFAULT;
			}
			break;

		case SCE_JSON_MAYBE_KEYWORD:
			if (!(kJsonCharClass[ch] & JsonMask_Identifier)) {
				buf[wordLen] = '\0';
				if (keywordLists[0]->InList(buf)) {
					styler.ColorTo(i, SCE_JSON_KEYWORD);
				} else if (ch == ':' || chNext == ':' || LexGetNextChar(styler, i + 1) == ':') {
					styler.ColorTo(i, SCE_JSON_PROPERTYNAME);
				}
				state = SCE_JSON_DEFAULT;
			} else if (wordLen < MaxLexWordLength) {
				buf[wordLen++] = static_cast<char>(ch);
			} else {
				state = SCE_JSON_IDENTIFIER;
			}
			break;

		case SCE_JSON_IDENTIFIER:
			if (!(kJsonCharClass[ch] & JsonMask_Identifier)) {
				if (ch == ':' || chNext == ':' || LexGetNextChar(styler, i + 1) == ':') {
					styler.ColorTo(i, SCE_JSON_PROPERTYNAME);
				}
				state = SCE_JSON_DEFAULT;
			}
			break;

		case SCE_JSON_STRING_DQ:
		case SCE_JSON_STRING_SQ:
			if (i == lineEndPos) { // atLineEnd
				if (lineContinuation) {
					lineContinuation = false;
				} else {
					styler.ColorTo(i, state);
					state = SCE_JSON_DEFAULT;
				}
			} else if (ch == '\\') {
				styler.ColorTo(i, state);
				if (IsEOLChar(chNext)) {
					styler.ColorTo(i + 1, SCE_JSON_DEFAULT);
					lineContinuation = true;
				} else {
					// highlight any character as escape sequence
					++i;
					if (chNext == 'u' || chNext == 'x') {
						int count = (chNext == 'u') ? 4 : 2;
						do {
							chNext = styler.SafeGetCharAt(i + 1);
							if (!IsHexDigit(chNext)) {
								break;
							}
							--count;
							++i;
						} while (count);
					}

					chNext = styler.SafeGetCharAt(i + 1);
					styler.ColorTo(i + 1, SCE_JSON_ESCAPECHAR);
					continue;
				}
			} else if ((state == SCE_JSON_STRING_DQ && ch == '\"') || (state == SCE_JSON_STRING_SQ && ch == '\'')) {
				if (chNext == ':' || LexGetNextChar(styler, i + 1) == ':') {
					styler.ColorTo(i + 1, SCE_JSON_PROPERTYNAME);
				} else {
					styler.ColorTo(i + 1, state);
				}
				state = SCE_JSON_DEFAULT;
				continue;
			}
			break;

		case SCE_JSON_LINECOMMENT:
			if (atLineStart) {
				styler.ColorTo(i, state);
				state = SCE_JSON_DEFAULT;
			}
			break;

		case SCE_JSON_BLOCKCOMMENT:
			if (ch == '*' && chNext == '/') {
				i++;
				chNext = styler.SafeGetCharAt(i + 1);
				styler.ColorTo(i + 1, state);
				state = SCE_JSON_DEFAULT;
				levelNext--;
				continue;
			}
			break;
		}

		if (state == SCE_JSON_DEFAULT) {
			const int nextState = kJsonCharClass[ch];
			const int charClass = nextState & 7;
			if (charClass) {
				styler.ColorTo(i, state);
				state = nextState >> 5;
				switch (charClass) {
				case JsonChar_BraceOpen:
					levelNext++;
					break;
				case JsonChar_BraceClose:
					levelNext--;
					break;
				case JsonChar_WordStart:
					buf[0] = static_cast<char>(ch);
					wordLen = 1;
					break;
				case JsonChar_Dot:
					if (IsADigit(chNext)) {
						state = SCE_JSON_NUMBER;
					}
					break;
				case JsonChar_Slash:
					if (chNext == '/') {
						state = SCE_JSON_LINECOMMENT;
					} else if (chNext == '*') {
						state = SCE_JSON_BLOCKCOMMENT;
						levelNext++;
						i++;
						chNext = styler.SafeGetCharAt(i + 1);
					}
					break;
				default:
					break;
				}
			}
		}

		if (styler.IsLeadByte(ch)) {
			// ignore trail byte in DBCS character
			i++;
			chNext = styler.SafeGetCharAt(i + 1);
		}

		atLineStart = i == lineEndPos;
		if (atLineStart) {
			if (fold) {
				const int levelUse = levelCurrent;
				int lev = levelUse | levelNext << 16;
				if (levelUse < levelNext) {
					lev |= SC_FOLDLEVELHEADERFLAG;
				}
				if (lev != styler.LevelAt(lineCurrent)) {
					styler.SetLevel(lineCurrent, lev);
				}
				levelCurrent = levelNext;
			}
			lineCurrent++;
			lineStartNext = styler.LineStart(lineCurrent + 1);
			lineEndPos = sci::min(lineStartNext, endPos) - 1;
		}
	}

	// Colourise remaining document
	styler.ColorTo(endPos, state);
}

}

LexerModule lmJSON(SCLEX_JSON, ColouriseJSONDoc, "json");
