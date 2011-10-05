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

#include "pcrecpp_test.h"
#include <pcre++.h>

#include <iostream>
using namespace std;
using namespace pcrepp;

#ifdef NDEBUG
#pragma comment(lib,"pcre++.lib")
#else
#pragma comment(lib,"pcre++d.lib")
#endif


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(pcrecpp_test,"ultratest");

void pcrecpp_test::setUp(){}

void pcrecpp_test::tearDown(){}

void pcrecpp_test::testmatch(){

  //__int64 a=164281*1024;
  //__int64 b=2402304*1000;
  cout << (int)(2402304/(164281*(1024/1000))) << endl;

  int i();
  string s="123 234 usd asd 2 35";
  Pcre re(string("(\\d+)"));
  string n = re.replace(s, "zweiundzwanzig($1) ");
  string n2 = re.replace(s, "zweiundzwanzig");

  cout << n.c_str() << endl;
  cout << n2.c_str() << endl;


  string hostname = "http://www.ultraie.com";
  string html= "<a \
    href='css/s.gif'>hello</a>\
    <link href=\"css/css_main.css\" rel=\"stylesheet\" type=\"text/css\"> \
    <link rel=\"shortcut icon\" href=\"favicon.ico\" > \
    </HEAD> \
    <BODY> \
    <td width=\"16\"><img src=\"images/new_top_1.gif\" width=\"16\" height=\"7\" /></td> \
    <td background=\"images/new_top_2.gif\"><img src=\"images/new_top_2.gif\" width=\"16\" height=\"7\" /></td> \
    <td width=\"15\"><img src=\"images/new_top_3.gif\" width=\"15\" height=\"7\" /></td> \
    </tr> \
    <tr> \
    <td width=\"16\" background=\"images/center_2_01.gif\"><IMG SRC=\"images/center_2_01.gif\" WIDTH=16 HEIGHT=24 ALT=\"\"></td> \
    <td><table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\"> \
    <tr> \
    <td width=\"194\"><a href=\"index.php\"><img src=\"images/new_logo.gif\" width=\"194\" height=\"81\" border=\"0\"></a></td> \
    <td align=\"right\" valign=\"top\" background=\"images/header_bg.jpg\"> \
    <img src=\"images/home_top.gif\" width=\"192\" height=\"41\" border=\"0\" usemap=\"#Map\" /></br>\
    <img src=\"images/english_for_everyone.gif\" width=\"225\" height=\"24\" border=\"0\"/> \
    <map name=\"Map\" id=\"Map\"> \
    <area shape=\"rect\" coords=\"0,3,40,20\" href=\"http://www.eslpod.com/website\" alt=\"ESL Podcast Home\" /> \
    <area shape=\"rect\" coords=\"66,5,109,20\" href=\"http://www.linguapod.com/eslpodcaststore/\" alt=\"ESL Podcast Store\" /> \
    <area shape=\"rect\" coords=\"135,4,173,20\" href=\"http://eslpod.com/eslpod_blog/\" alt=\"ESL Podcast Blog\"/> \
    </map> \
    </td> \
    </tr> \
    </table></td> \
    <td width=\"15\" background=\"images/center_2_04.gif\"><IMG SRC=\"images/center_2_04.gif\" WIDTH=15 HEIGHT=24 ALT=\"\"></td> \
    </tr>";

  Pcre reg("([^\\n]+\\n)");
  string str = "\nline1\nline2\nline3\n";
  size_t pos = 0;

  while (pos <= str.length()) {
    if( reg.search(str, (int)pos)) {
      pos = reg.get_match_end(0);
      cout << "   pos: " << pos << " match: " << reg.get_match(0);
    }
    else
      break;
  }

  Pcre reg2("<img\\s+.*?src=[\"|']?([^>\"'\\s]+)[\"|']?.*?\\s*/?>");
  size_t pos2 = 0;
  size_t html_size = html.length();
  while(pos2 <= html_size ){
    if(reg2.search(html,(int)pos2)) {
      pos2 = reg2.get_match_end(0);
      cout << "pos: " << pos2 << " match: " << reg2.get_match(0) /*<< "\n" << reg2.get_match(1) */<< endl;
    }
    else
      break;
  }

  Pcre reg3("<link\\s+.*?href=[\"|']?([^>\"'\\s]+)[\"|']?.*?\\s*/?>");
  html_size = html.length();
  pos2 = 0;
  while(pos2 <= html_size ){
    if(reg3.search(html,(int)pos2)) {
      pos2 = reg3.get_match_end(0);
      cout << "pos: " << pos2 << " match: " << reg3.get_match(0) /*<< "\n" << reg3.get_match(1) */<< endl;
    }
    else
      break;
  }

  {
    html= "<a href='css/s.gif'>hello</a><a href=http://www.6bee.com/css.gif>";
    Pcre reg4("<a\\s+.*?href=([\"|']?)([^>\"'\\shttp]+)([\"|']?.*?\\s*/?)>",PCRE_CASELESS);
    string replace_str = "<a href=$1http://www.ultraie.com/$2$3>";
    /*
    size_t pos2 = 0;
    size_t html_size = html.length();
    while(pos2 <= html_size ){
      if(reg4.search(html,(int)pos2)) {
        pos2 = reg4.get_match_end(0);
        cout << "pos: " << pos2 << " match: " << reg4.get_match(0) << endl;
      }
      else
        break;
    }*/
    //string n = reg4.replace(html, "<a href=\"http://www.ultraie.com/$1\">");
    html = reg4.replace(html, replace_str);
    cout << html.c_str() << endl;
  }
  

}
