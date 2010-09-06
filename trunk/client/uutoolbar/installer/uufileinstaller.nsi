;--------------------------------
;Include Modern UI
!include "MUI2.nsh"
!include "FileFunc.nsh"
!include "WordFunc.nsh"
!insertmacro GetFileVersion
!insertmacro VersionCompare
;--------------------------------
;General

;-----------------------------------------------------------------------------
; const definition
;-----------------------------------------------------------------------------
!define PRODUCT_WEB_SITE "http://www.6bee.net"
; The root can be changed if necessary
!define ROOT_            "F:\6beenetworks\client\uutoolbar"
!define DATABASE_ROOT    "${ROOT_}\db"
;!define DEPLOY_ROOT      "${ROOT_}\bin\release"
!define DEPLOY_ROOT      "${ROOT_}\bin\debug"
!define VENDOR_ROOT      "${ROOT_}\vendor"
!define PNAME            "6beeuploader"

;Name and file
Name          "牛比快传 - 6beeuploader文件上传工具"
Caption       "牛比快传安装文件"
Icon          "install.ico"
UninstallIcon "uninstall.ico"
OutFile       "6bee-uploader-installer.exe"

SetDateSave          on
SetDatablockOptimize on
CRCCheck             on
SetCompressor /SOLID lzma
SilentInstall        normal
BGGradient 000000 803000 FFFFFF
InstallColors FF8080 000030
XPStyle              on
AllowRootDirInstall  true

InstProgressFlags    smooth
;Default installation folder
InstallDir $PROGRAMFILES\6BeeNetworks\6beeuploader
BrandingText    ${PRODUCT_WEB_SITE}
InstallDirRegKey HKLM "Software\6BeeNetworks\6beeuploader" "Location"
CheckBitmap "modern.bmp"
;LicenseText "Terms and License"
;LicenseData "${ROOT_}\installer\license.txt"

;Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------
;Interface Settings

!define MUI_ABORTWARNING

Var PVERSION ;
;--------------------------------
;Language Selection Dialog Settings

;--------------------------------
;Pages

;!insertmacro MUI_PAGE_LICENSE "${ROOT_}\installer\license.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages

!insertmacro MUI_LANGUAGE "SimpChinese"

;--------------------------------
;Reserve Files


!insertmacro MUI_RESERVEFILE_LANGDLL


; 函数

!include "WinMessages.nsh"

;Check earlier installation
Function DetectVersion
  ClearErrors
  ReadRegStr $0 HKLM "SOFTWARE\6BeeNetworks\6beeuploader" "Version"
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
    MessageBox MB_YESNO "您已经安装了${PNAME}(版本$0)." IDYES
    Quit

installolder:
    MessageBox MB_YESNO \
      "${PNAME}(版本$0)已经在装在您的系统了.$\n我们强烈推荐您使用最新版本.$\n如果您真的要安装旧的版本，请卸载新版本 $0 再安装旧版本 $PVERSION."\
      IDYES
    Quit

freshinstall:
gooutdetection:

FunctionEnd
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


Function WriteuufileReg
  WriteRegStr HKLM SOFTWARE\6BeeNetworks\6beeuploader "Location" "$INSTDIR\" ; must have a back slash
  WriteRegStr HKLM SOFTWARE\6BeeNetworks\6beeuploader "Version" "$PVERSION"
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings\5.0\User Agent\Post Platform" "6beeuploader" ""
FunctionEnd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Function un.uufileReg
DeleteRegValue HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings\5.0\User Agent\Post Platform" "6beeuploader"
DeleteRegKey HKLM "SOFTWARE\Classes\*\shellex\ContextMenuHandlers\6bee_shellext"
DeleteRegKey HKLM "SOFTWARE\Classes\Directory\Background\shellex\ContextMenuHandlers\6bee_shellext"
DeleteRegKey HKLM "SOFTWARE\Classes\Directory\shellex\ContextMenuHandlers\6bee_shellext"
DeleteRegKey HKCR "*\shellex\ContextMenuHandlers\6bee_shellext"
DeleteRegKey HKCR "Directory\Background\shellex\ContextMenuHandlers\6bee_shellext"
DeleteRegKey HKCR "Directory\shellex\ContextMenuHandlers\6bee_shellext"
FunctionEnd
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;--------------------------------
;Installer Sections

Section "牛比快传工具" UUFileTool
  delete "$INSTDIR\*"
  delete "$INSTDIR\*.*"
  rmdir "$INSTDIR"
  IfFileExists "$INSTDIR" instdirexisting instdirnotexisting
instdirexisting:
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "6beeuploader INSTALLER" '"$EXEPATH"'
  MessageBox MB_YESNO|MB_ICONQUESTION "安装程序检测到${PNAME}旧版本的文件还未删除.$\n$\n为了完全删除旧版本的所有程序文件,你可以手动删除目录$INSTDIR或者重启电脑.$\n$\n您现在要重启电脑吗?" IDNO +2
    Reboot
	Quit

instdirnotexisting:
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "6beeuploader INSTALLER"
  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  File "${DEPLOY_ROOT}\6bee_uploader.exe"
  File "${DEPLOY_ROOT}\6beeres.dll"
  
  SetOutPath $FAVORITES
  ;File "${DATABASE_ROOT}\悠悠花园开放注册.url"
  
  SetOutPath $DESKTOP
  ;File "${DATABASE_ROOT}\悠悠花园开放注册.url"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\6beeuploader" "DisplayName" "6BeeNetworks 6beeuploader"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\6beeuploader" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\6beeuploader" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\6beeuploader" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

  Call WriteUUFileReg

  ;Add to start menu
  CreateDirectory "$SMPROGRAMS\牛比快传"
  CreateShortCut "$SMPROGRAMS\牛比快传\启动牛比快传.lnk" "$INSTDIR\6bee_uploader.exe" "" "$INSTDIR\6bee_uploader.exe" 0
  CreateShortCut "$SMPROGRAMS\牛比快传\UU快传网络版.lnk" "${PRODUCT_WEB_SITE}/upload.php" "" "$PROGRAMFILES\Internet Explorer\iexplore.exe" 0
  CreateShortCut "$SMPROGRAMS\牛比快传\卸载.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  
  ;Add to desktop
  CreateShortCut "$DESKTOP\牛比快传.lnk" "$INSTDIR\6bee_uploader.exe" "" "$INSTDIR\6bee_uploader.exe" 0
  
  ;Favorite
  ;CreateShortCut "$FAVORITES\UUFile\悠悠花园 开放注册.lnk" "$INSTDIR\悠悠花园开放注册.url" "" "$PROGRAMFILES\Internet Explorer\iexplore.exe" 0
  CreateShortCut "$QUICKLAUNCH\牛比快传.lnk" "$INSTDIR\6bee_uploader.exe" "" "$INSTDIR\6bee_uploader.exe" 0

SectionEnd

Section "右键菜单集成" UUShellExt
  SetOutPath $INSTDIR
  File "${DEPLOY_ROOT}\6bee_shellext.dll"
  RegDLL "$INSTDIR\6bee_shellext.dll"
  WriteRegStr HKLM "SOFTWARE\Classes\*\shellex\ContextMenuHandlers\6bee_shellext" "" "{6B12D717-7E95-4C50-8631-97AB1F213F6A}"
  WriteRegStr HKLM "SOFTWARE\Classes\Directory\Background\shellex\ContextMenuHandlers\6bee_shellext" "" "{6B12D717-7E95-4C50-8631-97AB1F213F6A}"
  WriteRegStr HKLM "SOFTWARE\Classes\Directory\shellex\ContextMenuHandlers\6bee_shellext" "" "{6B12D717-7E95-4C50-8631-97AB1F213F6A}"
  WriteRegStr HKCR "*\shellex\ContextMenuHandlers\6bee_shellext" "" "{6B12D717-7E95-4C50-8631-97AB1F213F6A}"
  WriteRegStr HKCR "Directory\Background\shellex\ContextMenuHandlers\6bee_shellext" "" "{6B12D717-7E95-4C50-8631-97AB1F213F6A}"
  WriteRegStr HKCR "Directory\shellex\ContextMenuHandlers\6bee_shellext" "" "{6B12D717-7E95-4C50-8631-97AB1F213F6A}"
SectionEnd
;--------------------------------
;Installer Functions

Function .onInit
  # 1028 is the id of the branding text control
  GetDlgItem $R0 $HWNDPARENT 1028
  CreateFont $R1 "Tahoma" 8 700
  SendMessage $R0 ${WM_SETFONT} $R1 0
  # set background color to white and text color to red
  ;SetCtlColors $R0 FFFFFF FF0000
  
  ;@todo should ask user to choose
  SetShellVarContext current
  ;SetShellVarContext all
  ${GetFileVersion} "${DEPLOY_ROOT}\6bee_uploader.exe" $PVERSION
  ;MessageBox MB_OK $PVERSION IDOK
  Call DetectVersion
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

;--------------------------------
;Descriptions

;USE A LANGUAGE STRING IF YOU WANT YOUR DESCRIPTIONS TO BE LANGAUGE SPECIFIC

;Assign descriptions to sections
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${UUFileTool} "6bee.net提供的文件上传工具,支持多文件断点续传!"
!insertmacro MUI_DESCRIPTION_TEXT ${UUShellExt} "集成牛比快传到操作系统,这样通过右击文件可以直接上传文件,更加方便快捷!"
!insertmacro MUI_FUNCTION_DESCRIPTION_END


;--------------------------------
;Uninstaller Section

Section "Uninstall"

  UnRegDLL "$INSTDIR\6bee_shellext.dll"
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\6beeuploader"
  DeleteRegKey HKLM SOFTWARE\6BeeNetworks\6beeuploader
  Call un.uufileReg
  
  ; Remove files and uninstaller
  ; Give an option
  Delete "$INSTDIR\*"
  Delete "$INSTDIR\*.*"
  Delete "$SMPROGRAMS\牛比快传\*"
  Delete "$SMPROGRAMS\牛比快传"
  Delete "$DESKTOP\牛比快传.lnk"
  Delete "$QUICKLAUNCH\牛比快传.lnk"
  
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\牛比快传\*.*"
  ; Remove directories used
  RMDir "$SMPROGRAMS\牛比快传"
  ;RMDir /r /REBOOTOK "$INSTDIR"
  RMDir "$INSTDIR"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

!insertmacro MUI_UNGETLANGUAGE

FunctionEnd
