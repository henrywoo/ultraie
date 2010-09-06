#include "mp3creator.h"
#include "../../dll/6beecommond/6beecommond.h"
#include <string>
#include "6bees_str.h"
using namespace _6bees_str;

namespace nsplugin{

  BEINITSTREAM    beInitStream     =NULL;
  BEENCODECHUNK    beEncodeChunk    =NULL;
  BEDEINITSTREAM    beDeinitStream   =NULL;
  BECLOSESTREAM    beCloseStream    =NULL;
  BEVERSION      beVersion        =NULL;
  BEWRITEVBRHEADER  beWriteVBRHeader =NULL;
  BEWRITEINFOTAG    beWriteInfoTag   =NULL;

  CMP3Creator::CMP3Creator():dwDone_mp3(0),dwDone(0)
  {
    m_breadytorecordmp3 = false;
    pWAVBuffer = NULL;
    pMP3Buffer = NULL;
    dwRead     = 0;
    dwWrite    = 0;
    dwFileSize = 0;
  }

  CMP3Creator::~CMP3Creator(){}

  /*
  bool CMP3Creator::CreateWaveFile(const ATL::CString& filepath){
  WAVEFORMATEX m_WaveFormatEx;
  m_WaveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
  m_WaveFormatEx.nChannels = 2;//should be changed to 2
  m_WaveFormatEx.wBitsPerSample = 16;
  m_WaveFormatEx.cbSize = 0;
  m_WaveFormatEx.nSamplesPerSec = 44000;
  m_WaveFormatEx.nAvgBytesPerSec = m_WaveFormatEx.nSamplesPerSec*(m_WaveFormatEx.wBitsPerSample/8);//=441000*2
  m_WaveFormatEx.nBlockAlign = (m_WaveFormatEx.wBitsPerSample/8)*  m_WaveFormatEx.nChannels;//=2*1

  WRITESOUNDFILE wsf={};
  PWRITESOUNDFILE pwsf = &wsf;
  wcscpy(wsf.lpszFileName,filepath);
  memcpy(&wsf.waveFormatEx,&m_WaveFormatEx,sizeof(m_WaveFormatEx));

  int cbWaveFormatEx = sizeof(WAVEFORMATEX) + pwsf->waveFormatEx.cbSize;
  m_hFile = ::mmioOpen(pwsf->lpszFileName,NULL, MMIO_CREATE|MMIO_WRITE|MMIO_EXCLUSIVE|MMIO_ALLOCBUF);
  if(!m_hFile) return false;

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
  //bLen = 0;
  return true;
  }*/

  void CMP3Creator::WriteDatatoFile(char* p,int len){
    static const int ZOOM=1;/// @todo -make this changable
    if(m_breadytorecordmp3 && p){
      int tmp=p[0],step=len/50;
      for(int i=0;i<len;){
        tmp += (int)p[i];
        i+=step;
      }
      if(tmp==0)
        return;
#if 0
      ::mmioWrite(m_hFile,p,len);
#endif
      pWAVBuffer=(PSHORT)p;
#if 0
      for (int i=0;i<len/2;i++){
        if (pWAVBuffer[i]<=SHRT_MAX/ZOOM && pWAVBuffer[i]>=SHRT_MIN/ZOOM)
          pWAVBuffer[i] = ZOOM*pWAVBuffer[i];
      }
#endif
      dwRead = len/2;
      err = beEncodeChunk(hbeStream, dwRead, pWAVBuffer, pMP3Buffer, &dwWrite);
      int st = (int)fwrite(pMP3Buffer,1,dwWrite,pFileOut);
      if(st != dwWrite){
        return;
      }
#if 0
      dwDone += dwRead*sizeof(SHORT);
#endif
      dwDone_mp3 += dwWrite;
    }
    return;
  }

  /*
  void CMP3Creator::CloseSoundFile(){  
  if(m_hFile){
  ::mmioAscend(m_hFile, &m_MMCKInfoChild, 0);
  ::mmioAscend(m_hFile, &m_MMCKInfoParent, 0);
  ::mmioClose(m_hFile, 0);
  m_hFile = NULL;
  dwDone = 0;
  pWAVBuffer = NULL;
  dwRead     = 0;
  dwWrite    = 0;
  dwFileSize = 0;
  }
  return;
  }
  */

  BOOL CMP3Creator::CreateMP3File(const ATL::CStringA& filepath_mp3){
    std::wstring _6beepath=_6beed_util::Get6BEEPath(0,true);
    _6beepath.append(_T("lame_enc.dll"));
    HINSTANCE hDLL=LoadLibraryW(_6beepath.c_str());
    if(hDLL==NULL){
      MessageBoxA(NULL,"Cannot find lame_enc.dll","ERROR",NULL);
      return FALSE;
    }
    BE_VERSION  Version={};
    BE_CONFIG  beConfig={};
    pFileOut    =NULL;
    Mp3FilePath  =filepath_mp3;
    dwSamples  =0;
    dwMP3Buffer=0;
    hbeStream  =0;
    err        =0;
    pMP3Buffer  =NULL;
    pWAVBuffer  =NULL;

    beInitStream  = (BEINITSTREAM) GetProcAddress(hDLL, TEXT_BEINITSTREAM);
    beEncodeChunk  = (BEENCODECHUNK) GetProcAddress(hDLL, TEXT_BEENCODECHUNK);
    beDeinitStream  = (BEDEINITSTREAM) GetProcAddress(hDLL, TEXT_BEDEINITSTREAM);
    beCloseStream  = (BECLOSESTREAM) GetProcAddress(hDLL, TEXT_BECLOSESTREAM);
    beVersion    = (BEVERSION) GetProcAddress(hDLL, TEXT_BEVERSION);
    beWriteVBRHeader= (BEWRITEVBRHEADER) GetProcAddress(hDLL,TEXT_BEWRITEVBRHEADER);
    beWriteInfoTag  = (BEWRITEINFOTAG) GetProcAddress(hDLL,TEXT_BEWRITEINFOTAG);
    if(!beInitStream || !beEncodeChunk || !beDeinitStream || !beCloseStream || !beVersion || !beWriteVBRHeader){
      ::MessageBox(NULL,_T("Unable to get LAME interfaces"),_T("Error!"),NULL);
      return FALSE;
    }

    beVersion( &Version );
    pFileOut= fopen(Mp3FilePath.GetString(),"wb+");
    if(pFileOut == NULL){
      return FALSE;
    }
    memset(&beConfig,0,sizeof(beConfig));
    beConfig.dwConfig = BE_CONFIG_LAME;

    beConfig.format.LHV1.dwStructVersion  = 1;
    beConfig.format.LHV1.dwStructSize    = sizeof(beConfig);    
    beConfig.format.LHV1.dwSampleRate    = 44100;        // INPUT FREQUENCY
    beConfig.format.LHV1.dwReSampleRate    = 0;          // DON"T RESAMPLE
    beConfig.format.LHV1.nMode        = BE_MP3_MODE_STEREO;//BE_MP3_MODE_STEREO,BE_MP3_MODE_MONO
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

  beConfig.format.LHV1.bCopyright = TRUE;


    err = beInitStream(&beConfig, &dwSamples, &dwMP3Buffer, &hbeStream);
    if(err != BE_ERR_SUCCESSFUL){
      return FALSE;
    }
    pMP3Buffer = new BYTE[dwMP3Buffer];
    dwRead=0;
    dwWrite=0;
    dwDone=0;
    dwFileSize=0;
    m_breadytorecordmp3 = true;
    return TRUE;
  }

  void CMP3Creator::CloseMP3File(){
    err = beDeinitStream(hbeStream, pMP3Buffer, &dwWrite);
    if(err != BE_ERR_SUCCESSFUL){
      beCloseStream(hbeStream);
      return;
    }
    if(dwWrite){
      if(fwrite(pMP3Buffer,1,dwWrite,pFileOut) != dwWrite){
        return;
      }
    }
    beCloseStream(hbeStream);
    delete [] pMP3Buffer;
    fclose(pFileOut);
    if(beWriteInfoTag){
      beWriteInfoTag(hbeStream,Mp3FilePath.GetString());
    }else{
      beWriteVBRHeader(Mp3FilePath.GetString());
    }
    pMP3Buffer = NULL;
    dwRead     = 0;
    dwWrite    = 0;
    dwFileSize = 0;
    m_breadytorecordmp3 = false;

    if (dwDone_mp3>0){
      USES_CONVERSION;
      if (_6beed_share::u_getieversion()==6){
        if(MessageBoxA(NULL,"Would you like to upload this web audio to your UltraIE space?","Web Audio Upload:",MB_YESNO|MB_ICONQUESTION)==IDYES){
          std::wstring t = _6bees_str::wstringmaker(CP_ACP) << Mp3FilePath.GetString();
          _6beed_util::InvokeUploader(t.c_str(),L"",L"",L"");
        }else{
          DeleteFileA(Mp3FilePath);
        }
      }else{
        std::wstring t = _6bees_str::wstringmaker(CP_ACP) << Mp3FilePath.GetString();
        _6beed_util::InvokeUploader(t.c_str(),L"",L"",L"");
      }
    }else{
      DeleteFileA(Mp3FilePath);
    }
    dwDone_mp3 = 0;
    return;
  }

};

