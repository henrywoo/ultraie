#ifndef _CNWSPIDLMGR_H_
#define _CNWSPIDLMGR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef	enum tagITEM_TYPE
{
  NWS_FOLDER=0x00000001,
  NWS_FILE=0x00000002,
}ITEM_TYPE;

typedef struct tagPIDLDATA
{
  ITEM_TYPE type;
  TCHAR    szName[1];
}PIDLDATA, FAR *LPPIDLDATA;

class CNWSPidlMgr{
public:
  LPITEMIDLIST Create(ITEM_TYPE iItemType,LPTSTR szName);
  void Delete(LPITEMIDLIST pidl);
  LPITEMIDLIST GetNextItem(LPCITEMIDLIST pidl);
  LPITEMIDLIST GetLastItem(LPCITEMIDLIST pidl);
  UINT GetByteSize(LPCITEMIDLIST pidl);
  bool IsSingle(LPCITEMIDLIST pidl);
  LPITEMIDLIST Concatenate(LPCITEMIDLIST, LPCITEMIDLIST);
  LPITEMIDLIST Copy(LPCITEMIDLIST pidlSrc);

  // Retrieve encrypted file name (without the path)
  // The pidl MUST remain valid until the caller has finished with the returned string.
  HRESULT GetName(LPCITEMIDLIST,LPTSTR);

  // Retrieve the item type (see above)
  ITEM_TYPE GetItemType(LPCITEMIDLIST pidl);

  HRESULT GetFullName(LPCITEMIDLIST pidl,LPTSTR szFullName,DWORD *pdwLen); 
  BOOL  HasSubFolder(LPCITEMIDLIST pidl); 
  HRESULT GetItemAttributes(LPCITEMIDLIST pidl,USHORT iAttrNum,LPTSTR pszAttrOut);

private:
  LPPIDLDATA GetDataPointer(LPCITEMIDLIST);
};

#endif//_CNWSPIDLMGR_H_