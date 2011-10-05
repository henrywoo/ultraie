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
#include "RecordSound.h"

CRecordSound::CRecordSound(int iHertz){
  memset(&m_WaveFormatEx, 0x00, sizeof(m_WaveFormatEx)/*=18*/);
  m_WaveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
  m_WaveFormatEx.nChannels = 1;//不能改
  m_WaveFormatEx.wBitsPerSample = 16;
  m_WaveFormatEx.cbSize = 0;
  m_WaveFormatEx.nSamplesPerSec = iHertz;
  m_WaveFormatEx.nAvgBytesPerSec = m_WaveFormatEx.nSamplesPerSec*(m_WaveFormatEx.wBitsPerSample/8);//=441000*2
  m_WaveFormatEx.nBlockAlign = (m_WaveFormatEx.wBitsPerSample/8)*  m_WaveFormatEx.nChannels;//=2*1

  m_bRecording = FALSE;
  m_Pause      = FALSE;
  m_Player     = NULL;
  m_pWriter     = NULL;
  m_iSoundBuffers =0;
}

CRecordSound::~CRecordSound(void){}

LPWAVEHDR CRecordSound::CreateWaveHeader(){
  LPWAVEHDR lpWaveHdr = new WAVEHDR;
  ZeroMemory(lpWaveHdr, sizeof(WAVEHDR));
  BYTE* lpByte = new BYTE[(m_WaveFormatEx.nBlockAlign*SOUNDSAMPLES)];

  lpWaveHdr->lpData = (char *) lpByte;
  lpWaveHdr->dwBufferLength = (m_WaveFormatEx.nBlockAlign*SOUNDSAMPLES);
  return lpWaveHdr;
}

void CRecordSound::AllocateBuffers(int nBuffers/*=5*/) {
  int i;
  for(i=0; i < nBuffers; i++){
    LPWAVEHDR lpWaveHdr = CreateWaveHeader();
    ::waveInPrepareHeader(m_hRecord, lpWaveHdr, sizeof(WAVEHDR));
    ::waveInAddBuffer    (m_hRecord, lpWaveHdr, sizeof(WAVEHDR));
    //The waveInAddBuffer function sends an input buffer to the given waveform-audio input device. When the buffer is filled, the application is notified.
    m_iSoundBuffers++;
  }
}

void CRecordSound::ProcessSoundData(BYTE* sound, DWORD dwSamples){
  /// @todo :
}

BOOL CRecordSound::StartRecord(){
  DWORD mmReturn = 0;
  if(m_bRecording)
    return FALSE;
  if(!m_bRecording){
    // open wave in device
    //WAVIN有一个回调函数
    //第二个参数设为WAVE_MAPPER。这意味着，你不在意具体使用哪个设备，由系统决定。
#pragma warning( disable: 4311)
#pragma warning( disable: 4312)
    MMRESULT mmReturn=(MMRESULT)::waveInOpen(&m_hRecord,
      WAVE_MAPPER,
      &m_WaveFormatEx,
      (DWORD)&OnCallbackBlock,
      (DWORD)this,
      (DWORD)CALLBACK_FUNCTION);
    if(mmReturn){
      TCHAR errorbuffer[MAX_PATH];
      TCHAR errorbuffer1[MAX_PATH];
      waveInGetErrorText(mmReturn,errorbuffer,MAX_PATH);
      wsprintf(errorbuffer1,_T("WAVEIN:%x:%s"),mmReturn,errorbuffer);
      _Error_MessageBox(errorbuffer1);
    }else{
      if(m_pWriter){
        WRITESOUNDFILE wsf={};
        wcscpy(wsf.lpszFileName,_T("sound.wav"));
        memcpy(&wsf.waveFormatEx,&m_WaveFormatEx,sizeof(m_WaveFormatEx));
        m_pWriter->CreateWaveFile(&wsf);
        if (!m_pWriter->CreateMP3File(&wsf)){
          return FALSE;
        }
        /// @todo 
        //m_pWriter->PostThreadMessage(WM_WRITESOUNDFILE_FILENAME, 0,(LPARAM)pwsf);
        //m_pWriter->PostThreadMessage(WM_WRITESOUNDFILE_FILENAME_MP3, 0,(LPARAM)pwsf);
      }
      AllocateBuffers(MAXINPUTBUFFERS);
      mmReturn = ::waveInStart(m_hRecord);//开始录音,WMI_DATA有数据了
      if(mmReturn!=MMSYSERR_NOERROR){
        TCHAR errorbuffer[MAX_PATH];
        TCHAR errorbuffer1[MAX_PATH];
        waveInGetErrorText(mmReturn, errorbuffer, MAX_PATH);
        wsprintf(errorbuffer1,_T("WAVEIN:%x:%s"),mmReturn,errorbuffer);
        _Error_MessageBox(errorbuffer1);
      }else{
        m_bRecording = TRUE;//正在录音
        return TRUE;
      }
    }
  }
  return TRUE;
}

void CRecordSound::OnSoundBlock(HWAVEIN hwl,UINT uMsg,DWORD dwInstance,DWORD dwParam1, DWORD dwParam2){
  LPWAVEHDR lpWaveHdr = (LPWAVEHDR) dwParam1;
  if(lpWaveHdr && uMsg == WIM_DATA){
    /*The WIM_DATA message is sent to the given waveform-audio input callback function when waveform-audio data is present in the input buffer 
    and the buffer is being returned to the application.The message can be sent when the buffer is full or after the waveInReset function is called.*/
    BYTE * lpInt = (BYTE*) lpWaveHdr->lpData;
    DWORD iRecorded = lpWaveHdr->dwBytesRecorded;
    if(m_bRecording)
      ::waveInUnprepareHeader(m_hRecord, lpWaveHdr, sizeof(WAVEHDR));
    //在保存系统返回的音频数据块之前，需要调用waveInUnprepareHeader发送一个通知告诉设备驱动程序该数据块已不能用于录音
    //好像什么也没有做
    /// @todo :
    ProcessSoundData(lpInt, iRecorded/sizeof(BYTE));
    if(m_pWriter && m_bRecording){
      WAVEHDR WriteHdr={};
      memcpy(&WriteHdr,lpWaveHdr,sizeof(WAVEHDR));
      if (!m_Pause){
        m_pWriter->WriteToSoundFile(&WriteHdr);
      }
    }
    //echo sound
    if(m_Player && m_bRecording){  
      /// @todo 
      //m_Player->PostThreadMessage(WM_PLAYSOUND_PLAYBLOCK, GetCurrentThreadId(),(LPARAM) lpWaveHdr);
    }else{
      delete lpInt;
      delete lpWaveHdr;
    }
    TCHAR debugbuffer[256];    
    wsprintf(debugbuffer, _T("SOUND BUFFER returned: %d,%d\n"),iRecorded,m_iSoundBuffers);

    m_iSoundBuffers--;
    if(m_bRecording){
      AllocateBuffers(1);//给录音设备添加数据块以继续录音，否则会因数据块耗尽而停止录音。
    }
  }
  return;
}

void CRecordSound::StopRecord(){
  MMRESULT mmReturn = 0;
  if(!m_bRecording)
    return;

  if(m_bRecording){
    m_bRecording = FALSE;
    mmReturn = ::waveInStop(m_hRecord);
    OutputDebugString(_T("waveinstop\n"));
    mmReturn = ::waveInReset(m_hRecord);  
    // give callback time to get rid of spare sound buffers
    Sleep(500);
    if(!mmReturn)
      mmReturn = ::waveInClose(m_hRecord);
    if(m_Player){
      /// @todo 
      //m_Player->PostThreadMessage(WM_PLAYSOUND_STOPPLAYING,0,0);
    }
    if(m_pWriter){
      m_pWriter->CloseSoundFile();
      m_pWriter->CloseMP3File();
      /// @todo 
      //m_pWriter->PostThreadMessage(WM_WRITESOUNDFILE_CLOSEFILE_MP3,0,0);
      //m_pWriter->PostThreadMessage(WM_WRITESOUNDFILE_CLOSEFILE,0,0);
    }
    return;
  }
  return;
}

void CRecordSound::OnEndThread(WPARAM wParam, LPARAM lParam){
  if(m_bRecording){
    StopRecord();
  }
  ::PostQuitMessage(0);
  return;
}

void CRecordSound::OnPtrSoundWriter(CMP3Creator* _Writer){
  m_pWriter = _Writer;
  return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this is a C style callback, not a part of the class it receives a instance pointer which is the pointer to the CRecordSound class
void CALLBACK OnCallbackBlock(HWAVEIN hwl,UINT uMsg,DWORD dwInstance,DWORD dwParam1, DWORD dwParam2){

  CRecordSound* pCallback = (CRecordSound*) dwInstance;
  if(pCallback)
    pCallback->OnSoundBlock(hwl,uMsg,dwInstance,dwParam1,dwParam2);
}
