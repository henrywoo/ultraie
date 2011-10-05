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

#pragma once
///////////////////////////////////////////////////////////////////////////////
#include <tlhelp32.h>
#include <tchar.h>

#define chINRANGE(low, Num, High) (((low) <= (Num)) && ((Num) <= (High)))

///////////////////////////////////////////////////////////////////////////////


class CToolhelp {
private:
  HANDLE m_hSnapshot;

public:
  CToolhelp(DWORD dwFlags = 0, DWORD dwProcessID = 0);
  ~CToolhelp();

  BOOL CreateSnapshot(DWORD dwFlags, DWORD dwProcessID = 0);

  BOOL ProcessFirst(PPROCESSENTRY32 ppe) const;
  BOOL ProcessNext(PPROCESSENTRY32 ppe) const;
  BOOL ProcessFind(DWORD dwProcessId, PPROCESSENTRY32 ppe) const;

  BOOL ModuleFirst(PMODULEENTRY32 pme) const;
  BOOL ModuleNext(PMODULEENTRY32 pme) const;
  BOOL ModuleFind(PVOID pvBaseAddr, PMODULEENTRY32 pme) const;
  BOOL ModuleFind(PTSTR pszModName, PMODULEENTRY32 pme) const;

  BOOL ThreadFirst(PTHREADENTRY32 pte) const;
  BOOL ThreadNext(PTHREADENTRY32 pte) const;

  BOOL HeapListFirst(PHEAPLIST32 phl) const;
  BOOL HeapListNext(PHEAPLIST32 phl) const;
  int  HowManyHeaps() const;

  // Note: The heap block functions do not reference a snapshot and
  // just walk the process's heap from the beginning each time. Infinite 
  // loops can occur if the target process changes its heap while the
  // functions below are enumerating the blocks in the heap.
  BOOL HeapFirst(PHEAPENTRY32 phe, DWORD dwProcessID, UINT_PTR dwHeapID) const;
  BOOL HeapNext(PHEAPENTRY32 phe) const;
  int  HowManyBlocksInHeap(DWORD dwProcessID, DWORD dwHeapId) const;
  BOOL IsAHeap(HANDLE hProcess, PVOID pvBlock, PDWORD pdwFlags) const;
};


///////////////////////////////////////////////////////////////////////////////


inline CToolhelp::CToolhelp(DWORD dwFlags, DWORD dwProcessID) {

  m_hSnapshot = INVALID_HANDLE_VALUE;
  CreateSnapshot(dwFlags, dwProcessID);
}


///////////////////////////////////////////////////////////////////////////////


inline CToolhelp::~CToolhelp() {

  if (m_hSnapshot != INVALID_HANDLE_VALUE)
    CloseHandle(m_hSnapshot);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CToolhelp::CreateSnapshot(DWORD dwFlags, DWORD dwProcessID) {

  if (m_hSnapshot != INVALID_HANDLE_VALUE)
    CloseHandle(m_hSnapshot);

  if (dwFlags == 0) {
    m_hSnapshot = INVALID_HANDLE_VALUE;
  } else {
    m_hSnapshot = CreateToolhelp32Snapshot(dwFlags, dwProcessID);
  }
  return(m_hSnapshot != INVALID_HANDLE_VALUE);
}


///////////////////////////////////////////////////////////////////////////////

inline BOOL CToolhelp::ProcessFirst(PPROCESSENTRY32 ppe) const {

  BOOL fOk = Process32First(m_hSnapshot, ppe);
  if (fOk && (ppe->th32ProcessID == 0))
    fOk = ProcessNext(ppe); // Remove the "[System Process]" (PID = 0)
  return(fOk);
}


inline BOOL CToolhelp::ProcessNext(PPROCESSENTRY32 ppe) const {

  BOOL fOk = Process32Next(m_hSnapshot, ppe);
  if (fOk && (ppe->th32ProcessID == 0))
    fOk = ProcessNext(ppe); // Remove the "[System Process]" (PID = 0)
  return(fOk);
}


inline BOOL CToolhelp::ProcessFind(DWORD dwProcessId, PPROCESSENTRY32 ppe) 
const {

  BOOL fFound = FALSE;
  for (BOOL fOk = ProcessFirst(ppe); fOk; fOk = ProcessNext(ppe)) {
    fFound = (ppe->th32ProcessID == dwProcessId);
    if (fFound) break;
  }
  return(fFound);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CToolhelp::ModuleFirst(PMODULEENTRY32 pme) const {

  return(Module32First(m_hSnapshot, pme));
}


inline BOOL CToolhelp::ModuleNext(PMODULEENTRY32 pme) const {

  return(Module32Next(m_hSnapshot, pme));
}

inline BOOL CToolhelp::ModuleFind(PVOID pvBaseAddr, PMODULEENTRY32 pme) const {

  BOOL fFound = FALSE;
  for (BOOL fOk = ModuleFirst(pme); fOk; fOk = ModuleNext(pme)) {
    fFound = (pme->modBaseAddr == pvBaseAddr);
    if (fFound) break;
  }
  return(fFound);
}

inline BOOL CToolhelp::ModuleFind(PTSTR pszModName, PMODULEENTRY32 pme) const {
  BOOL fFound = FALSE;
  for (BOOL fOk = ModuleFirst(pme); fOk; fOk = ModuleNext(pme)) {
    fFound = (lstrcmpi(pme->szModule,  pszModName) == 0) || 
      (lstrcmpi(pme->szExePath, pszModName) == 0);
    if (fFound) break;
  }
  return(fFound);
}


///////////////////////////////////////////////////////////////////////////////
inline BOOL CToolhelp::ThreadFirst(PTHREADENTRY32 pte) const {
  return(Thread32First(m_hSnapshot, pte));
}


inline BOOL CToolhelp::ThreadNext(PTHREADENTRY32 pte) const {
  return(Thread32Next(m_hSnapshot, pte));
}


///////////////////////////////////////////////////////////////////////////////


inline int CToolhelp::HowManyHeaps() const {
  int nHowManyHeaps = 0;
  HEAPLIST32 hl = { sizeof(hl) };
  for (BOOL fOk = HeapListFirst(&hl); fOk; fOk = HeapListNext(&hl))
    nHowManyHeaps++;
  return(nHowManyHeaps);
}


inline int CToolhelp::HowManyBlocksInHeap(DWORD dwProcessID, DWORD dwHeapID) const {
  int nHowManyBlocksInHeap = 0;
  HEAPENTRY32 he = { sizeof(he) };
  BOOL fOk = HeapFirst(&he, dwProcessID, dwHeapID);
  for (; fOk; fOk = HeapNext(&he))
    nHowManyBlocksInHeap++;
  return(nHowManyBlocksInHeap);
}


inline BOOL CToolhelp::HeapListFirst(PHEAPLIST32 phl) const {
  return(Heap32ListFirst(m_hSnapshot, phl));
}


inline BOOL CToolhelp::HeapListNext(PHEAPLIST32 phl) const {
  return(Heap32ListNext(m_hSnapshot, phl));
}


inline BOOL CToolhelp::HeapFirst(PHEAPENTRY32 phe, DWORD dwProcessID, UINT_PTR dwHeapID) const {
  return(Heap32First(phe, dwProcessID, dwHeapID));
}


inline BOOL CToolhelp::HeapNext(PHEAPENTRY32 phe) const {
  return(Heap32Next(phe));
}


inline BOOL CToolhelp::IsAHeap(HANDLE hProcess, PVOID pvBlock, PDWORD pdwFlags) const {
  HEAPLIST32 hl = { sizeof(hl) };
  for (BOOL fOkHL = HeapListFirst(&hl); fOkHL; fOkHL = HeapListNext(&hl)) {
    HEAPENTRY32 he = { sizeof(he) };
    BOOL fOkHE = HeapFirst(&he, hl.th32ProcessID, hl.th32HeapID);
    for (; fOkHE; fOkHE = HeapNext(&he)) {
      MEMORY_BASIC_INFORMATION mbi;
      VirtualQueryEx(hProcess, (PVOID) he.dwAddress, &mbi, sizeof(mbi));
      if (chINRANGE(mbi.AllocationBase, pvBlock, (PBYTE) mbi.AllocationBase + mbi.RegionSize)) {
        *pdwFlags = hl.dwFlags;
        return(TRUE);
      }
    }
  }
  return(FALSE);
}


//////////////////////////////// End of File //////////////////////////////////
