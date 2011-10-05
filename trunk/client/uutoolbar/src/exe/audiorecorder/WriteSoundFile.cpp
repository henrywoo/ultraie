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
#include "WriteSoundFile.h"
#include "../../dll/6beecommond/6beecommond.h"

BEINITSTREAM    beInitStream     =NULL;
BEENCODECHUNK    beEncodeChunk    =NULL;
BEDEINITSTREAM    beDeinitStream   =NULL;
BECLOSESTREAM    beCloseStream    =NULL;
BEVERSION    beVersion        =NULL;
BEWRITEVBRHEADER  beWriteVBRHeader =NULL;
BEWRITEINFOTAG    beWriteInfoTag   =NULL;

CMP3Creator::CMP3Creator(void):m_hFile(0){
  pMP3Buffer = NULL;
  pWAVBuffer = NULL;
  dwRead     = 0;
  dwWrite    = 0;
  dwDone     = 0;
  dwFileSize = 0;
}

CMP3Creator::~CMP3Creator(void){}

//收到WM_WRITESOUNDFILE_FILENAME的消息，然后创建文件！
void CMP3Creator::CreateWaveFile(PWRITESOUNDFILE pwsf){
  int cbWaveFormatEx = sizeof(WAVEFORMATEX) + pwsf->waveFormatEx.cbSize;
  m_hFile = ::mmioOpen(pwsf->lpszFileName,NULL, MMIO_CREATE|MMIO_WRITE|MMIO_EXCLUSIVE|MMIO_ALLOCBUF);
  if(!m_hFile) return;
  //write the first 44 bytes contents
  ZeroMemory(&m_MMCKInfoParent, sizeof(MMCKINFO));
  m_MMCKInfoParent.fccType = mmioFOURCC('W','A','V','E');
  MMRESULT mmResult        = ::mmioCreateChunk( m_hFile,&m_MMCKInfoParent,MMIO_CREATERIFF);
  ZeroMemory(&m_MMCKInfoChild, sizeof(MMCKINFO));
  m_MMCKInfoChild.ckid = mmioFOURCC('f','m','t',' ');
  m_MMCKInfoChild.cksize = cbWaveFormatEx;
  mmResult = ::mmioCreateChunk(m_hFile, &m_MMCKInfoChild, 0);

  mmResult = ::mmioWrite(m_hFile, (char*)&pwsf->waveFormatEx, cbWaveFormatEx);
  mmResult = ::mmioAscend(m_hFile, &m_MMCKInfoChild, 0);
  m_MMCKInfoChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
  mmResult = ::mmioCreateChunk(m_hFile, &m_MMCKInfoChild, 0);
  bLen = 0;
  return;
}


BOOL CMP3Creator::CreateMP3File(PWRITESOUNDFILE pwsf){
  ::_wchdir(_6beed_util::Get6BEEPath());
  HINSTANCE  hDLL=LoadLibrary(_T("lame_enc.dll"));
  if( NULL == hDLL ){
    _Error_MessageBox(_T("Error loading lame_enc.DLL"));
    return FALSE;
  }
  BE_VERSION  Version    ={0,};
  BE_CONFIG  beConfig  ={0,};
  pFileOut    =NULL;
  strFileOut    ="sound.mp3";
  dwSamples    =0;
  dwMP3Buffer    =0;
  hbeStream    =0;
  err        =0;
  pMP3Buffer    =NULL;
  pWAVBuffer    =NULL;

  beInitStream  = (BEINITSTREAM) GetProcAddress(hDLL, TEXT_BEINITSTREAM);
  beEncodeChunk  = (BEENCODECHUNK) GetProcAddress(hDLL, TEXT_BEENCODECHUNK);
  beDeinitStream  = (BEDEINITSTREAM) GetProcAddress(hDLL, TEXT_BEDEINITSTREAM);
  beCloseStream  = (BECLOSESTREAM) GetProcAddress(hDLL, TEXT_BECLOSESTREAM);
  beVersion    = (BEVERSION) GetProcAddress(hDLL, TEXT_BEVERSION);
  beWriteVBRHeader= (BEWRITEVBRHEADER) GetProcAddress(hDLL,TEXT_BEWRITEVBRHEADER);
  beWriteInfoTag  = (BEWRITEINFOTAG) GetProcAddress(hDLL,TEXT_BEWRITEINFOTAG);
  if(!beInitStream || !beEncodeChunk || !beDeinitStream || !beCloseStream || !beVersion || !beWriteVBRHeader){
    _Error_MessageBox(_T("Unable to get LAME interfaces"));
    return FALSE;
  }

  beVersion( &Version );
  pFileOut= fopen(strFileOut,"wb+");
  if(pFileOut == NULL){
    _Error_MessageBox(_T("Error creating file"));
    return FALSE;
  }
  memset(&beConfig,0,sizeof(beConfig));
  beConfig.dwConfig = BE_CONFIG_LAME;

  beConfig.format.LHV1.dwStructVersion  = 1;
  beConfig.format.LHV1.dwStructSize    = sizeof(beConfig);    
  beConfig.format.LHV1.dwSampleRate    = 44100;        // INPUT FREQUENCY
  beConfig.format.LHV1.dwReSampleRate    = 0;          // DON"T RESAMPLE
  beConfig.format.LHV1.nMode        = BE_MP3_MODE_MONO ;
  beConfig.format.LHV1.dwBitrate      = 32;          // MINIMUM BIT RATE
  beConfig.format.LHV1.dwMaxBitrate    = 48;          // MAXIMUM BIT RATE
  beConfig.format.LHV1.nPreset      = LQP_R3MIX;      // QUALITY PRESET SETTING
  beConfig.format.LHV1.dwMpegVersion    = MPEG1;        // MPEG VERSION (I or II)
  beConfig.format.LHV1.dwPsyModel      = 0;          // USE DEFAULT PSYCHOACOUSTIC MODEL 
  beConfig.format.LHV1.dwEmphasis      = 0;          // NO EMPHASIS TURNED ON
  beConfig.format.LHV1.bOriginal      = TRUE;          // SET ORIGINAL FLAG
  beConfig.format.LHV1.bWriteVBRHeader  = TRUE;          // Write INFO tag
  beConfig.format.LHV1.bNoRes        = TRUE;          // No Bit reservoir
  beConfig.format.LHV1.bEnableVBR      = TRUE;          // USE VBR
  beConfig.format.LHV1.nVBRQuality    = 5;          // SET VBR QUALITY


  err = beInitStream(&beConfig, &dwSamples, &dwMP3Buffer, &hbeStream);
  if(err != BE_ERR_SUCCESSFUL){
    _Error_MessageBox(_T("Error opening encoding stream"));
    return FALSE;
  }
  pMP3Buffer = new BYTE[dwMP3Buffer];
  pWAVBuffer = new SHORT[dwSamples];

  if(!pMP3Buffer){
    ::MessageBox(NULL,_T("Out of memory"),_T("ERROR"),NULL);
    return FALSE;
  }
  dwRead=0;
  dwWrite=0;
  dwDone=0;
  dwFileSize=0;

  //=====================================
  return TRUE;
}


/* wave data block header */
//typedef struct wavehdr_tag {
//    LPSTR       lpData;                 /* pointer to locked data buffer */
//    DWORD       dwBufferLength;         /* length of data buffer */
//    DWORD       dwBytesRecorded;        /* used for input only */
//    DWORD_PTR   dwUser;                 /* for client's use */
//    DWORD       dwFlags;                /* assorted flags (see defines) */
//    DWORD       dwLoops;                /* loop control counter */
//    struct wavehdr_tag FAR *lpNext;     /* reserved for driver */
//    DWORD_PTR   reserved;               /* reserved for driver */
//} WAVEHDR, *PWAVEHDR, NEAR *NPWAVEHDR, FAR *LPWAVEHDR;


void CMP3Creator::WriteToSoundFile(WAVEHDR* pWriteHdr){
  static const int ZOOM=25;
  bLen=pWriteHdr->dwBufferLength;
  if(pWriteHdr){
    char *soundbuffer = (char*)pWriteHdr->lpData;
    if(m_hFile && soundbuffer){
      //..................写文件.................
      /*
      The mmioWrite function writes a specified number of bytes to a file opened by using the mmioOpen function.
      LONG mmioWrite(
      HMMIO hmmio,  
      char _huge* pch,
      LONG cch
      );
      Returns the number of bytes actually written. If there is an error writing to the file, the return value is -1.
      */
      //从soundbuffer里面写1000单位的字节到文件m_hFile里面
      
      ::mmioWrite(m_hFile,//File handle of the file
        soundbuffer,//Pointer to the buffer to be written to the file.
        bLen//=1000 number of bytes
      );

      pWAVBuffer=(PSHORT)pWriteHdr->lpData;
      
      //改变音量大小 - ZOOM 可以定制!
      /// @todo : 回调函数画波形
      for (int i=0;i<bLen/2;i++){
        if (pWAVBuffer[i]<=SHRT_MAX/ZOOM && pWAVBuffer[i]>=SHRT_MIN/ZOOM)
          pWAVBuffer[i] = ZOOM*pWAVBuffer[i];
      }
      
      dwRead = bLen/2;
      err = beEncodeChunk(hbeStream, dwRead, pWAVBuffer, pMP3Buffer, &dwWrite);
      //pWAVBuffer[dwSamples]
      int st = (int)fwrite(pMP3Buffer,1,dwWrite,pFileOut);
      if(st != dwWrite){//1字节1字节的写(byte)
        _Error_MessageBox(_T("Output file write error"));
        return;
      }
      dwDone += dwRead*sizeof(SHORT);
    }
  }
  return;
}
void CMP3Creator::CloseSoundFile(){  
  if(m_hFile){
    ::mmioAscend(m_hFile, &m_MMCKInfoChild, 0);
    ::mmioAscend(m_hFile, &m_MMCKInfoParent, 0);
    ::mmioClose(m_hFile, 0);
    m_hFile = NULL;
  }
  return;
}

void CMP3Creator::CloseMP3File(){
  err = beDeinitStream(hbeStream, pMP3Buffer, &dwWrite);
  if(err != BE_ERR_SUCCESSFUL){
    beCloseStream(hbeStream);
    _Error_MessageBox(_T("beExitStream failed!!!"));
    return;
  }
  if( dwWrite ){
    if( fwrite( pMP3Buffer, 1, dwWrite, pFileOut ) != dwWrite ){
      _Error_MessageBox(_T("Output file write error..."));
      return;
    }
  }
  beCloseStream( hbeStream );
  delete [] pWAVBuffer;
  delete [] pMP3Buffer;
  fclose( pFileOut );

  if(beWriteInfoTag ){
    beWriteInfoTag( hbeStream, strFileOut );
  }else{
    beWriteVBRHeader( strFileOut );
  }
  return;
}

void CMP3Creator::OnEndThread(WPARAM wParam, LPARAM lParam){
  CloseSoundFile();
  ::PostQuitMessage(0);
  return;
}
