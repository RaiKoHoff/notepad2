#include "EditLexer.h"
#include "EditStyleX.h"

static KEYWORDLIST Keywords_TS = {{
//++Autogenerated -- start of section automatically generated
"abstract accessor as assert asserts async await break case catch class const constructor continue "
"debugger declare default delete do else enum extends false finally for function get global "
"if implements in infer instanceof interface intrinsic is keyof let new null of out override "
"package private protected public readonly return satisfies set static super switch this throw true try type typeof "
"undefined unique using var while with yield "

, // 1 types
"any bigint boolean never number object string symbol unknown void "

, // 2 directive
"export from import module namespace require "

, // 3 class
NULL

, // 4 interface
"ActiveXObject AggregateError Array ArrayBuffer AsyncFunction AsyncGeneratorFunction Atomics "
"BigInt BigInt64Array BigUint64Array Boolean DataView Date Debug Document Enumerator Error EvalError EventTarget "
"FinalizationRegistry Float32Array Float64Array FormData Function GeneratorFunction Int16Array Int32Array Int8Array JSON "
"Map Math Number Object Promise Proxy RangeError ReferenceError Reflect RegExp "
"Set SharedArrayBuffer String Symbol SyntaxError TypeError URIError Uint16Array Uint32Array Uint8Array Uint8ClampedArray "
"VBArray WScript WeakMap WeakRef WeakSet Worker XMLHttpRequest "

, // 5 enumeration
NULL

, // 6 constant
"BYTES_PER_ELEMENT DONE E EPSILON HEADERS_RECEIVED LN10 LN2 LOADING LOG10E LOG2E "
"MAX_SAFE_INTEGER MAX_VALUE MIN_SAFE_INTEGER MIN_VALUE NEGATIVE_INFINITY OPENED PI POSITIVE_INFINITY SQRT1_2 SQRT2 "
"UNSENT "

, // 7 decorator
NULL

, // 8 function
NULL

, // 9 properties
NULL

, // 10 TSDoc
"alpha amd-dependency amd-module beta decorator defaultValue deprecated eventProperty example experimental "
"inheritDoc internal label link override packageDocumentation param privateRemarks public "
"readonly reference remarks returns sealed see throws typeParam virtual "

, NULL, NULL, NULL, NULL
//--Autogenerated -- end of section automatically generated

, // 15 Code Snippet
"for^() if^() switch^() while^() else^if^() else^{} "
}};

static EDITSTYLE Styles_TS[] = {
	EDITSTYLE_DEFAULT,
	{ SCE_JS_WORD, NP2StyleX_Keyword, L"fore:#0000FF" },
	{ SCE_JS_WORD2, NP2StyleX_TypeKeyword, L"fore:#0000FF" },
	{ SCE_JS_DIRECTIVE, NP2StyleX_Directive, L"fore:#FF8000" },
	{ SCE_JS_CLASS, NP2StyleX_Class, L"bold; fore:#0080C0" },
	{ SCE_JS_INTERFACE, NP2StyleX_Interface, L"bold; fore:#1E90FF" },
	{ SCE_JS_ENUM, NP2StyleX_Enumeration, L"fore:#FF8000" },
	{ SCE_JS_DECORATOR, NP2StyleX_Decorator, L"fore:#FF8000" },
	{ SCE_JS_FUNCTION_DEFINITION, NP2StyleX_FunctionDefinition, L"bold; fore:#A46000" },
	{ SCE_JS_FUNCTION, NP2StyleX_Function, L"fore:#A46000" },
	{ SCE_JS_CONSTANT, NP2StyleX_Constant, L"fore:#B000B0" },
	{ MULTI_STYLE(SCE_JS_COMMENTLINE, SCE_JS_COMMENTBLOCK, 0, 0), NP2StyleX_Comment, L"fore:#608060" },
	{ MULTI_STYLE(SCE_JS_COMMENTLINEDOC, SCE_JS_COMMENTBLOCKDOC, 0, 0), NP2StyleX_DocComment, L"fore:#408040" },
	{ MULTI_STYLE(SCE_JS_COMMENTTAGAT, SCE_JS_COMMENTTAGXML, 0, 0), NP2StyleX_DocCommentTag, L"fore:#408080" },
	{ SCE_JS_TASKMARKER, NP2StyleX_TaskMarker, L"bold; fore:#408080" },
	{ MULTI_STYLE(SCE_JS_STRING_SQ, SCE_JS_STRING_DQ, SCE_JSX_STRING_SQ, SCE_JSX_STRING_DQ), NP2StyleX_String, L"fore:#008000" },
	{ SCE_JS_STRING_BT, NP2StyleX_TemplateLiteral, L"fore:#F08000" },
	{ SCE_JS_ESCAPECHAR, NP2StyleX_EscapeSequence, L"fore:#0080C0" },
	{ SCE_JS_KEY, NP2StyleX_Property, L"fore:#648000" },
	{ SCE_JS_REGEX, NP2StyleX_Regex, L"fore:#006633; back:#FFF1A8" },
	{ SCE_JSX_TAG, NP2StyleX_XMLTag, L"fore:#648000" },
	{ MULTI_STYLE(SCE_JSX_ATTRIBUTE, SCE_JSX_ATTRIBUTE_AT, 0, 0), NP2StyleX_XMLAttribute, L"fore:#FF4000" },
	{ SCE_JS_LABEL, NP2StyleX_Label, L"back:#FFC040" },
	{ SCE_JS_NUMBER, NP2StyleX_Number, L"fore:#FF0000" },
	{ MULTI_STYLE(SCE_JS_OPERATOR, SCE_JS_OPERATOR2, SCE_JS_OPERATOR_PF, 0), NP2StyleX_Operator, L"fore:#B000B0" },
};

EDITLEXER lexTypeScript = {
	SCLEX_JAVASCRIPT, NP2LEX_TYPESCRIPT,
//Settings++Autogenerated -- start of section automatically generated
		LexerAttr_AngleBracketGeneric,
		TAB_WIDTH_4, INDENT_WIDTH_4,
		(1 << 0) | (1 << 1) | (1 << 2), // class, anonymous object, method
		SCE_JS_FUNCTION_DEFINITION,
		'\\', SCE_JS_ESCAPECHAR, 0,
		0,
		0, 0,
		SCE_JS_OPERATOR, SCE_JS_OPERATOR2
		, KeywordAttr32(0, KeywordAttr_PreSorted) // keywords
		| KeywordAttr32(1, KeywordAttr_PreSorted) // types
		| KeywordAttr32(2, KeywordAttr_PreSorted) // directive
		| KeywordAttr32(4, KeywordAttr_PreSorted) // interface
		| KeywordAttr32(6, KeywordAttr_PreSorted) // constant
		| KeywordAttr64(8, KeywordAttr_NoLexer) // function
		| KeywordAttr64(9, KeywordAttr_NoLexer) // properties
		| KeywordAttr64(10, KeywordAttr_NoLexer | KeywordAttr_NoAutoComp) // TSDoc
		, SCE_JS_TASKMARKER,
		SCE_JSX_TEXT, SCE_JS_ESCAPECHAR,
//Settings--Autogenerated -- end of section automatically generated
	EDITLEXER_HOLE(L"TypeScript", Styles_TS),
	L"ts; tsx; cts; mts",
	&Keywords_TS,
	Styles_TS
};
