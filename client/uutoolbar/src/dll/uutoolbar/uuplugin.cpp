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

#include "stdafx.h"
#include "uuplugin.h"

namespace nsplugin{
  vector<uuplugin*> uuplugin::global_plugins_;

  uuplugin::uuplugin(){
    global_plugins_.push_back(this);
    init();
  }

  bool uuplugin::init(){
    len_ = 100;
    img_ = NULL;
    hwnd_= NULL;
    caption_=L"";
    tip_ = L"";
    return true;
  }

  bool uuplugin::SystemBasedInit(){
    return true;
  }

  bool uuplugin::SystemBasedTerm(){
    return true;
  }
};