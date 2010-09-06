#ifndef __6BEEAUDIORECORDER_WRITESOUNDFILE__
#define __6BEEAUDIORECORDER_WRITESOUNDFILE__

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <BladeMP3EncDLL.h>

typedef struct writesoundfile_tag {
  TCHAR lpszFileName[MAX_PATH];
  WAVEFORMATEX waveFormatEx;
  TCHAR buffer[100];
} WRITESOUNDFILE, *PWRITESOUNDFILE;

class CMP3Creator
{
public:
  CMP3Creator(void);
  ~CMP3Creator(void);
  
  void CreateWaveFile(PWRITESOUNDFILE pwsf);
  BOOL CreateMP3File(PWRITESOUNDFILE pwsf);

  void WriteToSoundFile(WAVEHDR* pWriteHdr);
  void OnEndThread(WPARAM wParam, LPARAM lParam);

  void CloseSoundFile();
  void CloseMP3File();

  HMMIO m_hFile;
  MMCKINFO m_MMCKInfoData;
  MMCKINFO m_MMCKInfoParent;
  MMCKINFO m_MMCKInfoChild;
private:
  FILE*    pFileOut;
  char    *strFileOut;
  DWORD    dwSamples;
  DWORD    dwMP3Buffer;
  HBE_STREAM  hbeStream;
  BE_ERR    err;
  PBYTE    pMP3Buffer;
  DWORD dwRead;
  DWORD dwWrite;
  DWORD dwDone;
  DWORD dwFileSize;
public:
  PSHORT    pWAVBuffer;
  int       bLen;
};

#endif