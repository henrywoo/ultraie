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

#include <UltraIE.h>
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

static const ::std::string __UUTOOLBAR__User__Logout_name = "Logout";

static const ::std::string __UUTOOLBAR__User__CreateFolder_name = "CreateFolder";

static const ::std::string __UUTOOLBAR__User__DeleteFolder_name = "DeleteFolder";

static const ::std::string __UUTOOLBAR__User__RenameFolder_name = "RenameFolder";

static const ::std::string __UUTOOLBAR__User__UsedSpaceSize_name = "UsedSpaceSize";

static const ::std::string __UUTOOLBAR__User__SaveFavLink_name = "SaveFavLink";

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
IceProxy::UUTOOLBAR::User::Login(const ::std::string& username, const ::std::string& password, const ::Ice::Context* __ctx)
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
            return __del->Login(username, password, __ctx);
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
IceProxy::UUTOOLBAR::User::Logout(const ::std::string& username, const ::Ice::Context* __ctx)
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
            return __del->Logout(username, __ctx);
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
IceProxy::UUTOOLBAR::User::CreateFolder(const ::std::string& foldername, ::Ice::Int FolderPriv, const ::Ice::Context* __ctx)
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
            return __del->CreateFolder(foldername, FolderPriv, __ctx);
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
IceProxy::UUTOOLBAR::User::DeleteFolder(const ::std::string& foldername, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__DeleteFolder_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->DeleteFolder(foldername, __ctx);
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
IceProxy::UUTOOLBAR::User::RenameFolder(const ::std::string& oldname, const ::std::string& newname, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__RenameFolder_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->RenameFolder(oldname, newname, __ctx);
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
IceProxy::UUTOOLBAR::User::UsedSpaceSize(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__UsedSpaceSize_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->UsedSpaceSize(__ctx);
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
IceProxy::UUTOOLBAR::User::SaveFavLink(const ::std::string& url, const ::std::string& lnkname, const ::std::string& desc, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__UUTOOLBAR__User__SaveFavLink_name);
            __delBase = __getDelegate(false);
            ::IceDelegate::UUTOOLBAR::User* __del = dynamic_cast< ::IceDelegate::UUTOOLBAR::User*>(__delBase.get());
            return __del->SaveFavLink(url, lnkname, desc, __ctx);
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

bool
IceDelegateM::UUTOOLBAR::User::Login(const ::std::string& username, const ::std::string& password, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__Login_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(username);
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
IceDelegateM::UUTOOLBAR::User::Logout(const ::std::string& username, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__Logout_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(username);
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
IceDelegateM::UUTOOLBAR::User::CreateFolder(const ::std::string& foldername, ::Ice::Int FolderPriv, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__CreateFolder_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(foldername);
        __os->write(FolderPriv);
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
IceDelegateM::UUTOOLBAR::User::DeleteFolder(const ::std::string& foldername, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__DeleteFolder_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(foldername);
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
IceDelegateM::UUTOOLBAR::User::RenameFolder(const ::std::string& oldname, const ::std::string& newname, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__RenameFolder_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(oldname);
        __os->write(newname);
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
IceDelegateM::UUTOOLBAR::User::UsedSpaceSize(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__UsedSpaceSize_name, ::Ice::Normal, __context);
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
IceDelegateM::UUTOOLBAR::User::SaveFavLink(const ::std::string& url, const ::std::string& lnkname, const ::std::string& desc, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__handler.get(), __UUTOOLBAR__User__SaveFavLink_name, ::Ice::Normal, __context);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(url);
        __os->write(lnkname);
        __os->write(desc);
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
IceDelegateD::UUTOOLBAR::User::Login(const ::std::string& username, const ::std::string& password, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& username, const ::std::string& password, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_username(username),
            _m_password(password)
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
            _result = servant->Login(_m_username, _m_password, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_username;
        const ::std::string& _m_password;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__Login_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, username, password, __current);
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
IceDelegateD::UUTOOLBAR::User::Logout(const ::std::string& username, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& username, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_username(username)
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
            _result = servant->Logout(_m_username, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_username;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__Logout_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, username, __current);
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
IceDelegateD::UUTOOLBAR::User::CreateFolder(const ::std::string& foldername, ::Ice::Int FolderPriv, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& foldername, ::Ice::Int FolderPriv, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_foldername(foldername),
            _m_FolderPriv(FolderPriv)
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
            _result = servant->CreateFolder(_m_foldername, _m_FolderPriv, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_foldername;
        ::Ice::Int _m_FolderPriv;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__CreateFolder_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, foldername, FolderPriv, __current);
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
IceDelegateD::UUTOOLBAR::User::DeleteFolder(const ::std::string& foldername, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& foldername, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_foldername(foldername)
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
            _result = servant->DeleteFolder(_m_foldername, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_foldername;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__DeleteFolder_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, foldername, __current);
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
IceDelegateD::UUTOOLBAR::User::RenameFolder(const ::std::string& oldname, const ::std::string& newname, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& oldname, const ::std::string& newname, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_oldname(oldname),
            _m_newname(newname)
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
            _result = servant->RenameFolder(_m_oldname, _m_newname, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_oldname;
        const ::std::string& _m_newname;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__RenameFolder_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, oldname, newname, __current);
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
IceDelegateD::UUTOOLBAR::User::UsedSpaceSize(const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(::Ice::Long& __result, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result)
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
            _result = servant->UsedSpaceSize(_current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        ::Ice::Long& _result;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__UsedSpaceSize_name, ::Ice::Normal, __context);
    ::Ice::Long __result;
    try
    {
        _DirectI __direct(__result, __current);
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
IceDelegateD::UUTOOLBAR::User::SaveFavLink(const ::std::string& url, const ::std::string& lnkname, const ::std::string& desc, const ::Ice::Context* __context)
{
    class _DirectI : public ::IceInternal::Direct
    {
    public:

        _DirectI(bool& __result, const ::std::string& url, const ::std::string& lnkname, const ::std::string& desc, const ::Ice::Current& __current) : 
            ::IceInternal::Direct(__current),
            _result(__result),
            _m_url(url),
            _m_lnkname(lnkname),
            _m_desc(desc)
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
            _result = servant->SaveFavLink(_m_url, _m_lnkname, _m_desc, _current);
            return ::Ice::DispatchOK;
        }
        
    private:
        
        bool& _result;
        const ::std::string& _m_url;
        const ::std::string& _m_lnkname;
        const ::std::string& _m_desc;
    };
    
    ::Ice::Current __current;
    __initCurrent(__current, __UUTOOLBAR__User__SaveFavLink_name, ::Ice::Normal, __context);
    bool __result;
    try
    {
        _DirectI __direct(__result, url, lnkname, desc, __current);
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
    ::std::string username;
    ::std::string password;
    __is->read(username);
    __is->read(password);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = Login(username, password, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___Logout(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string username;
    __is->read(username);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = Logout(username, __current);
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
    ::Ice::Int FolderPriv;
    __is->read(foldername);
    __is->read(FolderPriv);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = CreateFolder(foldername, FolderPriv, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___DeleteFolder(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string foldername;
    __is->read(foldername);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = DeleteFolder(foldername, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___RenameFolder(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string oldname;
    ::std::string newname;
    __is->read(oldname);
    __is->read(newname);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = RenameFolder(oldname, newname, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___UsedSpaceSize(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    __inS.is()->skipEmptyEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::Ice::Long __ret = UsedSpaceSize(__current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

::Ice::DispatchStatus
UUTOOLBAR::User::___SaveFavLink(::IceInternal::Incoming& __inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    __is->startReadEncaps();
    ::std::string url;
    ::std::string lnkname;
    ::std::string desc;
    __is->read(url);
    __is->read(lnkname);
    __is->read(desc);
    __is->endReadEncaps();
    ::IceInternal::BasicStream* __os = __inS.os();
    bool __ret = SaveFavLink(url, lnkname, desc, __current);
    __os->write(__ret);
    return ::Ice::DispatchOK;
}

static ::std::string __UUTOOLBAR__User_all[] =
{
    "CreateFolder",
    "DeleteFolder",
    "Login",
    "Logout",
    "RenameFolder",
    "SaveFavLink",
    "UsedSpaceSize",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping"
};

::Ice::DispatchStatus
UUTOOLBAR::User::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__UUTOOLBAR__User_all, __UUTOOLBAR__User_all + 11, current.operation);
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
            return ___DeleteFolder(in, current);
        }
        case 2:
        {
            return ___Login(in, current);
        }
        case 3:
        {
            return ___Logout(in, current);
        }
        case 4:
        {
            return ___RenameFolder(in, current);
        }
        case 5:
        {
            return ___SaveFavLink(in, current);
        }
        case 6:
        {
            return ___UsedSpaceSize(in, current);
        }
        case 7:
        {
            return ___ice_id(in, current);
        }
        case 8:
        {
            return ___ice_ids(in, current);
        }
        case 9:
        {
            return ___ice_isA(in, current);
        }
        case 10:
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
