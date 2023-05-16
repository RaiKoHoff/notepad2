#include "EditLexer.h"
#include "EditStyleX.h"

static KEYWORDLIST Keywords_PS1 = {{
//++Autogenerated -- start of section automatically generated
"assembly base begin break catch class command configuration continue data define do dynamicParam dynamicparam "
"else elseif end enum exit filter finally for foreach from function hidden if in inlineScript inlinescript interface "
"module namespace parallel param private process public return sequence static switch throw trap try type until using "
"var while workflow "

, // 1 type
"adsi array bigint bool byte char cimclass datetime decimal double float guid hashtable int int16 int32 int64 ipaddress "
"long mailaddress math ordered psobject psvariable ref regex runspace sbyte scriptblock short single string timespan "
"uint uint16 uint32 uint64 ulong uri ushort version void wmi wmiclass xml "

, // 2 cmdlet
"Add-Computer Add-Content Add-History Add-JobTrigger Add-LocalGroupMember Add-Member Add-PSSnapin Add-Type "
"Checkpoint-Computer "
"Clear-Content Clear-EventLog Clear-History Clear-Host Clear-Item Clear-ItemProperty Clear-RecycleBin Clear-Variable "
"Compare-Object Complete-Transaction Compress-Archive Connect-PSSession Connect-WSMan Convert-Path Convert-String "
"ConvertFrom-Csv ConvertFrom-Json ConvertFrom-Markdown "
"ConvertFrom-SddlString ConvertFrom-SecureString ConvertFrom-String ConvertFrom-StringData "
"ConvertTo-Csv ConvertTo-Html ConvertTo-Json ConvertTo-SecureString ConvertTo-Xml Copy-Item Copy-ItemProperty "
"Debug-Job Debug-Process Debug-Runspace Disable-ComputerRestore Disable-ExperimentalFeature Disable-JobTrigger "
"Disable-LocalUser "
"Disable-PSBreakpoint Disable-PSRemoting Disable-PSSessionConfiguration Disable-PSTrace Disable-PSWSManCombinedTrace "
"Disable-RunspaceDebug Disable-ScheduledJob Disable-WSManCredSSP Disable-WSManTrace "
"Disconnect-PSSession Disconnect-WSMan "
"Enable-ComputerRestore Enable-ExperimentalFeature Enable-JobTrigger Enable-LocalUser "
"Enable-PSBreakpoint Enable-PSRemoting Enable-PSSessionConfiguration Enable-PSTrace Enable-PSWSManCombinedTrace "
"Enable-RunspaceDebug Enable-ScheduledJob Enable-WSManCredSSP Enable-WSManTrace Enter-PSHostProcess Enter-PSSession "
"Exit-PSHostProcess Exit-PSSession Expand-Archive Export-Alias Export-BinaryMiLog "
"Export-Clixml Export-Console Export-Counter Export-Csv Export-FormatData Export-ModuleMember Export-ODataEndpointProxy "
"Export-PSSession "
"Find-Command Find-DscResource Find-Module Find-Package Find-PackageProvider Find-RoleCapability Find-Script "
"ForEach-Object Format-Custom Format-Hex Format-List Format-Table Format-Wide "
"Get-Acl Get-Alias Get-AuthenticodeSignature Get-ChildItem "
"Get-CimAssociatedInstance Get-CimClass Get-CimInstance Get-CimSession Get-Clipboard Get-CmsMessage "
"Get-Command Get-ComputerInfo Get-ComputerRestorePoint Get-Content Get-ControlPanelItem Get-Counter Get-Credential "
"Get-Culture Get-Date Get-Error Get-Event Get-EventLog Get-EventSubscriber Get-ExecutionPolicy Get-ExperimentalFeature "
"Get-FileHash Get-FormatData Get-Help Get-History Get-Host Get-HotFix "
"Get-InstalledModule Get-InstalledScript Get-Item Get-ItemProperty Get-ItemPropertyValue Get-Job Get-JobTrigger "
"Get-LocalGroup Get-LocalGroupMember Get-LocalUser Get-Location Get-LogProperties "
"Get-MarkdownOption Get-Member Get-Module Get-OperationValidation Get-Package Get-PackageProvider Get-PackageSource "
"Get-PfxCertificate Get-Process Get-PSBreakpoint Get-PSCallStack Get-PSDrive Get-PSHostProcessInfo Get-PSProvider "
"Get-PSReadLineKeyHandler Get-PSReadLineOption Get-PSRepository "
"Get-PSSession Get-PSSessionCapability Get-PSSessionConfiguration Get-PSSnapin Get-PSSubsystem "
"Get-Random Get-Runspace Get-RunspaceDebug Get-ScheduledJob Get-ScheduledJobOption Get-Service "
"Get-TimeZone Get-TraceSource Get-Transaction Get-TypeData Get-UICulture Get-Unique Get-Uptime Get-Variable Get-Verb "
"Get-WinEvent Get-WmiObject Get-WSManCredSSP Get-WSManInstance Group-Object "
"Import-Alias Import-BinaryMiLog Import-Clixml Import-Counter Import-Csv Import-LocalizedData Import-Module "
"Import-PackageProvider Import-PowerShellDataFile Import-PSSession "
"Install-Module Install-Package Install-PackageProvider Install-Script Invoke-AsWorkflow Invoke-CimMethod Invoke-Command "
"Invoke-Expression Invoke-History Invoke-Item Invoke-OperationValidation Invoke-RestMethod "
"Invoke-WebRequest Invoke-WmiMethod Invoke-WSManAction "
"Join-Path Join-String Limit-EventLog Measure-Command Measure-Object Move-Item Move-ItemProperty "
"New-Alias New-CimInstance New-CimSession New-CimSessionOption New-Event New-EventLog New-FileCatalog New-Guid "
"New-Item New-ItemProperty New-JobTrigger New-LocalGroup New-LocalUser New-Module New-ModuleManifest New-Object "
"New-PSDrive New-PSRoleCapabilityFile New-PSSession New-PSSessionConfigurationFile New-PSSessionOption "
"New-PSTransportOption New-PSWorkflowExecutionOption New-PSWorkflowSession "
"New-ScheduledJobOption New-ScriptFileInfo New-Service New-TemporaryFile New-TimeSpan New-Variable "
"New-WebServiceProxy New-WinEvent New-WSManInstance New-WSManSessionOption "
"Out-Default Out-File Out-GridView Out-Host Out-Null Out-Printer Out-String "
"Pop-Location Protect-CmsMessage PSConsoleHostReadLine Publish-Module Publish-Script Push-Location "
"Read-Host Receive-Job Receive-PSSession Register-ArgumentCompleter Register-CimIndicationEvent Register-EngineEvent "
"Register-ObjectEvent Register-PackageSource Register-PSRepository Register-PSSessionConfiguration Register-ScheduledJob "
"Register-WmiEvent Remove-Alias Remove-CimInstance Remove-CimSession Remove-Computer Remove-Event Remove-EventLog "
"Remove-Item Remove-ItemProperty Remove-Job Remove-JobTrigger Remove-LocalGroup Remove-LocalGroupMember Remove-LocalUser "
"Remove-Module Remove-PSBreakpoint Remove-PSDrive Remove-PSReadLineKeyHandler Remove-PSSession Remove-PSSnapin "
"Remove-Service Remove-TypeData Remove-Variable Remove-WmiObject Remove-WSManInstance "
"Rename-Computer Rename-Item Rename-ItemProperty Rename-LocalGroup Rename-LocalUser "
"Reset-ComputerMachinePassword Resolve-Path Restart-Computer Restart-Service Restore-Computer Resume-Job Resume-Service "
"Save-Help Save-Module Save-Package Save-Script Select-Object Select-String Select-Xml Send-MailMessage "
"Set-Acl Set-Alias Set-AuthenticodeSignature Set-CimInstance Set-Clipboard Set-Content Set-Date Set-ExecutionPolicy "
"Set-Item Set-ItemProperty Set-JobTrigger Set-LocalGroup Set-LocalUser Set-Location Set-LogProperties Set-MarkdownOption "
"Set-PackageSource "
"Set-PSBreakpoint Set-PSDebug Set-PSReadLineKeyHandler Set-PSReadLineOption Set-PSRepository Set-PSSessionConfiguration "
"Set-ScheduledJob Set-ScheduledJobOption Set-Service Set-StrictMode Set-TimeZone Set-TraceSource Set-Variable "
"Set-WmiInstance Set-WSManInstance Set-WSManQuickConfig Show-Command Show-ControlPanelItem Show-EventLog Show-Markdown "
"Sort-Object Split-Path "
"Start-Job Start-Process Start-Service Start-Sleep Start-ThreadJob Start-Trace Start-Transaction Start-Transcript "
"Stop-Computer Stop-Job Stop-Process Stop-Service Stop-Trace Stop-Transcript Suspend-Job Suspend-Service "
"Tee-Object Test-ComputerSecureChannel Test-Connection Test-FileCatalog Test-Json Test-ModuleManifest "
"Test-Path Test-PSSessionConfigurationFile Test-ScriptFileInfo Test-WSMan Trace-Command "
"Unblock-File Undo-Transaction Uninstall-Module Uninstall-Package Uninstall-Script Unprotect-CmsMessage Unregister-Event "
"Unregister-PackageSource Unregister-PSRepository Unregister-PSSessionConfiguration Unregister-ScheduledJob "
"Update-FormatData Update-Help Update-List Update-Module Update-ModuleManifest Update-Script Update-ScriptFileInfo "
"Update-TypeData Use-Transaction "
"Wait-Debugger Wait-Event Wait-Job Wait-Process Where-Object Write-Debug Write-Error Write-EventLog Write-Host "
"Write-Information Write-Output Write-Progress Write-Verbose Write-Warning "

, // 3 alias
"ac asnp cat cd cfs chdir clc clear clhy cli clp cls clv cnsn compare copy cp cpi cpp curl cvpa dbp del diff dir dnsn "
"ebp echo epal epcsv epsn erase etsn exsn fc fhx fl ft fw "
"gal gbp gc gcb gci gcm gcs gdr gerr ghy gi gin gjb gl gm gmo gp gps gpv group gsn gsnp gsv gtz gu gv gwmi "
"h help history icm iex ihy ii ipal ipcsv ipmo ipsn irm ise iwmi iwr kill lp ls man md measure mi mkdir mount move mp mv "
"nal ndr ni nmo npssc nsn nv ogv oh oss pause popd prompt ps pushd pwd "
"r rbp rcjb rcsn rd rdr ren ri rjb rm rmdir rmo rni rnp rp rsn rsnp rujb rv rvpa rwmi "
"sajb sal saps sasv sbp sc scb select set shcm si sl sleep sls sort sp spjb spps spsv start stz sujb sv swmi tee trcm "
"wget where wjb write "

, // 4 pre-defined variables
"Alias args ConfirmPreference ConsoleFileName DebugPreference "
"EnabledExperimentalFeatures Env Error ErrorActionPreference ErrorView Event EventArgs EventSubscriber ExecutionContext "
"false foreach FormatEnumerationLimit Function HOME Host InformationPreference input IsCoreCLR IsLinux IsMacOS IsWindows "
"LastExitCode LogCommandHealthEvent LogCommandLifecycleEvent LogEngineHealthEvent LogEngineLifecycleEvent "
"LogProviderHealthEvent LogProviderLifecycleEvent "
"Matches MaximumHistoryCount MyInvocation NestedPromptLevel null OFS OutputEncoding "
"PID PROFILE ProgressPreference PSBoundParameters PSCmdlet PSCommandPath PSCulture "
"PSDebugContext PSDefaultParameterValues PSEdition PSEmailServer PSHOME PSItem PSModuleAutoLoadingPreference "
"PSNativeCommandArgumentPassing "
"PSScriptRoot PSSenderInfo PSSessionApplicationName PSSessionConfigurationName PSSessionOption PSStyle PSUICulture "
"PSVersionTable PWD "
"Sender ShellId StackTrace switch this Transcript true Variable VerbosePreference WarningPreference WhatIfPreference "

, // 5 parameters
NULL

, // 6 misc
"Description EndEegion Example ExternalHelp ForwardHelpCategory ForwardHelpTargetName Functionality Inputs Link Notes "
"Outputs Parameter Region RemoteHelpRunspace Requires Role Synopsis default endregion global local region requires "
"script "

, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
//--Autogenerated -- end of section automatically generated
}};

static EDITSTYLE Styles_PS1[] = {
	EDITSTYLE_DEFAULT,
	{ SCE_POWERSHELL_KEYWORD, NP2StyleX_Keyword, L"bold; fore:#FF8000" },
	{ SCE_POWERSHELL_TYPE, NP2StyleX_Type, L"fore:#1E90FF" },
	{ SCE_POWERSHELL_DIRECTIVE, NP2StyleX_Directive, L"fore:#FF8000" },
	{ SCE_POWERSHELL_ATTRIBUTE, NP2StyleX_Attribute, L"fore:#FF8000" },
	{ SCE_POWERSHELL_CLASS, NP2StyleX_Class, L"bold; fore:#1E90FF" },
	{ SCE_POWERSHELL_ENUM, NP2StyleX_Enumeration, L"fore:#FF8000" },
	{ SCE_POWERSHELL_CMDLET, NP2StyleX_Cmdlet, L"fore:#9E4D2A" },
	{ SCE_POWERSHELL_ALIAS, NP2StyleX_Alias, L"bold; fore:#0080C0" },
	{ SCE_POWERSHELL_FUNCTION_DEFINITION, NP2StyleX_FunctionDefinition, L"bold; fore:#A46000" },
	{ SCE_POWERSHELL_FUNCTION, NP2StyleX_Function, L"fore:#A46000" },
	{ SCE_POWERSHELL_PARAMETER, NP2StyleX_Parameter, L"fore:#3A6EA5" },
	{ MULTI_STYLE(SCE_POWERSHELL_BUILTIN_VARIABLE, SCE_POWERSHELL_VARIABLE_SCOPE, 0, 0), NP2StyleX_PredefinedVariable, L"bold; fore:#007F7F" },
	{ MULTI_STYLE(SCE_POWERSHELL_VARIABLE, SCE_POWERSHELL_BRACE_VARIABLE, 0, 0), NP2StyleX_Variable, L"fore:#003CE6" },
	{ MULTI_STYLE(SCE_POWERSHELL_COMMENTLINE, SCE_POWERSHELL_COMMENTBLOCK, 0, 0), NP2StyleX_Comment, L"fore:#608060" },
	{ SCE_POWERSHELL_COMMENTTAG, NP2StyleX_DocCommentTag, L"fore:#408080" },
	{ MULTI_STYLE(SCE_POWERSHELL_STRING_DQ, SCE_POWERSHELL_HERE_STRING_DQ, 0, 0), NP2StyleX_String, L"fore:#008000" },
	{ MULTI_STYLE(SCE_POWERSHELL_STRING_SQ, SCE_POWERSHELL_HERE_STRING_SQ, 0, 0), NP2StyleX_VerbatimString, L"fore:#008080" },
	{ SCE_POWERSHELL_ESCAPECHAR, NP2StyleX_EscapeSequence, L"fore:#0080C0" },
	{ SCE_POWERSHELL_LABEL, NP2StyleX_Label, L"back:#FFC040" },
	{ SCE_POWERSHELL_NUMBER, NP2StyleX_Number, L"fore:#FF0000" },
	{ MULTI_STYLE(SCE_POWERSHELL_OPERATOR, SCE_POWERSHELL_OPERATOR2, 0, 0), NP2StyleX_Operator, L"fore:#B000B0" },
};

EDITLEXER lexPowerShell = {
	SCLEX_POWERSHELL, NP2LEX_POWERSHELL,
//Settings++Autogenerated -- start of section automatically generated
		LexerAttr_Default,
		TAB_WIDTH_4, INDENT_WIDTH_4,
		(1 << 0) | (1 << 1), // class, function
		SCE_POWERSHELL_FUNCTION_DEFINITION,
		'`', SCE_POWERSHELL_ESCAPECHAR, 0,
		0,
		0, 0,
		SCE_POWERSHELL_OPERATOR, SCE_POWERSHELL_OPERATOR2
		, KeywordAttr32(0, KeywordAttr_PreSorted) // keywords
		| KeywordAttr32(1, KeywordAttr_PreSorted) // type
		| KeywordAttr32(2, KeywordAttr_MakeLower | KeywordAttr_PreSorted) // cmdlet
		| KeywordAttr32(3, KeywordAttr_PreSorted) // alias
		| KeywordAttr32(4, KeywordAttr_MakeLower | KeywordAttr_PreSorted) // pre-defined variables
		| KeywordAttr32(5, KeywordAttr_NoLexer) // parameters
		| KeywordAttr32(6, KeywordAttr_NoLexer) // misc
		, SCE_POWERSHELL_TASKMARKER,
//Settings--Autogenerated -- end of section automatically generated
	EDITLEXER_HOLE(L"PowerShell Script", Styles_PS1),
	L"ps1; psc1; psd1; psm1",
	&Keywords_PS1,
	Styles_PS1
};
