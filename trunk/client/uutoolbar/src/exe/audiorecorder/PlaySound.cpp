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

#include "StdAfx.h"
#include "PlaySound.h"

CPlaySound::CPlaySound(int iHertz){
  m_nOutputBuffers = 0;
  m_nMaxOutputBuffers = 0;
  memset(&m_WaveFormatEx,0x00,sizeof(m_WaveFormatEx));
  m_WaveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
  m_WaveFormatEx.nChannels = 1;
  m_WaveFormatEx.wBitsPerSample = 16;
  m_WaveFormatEx.cbSize = 0;
  m_WaveFormatEx.nSamplesPerSec = iHertz; //20050;
  m_WaveFormatEx.nAvgBytesPerSec = m_WaveFormatEx.nSamplesPerSec*(m_WaveFormatEx.wBitsPerSample/8);
  m_WaveFormatEx.nBlockAlign = (m_WaveFormatEx.wBitsPerSample/8)*  m_WaveFormatEx.nChannels;
  m_bPlay = FALSE;
}


CPlaySound::~CPlaySound(void){}

void CPlaySound::OnStartPlaying()
{
  DWORD mmReturn = 0;
  if(m_bPlay)  return;
  if(!m_bPlay)
  {
    MMRESULT mmReturn = ::waveOutOpen( &m_hPlay, WAVE_MAPPER, &m_WaveFormatEx, ::GetCurrentThreadId(), 0, CALLBACK_THREAD);// open wave in device??
    if(mmReturn)
    {
      TCHAR errorbuffer[MAX_PATH];
      TCHAR errorbuffer1[MAX_PATH];
      waveOutGetErrorText( mmReturn,errorbuffer,MAX_PATH);
      wsprintf(errorbuffer1,_T("WAVEOUT:%x:%s"),mmReturn,errorbuffer);
      _Error_MessageBox(errorbuffer1);
    }else{
      m_bPlay = TRUE;
    }
  }
  return;
}
void CPlaySound::OnStopPlaying(WPARAM wParam, LPARAM lParam)
{
  MMRESULT mmReturn = 0;
  if(!m_bPlay)
    return;

  if(m_bPlay)
  {
    MSG msg;
    mmReturn = ::waveOutReset(m_hPlay);
    Sleep(20);
    while(::PeekMessage(&msg,NULL,MM_WOM_OPEN,MM_WOM_DONE,PM_REMOVE))
    {
      if(msg.message == MM_WOM_OPEN || msg.message == MM_WOM_CLOSE)
        continue;

      OnEndPlaySoundData(msg.wParam,msg.lParam);
      Sleep(5);
    }
    if(!mmReturn)
      m_bPlay = FALSE;
    Sleep(500);
    if(!mmReturn)
      mmReturn = ::waveOutClose(m_hPlay);
    return;
  }
  return;
}

void CPlaySound::OnEndPlaySoundData(WPARAM wParam, LPARAM lParam)
{
  LPWAVEHDR lpHdr = (LPWAVEHDR) lParam;
  if(lpHdr)
  {
    ::waveOutUnprepareHeader(m_hPlay, lpHdr, sizeof(WAVEHDR));

    if(lpHdr->lpData)
      delete ((BYTE*) lpHdr->lpData);
    delete lpHdr;
  }
  return;
}

void CPlaySound::OnWriteSoundData(LPWAVEHDR _lpHdr)
{
  LPWAVEHDR lpHdr = _lpHdr;
  MMRESULT mmResult = 0;
  if(lpHdr)
  {
    char debugbuffer[256];
    sprintf(debugbuffer, "SOUND BUFFER written: %d, %d\n",lpHdr->dwBufferLength,m_nOutputBuffers);
    if(m_bPlay)
    {
      BYTE* lpInt = (BYTE*) lpHdr->lpData;
      DWORD dwSamples = lpHdr->dwBufferLength/sizeof(BYTE);
      ProcessSoundData(lpInt, dwSamples);
      mmResult = ::waveOutPrepareHeader(m_hPlay, lpHdr, sizeof(WAVEHDR));
      if(mmResult)
        _Error_MessageBox(_T("error from waveoutprepareheader\n"));
      mmResult = ::waveOutWrite(m_hPlay, lpHdr, sizeof(WAVEHDR));
      if(mmResult)
        _Error_MessageBox(_T("error from waveoutwrite\n"));
      m_nOutputBuffers++;
    }
  }
  return;
}

LPWAVEHDR CPlaySound::CreateWaveHeader()
{
  LPWAVEHDR lpHdr = new WAVEHDR;
  ZeroMemory(lpHdr, sizeof(WAVEHDR));
  BYTE* lpByte = new BYTE[(m_WaveFormatEx.nBlockAlign*SOUNDSAMPLES)];

  lpHdr->lpData = (char *) lpByte;
  lpHdr->dwBufferLength = (m_WaveFormatEx.nBlockAlign*SOUNDSAMPLES);
  return lpHdr;

}

void CPlaySound::ProcessSoundData(BYTE *sound, DWORD dwSamples){}

void CPlaySound::OnEndThread(WPARAM wParam, LPARAM lParam)
{
  if(m_bPlay)
    OnStopPlaying(0,0);
  ::PostQuitMessage(0);
  return;
}