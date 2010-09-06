#include "stdafx.h"
#include "UUShellExt.h"
#include <direct.h>
#include <6bees_gdi.h>
#include <6bees_util.h>
#include <6bees_const.h>
#include "resource.h"
#pragma comment(lib,"shlwapi")

#define _SIMPLE_

#ifdef __CHINESE__
#define MA_MY6BEESPACE "六只蜜蜂主页"
#define MW_MY6BEESPACE _T(MA_MY6BEESPACE)
#define MA_UPLOAD "上传文件到六只蜜蜂"
#define MW_UPLOAD _T(MA_UPLOAD)
#define MA_VIDEO "拍摄视频"
#define MW_VIDEO _T(MA_VIDEO)
#define MA_AUDIO "录制音频"
#define MW_AUDIO _T(MA_AUDIO)
#define MA_SCAP "屏幕捕捉"
#define MW_SCAP _T(MA_SCAP)
//
#define MA_COPYPATH "拷贝当前文件(夹)路径"
#define MW_COPYPATH _T(MA_COPYPATH)
#define MA_DOS "打开Windows命令行"
#define MW_DOS _T(MA_DOS)
#else
#define MA_MY6BEESPACE "Go to My UltraIE Space"
#define MW_MY6BEESPACE _T(MA_MY6BEESPACE)
#define MA_UPLOAD "Upload to UltraIE"
#define MW_UPLOAD _T(MA_UPLOAD)
#define MA_VIDEO "Shoot a Video"
#define MW_VIDEO _T(MA_VIDEO)
#define MA_AUDIO "Record Audio"
#define MW_AUDIO _T(MA_AUDIO)
#define MA_SCAP "Screen Capture"
#define MW_SCAP _T(MA_SCAP)
//
#define MA_COPYPATH "Copy File/Folder Path"
#define MW_COPYPATH _T(MA_COPYPATH)
#define MA_DOS "Windows Command Console"
#define MW_DOS _T(MA_DOS)
#endif
//
enum{ID_MY6BEESPACE,ID_UPLOAD/*,ID_VIDEO,ID_AUDIO,ID_SCAP*/,ID_COPYPATH,ID_DOS};

HRESULT CUUShellExt::FinalConstruct(){
  HIMAGELIST hImageList = ImageList_LoadImage(_AtlBaseModule.GetResourceInstance(),
    MAKEINTRESOURCE(IDB_CONTEXTMENU_12),12,1,CLR_DEFAULT,IMAGE_BITMAP,
    LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_SHARED);
  m_ImageList.Attach(hImageList);
  return S_OK;
}
void CUUShellExt::FinalRelease(){
  m_ImageList.Destroy();
}

HRESULT CUUShellExt::Initialize( LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID ){
  HRESULT hr = S_OK;
  try{
    FORMATETC fmt={CF_HDROP,NULL,DVASPECT_CONTENT,-1,TYMED_HGLOBAL};
    STGMEDIUM stg;
    if(pDataObj!=NULL && SUCCEEDED(pDataObj->GetData(&fmt,&stg))){
      HDROP hDrop = (HDROP)GlobalLock(stg.hGlobal);
      if (NULL!=hDrop){
        int fcount = DragQueryFile(hDrop,0xFFFFFFFF,NULL,0);
        if (fcount>0){
          selectedFiles.clear();
          TCHAR selectedFile[MAX_PATH+2];
          for (int i=0; i<fcount;i++){
            if(0 == DragQueryFile(hDrop,i,selectedFile,MAX_PATH)){
              hr = E_INVALIDARG;
            }else{
              selectedFiles.push_back(selectedFile);
            }
          }
        }
        GlobalUnlock(stg.hGlobal);
        ReleaseStgMedium(&stg);
      }else{
        selectedFiles.clear();
      }
    }
  }catch(...){
    return E_INVALIDARG;
  }
  return hr;
}

HRESULT CUUShellExt::QueryContextMenu(HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd, UINT uidLastCmd, UINT uFlags){
  //if(uFlags & CMF_DEFAULTONLY ) return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0 );
  UINT uCmdID = uidFirstCmd;
  UINT uM = uMenuIndex;
  try{
    InsertMenuW(hmenu, uM, MF_SEPARATOR|MF_BYPOSITION, NULL, NULL);

    InsertMenuW(hmenu,++uM,MF_STRING|MF_BYPOSITION,uCmdID++,MW_MY6BEESPACE);
    SetMenuItemBitmaps(hmenu, uM, MF_BYPOSITION, _6bees_gdi::icon2bitmap(m_ImageList.GetIcon(4)), NULL);
    InsertMenuW(hmenu,++uM,MF_STRING|MF_BYPOSITION,uCmdID++,MW_UPLOAD);
    SetMenuItemBitmaps(hmenu, uM, MF_BYPOSITION, _6bees_gdi::icon2bitmap(m_ImageList.GetIcon(5)), NULL);

    InsertMenuW(hmenu, ++uM, MF_SEPARATOR|MF_BYPOSITION, NULL, NULL);
#ifndef _SIMPLE_
    InsertMenuW(hmenu,++uM,MF_STRING|MF_BYPOSITION,uCmdID++,MW_COPYPATH);
    SetMenuItemBitmaps(hmenu, uM, MF_BYPOSITION, _6bees_gdi::icon2bitmap(m_ImageList.GetIcon(22)), NULL);

    if (selectedFiles.size()<=1){
      InsertMenuW(hmenu,++uM,MF_STRING|MF_BYPOSITION,uCmdID++,MW_DOS);
      SetMenuItemBitmaps(hmenu, uM, MF_BYPOSITION, _6bees_gdi::icon2bitmap(m_ImageList.GetIcon(26)), NULL);
    }
#endif
    InsertMenuW( hmenu, ++uM, MF_SEPARATOR|MF_BYPOSITION, NULL, NULL );
  }catch(...){}
  return MAKE_HRESULT(SEVERITY_SUCCESS,FACILITY_NULL,uM-uMenuIndex);//2 entry added
}

/// Check idCmd, it must be 0 or 1 since we have two menu items.
/// if(idCmd > 1 ) return E_INVALIDARG;
/// If Explorer is asking for a help string, copy our string into the supplied buffer,
/// you can see the help info in status bar of explorer
HRESULT CUUShellExt::GetCommandString(UINT idCmd, UINT uFlags, UINT* pwReserved, LPSTR pszName, UINT cchMax){
  try{
    switch (idCmd)
    {
    case ID_UPLOAD:{
      if(uFlags & GCS_HELPTEXT ){
        if (uFlags & GCS_UNICODE){
          lstrcpynW((LPWSTR) pszName, MW_UPLOAD, cchMax);
        }else{
          lstrcpynA(pszName, MA_UPLOAD, cchMax );
        }
        return S_OK;
      }
      }break;
    case ID_COPYPATH:{
      if(uFlags & GCS_HELPTEXT ){
        if (uFlags & GCS_UNICODE){
          lstrcpynW((LPWSTR) pszName, MW_COPYPATH, cchMax);
        }else{
          lstrcpynA(pszName, MA_COPYPATH, cchMax );
        }
        return S_OK;
      }
             }break;
    default:break;
    }
  }catch (...){
    return E_INVALIDARG;
  }

  return S_OK;
}

HRESULT CUUShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO pCmdInfo ){
  try{
    switch (LOWORD(pCmdInfo->lpVerb)){
      case ID_MY6BEESPACE:
        ShellExecute(NULL,_T("open"),_6bees_const::kUltraIEURL,_T(""),_T(""),SW_SHOW);
        break;
      case ID_UPLOAD:{
        using _6bees_util::ultrapath;
        wstring cmd(ultrapath::R().get(ultrapath::uploader));
        cmd += L" -m upload";
        wstring tmp;
        for(std::vector<ATL::CString>::const_iterator i = selectedFiles.begin();i!=selectedFiles.end();i++){
          if (!PathIsDirectory(*i)){
            tmp = tmp + L" \"";
            tmp += i->GetString();
            tmp += L"\"";
          }
        }
        if (!tmp.empty()){
          cmd += tmp;
          STARTUPINFO si = {sizeof(si)};
          PROCESS_INFORMATION pi;
          if (CreateProcessW(NULL,(LPWSTR)cmd.c_str(),NULL,NULL,false,0,NULL,NULL,&si,&pi)){/// @todo 
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
          }else{
            ::MessageBox(NULL,_T("Cannot find Ultra Uploader!"),_T("ERROR TO START Uploader."),MB_ICONINFORMATION);
          }
        }
        selectedFiles.clear();
          }break;
      /*case ID_VIDEO:{
        STARTUPINFO si = {sizeof(si)};
        PROCESS_INFORMATION pi;
        wchar_t* tmp = (LPWSTR)_6beed_util::Get6BEEPath(_6bees_const::kCapVideoName);
        if (CreateProcessW(NULL,tmp,NULL,NULL,false,0,NULL,NULL,&si,&pi)){
          CloseHandle(pi.hProcess);
          CloseHandle(pi.hThread);
        }}break;
      case ID_AUDIO:{
        STARTUPINFO si = {sizeof(si)};
        PROCESS_INFORMATION pi;
        wchar_t* tmp = (LPWSTR)_6beed_util::Get6BEEPath(_6bees_const::kAudioRecorderName);
        if (CreateProcessW(NULL,tmp,NULL,NULL,false,0,NULL,NULL,&si,&pi)){
          CloseHandle(pi.hProcess);
          CloseHandle(pi.hThread);
        }}break;
      case ID_SCAP:{
        STARTUPINFO si = {sizeof(si)};
        PROCESS_INFORMATION pi;
        if (CreateProcessW(NULL,(LPWSTR)_6beed_util::Get6BEEPath(_6bees_const::kScreenCapName),NULL,NULL,false,0,NULL,NULL,&si,&pi)){
          CloseHandle(pi.hProcess);
          CloseHandle(pi.hThread);
        }}break;*/
#ifndef _SIMPLE_
      case ID_COPYPATH:
        if (::OpenClipboard(NULL)) {
          ATL::CString pStrClipboardText;
          int nFileCount=(int)selectedFiles.size();
          if (nFileCount>0){
            for(std::vector<ATL::CString>::const_iterator i = selectedFiles.begin();i!=selectedFiles.end();){
              pStrClipboardText.Append(*i);
              if (++i != selectedFiles.end()){
                pStrClipboardText.Append(_T("\r\n"));
              }
            }
          }else{
            USES_CONVERSION;
            pStrClipboardText = A2CW(pCmdInfo->lpDirectory);
          }
          ::EmptyClipboard();
          int len = (int)(pStrClipboardText.GetLength()+1)*2;
          HGLOBAL hText = ::GlobalAlloc(GMEM_SHARE, len);
          wchar_t* pText = (LPTSTR) ::GlobalLock(hText);
          wcscpy_s(pText,len,pStrClipboardText.GetString());
          ::SetClipboardData(CF_UNICODETEXT,hText);
          ::GlobalUnlock(hText);
          ::CloseClipboard();
        }break;
      case ID_DOS:
        if (selectedFiles.empty()){
          _chdir(pCmdInfo->lpDirectory);
        }else{
          _wchdir(selectedFiles[0]);
        }
        ShellExecuteA(NULL,"open","cmd.exe","","",SW_SHOW);
        break;
#endif
      default:
        break;
    }
  }catch(...) {
    return E_INVALIDARG;
  }
  return S_OK;
}

STDMETHODIMP CUUShellExt::HandleMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam){
  return S_OK;
}

STDMETHODIMP CUUShellExt::HandleMenuMsg2(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult){
  return S_OK;
}