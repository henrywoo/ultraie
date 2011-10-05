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

#ifndef __UUICED_CONSTDEF__
#define __UUICED_CONSTDEF__

#define DEPLOYMENT


#define MYSQLDBNAME "UltraIEDB"
#define MYSQLDBUSER "root"
//#define MYSQLPORT   6033
#define MYSQLSOCKET "/var/lib/mysql/mysql.sock"


#ifdef DEPLOYMENT

#define SERVERIP "66.235.160.14"
#define SERVER_PORT "9090"
#define MYSQLDBPASS "simonwoo"
const static char* UPLOAD_ROOT="/upload/";
const static char* DOWNLOAD_ROOT="/download/./";
#define DOC_ROOT "/home/simonwoo/www.ultraie.com/pages"
#define THUMB_ROOT DOC_ROOT "/OLBM/thumb/"
#define ICON_ROOT DOC_ROOT "/OLBM/icon/"
#define HTML_ROOT DOC_ROOT "/ultrafiles/html/"
#define IMG_ROOT DOC_ROOT "/ultrafiles/image/"

#else

#define SERVERIP "192.168.0.152"
#define SERVER_PORT "9090"
#define MYSQLDBPASS "juikahdfkasdhf#%$#^&#$%^"
const static char* UPLOAD_ROOT="/ad8/h/upload/";
const static char* DOWNLOAD_ROOT="/ad8/h/download/./";
const static char* THUMB_ROOT="/usr/local/apache223/htdocs/OLBM/thumb/";
const static char* ICON_ROOT="/usr/local/apache223/htdocs/OLBM/icon/";
const static char* HTML_ROOT="/usr/local/apache223/htdocs/ultrafiles/html/";
const static char* IMG_ROOT="/usr/local/apache223/htdocs/ultrafiles/image/";

#endif

#define URL "tcp -h " SERVERIP " -p " SERVER_PORT " -z"
#define SVRID "UUICE"
#define PROXYSTR SVRID ":" URL
#define SVRADAPTER "UUICEAdapter"
#define UltraIEURL  "http://www.ultraie.com/"
#define UUFileURL  "http://www.uufile.com/"

#define LOWERWSTR(x) std::transform(x.begin(), x.end(), x.begin(), (int (*)(int))std::tolower)
#define UPPERWSTR(x) std::transform(x.begin(), x.end(), x.begin(), (int (*)(int))std::toupper)
#define PRINTFUNCLINE {cout << "Function:" << __FUNCTION__ << "\tLine:" << __LINE__ << endl;}


#endif
