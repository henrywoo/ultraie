#ifndef _MY_IMAGELIST_H
#define _MY_IMAGELIST_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CShellImageLists
{
public:

   HIMAGELIST m_hImageListSmall;
   HIMAGELIST m_hImageListLarge;

   CShellImageLists() : m_hImageListSmall(NULL), m_hImageListLarge(NULL)
   {
   }
   virtual ~CShellImageLists()
   {
      if( m_hImageListSmall ) 
		  ImageList_Destroy(m_hImageListSmall);
      if( m_hImageListLarge ) 
		  ImageList_Destroy(m_hImageListLarge);
   }
};

#endif //_MY_IMAGELIST_H