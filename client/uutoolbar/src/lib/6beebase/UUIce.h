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

#ifndef __uuice_h__
#define __uuice_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <Ice/StreamF.h>
#include <Ice/UndefSysMacros.h>

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

namespace IceProxy
{

namespace UUTOOLBAR
{

class User;

}

}

namespace UUTOOLBAR
{

class User;
bool operator==(const User&, const User&);
bool operator<(const User&, const User&);

}

namespace IceInternal
{

::Ice::Object* upCast(::UUTOOLBAR::User*);
::IceProxy::Ice::Object* upCast(::IceProxy::UUTOOLBAR::User*);

}

namespace UUTOOLBAR
{

typedef ::IceInternal::Handle< ::UUTOOLBAR::User> UserPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::UUTOOLBAR::User> UserPrx;

void __read(::IceInternal::BasicStream*, UserPrx&);
void __patch__UserPtr(void*, ::Ice::ObjectPtr&);

}

namespace UUTOOLBAR
{

typedef ::std::vector< ::Ice::Byte> Imgdata;

struct Image
{
    ::std::string name;
    ::UUTOOLBAR::Imgdata data;

    bool operator==(const Image&) const;
    bool operator<(const Image&) const;
    bool operator!=(const Image& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const Image& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const Image& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const Image& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::UUTOOLBAR::Image> ImgSeq;
void __writeImgSeq(::IceInternal::BasicStream*, const ::UUTOOLBAR::Image*, const ::UUTOOLBAR::Image*);
void __readImgSeq(::IceInternal::BasicStream*, ImgSeq&);

struct OLBMValue
{
    ::std::string title;
    ::std::string description;
    ::std::string tag;
    ::std::string folder;
    ::std::string iconURL;

    bool operator==(const OLBMValue&) const;
    bool operator<(const OLBMValue&) const;
    bool operator!=(const OLBMValue& __rhs) const
    {
        return !operator==(__rhs);
    }
    bool operator<=(const OLBMValue& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const OLBMValue& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const OLBMValue& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::map< ::std::string, ::UUTOOLBAR::OLBMValue> OLBMItems;
void __writeOLBMItems(::IceInternal::BasicStream*, const OLBMItems&);
void __readOLBMItems(::IceInternal::BasicStream*, OLBMItems&);

typedef ::std::vector< ::std::string> IEFavURLs;

enum FOLDERPRIV
{
    ANYONECAN,
    FRIENDCAN,
    ONLYSELF,
    OTHERSNEEDPASS
};

void __write(::IceInternal::BasicStream*, FOLDERPRIV);
void __read(::IceInternal::BasicStream*, FOLDERPRIV&);

enum LOGINRESPONSE
{
    UNKNOWNUSER,
    WRONGPASS,
    LOGINOK,
    ACCOUNTDISABLED
};

void __write(::IceInternal::BasicStream*, LOGINRESPONSE);
void __read(::IceInternal::BasicStream*, LOGINRESPONSE&);

enum UPLOADFILETYPE
{
    MYICON,
    MYTHUMB
};

void __write(::IceInternal::BasicStream*, UPLOADFILETYPE);
void __read(::IceInternal::BasicStream*, UPLOADFILETYPE&);

}

namespace IceProxy
{

namespace UUTOOLBAR
{

class User : virtual public ::IceProxy::Ice::Object
{
public:

    ::UUTOOLBAR::LOGINRESPONSE Login(const ::std::string& email, const ::std::string& password, ::std::string& sid)
    {
        return Login(email, password, sid, 0);
    }
    ::UUTOOLBAR::LOGINRESPONSE Login(const ::std::string& email, const ::std::string& password, ::std::string& sid, const ::Ice::Context& __ctx)
    {
        return Login(email, password, sid, &__ctx);
    }
    
private:

    ::UUTOOLBAR::LOGINRESPONSE Login(const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Context*);
    
public:

    ::UUTOOLBAR::LOGINRESPONSE LoginUUFile(const ::std::string& email, const ::std::string& password, ::std::string& sid)
    {
        return LoginUUFile(email, password, sid, 0);
    }
    ::UUTOOLBAR::LOGINRESPONSE LoginUUFile(const ::std::string& email, const ::std::string& password, ::std::string& sid, const ::Ice::Context& __ctx)
    {
        return LoginUUFile(email, password, sid, &__ctx);
    }
    
private:

    ::UUTOOLBAR::LOGINRESPONSE LoginUUFile(const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Context*);
    
public:

    bool Logout(const ::std::string& sid)
    {
        return Logout(sid, 0);
    }
    bool Logout(const ::std::string& sid, const ::Ice::Context& __ctx)
    {
        return Logout(sid, &__ctx);
    }
    
private:

    bool Logout(const ::std::string&, const ::Ice::Context*);
    
public:

    bool CreateFolder(const ::std::string& foldername, ::UUTOOLBAR::FOLDERPRIV fp, const ::std::string& sid)
    {
        return CreateFolder(foldername, fp, sid, 0);
    }
    bool CreateFolder(const ::std::string& foldername, ::UUTOOLBAR::FOLDERPRIV fp, const ::std::string& sid, const ::Ice::Context& __ctx)
    {
        return CreateFolder(foldername, fp, sid, &__ctx);
    }
    
private:

    bool CreateFolder(const ::std::string&, ::UUTOOLBAR::FOLDERPRIV, const ::std::string&, const ::Ice::Context*);
    
public:

    bool DeleteFileFolder(const ::std::string& foldername, const ::std::string& sid)
    {
        return DeleteFileFolder(foldername, sid, 0);
    }
    bool DeleteFileFolder(const ::std::string& foldername, const ::std::string& sid, const ::Ice::Context& __ctx)
    {
        return DeleteFileFolder(foldername, sid, &__ctx);
    }
    
private:

    bool DeleteFileFolder(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    bool RenameFileFolder(const ::std::string& oldname, const ::std::string& newname, const ::std::string& sid)
    {
        return RenameFileFolder(oldname, newname, sid, 0);
    }
    bool RenameFileFolder(const ::std::string& oldname, const ::std::string& newname, const ::std::string& sid, const ::Ice::Context& __ctx)
    {
        return RenameFileFolder(oldname, newname, sid, &__ctx);
    }
    
private:

    bool RenameFileFolder(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    ::Ice::Long PostUploadFile(const ::std::string& filename, ::Ice::Long filesize, const ::std::string& crc32, const ::std::string& desc, const ::std::string& tag, const ::std::string& email, const ::std::string& sid, ::std::string& fetchurl)
    {
        return PostUploadFile(filename, filesize, crc32, desc, tag, email, sid, fetchurl, 0);
    }
    ::Ice::Long PostUploadFile(const ::std::string& filename, ::Ice::Long filesize, const ::std::string& crc32, const ::std::string& desc, const ::std::string& tag, const ::std::string& email, const ::std::string& sid, ::std::string& fetchurl, const ::Ice::Context& __ctx)
    {
        return PostUploadFile(filename, filesize, crc32, desc, tag, email, sid, fetchurl, &__ctx);
    }
    
private:

    ::Ice::Long PostUploadFile(const ::std::string&, ::Ice::Long, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Context*);
    
public:

    bool GetUserInfo(const ::std::string& email, ::Ice::Long& spaceused, ::Ice::Long& ULnum)
    {
        return GetUserInfo(email, spaceused, ULnum, 0);
    }
    bool GetUserInfo(const ::std::string& email, ::Ice::Long& spaceused, ::Ice::Long& ULnum, const ::Ice::Context& __ctx)
    {
        return GetUserInfo(email, spaceused, ULnum, &__ctx);
    }
    
private:

    bool GetUserInfo(const ::std::string&, ::Ice::Long&, ::Ice::Long&, const ::Ice::Context*);
    
public:

    bool UploadImgSeq(const ::UUTOOLBAR::ImgSeq& imgs, const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::UPLOADFILETYPE type, ::Ice::Long id)
    {
        return UploadImgSeq(imgs, email, sid, type, id, 0);
    }
    bool UploadImgSeq(const ::UUTOOLBAR::ImgSeq& imgs, const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::UPLOADFILETYPE type, ::Ice::Long id, const ::Ice::Context& __ctx)
    {
        return UploadImgSeq(imgs, email, sid, type, id, &__ctx);
    }
    
private:

    bool UploadImgSeq(const ::UUTOOLBAR::ImgSeq&, const ::std::string&, const ::std::string&, ::UUTOOLBAR::UPLOADFILETYPE, ::Ice::Long, const ::Ice::Context*);
    
public:

    ::Ice::Long SaveOnlineBookmark(const ::std::string& url, const ::std::string& title, const ::std::string& desc, const ::std::string& tag, const ::std::string& email, const ::std::string& sid)
    {
        return SaveOnlineBookmark(url, title, desc, tag, email, sid, 0);
    }
    ::Ice::Long SaveOnlineBookmark(const ::std::string& url, const ::std::string& title, const ::std::string& desc, const ::std::string& tag, const ::std::string& email, const ::std::string& sid, const ::Ice::Context& __ctx)
    {
        return SaveOnlineBookmark(url, title, desc, tag, email, sid, &__ctx);
    }
    
private:

    ::Ice::Long SaveOnlineBookmark(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    bool GetOLBMURLs(const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::IEFavURLs& existingOLBMURLs)
    {
        return GetOLBMURLs(email, sid, existingOLBMURLs, 0);
    }
    bool GetOLBMURLs(const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::IEFavURLs& existingOLBMURLs, const ::Ice::Context& __ctx)
    {
        return GetOLBMURLs(email, sid, existingOLBMURLs, &__ctx);
    }
    
private:

    bool GetOLBMURLs(const ::std::string&, const ::std::string&, ::UUTOOLBAR::IEFavURLs&, const ::Ice::Context*);
    
public:

    bool UploadOLBM(const ::UUTOOLBAR::OLBMItems& items, const ::std::string& email, const ::std::string& sid)
    {
        return UploadOLBM(items, email, sid, 0);
    }
    bool UploadOLBM(const ::UUTOOLBAR::OLBMItems& items, const ::std::string& email, const ::std::string& sid, const ::Ice::Context& __ctx)
    {
        return UploadOLBM(items, email, sid, &__ctx);
    }
    
private:

    bool UploadOLBM(const ::UUTOOLBAR::OLBMItems&, const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    bool DownloadOLBM(const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::OLBMItems& items)
    {
        return DownloadOLBM(email, sid, items, 0);
    }
    bool DownloadOLBM(const ::std::string& email, const ::std::string& sid, ::UUTOOLBAR::OLBMItems& items, const ::Ice::Context& __ctx)
    {
        return DownloadOLBM(email, sid, items, &__ctx);
    }
    
private:

    bool DownloadOLBM(const ::std::string&, const ::std::string&, ::UUTOOLBAR::OLBMItems&, const ::Ice::Context*);
    
public:

    bool PostUploadThumb(const ::std::string& thumbname, ::Ice::Long id, const ::std::string& sid)
    {
        return PostUploadThumb(thumbname, id, sid, 0);
    }
    bool PostUploadThumb(const ::std::string& thumbname, ::Ice::Long id, const ::std::string& sid, const ::Ice::Context& __ctx)
    {
        return PostUploadThumb(thumbname, id, sid, &__ctx);
    }
    
private:

    bool PostUploadThumb(const ::std::string&, ::Ice::Long, const ::std::string&, const ::Ice::Context*);
    
public:

    bool PostUploadIcon(const ::std::string& iconname, ::Ice::Long id, const ::std::string& sid)
    {
        return PostUploadIcon(iconname, id, sid, 0);
    }
    bool PostUploadIcon(const ::std::string& iconname, ::Ice::Long id, const ::std::string& sid, const ::Ice::Context& __ctx)
    {
        return PostUploadIcon(iconname, id, sid, &__ctx);
    }
    
private:

    bool PostUploadIcon(const ::std::string&, ::Ice::Long, const ::std::string&, const ::Ice::Context*);
    
public:

    bool GetFTPInfoForUpload(const ::std::string& email, ::std::string& ip, ::Ice::Short& port)
    {
        return GetFTPInfoForUpload(email, ip, port, 0);
    }
    bool GetFTPInfoForUpload(const ::std::string& email, ::std::string& ip, ::Ice::Short& port, const ::Ice::Context& __ctx)
    {
        return GetFTPInfoForUpload(email, ip, port, &__ctx);
    }
    
private:

    bool GetFTPInfoForUpload(const ::std::string&, ::std::string&, ::Ice::Short&, const ::Ice::Context*);
    
public:

    ::std::string IsLatestVersion(const ::std::string& version)
    {
        return IsLatestVersion(version, 0);
    }
    ::std::string IsLatestVersion(const ::std::string& version, const ::Ice::Context& __ctx)
    {
        return IsLatestVersion(version, &__ctx);
    }
    
private:

    ::std::string IsLatestVersion(const ::std::string&, const ::Ice::Context*);
    
public:

    ::std::string IsFileUploaded(const ::std::string& crc32, ::Ice::Long filesize)
    {
        return IsFileUploaded(crc32, filesize, 0);
    }
    ::std::string IsFileUploaded(const ::std::string& crc32, ::Ice::Long filesize, const ::Ice::Context& __ctx)
    {
        return IsFileUploaded(crc32, filesize, &__ctx);
    }
    
private:

    ::std::string IsFileUploaded(const ::std::string&, ::Ice::Long, const ::Ice::Context*);
    
public:
    
    ::IceInternal::ProxyHandle<User> ice_context(const ::Ice::Context& __context) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_context(__context).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_context(__context).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_adapterId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_adapterId(__id).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_endpoints(__endpoints).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_locatorCacheTimeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_locatorCacheTimeout(__timeout).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_connectionCached(bool __cached) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_connectionCached(__cached).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_endpointSelection(__est).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_secure(bool __secure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_secure(__secure).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_preferSecure(bool __preferSecure) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_preferSecure(__preferSecure).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_router(const ::Ice::RouterPrx& __router) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_router(__router).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_router(__router).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_locator(__locator).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_collocationOptimized(bool __co) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_collocationOptimized(__co).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_twoway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_twoway().get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_twoway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_oneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_oneway().get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_oneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_batchOneway() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_batchOneway().get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_datagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_datagram().get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_datagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_batchDatagram() const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_batchDatagram().get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_compress(bool __compress) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_compress(__compress).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_timeout(int __timeout) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_timeout(__timeout).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    #endif
    }
    
    ::IceInternal::ProxyHandle<User> ice_connectionId(const std::string& __id) const
    {
    #if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
        typedef ::IceProxy::Ice::Object _Base;
        return dynamic_cast<User*>(_Base::ice_connectionId(__id).get());
    #else
        return dynamic_cast<User*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    #endif
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

namespace IceDelegate
{

namespace UUTOOLBAR
{

class User : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::UUTOOLBAR::LOGINRESPONSE Login(const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Context*) = 0;

    virtual ::UUTOOLBAR::LOGINRESPONSE LoginUUFile(const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Context*) = 0;

    virtual bool Logout(const ::std::string&, const ::Ice::Context*) = 0;

    virtual bool CreateFolder(const ::std::string&, ::UUTOOLBAR::FOLDERPRIV, const ::std::string&, const ::Ice::Context*) = 0;

    virtual bool DeleteFileFolder(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual bool RenameFileFolder(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::Ice::Long PostUploadFile(const ::std::string&, ::Ice::Long, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Context*) = 0;

    virtual bool GetUserInfo(const ::std::string&, ::Ice::Long&, ::Ice::Long&, const ::Ice::Context*) = 0;

    virtual bool UploadImgSeq(const ::UUTOOLBAR::ImgSeq&, const ::std::string&, const ::std::string&, ::UUTOOLBAR::UPLOADFILETYPE, ::Ice::Long, const ::Ice::Context*) = 0;

    virtual ::Ice::Long SaveOnlineBookmark(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual bool GetOLBMURLs(const ::std::string&, const ::std::string&, ::UUTOOLBAR::IEFavURLs&, const ::Ice::Context*) = 0;

    virtual bool UploadOLBM(const ::UUTOOLBAR::OLBMItems&, const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual bool DownloadOLBM(const ::std::string&, const ::std::string&, ::UUTOOLBAR::OLBMItems&, const ::Ice::Context*) = 0;

    virtual bool PostUploadThumb(const ::std::string&, ::Ice::Long, const ::std::string&, const ::Ice::Context*) = 0;

    virtual bool PostUploadIcon(const ::std::string&, ::Ice::Long, const ::std::string&, const ::Ice::Context*) = 0;

    virtual bool GetFTPInfoForUpload(const ::std::string&, ::std::string&, ::Ice::Short&, const ::Ice::Context*) = 0;

    virtual ::std::string IsLatestVersion(const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::std::string IsFileUploaded(const ::std::string&, ::Ice::Long, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace UUTOOLBAR
{

class User : virtual public ::IceDelegate::UUTOOLBAR::User,
             virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::UUTOOLBAR::LOGINRESPONSE Login(const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Context*);

    virtual ::UUTOOLBAR::LOGINRESPONSE LoginUUFile(const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Context*);

    virtual bool Logout(const ::std::string&, const ::Ice::Context*);

    virtual bool CreateFolder(const ::std::string&, ::UUTOOLBAR::FOLDERPRIV, const ::std::string&, const ::Ice::Context*);

    virtual bool DeleteFileFolder(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual bool RenameFileFolder(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::Ice::Long PostUploadFile(const ::std::string&, ::Ice::Long, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Context*);

    virtual bool GetUserInfo(const ::std::string&, ::Ice::Long&, ::Ice::Long&, const ::Ice::Context*);

    virtual bool UploadImgSeq(const ::UUTOOLBAR::ImgSeq&, const ::std::string&, const ::std::string&, ::UUTOOLBAR::UPLOADFILETYPE, ::Ice::Long, const ::Ice::Context*);

    virtual ::Ice::Long SaveOnlineBookmark(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual bool GetOLBMURLs(const ::std::string&, const ::std::string&, ::UUTOOLBAR::IEFavURLs&, const ::Ice::Context*);

    virtual bool UploadOLBM(const ::UUTOOLBAR::OLBMItems&, const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual bool DownloadOLBM(const ::std::string&, const ::std::string&, ::UUTOOLBAR::OLBMItems&, const ::Ice::Context*);

    virtual bool PostUploadThumb(const ::std::string&, ::Ice::Long, const ::std::string&, const ::Ice::Context*);

    virtual bool PostUploadIcon(const ::std::string&, ::Ice::Long, const ::std::string&, const ::Ice::Context*);

    virtual bool GetFTPInfoForUpload(const ::std::string&, ::std::string&, ::Ice::Short&, const ::Ice::Context*);

    virtual ::std::string IsLatestVersion(const ::std::string&, const ::Ice::Context*);

    virtual ::std::string IsFileUploaded(const ::std::string&, ::Ice::Long, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace UUTOOLBAR
{

class User : virtual public ::IceDelegate::UUTOOLBAR::User,
             virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::UUTOOLBAR::LOGINRESPONSE Login(const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Context*);

    virtual ::UUTOOLBAR::LOGINRESPONSE LoginUUFile(const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Context*);

    virtual bool Logout(const ::std::string&, const ::Ice::Context*);

    virtual bool CreateFolder(const ::std::string&, ::UUTOOLBAR::FOLDERPRIV, const ::std::string&, const ::Ice::Context*);

    virtual bool DeleteFileFolder(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual bool RenameFileFolder(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::Ice::Long PostUploadFile(const ::std::string&, ::Ice::Long, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Context*);

    virtual bool GetUserInfo(const ::std::string&, ::Ice::Long&, ::Ice::Long&, const ::Ice::Context*);

    virtual bool UploadImgSeq(const ::UUTOOLBAR::ImgSeq&, const ::std::string&, const ::std::string&, ::UUTOOLBAR::UPLOADFILETYPE, ::Ice::Long, const ::Ice::Context*);

    virtual ::Ice::Long SaveOnlineBookmark(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual bool GetOLBMURLs(const ::std::string&, const ::std::string&, ::UUTOOLBAR::IEFavURLs&, const ::Ice::Context*);

    virtual bool UploadOLBM(const ::UUTOOLBAR::OLBMItems&, const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual bool DownloadOLBM(const ::std::string&, const ::std::string&, ::UUTOOLBAR::OLBMItems&, const ::Ice::Context*);

    virtual bool PostUploadThumb(const ::std::string&, ::Ice::Long, const ::std::string&, const ::Ice::Context*);

    virtual bool PostUploadIcon(const ::std::string&, ::Ice::Long, const ::std::string&, const ::Ice::Context*);

    virtual bool GetFTPInfoForUpload(const ::std::string&, ::std::string&, ::Ice::Short&, const ::Ice::Context*);

    virtual ::std::string IsLatestVersion(const ::std::string&, const ::Ice::Context*);

    virtual ::std::string IsFileUploaded(const ::std::string&, ::Ice::Long, const ::Ice::Context*);
};

}

}

namespace UUTOOLBAR
{

class User : virtual public ::Ice::Object
{
public:

    typedef UserPrx ProxyType;
    typedef UserPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::UUTOOLBAR::LOGINRESPONSE Login(const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___Login(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::UUTOOLBAR::LOGINRESPONSE LoginUUFile(const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___LoginUUFile(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool Logout(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___Logout(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool CreateFolder(const ::std::string&, ::UUTOOLBAR::FOLDERPRIV, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___CreateFolder(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool DeleteFileFolder(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___DeleteFileFolder(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool RenameFileFolder(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___RenameFileFolder(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Long PostUploadFile(const ::std::string&, ::Ice::Long, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___PostUploadFile(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool GetUserInfo(const ::std::string&, ::Ice::Long&, ::Ice::Long&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___GetUserInfo(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool UploadImgSeq(const ::UUTOOLBAR::ImgSeq&, const ::std::string&, const ::std::string&, ::UUTOOLBAR::UPLOADFILETYPE, ::Ice::Long, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___UploadImgSeq(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Long SaveOnlineBookmark(const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___SaveOnlineBookmark(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool GetOLBMURLs(const ::std::string&, const ::std::string&, ::UUTOOLBAR::IEFavURLs&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___GetOLBMURLs(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool UploadOLBM(const ::UUTOOLBAR::OLBMItems&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___UploadOLBM(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool DownloadOLBM(const ::std::string&, const ::std::string&, ::UUTOOLBAR::OLBMItems&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___DownloadOLBM(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool PostUploadThumb(const ::std::string&, ::Ice::Long, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___PostUploadThumb(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool PostUploadIcon(const ::std::string&, ::Ice::Long, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___PostUploadIcon(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool GetFTPInfoForUpload(const ::std::string&, ::std::string&, ::Ice::Short&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___GetFTPInfoForUpload(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::std::string IsLatestVersion(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___IsLatestVersion(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::std::string IsFileUploaded(const ::std::string&, ::Ice::Long, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___IsFileUploaded(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
