#ifndef __6BEEUPLOADER_UTIL_H__
#define __6BEEUPLOADER_UTIL_H__

#include <comdef.h>
#include "FileProfile.h"
#include "6bees_timer.h"

////////////////////////////////////////////////////////////////////
typedef CListArray<CFileProfile> FList;//File List


/// @brief packet info transfered to a new thread
struct tpacket{
  CListWindowFileData* t_pWndFD;
  CFileProfile*  t_pfp;
  FList*    t_up_flist;
  _6bees_timer::CTimer*  t_timer;
  _6bees_socket::ftpclient* t_ftpclient;
  LPVOID   t_pmaindlg;
  int  t_ID;
} threadpacket[ATSMAXCOUNT];


struct ppacket{
  CListWindowFileData* cfd;
  CString  fpath;
  _6bees_socket::ftpclient* _6bees_socket;
  __int64* psbytes;
  int status;
};

_6bees_timer::CTimer m_Timer[ATSMAXCOUNT];

///////////////////////////////////////////////////////////////////////////////////
list<CFileProfile*> sfp_waiting_list;//can we use queue/deque

set<CString>    sfp_active;// abs path set

CFileProfile WaitingFileArray500[500];//maximum 500 waiting status files

CFileProfile active_fp[ATSMAXCOUNT];

CRITICAL_SECTION csection;

#endif