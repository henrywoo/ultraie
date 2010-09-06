;-----------------------------------------------------------------------------
; installation file for ultraie
; 2008 wufuheng@gmail.com
;-----------------------------------------------------------------------------
!include "FileFunc.nsh"
!insertmacro GetFileVersion
!include "WordFunc.nsh"
;!include "mui2.nsh"
;!include "nsDialogs.nsh"

!insertmacro mui_language "simpchinese"
!insertmacro VersionCompare

;-----------------------------------------------------------------------------
; const definition
;-----------------------------------------------------------------------------
!define PRODUCT_WEB_SITE "http://www.uufile.com"
; The root can be changed if necessary
!define ROOT_            "H:\6beenetworks\client\uutoolbar"
!define DATABASE_ROOT    "${ROOT_}\db"
!define DEPLOY_ROOT      "${ROOT_}\bin\release"
!define VENDOR_ROOT      "${ROOT_}\vendor"
!define PNAME            "UUFile"

Name          "悠悠快传 - UUFile文件上传工具"
Caption       "悠悠快传安装文件"
Icon          "install.ico"
UninstallIcon "uninstall.ico"
OutFile       "UUFile.exe"

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
InstallDir $PROGRAMFILES\6BeeNetworks\UUFile
BrandingText    ${PRODUCT_WEB_SITE}
InstallDirRegKey HKLM "Software\6BeeNetworks\UUFile" "Location"
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
  ReadRegStr $0 HKLM "SOFTWARE\6BeeNetworks\UUFile" "Version"
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
  ${GetFileVersion} "${DEPLOY_ROOT}\uufile_uploader.exe" $PVERSION
  ;MessageBox MB_OK $PVERSION IDOK
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
    WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "UUFILE INSTALLER" "$EXEPATH"
    MessageBox MB_YESNO|MB_ICONQUESTION "安装程序检测到${PNAME}旧版本的文件还未删除.$\n$\n为了完全删除旧版本的所有程序文件,你可以手动删除目录$INSTDIR或者重启电脑.$\n$\n您现在要重启电脑吗?" IDNO +2
        Reboot
		Quit

instdirnotexisting:
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "UUFILE INSTALLER"
  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  File ${DEPLOY_ROOT}\uufile_uploader.exe
  File "${DEPLOY_ROOT}\uufile_shellext.dll"
  File "${DEPLOY_ROOT}\6beeres.dll"
  
  SetOutPath $FAVORITES
  ;File "${DATABASE_ROOT}\悠悠花园开放注册.url"
  ;File "${DATABASE_ROOT}\UUFile.COM - 悠悠空间 您的免费文件托管地 -无限空间免费网盘- 支持任何文件类型.url"
  
  SetOutPath $DESKTOP
  ;File "${DATABASE_ROOT}\UUFile.COM - 悠悠空间 您的免费文件托管地 -无限空间免费网盘- 支持任何文件类型.url"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UUFile" "DisplayName" "6BeeNetworks UUFile"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UUFile" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UUFile" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UUFile" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

  Call WriteUUFileReg
  ;RegDLL "$INSTDIR\UUToolBar.dll"
  ;RegDLL "$INSTDIR\builtinsearch.dll"
  
  RegDLL "$INSTDIR\uufile_shellext.dll"

SectionEnd


; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"
  ;Add to start menu
  CreateDirectory "$SMPROGRAMS\悠悠快传"
  CreateShortCut "$SMPROGRAMS\悠悠快传\启动悠悠快传.lnk" "$INSTDIR\uufile_uploader.exe" "" "$INSTDIR\uufile_uploader.exe" 0
  CreateShortCut "$SMPROGRAMS\悠悠快传\UU快传网络版.lnk" "${PRODUCT_WEB_SITE}/upload.php" "" "$PROGRAMFILES\Internet Explorer\iexplore.exe" 0
  CreateShortCut "$SMPROGRAMS\悠悠快传\卸载.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  
  ;Add to desktop
  CreateShortCut "$DESKTOP\悠悠快传.lnk" "$INSTDIR\uufile_uploader.exe" "" "$INSTDIR\uufile_uploader.exe" 0
  
  ;Favorite
  ;CreateShortCut "$FAVORITES\UUFile\悠悠花园 开放注册.lnk" "$INSTDIR\悠悠花园开放注册.url" "" "$PROGRAMFILES\Internet Explorer\iexplore.exe" 0
  CreateShortCut "$QUICKLAUNCH\悠悠快传.lnk" "$INSTDIR\uufile_uploader.exe" "" "$INSTDIR\uufile_uploader.exe" 0
  
SectionEnd

; Uninstaller
Section "Uninstall"
  UnRegDLL "$INSTDIR\uufile_shellext.dll"
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\UUFile"
  DeleteRegKey HKLM SOFTWARE\6BeeNetworks\UUFile
  Call un.uufileReg
  
  ; Remove files and uninstaller
  ; Give an option
  Delete "$INSTDIR\*"
  Delete "$SMPROGRAMS\悠悠快传\*"
  Delete "$SMPROGRAMS\悠悠快传"
  Delete "$DESKTOP\悠悠快传.lnk"
  Delete "$QUICKLAUNCH\悠悠快传.lnk"
  
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\悠悠快传\*.*"
  ; Remove directories used
  RMDir "$SMPROGRAMS\悠悠快传"
  RMDir /r /REBOOTOK "$INSTDIR"
SectionEnd

Function WriteuufileReg
WriteRegStr HKLM SOFTWARE\6BeeNetworks\UUFile "Location" "$INSTDIR\" ; must have a back slash
WriteRegStr HKLM SOFTWARE\6BeeNetworks\UUFile "Version" "$PVERSION"
WriteRegStr HKLM "SOFTWARE\Classes\*\shellex\ContextMenuHandlers\uufile_shellext" "" "{6B12D717-7E95-4C50-8631-97AB1F213F6A}"
WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings\5.0\User Agent\Post Platform" "UUFile" ""
WriteRegStr HKLM "SOFTWARE\Classes\Directory\Background\shellex\ContextMenuHandlers\uufile_shellext" "" "{6B12D717-7E95-4C50-8631-97AB1F213F6A}"
WriteRegStr HKLM "SOFTWARE\Classes\Directory\shellex\ContextMenuHandlers\uufile_shellext" "" "{6B12D717-7E95-4C50-8631-97AB1F213F6A}"
WriteRegStr HKCR "*\shellex\ContextMenuHandlers\uufile_shellext" "" "{6B12D717-7E95-4C50-8631-97AB1F213F6A}"
WriteRegStr HKCR "Directory\Background\shellex\ContextMenuHandlers\uufile_shellext" "" "{6B12D717-7E95-4C50-8631-97AB1F213F6A}"
WriteRegStr HKCR "Directory\shellex\ContextMenuHandlers\uufile_shellext" "" "{6B12D717-7E95-4C50-8631-97AB1F213F6A}"
FunctionEnd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Function un.uufileReg
DeleteRegValue HKCU "Software\Microsoft\Internet Explorer\Explorer Bars\{92268371-AE67-4B85-BF60-157E57009ECC}" "BarSize"
DeleteRegValue HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings\5.0\User Agent\Post Platform" "UUFile"
DeleteRegKey HKLM "SOFTWARE\Classes\*\shellex\ContextMenuHandlers\uufile_shellext"
DeleteRegKey HKLM "SOFTWARE\Classes\Directory\Background\shellex\ContextMenuHandlers\uufile_shellext"
DeleteRegKey HKLM "SOFTWARE\Classes\Directory\shellex\ContextMenuHandlers\uufile_shellext"
DeleteRegKey HKCR "*\shellex\ContextMenuHandlers\uufile_shellext"
DeleteRegKey HKCR "Directory\Background\shellex\ContextMenuHandlers\uufile_shellext"
DeleteRegKey HKCR "Directory\shellex\ContextMenuHandlers\uufile_shellext"
FunctionEnd
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
