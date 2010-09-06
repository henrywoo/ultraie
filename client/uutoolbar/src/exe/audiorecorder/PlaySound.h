#pragma once
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <math.h>

class CPlaySound
{
public:
  CPlaySound(int iHertz=44000);
  ~CPlaySound(void);

  void OnStartPlaying();
  void OnStopPlaying(WPARAM wParam, LPARAM lParam);
  void OnEndPlaySoundData(WPARAM wParam, LPARAM lParam);
  void OnWriteSoundData(LPWAVEHDR _lpHdr);
  void OnEndThread(WPARAM wParam, LPARAM lParam);

  void ProcessSoundData(BYTE *sound, DWORD dwSamples);
private:
  int  m_nOutputBuffers;
  int m_nMaxOutputBuffers;
  WAVEFORMATEX m_WaveFormatEx;
  BOOL m_bPlay;
  HWAVEOUT m_hPlay;

protected:
  LPWAVEHDR CreateWaveHeader();

  
};
