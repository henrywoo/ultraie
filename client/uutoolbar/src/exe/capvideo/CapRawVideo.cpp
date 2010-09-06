#include "stdafx.h"
#include "CapVideo.h"
#include "CapRawVideo.h"
#include "6Beeffmpeg.h"
#include "../../dll/6beecommond/6beecommond.h"
#include "../../lib/6beebase/6bees_util.h"
#include <process.h>
#include <6bees_const.h>

#include <atlapp.h>
#include <atlgdi.h>//WTL::CDC

#pragma comment(lib,"6beecommond.lib")

//////////////////////////////////////////////////////////////////////////
char* getusername_short(){
  static char tmpname[128]={};
  string tmp(_6beed_share::u_getEmail());
  tmp=tmp.substr(0,tmp.find("@"));
  sprintf_s(tmpname,128,"%s_%lu",tmp.c_str(),time(0));
  return tmpname;
}
HWND caphwnd=NULL;
UINT WINAPI GeneratePicFromWindow(LPVOID p){
  HWND* phandleofcapwnd = (HWND*)p;
  WTL::CDC dcScreen;
  dcScreen.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
  Sleep(500);

  RECT caprect={};
  GetWindowRect(*phandleofcapwnd,&caprect);
  int img_width = caprect.right - caprect.left;
  int img_height= caprect.bottom - caprect.top;
  CLSID clsid;
  GetEncoderClsid(_T("image/jpeg"), &clsid);
  ImageRender ir2;
  if(!ir2.CreateImage(img_width,img_height)){return false;}
  HDC renderDC2 = ir2.GetDC();
  BitBlt(renderDC2,0,0,caprect.right-caprect.left,caprect.bottom-caprect.top,dcScreen.m_hDC,caprect.left,caprect.top,SRCCOPY);
  CString AbsPicFilename;
  using _6bees_util::ultrapath;
  const wstring& upath = ultrapath::R().get(ultrapath::picdir);
  if(_6bees_util::MakeSureDir(upath.c_str())){
    USES_CONVERSION;
    AbsPicFilename.Format(_T("%s%s_%lu.jpg"),
      upath.c_str(),
      A2CW(getusername_short()),
      time(0));
    ir2.SaveToFile(AbsPicFilename.GetString(), &clsid);
  }
  return 1;
}
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CCapRawVideo, CWinThread)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCapRawVideo, CWinThread)
  ON_THREAD_MESSAGE(WM_STARTPREVIEW, StartPreview)
  ON_THREAD_MESSAGE(WM_SHUTDOWN,     StopCapApp)
  ON_THREAD_MESSAGE(WM_RECORD2AVI,   Record2avi)
  ON_THREAD_MESSAGE(WM_CAPTUREPIC,   CapturePic)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// for mpg, STREAM_FRAME_RATE cannot be 10. 25 is ok!
CCapRawVideo::CCapRawVideo():
av_extension("3gp"),
filename(getusername_short()),
final_filename(){
  preview_area_width = PREVIEW_WIDTH;
  preview_area_height= PREVIEW_HEIGHT;
  static const int DEVLEN=80;
  wchar_t szDevName[DEVLEN]={};
  wchar_t szDevVer[DEVLEN]={};
  wchar_t capDev[DEVLEN*2]={};

  for (UINT index=0;index<10;index++){
    if(capGetDriverDescription(index,szDevName,DEVLEN,szDevVer,DEVLEN) ){
      wsprintf(capDev,_T("%s-%s"),szDevName,szDevVer);
      capdevlist.push_back(capDev);
    }else{
      if(index==0){
        capdevlist.push_back(_T("NODEVICE"));
        ::MessageBox(NULL,_T("Sorry, there is NO capture device found in your computer!"),_T("ERROR"),NULL);
        //PostQuitMessage(0);
      }
      break;
    }
  }
}

CCapRawVideo::~CCapRawVideo(){}

BOOL CCapRawVideo::InitInstance(){
  return TRUE;
}

int CCapRawVideo::ExitInstance(){
  return CWinThread::ExitInstance();
}

void CCapRawVideo::StartPreview(WPARAM wParam, LPARAM lParam){
  InitializeCriticalSection(&CriticalSection);
  dlg = reinterpret_cast<CStatic *> (wParam);
  Sleep(500);//����Ҫͣһ���ӣ���֪��Ϊʲô...
  ghWndCap = capCreateCaptureWindow(_T("6BEECAPWND"),WS_CHILD|WS_VISIBLE,0,0,preview_area_width,preview_area_height,dlg->GetSafeHwnd(),1);
  if (ghWndCap==NULL){
    ::AfxMessageBox(_T("ERROR: Create capture window failed."));
    return;
  }
  // there maybe exist several cap drivers
  for (int i=0;i<9;++i){
    if (capDriverConnect(ghWndCap,i)){
      break;
    }
  }
  CAPDRIVERCAPS gCapDriverCaps={};
  if (!capDriverGetCaps(ghWndCap,&gCapDriverCaps,sizeof CAPDRIVERCAPS)){
    ::AfxMessageBox(_T("ERROR: Cannot Get capture driver ability."));
    return;
  }
  CAPTUREPARMS CapParms={};
  capCaptureGetSetup(ghWndCap,&CapParms,sizeof (CAPTUREPARMS));
  //CapParms.dwRequestMicroSecPerFrame=33333;//��������
  CapParms.fLimitEnabled = FALSE;//����ʱ������
  CapParms.fCaptureAudio = TRUE;//�Ƿ�׽��Ƶ
  //CapParms.fCaptureAudio = FALSE;
  CapParms.fMCIControl = FALSE;//MCI Device֧��
  //CapParms.fMCIControl = TRUE;
  CapParms.fYield = TRUE;//���ô���,���Ϊfalse,��׽������������
  CapParms.vKeyAbort = VK_ESCAPE;//ֹͣ��׽������
  CapParms.fAbortLeftMouse = FALSE;
  CapParms.fAbortRightMouse = FALSE;
  //CapParms.dwRequestMicroSecPerFrame=(DWORD)(1.0e6/30.0);//66667
  CapParms.dwRequestMicroSecPerFrame=(DWORD)(1.0e6/STREAM_FRAME_RATE);
  //CapParms.wNumVideoRequested = 5;

  capCaptureSetSetup(ghWndCap,&CapParms,sizeof(CAPTUREPARMS));//���ı�Ĳ������ظ�����
  capPreviewScale(ghWndCap, 1);//����Ԥ��ʱ�ı���
  if (!capPreviewRate(ghWndCap,1000/STREAM_FRAME_RATE)){
    ::AfxMessageBox(_T("Get capture driver ability."));
    return;
  }
  if (!capPreview(ghWndCap,TRUE)){
    ::AfxMessageBox(_T("Get capture driver ability."));
    return;
  }
  //capCaptureSequence(ghWndCap);//���������ʼ������Ƶ��

  //��ʽ����Ƶ����
  WAVEFORMATEX m_WaveFormatEx;
  m_WaveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
  m_WaveFormatEx.nChannels  = 1;//���ܸ�
  //m_WaveFormatEx.nChannels  = 2;
  m_WaveFormatEx.wBitsPerSample  = 16;
  m_WaveFormatEx.cbSize = 0;
  m_WaveFormatEx.nSamplesPerSec  = 22050;
  m_WaveFormatEx.nAvgBytesPerSec = m_WaveFormatEx.nSamplesPerSec*(m_WaveFormatEx.wBitsPerSample/8);//=220500*2
  m_WaveFormatEx.nBlockAlign     = (m_WaveFormatEx.wBitsPerSample/8)*  m_WaveFormatEx.nChannels;//=2*1
  capSetAudioFormat(ghWndCap, &m_WaveFormatEx, sizeof(WAVEFORMATEX));
  //�ҵ����г�ϵͳ����װ��������Ƶѹ����ʽ
  /*
  int i;
  int fccType=0;
  HIC hic;
  ICINFO info;
  CString strDriverName;
  AVICOMPRESSOPTIONS aviOpt;
  for(i=0; ICInfo(fccType,i,&info); i++){
  hic=ICOpen(info.fccType,info.fccHandler,ICMODE_QUERY);
  ICGetInfo(hic,&info,sizeof(ICINFO));
  //strDriverName=CString(info.szDescription);
  strDriverName=CString(info.szName);
  AfxMessageBox(strDriverName);
  ICClose(hic);
  }
  */
}


void CCapRawVideo::Record2avi(WPARAM wParam, LPARAM lParam){
  using _6bees_util::ultrapath;
  const wstring& upath = ultrapath::R().get(ultrapath::videodir);
  if(!_6bees_util::MakeSureDir(upath.c_str())){
    return;
  }
  ::_wchdir(upath.c_str());
  //ffmpeg code start.............................but only write av header
  av_register_all();
  final_filename.Format("%s.%s",filename,av_extension);
  fmt = guess_format(NULL,final_filename.GetString(),NULL);//flv,3gp,mov,avi,wmv
  if (!fmt){
    printf("Could not deduce output format from file extension: using MPEG.\n");
    fmt = guess_format("mpeg", NULL, NULL);
  }
  oc = av_alloc_format_context();
  oc->oformat = fmt;
  video_st = add_video_stream(oc,fmt->video_codec);//����video��
  audio_st = add_audio_stream(oc,fmt->audio_codec);//����audio��
  if (av_set_parameters(oc, NULL)<0){
    AfxMessageBox(_T("Invalid output format parameters\n"));
    return;/// @todo 
  }
  dump_format(oc, 0, final_filename.GetString(), 1);
  if(!video_st || !open_video(oc,video_st)){
    return;
  }
  if(audio_st)open_audio(oc,audio_st);
  if(!(fmt->flags&AVFMT_NOFILE)){
    if (url_fopen(&oc->pb,final_filename,URL_WRONLY)<0){
      fprintf(stderr,"Could not open '%s'\n",final_filename);
      return;/// @todo 
    }
  }
  av_write_header(oc);
  //ffmpeg code finish.........................................
  /////////////////////////////////////////////////////////////
  DWORD dwSize = capGetVideoFormatSize(ghWndCap);
  BITMAPINFO bi={};
  capGetVideoFormat(ghWndCap, &bi, dwSize);
  if (bi.bmiHeader.biWidth!=preview_area_width
    ||bi.bmiHeader.biHeight!=preview_area_height
    ||bi.bmiHeader.biCompression!=mmioFOURCC('I','4','2','0'))
  { //ǿ��������Ƶ��ʽ!
    bi.bmiHeader.biWidth  = preview_area_width;
    bi.bmiHeader.biHeight = preview_area_height;
    bi.bmiHeader.biCompression = mmioFOURCC('I','4','2','0');    
    bi.bmiHeader.biBitCount=12;//I420Ӧ����12λ,24λ��RGBͼ��,R,G,B��8λ
    bi.bmiHeader.biSizeImage=bi.bmiHeader.biWidth * bi.bmiHeader.biHeight * 3/2;
    capSetVideoFormat(this->ghWndCap,&bi,dwSize);//������Ƶ��ʽ
  }
  if( (stream = fopen( "cap.log", "w+" )) == NULL )
    AfxMessageBox( _T("The file 'data2' was not opened\n" ));
  if( (stream2 = fopen( "cap2.log", "w+" )) == NULL )
    AfxMessageBox( _T("The file 'data2' was not opened\n" ));  
  myWnd=ghWndCap;
  capSetUserData(myWnd,this);
  pVideo  = new BYTE[PREVIEW_WIDTH*PREVIEW_HEIGHT*3/2];

  if (!capSetCallbackOnVideoStream(myWnd,&VideoCallBack)){
    ::AfxMessageBox(_T("Can not call Call callback function."));
  }
  if (!capSetCallbackOnWaveStream(myWnd,&WaveCallBack)){
    ::AfxMessageBox(_T("Can not call Call callback function."));
  }
  QueryPerformanceFrequency(&litmp);
  dfFreq = (double)litmp.QuadPart;
  ::AfxBeginThread(WriteCBRVedio,NULL,THREAD_PRIORITY_NORMAL,0,0,NULL);  

  //capCaptureSequence(ghWndCap);//This will automatically generate an avi file under c drive.Now we donot want this.
  capCaptureSequenceNoFile(this->ghWndCap);
  return;
}

void CCapRawVideo::StopCapApp(WPARAM wParam, LPARAM lParam){
  gstage=G_PRESTOP;
  ///////////////////////////////////////////////////////////////
  if (stream)  fclose(stream);
  if (stream2) fclose(stream2);
  // ffmpeg stop video and audio ..................

  close_video(oc, video_st);
  close_audio(oc, audio_st);

  av_write_trailer(oc);
  for(unsigned int i = 0; i < oc->nb_streams; i++) {
    av_freep(&oc->streams[i]->codec);
    av_freep(&oc->streams[i]);
  }
  if (!(fmt->flags & AVFMT_NOFILE)) {
    url_fclose(&oc->pb);
  }
  av_free(oc);

  //////////////////////////////////////////////////////////////
  delete [] pVideo;
  if(ghWndCap) capPreview(ghWndCap,FALSE);
  if(ghWndCap) capCaptureAbort(ghWndCap);
  if(ghWndCap) capDriverDisconnect(ghWndCap);
  DeleteCriticalSection(&CriticalSection);
  //////////////////////////////////////////////////////////////

  return;
}

void CCapRawVideo::CapturePic(WPARAM wParam, LPARAM lParam){
  caphwnd = ghWndCap;
  ::_beginthreadex(NULL,0,GeneratePicFromWindow,(LPVOID)&caphwnd,0,NULL);
  //capDlgVideoFormat(this->ghWndCap);
  //capDlgVideoCompression(this->ghWndCap);
  return;
}
wstring CCapRawVideo::getdevinfo(unsigned int i){
  return (i >= capdevlist.size()) ? this->capdevlist[0] : this->capdevlist[i];
}


int CCapRawVideo::getframerate(){
  return framerate;
}

const char* CCapRawVideo::getavfilename(){
  return final_filename.GetString();
}
