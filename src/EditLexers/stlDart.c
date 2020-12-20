#include "EditLexer.h"
#include "EditStyleX.h"

static KEYWORDLIST Keywords_Dart = {{
//++Autogenerated -- start of section automatically generated
"abstract as assert async await break case catch class const continue covariant default deferred do "
"else enum export extends extension external factory false final finally for get hide "
"if implements import in interface is late library mixin native new null of on operator part required rethrow return "
"set show static super switch sync this throw true try typedef var while with yield "

, // 1 types
"Function bool double dynamic int num void "

, // 2 class
"BidirectionalIterator BigInt Comparable Comparator Completer "
"DateTime Deprecated Directory DoubleLinkedQueue DoubleLinkedQueueEntry Duration Error Exception Expando "
"File FileStat FileSystemEntity FileSystemEvent Future FutureOr HasNextIterator HashMap HashSet "
"IOException Invocation Iterable IterableBase IterableMixin Iterator "
"LinkedHashMap LinkedHashSet LinkedList LinkedListEntry List ListBase ListMixin ListQueue "
"Map MapBase MapEntry MapMixin MapView Match Null OSError Object Pattern Platform Point Process Queue "
"Random RawSocket RawSocketEvent Rectangle RegExp RegExpMatch RuneIterator Runes "
"ServerSocket Set SetBase SetMixin Sink Socket SocketException SplayTreeMap SplayTreeSet "
"StackTrace Stopwatch Stream String StringBuffer StringSink Symbol SystemHash "
"Timer Type Uri UriData "

, // 3 enum
NULL

, // 4 metadata
"Deprecated( Since( deprecated override patch pragma( "

, // 5 function
"identical( identityHashCode( main( print( "

, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
//--Autogenerated -- end of section automatically generated
}};

static EDITSTYLE Styles_Dart[] = {
	EDITSTYLE_DEFAULT,
	{ SCE_DART_WORD, NP2StyleX_Keyword, L"fore:#0000FF" },
	{ SCE_DART_WORD2, NP2StyleX_TypeKeyword, L"fore:#0000FF" },
	{ SCE_DART_METADATA, NP2StyleX_Metadata, L"fore:#FF8000" },
	{ SCE_DART_CLASS, NP2StyleX_Class, L"fore:#0080FF" },
	{ SCE_DART_ENUM, NP2StyleX_Enumeration, L"fore:#FF8000" },
	{ SCE_DART_FUNCTION, NP2StyleX_Function, L"fore:#A46000" },
	{ MULTI_STYLE(SCE_DART_COMMENTBLOCK, SCE_DART_COMMENTLINE, 0, 0), NP2StyleX_Comment, L"fore:#608060" },
	{ MULTI_STYLE(SCE_DART_COMMENTBLOCKDOC, SCE_DART_COMMENTLINEDOC, 0, 0), NP2StyleX_DocComment, L"fore:#408040" },
	{ MULTI_STYLE(SCE_DART_STRING_SQ, SCE_DART_STRING_DQ, 0, 0), NP2StyleX_String, L"fore:#008000" },
	{ MULTI_STYLE(SCE_DART_TRIPLE_STRING_SQ, SCE_DART_TRIPLE_STRING_DQ, SCE_DART_TRIPLE_STRING_SQSTART, SCE_DART_TRIPLE_STRING_SQEND), NP2StyleX_TripleQuotedString, L"fore:#F08000" },
	{ MULTI_STYLE(SCE_DART_RAWSTRING_SQ, SCE_DART_RAWSTRING_DQ, SCE_DART_TRIPLE_RAWSTRING_SQ, SCE_DART_TRIPLE_RAWSTRING_DQ), NP2StyleX_RawString, L"fore:#F08000" },
	{ SCE_DART_ESCAPECHAR, NP2StyleX_EscapeSequence, L"fore:#0080C0" },
	{ SCE_DART_LABEL, NP2StyleX_Label, L"fore:#7C5AF3" },
	{ SCE_DART_NUMBER, NP2StyleX_Number, L"fore:#FF0000" },
	{ SCE_DART_VARIABLE, NP2StyleX_Variable, L"fore:#9E4D2A" },
	{ MULTI_STYLE(SCE_DART_SYMBOL_OPERATOR, SCE_DART_SYMBOL_IDENTIFIER, 0, 0), NP2StyleX_Symbol, L"fore:#7C5AF3" },
	{ MULTI_STYLE(SCE_DART_OPERATOR, SCE_DART_OPERATOR2, 0, 0), NP2StyleX_Operator, L"fore:#B000B0" },
};

EDITLEXER lexDart = {
	SCLEX_DART, NP2LEX_DART,
	EDITLEXER_HOLE(L"Dart Source", Styles_Dart),
	L"dart",
	&Keywords_Dart,
	Styles_Dart
};
