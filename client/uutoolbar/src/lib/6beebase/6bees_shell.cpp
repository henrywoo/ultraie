#include "6bees_shell.h"
#include "6bees_util.h"
#include "6bees_net.h"
#include "6bees_html.h"
#include <intshcut.h>
#include <shlguid.h>
#include <shlobj.h>
#include <map>

#include <sys\types.h> // For stat structure
#include <sys\stat.h>
#include <fstream> // For wstream
#include "6bees_str.h"
using _6bees_str::stringmaker;
using _6bees_str::wstringmaker;

namespace _6bees_shell{
  using namespace std;
  using namespace UUTOOLBAR;

  wstring GetLnkFullPath(const wstring& f,const wstring& t){
    wstring folder(f);
    wstring title(t);
    _6bees_str::FilterStr(title);
    _6bees_str::strreplace(folder,_T("|"),_T("\\"));
    _6bees_str::strreplace(title,L":",L" ");///@todo
    wstring csTargetFolder = GetSpecialFolderLocation(CSIDL_FAVORITES);
    csTargetFolder += L"\\";
    csTargetFolder += folder.c_str();
    return wstring(csTargetFolder + L"\\" + title);
  }

  wstring GetSpecialFolderLocation(int nFolderID){
    LPITEMIDLIST pidl;
    if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, nFolderID, &pidl))){
      wchar_t szPath[_MAX_PATH]={};
      if (SHGetPathFromIDList(pidl, szPath)){
        return wstring(szPath);
      }
    }
    return wstring();
  }

  // title.url, http://www.163.com,C:\tmp\favicon.ico
  bool CreateInternetShortcut(const wstring& _LinkFilename,
    const wstring& Argument,
    const wstring& csIconLocalPath,
    const wstring& subfolder)
  {
    CoInitialize(NULL);
    IUniformResourceLocator* pURL=NULL;
    IPersistFile* pPf=NULL;
    wstring wszSCPath=GetLnkFullPath(subfolder,_LinkFilename) + L".url";
    _6bees_util::MakeSureDir(wszSCPath.c_str());

    HRESULT hr= CoCreateInstance(CLSID_InternetShortcut, NULL, CLSCTX_INPROC_SERVER,IID_IUniformResourceLocator, (void **)&pURL);
    if (FAILED(hr))
      return false;
    hr = pURL->SetURL(Argument.c_str(),IURL_SETURL_FL_GUESS_PROTOCOL);
    if (FAILED(hr))
      return false;
    hr=pURL->QueryInterface(IID_IPersistFile, (void **)&pPf);
    if (FAILED(hr)) {
      pURL->Release();
      ::CoUninitialize();
      return false;
    }
    hr = pPf->Save(wszSCPath.c_str(),TRUE);
    if (FAILED(hr))
      return false;
    // set the property : icon
    CComPtr<IPropertySetStorage> spIPropertySetStorage;
    hr=pURL->QueryInterface(IID_IPropertySetStorage, (void **)&spIPropertySetStorage);
    if (FAILED(hr)) {
      pURL->Release();
      ::CoUninitialize();
      return false;
    }
    CComPtr<IPropertyStorage> spIPropertyStorage;
    hr = spIPropertySetStorage->Open(FMTID_Intshcut, STGM_READWRITE, &spIPropertyStorage);
    if (FAILED(hr)) {
      pURL->Release();
      ::CoUninitialize();
      return false;
    }

    PROPSPEC propspecw[2]={};
    propspecw[0].ulKind = PRSPEC_PROPID;
    propspecw[0].propid = PID_IS_ICONINDEX;  
    propspecw[1].ulKind = PRSPEC_PROPID;
    propspecw[1].propid = PID_IS_ICONFILE;  
    PROPVARIANT propvarw[2]={};
    PropVariantInit(&propvarw[0]);
    PropVariantInit(&propvarw[1]);
    propvarw[0].vt = VT_I4;
    propvarw[0].lVal = 0;
    propvarw[1].vt = VT_LPWSTR;
    //propvarw[1].pwszVal = L"D:\\CProjects\\GShortcut\\shortcutjp.ico"; //works
    //propvarw[1].pwszVal = L"http://www.163.com/favicon.ico"; // this also works   !!!!!!
    if (!csIconLocalPath.empty())
      propvarw[1].pwszVal = CComBSTR(csIconLocalPath.c_str());
    else{
      wstring iepath = GetSpecialFolderLocation(CSIDL_PROGRAM_FILES);
      iepath += L"\\Internet Explorer\\iexplore.exe";
      propvarw[1].pwszVal = CComBSTR(iepath.c_str());
    }
    hr = spIPropertyStorage->WriteMultiple(2, propspecw, propvarw, 0); 
    hr = spIPropertyStorage->Commit(STGC_OVERWRITE);  //STGC_OVERWRITE  STGC_DEFAULT 
    hr = pPf->Save(NULL,TRUE);
    pURL->Release();
    pPf->Release();
    ::CoUninitialize();
    return true;
  }

  /// @todo - iconurl database implementation
  void getOLBMItems(OLBMItems& items,
    ImgSeq& icons,
    const PFAVORITELIST& pFavoListCurFolderRoot,
    const wstring& folder,
    const IEFavURLs& existingOLBM)
  {
    static map<ATL::CStringA,string> hostname_iconURL;//cache for icon url
    //static map<ATL::CStringA,string> hostname_iconpath;//cache for icon path
    if(!pFavoListCurFolderRoot)
      return;
    PFAVORITELIST pFavoListCur = pFavoListCurFolderRoot;
    BOOL bFolderFinished = FALSE, bURLFinished = FALSE;
    do{
    switch(pFavoListCur->m_favoType){
    case SFGAO_FOLDER:
      {
        wstring realfolder=folder+(folder.empty()?L"":L"|")+pFavoListCur->m_pFavoTitle;
        getOLBMItems(items,icons,pFavoListCur->m_pSubFolder,realfolder,existingOLBM);

        if(pFavoListCur->m_pNextFolder)
          pFavoListCur  = pFavoListCur->m_pNextFolder;
        else{
          pFavoListCur  = pFavoListCurFolderRoot;
          bFolderFinished  = TRUE;
        }
      }break;
    case SFGAO_LINK:
      {
        string url=stringmaker()<<pFavoListCur->m_pFavoURL;
        if (find(existingOLBM.begin(),existingOLBM.end(),url)==existingOLBM.end()){
          string title = stringmaker()<< pFavoListCur->m_pFavoTitle;
          string myfolder=stringmaker()<< folder;
          OLBMValue olbmVal;
          olbmVal.title = title;
          olbmVal.description = "";
          olbmVal.tag = "";
          olbmVal.folder = myfolder;
          // set up a database locally: map webdomain->iconurl
          olbmVal.iconURL = "";

          /*
          _6bees_net::apr_uri_t t;
          if(_6bees_net::_6beeparseuri(url.c_str(),t)){
            if (hostname_iconURL.find(t.hostname)==hostname_iconURL.end()){
              string iconURL;
              int iconsize=_6bees_html::getValidFavicon(url,iconURL);
              if (iconsize>0 && iconsize<100*1024){
                olbmVal.iconURL = iconURL;
              }
              hostname_iconURL[t.hostname]=iconURL;
            }else{
              olbmVal.iconURL = hostname_iconURL[t.hostname];
            }
          }else{
            olbmVal.iconURL = "";
          }
          */

          /*
          ATL::CStringA hostname,urlbody,urlfile;
          if(_6bees_net::ParseHttpURL(url.c_str(),hostname,urlbody,urlfile)){
            if (hostname_iconURL.find(hostname)==hostname_iconURL.end()){
              ATL::CStringA iconURL;
              int iconsize=getValidFavicon(url.c_str(),iconURL);
              if (iconsize>0 && iconsize<100*1024){
                olbmVal.iconURL = iconURL.GetString();
              }
              hostname_iconURL[hostname]=iconURL;
            }else{
              olbmVal.iconURL = hostname_iconURL[hostname];
            }
          }else{
            olbmVal.iconURL = "";
          }
          */
          items[url] = olbmVal;
        }

        if(pFavoListCur->m_pNextURL)
          pFavoListCur  = pFavoListCur->m_pNextURL;
        else{
          pFavoListCur  = pFavoListCurFolderRoot;
          bURLFinished  = TRUE;
        }
      }break;
    default:
      if(pFavoListCur->m_pNextFolder){
        if(!bFolderFinished){
          pFavoListCur  = pFavoListCur->m_pNextFolder;
          bFolderFinished  = FALSE;
          break;
        }
      } else {
        bFolderFinished = TRUE;
      }

      if(pFavoListCur->m_pNextURL){
        if(!bURLFinished){
          pFavoListCur  = pFavoListCurFolderRoot->m_pNextURL;
          bURLFinished  = FALSE;
        }
      } else {
        bURLFinished  = TRUE;
      }
      break;
      }
    }while(!(bFolderFinished && bURLFinished));
  }

  void getexistingIEFavURLs(IEFavURLs& existingIEFavURLs,const PFAVORITELIST& pFavoListCurFolderRoot,const wstring& folder){
    if(!pFavoListCurFolderRoot)
      return;
    PFAVORITELIST pFavoListCur = pFavoListCurFolderRoot;
    BOOL bFolderFinished = FALSE, bURLFinished = FALSE;
    do{
    switch(pFavoListCur->m_favoType){
    case SFGAO_FOLDER:
      {
        wstring realfolder=folder+(folder.empty()?L"":L"|")+pFavoListCur->m_pFavoTitle;
        getexistingIEFavURLs(existingIEFavURLs,pFavoListCur->m_pSubFolder,realfolder);

        if(pFavoListCur->m_pNextFolder)
          pFavoListCur  = pFavoListCur->m_pNextFolder;
        else{
          pFavoListCur  = pFavoListCurFolderRoot;
          bFolderFinished  = TRUE;
        }
      }break;
    case SFGAO_LINK:
      {
        string url=stringmaker()<< pFavoListCur->m_pFavoURL;
        existingIEFavURLs.push_back(url);

        if(pFavoListCur->m_pNextURL)
          pFavoListCur  = pFavoListCur->m_pNextURL;
        else{
          pFavoListCur  = pFavoListCurFolderRoot;
          bURLFinished  = TRUE;
        }
      }break;
    default:
      if(pFavoListCur->m_pNextFolder){
        if(!bFolderFinished){
          pFavoListCur  = pFavoListCur->m_pNextFolder;
          bFolderFinished  = FALSE;
          break;
        }
      } else {
        bFolderFinished = TRUE;
      }

      if(pFavoListCur->m_pNextURL){
        if(!bURLFinished){
          pFavoListCur  = pFavoListCurFolderRoot->m_pNextURL;
          bURLFinished  = FALSE;
        }
      } else {
        bURLFinished  = TRUE;
      }
      break;
      }
    }while(!(bFolderFinished && bURLFinished));
  }

  void FilterIEFav(OLBMItems& items){
    OLBMItems newitems;
    OLBMItems::const_iterator begin= items.begin();
    OLBMItems::const_iterator end= items.end();
    for(map<string,OLBMValue>::const_iterator i=begin;i!=end;++i){
      wstring title = wstringmaker()<< (*i).second.title;
      wstring folder = wstringmaker()<< (*i).second.folder;
      wstring lnkpath = GetLnkFullPath(folder,title) + L".url";
      if (!PathFileExists(lnkpath.c_str())){
        newitems[i->first] = i->second;
      }
    }
    items = newitems;
  }
  
  void writeOLBMItemstoIEFav(const OLBMItems& items){
    OLBMItems::const_iterator begin= items.begin();
    OLBMItems::const_iterator end= items.end();
    for(map<string,OLBMValue>::const_iterator i=begin;i!=end;++i){
      wstring url = wstringmaker()<< (*i).first;
      wstring title = wstringmaker()<< (*i).second.title;
      _6bees_str::FilterStr(title);
      wstring iconURL = wstringmaker()<< (*i).second.iconURL;
      wstring folder = wstringmaker()<< (*i).second.folder;
      CreateInternetShortcut(title,url,iconURL,folder);
    }
  }

  /**
  * Get the size of a file
  */
  int GetFileSizeA( const char * szFileName ){ 
    struct stat fileStat; 
    int err = stat( szFileName, &fileStat ); 
    if (0 != err) return -1;
    return fileStat.st_size; 
  }

  bool GenICEImg(const string& imgpath,::UUTOOLBAR::Image& img){
    if (!PathFileExistsA(imgpath.c_str())){
      return false;
    }

    int imgsize=GetFileSizeA(imgpath.c_str());
    img.data.resize(imgsize);

    ifstream f;
    f.open(imgpath.c_str(),ios_base::binary|ios_base::in);
    if (!f.good() || f.eof() || !f.is_open()){
      return false;
    }
    f.read((char*)&img.data[0],imgsize);/// @todo  - can I safely cast it to char*
    f.close();

    img.name = PathFindFileNameA(imgpath.c_str());
    return true;
  }

};


