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

#ifndef __UUPLUGIN_IMGCAPTURE_UTIL__
#define __UUPLUGIN_IMGCAPTURE_UTIL__

#include <windows.h>
#include <string>

namespace nsplugin{

  enum COLUMN{I_IMGHASSHADOW,I_IMGHASBORDER,I_IMGBORDERCOLOR};

  std::string GetDBPath();

  void GetImgCapParam(bool& border,bool& shadow,unsigned long& color);

  void u_setImg_has_shadow(bool b);

  void u_setImg_has_border(bool b);

  void u_setImg_border_color(COLORREF color);

};




#endif