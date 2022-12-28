#include "EditLexer.h"
#include "EditStyleX.h"

static KEYWORDLIST Keywords_CMake = {{
//++Autogenerated -- start of section automatically generated
"block( break( continue( else( elseif( endblock( endforeach( endfunction( endif( endmacro( endwhile( foreach( function( "
"if( macro( return( while( "

, // 1 commands
"add_compile_definitions( add_compile_options( add_custom_command( add_custom_target( add_definitions( add_dependencies( "
"add_executable( add_library( add_link_options( add_subdirectory( add_test( aux_source_directory( "
"build_command( "
"cmake_host_system_information( cmake_language( cmake_minimum_required( cmake_parse_arguments( cmake_path( cmake_policy( "
"configure_file( create_test_sourcelist( ctest_build( ctest_configure( ctest_coverage( ctest_empty_binary_directory( "
"ctest_memcheck( ctest_read_custom_files( ctest_run_script( ctest_sleep( ctest_start( ctest_submit( ctest_test( "
"ctest_update( ctest_upload( "
"define_property( enable_language( enable_testing( execute_process( export( "
"file( find_file( find_library( find_package( find_path( find_program( fltk_wrap_ui( "
"get_cmake_property( get_directory_property( get_filename_component( get_property( get_source_file_property( "
"get_target_property( get_test_property( "
"include( include_directories( include_external_msproject( include_guard( include_regular_expression( install( "
"link_directories( link_libraries( list( load_cache( mark_as_advanced( math( message( option( project( "
"remove_definitions( "
"separate_arguments( "
"set( set_directory_properties( set_property( set_source_files_properties( set_target_properties( set_tests_properties( "
"site_name( source_group( string( "
"target_compile_definitions( target_compile_features( target_compile_options( target_include_directories( "
"target_link_directories( target_link_libraries( target_link_options( target_precompile_headers( target_sources( "
"try_compile( try_run( "
"unset( variable_watch( "

, // 2 parameters
"ABSOLUTE ABSOLUTE_PATH AFTER ALGO ALIAS ALL ALL_COMPONENTS ALPHABET AND ANDROID_MK ANGLE-R ANY APPEND APPEND_STRING "
"ARCHIVE ARCHIVE_CREATE ARCHIVE_EXTRACT ARGC ARGS ARGV ASCENDING ASCII AT AUTHOR_WARNING "
"BASE_DIR BASE_DIRECTORY BASE_DIRS BEFORE BINARY_DIR BOOL BOTH BRIEF_DOCS "
"BUILD BUILD_ID BUILD_INTERFACE BUNDLE BUNDLE_EXECUTABLE BYPASS_PROVIDER BYPRODUCTS "
"CACHED_VARIABLE CALL CANCEL_CALL CAPTURE_CMAKE_ERROR CASE CDASH_UPLOAD CDASH_UPLOAD_TYPE "
"CHECK_FAIL CHECK_PASS CHECK_START CHMOD CHMOD_RECURSE CLEAR CMAKE_FIND_ROOT_PATH_BOTH CMAKE_FLAGS CMAKE_PATH CODE "
"COMMA COMMAND COMMAND_CONFIG COMMAND_ECHO COMMAND_ERROR_IS_FATAL COMMAND_EXPAND_LISTS COMMENT COMPARE "
"COMPILE_DEFINITIONS COMPILE_FEATURES COMPILE_LANGUAGE COMPILE_LANG_AND_ID COMPILE_OUTPUT_VARIABLE COMPONENT COMPONENTS "
"COMPRESSION COMPRESSION_LEVEL CONCAT CONDITION "
"CONFIG CONFIGS CONFIGURATION CONFIGURATIONS CONFIGURE CONFIGURE_DEPENDS CONFLICTING_DEPENDENICES_PREFIX CONTENT CONVERT "
"COPY COPYONLY COPY_FILE COPY_FILE_ERROR COPY_ON_ERROR CREATE_LINK CUDA_COMPILER_ID CUDA_COMPILER_VERSION "
"CXX_COMPILER_ID CXX_COMPILER_VERSION CXX_MODULES CXX_MODULES_BMI CXX_MODULES_DIRECTORY CXX_MODULE_HEADER_UNITS "
"C_COMPILER_ID C_COMPILER_VERSION "
"DEBUG DECIMAL DEFECT_COUNT DEFER DEFINED DEFINITION DEPENDS DEPFILE DEPRECATION DESCENDING DESCRIPTION DESTINATION "
"DEVICE_LINK DIRECTORIES DIRECTORY DIRECTORY_PERMISSIONS DOC DOWNLOAD "
"ECHO_ERROR_VARIABLE ECHO_OUTPUT_VARIABLE ENCODING END EQUAL "
"ERROR_FILE ERROR_QUIET ERROR_STRIP_TRAILING_WHITESPACE ERROR_VARIABLE ESCAPE_QUOTES EVAL EXACT "
"EXCLUDE EXCLUDE_FIXTURE EXCLUDE_FIXTURE_CLEANUP EXCLUDE_FIXTURE_SETUP EXCLUDE_FROM_ALL EXCLUDE_LABEL EXECUTABLES EXISTS "
"EXPAND_TILDE EXPECTED_HASH EXPECTED_MD5 EXPORT EXPORT_ANDROID_MK EXPORT_LINK_INTERFACE_LIBRARIES EXPR "
"EXT EXTENSION EXTRA_INCLUDE "
"FATAL_ERROR FETCHCONTENT_MAKEAVAILABLE_SERIAL "
"FILE FILENAME FILEPATH FILES FILES_MATCHING FILE_BASENAME FILE_PERMISSIONS FILE_SET FILTER FIND FIND_PACKAGE FLAGS "
"FOLLOW_SYMLINKS FOLLOW_SYMLINK_CHAIN FOR FORCE FORMAT FRAMEWORK FULL_DOCS FUNCTION "
"Fortran_COMPILER_ID Fortran_COMPILER_VERSION "
"GENERATE GENEX_EVAL GENEX_STRIP GET GET_CALL GET_CALL_IDS GET_EXTENSION GET_FILENAME GET_MESSAGE_LOG_LEVEL "
"GET_PARENT_PATH GET_RELATIVE_PART GET_ROOT_DIRECTORY GET_ROOT_NAME GET_ROOT_PATH GET_RUNTIME_DEPENDENCIES GET_STEM "
"GLOB GLOBAL GLOB_RECURSE GREATER GREATER_EQUAL GROUP GUARD GUID "
"HASH "
"HAS_EXTENSION HAS_FILENAME HAS_PARENT_PATH HAS_RELATIVE_PART HAS_ROOT_DIRECTORY HAS_ROOT_NAME HAS_ROOT_PATH HAS_STEM "
"HEADERS HEX HEXADECIMAL HINTS HIP_COMPILER_ID HIP_COMPILER_VERSION HOMEPAGE_URL HOST HOST_LINK HTTPHEADER "
"ID ID_VAR IF IGNORED IMPLICIT_DEPENDS IMPORTED IMPORTED_RUNTIME_ARTIFACTS IN INACTIVITY_TIMEOUT "
"INCLUDE INCLUDES INCLUDE_INTERNALS INCLUDE_LABEL INHERITED INITIALIZE_FROM_VARIABLE INPUT INPUT_FILE "
"INSENSITIVE INSERT INSTALL INSTALL_INTERFACE INSTALL_PREFIX INTERFACE INTERNAL IN_LIST "
"ISPC_COMPILER_ID ISPC_COMPILER_VERdSION IS_ABSOLUTE IS_DIRECTORY IS_NEWER_THAN IS_PREFIX IS_RELATIVE IS_SYMLINK ITEMS "
"JOB_POOL JOIN JSON "
"LABELS LANGUAGES LAST LAST_EXT LAST_ONLY LENGTH LENGTH_MAXIMUM LENGTH_MINIMUM LESS LESS_EQUAL LIBRARIES LIBRARY "
"LIMIT LIMIT_COUNT LIMIT_INPUT LIMIT_OUTPUT "
"LINK_GROUP LINK_LANGUAGE LINK_LANG_AND_ID LINK_LIBRARIES LINK_LIBRARY LINK_ONLY LINK_OPTIONS LINK_PRIVATE LINK_PUBLIC "
"LISTS LIST_DIRECTORIES LIST_ONLY LOCK LOG LOWER_CASE "
"MACOSX_BUNDLE MAIN_DEPENDENCY MAKE_C_IDENTIFIER MAKE_DIRECTORY MATCH MATCHAL MATCHES MD5 MEMBER MESSAGE_NEVER "
"MODIFIED_ACCESS MODULE MODULES MTIME "
"NAME NAMELINK_COMPONENT NAMELINK_ONLY NAMELINK_SKIP NAMES NAMESPACE NAMES_PER_DIR NAME_WE NAME_WLE "
"NATIVE_COMMAND NATIVE_PATH NATURAL NETRC NETRC_FILE NEW NEWLINE_CONSUME NEWLINE_STYLE NEW_PROCESS NORMALIZE NORMAL_PATH "
"NOT NOTEQUAL NOTICE NO_CACHE NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_FIND_ROOT_PATH NO_CMAKE_INSTALL_PREFIX "
"NO_CMAKE_PACKAGE_REGISTRY NO_CMAKE_PATH NO_CMAKE_SYSTEM_PACKAGE_REGISTRY NO_CMAKE_SYSTEM_PATH NO_DEFAULT_PATH "
"NO_HEX_CONVERSION NO_MODULE NO_PACKAGE_ROOT_PATH NO_POLICY_SCOPE NO_REPLACE "
"NO_SOURCE_PERMISSIONS NO_SYSTEM_ENVIRONMENT_PATH NUMBER_ERRORS NUMBER_WARNINGS "
"OBJCXX_COMPILER_ID OBJCXX_COMPILER_VERSION OBJC_COMPILER_ID OBJC_COMPILER_VERSION OBJECT OBJECTS OFFSET OLD "
"ONLY ONLY_CMAKE_FIND_ROOT_PATH ONLY_IF_DIFFERENT OPTIONAL OPTIONAL_COMPONENTS OPTIONS OR ORDER OUTPUT "
"OUTPUT_CONFIG OUTPUT_FILE OUTPUT_FORMAT OUTPUT_JUNIT OUTPUT_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE "
"PACKAGE PARALLEL_LEVEL PARENT_PATH PARENT_SCOPE PARSE_ARGV PARTS PATH PATHS PATH_EQUAL PATH_SUFFIXES PATTERN PATTERNS "
"PERMISSIONS PLATFORM PLATFORM_ID POLICIES POLICY POP POP_BACK POP_FRONT "
"POST_BUILD POST_EXCLUDE_FILES POST_EXCLUDE_REGEXES POST_INCLUDE_FILES POST_INCLUDE_REGEXES "
"PREFIX PREPEND PRE_BUILD PRE_EXCLUDE_REGEXES PRE_INCLUDE_REGEXES PRE_LINK PRIVATE PRIVATE_HEADER "
"PROCESS PROGRAM PROGRAMS PROGRAM_ARGS PROJECT PROJECT_NAME PROPAGATE PROPERTIES PROPERTY PUBLIC PUBLIC_HEADER PUSH "
"QUERY QUIET "
"RANDOM RANDOM_SEED RANGE RANGE_END RANGE_START READ READ_ACCESS READ_SYMLINK READ_WITH_PREFIX REALPATH REAL_PATH "
"REGEX REGISTRY_VIEW REGULAR_EXPRESSION RELATIVE RELATIVE_PART RELATIVE_PATH RELEASE "
"REMOVE REMOVED_ACCESS REMOVE_AT REMOVE_DUPLICATES REMOVE_EXTENSION REMOVE_FILENAME REMOVE_ITEM REMOVE_RECURSE RENAME "
"REPEAT REPLACE REPLACE_EXTENSION REPLACE_FILENAME REQUIRED "
"RESOLVED_DEPENDENCIES_VAR RESOURCE RESOURCE_SPEC_FILE RESULT RESULTS_VARIABLE RESULT_VARIABLE "
"RETRY_COUNT RETRY_DELAY RETURN_VALUE REUSE_FROM REVERSE ROOT_DIRECTORY ROOT_NAME ROOT_PATH "
"RUNTIME RUNTIME_DEPENDENCIES RUNTIME_DEPENDENCY_SET "
"RUN_OUTPUT_STDERR_VARIABLE RUN_OUTPUT_STDOUT_VARIABLE RUN_OUTPUT_VARIABLE "
"SCHEDULE_RANDOM SCOPE_FOR SCRIPT SEMICOLON SEND_ERROR SENSITIVE SEPARATE_ARGS SEPARATOR SET SET_DEPENDENCY_PROVIDER "
"SHA1 SHA224 SHA256 SHA384 SHA3_224 SHA3_256 SHA3_384 SHA3_512 SHA512 SHARED SHELL_PATH SHOW_PROGRESS SIZE "
"SORT SOURCE SOURCES SOURCE_DIR SOURCE_FROM_CONTENT SOURCE_FROM_FILE SOURCE_FROM_VAR START STATIC STATUS STEM "
"STOP_ON_FAILURE STOP_TIME STREQUAL STRGREATER STRGREATER_EQUAL STRIDE STRING STRINGS STRIP STRLESS STRLESS_EQUAL "
"SUBKEYS SUBLIST SUBMIT_URL SUBSTRING SUPPORTED_METHODS SYMBOLIC SYSTEM "
"TARGET TARGETS TARGET_BUNDLE_CONTENT_DIR TARGET_BUNDLE_DIR TARGET_DIRECTORY TARGET_EXISTS "
"TARGET_FILE TARGET_FILE_BASE_NAME TARGET_FILE_DIR TARGET_FILE_NAME TARGET_FILE_PREFIX TARGET_FILE_SUFFIX "
"TARGET_GENEX_EVAL TARGET_LINKER_FILE TARGET_LINKER_FILE_BASE_NAME TARGET_LINKER_FILE_DIR TARGET_LINKER_FILE_NAME "
"TARGET_LINKER_FILE_PREFIX TARGET_LINKER_FILE_SUFFIX TARGET_NAME TARGET_NAME_IF_EXISTS TARGET_OBJECTS "
"TARGET_PDB_FILE TARGET_PDB_FILE_BASE_NAME TARGET_PDB_FILE_DIR TARGET_PDB_FILE_NAME TARGET_POLICY TARGET_PROPERTY "
"TARGET_RUNTIME_DLLS TARGET_SONAME_FILE TARGET_SONAME_FILE_DIR TARGET_SONAME_FILE_NAME TEST TEST_LOAD TIMEOUT TIMESTAMP "
"TLS_CAINFO TLS_VERIFY "
"TOLOWER TOUCH TOUCH_NOCREATE TOUPPER TO_CMAKE_PATH TO_CMAKE_PATH_LIST TO_NATIVE_PATH TO_NATIVE_PATH_LIST "
"TRACE TRANSFORM TREE TYPE "
"UNIX_COMMAND UNKNOWN_MODIFIED_ACCESS UNKNOWN_READ_ACCESS UNRESOLVED_DEPENDENCIES_VAR UPLOAD UPPER UPPER_CASE "
"USERPWD USES_TERMINAL USE_SOURCE_PERMISSIONS UTC UUID "
"VALIDATOR VALUE VALUE_NAMES VARIABLE VARIABLES "
"VERBATIM VERSION VERSION_EQUAL VERSION_GREATER VERSION_GREATER_EQUAL VERSION_LESS VERSION_LESS_EQUAL VIEW "
"WARNING WINDOWS_COMMAND WINDOWS_REGISTRY WORKING_DIRECTORY WRITE ZIP_LISTS "

, // 3 properties
"ABSTRACT ADVANCED AUTOMOC AUTORCC AUTOUIC COST DEFINITIONS DISABLED ENVIRONMENT EchoString FOLDER FQDN "
"GENERATED GNUtoMS HELPSTRING HOSTNAME LANGUAGE LOCATION MACROS MEASUREMENT MODIFIED PROCESSORS "
"SOVERSION SUBDIRECTORIES SUFFIX TESTS XCTEST "

, // 4 variables
"ANDROID APPLE ASM ASMFLAGS BORLAND BSD "
"CACHE CC CFLAGS CSFLAGS CUDAARCHS CUDACXX CUDAFLAGS CUDAHOSTCXX CXX CXXFLAGS CYGWIN DESTDIR ENV FC FFLAGS GHSMULTI "
"HIPCXX HIPFLAGS IOS ISPC ISPCFLAGS LDFLAGS LINUX "
"MINGW MSVC MSVC10 MSVC11 MSVC12 MSVC14 MSVC60 MSVC70 MSVC71 MSVC80 MSVC90 MSYS OBJC OBJCXX RC RCFLAGS SWIFTC UNIX "
"VERBOSE WIN32 WINCE XCODE "

, // 5 values
"ANSI AUTO BIN Build CRLF Configure Coverage DATA DOS Debug Done ExtraFiles FALSE GROUP_EXECUTE GROUP_READ GROUP_WRITE "
"IGNORE INFO LF LIB LOCALE LOCALSTATE MAN MemCheck MinSizeRel NO NONE NOTFOUND Notes "
"OEM OFF ON OWNER_EXECUTE OWNER_READ OWNER_WRITE RUNSTATE RelWithDebInfo Release "
"SBIN SETGID SETUID SHAREDSTATE SYSCONF Start Submit TRUE Test "
"UTF-16BE UTF-16LE UTF-32BE UTF-32LE UTF-8 UTF8 Update Upload WORLD_EXECUTE WORLD_READ WORLD_WRITE YES "

, // 6 long properties
NULL

, // 7 long variables
NULL

, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
//--Autogenerated -- end of section automatically generated
}};

static EDITSTYLE Styles_CMake[] = {
	EDITSTYLE_DEFAULT,
	{ SCE_CMAKE_WORD, NP2StyleX_Keyword, L"fore:#0000FF" },
	{ SCE_CMAKE_COMMANDS, NP2StyleX_Command, L"fore:#FF8000" },
	{ SCE_CMAKE_FUNCATION, NP2StyleX_Function, L"fore:#808000" },
	{ SCE_CMAKE_MACRO, NP2StyleX_Macro, L"fore:#808000" },
	{ SCE_CMAKE_PARAMETERS, NP2StyleX_Parameter, L"fore:#0C71C3" },
	{ SCE_CMAKE_PROPERTIES, NP2StyleX_Property, L"fore:#A46000" },
	{ SCE_CMAKE_VALUES, NP2StyleX_Value, L"fore:#008287" },
	{ MULTI_STYLE(SCE_CMAKE_COMMENT, SCE_CMAKE_BLOCK_COMMENT, 0, 0), NP2StyleX_Comment, L"fore:#608060" },
	{ SCE_CMAKE_STRING, NP2StyleX_String, L"fore:#008080" },
	{ SCE_CMAKE_BRACKET_ARGUMENT, NP2StyleX_BracketArgument, L"fore:#648000" },
	{ MULTI_STYLE(SCE_CMAKE_VARIABLE, SCE_CMAKE_VARIABLE_DOLLAR, SCE_CMAKE_VARIABLE_AT, 0), NP2StyleX_Variable, L"fore:#9E4D2A" },
	{ SCE_CMAKE_ESCAPECHAR, NP2StyleX_EscapeSequence, L"fore:#0080C0" },
	{ SCE_CMAKE_NUMBER, NP2StyleX_Number, L"fore:#FF0000" },
	{ SCE_CMAKE_OPERATOR, NP2StyleX_Operator, L"fore:#B000B0" },
};

EDITLEXER lexCMake = {
	SCLEX_CMAKE, NP2LEX_CMAKE,
//Settings++Autogenerated -- start of section automatically generated
	{
		LexerAttr_AngleBracketGeneric,
		TAB_WIDTH_4, INDENT_WIDTH_4,
		(1 << 0) | (1 << 1), // level1, level2
		0,
		'\\', SCE_CMAKE_ESCAPECHAR, 0,
		SCE_CMAKE_WORD,
		0, 0,
		SCE_CMAKE_OPERATOR, 0
		, KeywordAttr32(0, KeywordAttr_PreSorted) // keywords
		| KeywordAttr32(1, KeywordAttr_PreSorted) // commands
		| KeywordAttr32(2, KeywordAttr_PreSorted) // parameters
		| KeywordAttr32(3, KeywordAttr_PreSorted) // properties
		| KeywordAttr32(4, KeywordAttr_PreSorted) // variables
		| KeywordAttr32(5, KeywordAttr_PreSorted) // values
		| KeywordAttr32(6, KeywordAttr_NoLexer) // long properties
		| KeywordAttr64(7, KeywordAttr_NoLexer) // long variables
	},
//Settings--Autogenerated -- end of section automatically generated
	EDITLEXER_HOLE(L"CMake Script", Styles_CMake),
	L"cmake; ctest",
	&Keywords_CMake,
	Styles_CMake
};
