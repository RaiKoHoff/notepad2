#include "EditLexer.h"
#include "EditStyleX.h"

static KEYWORDLIST Keywords_Awk = {{
//++Autogenerated -- start of section automatically generated
"@include @load @namespace BEGIN BEGINFILE END ENDFILE break case continue default delete do else exit for function "
"getline if in next nextfile print printf return switch while "

, // 1 predefined variable
"ARGC ARGIND ARGV BINMODE CONVFMT ENVIRON ERRNO FIELDWIDTHS FILENAME FNR FPAT FS FUNCTAB IGNORECASE LINT NF NR "
"OFMT OFS ORS PREC PROCINFO RLENGTH ROUNDMODE RS RSTART RT SUBSEP SYMTAB TEXTDOMAIN "

, // 2 built-in function
"and( asort( asorti( atan2( bindtextdomain( close( compl( cos( dcgettext( dcngettext( exp( fflush( gensub( gsub( "
"index( int( isarray( length( log( lshift( match( mktime( or( patsplit( rand( rshift( "
"sin( split( sprintf( sqrt( srand( strftime( strtonum( sub( substr( system( systime( tolower( toupper( typeof( xor( "

, // 3 library function
"adequate_math_precision( assert( beginfile( bits2str( chr( cliff_rand( ctime( disable_assigns( "
"endfile( endgrent( endpwent( "
"getgrent( getgrgid( getgrnam( getgruser( getlocaltime( getopt( getpwent( getpwnam( getpwuid( intdiv0( join( ord( "
"process_array( quicksort( quicksort_swap( readfile( rewind( round( shell_quote( walk_array( zerofile( "

, // 4 misc
"AWKBUFSIZE AWKLIBPATH AWKPATH AWKREADFUNC AWK_HASH "
"GAWK_LOCALE_DIR GAWK_MSEC_SLEEP GAWK_MSG_SRC GAWK_NO_DFA GAWK_READ_TIMEOUT GAWK_SOCK_RETRIES GAWK_STACKSIZE "
"INT_CHAIN_MAX NONFATAL POSIXLY_CORRECT READ_TIMEOUT RETRY STR_CHAIN_MAX TIDYMEM api_major api_minor argv array awk "
"builtin egid errno euid extension gawk gid gmp_version identifiers mpfr_version number "
"pgrpid pid platform ppid prec_max prec_min pty regexp scalar sorted_in strftime string strnum "
"uid unassigned untyped user version "

, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
//--Autogenerated -- end of section automatically generated

, // 15 Code Snippet
"for^() if^() switch^() while^() else^if^() else^{} "
}};

static EDITSTYLE Styles_Awk[] = {
	EDITSTYLE_DEFAULT,
	{ SCE_AWK_WORD, NP2StyleX_Keyword, L"fore:#0000FF" },
	{ SCE_AWK_FUNCTION_DEFINITION, NP2StyleX_FunctionDefinition, L"bold; fore:#A46000" },
	{ SCE_AWK_FUNCTION, NP2StyleX_Function, L"fore:#A46000" },
	{ SCE_AWK_BUILTIN_FUNCTION, NP2StyleX_BuiltInFunction, L"fore:#0080C0" },
	{ SCE_AWK_BUILTIN_VARIABLE, NP2StyleX_PredefinedVariable, L"fore:#B000B0" },
	{ SCE_AWK_COMMENTLINE, NP2StyleX_Comment, L"fore:#608060" },
	{ SCE_AWK_STRING, NP2StyleX_String, L"fore:#008000" },
	{ SCE_AWK_ESCAPECHAR, NP2StyleX_EscapeSequence, L"fore:#0080C0" },
	{ SCE_AWK_FORMAT_SPECIFIER, NP2StyleX_FormatSpecifier, L"fore:#7C5AF3" },
	{ SCE_AWK_REGEX, NP2StyleX_Regex, L"fore:#006633; back:#FFF1A8" },
	{ SCE_AWK_NUMBER, NP2StyleX_Number, L"fore:#FF0000" },
	{ SCE_AWK_VARIABLE, NP2StyleX_Variable, L"fore:#CC3300" },
	{ MULTI_STYLE(SCE_AWK_OPERATOR, SCE_AWK_OPERATOR_PF, 0, 0), NP2StyleX_Operator, L"fore:#B000B0" },
};

EDITLEXER lexAwk = {
	SCLEX_AWK, NP2LEX_AWK,
//Settings++Autogenerated -- start of section automatically generated
	{
		LexerAttr_NoBlockComment |
		LexerAttr_PrintfFormatSpecifier,
		TAB_WIDTH_4, INDENT_WIDTH_4,
		(1 << 0) | (1 << 1), // namespace, function
		SCE_AWK_FUNCTION_DEFINITION,
		'\\', SCE_AWK_ESCAPECHAR, SCE_AWK_FORMAT_SPECIFIER,
		0,
		0, 0,
		SCE_AWK_OPERATOR, 0
		, KeywordAttr32(0, KeywordAttr_PreSorted) // keywords
		| KeywordAttr32(1, KeywordAttr_PreSorted) // predefined variable
		| KeywordAttr32(2, KeywordAttr_PreSorted) // built-in function
		| KeywordAttr32(3, KeywordAttr_NoLexer) // library function
		| KeywordAttr32(4, KeywordAttr_NoLexer) // misc
	},
//Settings--Autogenerated -- end of section automatically generated
	EDITLEXER_HOLE(L"Awk Script", Styles_Awk),
	L"awk",
	&Keywords_Awk,
	Styles_Awk
};
