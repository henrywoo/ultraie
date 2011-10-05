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

#ifndef INC_RC_SMART_PTR_H
#define INC_RC_SMART_PTR_H

namespace nsSP
{
  // base class for reference-counted objects
  class RCObject
  {
  public:  
    void AddReference() { ++m_iRefCount; }
    void RemoveReference()
    {
      if (--m_iRefCount == 0)
        delete this;
    }

    void MarkUnshareable() { m_fShareable = false; }
    bool IsShareable() const { return m_fShareable; }
    bool IsShared() const { return m_iRefCount > 1; }

  protected:
    RCObject() : m_iRefCount(0), m_fShareable(true) {}
    RCObject(const RCObject& /*rhs*/) : m_iRefCount(0), m_fShareable(true) {}
    RCObject& operator=(const RCObject& /*rhs*/) { return *this; }

    virtual ~RCObject() {}

  private:
    int  m_iRefCount;
    bool m_fShareable;
  };

  /******************************************************************************
  *   Template Class RCPtr 
  ******************************************************************************/
  // template class for smart pointers-to-T objects; T must support the RCObject interface
  template<class T>
  class RCPtr
  {
  public:
    RCPtr(T* realPtr = 0) : m_Pointee(realPtr) { Init(); }
    RCPtr(const RCPtr& rhs) : m_Pointee(rhs.m_Pointee) { Init(); }
    ~RCPtr() { if (m_Pointee) m_Pointee->RemoveReference(); }
    RCPtr& operator=(const RCPtr& rhs)
    {
      if (m_Pointee != rhs.m_Pointee)
      {
        T* pOldPointee = m_Pointee;
        m_Pointee = rhs.m_Pointee;
        Init(); 

        if (pOldPointee)
          pOldPointee->RemoveReference();  
      }

      return *this;
    }
    T* operator->() const { return m_Pointee; };
    T& operator*() const { return *m_Pointee; };

    bool IsNull() const  { return m_Pointee==NULL; }
    bool IsValid() const { return m_Pointee!=NULL; }

  private:
    T* m_Pointee;

    void Init()
    {
      if (m_Pointee == 0)
        return;

      if (m_Pointee->IsShareable() == false)
        m_Pointee = new T(*m_Pointee);

      m_Pointee->AddReference();
    }
  };


  template<class T>
  class RCIPtr
  {
  public:
    RCIPtr(T* realPtr = 0) :
        m_pCounter(new CountHolder)
        { 
          m_pCounter->m_Pointee = realPtr;
          Init();
        }

        RCIPtr(const RCIPtr& rhs) : m_pCounter(rhs.m_pCounter) { Init(); }
        ~RCIPtr() { m_pCounter->RemoveReference(); }
        RCIPtr& operator=(const RCIPtr& rhs)
        {
          if (m_pCounter != rhs.m_pCounter)
          {
            m_pCounter->RemoveReference(); 
            m_pCounter = rhs.m_pCounter;
            Init();
          }
          return *this;
        }

        T* operator->() const { return m_pCounter->m_Pointee; }
        T& operator*() const { return *(m_pCounter->m_Pointee); }

        RCObject& GetRCObject() { return *m_pCounter; }

        bool IsNull() const  { return m_pCounter==NULL?true:m_pCounter->m_Pointee==NULL; }
        bool IsValid() const { return m_pCounter==NULL?false:m_pCounter->m_Pointee!=NULL; }

  private:
    struct CountHolder: public RCObject
    {
      ~CountHolder()
      { 
        delete m_Pointee;
      }
      T* m_Pointee;
    };

    CountHolder* m_pCounter;
    void Init()
    {
      if (m_pCounter->IsShareable() == false)
      {
        T* pOldValue = m_pCounter->m_Pointee;
        m_pCounter = new CountHolder;
        m_pCounter->m_Pointee = new T(*pOldValue);
      } 
      m_pCounter->AddReference();
    }
  };

};
#endif // INC_RC_SMART_PTR_H
