@clang-tidy %1 -- -m64 -std=c17 -D_WIN64 -DNOMINMAX -DNDEBUG -DSTRICT_TYPED_ITEMIDS -DUNICODE -D_UNICODE -D_WIN32_WINNT=0x0600 -DWINVER=0x0600 -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -I../scintilla/include -Wall -Wextra -Wshadow -Wimplicit-fallthrough -Wformat=2 -Wundef -Wcomma 1>tidy.log
