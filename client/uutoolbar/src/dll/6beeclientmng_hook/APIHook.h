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

class CAPIHook {
public:
  // Hook a function in all modules
  CAPIHook(PSTR pszCalleeModName, PSTR pszFuncName, PROC pfnHook);
  // Unhook a function from all modules
  ~CAPIHook();
  // Returns the original address of the hooked function
  operator PROC() { return(m_pfnOrig); }
  // Hook module w/CAPIHook implementation?
  // I have to make it static because I need to use it 
  // in ReplaceIATEntryInAllMods
  static BOOL ExcludeAPIHookMod; 
  // Calls the real GetProcAddress 
  static FARPROC WINAPI GetProcAddressRaw(HMODULE hmod, PCSTR pszProcName);
private:
  static PVOID sm_pvMaxAppAddr; // Maximum private memory address
  static CAPIHook* sm_pHead;    // Address of first object
  CAPIHook* m_pNext;            // Address of next  object
  PCSTR m_pszCalleeModName;     // Module containing the function (ANSI)
  PCSTR m_pszFuncName;          // Function name in callee (ANSI)
  PROC  m_pfnOrig;              // Original function address in callee
  PROC  m_pfnHook;              // Hook function address
private:
  // Replaces a symbol's address in a module's import section
  static void WINAPI ReplaceIATEntryInAllMods(PCSTR pszCalleeModName, PROC pfnOrig, PROC pfnHook);
  // Replaces a symbol's address in all modules' import sections
  static void WINAPI ReplaceIATEntryInOneMod(PCSTR pszCalleeModName, PROC pfnOrig, PROC pfnHook, HMODULE hmodCaller);
  // Replaces a symbol's address in a module's export sections
  static void ReplaceEATEntryInOneMod(HMODULE hmod, PCSTR pszFunctionName, PROC pfnNew);
private:
  // Used when a DLL is newly loaded after hooking a function
  static void    WINAPI FixupNewlyLoadedModule(HMODULE hmod, DWORD dwFlags);
  // Used to trap when DLLs are newly loaded
  static HMODULE WINAPI MyLoadLibraryA(PCSTR pszModulePath);
  static HMODULE WINAPI MyLoadLibraryW(PCWSTR pszModulePath);
  static HMODULE WINAPI MyLoadLibraryExA(PCSTR pszModulePath, HANDLE hFile, DWORD dwFlags);
  static HMODULE WINAPI MyLoadLibraryExW(PCWSTR pszModulePath, HANDLE hFile, DWORD dwFlags);
  // Returns address of replacement function if hooked function is requested
  static FARPROC WINAPI MyGetProcAddress(HMODULE hmod, PCSTR pszProcName);
private:
  // Instantiates hooks on these functions
  static CAPIHook sm_LoadLibraryA;
  static CAPIHook sm_LoadLibraryW;
  static CAPIHook sm_LoadLibraryExA;
  static CAPIHook sm_LoadLibraryExW;
  static CAPIHook sm_GetProcAddress;
};
//////////////////////////////// End of File //////////////////////////////////