// This file is part of Notepad2.
// See License.txt for details about distribution and modification.
#pragma once

//! Enable customize toolbar labels
// When enabled, user can change toolbar button tooltip text
// in the "Toolbar Labels" section int Notepad2.ini
#define NP2_ENABLE_CUSTOMIZE_TOOLBAR_LABELS		0

//! Enable the .LOG feature
// When enabled and Notepad2 opens a file starts with ".LOG" (in upper case),
// Notepad2 will append current time to the file.
// This is a hiden feature in Windows Notepad.
#define NP2_ENABLE_DOT_LOG_FEATURE				0

//! Enable localization with satellite resource DLLs.
#define NP2_ENABLE_APP_LOCALIZATION_SATELLITE	0
//! Enable localization with Multilingual User Interface (Vista and later).
#define NP2_ENABLE_APP_LOCALIZATION_MUI			0

//! Enable localization for scheme/lexer names.
#define NP2_ENABLE_LEXER_NAME_LOCALIZATION		0
//! Enable localization for scheme/lexer style names.
#define NP2_ENABLE_STYLE_NAME_LOCALIZATION		0
