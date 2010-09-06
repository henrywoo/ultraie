#ifndef __PLUGINS_WEBAUDIOREC_MP3CREATOR__
#define __PLUGINS_WEBAUDIOREC_MP3CREATOR__

#include <windows.h>
#include <stdio.h>
#include <BladeMP3EncDLL.h>
#include <atlstr.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

namespace nsplugin{

  typedef struct writesoundfile_tag {
    TCHAR lpszFileName[MAX_PATH];
    WAVEFORMATEX waveFormatEx;
    TCHAR buffer[100];
  } WRITESOUNDFILE, *PWRITESOUNDFILE;


  class CMP3Creator{
    FILE*      pFileOut;
    DWORD    dwSamples;
    DWORD    dwMP3Buffer;
    HBE_STREAM    hbeStream;
    BE_ERR    err;
    PBYTE      pMP3Buffer;

    DWORD dwRead;
    DWORD dwWrite;
    DWORD dwFileSize;

  public:
    CMP3Creator();
    ~CMP3Creator();

    //bool CreateWaveFile(const ATL::CString&);
    void WriteDatatoFile(char* p,int len);
    //void CloseSoundFile();

    BOOL CreateMP3File(const ATL::CStringA&);
    void CloseMP3File();

    //HMMIO m_hFile;
    //MMCKINFO m_MMCKInfoData;
    //MMCKINFO m_MMCKInfoParent;
    //MMCKINFO m_MMCKInfoChild;

    PSHORT pWAVBuffer;
    DWORD dwDone;//size for wav file
    DWORD dwDone_mp3;//size for mp3 file

    ATL::CStringA  Mp3FilePath;
    bool m_breadytorecordmp3;
  };

};


#endif