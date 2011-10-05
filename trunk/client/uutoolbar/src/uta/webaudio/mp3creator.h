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

#ifndef __PLUGINS_WEBAUDIOREC_MP3CREATOR__
#define __PLUGINS_WEBAUDIOREC_MP3CREATOR__

#include <windows.h>
#include <stdio.h>
#include <BladeMP3EncDLL.h>
#include <atlstr.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

namespace nsplugin{

  typedef struct writesoundfile_tag {
    TCHAR lpszFileName[MAX_PATH];
    WAVEFORMATEX waveFormatEx;
    TCHAR buffer[100];
  } WRITESOUNDFILE, *PWRITESOUNDFILE;


  class CMP3Creator{
    FILE*      pFileOut;
    DWORD    dwSamples;
    DWORD    dwMP3Buffer;
    HBE_STREAM    hbeStream;
    BE_ERR    err;
    PBYTE      pMP3Buffer;

    DWORD dwRead;
    DWORD dwWrite;
    DWORD dwFileSize;

  public:
    CMP3Creator();
    ~CMP3Creator();

    //bool CreateWaveFile(const ATL::CString&);
    void WriteDatatoFile(char* p,int len);
    //void CloseSoundFile();

    BOOL CreateMP3File(const ATL::CStringA&);
    void CloseMP3File();

    //HMMIO m_hFile;
    //MMCKINFO m_MMCKInfoData;
    //MMCKINFO m_MMCKInfoParent;
    //MMCKINFO m_MMCKInfoChild;

    PSHORT pWAVBuffer;
    DWORD dwDone;//size for wav file
    DWORD dwDone_mp3;//size for mp3 file

    ATL::CStringA  Mp3FilePath;
    bool m_breadytorecordmp3;
  };

};


#endif