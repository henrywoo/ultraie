;-----------------------------------------------------------------------------
; installation file for ultraie
; 2008 wufuheng@gmail.com
;-----------------------------------------------------------------------------
!include "FileFunc.nsh"
!insertmacro GetFileVersion
!include "WordFunc.nsh"
!include "nsDialogs.nsh"


!insertmacro VersionCompare

;-----------------------------------------------------------------------------
; const definition
;-----------------------------------------------------------------------------
!define PRODUCT_WEB_SITE "http://www.ultraie.com"
; The root can be changed if necessary
!define ROOT_            "F:\6beenetworks\client\uutoolbar"
!define TEMPLATE_ROOT    "${ROOT_}\template\cn"
!define DATABASE_ROOT    "${ROOT_}\db"
!define DEPLOY_ROOT      "${ROOT_}\bin\debug"
!define VENDOR_ROOT      "${ROOT_}\vendor"
!define PNAME            "UltraIE"

Name          "UltraIE(debug)"
Caption       "UltraIE Installer(debug)"
Icon          "install.ico"
UninstallIcon "uninstall.ico"
OutFile       "UltraIE_Installer_debug.exe"

SetDateSave          on
SetDatablockOptimize on
CRCCheck             on
SetCompressor /SOLID lzma
SilentInstall        normal
BGGradient 000000 800000 FFFFFF
InstallColors FF8080 000030
XPStyle              on
AllowRootDirInstall  true

InstProgressFlags    smooth
InstallDir $PROGRAMFILES\6BeeNetworks\UltraIE
BrandingText    ${PRODUCT_WEB_SITE}
InstallDirRegKey HKLM "Software\6BeeNetworks\UltraIE" "Location"
CheckBitmap "modern.bmp"
LicenseText "Terms and License"
LicenseData "license.txt"

RequestExecutionLevel admin

Var PVERSION ;

;!insertmacro Locate
;!insertmacro VersionCompare

Function ConnectInternet
  Push $R0
    ClearErrors
    Dialer::AttemptConnect
    IfErrors noie3
    Pop $R0
    StrCmp $R0 "online" connected
      MessageBox MB_OK|MB_ICONSTOP "Cannot connect to the internet."
      Quit
    noie3:
    ; IE3 not installed
    MessageBox MB_OK|MB_ICONINFORMATION "Please connect to the internet now."
    connected:
  Pop $R0
FunctionEnd

Function InstallVC2005Sp1ReDis
  Call ConnectInternet ;Make an internet connection (if no connection available)

  StrCpy $2 "$TEMP\vcredist_x86.exe"
  NSISdl::download http://download.microsoft.com/download/e/1/c/e1c773de-73ba-494a-a5ba-f24906ecf088/vcredist_x86.exe $2
  Pop $0
  StrCmp $0 success onsuccess
    SetDetailsView show
    DetailPrint "download failed: $0"
    Abort
  onsuccess:
    ExecWait '"$2" /q:a /c:"VCREDI~3.EXE /q:a /c:""msiexec /i vcredist.msi /qb!"" "'
    Delete $2
FunctionEnd

;-------------------------------
; Test if Visual Studio Redistributables 2005+ SP1 installed
; Returns -1 if there is no VC redistributables intstalled
Function CheckVCRedist
   Push $R0
   ClearErrors
   ReadRegDword $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{7299052b-02a4-4627-81f2-1818da5d550d}" "Version"

   ; if VS 2005+ redist SP1 not installed, install it
   IfErrors NOVSRedistInstalled VSRedistInstalled

NOVSRedistInstalled:
  DetailPrint "DONOT HAVE VC 2005 RUNTIME"
  Call InstallVC2005Sp1ReDis
  ;Abort
  
VSRedistInstalled:
  Exch $R0
  
FunctionEnd


Section
  /*xtInfoPlugin::GetLanguage
  Pop $2 ;Windows system language
  MessageBox MB_OK "OS Lang: $2" IDOK
  xtInfoPlugin::IsAdministrator
  Pop $0 ;"true" if the current user has administrator rights, otherwise it is "false"
  strcmp $0 "true" 0 notAdmin*/
  /*
  xtInfoPlugin::GetWindowsid
  Pop $1 ;Windows's version
  MessageBox MB_OK "Windows ID: $1" IDOK
  strcmp $1 "xp" 0 notXP*/
  ;Call GetIEVersion
  ;Pop $R0
  ;strcmp $R0 "7" 0 notIE7

  ;Call GetWindowsVersion
  ;pop $R1
  ;;MessageBox MB_OK "Windows version: $R1" IDOK
  ;strcmp $R1 "XP" 0 notXP
  
  
  Call CheckVCRedist
  goto _isGOOD
notIE7:
  MessageBox MB_OK "Sorry. Your are noting using IE7." IDOK
  QUIT
/*notAdmin:
  MessageBox MB_OK "Sorry, you need to have administrator priveledge to install ultraie. Thanks." IDOK
  QUIT*/
notXP:
  MessageBox MB_OK "Sorry, you operating system need to be windows XP or you can not install ultraie. Thanks" IDOK
  QUIT
_isGOOD:
SectionEnd


; GetIEVersion
;
; Based on Yazno's function, http://yazno.tripod.com/
; Updated 2007-06-02 Ch. Bergmann (Yazno)
; Returns on top of stack
; 1-7 (Installed IE Version)
; or
; '' (IE is not installed)
;
; Usage:
;   Call GetIEVersion
;   Pop $R0
;   ; at this point $R0 is "5" or whatnot

Function GetIEVersion
  Push $R0
  ClearErrors
  ReadRegStr $R0 HKLM "Software\Microsoft\Internet Explorer" "Version"
  IfErrors lbl_123 lbl_4567

  lbl_4567: ; ie 4+
    Strcpy $R0 $R0 1
  Goto lbl_done

  lbl_123: ; older ie version
    ClearErrors
    ReadRegStr $R0 HKLM "Software\Microsoft\Internet Explorer" "IVer"
    IfErrors lbl_error

      StrCpy $R0 $R0 3
        StrCmp $R0 '100' lbl_ie1
        StrCmp $R0 '101' lbl_ie2
        StrCmp $R0 '102' lbl_ie2
        StrCpy $R0 '3' ; default to ie3 if not 100, 101, or 102.
        Goto lbl_done
          lbl_ie1:
            StrCpy $R0 '1'
          Goto lbl_done
          lbl_ie2:
            StrCpy $R0 '2'
          Goto lbl_done
       lbl_error:
         StrCpy $R0 ''
   lbl_done:
   Exch $R0
FunctionEnd

Function GetWindowsVersion

  Push $R0
  Push $R1

  ClearErrors

  ReadRegStr $R0 HKLM \
  "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion

  IfErrors 0 lbl_winnt

  ; we are not NT
  ReadRegStr $R0 HKLM \
  "SOFTWARE\Microsoft\Windows\CurrentVersion" VersionNumber

  StrCpy $R1 $R0 1
  StrCmp $R1 '4' 0 lbl_error

  StrCpy $R1 $R0 3

  StrCmp $R1 '4.0' lbl_win32_95
  StrCmp $R1 '4.9' lbl_win32_ME lbl_win32_98

  lbl_win32_95:
    StrCpy $R0 '95'
  Goto lbl_done

  lbl_win32_98:
    StrCpy $R0 '98'
  Goto lbl_done

  lbl_win32_ME:
    StrCpy $R0 'ME'
  Goto lbl_done

  lbl_winnt:

  StrCpy $R1 $R0 1

  StrCmp $R1 '3' lbl_winnt_x
  StrCmp $R1 '4' lbl_winnt_x

  StrCpy $R1 $R0 3

  StrCmp $R1 '5.0' lbl_winnt_2000
  StrCmp $R1 '5.1' lbl_winnt_XP
  StrCmp $R1 '5.2' lbl_winnt_2003
  StrCmp $R1 '6.0' lbl_winnt_vista lbl_error

  lbl_winnt_x:
    StrCpy $R0 "NT $R0" 6
  Goto lbl_done

  lbl_winnt_2000:
    Strcpy $R0 '2000'
  Goto lbl_done

  lbl_winnt_XP:
    Strcpy $R0 'XP'
  Goto lbl_done

  lbl_winnt_2003:
    Strcpy $R0 '2003'
  Goto lbl_done

  lbl_winnt_vista:
    Strcpy $R0 'Vista'
  Goto lbl_done

  lbl_error:
    Strcpy $R0 ''
  lbl_done:

  Pop $R1
  Exch $R0

FunctionEnd

/*
Section
  ;Banner::show /NOUNLOAD 'checking dependencies...'
  ${Locate} "$WINDIR\WinSxs" "/L=F /M=ATL80.DLL" "LocateCallback"
  IfErrors donothavevc2005 havevc2005
  donothavevc2005:
    MessageBox MB_OK "You do not have VC 2005 Runtime installed.\
    Do you want to install?" IDOK 0
    Call InstallVC2005Sp1ReDis
    ;goto chkdepfinish
  havevc2005:
    MessageBox MB_OK "You have installed VC 2005 Runtime installed already." IDOK
  ;chkdepfinish:
  ;  Banner::destroy
SectionEnd

Function LocateCallback
	StrCpy "$0" StopLocate
	Push "$0"
FunctionEnd
*/


; Pages
;Page license
;Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

ShowInstDetails show
ShowUnInstDetails show


!include "WinMessages.nsh"

;Check earlier installation
Function DetectVersion
  ClearErrors
  ReadRegStr $0 HKLM "SOFTWARE\6BeeNetworks\UltraIE" "ToolbarVersion"
  IfErrors freshinstall
  ;MessageBox MB_OKCANCEL $0 IDOK
  ${VersionCompare} $0 $PVERSION $1
  IntCmp $1 1 installolder versionequal calluninstaller

calluninstaller:
  ClearErrors
  ReadRegStr $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${PNAME}" "UninstallString"
  ;IfFileExists "$R0" 0 +2
	Exec $R0
  goto gooutdetection

versionequal:
    MessageBox MB_YESNO "${PNAME} version $0 seems to be already installed on your system." IDYES
    Quit

installolder:
    MessageBox MB_YESNO \
      "${PNAME} version $0 seems to be already installed on your system.$\nIt is strongly recommanded not to use an older version.$\nIf you really want to do this, please uninstall version $0 firstly and then install version $PVERSION."\
      IDYES
    Quit

freshinstall:
gooutdetection:

FunctionEnd


Function .onGUIInit
  ${GetFileVersion} "${DEPLOY_ROOT}\uutoolbar.dll" $PVERSION
  Call DetectVersion
  # 1028 is the id of the branding text control
  GetDlgItem $R0 $HWNDPARENT 1028
  CreateFont $R1 "Tahoma" 8 700
  SendMessage $R0 ${WM_SETFONT} $R1 0
  # set background color to white and text color to red
  ;SetCtlColors $R0 FFFFFF FF0000
  
  ;@todo should ask user to choose
  SetShellVarContext current
  ;SetShellVarContext all

FunctionEnd



; The stuff to install
Section "UUToolBar (required)"
  IfFileExists "$INSTDIR" instdirexisting instdirnotexisting
instdirexisting:
    MessageBox MB_YESNO "${PNAME} has not been remove clearly. You may need to restart your machine to solve this problem." IDYES
		Quit

instdirnotexisting:

  SectionIn RO
  ;MessageBox MB_OKCANCEL "You have installed VC 2005 Runtime installed already." IDOK
  
  CreateDirectory $INSTDIR\Plugin
  CreateDirectory $INSTDIR\Template
  CreateDirectory $INSTDIR\Template\cn
  CreateDirectory $INSTDIR\Template\en

  ; to do
  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  ;SetOutPath $SYSDIR
  ;File "dwmapi.dll"  ; showdoc.dll need this

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File waitthumbnail.jpg
  ;File ${DEPLOY_ROOT}\6BeeClientMng.exe
  File ${DEPLOY_ROOT}\6BeeUploader.exe
  File ${DEPLOY_ROOT}\6beehtmleditor.exe
  
  ;File "ATL80.dll"
  File "${DEPLOY_ROOT}\UUToolBar.dll"
  ;File "${DEPLOY_ROOT}\6BeeClientMngHook.dll"
  File "${DEPLOY_ROOT}\6Beecommond.dll"
  File "${DEPLOY_ROOT}\builtinsearch.dll"

  ;File "${DEPLOY_ROOT}\6BeeShellExt.dll"

  File "${DEPLOY_ROOT}\6BeeRes.dll"
  File "${DEPLOY_ROOT}\6beemousehook.dll"

  File "${VENDOR_ROOT}\ice-3.3.0\bin\bzip2d.dll"
  File "${VENDOR_ROOT}\ice-3.3.0\bin\ice33d.dll"
  File "${VENDOR_ROOT}\ice-3.3.0\bin\iceutil33d.dll"
  File "${VENDOR_ROOT}\sqlite3\bin\sqlite3.dll"
  File "${VENDOR_ROOT}\pcrecpp\bin\pcred.dll"
  File "${VENDOR_ROOT}\pcrecpp\bin\pcre++d.dll"
  File "${VENDOR_ROOT}\lame-3.97\bin\lame_enc.dll"
  
  File "${DATABASE_ROOT}\main.db"
  
  File "${TEMPLATE_ROOT}\uta.ico"
  
  SetOutPath $INSTDIR\Template\cn
  File "${TEMPLATE_ROOT}\base.css"
  File "${TEMPLATE_ROOT}\cancel.png"
  File "${TEMPLATE_ROOT}\colorpicker.js"
  File "${TEMPLATE_ROOT}\delete.png"
  File "${TEMPLATE_ROOT}\error.html"
  File "${TEMPLATE_ROOT}\error.png"
  File "${TEMPLATE_ROOT}\generalsetting.html"
  File "${TEMPLATE_ROOT}\bg.gif"
  File "${TEMPLATE_ROOT}\htm.gif"
  File "${TEMPLATE_ROOT}\loading.gif"
  File "${TEMPLATE_ROOT}\folder.gif"
  
  CreateDirectory $INSTDIR\Plugin\alexa
  SetOutPath $INSTDIR\Plugin\alexa
  File "${DEPLOY_ROOT}\plugin\alexa\alexa.uta"
  
  CreateDirectory $INSTDIR\Plugin\ggpr
  SetOutPath $INSTDIR\Plugin\ggpr
  File "${DEPLOY_ROOT}\plugin\ggpr\ggpr.uta"
  
  CreateDirectory $INSTDIR\Plugin\imgcapture
  SetOutPath $INSTDIR\Plugin\imgcapture
  File "${DEPLOY_ROOT}\plugin\imgcapture\imgcapture.uta"
  File "${DATABASE_ROOT}\imgcapture.db"
  File "${TEMPLATE_ROOT}\imgcapsetting.html"
  
  CreateDirectory $INSTDIR\Plugin\smarthistory
  SetOutPath $INSTDIR\Plugin\smarthistory
  File "${DEPLOY_ROOT}\plugin\smarthistory\smarthistory.uta"
  File "${DATABASE_ROOT}\smarthistory.db"
  File "${TEMPLATE_ROOT}\smarthistory.html"
  
  CreateDirectory $INSTDIR\Plugin\onlinebookmark
  SetOutPath $INSTDIR\Plugin\onlinebookmark
  File "${DEPLOY_ROOT}\plugin\onlinebookmark\onlinebookmark.uta"
  File "${TEMPLATE_ROOT}\olbmsetting.html"
  File "${TEMPLATE_ROOT}\olbmupload.html"
  File "${TEMPLATE_ROOT}\do_upload.html"
  File "${TEMPLATE_ROOT}\do_download.html"
  File "${TEMPLATE_ROOT}\olbmdownload.html"
  File "${TEMPLATE_ROOT}\olbmfinished.html"
  File "${TEMPLATE_ROOT}\olbmloading.html"

  CreateDirectory $INSTDIR\Plugin\webaudio
  SetOutPath $INSTDIR\Plugin\webaudio
  File "${DEPLOY_ROOT}\plugin\webaudio\webaudio.uta"
  
  CreateDirectory $INSTDIR\Plugin\webclip
  SetOutPath $INSTDIR\Plugin\webclip
  File "${DEPLOY_ROOT}\plugin\webclip\webclip.uta"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UltraIE" "DisplayName" "6BeeNetworks UltraIE"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UltraIE" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UltraIE" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UltraIE" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

  Call WriteUltraIEReg
  RegDLL "$INSTDIR\UUToolBar.dll"
  RegDLL "$INSTDIR\builtinsearch.dll"
  RegDLL "$INSTDIR\6BeeShellExt.dll"

SectionEnd


; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"
  CreateDirectory "$SMPROGRAMS\UltraIE"
  CreateShortCut "$SMPROGRAMS\UltraIE\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
SectionEnd

; Uninstaller
Section "Uninstall"
  UnRegDLL "$INSTDIR\UUToolBar.dll"
  UnRegDLL "$INSTDIR\6BeeShellExt.dll"
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UltraIE"
  ;DeleteRegKey HKLM SOFTWARE\6BeeNetworks
  DeleteRegKey HKLM SOFTWARE\6BeeNetworks\UltraIE
  Call un.UltraIEReg
  
  ; Remove files and uninstaller
  ; Give an option
  ;Delete $INSTDIR\Cache\*
  ;Delete $INSTDIR\Picture\*
  ;Delete $INSTDIR\Template\*
  Delete $INSTDIR\Plugin\*
  Delete $INSTDIR\*
  Delete $SMPROGRAMS\UltraIE\Uninstall.lnk
  Delete $SMPROGRAMS\UltraIE
  
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\UltraIE\*.*"
  ; Remove directories used
  RMDir "$SMPROGRAMS\UltraIE"
  RMDir /r /REBOOTOK "$INSTDIR"
SectionEnd

Function WriteUltraIEReg
WriteRegStr HKLM SOFTWARE\6BeeNetworks\UltraIE "Location" "$INSTDIR\" ; must have a back slash
WriteRegStr HKLM SOFTWARE\6BeeNetworks\UltraIE "ToolbarVersion" "$PVERSION"
WriteRegStr HKLM SOFTWARE\6BeeNetworks\UltraIE "DefaultSE" "Google"
WriteRegStr HKLM SOFTWARE\6BeeNetworks\UltraIE\Disabled "" ""

WriteRegStr HKCR "UUToolBar.UUFilter.1" "" "UUFilter"
WriteRegStr HKCR "UUToolBar.UUFilter.1\CLSID" "" "{4621B910-2D7B-45AA-971C-E8CB5F890495}"
WriteRegStr HKLM "SOFTWARE\Classes\*\shellex\ContextMenuHandlers\6BeeShellExt" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
WriteRegStr HKLM "SOFTWARE\Classes\CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}" "" "UUShellExt Class"
WriteRegStr HKLM "SOFTWARE\Classes\CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}" "AppID" "{1CFAE4B0-1A46-409F-B0A7-B12059134809}"
WriteRegStr HKLM "SOFTWARE\Classes\CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}\InprocServer32" "ThreadingModel" "Apartment"
WriteRegStr HKLM "SOFTWARE\Classes\CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}\InprocServer32" "" "$INSTDIR\6BeeShellExt.dll"
WriteRegStr HKLM "SOFTWARE\Classes\CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}\ProgID" "" "My6BeeShellExt.UUShellExt.1"
WriteRegStr HKLM "SOFTWARE\Classes\CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}\TypeLib" "" "{68849F1B-EC6F-484C-AF6D-35768F422412}"
WriteRegStr HKLM "SOFTWARE\Classes\CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}\VersionIndependentProgID" "" "My6BeeShellExt.UUShellExt"
;WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "6BeeClientMng" "$INSTDIR\6BeeClientMng.exe"

WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings\5.0\User Agent\Post Platform" "UltraIE(6BEENETWORKS 2009)" ""
WriteRegStr HKLM "SOFTWARE\Classes\Directory\Background\shellex\ContextMenuHandlers\6BeeShellExt" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
WriteRegStr HKLM "SOFTWARE\Classes\Directory\shellex\ContextMenuHandlers\6BeeShellExt" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
WriteRegStr HKLM "SOFTWARE\Classes\My6BeeShellExt.UUShellExt" "" "UUShellExt Class"
WriteRegStr HKLM "SOFTWARE\Classes\My6BeeShellExt.UUShellExt\CLSID" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
WriteRegStr HKLM "SOFTWARE\Classes\My6BeeShellExt.UUShellExt\CurVer" "" "My6BeeShellExt.UUShellExt.1"
WriteRegStr HKLM "SOFTWARE\Classes\My6BeeShellExt.UUShellExt.1" "" "UUShellExt Class"
WriteRegStr HKLM "SOFTWARE\Classes\My6BeeShellExt.UUShellExt.1\CLSID" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"

WriteRegStr HKCR "*\shellex\ContextMenuHandlers\6BeeShellExt" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
WriteRegStr HKCR "AppID\6BeeShellExt.DLL" "AppID" "{1CFAE4B0-1A46-409F-B0A7-B12059134809}"
WriteRegStr HKCR "CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}" "" "UUShellExt Class"
WriteRegStr HKCR "CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}" "AppID" "{1CFAE4B0-1A46-409F-B0A7-B12059134809}"
WriteRegStr HKCR "CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}\InprocServer32" "ThreadingModel" "Apartment"
WriteRegStr HKCR "CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}\InprocServer32" "" "$INSTDIR\6BeeShellExt.dll"
WriteRegStr HKCR "CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}\ProgID" "" "My6BeeShellExt.UUShellExt.1"
WriteRegStr HKCR "CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}\TypeLib" "" "{68849F1B-EC6F-484C-AF6D-35768F422412}"
WriteRegStr HKCR "CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}\VersionIndependentProgID" "" "My6BeeShellExt.UUShellExt"
WriteRegStr HKCR "Directory\Background\shellex\ContextMenuHandlers\6BeeShellExt" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
WriteRegStr HKCR "Directory\shellex\ContextMenuHandlers\6BeeShellExt" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
WriteRegStr HKCR "My6BeeShellExt.UUShellExt" "" "UUShellExt Class"
WriteRegStr HKCR "My6BeeShellExt.UUShellExt\CLSID" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
WriteRegStr HKCR "My6BeeShellExt.UUShellExt\CurVer" "" "My6BeeShellExt.UUShellExt.1"
WriteRegStr HKCR "My6BeeShellExt.UUShellExt.1" "" "UUShellExt Class"
WriteRegStr HKCR "My6BeeShellExt.UUShellExt.1\CLSID" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
WriteRegStr HKCU "Software\Microsoft\Internet Explorer\TabbedBrowsing" "UseMRUSwitching" "no"
WriteRegDWORD HKCU "Software\Microsoft\Internet Explorer\TabbedBrowsing" "WarnOnClose" 0x0
WriteRegStr HKCU "Software\Microsoft\Internet Explorer\TabbedBrowsing" "" ""
WriteRegDWORD HKCU "Software\Microsoft\Internet Explorer\TabbedBrowsing" "OpenAdjacent" 0x1
WriteRegDWORD HKCU "Software\Microsoft\Internet Explorer\TabbedBrowsing" "Enabled" 0x1
WriteRegDWORD HKCU "Software\Microsoft\Internet Explorer\TabbedBrowsing" "UseHomepageForNewTab" 0x0
WriteRegDWORD HKCU "Software\Microsoft\Internet Explorer\TabbedBrowsing" "WarnOnCloseAdvanced" 0x1
WriteRegDWORD HKCU "Software\Microsoft\Internet Explorer\TabbedBrowsing" "OpenInForeground" 0x0
WriteRegDWORD HKCU "Software\Microsoft\Internet Explorer\TabbedBrowsing" "ShortcutBehavior" 0x1
WriteRegDWORD HKCU "Software\Microsoft\Internet Explorer\TabbedBrowsing" "OpenAllHomePages" 0x1
WriteRegDWORD HKCU "Software\Microsoft\Internet Explorer\TabbedBrowsing" "QuickTabsThreshold" 0x2
WriteRegDWORD HKCU "Software\Microsoft\Internet Explorer\TabbedBrowsing" "PopupsUseNewWindow" 0x2

; uta file association
WriteRegStr HKCR ".uta" "" "utafile"
WriteRegStr HKCR "utafile" "" ""
WriteRegStr HKCR "utafile\DefaultIcon" "" "$INSTDIR\uta.ico"
FunctionEnd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Function un.UltraIEReg
DeleteRegValue HKCU "Software\Microsoft\Internet Explorer\Explorer Bars\{92268371-AE67-4B85-BF60-157E57009ECC}" "BarSize"
DeleteRegKey HKCR "builtinsearch.horizontalbar"
DeleteRegKey HKLM "SOFTWARE\Classes\CLSID\{92268371-AE67-4B85-BF60-157E57009ECC}"
DeleteRegKey HKLM "SOFTWARE\Classes\builtinsearch.horizontalbar.1"
DeleteRegKey HKCR "builtinsearch.horizontalbar.1"
DeleteRegKey HKLM "SOFTWARE\Classes\builtinsearch.horizontalbar"
DeleteRegKey HKCR "CLSID\{92268371-AE67-4B85-BF60-157E57009ECC}"

DeleteRegValue HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings\5.0\User Agent\Post Platform" "UltraIE(6BEENETWORKS 2009)"
;DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "6BeeClientMng"
DeleteRegKey HKCR "UUToolBar.UUFilter.1"
DeleteRegKey HKLM "SOFTWARE\Classes\*\shellex\ContextMenuHandlers\6BeeShellExt"
DeleteRegKey HKLM "SOFTWARE\Classes\CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}"
DeleteRegKey HKLM "SOFTWARE\Classes\Directory\Background\shellex\ContextMenuHandlers\6BeeShellExt"
DeleteRegKey HKLM "SOFTWARE\Classes\Directory\shellex\ContextMenuHandlers\6BeeShellExt"
DeleteRegKey HKLM "SOFTWARE\Classes\My6BeeShellExt.UUShellExt"
DeleteRegKey HKLM "SOFTWARE\Classes\My6BeeShellExt.UUShellExt.1"
DeleteRegKey HKCR "*\shellex\ContextMenuHandlers\6BeeShellExt"
DeleteRegKey HKCR "AppID\6BeeShellExt.DLL"
DeleteRegKey HKCR "Directory\Background\shellex\ContextMenuHandlers\6BeeShellExt"
DeleteRegKey HKCR "Directory\shellex\ContextMenuHandlers\6BeeShellExt"
DeleteRegKey HKCR "My6BeeShellExt.UUShellExt"
DeleteRegKey HKCR "My6BeeShellExt.UUShellExt.1"
DeleteRegKey HKCR "PROTOCOLS\Handler\ultra"
DeleteRegKey HKLM "SOFTWARE\Classes\UUToolBar.ToolBarCOM.1"
DeleteRegKey HKLM "SOFTWARE\Classes\UUToolBar.ToolBarCOM"
DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects\{4621B910-2D7B-45AA-971C-E8CB5F890495}"
DeleteRegKey HKLM "SOFTWARE\Classes\UUToolBar.UUFilter"
DeleteRegKey HKLM "SOFTWARE\Classes\UUToolBar.UUFilter.1"
DeleteRegKey HKLM "SOFTWARE\Classes\UUToolBar.UltraProtocol.1"
DeleteRegKey HKLM "SOFTWARE\Classes\UUToolBar.UltraProtocol"
DeleteRegKey HKLM "SOFTWARE\Classes\TypeLib\{9B91D4F3-E5E5-4624-AC75-14E3271E6891}"
DeleteRegKey HKCR "AppID\UUToolBar.DLL"
DeleteRegKey HKCR "CLSID\{0E1F40A1-4D93-435A-A8C5-273FE2ECF99C}"
DeleteRegKey HKCR "CLSID\{4621B910-2D7B-45AA-971C-E8CB5F890495}"
DeleteRegKey HKCR "CLSID\{4D8A34F0-CD29-4314-ADBF-812B4AB06AC6}"
DeleteRegKey HKCR "CLSID\{F33275F5-793B-46B4-892E-5CF86B5E289D}"
DeleteRegKey HKCR "UUToolBar.ToolBarCOM"
DeleteRegKey HKCR "UUToolBar.ToolBarCOM.1"
DeleteRegKey HKCR "UUToolBar.UltraProtocol"
DeleteRegKey HKCR "UUToolBar.UltraProtocol.1"
DeleteRegKey HKCR "UUToolBar.UUFilter"
FunctionEnd
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

