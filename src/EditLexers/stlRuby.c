#include "EditLexer.h"
#include "EditStyleX.h"

static KEYWORDLIST Keywords_Ruby = {{
//++Autogenerated -- start of section automatically generated
"ARGF ARGV BEGIN DATA END ENV FALSE NIL RUBY_PLATFORM RUBY_RELEASE_DATE RUBY_VERSION STDERR STDIN STDOUT TRUE "
"__ENCODING__ __FILE__ __LINE__ alias and begin break case class def defined? do else elsif end ensure false for "
"if in include module next nil not or private protected public raise redo refine require rescue retry return self super "
"then true undef unless until when while yield "

, // 1 code fold
"begin case class def do for if module unless until while "

, // 2 re
"and begin break case do else elsif if next not or return unless until when "

, // 3 pre-defined variables
"$DEBUG $FILENAME $LOADED_FEATURES $LOAD_PATH $VERBOSE $stderr $stdin $stdout "

, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
//--Autogenerated -- end of section automatically generated
}};

static EDITSTYLE Styles_Ruby[] = {
	EDITSTYLE_DEFAULT,
	{ MULTI_STYLE(SCE_RB_WORD, SCE_RB_WORD_DEMOTED, 0, 0), NP2StyleX_Keyword, L"bold; fore:#FF8000" },
	{ SCE_RB_MODULE_NAME, NP2StyleX_Module, L"bold; fore:#007F7F" },
	{ SCE_RB_CLASSNAME, NP2StyleX_Class, L"bold; fore:#007F7F" },
	{ SCE_RB_DEFNAME, NP2StyleX_FunctionDefine, L"fore:#0080C0" },
	{ SCE_RB_FUNCTION, NP2StyleX_Function, L"fore:#A46000" },
	{ SCE_RB_NUMBER, NP2StyleX_Number, L"fore:#FF0000" },
	{ SCE_RB_OPERATOR, NP2StyleX_Operator, L"fore:#B000B0" },
	{ SCE_RB_COMMENTLINE, NP2StyleX_Comment, L"fore:#608060" },
	{ MULTI_STYLE(SCE_RB_STRING, SCE_RB_CHARACTER, 0, 0), NP2StyleX_String, L"fore:#008000" },
	{ SCE_RB_POD, NP2StyleX_POD, L"fore:#004000; back:#C0FFC0; eolfilled" },
	{ SCE_RB_BACKTICKS, NP2StyleX_Backticks, L"fore:#FF0080" },
	{ SCE_RB_REGEX, NP2StyleX_Regex, L"back:#A0FFA0" },
	{ SCE_RB_SYMBOL, NP2StyleX_Symbol, L"bold; fore:#FF4F0F" },
	{ MULTI_STYLE(SCE_RB_CLASS_VAR, SCE_RB_INSTANCE_VAR, SCE_RB_GLOBAL, 0), NP2StyleX_Variable, L"fore:#003CE6" },
	{ SCE_RB_HERE_DELIM, NP2StyleX_HeredocDelimiter, L"fore:#648000" },
	{ SCE_RB_HERE_Q, NP2StyleX_HeredocSingleQuoted, L"fore:#648000" },
	{ SCE_RB_HERE_QQ, NP2StyleX_HeredocDoubleQuoted,L"fore:#648000" },
	{ SCE_RB_HERE_QX, NP2StyleX_HeredocBackticks, L"fore:#E24000; back:#FFF1A8" },
	{ SCE_RB_STRING_Q, NP2StyleX_SingleQuotedString_q, L"fore:#008000" },
	{ SCE_RB_STRING_QQ, NP2StyleX_DoubleQuotedString_qq, L"fore:#008000" },
	{ SCE_RB_STRING_QX, NP2StyleX_Backticks_qx, L"fore:#E24000; back:#FFF1A8" },
	{ SCE_RB_STRING_QR, NP2StyleX_Regex_qr, L"fore:#006633; back:#FFF1A8" },
	{ SCE_RB_STRING_QW, NP2StyleX_Array_qw, L"fore:#003CE6" },
	{ SCE_RB_DATASECTION, NP2StyleX_DataSection, L"fore:#600000; back:#FFF0D8; eolfilled" },
};

EDITLEXER lexRuby = {
	SCLEX_RUBY, NP2LEX_RUBY,
	EDITLEXER_HOLE(L"Ruby Script", Styles_Ruby),
	L"rb; ruby; rbw; rake; rjs; gemspec; podspec; cr",
	&Keywords_Ruby,
	Styles_Ruby
};
