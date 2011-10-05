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

#ifndef __6BEEAUDIORECORDER_WRITESOUNDFILE__
#define __6BEEAUDIORECORDER_WRITESOUNDFILE__

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <BladeMP3EncDLL.h>

typedef struct writesoundfile_tag {
  TCHAR lpszFileName[MAX_PATH];
  WAVEFORMATEX waveFormatEx;
  TCHAR buffer[100];
} WRITESOUNDFILE, *PWRITESOUNDFILE;

class CMP3Creator
{
public:
  CMP3Creator(void);
  ~CMP3Creator(void);
  
  void CreateWaveFile(PWRITESOUNDFILE pwsf);
  BOOL CreateMP3File(PWRITESOUNDFILE pwsf);

  void WriteToSoundFile(WAVEHDR* pWriteHdr);
  void OnEndThread(WPARAM wParam, LPARAM lParam);

  void CloseSoundFile();
  void CloseMP3File();

  HMMIO m_hFile;
  MMCKINFO m_MMCKInfoData;
  MMCKINFO m_MMCKInfoParent;
  MMCKINFO m_MMCKInfoChild;
private:
  FILE*    pFileOut;
  char    *strFileOut;
  DWORD    dwSamples;
  DWORD    dwMP3Buffer;
  HBE_STREAM  hbeStream;
  BE_ERR    err;
  PBYTE    pMP3Buffer;
  DWORD dwRead;
  DWORD dwWrite;
  DWORD dwDone;
  DWORD dwFileSize;
public:
  PSHORT    pWAVBuffer;
  int       bLen;
};

#endif