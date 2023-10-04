#include "EditLexer.h"
#include "EditStyleX.h"

static KEYWORDLIST Keywords_Rebol = {{
//++Autogenerated -- start of section automatically generated
"alias all and any as assert break case catch comment construct context continue declare does either exit "
"false forall foreach forever func function get has if loop namespace no none not null object off on or pop push "
"reduce remove-each repeat return routine set size? switch throw true try unless until use variadic while with xor yes "

, // 1 directive
"call case default define do either enum export get get-definition if import in include inline pop-path "
"register-intrinsics script switch syscall system system-global typecheck u16 user-code verbose "

, // 2 datatype
"action! binary! bitset! block! byte! byte-ptr! c-string! char! closure! command! datatype! date! decimal! "
"email! end! error! event! file! float! float32! float64! frame! function! get-path! get-word! gob! handle! hash! "
"image! int-ptr! integer! issue! library! list! lit-path! lit-word! logic! map! module! money! native! none! object! op! "
"pair! paren! path! percent! point! pointer! port! rebcode! ref! refinement! routine! "
"set-path! set-word! string! struct! subroutine! tag! task! time! tuple! typeset! unset! url! utype! vector! word! "

, // 3 function
"about abs absolute action? add ajoin also alter and~ any-block? any-function? any-path? any-string? any-word? "
"append apply arccosine arcsine arctangent array as-binary as-pair as-string ascii? ask at attempt "
"back binary? bind bind? bitset? block? body-of browse bugs "
"call caret-to-offset cause-error cd change change-dir char? charset chat checksum "
"clean-path clear close closure closure? collect complement compose compress confirm copy cosine create cursor "
"datatype? date? debase decimal? decloak decode decode-url decompress default dehex "
"delect delete deline delta-profile delta-time demo detab difference dir? dirize disarm divide "
"do do-codec do-commands do-events docs dp draw dt dump dump-obj "
"echo effect eighth else email? empty? enbase encloak encode enline entab equal? error? eval even? event? evoke "
"exclude exists? exp extend extract "
"fifth file? find find-module find-script first first+ for form format forskip found? fourth frame? "
"funco funct function? functor "
"get-env get-path? get-word? gob? greater-or-equal? greater? halt handle-events handle? head head? help "
"image? import in in-dir index? info? input insert install integer? intersect issue? join "
"last latin1? launch layout length? lesser-or-equal? lesser? library? license list-dir list-env lit-path? lit-word? "
"load load-gui log-10 log-2 log-e logic? lowercase ls "
"make make-dir make-scheme map map-each map-event map-gob-offset map? max maximum maximum-of min minimum minimum-of "
"mkdir mod modified? module module? modulo mold money? more move multiply "
"native native? negate negative? new-line new-line? next ninth none? not-equal? now number? "
"object? odd? offset-to-caret offset? op? open open? or~ "
"pair? paren? parse past? path? percent? pick poke port? positive? power prin print print-line printf probe protect pwd "
"query quit "
"random read rebcode? recycle refinement? reflect reform rejoin remainder remold remove rename repend replace "
"request-file resolve reverse reword rm round routine? run "
"same? save scalar? script? second secure select series? set-path? set-word? seventh shift show "
"sign? sine sixth size-text skip sort source spec-of split split-path square-root "
"stack stats strict-equal? strict-not-equal? string? struct? subtract suffix? swap "
"tag? tail tail? take tangent task task? tenth third time? title-of to to-action to-binary to-bitset to-block "
"to-char to-closure to-datatype to-date to-decimal to-email to-end to-error to-event to-file to-frame to-function "
"to-get-path to-get-word to-gob to-handle to-hex to-image to-integer to-issue "
"to-library to-lit-path to-lit-word to-local-file to-logic to-map to-module to-money to-native to-none to-object to-op "
"to-pair to-paren to-path to-percent to-port to-rebcode to-rebol-file to-refinement to-relative-file to-routine "
"to-set-path to-set-word to-string to-struct to-tag to-task to-time to-tuple to-typeset to-unset to-url to-utype "
"to-vector to-word trace transcode trim true? tuple? type? types-of typeset? "
"unhandle-events union unique unprotect unset unset? unview update upgrade uppercase url? usage utf? utype? "
"value? values-of vector? view wait wake-up what what-dir why? word? words-of write xor~ zero? "

, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
//--Autogenerated -- end of section automatically generated

, // 15 Code Snippet
"REBOL^[] Red^[] Red/System^[]"
}};

static EDITSTYLE Styles_Rebol[] = {
	EDITSTYLE_DEFAULT,
	{ SCE_REBOL_WORD, NP2StyleX_Keyword, L"bold; fore:#FF8000" },
	{ SCE_REBOL_TYPE, NP2StyleX_Type, L"fore:#0080FF" },
	{ SCE_REBOL_DIRECTIVE, NP2StyleX_Directive, L"fore:#FF8000" },
	{ SCE_REBOL_PROPERTY, NP2StyleX_Property, L"fore:#A46000" },
	{ SCE_REBOL_MACRO, NP2StyleX_Macro, L"fore:#B000B0" },
	{ MULTI_STYLE(SCE_REBOL_COMMENTLINE, SCE_REBOL_COMMENTBLOCK, SCE_REBOL_BINARYCOMMENT, 0), NP2StyleX_Comment, L"fore:#608060" },
	{ MULTI_STYLE(SCE_REBOL_QUOTEDSTRING, SCE_REBOL_CHARACTER, 0, 0), NP2StyleX_String, L"fore:#008000" },
	{ SCE_REBOL_BRACEDSTRING, NP2StyleX_RawString, L"fore:#F08000" },
	{ SCE_REBOL_BINARY, NP2StyleX_Bytes, L"fore:#C08000" },
	{ SCE_REBOL_ESCAPECHAR, NP2StyleX_EscapeSequence, L"fore:#0080C0" },
	{ SCE_REBOL_TAG, NP2StyleX_HTMLTag, L"fore:#648000" },
	{ SCE_REBOL_TAG_ATTR, NP2StyleX_HTMLAttribute, L"fore:#FF4000" },
	{ SCE_REBOL_TAG_VALUE, NP2StyleX_HTMLValue, L"fore:#3A6EA5" },
	{ MULTI_STYLE(SCE_REBOL_FILE, SCE_REBOL_QUOTEDFILE, 0, 0), NP2StyleX_File, L"fore:#648000" },
	{ SCE_REBOL_MONEY, NP2StyleX_Money, L"fore:#7C5AF3" },
	{ SCE_REBOL_ISSUE, NP2StyleX_Issue, L"fore:#9E4D2A" },
	{ MULTI_STYLE(SCE_REBOL_DATE, SCE_REBOL_TIME, 0, 0), NP2StyleX_DateTime, L"fore:#008080" },
	{ SCE_REBOL_PAIR, NP2StyleX_Pair, L"fore:#7C5AF3" },
	{ SCE_REBOL_TUPLE, NP2StyleX_Tuple, L"fore:#7C5AF3" },
	{ SCE_REBOL_SYMBOL, NP2StyleX_Symbol, L"fore:#7C5AF3" },
	{ SCE_REBOL_NUMBER, NP2StyleX_Number, L"fore:#FF0000" },
	{ SCE_REBOL_OPERATOR, NP2StyleX_Operator, L"fore:#B000B0" },
};

EDITLEXER lexRebol = {
	SCLEX_REBOL, NP2LEX_REBOL,
//Settings++Autogenerated -- start of section automatically generated
		LexerAttr_CppPreprocessor,
		TAB_WIDTH_4, INDENT_WIDTH_4,
		(1 << 0) | (1 << 1), // level1, level2
		0,
		'^', SCE_REBOL_ESCAPECHAR, 0,
		0,
		0, SCE_REBOL_SYMBOL,
		SCE_REBOL_OPERATOR, 0
		, KeywordAttr32(0, KeywordAttr_PreSorted) // keywords
		| KeywordAttr32(1, KeywordAttr_PreSorted) // directive
		| KeywordAttr32(2, KeywordAttr_NoLexer) // datatype
		| KeywordAttr32(3, KeywordAttr_NoLexer) // function
		, SCE_REBOL_TASKMARKER,
		SCE_REBOL_BRACEDSTRING, SCE_REBOL_QUOTEDSTRING,
//Settings--Autogenerated -- end of section automatically generated
	EDITLEXER_HOLE(L"Rebol Script", Styles_Rebol),
	L"r; reb; red; reds",
	&Keywords_Rebol,
	Styles_Rebol
};
