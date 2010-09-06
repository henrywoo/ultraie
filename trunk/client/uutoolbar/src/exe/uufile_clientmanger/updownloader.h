#pragma once

#include <vector>
#include <string>
#include <pcre++.h>
#include "6bees_net.h"
#include <map>

#ifdef NDEBUG
#pragma comment(lib,"pcre++.lib")
#else
#pragma comment(lib,"pcre++d.lib")
#endif

using namespace std;
using namespace _6bees_net;
using namespace pcrepp;

//Format:
//-m upload -f "XXXX" "YYYY" "ZZZZ"
//-m download -l "http://xxx.txt" -f "xxx.txt" -l "ftp://yyy.txt" -f "yyy.txt"
//-m thumbupload -f "localthumbname" "remotethumbname"
bool ParseCmd(string cmdline,
                  UPLOADERMODE& mode,
                  vector<wstring>* r);

int CallUploader(const wchar_t* cmdline);