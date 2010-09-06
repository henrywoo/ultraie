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