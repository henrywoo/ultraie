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

// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.3.0
// Generated from file `UltraIE.ice'

#ifndef __UltraIE_h__
#define __UltraIE_h__

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

namespace IceProxy
{

namespace UUTOOLBAR
{

class User : virtual public ::IceProxy::Ice::Object
{
public:

    bool Login(const ::std::string& username, const ::std::string& password)
    {
        return Login(username, password, 0);
    }
    bool Login(const ::std::string& username, const ::std::string& password, const ::Ice::Context& __ctx)
    {
        return Login(username, password, &__ctx);
    }
    
private:

    bool Login(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    bool Logout(const ::std::string& username)
    {
        return Logout(username, 0);
    }
    bool Logout(const ::std::string& username, const ::Ice::Context& __ctx)
    {
        return Logout(username, &__ctx);
    }
    
private:

    bool Logout(const ::std::string&, const ::Ice::Context*);
    
public:

    bool CreateFolder(const ::std::string& foldername, ::Ice::Int FolderPriv)
    {
        return CreateFolder(foldername, FolderPriv, 0);
    }
    bool CreateFolder(const ::std::string& foldername, ::Ice::Int FolderPriv, const ::Ice::Context& __ctx)
    {
        return CreateFolder(foldername, FolderPriv, &__ctx);
    }
    
private:

    bool CreateFolder(const ::std::string&, ::Ice::Int, const ::Ice::Context*);
    
public:

    bool DeleteFolder(const ::std::string& foldername)
    {
        return DeleteFolder(foldername, 0);
    }
    bool DeleteFolder(const ::std::string& foldername, const ::Ice::Context& __ctx)
    {
        return DeleteFolder(foldername, &__ctx);
    }
    
private:

    bool DeleteFolder(const ::std::string&, const ::Ice::Context*);
    
public:

    bool RenameFolder(const ::std::string& oldname, const ::std::string& newname)
    {
        return RenameFolder(oldname, newname, 0);
    }
    bool RenameFolder(const ::std::string& oldname, const ::std::string& newname, const ::Ice::Context& __ctx)
    {
        return RenameFolder(oldname, newname, &__ctx);
    }
    
private:

    bool RenameFolder(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    ::Ice::Long UsedSpaceSize()
    {
        return UsedSpaceSize(0);
    }
    ::Ice::Long UsedSpaceSize(const ::Ice::Context& __ctx)
    {
        return UsedSpaceSize(&__ctx);
    }
    
private:

    ::Ice::Long UsedSpaceSize(const ::Ice::Context*);
    
public:

    bool SaveFavLink(const ::std::string& url, const ::std::string& lnkname, const ::std::string& desc)
    {
        return SaveFavLink(url, lnkname, desc, 0);
    }
    bool SaveFavLink(const ::std::string& url, const ::std::string& lnkname, const ::std::string& desc, const ::Ice::Context& __ctx)
    {
        return SaveFavLink(url, lnkname, desc, &__ctx);
    }
    
private:

    bool SaveFavLink(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
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

    virtual bool Login(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual bool Logout(const ::std::string&, const ::Ice::Context*) = 0;

    virtual bool CreateFolder(const ::std::string&, ::Ice::Int, const ::Ice::Context*) = 0;

    virtual bool DeleteFolder(const ::std::string&, const ::Ice::Context*) = 0;

    virtual bool RenameFolder(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::Ice::Long UsedSpaceSize(const ::Ice::Context*) = 0;

    virtual bool SaveFavLink(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;
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

    virtual bool Login(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual bool Logout(const ::std::string&, const ::Ice::Context*);

    virtual bool CreateFolder(const ::std::string&, ::Ice::Int, const ::Ice::Context*);

    virtual bool DeleteFolder(const ::std::string&, const ::Ice::Context*);

    virtual bool RenameFolder(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::Ice::Long UsedSpaceSize(const ::Ice::Context*);

    virtual bool SaveFavLink(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*);
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

    virtual bool Login(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual bool Logout(const ::std::string&, const ::Ice::Context*);

    virtual bool CreateFolder(const ::std::string&, ::Ice::Int, const ::Ice::Context*);

    virtual bool DeleteFolder(const ::std::string&, const ::Ice::Context*);

    virtual bool RenameFolder(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::Ice::Long UsedSpaceSize(const ::Ice::Context*);

    virtual bool SaveFavLink(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Context*);
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

    virtual bool Login(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___Login(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool Logout(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___Logout(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool CreateFolder(const ::std::string&, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___CreateFolder(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool DeleteFolder(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___DeleteFolder(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool RenameFolder(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___RenameFolder(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Long UsedSpaceSize(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___UsedSpaceSize(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual bool SaveFavLink(const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___SaveFavLink(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

#endif
