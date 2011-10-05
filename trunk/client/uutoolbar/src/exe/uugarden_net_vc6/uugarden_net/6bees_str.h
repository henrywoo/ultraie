// UltraIE - Yet Another IE Add-on
// Copyright (C) 2006-2010
// Simon Wu Fuheng (simonwoo2000 AT gmail.com), Singapore
// Homepage: http://www.linkedin.com/in/simonwoo
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef __STUB_SKELETON_STRINGMAKER_H__
#define __STUB_SKELETON_STRINGMAKER_H__

#include <string>

#define CP_ACP                    0           // default to ANSI code page
#define CP_UTF8                   65001       // UTF-8 translation
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;


namespace _6bees_str{

  using namespace std;


  template <class T>
  bool startwith(const T& source, const T& head){
      return source.compare(0,head.size(),head) == 0;
  }

  string GetFileExt(const string& str,char delimiter='.');

  wstring GetFileExt(const wstring& str,wchar_t delimiter=L'.');

  void FilterStr(wstring& str);

  wstring GenUniqStr2();

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
