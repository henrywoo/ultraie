#pragma once

const COLORREF  INFO_EDGECOLOR = RGB(0, 0, 0);
const COLORREF  INFO_TEXT = RGB(0, 0, 0);

const COLORREF  SELECT_RECTCOLOR= RGB(255, 20, 255);
const COLORREF  CROSS_LINECOLOR = RGB(255, 0, 255);

const int INFO_WIDTH =300;
const int INFO_HEIGHT=350;

const int INFO_OFFSET   = 2;
const int TEXT_OFFSET   = 5;
const int INFO_HEADER_H = 5;

const RECT LEFT_RECT={INFO_OFFSET, INFO_OFFSET, INFO_OFFSET + INFO_WIDTH,INFO_OFFSET + INFO_HEIGHT};

const int content_height=110-TEXT_OFFSET;

// picture effect setting - TODO
const int border_size = 1;
const int shadow_size = 6;// width and height are both 6;

const int MINRECT=10;
const int HOTKEYID_COPYIMG=100;

#ifdef __CHINESE__
const wchar_t* const info_capture  =_T("双击以截取图片");
const wchar_t* const info_hotkey   =_T("Ctrl+C 复制图片到剪贴板");
const wchar_t* const info_deselect =_T("右键单击反选");
const wchar_t* const info_adjust   =_T("左键单击并拖动以调整截屏区域");
const wchar_t* const info_select   =_T("左键单击并拖动以选择截取区域");
const wchar_t* const info_exit     =_T("右键单击退出");
const wchar_t* const webele_cap =_T("左键单击开始截取");
#else
const wchar_t* const info_capture  =_T("Double left click to capture image.");
const wchar_t* const info_hotkey   =_T("Ctrl+C to copy image to clipboard.");
const wchar_t* const info_deselect =_T("Right click to de-select.");
const wchar_t* const info_adjust   =_T("Left click down and drag to adjust capture area.");
const wchar_t* const info_select   =_T("Left click down and drag to select capture area.");
const wchar_t* const info_exit     =_T("Right click to exit.");
const wchar_t* const webele_cap =_T("Left click to capture image/clip webpage block.");
#endif
//////////////////////////////////////////////////////////////////////////
const int INFO_HEIGHT_WEBELE=60;
const RECT INFO_RECT_WEBELE_LEFT={INFO_OFFSET, INFO_OFFSET, INFO_OFFSET + INFO_WIDTH,INFO_OFFSET + INFO_HEIGHT_WEBELE};


const int Thumb_Width =182;
const int Thumb_Height=137;

const COLORREF MASKPURPLE = RGB(255,0,255);
const COLORREF MASKGREY = RGB(193,193,193);
const COLORREF BGWHITE = RGB(253,253,253);
const COLORREF GREENLITE = RGB(240,250,210);