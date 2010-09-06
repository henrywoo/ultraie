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
!define TEMPLATE_ROOT    "${ROOT_}\template\cn"
!define DATABASE_ROOT    "${ROOT_}\db"
!define DEPLOY_ROOT      "${ROOT_}\bin\debug"
!define VENDOR_ROOT      "${ROOT_}\vendor"
!define PNAME            "6BeeToolKit"

;Name and file
Name          "��ֻ�۷����"
Caption       "��ֻ�۷������װ�ļ�"
Icon          "install.ico"
UninstallIcon "uninstall.ico"
OutFile       "6bee-setup.exe"

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
InstallDir $PROGRAMFILES\6BeeNetworks\6Bee
BrandingText    ${PRODUCT_WEB_SITE}
InstallDirRegKey HKLM "Software\6BeeNetworks\6Bee" "Location"
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


; ����

!include "WinMessages.nsh"

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
    DetailPrint "����ʧ��: $0"
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
  DetailPrint "���Ȱ�װVC2005ʵʱ���п�"
  Call InstallVC2005Sp1ReDis
  ;Abort
VSRedistInstalled:
  Exch $R0
FunctionEnd

;Check earlier installation
Function DetectVersion
  ClearErrors
  ReadRegStr $0 HKLM "SOFTWARE\6BeeNetworks\6Bee" "Version"
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
    MessageBox MB_YESNO "���Ѿ���װ��${PNAME}(�汾$0)." IDYES
    Quit

installolder:
    MessageBox MB_YESNO \
      "${PNAME}(�汾$0)�Ѿ���װ������ϵͳ��.$\n����ǿ���Ƽ���ʹ�����°汾.$\n��������Ҫ��װ�ɵİ汾����ж���°汾 $0 �ٰ�װ�ɰ汾 $PVERSION."\
      IDYES
    Quit

freshinstall:
gooutdetection:

FunctionEnd
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


Function Write6BeeReg
  WriteRegStr HKLM SOFTWARE\6BeeNetworks\6Bee "Location" "$INSTDIR\" ; must have a back slash
  WriteRegStr HKLM SOFTWARE\6BeeNetworks\6Bee "Version" "$PVERSION"
  WriteRegStr HKLM SOFTWARE\6BeeNetworks\6Bee\Disabled "" ""
  WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings\5.0\User Agent\Post Platform" "6Bee" ""
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PNAME}" "DisplayName" "6BeeNetworks"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PNAME}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PNAME}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PNAME}" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
FunctionEnd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Function un.6BeeReg
DeleteRegValue HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings\5.0\User Agent\Post Platform" "6Bee"
DeleteRegKey HKLM "SOFTWARE\Classes\*\shellex\ContextMenuHandlers\6beeshellext"
DeleteRegKey HKLM "SOFTWARE\Classes\Directory\Background\shellex\ContextMenuHandlers\6beeshellext"
DeleteRegKey HKLM "SOFTWARE\Classes\Directory\shellex\ContextMenuHandlers\6beeshellext"
DeleteRegKey HKCR "*\shellex\ContextMenuHandlers\6beeshellext"
DeleteRegKey HKCR "Directory\Background\shellex\ContextMenuHandlers\6beeshellext"
DeleteRegKey HKCR "Directory\shellex\ContextMenuHandlers\6beeshellext"
FunctionEnd
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;--------------------------------
;Installer Sections

Section "��ֻ�۷乤��" 6BeeTool
  delete "$INSTDIR\*"
  delete "$INSTDIR\*.*"
  delete "$INSTDIR"
  RMDir /r "$INSTDIR"
  IfFileExists "$INSTDIR" instdirexisting instdirnotexisting
instdirexisting:
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "6BeeInstaller" '"$EXEPATH"'
  MessageBox MB_YESNO|MB_ICONQUESTION "��װ�����⵽${PNAME}�ɰ汾���ļ���δɾ��.$\n$\nΪ����ȫɾ���ɰ汾�����г����ļ�,������ֶ�ɾ��Ŀ¼$INSTDIR������������.$\n$\n������Ҫ����������?" IDNO +2
    Reboot
	Quit

instdirnotexisting:
  DeleteRegValue HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "6BeeInstaller"
  SectionIn RO
  
  CreateDirectory $INSTDIR\Plugin
  CreateDirectory $INSTDIR\Template
  CreateDirectory $INSTDIR\Template\cn
  CreateDirectory $INSTDIR\Template\en

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

  File "${DEPLOY_ROOT}\6beeres_ch.dll"
  File "${DEPLOY_ROOT}\6beemousehook.dll"
/*
  File "${VENDOR_ROOT}\ice-3.3.0\bin\bzip2.dll"
  File "${VENDOR_ROOT}\ice-3.3.0\bin\ice33.dll"
  File "${VENDOR_ROOT}\ice-3.3.0\bin\iceutil33.dll"
  File "${VENDOR_ROOT}\sqlite3\bin\sqlite3.dll"
  File "${VENDOR_ROOT}\pcrecpp\bin\pcre.dll"
  File "${VENDOR_ROOT}\pcrecpp\bin\pcre++.dll"
*/
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

  Call Write6BeeReg

  ;Add to start menu
  CreateDirectory "$SMPROGRAMS\��ֻ�۷�"
  ;CreateShortCut "$SMPROGRAMS\��ֻ�۷�\������ֻ�۷�.lnk" "$INSTDIR\6beeuploader.exe" "-m upload" "$INSTDIR\6beeuploader.exe" 0
  CreateShortCut "$SMPROGRAMS\��ֻ�۷�\�����ϴ��ļ�.lnk" "${PRODUCT_WEB_SITE}/upload.php" "" "$PROGRAMFILES\Internet Explorer\iexplore.exe" 0
  CreateShortCut "$SMPROGRAMS\��ֻ�۷�\ж��.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  
  ;Add to desktop
  ;CreateShortCut "$DESKTOP\��ֻ�۷�.lnk" "$INSTDIR\6beeuploader.exe" "-m upload" "$INSTDIR\6beeuploader.exe" 0
  
  ;Favorite
  CreateShortCut "$FAVORITES\��ֻ�۷���ҳ.lnk" "${TEMPLATE_ROOT}\��ֻ�۷���ҳ.url" "" "$PROGRAMFILES\Internet Explorer\iexplore.exe" 0
  
  ;Quick Launch
  ;CreateShortCut "$QUICKLAUNCH\��ֻ�۷�.lnk" "$INSTDIR\6beeuploader.exe" "-m upload" "$INSTDIR\6beeuploader.exe" 0
  
  RegDLL "$INSTDIR\UUToolBar.dll"
  RegDLL "$INSTDIR\builtinsearch.dll"

SectionEnd
/*
Section "�Ҽ��˵�����" UUShellExt
  SetOutPath $INSTDIR
  File "${DEPLOY_ROOT}\6beeshellext.dll"
  RegDLL "$INSTDIR\6beeshellext.dll"
  WriteRegStr HKLM "SOFTWARE\Classes\*\shellex\ContextMenuHandlers\6beeshellext" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
  WriteRegStr HKLM "SOFTWARE\Classes\Directory\Background\shellex\ContextMenuHandlers\6beeshellext" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
  WriteRegStr HKLM "SOFTWARE\Classes\Directory\shellex\ContextMenuHandlers\6beeshellext" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
  WriteRegStr HKCR "*\shellex\ContextMenuHandlers\6beeshellext" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
  WriteRegStr HKCR "Directory\Background\shellex\ContextMenuHandlers\6beeshellext" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
  WriteRegStr HKCR "Directory\shellex\ContextMenuHandlers\6beeshellext" "" "{F33275F5-793B-46B4-892E-5CF86B5E289D}"
SectionEnd
*/

/*
Section "��ʾAlexa��վȫ������" Alexa
  CreateDirectory $INSTDIR\Plugin\alexa
  SetOutPath $INSTDIR\Plugin\alexa
  File "${DEPLOY_ROOT}\plugin\alexa\alexa.uta"
SectionEnd

Section "��ʾ��ҳGoogle PR" GooglePR
  CreateDirectory $INSTDIR\Plugin\ggpr
  SetOutPath $INSTDIR\Plugin\ggpr
  File "${DEPLOY_ROOT}\plugin\ggpr\ggpr.uta"
SectionEnd
*/

Section "��վAlexa������Google PRֵ" AlexaGoogle
  CreateDirectory $INSTDIR\Plugin\alexa
  SetOutPath $INSTDIR\Plugin\alexa
  File "${DEPLOY_ROOT}\plugin\alexa\alexa.uta"

  CreateDirectory $INSTDIR\Plugin\ggpr
  SetOutPath $INSTDIR\Plugin\ggpr
  File "${DEPLOY_ROOT}\plugin\ggpr\ggpr.uta"
SectionEnd

Section "��ҳ���������" ImgCap
  CreateDirectory $INSTDIR\Plugin\imgcapture
  SetOutPath $INSTDIR\Plugin\imgcapture
  File "${DEPLOY_ROOT}\plugin\imgcapture\imgcapture.uta"
  File "${DATABASE_ROOT}\imgcapture.db"
  File "${TEMPLATE_ROOT}\imgcapsetting.html"
SectionEnd

Section "��ҳ�ı���ȡ" WebClip
  CreateDirectory $INSTDIR\Plugin\webclip
  SetOutPath $INSTDIR\Plugin\webclip
  File "${DEPLOY_ROOT}\plugin\webclip\webclip.uta"
SectionEnd

Section "�������ʷ��¼" SmartHist
  CreateDirectory $INSTDIR\Plugin\smarthistory
  SetOutPath $INSTDIR\Plugin\smarthistory
  File "${DEPLOY_ROOT}\plugin\smarthistory\smarthistory.uta"
  File "${DATABASE_ROOT}\smarthistory.db"
  File "${TEMPLATE_ROOT}\smarthistory.html"
SectionEnd

Section "���������,¼��" WebAudio
  CreateDirectory $INSTDIR\Plugin\webaudio
  SetOutPath $INSTDIR\Plugin\webaudio
  File "${DEPLOY_ROOT}\plugin\webaudio\webaudio.uta"
SectionEnd



Section "������ǩ" OLBMark
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
  
  Call CheckVCRedist

  ;@todo should ask user to choose
  SetShellVarContext current
  ;SetShellVarContext all
  ${GetFileVersion} "${DEPLOY_ROOT}\UUToolBar.dll" $PVERSION
  ;MessageBox MB_OK $PVERSION IDOK
  Call DetectVersion
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

;--------------------------------
;Descriptions

;USE A LANGUAGE STRING IF YOU WANT YOUR DESCRIPTIONS TO BE LANGAUGE SPECIFIC

;Assign descriptions to sections
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${6BeeTool} "��ֻ�۷��ṩ���ļ��ϴ�����,֧�ֶ��ļ��ϵ�����"
;!insertmacro MUI_DESCRIPTION_TEXT ${UUShellExt} "������ֻ�۷䵽����ϵͳ,����ͨ���һ��ļ�����ֱ���ϴ��ļ�,���ӷ�����"
;!insertmacro MUI_DESCRIPTION_TEXT ${Alexa} "��ʾ��վ��Alexaȫ������"
;!insertmacro MUI_DESCRIPTION_TEXT ${GooglePR} "��ʾ��ҳ��Google PR"
!insertmacro MUI_DESCRIPTION_TEXT ${AlexaGoogle} "��ʾ��վ��Alexaȫ������,����ҳ��Google PRֵ"

!insertmacro MUI_DESCRIPTION_TEXT ${ImgCap} "��ҳ,�������"
!insertmacro MUI_DESCRIPTION_TEXT ${WebClip} "��ҳ�ı�ͼƬ��ȡ"
!insertmacro MUI_DESCRIPTION_TEXT ${SmartHist} "��¼���������ʷ,����������"
!insertmacro MUI_DESCRIPTION_TEXT ${WebAudio} "���������,¼�����������ΪMp3�ļ�"
!insertmacro MUI_DESCRIPTION_TEXT ${OLBMark} "������ǩ"
!insertmacro MUI_FUNCTION_DESCRIPTION_END


;--------------------------------
;Uninstaller Section

Section "Uninstall"

  UnRegDLL "$INSTDIR\6beeshellext.dll"
  UnRegDLL "$INSTDIR\UUToolBar.dll"
  UnRegDLL "$INSTDIR\builtinsearch.dll"
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PNAME}"
  DeleteRegKey HKLM SOFTWARE\6BeeNetworks\6Bee
  Call un.6BeeReg
  
  ; Remove files and uninstaller
  ; Give an option
  ;Delete "$INSTDIR\Plugin\*"
  ;Delete "$INSTDIR\Template\cn\*"
  ;Delete "$INSTDIR\Template\en\*"
  ;Delete "$INSTDIR\Template\*"
  ;Delete "$INSTDIR\*"
  ;Delete "$INSTDIR\*.*"
  
  Delete "$SMPROGRAMS\��ֻ�۷�\*"
  Delete "$SMPROGRAMS\��ֻ�۷�"
  Delete "$DESKTOP\��ֻ�۷�.lnk"
  Delete "$QUICKLAUNCH\��ֻ�۷�.lnk"
  
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\��ֻ�۷�\*.*"
  ; Remove directories used
  RMDir "$SMPROGRAMS\��ֻ�۷�"
  ;RMDir /r /REBOOTOK "$INSTDIR"
  RMDir /r "$INSTDIR"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

!insertmacro MUI_UNGETLANGUAGE

FunctionEnd
