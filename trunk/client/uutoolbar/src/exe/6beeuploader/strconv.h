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