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
