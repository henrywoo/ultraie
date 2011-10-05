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

#pragma once
#include "PlaySound.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


class CPlayMMSound
{
public:
  CPlayMMSound();
  ~CPlayMMSound();

  BYTE* ReadSoundFile(int*dwBytesRead);
  BOOL OpenSoundFile(const TCHAR* csFileName);
  void OnEndThread(WPARAM wParam, LPARAM lParam);

  UINT PlaySound();

  void CloseSoundFile(WPARAM wParam, LPARAM lParam);
  void OnPlaySoundPtr(WPARAM wParam, LPARAM lParam);

  TCHAR m_FileName[MAX_PATH];

  HMMIO m_hmmio;
  MMCKINFO m_MMCkInfoParent;
  MMCKINFO m_MMCkInfoChild;
  WAVEFORMATEX m_PCMWaveFmtRecord;
  BYTE m_WaveFormatExtras[MAX_PATH]; // for non PCM waveformatex there are extra bytes at the end
  BYTE * m_SoundBuffer;
  int m_BytesToRead;

  CPlaySound* m_pPlaySound;
  BOOL m_bContinuePlaying;
  CWindow* pb;
};
