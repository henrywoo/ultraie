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
#include "PlayMMSound.h"

/// @todo :ʵ�ֲ��ź��һ��֪ͨ��Ϣ!֪��ʲôʱ�򲥷����!

CPlayMMSound::CPlayMMSound()
{
  m_pPlaySound = NULL;
  wcscpy(m_FileName,_T(""));
  ZeroMemory(&m_MMCkInfoParent,sizeof(MMCKINFO));
  ZeroMemory(&m_MMCkInfoChild,sizeof(MMCKINFO));
  m_SoundBuffer = NULL;
  pb = NULL;
}
CPlayMMSound::~CPlayMMSound()
{
  if(m_SoundBuffer)
    delete m_SoundBuffer;
}
BOOL CPlayMMSound::OpenSoundFile(const TCHAR* csFileName){
  CloseSoundFile(0,0);
  m_hmmio = mmioOpen((LPWSTR)csFileName,NULL,MMIO_READ);
  if(!m_hmmio){
    _Error_MessageBox(_T("unable to open Sound MM File"));
    return FALSE;
  }
  m_MMCkInfoParent.fccType = mmioFOURCC('W','A','V','E');
  int errorCode = mmioDescend(m_hmmio, &m_MMCkInfoParent,NULL,MMIO_FINDRIFF);
  if(errorCode){
    _Error_MessageBox(_T("Error descending into file"));
    mmioClose(m_hmmio,0);
    m_hmmio = NULL;
    return FALSE;
  }
  m_MMCkInfoChild.ckid = mmioFOURCC('f','m','t',' ');
  errorCode = mmioDescend(m_hmmio,&m_MMCkInfoChild,&m_MMCkInfoParent,MMIO_FINDCHUNK);
  if(errorCode){
    _Error_MessageBox(_T("Error descending in file"));
    mmioClose(m_hmmio,0);
    m_hmmio = NULL;
    return FALSE;
  }
  DWORD bytesRead = mmioRead(m_hmmio,(LPSTR)&m_PCMWaveFmtRecord,m_MMCkInfoChild.cksize);
  if(bytesRead < 0){
    _Error_MessageBox(_T("Error reading PCM wave format record"));
    mmioClose(m_hmmio,0);
    return FALSE;
  }
  errorCode = mmioAscend(m_hmmio,&m_MMCkInfoChild,0);
  if(errorCode){
    _Error_MessageBox(_T("Error ascending in File"));
    mmioClose(m_hmmio,0);
    m_hmmio = NULL;
    return FALSE;
  }
  m_MMCkInfoChild.ckid = mmioFOURCC('d','a','t','a');
  errorCode = mmioDescend(m_hmmio,&m_MMCkInfoChild,&m_MMCkInfoParent,MMIO_FINDCHUNK);
  if(errorCode){
    _Error_MessageBox(_T("error reading data chunk"));
    mmioClose(m_hmmio,0);
    m_hmmio = NULL;
    return FALSE;
  }
  m_BytesToRead = m_PCMWaveFmtRecord.nChannels*(m_PCMWaveFmtRecord.wBitsPerSample/sizeof(BYTE))*m_PCMWaveFmtRecord.nBlockAlign*100;
  if(m_BytesToRead > (int)m_MMCkInfoChild.cksize){
    m_BytesToRead = m_MMCkInfoChild.cksize;
  }
  m_bContinuePlaying = TRUE;
  UINT nTID = 0;
  this->PlaySound();
  return TRUE;
}
BYTE* CPlayMMSound::ReadSoundFile(int*dwBytesRead)
{
  if(m_BytesToRead <= 0)
    return NULL;
  if(!m_hmmio)
    return NULL;
  BYTE * pSoundBuffer = new BYTE[m_BytesToRead];
  if(!pSoundBuffer)
    return NULL;
  DWORD dwRetc = mmioRead(m_hmmio,(LPSTR)pSoundBuffer, m_BytesToRead);
  if(dwRetc == -1)
  {
    _Error_MessageBox(_T("Error reading WAVE file\n"));
    if(pSoundBuffer)
      delete pSoundBuffer;
    return NULL;
  }else if(dwRetc == 0){
    if(pSoundBuffer)
      delete pSoundBuffer;
    CloseSoundFile(0,0);
    m_bContinuePlaying = FALSE;
    return NULL;
  }
  if(dwBytesRead)
    *dwBytesRead = dwRetc;
  char debugBuffer[MAX_PATH];
  sprintf(debugBuffer,"read %d bytes\n",dwRetc);
  return pSoundBuffer;
}
void  CPlayMMSound::CloseSoundFile(WPARAM wParam,LPARAM lParam)
{
  m_bContinuePlaying = FALSE;
  if(m_hmmio)
  {
    mmioClose(m_hmmio,0);
    m_hmmio = NULL;
    //pb->EnableWindow(TRUE);
  }
  return;
}

void CPlayMMSound::OnPlaySoundPtr(WPARAM wParam,LPARAM lParam){
  CPlaySound*pPlaySound = (CPlaySound*)lParam;
  m_pPlaySound = pPlaySound;
  return;
}

void CPlayMMSound::OnEndThread(WPARAM wParam, LPARAM lParam)
{
  CloseSoundFile(0,0);
  ::PostQuitMessage(0);
  return;
}

UINT CPlayMMSound::PlaySound()
{
  if(m_pPlaySound)
  {
    m_pPlaySound->OnStartPlaying();
  }else{
    return FALSE;
  }
  int dwBytesRead = 0;
  BYTE* pSoundBuffer = ReadSoundFile(&dwBytesRead);
  while(m_bContinuePlaying)
  {
    WAVEHDR* pWaveHdr = new WAVEHDR;
    ZeroMemory(pWaveHdr,sizeof(WAVEHDR));
    pWaveHdr->lpData = (char*)pSoundBuffer;
    pWaveHdr->dwBufferLength = dwBytesRead;
    if(m_pPlaySound)
      m_pPlaySound->OnWriteSoundData(pWaveHdr);
    else
      break;
    pSoundBuffer = ReadSoundFile(&dwBytesRead);
  }
  
  return TRUE;
}
