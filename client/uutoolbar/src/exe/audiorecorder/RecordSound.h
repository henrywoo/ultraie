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

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "PlaySound.h"
#include "WriteSoundFile.h"



class CRecordSound
{
public:
  CRecordSound(int iHertz=HERTZ);
  ~CRecordSound(void);

  LPWAVEHDR CreateWaveHeader();
  void AllocateBuffers(int nBuffers);

  virtual void ProcessSoundData(BYTE* sound, DWORD dwSamples);
  BOOL StartRecord();
  void StopRecord();

  void OnPtrSoundPlayer(WPARAM wParam, LPARAM lParam);
  void OnPtrSoundWriter(CMP3Creator* m_pWriter);

  void OnEndThread(WPARAM wParam, LPARAM lParam);
  void OnSoundBlock(HWAVEIN hwl,UINT uMsg,DWORD dwInstance,DWORD dwParam1, DWORD dwParam2);
  
protected:
  HWAVEIN          m_hRecord;
  WAVEFORMATEX     m_WaveFormatEx;
  int              m_iSoundBuffers;

  CPlaySound*      m_Player;
  CMP3Creator* m_pWriter;

public:
  BOOL             m_bRecording;
  BOOL       m_Pause;
};


// this is a C style callback, not a part of the class
// it receives a instance pointer which is the pointer to
// the CRecordSound class
void CALLBACK OnCallbackBlock(HWAVEIN hwl,UINT uMsg,DWORD dwInstance,DWORD dwParam1, DWORD dwParam2);