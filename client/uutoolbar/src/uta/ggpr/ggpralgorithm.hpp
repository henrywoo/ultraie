#ifndef __PLUGINS_GGPR_GGPRALGORITHM__
#define __PLUGINS_GGPR_GGPRALGORITHM__

#include <string>

namespace nsplugin{
  void GooglePRHash(unsigned int& a, unsigned int& b, unsigned int& c);
  std::wstring GoogleCH(std::wstring url);
  unsigned int GetGGPR(const wchar_t* url,const wchar_t* strIP);
};


#endif