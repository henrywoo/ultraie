#include "6bees_str.h"
#include <atlbase.h>
#include <atlstr.h>

#include <strsafe.h>// StringCchPrintfW
#include <time.h>

#include <algorithm>//remove_if
#include <functional>



#ifndef __6BEEUPLOADER__

#include <pcre++.h>
#ifdef NDEBUG
#pragma comment(lib,"pcre++.lib")
#else
#pragma comment(lib,"pcre++d.lib")
#endif
using namespace pcrepp;

#endif

namespace _6bees_str{
  

  bool strreplace(wstring& source, const wchar_t* change, const wchar_t* to){
    ATL::CStringW tmp(source.c_str());
    tmp.Replace(change,to);
    source = tmp;
    return true;
  }

  bool strreplace(string& source, const char* change, const char* to){
    ATL::CStringA tmp(source.c_str());
    tmp.Replace(change,to);
    source = tmp;
    return true;
  }

  string GetFileExt(const string& str,char delimiter){
    size_t t1 = str.find_last_of(delimiter);
    return str.substr(t1+1,str.size()-t1-1);
  }

  wstring GetFileExt(const wstring& str,wchar_t delimiter){
    size_t t1 = str.find_last_of(delimiter);
    return str.substr(t1+1,str.size()-t1-1);
  }

  void FilterStr(wstring& str){
    static const wchar_t* const kfilter = L"][`|{}/\\~!@#$%&*()+='\":;?<>，：；。‘”？！~《》【】（）-«»";
    const wchar_t* tmp = kfilter;
    while ((*tmp)!=L'\0'){
      str.erase(
        remove_if(str.begin(),str.end(),bind2nd(equal_to<wchar_t>(), (*tmp))),
        str.end());
      tmp++;
    }
  }

  wstring GenUniqStr2(){
    static int count=0;
    ULONG thetime = (ULONG)time(0)+(++count);
    return wstringmaker() << thetime;
  }

#ifndef __6BEEUPLOADER__
  wstring GenFileName(const wchar_t* oriname){
    if (oriname==NULL){
      return GenUniqStr2();
    }
    wstring tmp(oriname);
    FilterStr(tmp);
    string tmp2 = stringmaker(CP_ACP) << tmp;
    Pcre re2(string("[\\s]+"),PCRE_CASELESS);
    string tmp3 = re2.replace(tmp2,string(" "));
    return wstringmaker(CP_ACP) << tmp3;
  }

  wstring GetSugTxt(const wchar_t* s){
    static const char* const kstopwords = 
      ",((a)|(an)|(and)|(are)|(as)|(at)|(be)|(but)|(by)|(for)|(if)|(in)|(into)|(is)|(it)|(no)|(not)|(of)|"
      "(on)|(or)|(such)|(that)|(the)|(their)|(then)|(there)|(these)|(they)|(this)|(to)|(was)|(will)|(with)),";
    wstring tmp(s);
    FilterStr(tmp);
    string tmp2 = stringmaker(CP_ACP) << tmp;
    Pcre re2(string("[\\s]+"),PCRE_CASELESS);
    string tmp3 = re2.replace(tmp2,string(","));
    Pcre re3(string(kstopwords),PCRE_CASELESS);
    string tmp4 = re3.replace(tmp3,string(","));
    return wstringmaker(CP_ACP) << tmp4;
  }
#endif

  wchar_t* toWideString(const char *mbstr,unsigned int codePage){
    wchar_t *wstr = 0;
    if (mbstr) {
      size_t origsize = strlen(mbstr);
      if (origsize == 0) {
        wstr = new wchar_t [1];
        wstr [0] = 0;
        return wstr;
      }
      int rc = MultiByteToWideChar(codePage, 0, mbstr, -1, wstr, 0);
      wstr = new wchar_t [rc+1];
      rc = MultiByteToWideChar(codePage, 0, mbstr, -1, wstr, rc);
    }
    return wstr;
  }

  bool toWideString(const char *mbstr,wstring& dest,unsigned int codePage){
    try{
      wchar_t* wstr = toWideString(mbstr, codePage);
      dest = wstr;
      delete [] wstr;
      return true;
    }catch(...){
      return false;
    }
  }

  char* toAnsiString(const wchar_t *wcs,unsigned int codePage){
    char *mbstr = 0;
    if (wcs) {
      size_t numConvertedChars = 0;
      size_t origsize = wcslen(wcs) +1;
      int rc = WideCharToMultiByte(codePage, 0, wcs, -1, mbstr, 0, NULL, NULL);
      mbstr = new char [rc+1];
      WideCharToMultiByte(codePage, 0, wcs, -1, mbstr, rc, NULL, NULL);
      mbstr[rc] = 0;
    }
    return mbstr;
  }

  bool toAnsiString(const wchar_t *wcs,string& dest,unsigned int codePage){
    try{
      char* str = toAnsiString(wcs, codePage);
      dest = str;
      delete [] str;
      return true;
    }catch(...){
      return false;
    }
  }

  wstringmaker& wstringmaker::operator<<(DWORD dwNum){
    DWORD dwTemp = dwNum;
    int iCnt=1; // name lookup of 'iCnt' changed for new ISO 'for' scoping
    for( ; (dwTemp/=10) != 0; iCnt++ );
    size_t nsize = str_.size() + iCnt;
    str_.resize(nsize);
    const wchar_t* tmp = str_.c_str();
    wchar_t* buffer = &(*str_.begin());
    if(!SUCCEEDED(StringCchPrintfW(buffer,nsize+1,L"%s%u",tmp,dwNum))){
      str_ = L"";
    }
    return *this;
  }

  wstringmaker& wstringmaker::operator<<(const wstring& stradd){
    str_ += stradd;
    return *this;
  }

  wstringmaker& wstringmaker::operator<<(const string& stradd){
    wstring tmp;
    toWideString(stradd.c_str(),tmp,codepage_);
    str_ += tmp;
    return *this;
  }

  stringmaker& stringmaker::operator<<(DWORD dwNum){
    DWORD dwTemp = dwNum;
    int iCnt=1; // name lookup of 'iCnt' changed for new ISO 'for' scoping
    for( ; (dwTemp/=10) != 0; iCnt++ );
    size_t nsize = str_.size() + iCnt;
    str_.resize(nsize);
    const char* tmp = str_.c_str();
    char* buffer = &(*str_.begin());
    if(!SUCCEEDED(StringCchPrintfA(buffer,nsize+1,"%s%u",tmp,dwNum))){
      str_ = "";
    }
    return *this;
  }

  stringmaker& stringmaker::operator<<(const string& stradd){
    str_ += stradd;
    return *this;
  }

  stringmaker& stringmaker::operator<<(const wstring& stradd){
    string tmp;
    toAnsiString(stradd.c_str(),tmp,codepage_);
    str_ += tmp;
    return *this;
  }

};