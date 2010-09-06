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