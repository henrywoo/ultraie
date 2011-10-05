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

#ifndef __6BEECAPVIDEO_6BEEFFMPEG__
#define __6BEECAPVIDEO_6BEEFFMPEG__
#pragma once

#pragma comment(lib,"avutil-49.lib")
#pragma comment(lib,"avcodec-51.lib")
#pragma comment(lib,"avformat-51.lib")

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>

#include "avcodec.h"
#include "avformat.h"
#include "swscale.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

CRITICAL_SECTION CriticalSection={};
HWND myWnd=NULL;
FILE* pFileOut=NULL;
FILE *stream=NULL;
FILE *stream2=NULL;
BYTE * pVideo=NULL;
char logtime[20]={};
char logtime2[50]={};
int framerate=0;
DWORD dwStart=0;
LONGLONG QPart1=0,QPart2=0;
double dfMinus=.0, dfFreq=.0, dfTim=.0; 
LARGE_INTEGER litmp={};

// For WaveCallBack function
LPSTR audiobuf=NULL;
int sample_size= 576*2*1; //1152*2*1;
BYTE * tmp=NULL;
BYTE * pSound=NULL;
BYTE * totalbuf=NULL;
int totalsize=0;
int offset=0;
const int STREAM_FRAME_RATE = 25;

static int sws_flags = SWS_BICUBIC;

AVFrame *picture=NULL,*tmp_picture=NULL;
uint8_t *video_outbuf=NULL;
int frame_count=0,video_outbuf_size=0;

//audio output...
float t=.0, tincr=.0, tincr2=.0;
int16_t *samples=NULL;
uint8_t *audio_outbuf=NULL;
int audio_outbuf_size=0;
int audio_input_frame_size=0;
//////////////////////////////////////////////

//全局变量
AVOutputFormat *fmt=NULL;
AVFormatContext *oc=NULL;
AVStream *video_st=NULL,*audio_st=NULL;
double audio_pts=.0, video_pts=.0;
///////////////////////////////////////////////////////////////////////////////////////////////////

static AVStream* add_video_stream(AVFormatContext *oc, int codec_id){
  AVStream *st = av_new_stream(oc, 0);
  if (!st) {
    //fprintf(stderr, "Could not alloc stream\n");
    return NULL;
  }
  int bitrate=400000;

  AVCodecContext *c = st->codec;
  c->codec_id = (CodecID)codec_id;
  c->codec_type = CODEC_TYPE_VIDEO;
  c->bit_rate = bitrate;//比特率，决定画面清晰度的
  c->width = PREVIEW_WIDTH;
  c->height = PREVIEW_HEIGHT;
  c->time_base.den = STREAM_FRAME_RATE;//Just for MPG format
  c->time_base.num = 1;
  c->gop_size = 12;
  c->pix_fmt = PIX_FMT_YUV420P;
  if (c->codec_id == CODEC_ID_MPEG2VIDEO) {
    c->max_b_frames = 2;
  }
  if (c->codec_id == CODEC_ID_MPEG1VIDEO){//
    c->mb_decision=2;
  }
  if(!strcmp(oc->oformat->name, "mp4") || !strcmp(oc->oformat->name, "mov") || !strcmp(oc->oformat->name, "3gp")){
    c->flags |= CODEC_FLAG_GLOBAL_HEADER;
  }
  return st;
}

static AVFrame *alloc_picture(int pix_fmt, int width, int height){
  AVFrame *picture=NULL;
  uint8_t *picture_buf=NULL;
  int size=0;

  if ((picture = avcodec_alloc_frame())==NULL)
    return NULL;
  size = avpicture_get_size(pix_fmt, width, height);
  picture_buf = reinterpret_cast <uint8_t*>(av_malloc(size));
  if (!picture_buf) {
    av_free(picture);
    return NULL;
  }
  avpicture_fill((AVPicture *)picture, picture_buf,pix_fmt,width,height);
  return picture;
}
static bool open_video(AVFormatContext *oc, AVStream *st){
  AVCodec *codec;
  AVCodecContext *c;
  c = st->codec;

  codec = avcodec_find_encoder(c->codec_id);
  if (!codec) {
    MessageBoxA(NULL, "codec not found","ERROR",NULL);
    return false;
  }
  if (avcodec_open(c, codec) < 0) {
    MessageBoxA(NULL, "could not open codec","ERROR",NULL);
    return false;
  }
  video_outbuf = NULL;
  if (!(oc->oformat->flags & AVFMT_RAWPICTURE)) {
    video_outbuf_size = 200000;
    video_outbuf = reinterpret_cast <uint8_t*>(av_malloc(video_outbuf_size));
  }
  picture = alloc_picture(c->pix_fmt, c->width, c->height);
  if (!picture) {
    MessageBoxA(NULL, "Could not allocate picture.","ERROR",NULL);
    return false;
  }
  tmp_picture = NULL;
  return true;
}

static void close_video(AVFormatContext *oc, AVStream *st){
  avcodec_close(st->codec);
  av_free(picture->data[0]);
  av_free(picture);
  if (tmp_picture) {
    av_free(tmp_picture->data[0]);
    av_free(tmp_picture);
  }
  av_free(video_outbuf);
}

//将lpData里面的数据放到pict里面
static void fill_yuv_image(AVFrame *pict, int frame_index, int width, int height, unsigned char * inputData){

  int y=0;
  for (;y<height*width;y++){
    pict->data[0][y]=inputData[y];
  }
  for (y=0;y<(height/2)*(width/2);y++){
    pict->data[1][y]=inputData[height*width+y];
    pict->data[2][y]=inputData[height*width*5/4+y];
  }
  pict->linesize[0]=width;
  pict->linesize[1]=width/2;
  pict->linesize[2]=width/2;
}

static void write_video_frame(AVFormatContext *oc, AVStream *st, unsigned char * inputVideoData){
  int out_size=0, ret=0;
  static struct SwsContext *img_convert_ctx;

  AVCodecContext *c = st->codec;
  fill_yuv_image(picture, frame_count, c->width, c->height, inputVideoData);

  if (oc->oformat->flags & AVFMT_RAWPICTURE){
    AVPacket pkt={};
    av_init_packet(&pkt);

    pkt.flags |= PKT_FLAG_KEY;
    pkt.stream_index= st->index;
    pkt.data= (uint8_t *)picture;
    pkt.size= sizeof(AVPicture);
    //写入一帧
    ret = av_write_frame(oc, &pkt);
  } else {
    out_size = avcodec_encode_video(c, video_outbuf, video_outbuf_size, picture);
    if (out_size > 0) {
      AVPacket pkt={};
      av_init_packet(&pkt);
      pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, st->time_base);
      if(c->coded_frame->key_frame)
        pkt.flags |= PKT_FLAG_KEY;
      pkt.stream_index= st->index;
      pkt.data= video_outbuf;
      pkt.size= out_size;
      //写入一帧
      ret = av_write_frame(oc, &pkt);
    } else {
      ret = 0;
    }
  }
  if (ret!=0) {
    //fprintf(stderr, "Error while writing video frame\n");
    return;
  }
  frame_count++;
}

//*********************************** audio functions *********************************************//
static AVStream *add_audio_stream(AVFormatContext *oc, int codec_id){
  AVCodecContext *c;
  AVStream *st;
  st = av_new_stream(oc, 1);
  if (!st) {
    //fprintf(stderr, "Could not alloc stream\n");
    return NULL;
  }
  c = st->codec;
  c->codec_id = CODEC_ID_MP3;//指定为MP3
  c->codec_type = CODEC_TYPE_AUDIO;
  c->bit_rate = 32000;
  c->sample_rate = 22050;
  c->channels = 1;
  c->flags |= CODEC_FLAG_GLOBAL_HEADER;
  return st;
}

void open_audio(AVFormatContext *oc, AVStream *st){
  AVCodecContext* c = st->codec;
  AVCodec* codec = avcodec_find_encoder(c->codec_id);
  if (avcodec_open(c, codec) < 0) {
    //fprintf(stderr, "could not open codec\n");
    return;
  }
  audio_outbuf_size = 10000;
  audio_outbuf = reinterpret_cast <uint8_t*>(av_malloc(audio_outbuf_size));
  audio_input_frame_size = c->frame_size;
  //samples = (int16_t*) (av_malloc(audio_input_frame_size * 2 * c->channels));
}

static void write_audio_frame(AVFormatContext *oc, AVStream *st, BYTE * inputAudioData){
  AVCodecContext *c;
  AVPacket pkt;
  av_init_packet(&pkt);
  c = st->codec;
  samples = (int16_t *)inputAudioData;

  pkt.size= avcodec_encode_audio(c, audio_outbuf, audio_outbuf_size, samples);//编码一帧
  pkt.pts= av_rescale_q(c->coded_frame->pts, c->time_base, st->time_base);
  pkt.flags |= PKT_FLAG_KEY;
  pkt.stream_index= st->index;
  pkt.data= audio_outbuf;
  if (pkt.size!=0){
    av_write_frame(oc, &pkt); //写入一帧
    //av_interleaved_write_frame(oc, &pkt);
  }
}

void close_audio(AVFormatContext *oc, AVStream *st){
  if(st)
    avcodec_close(st->codec);
  //av_free(samples);//System will manage inputAudioData automatically, so there is no need to release again!
  av_free(audio_outbuf);
}


const LONG G_INITIAL=0;
const LONG G_VEREADY=1;
const LONG G_WAREADY=2;
const LONG G_PRESTOP=3;
LONG gstage = G_INITIAL;

UINT WriteCBRVedio(LPVOID pParam){
  if (gstage==G_PRESTOP){
    return 0;
  }
  DWORD astart=GetTickCount();
  DWORD afin=0;
  int cha2=0;
  double ttmp=0.0;
  int recordframecount=0;
  for (;gstage!=3;){
    //=0.05*STREAM_DURATION 要音频 gstage==2说明AV都准备好了      
    if (gstage==2 && (dfTim-ttmp>=1.0/STREAM_FRAME_RATE || fabs(ttmp)<=1e-6)){
      if(recordframecount==0){
        astart=GetTickCount();
        QueryPerformanceCounter(&litmp);
        QPart1 = litmp.QuadPart;// 获得初始值
      }      
      if (audio_st)
        audio_pts = (double)audio_st->pts.val * audio_st->time_base.num / audio_st->time_base.den;
      else
        audio_pts = 0.0;
      if (video_st)
        video_pts = (double)video_st->pts.val * video_st->time_base.num / video_st->time_base.den;
      else
        video_pts = 0.0;      
      if ((video_st && !(video_st && audio_st && audio_pts<video_pts))){
        if (1){
          afin=GetTickCount();
          cha2=afin-astart;
          astart=afin;
          if (cha2>5){
            sprintf_s(logtime2,50,"%d-%f-%d\n",cha2,dfTim,recordframecount);
            fwrite(logtime2,1,50,stream2);
            ttmp=dfTim;
            EnterCriticalSection(&CriticalSection);
            write_video_frame(oc, video_st, pVideo);
            LeaveCriticalSection(&CriticalSection);
            recordframecount++; //recordframecount是已经录制的帧数
          }
        }
      }
    }
    //////////////////////////////////////////////////////////////////////////////
    if (recordframecount>0){
      ::QueryPerformanceCounter(&litmp);
      QPart2 = litmp.QuadPart;
      dfMinus = (double)(QPart2-QPart1);
      dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
    }
    //////////////////////////////////////////////////////////////////////////////
  }
  return 0;
}

//将lpVHdr->lpData中的视频数据编码并写入flv文件
LRESULT  CALLBACK VideoCallBack(HWND hWnd,LPVIDEOHDR lpVHdr){
  if (lpVHdr->dwFlags & VHDR_DONE){//0x00000001
    if(gstage==G_PRESTOP)return 1;
    EnterCriticalSection(&CriticalSection);
    memcpy(pVideo,lpVHdr->lpData,lpVHdr->dwBufferLength/* =PREVIEW_WIDTH*PREVIEW_HEIGHT*3/2 */);//?
    LeaveCriticalSection(&CriticalSection);
    if (gstage!=2) gstage=1;
    if (gstage==1){
      dwStart = GetTickCount();
    }else{
      DWORD now=GetTickCount();
      int cha=now-dwStart;
      framerate=(cha==0)?0:1000/cha;
      sprintf_s(logtime,20,"%d-%d\n",framerate,lpVHdr->dwBufferLength);
      dwStart=now;
      fwrite(logtime,1,10,stream);
    }          
  }
  return 1;
}


LRESULT  CALLBACK WaveCallBack(HWND hWnd,LPWAVEHDR lpWaveHdr){
  if(gstage==G_PRESTOP) return 1;
  if(gstage==1)
    gstage=2;
  //ffmpeg audio compression start ..................  
  if (audio_st)
    audio_pts = (double)audio_st->pts.val * audio_st->time_base.num / audio_st->time_base.den;
  else
    audio_pts = 0.0;
  if (video_st)
    video_pts = (double)video_st->pts.val * video_st->time_base.num / video_st->time_base.den;
  else
    video_pts = 0.0;
  if (!video_st || (video_st && audio_st && audio_pts<video_pts)){//将lpWAVdr->lpData中的音频数据编码并写入flv文件
    int v=0;
    pSound   = new BYTE[sample_size];
    totalbuf = new BYTE[lpWaveHdr->dwBufferLength];
    totalsize= lpWaveHdr->dwBufferLength;
    memcpy(totalbuf,lpWaveHdr->lpData,lpWaveHdr->dwBufferLength);
    if (totalsize >= sample_size){
      for (;totalsize>=sample_size;v++){
        memcpy(pSound,totalbuf,sample_size);
        totalbuf = totalbuf + sample_size;
        totalsize= totalsize- sample_size;

        EnterCriticalSection(&CriticalSection);
        write_audio_frame(oc, audio_st, pSound);
        LeaveCriticalSection(&CriticalSection);

        memset(pSound,0,sample_size);
      }
      offset=sample_size-totalsize;
      if (offset>0){
        memcpy(pSound,totalbuf,totalsize);
      }
    }else{
      if (offset>0){
        if (offset+totalsize>sample_size){
          tmp=pSound+offset;
          memcpy(tmp,totalbuf,sample_size-offset);

          EnterCriticalSection(&CriticalSection);
          write_audio_frame(oc, audio_st, pSound);
          LeaveCriticalSection(&CriticalSection);

          offset=totalsize-(sample_size-offset);
          totalbuf += (sample_size-offset);
          memcpy(pSound,totalbuf,offset);
        }else{
          offset += totalsize;
          tmp=pSound+offset;
          memcpy(tmp,totalbuf,sample_size-offset);
        }
      }
    }
    totalbuf -= sample_size*v;
    delete [] pSound;
    delete [] totalbuf;
  }
  //ffmpeg audio compression finish ..................
  return 1;
}



#endif // _DEBUG

