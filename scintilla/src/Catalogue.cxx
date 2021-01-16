// Scintilla source code edit control
/** @file Catalogue.cxx
 ** Lexer infrastructure.
 ** Contains a list of LexerModules which can be searched to find a module appropriate for a
 ** particular language.
 **/
// Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <cstdlib>
#include <cassert>
#include <cstring>

#include <stdexcept>
#include <vector>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "LexerModule.h"
#include "CatalogueModules.h"
#include "Catalogue.h"

using namespace Scintilla;

namespace {

CatalogueModules catalogueDefault;

}

const LexerModule *Catalogue::Find(int language) noexcept {
	return catalogueDefault.Find(language);
}

const LexerModule *Catalogue::Find(const char *languageName) noexcept {
	return catalogueDefault.Find(languageName);
}

void Catalogue::AddLexerModule(LexerModule *plm) {
	catalogueDefault.AddLexerModule(plm);
}

// To add or remove a lexer, add or remove its file and run LexGen.py.

// Force a reference to all of the Scintilla lexers so that the linker will
// not remove the code of the lexers.
void Scintilla_LinkLexers() {
#if !defined(SCI_EMPTYCATALOGUE)

	if (!catalogueDefault.Empty()) {
		return;
	}

//++Autogenerated -- run scripts/LexGen.py to regenerate
//**\(\textern LexerModule \*;\n\)
	//extern LexerModule lmA68k;
	//extern LexerModule lmAbaqus;
	//extern LexerModule lmAda;
	//extern LexerModule lmAHK;
	//extern LexerModule lmAPDL;
	//extern LexerModule lmAs;
	extern LexerModule lmAsm;
	//extern LexerModule lmAsn1;
	//extern LexerModule lmASY;
	extern LexerModule lmAU3;
	//extern LexerModule lmAVE;
	extern LexerModule lmAVS;
	//extern LexerModule lmBaan;
	extern LexerModule lmBash;
	extern LexerModule lmBatch;
	//extern LexerModule lmBibTeX;
	//extern LexerModule lmBlitzBasic;
	//extern LexerModule lmBullant;
	//extern LexerModule lmCaml;
	extern LexerModule lmCIL;
	//extern LexerModule lmClw;
	//extern LexerModule lmClwNoCase;
	extern LexerModule lmCmake;
	//extern LexerModule lmCOBOL;
	//extern LexerModule lmCoffeeScript;
	extern LexerModule lmConf;
	extern LexerModule lmCPP;
	//extern LexerModule lmCPPNoCase;
	//extern LexerModule lmCsound;
	extern LexerModule lmCss;
	//extern LexerModule lmD;
	extern LexerModule lmDart;
	//extern LexerModule lmDataflex;
	extern LexerModule lmDiff;
	//extern LexerModule lmDMAP;
	//extern LexerModule lmDMIS;
	//extern LexerModule lmECL;
	//extern LexerModule lmEDIFACT;
	//extern LexerModule lmEiffel;
	//extern LexerModule lmEiffelkw;
	//extern LexerModule lmErlang;
	//extern LexerModule lmErrorList;
	//extern LexerModule lmESCRIPT;
	//extern LexerModule lmF77;
	//extern LexerModule lmFlagShip;
	//extern LexerModule lmForth;
	extern LexerModule lmFortran;
	//extern LexerModule lmFreeBasic;
	extern LexerModule lmFSharp;
	//extern LexerModule lmGAP;
	extern LexerModule lmGN;
	extern LexerModule lmGo;
	extern LexerModule lmGraphViz;
	//extern LexerModule lmGui4Cli;
	//extern LexerModule lmHaskell;
	//extern LexerModule lmHollywood;
	extern LexerModule lmHTML;
	//extern LexerModule lmIHex;
	//extern LexerModule lmIndent;
	extern LexerModule lmInno;
	extern LexerModule lmJavaScript;
	extern LexerModule lmJSON;
	extern LexerModule lmJulia;
	//extern LexerModule lmKix;
	extern LexerModule lmKotlin;
	//extern LexerModule lmKVIrc;
	extern LexerModule lmLatex;
	extern LexerModule lmLisp;
	//extern LexerModule lmLiterateHaskell;
	extern LexerModule lmLLVM;
	//extern LexerModule lmLot;
	//extern LexerModule lmLout;
	extern LexerModule lmLua;
	//extern LexerModule lmMagikSF;
	extern LexerModule lmMake;
	//extern LexerModule lmMarkdown;
	extern LexerModule lmMatlab;
	//extern LexerModule lmMaxima;
	//extern LexerModule lmMETAPOST;
	//extern LexerModule lmMMIXAL;
	//extern LexerModule lmModula;
	//extern LexerModule lmMSSQL;
	//extern LexerModule lmMySQL;
	//extern LexerModule lmNim;
	//extern LexerModule lmNimrod;
	//extern LexerModule lmNncrontab;
	extern LexerModule lmNsis;
	extern LexerModule lmNull;
	//extern LexerModule lmOctave;
	//extern LexerModule lmOpal;
	extern LexerModule lmPascal;
	//extern LexerModule lmPB;
	extern LexerModule lmPerl;
	//extern LexerModule lmPHPSCRIPT;
	//extern LexerModule lmPLM;
	//extern LexerModule lmPO;
	//extern LexerModule lmPOV;
	//extern LexerModule lmPowerPro;
	extern LexerModule lmPowerShell;
	//extern LexerModule lmProgress;
	extern LexerModule lmProps;
	//extern LexerModule lmPS;
	//extern LexerModule lmPureBasic;
	extern LexerModule lmPython;
	extern LexerModule lmR;
	//extern LexerModule lmRaku;
	extern LexerModule lmREBOL;
	//extern LexerModule lmRegistry;
	extern LexerModule lmRuby;
	extern LexerModule lmRust;
	//extern LexerModule lmSAS;
	//extern LexerModule lmScriptol;
	extern LexerModule lmSmali;
	//extern LexerModule lmSmalltalk;
	//extern LexerModule lmSML;
	//extern LexerModule lmSorc;
	//extern LexerModule lmSpecman;
	//extern LexerModule lmSpice;
	extern LexerModule lmSQL;
	//extern LexerModule lmSrec;
	//extern LexerModule lmStata;
	//extern LexerModule lmSTTXT;
	extern LexerModule lmSwift;
	//extern LexerModule lmTACL;
	//extern LexerModule lmTADS3;
	//extern LexerModule lmTAL;
	extern LexerModule lmTCL;
	//extern LexerModule lmTCMD;
	//extern LexerModule lmTEHex;
	//extern LexerModule lmTeX;
	extern LexerModule lmTexinfo;
	extern LexerModule lmTOML;
	//extern LexerModule lmTxt2tags;
	extern LexerModule lmVB;
	extern LexerModule lmVBScript;
	extern LexerModule lmVerilog;
	extern LexerModule lmVHDL;
	extern LexerModule lmVim;
	//extern LexerModule lmVisualProlog;
	extern LexerModule lmWASM;
	//extern LexerModule lmX12;
	extern LexerModule lmXML;
	extern LexerModule lmYAML;

//--Autogenerated -- end of automatically generated section

	catalogueDefault.AddLexerModules({
//++Autogenerated -- run scripts/LexGen.py to regenerate
//**\(\t\t&\*,\n\)
		//&lmA68k,
		//&lmAbaqus,
		//&lmAda,
		//&lmAHK,
		//&lmAPDL,
		//&lmAs,
		&lmAsm,
		//&lmAsn1,
		//&lmASY,
		&lmAU3,
		//&lmAVE,
		&lmAVS,
		//&lmBaan,
		&lmBash,
		&lmBatch,
		//&lmBibTeX,
		//&lmBlitzBasic,
		//&lmBullant,
		//&lmCaml,
		&lmCIL,
		//&lmClw,
		//&lmClwNoCase,
		&lmCmake,
		//&lmCOBOL,
		//&lmCoffeeScript,
		&lmConf,
		&lmCPP,
		//&lmCPPNoCase,
		//&lmCsound,
		&lmCss,
		//&lmD,
		&lmDart,
		//&lmDataflex,
		&lmDiff,
		//&lmDMAP,
		//&lmDMIS,
		//&lmECL,
		//&lmEDIFACT,
		//&lmEiffel,
		//&lmEiffelkw,
		//&lmErlang,
		//&lmErrorList,
		//&lmESCRIPT,
		//&lmF77,
		//&lmFlagShip,
		//&lmForth,
		&lmFortran,
		//&lmFreeBasic,
		&lmFSharp,
		//&lmGAP,
		&lmGN,
		&lmGo,
		&lmGraphViz,
		//&lmGui4Cli,
		//&lmHaskell,
		//&lmHollywood,
		&lmHTML,
		//&lmIHex,
		//&lmIndent,
		&lmInno,
		&lmJavaScript,
		&lmJSON,
		&lmJulia,
		//&lmKix,
		&lmKotlin,
		//&lmKVIrc,
		&lmLatex,
		&lmLisp,
		//&lmLiterateHaskell,
		&lmLLVM,
		//&lmLot,
		//&lmLout,
		&lmLua,
		//&lmMagikSF,
		&lmMake,
		//&lmMarkdown,
		&lmMatlab,
		//&lmMaxima,
		//&lmMETAPOST,
		//&lmMMIXAL,
		//&lmModula,
		//&lmMSSQL,
		//&lmMySQL,
		//&lmNim,
		//&lmNimrod,
		//&lmNncrontab,
		&lmNsis,
		&lmNull,
		//&lmOctave,
		//&lmOpal,
		&lmPascal,
		//&lmPB,
		&lmPerl,
		//&lmPHPSCRIPT,
		//&lmPLM,
		//&lmPO,
		//&lmPOV,
		//&lmPowerPro,
		&lmPowerShell,
		//&lmProgress,
		&lmProps,
		//&lmPS,
		//&lmPureBasic,
		&lmPython,
		&lmR,
		//&lmRaku,
		&lmREBOL,
		//&lmRegistry,
		&lmRuby,
		&lmRust,
		//&lmSAS,
		//&lmScriptol,
		&lmSmali,
		//&lmSmalltalk,
		//&lmSML,
		//&lmSorc,
		//&lmSpecman,
		//&lmSpice,
		&lmSQL,
		//&lmSrec,
		//&lmStata,
		//&lmSTTXT,
		&lmSwift,
		//&lmTACL,
		//&lmTADS3,
		//&lmTAL,
		&lmTCL,
		//&lmTCMD,
		//&lmTEHex,
		//&lmTeX,
		&lmTexinfo,
		&lmTOML,
		//&lmTxt2tags,
		&lmVB,
		&lmVBScript,
		&lmVerilog,
		&lmVHDL,
		&lmVim,
		//&lmVisualProlog,
		&lmWASM,
		//&lmX12,
		&lmXML,
		&lmYAML,

//--Autogenerated -- end of automatically generated section
	});

#endif
}
