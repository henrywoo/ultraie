// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `uuice.ice'

#include <uuice.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/ScopedArray.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 303
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 0
#       error Ice patch level mismatch!
#   endif
#endif

static const ::std::string __UUTOOLBAR__User__Login_name = "Login";

static const ::std::string __UUTOOLBAR__User__LoginUUFile_name = "LoginUUFile";

static const ::std::string __UUTOOLBAR__User__Logout_name = "Logout";

static const ::std::string __UUTOOLBAR__User__CreateFolder_name = "CreateFolder";

static const ::std::string __UUTOOLBAR__User__DeleteFileFolder_name = "DeleteFileFolder";

static const ::std::string __UUTOOLBAR__User__RenameFileFolder_name = "RenameFileFolder";

static const ::std::string __UUTOOLBAR__User__PostUploadFile_name = "PostUploadFile";

static const ::std::string __UUTOOLBAR__User__GetUserInfo_name = "GetUserInfo";

static const ::std::string __UUTOOLBAR__User__UploadImgSeq_name = "UploadImgSeq";

static const ::std::string __UUTOOLBAR__User__SaveOnlineBookmark_name = "SaveOnlineBookmark";

static const ::std::string __UUTOOLBAR__User__GetOLBMURLs_name = "GetOLBMURLs";

static const ::std::string __UUTOOLBAR__User__UploadOLBM_name = "UploadOLBM";

static const ::std::string __UUTOOLBAR__User__DownloadOLBM_name = "DownloadOLBM";

static const ::std::string __UUTOOLBAR__User__PostUploadThumb_name = "PostUploadThumb";

static const ::std::string __UUTOOLBAR__User__PostUploadIcon_name = "PostUploadIcon";

static const ::std::string __UUTOOLBAR__User__GetFTPInfoForUpload_name = "GetFTPInfoForUpload";

static const ::std::string __UUTOOLBAR__User__IsLatestVersion_name = "IsLatestVersion";

static const ::std::string __UUTOOLBAR__User__IsFileUploaded_name = "IsFileUploaded";

::Ice::Object* IceInternal::upCast(::UUTOOLBAR::User* p) { return p; }
::IceProxy::Ice::Object* IceInternal::upCast(::IceProxy::UUTOOLBAR::User* p) { return p; }

void
UUTOOLBAR::__read(::IceInternal::BasicStream* __is, ::UUTOOLBAR::UserPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::UUTOOLBAR::User;
        v->__copyFrom(proxy);
    }
}

bool
UUTOOLBAR::Image::operator==(const Image& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(name != __rhs.name)
    {
        return false;
    }
    if(data != __rhs.data)
    {
        return false;
    }
    return true;
}

bool
UUTOOLBAR::Image::operator<(const Image& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(name < __rhs.name)
    {
        return true;
    }
    else if(__rhs.name < name)
    {
        return false;
    }
    if(data < __rhs.data)
    {
        return true;
    }
    else if(__rhs.data < data)
    {
        return false;
    }
    return false;
}

void
UUTOOLBAR::Image::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(name);
    if(data.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&data[0], &data[0] + data.size());
    }
}

void
UUTOOLBAR::Image::__read(::IceInternal::BasicStream* __is)
{
    __is->read(name);
    ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> ___data;
    __is->read(___data);
    ::std::vector< ::Ice::Byte>(___data.first, ___data.second).swap(data);
}

void
UUTOOLBAR::__writeImgSeq(::IceInternal::BasicStream* __os, const ::UUTOOLBAR::Image* begin, const ::UUTOOLBAR::Image* end)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        begin[i].__write(__os);
    }
}

void
UUTOOLBAR::__readImgSeq(::IceInternal::BasicStream* __is, ::UUTOOLBAR::ImgSeq& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 2);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        v[i].__read(__is);
        __is->checkSeq();
        __is->endElement();
    }
    __is->endSeq(sz);
}

bool
UUTOOLBAR::OLBMValue::operator==(const OLBMValue& __rhs) const
{
    if(this == &__rhs)
    {
        return true;
    }
    if(title != __rhs.title)
    {
        return false;
    }
    if(description != __rhs.description)
    {
        return false;
    }
    if(tag != __rhs.tag)
    {
        return false;
    }
    if(folder != __rhs.folder)
    {
        return false;
    }
    if(iconURL != __rhs.iconURL)
    {
        return false;
    }
    return true;
}

bool
UUTOOLBAR::OLBMValue::operator<(const OLBMValue& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(title < __rhs.title)
    {
        return true;
    }
    else if(__rhs.title < title)
    {
        return false;
    }
    if(description < __rhs.description)
    {
        return true;
    }
    else if(__rhs.description < description)
    {
        return false;
    }
    if(tag < __rhs.tag)
    {
        return true;
    }
    else if(__rhs.tag < tag)
    {
        return false;
    }
    if(folder < __rhs.folder)
    {
        return true;
    }
    else if(__rhs.folder < folder)
    {
        return false;
    }
    if(iconURL < __rhs.iconURL)
    {
        return true;
    }
    else if(__rhs.iconURL < iconURL)
    {
        return false;
    }
    return false;
}

void
UUTOOLBAR::OLBMValue::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(title);
    __os->write(description);
    __os->write(tag);
    __os->write(folder);
    __os->write(iconURL);
}

void
UUTOOLBAR::OLBMValue::__read(::IceInternal::BasicStream* __is)
{
    __is->read(title);
    __is->read(description);
    __is->read(tag);
    __is->read(folder);
    __is->read(iconURL);
}

void
UUTOOLBAR::__writeOLBMItems(::IceInternal::BasicStream* __os, const ::UUTOOLBAR::OLBMItems& v)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::UUTOOLBAR::OLBMItems::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
        __os->write(p->first);
        p->second.__write(__os);
    }
}

void
UUTOOLBAR::__readOLBMItems(::IceInternal::BasicStream* __is, ::UUTOOLBAR::OLBMItems& v)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
        ::std::pair<const  ::std::string, ::UUTOOLBAR::OLBMValue> pair;
        __is->read(const_cast< ::std::string&>(pair.first));
        ::UUTOOLBAR::OLBMItems::iterator __i = v.insert(v.end(), pair);
        __i->second.__read(__is);
    }
}

void
UUTOOLBAR::__write(::IceInternal::BasicStream* __os, ::UUTOOLBAR::FOLDERPRIV v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 4);
}

void
UUTOOLBAR::__read(::IceInternal::BasicStream* __is, ::UUTOOLBAR::FOLDERPRIV& v)
{
    ::Ice::Byte val;
    __is->read(val, 4);
    v = static_cast< ::UUTOOLBAR::FOLDERPRIV>(val);
}

void
UUTOOLBAR::__write(::IceInternal::BasicStream* __os, ::UUTOOLBAR::LOGINRESPONSE v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 4);
}

void
UUTOOLBAR::__read(::IceInternal::BasicStream* __is, ::UUTOOLBAR::LOGINRESPONSE& v)
{
    ::Ice::Byte val;
    __is->read(val, 4);
    v = static_cast< ::UUTOOLBAR::LOGINRESPONSE>(val);
}

void
UUTOOLBAR::__write(::IceInternal::BasicStream* __os, ::UUTOOLBAR::UPLOADFILETYPE v)
{
    __os->write(static_cast< ::Ice::Byte>(v), 2);
}

void
UUTOOLBAR::__read(::IceInternal::BasicStream* __is, ::UUTOOLBAR::UPLOADFILETYPE& v)
{
    ::Ice::Byte val;
    __is->read(val, 2);
    v = static_cast< ::UUTOOLBAR::UPLOADFILETYPE>(val);
}

::UUTOOLBAR::LOGINRESPONSE
IceProxy::UUTOOLBAR::User::Login(const ::std::string& email, const ::std::string& password, ::std::string& sid, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__Login_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->Login(email, password, sid, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::UUTOOLBAR::LOGINRESPONSE
IceProxy::UUTOOLBAR::User::LoginUUFile(const ::std::string& email, const ::std::string& password, ::std::string& sid, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__LoginUUFile_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->LoginUUFile(email, password, sid, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::UUTOOLBAR::User::Logout(const ::std::string& sid, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__Logout_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->Logout(sid, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::UUTOOLBAR::User::CreateFolder(const ::std::string& foldername, ::UUTOOLBAR::FOLDERPRIV fp, const ::std::string& sid, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__CreateFolder_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->CreateFolder(foldername, fp, sid, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::UUTOOLBAR::User::DeleteFileFolder(const ::std::string& foldername, const ::std::string& sid, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__DeleteFileFolder_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->DeleteFileFolder(foldername, sid, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::UUTOOLBAR::User::RenameFileFolder(const ::std::string& oldname, const ::std::string& newname, const ::std::string& sid, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__RenameFileFolder_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->RenameFileFolder(oldname, newname, sid, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::Long
IceProxy::UUTOOLBAR::User::PostUploadFile(const ::std::string& filename, ::Ice::Long filesize, const ::std::string& crc32, const ::std::string& desc, const ::std::string& tag, const ::std::string& email, const ::std::string& sid, ::std::string& fetchurl, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__PostUploadFile_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->PostUploadFile(filename, filesize, crc32, desc, tag, email, sid, fetchurl, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::UUTOOLBAR::User::GetUserInfo(const ::std::string& email, ::Ice::Long& spaceused, ::Ice::Long& ULnum, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__GetUserInfo_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->GetUserInfo(email, spaceused, ULnum, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::UUTOOLBAR::User::UploadImgSeq(const ::UUTOOLBAR::ImgSeq& imgs, const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::UPLOADFILETYPE type, ::Ice::Long id, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__UploadImgSeq_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->UploadImgSeq(imgs, email, sid, type, id, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::Ice::Long
IceProxy::UUTOOLBAR::User::SaveOnlineBookmark(const ::std::string& url, const ::std::string& title, const ::std::string& desc, const ::std::string& tag, const ::std::string& email, const ::std::string& sid, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__SaveOnlineBookmark_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->SaveOnlineBookmark(url, title, desc, tag, email, sid, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::UUTOOLBAR::User::GetOLBMURLs(const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::IEFavURLs& existingOLBMURLs, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__GetOLBMURLs_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->GetOLBMURLs(email, sid, existingOLBMURLs, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::UUTOOLBAR::User::UploadOLBM(const ::UUTOOLBAR::OLBMItems& items, const ::std::string& email, const ::std::string& sid, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__UploadOLBM_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->UploadOLBM(items, email, sid, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::UUTOOLBAR::User::DownloadOLBM(const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::OLBMItems& items, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__DownloadOLBM_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->DownloadOLBM(email, sid, items, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::UUTOOLBAR::User::PostUploadThumb(const ::std::string& thumbname, ::Ice::Long id, const ::std::string& sid, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__PostUploadThumb_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->PostUploadThumb(thumbname, id, sid, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::UUTOOLBAR::User::PostUploadIcon(const ::std::string& iconname, ::Ice::Long id, const ::std::string& sid, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__PostUploadIcon_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->PostUploadIcon(iconname, id, sid, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

bool
IceProxy::UUTOOLBAR::User::GetFTPInfoForUpload(const ::std::string& email, ::std::string& ip, ::Ice::Short& port, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__GetFTPInfoForUpload_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->GetFTPInfoForUpload(email, ip, port, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::std::string
IceProxy::UUTOOLBAR::User::IsLatestVersion(const ::std::string& version, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__IsLatestVersion_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->IsLatestVersion(version, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

::std::string
IceProxy::UUTOOLBAR::User::IsFileUploaded(const ::std::string& crc32, ::Ice::Long filesize, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__IsFileUploaded_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->IsFileUploaded(crc32, filesize, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex, 0);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, 0, __cnt);
        }
    }
}

const ::std::string&
IceProxy::UUTOOLBAR::User::ice_staticId()
{
    return ::UUTOOLBAR::User::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::UUTOOLBAR::User::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::UUTOOLBAR::User);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::UUTOOLBAR::User::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::UUTOOLBAR::User);
}

::IceProxy::Ice::Object*
IceProxy::UUTOOLBAR::User::__newInstance() const
{
    return new User;
}

::UUTOOLBAR::LOGINRESPONSE
IceDelegateM::UUTOOLBAR::User::Login(const ::std::string& email, const ::std::string& password, ::std::string& sid, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__Login_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(email);
        __os->write(password);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::UUTOOLBAR::LOGINRESPONSE __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(sid);
        ::UUTOOLBAR::__read(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::UUTOOLBAR::LOGINRESPONSE
IceDelegateM::UUTOOLBAR::User::LoginUUFile(const ::std::string& email, const ::std::string& password, ::std::string& sid, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__LoginUUFile_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(email);
        __os->write(password);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::UUTOOLBAR::LOGINRESPONSE __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(sid);
        ::UUTOOLBAR::__read(__is, __ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::UUTOOLBAR::User::Logout(const ::std::string& sid, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__Logout_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(sid);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::UUTOOLBAR::User::CreateFolder(const ::std::string& foldername, ::UUTOOLBAR::FOLDERPRIV fp, const ::std::string& sid, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__CreateFolder_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(foldername);
        ::UUTOOLBAR::__write(__os, fp);
        __os->write(sid);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::UUTOOLBAR::User::DeleteFileFolder(const ::std::string& foldername, const ::std::string& sid, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__DeleteFileFolder_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(foldername);
        __os->write(sid);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::UUTOOLBAR::User::RenameFileFolder(const ::std::string& oldname, const ::std::string& newname, const ::std::string& sid, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__RenameFileFolder_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(oldname);
        __os->write(newname);
        __os->write(sid);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Long
IceDelegateM::UUTOOLBAR::User::PostUploadFile(const ::std::string& filename, ::Ice::Long filesize, const ::std::string& crc32, const ::std::string& desc, const ::std::string& tag, const ::std::string& email, const ::std::string& sid, ::std::string& fetchurl, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__PostUploadFile_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(filename);
        __os->write(filesize);
        __os->write(crc32);
        __os->write(desc);
        __os->write(tag);
        __os->write(email);
        __os->write(sid);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::Ice::Long __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(fetchurl);
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::UUTOOLBAR::User::GetUserInfo(const ::std::string& email, ::Ice::Long& spaceused, ::Ice::Long& ULnum, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__GetUserInfo_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(email);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(spaceused);
        __is->read(ULnum);
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::UUTOOLBAR::User::UploadImgSeq(const ::UUTOOLBAR::ImgSeq& imgs, const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::UPLOADFILETYPE type, ::Ice::Long id, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__UploadImgSeq_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        if(imgs.size() == 0)
        {
            __os->writeSize(0);
        }
        else
        {
            ::UUTOOLBAR::__writeImgSeq(__os, &imgs[0], &imgs[0] + imgs.size());
        }
        __os->write(email);
        __os->write(sid);
        ::UUTOOLBAR::__write(__os, type);
        __os->write(id);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::Ice::Long
IceDelegateM::UUTOOLBAR::User::SaveOnlineBookmark(const ::std::string& url, const ::std::string& title, const ::std::string& desc, const ::std::string& tag, const ::std::string& email, const ::std::string& sid, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__SaveOnlineBookmark_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(url);
        __os->write(title);
        __os->write(desc);
        __os->write(tag);
        __os->write(email);
        __os->write(sid);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::Ice::Long __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::UUTOOLBAR::User::GetOLBMURLs(const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::IEFavURLs& existingOLBMURLs, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__GetOLBMURLs_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(email);
        __os->write(sid);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(existingOLBMURLs);
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::UUTOOLBAR::User::UploadOLBM(const ::UUTOOLBAR::OLBMItems& items, const ::std::string& email, const ::std::string& sid, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__UploadOLBM_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        ::UUTOOLBAR::__writeOLBMItems(__os, items);
        __os->write(email);
        __os->write(sid);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::UUTOOLBAR::User::DownloadOLBM(const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::OLBMItems& items, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__DownloadOLBM_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(email);
        __os->write(sid);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        ::UUTOOLBAR::__readOLBMItems(__is, items);
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::UUTOOLBAR::User::PostUploadThumb(const ::std::string& thumbname, ::Ice::Long id, const ::std::string& sid, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__PostUploadThumb_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(thumbname);
        __os->write(id);
        __os->write(sid);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::UUTOOLBAR::User::PostUploadIcon(const ::std::string& iconname, ::Ice::Long id, const ::std::string& sid, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__PostUploadIcon_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(iconname);
        __os->write(id);
        __os->write(sid);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

bool
IceDelegateM::UUTOOLBAR::User::GetFTPInfoForUpload(const ::std::string& email, ::std::string& ip, ::Ice::Short& port, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__GetFTPInfoForUpload_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(email);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        bool __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(ip);
        __is->read(port);
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::std::string
IceDelegateM::UUTOOLBAR::User::IsLatestVersion(const ::std::string& version, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__IsLatestVersion_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(version);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::std::string __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::std::string
IceDelegateM::UUTOOLBAR::User::IsFileUploaded(const ::std::string& crc32, ::Ice::Long filesize, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__IsFileUploaded_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(crc32);
        __os->write(filesize);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        if(!__ok)
        {
            try
            {
                __og.throwUserException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                ::Ice::UnknownUserException __uue(__FILE__, __LINE__, __ex.ice_name());
                throw __uue;
            }
        }
        ::std::string __ret;
        ::IceInternal::BasicStream* __is = __og.is();
        __is->startReadEncaps();
        __is->read(__ret);
        __is->endReadEncaps();
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::UUTOOLBAR::LOGINRESPONSE
IceDelegateD::UUTOOLBAR::User::Login(const ::std::string& email, const ::std::string& password, ::std::string& sid, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::UUTOOLBAR::LOGINRESPONSE& __result, const ::std::string& email, const ::std::string& password, ::std::string& sid, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_email(email),
            _m_password(password),
            _m_sid(sid)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->Login(_m_email, _m_password, _m_sid, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::UUTOOLBAR::LOGINRESPONSE& _result;
        const ::std::string& _m_email;
        const ::std::string& _m_password;
        ::std::string& _m_sid;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__Login_name, ::Ice::Normal, __context);
    ::UUTOOLBAR::LOGINRESPONSE __result;
    try
    {
        _DirectI __direct(__result, email, password, sid, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::UUTOOLBAR::LOGINRESPONSE
IceDelegateD::UUTOOLBAR::User::LoginUUFile(const ::std::string& email, const ::std::string& password, ::std::string& sid, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::UUTOOLBAR::LOGINRESPONSE& __result, const ::std::string& email, const ::std::string& password, ::std::string& sid, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_email(email),
            _m_password(password),
            _m_sid(sid)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->LoginUUFile(_m_email, _m_password, _m_sid, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::UUTOOLBAR::LOGINRESPONSE& _result;
        const ::std::string& _m_email;
        const ::std::string& _m_password;
        ::std::string& _m_sid;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__LoginUUFile_name, ::Ice::Normal, __context);
    ::UUTOOLBAR::LOGINRESPONSE __result;
    try
    {
        _DirectI __direct(__result, email, password, sid, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::UUTOOLBAR::User::Logout(const ::std::string& sid, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& sid, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_sid(sid)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->Logout(_m_sid, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_sid;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__Logout_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, sid, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::UUTOOLBAR::User::CreateFolder(const ::std::string& foldername, ::UUTOOLBAR::FOLDERPRIV fp, const ::std::string& sid, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& foldername, ::UUTOOLBAR::FOLDERPRIV fp, const ::std::string& sid, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_foldername(foldername),
            _m_fp(fp),
            _m_sid(sid)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->CreateFolder(_m_foldername, _m_fp, _m_sid, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_foldername;
        ::UUTOOLBAR::FOLDERPRIV _m_fp;
        const ::std::string& _m_sid;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__CreateFolder_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, foldername, fp, sid, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::UUTOOLBAR::User::DeleteFileFolder(const ::std::string& foldername, const ::std::string& sid, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& foldername, const ::std::string& sid, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_foldername(foldername),
            _m_sid(sid)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->DeleteFileFolder(_m_foldername, _m_sid, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_foldername;
        const ::std::string& _m_sid;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__DeleteFileFolder_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, foldername, sid, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::UUTOOLBAR::User::RenameFileFolder(const ::std::string& oldname, const ::std::string& newname, const ::std::string& sid, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& oldname, const ::std::string& newname, const ::std::string& sid, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_oldname(oldname),
            _m_newname(newname),
            _m_sid(sid)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->RenameFileFolder(_m_oldname, _m_newname, _m_sid, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_oldname;
        const ::std::string& _m_newname;
        const ::std::string& _m_sid;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__RenameFileFolder_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, oldname, newname, sid, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::Long
IceDelegateD::UUTOOLBAR::User::PostUploadFile(const ::std::string& filename, ::Ice::Long filesize, const ::std::string& crc32, const ::std::string& desc, const ::std::string& tag, const ::std::string& email, const ::std::string& sid, ::std::string& fetchurl, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Long& __result, const ::std::string& filename, ::Ice::Long filesize, const ::std::string& crc32, const ::std::string& desc, const ::std::string& tag, const ::std::string& email, const ::std::string& sid, ::std::string& fetchurl, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_filename(filename),
            _m_filesize(filesize),
            _m_crc32(crc32),
            _m_desc(desc),
            _m_tag(tag),
            _m_email(email),
            _m_sid(sid),
            _m_fetchurl(fetchurl)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->PostUploadFile(_m_filename, _m_filesize, _m_crc32, _m_desc, _m_tag, _m_email, _m_sid, _m_fetchurl, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Long& _result;
        const ::std::string& _m_filename;
        ::Ice::Long _m_filesize;
        const ::std::string& _m_crc32;
        const ::std::string& _m_desc;
        const ::std::string& _m_tag;
        const ::std::string& _m_email;
        const ::std::string& _m_sid;
        ::std::string& _m_fetchurl;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__PostUploadFile_name, ::Ice::Normal, __context);
    ::Ice::Long __result;
    try
    {
        _DirectI __direct(__result, filename, filesize, crc32, desc, tag, email, sid, fetchurl, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::UUTOOLBAR::User::GetUserInfo(const ::std::string& email, ::Ice::Long& spaceused, ::Ice::Long& ULnum, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& email, ::Ice::Long& spaceused, ::Ice::Long& ULnum, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_email(email),
            _m_spaceused(spaceused),
            _m_ULnum(ULnum)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->GetUserInfo(_m_email, _m_spaceused, _m_ULnum, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_email;
        ::Ice::Long& _m_spaceused;
        ::Ice::Long& _m_ULnum;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__GetUserInfo_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, email, spaceused, ULnum, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::UUTOOLBAR::User::UploadImgSeq(const ::UUTOOLBAR::ImgSeq& imgs, const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::UPLOADFILETYPE type, ::Ice::Long id, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::UUTOOLBAR::ImgSeq& imgs, const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::UPLOADFILETYPE type, ::Ice::Long id, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_imgs(imgs),
            _m_email(email),
            _m_sid(sid),
            _m_type(type),
            _m_id(id)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->UploadImgSeq(_m_imgs, _m_email, _m_sid, _m_type, _m_id, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::UUTOOLBAR::ImgSeq& _m_imgs;
        const ::std::string& _m_email;
        const ::std::string& _m_sid;
        ::UUTOOLBAR::UPLOADFILETYPE _m_type;
        ::Ice::Long _m_id;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__UploadImgSeq_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, imgs, email, sid, type, id, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::Long
IceDelegateD::UUTOOLBAR::User::SaveOnlineBookmark(const ::std::string& url, const ::std::string& title, const ::std::string& desc, const ::std::string& tag, const ::std::string& email, const ::std::string& sid, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Long& __result, const ::std::string& url, const ::std::string& title, const ::std::string& desc, const ::std::string& tag, const ::std::string& email, const ::std::string& sid, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_url(url),
            _m_title(title),
            _m_desc(desc),
            _m_tag(tag),
            _m_email(email),
            _m_sid(sid)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->SaveOnlineBookmark(_m_url, _m_title, _m_desc, _m_tag, _m_email, _m_sid, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Long& _result;
        const ::std::string& _m_url;
        const ::std::string& _m_title;
        const ::std::string& _m_desc;
        const ::std::string& _m_tag;
        const ::std::string& _m_email;
        const ::std::string& _m_sid;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__SaveOnlineBookmark_name, ::Ice::Normal, __context);
    ::Ice::Long __result;
    try
    {
        _DirectI __direct(__result, url, title, desc, tag, email, sid, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::UUTOOLBAR::User::GetOLBMURLs(const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::IEFavURLs& existingOLBMURLs, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::IEFavURLs& existingOLBMURLs, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_email(email),
            _m_sid(sid),
            _m_existingOLBMURLs(existingOLBMURLs)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->GetOLBMURLs(_m_email, _m_sid, _m_existingOLBMURLs, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_email;
        const ::std::string& _m_sid;
        ::UUTOOLBAR::IEFavURLs& _m_existingOLBMURLs;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__GetOLBMURLs_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, email, sid, existingOLBMURLs, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::UUTOOLBAR::User::UploadOLBM(const ::UUTOOLBAR::OLBMItems& items, const ::std::string& email, const ::std::string& sid, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::UUTOOLBAR::OLBMItems& items, const ::std::string& email, const ::std::string& sid, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_items(items),
            _m_email(email),
            _m_sid(sid)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->UploadOLBM(_m_items, _m_email, _m_sid, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::UUTOOLBAR::OLBMItems& _m_items;
        const ::std::string& _m_email;
        const ::std::string& _m_sid;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__UploadOLBM_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, items, email, sid, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::UUTOOLBAR::User::DownloadOLBM(const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::OLBMItems& items, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::OLBMItems& items, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_email(email),
            _m_sid(sid),
            _m_items(items)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->DownloadOLBM(_m_email, _m_sid, _m_items, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_email;
        const ::std::string& _m_sid;
        ::UUTOOLBAR::OLBMItems& _m_items;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__DownloadOLBM_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, email, sid, items, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::UUTOOLBAR::User::PostUploadThumb(const ::std::string& thumbname, ::Ice::Long id, const ::std::string& sid, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& thumbname, ::Ice::Long id, const ::std::string& sid, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_thumbname(thumbname),
            _m_id(id),
            _m_sid(sid)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->PostUploadThumb(_m_thumbname, _m_id, _m_sid, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_thumbname;
        ::Ice::Long _m_id;
        const ::std::string& _m_sid;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__PostUploadThumb_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, thumbname, id, sid, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::UUTOOLBAR::User::PostUploadIcon(const ::std::string& iconname, ::Ice::Long id, const ::std::string& sid, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& iconname, ::Ice::Long id, const ::std::string& sid, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_iconname(iconname),
            _m_id(id),
            _m_sid(sid)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->PostUploadIcon(_m_iconname, _m_id, _m_sid, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_iconname;
        ::Ice::Long _m_id;
        const ::std::string& _m_sid;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__PostUploadIcon_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, iconname, id, sid, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

bool
IceDelegateD::UUTOOLBAR::User::GetFTPInfoForUpload(const ::std::string& email, ::std::string& ip, ::Ice::Short& port, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& email, ::std::string& ip, ::Ice::Short& port, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_email(email),
            _m_ip(ip),
            _m_port(port)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->GetFTPInfoForUpload(_m_email, _m_ip, _m_port, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_email;
        ::std::string& _m_ip;
        ::Ice::Short& _m_port;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__GetFTPInfoForUpload_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, email, ip, port, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::std::string
IceDelegateD::UUTOOLBAR::User::IsLatestVersion(const ::std::string& version, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::std::string& __result, const ::std::string& version, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_version(version)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->IsLatestVersion(_m_version, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::std::string& _result;
        const ::std::string& _m_version;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__IsLatestVersion_name, ::Ice::Normal, __context);
    ::std::string __result;
    try
    {
        _DirectI __direct(__result, version, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::std::string
IceDelegateD::UUTOOLBAR::User::IsFileUploaded(const ::std::string& crc32, ::Ice::Long filesize, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::std::string& __result, const ::std::string& crc32, ::Ice::Long filesize, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_crc32(crc32),
            _m_filesize(filesize)
        {
        }
        
        virtual ::Ice::DispatchStatus
        run(::Ice::Object* object)
        {
            ::UUTOOLBAR::User* servant = dynamic_cast< ::UUTOOLBAR::User*>(object);
            if(!servant)
            {
                throw ::Ice::OperationNotExistException(__FILE__, __LINE__, _current.id, _current.facet, _current.operation);
            }
            _result = servant->IsFileUploaded(_m_crc32, _m_filesize, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::std::string& _result;
        const ::std::string& _m_crc32;
        ::Ice::Long _m_filesize;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__IsFileUploaded_name, ::Ice::Normal, __context);
    ::std::string __result;
    try
    {
        _DirectI __direct(__result, crc32, filesize, __current);
        try
        {
            __direct.servant()->__collocDispatch(__direct);
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
    }
    catch(const ::Ice::SystemException&)
    {
        throw;
    }
    catch(const ::IceInternal::LocalExceptionWrapper&)
    {
        throw;
    }
    catch(const ::std::exception& __ex)
    {
        ::IceInternal::LocalExceptionWrapper::throwWrapper(__ex);
    }
    catch(...)
    {
        throw ::IceInternal::LocalExceptionWrapper(::Ice::UnknownException(__FILE__, __LINE__, "unknown c++ exception"), false);
    }
    return __result;
}

::Ice::ObjectPtr
UUTOOLBAR::User::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __UUTOOLBAR__User_ids[2] =
{
    "::Ice::Object",
    "::UUTOOLBAR::User"
};

bool
UUTOOLBAR::User::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__UUTOOLBAR__User_ids, __UUTOOLBAR__User_ids + 2, _s);
}

::std::vector< ::std::string>
UUTOOLBAR::User::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__UUTOOLBAR__User_ids[0], &__UUTOOLBAR__User_ids[2]);
}

const ::std::string&
UUTOOLBAR::User::ice_id(const ::Ice::Current&) const
{
    return __UUTOOLBAR__User_ids[1];
}

const ::std::string&
UUTOOLBAR::User::ice_staticId()
{
    return __UUTOOLBAR__User_ids[1];
}

::Ice::DispatchStatus
UUTOOLBAR::User::___Login(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string email;
    ::std::string password;
    __is->read(email);
    __is->read(password);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string sid;
    ::UUTOOLBAR::LOGINRESPONSE __ret = Login(email, password, sid, __current);
    __os->write(sid);
    ::UUTOOLBAR::__write(__os, __ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___LoginUUFile(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string email;
    ::std::string password;
    __is->read(email);
    __is->read(password);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string sid;
    ::UUTOOLBAR::LOGINRESPONSE __ret = LoginUUFile(email, password, sid, __current);
    __os->write(sid);
    ::UUTOOLBAR::__write(__os, __ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___Logout(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string sid;
    __is->read(sid);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = Logout(sid, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___CreateFolder(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string foldername;
    ::UUTOOLBAR::FOLDERPRIV fp;
    ::std::string sid;
    __is->read(foldername);
    ::UUTOOLBAR::__read(__is, fp);
    __is->read(sid);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = CreateFolder(foldername, fp, sid, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___DeleteFileFolder(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string foldername;
    ::std::string sid;
    __is->read(foldername);
    __is->read(sid);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = DeleteFileFolder(foldername, sid, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___RenameFileFolder(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string oldname;
    ::std::string newname;
    ::std::string sid;
    __is->read(oldname);
    __is->read(newname);
    __is->read(sid);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = RenameFileFolder(oldname, newname, sid, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___PostUploadFile(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string filename;
    ::Ice::Long filesize;
    ::std::string crc32;
    ::std::string desc;
    ::std::string tag;
    ::std::string email;
    ::std::string sid;
    __is->read(filename);
    __is->read(filesize);
    __is->read(crc32);
    __is->read(desc);
    __is->read(tag);
    __is->read(email);
    __is->read(sid);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string fetchurl;
    ::Ice::Long __ret = PostUploadFile(filename, filesize, crc32, desc, tag, email, sid, fetchurl, __current);
    __os->write(fetchurl);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___GetUserInfo(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string email;
    __is->read(email);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Long spaceused;
    ::Ice::Long ULnum;
    bool __ret = GetUserInfo(email, spaceused, ULnum, __current);
    __os->write(spaceused);
    __os->write(ULnum);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___UploadImgSeq(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::UUTOOLBAR::ImgSeq imgs;
    ::std::string email;
    ::std::string sid;
    ::UUTOOLBAR::UPLOADFILETYPE type;
    ::Ice::Long id;
    ::UUTOOLBAR::__readImgSeq(__is, imgs);
    __is->read(email);
    __is->read(sid);
    ::UUTOOLBAR::__read(__is, type);
    __is->read(id);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = UploadImgSeq(imgs, email, sid, type, id, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___SaveOnlineBookmark(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string url;
    ::std::string title;
    ::std::string desc;
    ::std::string tag;
    ::std::string email;
    ::std::string sid;
    __is->read(url);
    __is->read(title);
    __is->read(desc);
    __is->read(tag);
    __is->read(email);
    __is->read(sid);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Long __ret = SaveOnlineBookmark(url, title, desc, tag, email, sid, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___GetOLBMURLs(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string email;
    ::std::string sid;
    __is->read(email);
    __is->read(sid);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::UUTOOLBAR::IEFavURLs existingOLBMURLs;
    bool __ret = GetOLBMURLs(email, sid, existingOLBMURLs, __current);
    if(existingOLBMURLs.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&existingOLBMURLs[0], &existingOLBMURLs[0] + existingOLBMURLs.size());
    }
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___UploadOLBM(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::UUTOOLBAR::OLBMItems items;
    ::std::string email;
    ::std::string sid;
    ::UUTOOLBAR::__readOLBMItems(__is, items);
    __is->read(email);
    __is->read(sid);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = UploadOLBM(items, email, sid, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___DownloadOLBM(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string email;
    ::std::string sid;
    __is->read(email);
    __is->read(sid);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::UUTOOLBAR::OLBMItems items;
    bool __ret = DownloadOLBM(email, sid, items, __current);
    ::UUTOOLBAR::__writeOLBMItems(__os, items);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___PostUploadThumb(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string thumbname;
    ::Ice::Long id;
    ::std::string sid;
    __is->read(thumbname);
    __is->read(id);
    __is->read(sid);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = PostUploadThumb(thumbname, id, sid, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___PostUploadIcon(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string iconname;
    ::Ice::Long id;
    ::std::string sid;
    __is->read(iconname);
    __is->read(id);
    __is->read(sid);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = PostUploadIcon(iconname, id, sid, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___GetFTPInfoForUpload(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string email;
    __is->read(email);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string ip;
    ::Ice::Short port;
    bool __ret = GetFTPInfoForUpload(email, ip, port, __current);
    __os->write(ip);
    __os->write(port);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___IsLatestVersion(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string version;
    __is->read(version);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = IsLatestVersion(version, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___IsFileUploaded(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string crc32;
    ::Ice::Long filesize;
    __is->read(crc32);
    __is->read(filesize);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::std::string __ret = IsFileUploaded(crc32, filesize, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

static ::std::string __UUTOOLBAR__User_all[] =
{
    "CreateFolder",
    "DeleteFileFolder",
    "DownloadOLBM",
    "GetFTPInfoForUpload",
    "GetOLBMURLs",
    "GetUserInfo",
    "IsFileUploaded",
    "IsLatestVersion",
    "Login",
    "LoginUUFile",
    "Logout",
    "PostUploadFile",
    "PostUploadIcon",
    "PostUploadThumb",
    "RenameFileFolder",
    "SaveOnlineBookmark",
    "UploadImgSeq",
    "UploadOLBM",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
UUTOOLBAR::User::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__UUTOOLBAR__User_all, __UUTOOLBAR__User_all + 22, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - __UUTOOLBAR__User_all)
    {
        case 0:
        {
            return ___CreateFolder(in, current);
        }
        case 1:
        {
            return ___DeleteFileFolder(in, current);
        }
        case 2:
        {
            return ___DownloadOLBM(in, current);
        }
        case 3:
        {
            return ___GetFTPInfoForUpload(in, current);
        }
        case 4:
        {
            return ___GetOLBMURLs(in, current);
        }
        case 5:
        {
            return ___GetUserInfo(in, current);
        }
        case 6:
        {
            return ___IsFileUploaded(in, current);
        }
        case 7:
        {
            return ___IsLatestVersion(in, current);
        }
        case 8:
        {
            return ___Login(in, current);
        }
        case 9:
        {
            return ___LoginUUFile(in, current);
        }
        case 10:
        {
            return ___Logout(in, current);
        }
        case 11:
        {
            return ___PostUploadFile(in, current);
        }
        case 12:
        {
            return ___PostUploadIcon(in, current);
        }
        case 13:
        {
            return ___PostUploadThumb(in, current);
        }
        case 14:
        {
            return ___RenameFileFolder(in, current);
        }
        case 15:
        {
            return ___SaveOnlineBookmark(in, current);
        }
        case 16:
        {
            return ___UploadImgSeq(in, current);
        }
        case 17:
        {
            return ___UploadOLBM(in, current);
        }
        case 18:
        {
            return ___ice_id(in, current);
        }
        case 19:
        {
            return ___ice_ids(in, current);
        }
        case 20:
        {
            return ___ice_isA(in, current);
        }
        case 21:
        {
            return ___ice_ping(in, current);
        }
    }

    assert(false);
    throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
}

void
UUTOOLBAR::User::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
UUTOOLBAR::User::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
UUTOOLBAR::User::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type UUTOOLBAR::User was not generated with stream support";
    throw ex;
}

void
UUTOOLBAR::User::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type UUTOOLBAR::User was not generated with stream support";
    throw ex;
}

void 
UUTOOLBAR::__patch__UserPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::UUTOOLBAR::UserPtr* p = static_cast< ::UUTOOLBAR::UserPtr*>(__addr);
    assert(p);
    *p = ::UUTOOLBAR::UserPtr::dynamicCast(v);
    if(v && !*p)
    {
        IceInternal::Ex::throwUOE(::UUTOOLBAR::User::ice_staticId(), v->ice_id());
    }
}

bool
UUTOOLBAR::operator==(const ::UUTOOLBAR::User& l, const ::UUTOOLBAR::User& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
UUTOOLBAR::operator<(const ::UUTOOLBAR::User& l, const ::UUTOOLBAR::User& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}
