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

#ifdef MY6BEECLIENTMNG_H_EXPORTS
#define MY6BEECLIENTMNG_H_API __declspec(dllexport)
#else
#define MY6BEECLIENTMNG_H_API __declspec(dllimport)
#endif

extern "C"{
  MY6BEECLIENTMNG_H_API BOOL WINAPI InstallIECreatProcessHook();
  MY6BEECLIENTMNG_H_API BOOL WINAPI UnInstallIECreatProcessHook();
}