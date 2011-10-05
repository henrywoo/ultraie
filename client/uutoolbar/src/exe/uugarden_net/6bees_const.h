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

#ifndef __ULTRAIE_6BEES_CONST__
#define __ULTRAIE_6BEES_CONST__

#include <tchar.h>

/// @brief constant value
namespace _6bees_const{

  /// Windows Title
  const wchar_t* const kUploader_WndTitle=_T("UltraIE Uploader - Upload To your Web Space!");
  const wchar_t* const kClientMng_WndTitle=_T("Log in to UltraIE.com");
  const wchar_t* const kCapImg_EntirePage_WndTitle = _T("Capture the whole page");
  const wchar_t* const kCapImg_WebEleme_WndTitle = _T("Capture web element");
  const wchar_t* const kCapImg_Window_WndTitle = _T("Capture window");

  /// Window Class
  const wchar_t* const kWND_NAME_REFLECTION=_T("ULTRAIEREFLECTION");
  const wchar_t* const kWND_NAME_TBWND=_T("ULTRAIETBWND");

  /// File name
  /*
  const wchar_t* const kDBName = _T("main.db");
  const wchar_t* const kUploaderName=_T("6beeuploader.exe");
  const wchar_t* const kAudioRecorderName=_T("6beeaudiorec.exe");
  const wchar_t* const kCapVideoName=_T("6beecapvideo.exe");
  const wchar_t* const kHTMLEditName=_T("6beehtmleditor.exe");
  const wchar_t* const kClientMngName=_T("6beeclientmng.exe");
  const wchar_t* const kResDLLName=_T("6beeres.dll");

  /// Directory name
  const wchar_t* const kTemplateDir=_T("Template");
  const wchar_t* const kIconDir=_T("Template\\Icon");
  const wchar_t* const kPictureDir=_T("Picture");
  const wchar_t* const kCacheDir=_T("Cache");
  const wchar_t* const kAudioDir=_T("Audio");
  const wchar_t* const kVideoDir=_T("Video");
  const wchar_t* const kPluginDir=_T("Plugin");
  */

  /// URL
  const wchar_t* const kUltraIEURL=_T("http://www.ultraIE.com/");
  const wchar_t* const kUltraIEURL_REGISTER=_T("http://www.ultraIE.com/register.html");
  const wchar_t* const kUltraIEURL_FORGETPASS=_T("http://www.ultraIE.com/forgetpass.html");

  /// Others
  const wchar_t* const kHighLightStyle= _T("STYLE=\"font-weight:bolder;font-size:13px;")
    _T("font-family:arial,'lucida console',sans-serif;")
    _T("color:maroon; background-color:yellow;border:1px solid red;\"");

  static const wchar_t* const kHtml_Template=
    L"<!-- ultraie webclipper - saved from url=%s -->\r\n\
     <HTML>\r\n<HEAD>\r\n\
     <TITLE>%s</TITLE>\r\n\
     <META http-equiv=Content-Type content=\"text/html; charset=utf-8\">\r\n\
     <BASE target=_top></HEAD>\r\n\
     <BODY style=\"border:0px;font:11px arial\" scroll=yes>\r\n\
     %s\r\n\
     </BODY></HTML>";

  ///enum of OS version
  enum OST{XP,VISTA,OTHERNT,OLDWIN};

  /// net
  enum UPLOADERMODE{UPLOAD_MODE,DIRECTUPLOAD_MODE,UPLOAD_THUMB_MODE,UPLOAD_ICON_MODE,DOWNLOAD_MODE};

  /// default port
  const int APR_URI_FTP_DEFAULT_PORT         =21;/// default FTP port
  const int APR_URI_SSH_DEFAULT_PORT         =22;/// default SSH port
  const int APR_URI_TELNET_DEFAULT_PORT      =23;/// default telnet port
  const int APR_URI_GOPHER_DEFAULT_PORT      =70;/// default Gopher port
  const int APR_URI_HTTP_DEFAULT_PORT        =80;/// default HTTP port
  const int APR_URI_POP_DEFAULT_PORT        =110;/// default POP port
  const int APR_URI_NNTP_DEFAULT_PORT       =119;/// default NNTP port
  const int APR_URI_IMAP_DEFAULT_PORT       =143;/// default IMAP port
  const int APR_URI_PROSPERO_DEFAULT_PORT   =191;/// default Prospero port
  const int APR_URI_WAIS_DEFAULT_PORT       =210;/// default WAIS port
  const int APR_URI_LDAP_DEFAULT_PORT       =389;/// default LDAP port
  const int APR_URI_HTTPS_DEFAULT_PORT      =443;/// default HTTPS port
  const int APR_URI_RTSP_DEFAULT_PORT       =554;/// default RTSP port
  const int APR_URI_SNEWS_DEFAULT_PORT      =563;/// default SNEWS port
  const int APR_URI_ACAP_DEFAULT_PORT       =674;/// default ACAP port
  const int APR_URI_NFS_DEFAULT_PORT        =2049;/// default NFS port
  const int APR_URI_TIP_DEFAULT_PORT        =3372;/// default TIP port
  const int APR_URI_SIP_DEFAULT_PORT        =5060;/// default SIP port

  /// max request/response header size
  const int MAXHEADERSIZE = 1024*4; // 4K

  /// max IP address size
  const int MAXIPADDRSIZE = 256;

  /// images of contextmenuicons.bmp, start with lower case p
  /// actually, this should be in 6beeres.h
  enum BEEIMGS{
    pgoogle=0, pbaidu, pyahoo, pcleaner, phome, pupload, pwebcam, pvoice, pleave,
    pkey, pmagic, pexit, ptag, pgood, pbad, paddfav, pcut, pinfo, psetup, pstats,
    pglobe, pglobelink, ppages, pgreen, pup, pfolder, papp, precord, pstop, pdesc,
    psetup2, pfolder2, ppages2, pwebelement, pwebelement2, ppages3, papp2, plink,
    pclip, ppages4, pwebpage, pemptypage, pultraie, pmediadetect, pantivirus,
    pyellowbook, paudioon, paudiooff, palexa, palexaoff, pfavdown, pfavup, pfav,
    pfavsetup
  };


  // The height of toolbar button
  /// @todo should move out, or search box should be an separate DLL.
  const int ktbbuttonheight = 24;
  const int kWM_SE_GG      = WM_USER+550;
  const int kWM_SE_BAIDU   = WM_USER+551;
  const int kWM_SE_YAHOO   = WM_USER+552;
  const int kWM_SE_SETTING = WM_USER+553;

};


#endif