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

#include "stdafx.h"
#include "6bees_httpclient.h"
#include <string>
#include "6bees_socket.h"

using namespace _6bees_socket;

httpclient::httpclient():
port_(-1),
responseheadersize_(-1),
httpconnected_(false),
httpresponsed_(false),
sockcon_(CreateNewBlockSocket())
{
  memset(requestheader_,0,MAXHEADERSIZE);
  memset(responseheader_,0,MAXHEADERSIZE);
}

bool httpclient::ConnectHttpServer(const char* hostname,int port){
  try{
    sockcon_->Create(SOCK_STREAM);
    CSockAddr adr = sockcon_->GetHostByName(hostname,port);
    sockcon_->Connect(adr);
    httpconnected_ = true;
    port_ = port;
  }catch(sckexception&){
    return false;
  }
  return true;
}

#define sprintf_s sprintf
#define strcat_s strcat
#define _ltoa_s _ltoa

const char* httpclient::FormatRequestHeader(const char *pServer,
  const char *pObject,
  long &Length,
  const char *pCookie,
  const char *pReferer,
  long nFrom,
  long nTo)
{
  char szPort[10];
  char szTemp[20];
  sprintf_s(szPort,"%d",port_);
  memset(requestheader_,'\0',MAXHEADERSIZE);
  ///1. Request method,request url path, version
  strcat_s(requestheader_,"GET ");
  if(pObject==NULL){
    pObject = "/";
  }
  strcat_s(requestheader_,pObject);
  strcat_s(requestheader_," HTTP/1.1");
  strcat_s(requestheader_,"\r\n");
  ///2. Host
  strcat_s(requestheader_,"Host:");
  strcat_s(requestheader_,pServer);
  strcat_s(requestheader_,"\r\n");
  ///3. Referer
  if(pReferer != NULL){
    strcat_s(requestheader_,"Referer:");
    strcat_s(requestheader_,pReferer);
    strcat_s(requestheader_,"\r\n");
  }
  ///4. Accept data type
  strcat_s(requestheader_,"Accept:*/*");
  strcat_s(requestheader_,"\r\n");
  ///5. user agent
  strcat_s(requestheader_,"User-Agent:Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.0.7) Gecko/2009021910 YFF3 Firefox/3.0.7");
  strcat_s(requestheader_,"\r\n");
  ///6. connection setting and keep alive
  strcat_s(requestheader_,"Connection:Keep-Alive");
  strcat_s(requestheader_,"\r\n");
  ///7. Cookie.
  if(pCookie != NULL){
    strcat_s(requestheader_,"Set Cookie:0");
    strcat_s(requestheader_,pCookie);
    strcat_s(requestheader_,"\r\n");
  }
  ///8. request data starting from
  if(nFrom>0||nTo>0){
    strcat_s(requestheader_,"Range: bytes=");
    _ltoa_s(nFrom,szTemp,10);
    strcat_s(requestheader_,szTemp);
    strcat_s(requestheader_,"-");
    if(nTo > nFrom){
      _ltoa_s(nTo,szTemp,10);
      strcat_s(requestheader_,szTemp);
    }
    strcat_s(requestheader_,"\r\n");
  }
  ///9. others
  strcat_s(requestheader_,
    "Cache-Control: no-cache\r\n"
    "TE: trailers, deflate;q=0.5\r\n"
  );
  ///10. empty line
  strcat_s(requestheader_,"\r\n");

  Length=(long)strlen(requestheader_);
  return requestheader_;
}

bool httpclient::SendReqAndGetRespHeader(){
  try{
    if (requestheader_){
      static int timeout = 3;
      if(sockcon_->Send(requestheader_,(int)strlen(requestheader_),timeout)>0){
        int nLength=0;
        if(GetResponseHeader(nLength)){
          return nLength>0;
        }
      }
    }
  }catch(sckexception&){
    return false;
  }
  return false;
}

char* httpclient::GetResponseHeader(int &nLength){
  try{
    if(!httpresponsed_){
      char c = 0;
      int nIndex = 0;
      bool bEndResponse = false;
      while(!bEndResponse && nIndex < MAXHEADERSIZE){
        sockcon_->Receive(&c,1,3);
        responseheader_[nIndex++] = c;
        if(nIndex >= 4){
          if(responseheader_[nIndex - 4] == '\r' &&
            responseheader_[nIndex - 3] == '\n' &&
            responseheader_[nIndex - 2] == '\r' &&
            responseheader_[nIndex - 1] == '\n')
          {
            bEndResponse = true;
          }
        }
      }
      responseheadersize_ = nIndex;
      httpresponsed_ = true;
    }
    nLength = responseheadersize_;
    return responseheader_;
  }catch(sckexception&){
    return NULL;
  }
}

/// Note: caller is in charge of releasing the memory
char* httpclient::GetHTTPData(int& clen){
  static const int BLOCKSIZE = 1024;
  static const int kMAXTIMEOUT = 10;//10 seconds time out
  if(!httpconnected_)
    return NULL;

  clen=0;//file length
  char tmp[BLOCKSIZE];
  char contentsize[100];
  int result=GetField("Content-Length",contentsize,100);
  if (result == -1){
    if (GetServerState()==200){
      //chunked http response, no Content-Length, then just receive until it finish or fail
      vector<char> data;
      bool firstthunk=true;
      for (long nLength=1;nLength>0;){
        nLength = sockcon_->Receive(tmp,BLOCKSIZE,kMAXTIMEOUT);
        if(nLength<0){
          clen = (int)data.size();
          break;
          //return data.empty() ? NULL : &data[0];
        }else if(nLength==0){
          sockcon_->Close();
          clen = (int)data.size();
          break;
          //return data.empty() ? NULL : &data[0];
        }else{
          if(firstthunk){
            char *p = &tmp[0];
            for(int j=0;j<nLength;++j){
              if (tmp[j]=='\r' && tmp[j+1]=='\n'){//python >>> ord('\n') == 10 >>> ord('\r') ==13
                p += j+2;
                break;
              }
            }
            std::copy(p,tmp+nLength,back_inserter(data));
            firstthunk = false;
          }else{
            std::copy(tmp,tmp+nLength,back_inserter(data));
          }
        }
      }
      char * pdata = new char[clen+1];
      //memcpy_s(pdata,clen,&data[0],clen);
      memcpy(pdata,&data[0],clen);
      pdata[clen]='\0';
      return pdata;
    }else{
      return NULL;
    }
  }else{
    clen = atoi(contentsize);
  }
  char* httpcontent = new char[clen+1];
  memset(httpcontent,'\0',clen+1);

  
  long nLength=1;
  long p=0;
  for (;nLength>0;){
    nLength = sockcon_->Receive(tmp,BLOCKSIZE,kMAXTIMEOUT);
    if(nLength<0){
      delete [] httpcontent;
      return NULL;
    }else if(nLength==0){
      sockcon_->Close();
      break;
    }else if(nLength+p>clen){
      delete [] httpcontent;
      return NULL;
    }else{
      memcpy(httpcontent+p,tmp,nLength);
      p += nLength;
      if (p==clen){
        break;
      }
    }
  }
  return httpcontent;
}

int httpclient::GetField(const char *name, char *szValue, int nMaxLength){
  if(!httpresponsed_)
    return -1;

  std::string strRespons(responseheader_);
  int nPos =  (int)strRespons.find(name);
  if(nPos != std::string::npos){
    nPos += (int)strlen(name);
    nPos += 2;
    int nCr = (int)strRespons.find("\r\n",nPos);
    std::string strValue(strRespons.substr(nPos,nCr - nPos));
    strcpy(szValue,strValue.c_str());
    return (nCr - nPos);
  }else{
    return -1;
  }
}

int httpclient::GetServerState(){
  if(!httpresponsed_)
    return -1;
  char szState[3];
  szState[0] = responseheader_[9];
  szState[1] = responseheader_[10];
  szState[2] = responseheader_[11];
  return atoi(szState);
}
