#pragma once

#include <string>
#include <stdio.h>
#include <iostream>
#include <6BeeCommons.h>

namespace _6Bee{
	bool wcstombs2(const std::wstring &str,std::string& re){
		unsigned int codepage=(strcmp(getLangEncoding(),"UTF-8")==0)?CP_UTF8:CP_ACP;
		return toAnsiString(str.c_str(),re,codepage);
	}
}