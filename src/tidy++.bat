@clang-tidy %1 -- -m64 -x c++ -std=c++2a -ferror-limit=1000 -D_WIN64 -DNDEBUG -DUNICODE -D_UNICODE -D_WIN32_WINNT=0x0502 -DWINVER=0x0502 -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -DSCI_LEXER -DNO_CXX11_REGEX -I../scintilla/include -Wall -Wextra -Wshadow -Wimplicit-fallthrough -Wformat=2 -Wcomma -Wrange-loop-analysis 1>tidy.log
