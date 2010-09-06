#ifndef __STUB_SKELETON_STRINGMAKER_H__
#define __STUB_SKELETON_STRINGMAKER_H__

#include <windows.h>
#include <string>

namespace _6bees_str{

  using namespace std;

  bool strreplace(wstring& source,
    const wchar_t* change,
    const wchar_t* to);

  bool strreplace(string& source,
    const char* change,
    const char* to);

  template <class T>
  bool startwith(const T& source, const T& head){
      return source.compare(0,head.size(),head) == 0;
  }

  string GetFileExt(const string& str,char delimiter='.');

  wstring GetFileExt(const wstring& str,wchar_t delimiter=L'.');

  void FilterStr(wstring& str);
#ifndef __6BEEUPLOADER__
  wstring GetSugTxt(const wchar_t* s);
#endif
  wstring GenUniqStr2();

#ifndef __6BEEUPLOADER__
  wstring GenFileName(const wchar_t* oriname);
#endif
  char* toAnsiString(const wchar_t *wcs,unsigned int codePage=CP_UTF8);

  wchar_t* toWideString(const char *mbstr,unsigned int codePage=CP_UTF8);
  
  bool toAnsiString(const wchar_t *wcs,string& dest,unsigned int codePage=CP_UTF8);

  bool toWideString(const char *mbstr,wstring& dest,unsigned int codePage=CP_UTF8);

  /// @brief a functor of making wstring like this:
  /// @brief wstring hello = wstringmaker << "hello" << 123;
  /// @brief fail - empty wstring; succeed - a non-empty wstring
  class wstringmaker{
  private:
    wstring str_;
    unsigned int codepage_;
  public:
    wstringmaker(unsigned int cp=CP_UTF8):codepage_(cp){}
    wstringmaker(const wstring& a):codepage_(CP_UTF8),str_(a){}
    wstringmaker(const wstring& a,unsigned int cp):codepage_(cp),str_(a){}
    wstringmaker& operator<<(DWORD dwNum);
    wstringmaker& operator<<(const wstring& stradd);
    wstringmaker& operator<<(const string& stradd);
    operator wstring() const{return str_;}
    const wchar_t* c_str() const{return str_.c_str();}
  };

  class stringmaker{
  private:
    string str_;
    unsigned int codepage_;
  public:
    stringmaker(unsigned int cp=CP_UTF8):codepage_(cp){}
    stringmaker(const string& a):codepage_(CP_UTF8),str_(a){}
    stringmaker(const string& a,unsigned int cp):codepage_(cp),str_(a){}
    stringmaker& operator<<(DWORD dwNum);
    stringmaker& operator<<(const string& stradd);
    stringmaker& operator<<(const wstring& stradd);
    operator string() const{return str_;}
    const char* c_str() const{return str_.c_str();}
  };

};


#endif
