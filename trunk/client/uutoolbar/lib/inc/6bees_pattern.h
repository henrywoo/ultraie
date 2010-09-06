#ifndef INC_DEFINEMENTS_H
#define INC_DEFINEMENTS_H

#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <string>
#include <set>

#include "uulogging.h"
#include "6bees_util.h"

namespace _6bees_pattern{

  /// Base class for implementing the notification stuff.
  /// @remarks Inherit public (instead of private) because it wouldn't compile under Dev-C++
  template <typename T, typename T2>
  class CObserverBase : public std::set<T>{
  public:
    typedef typename std::set<T> base_type;
    typedef typename std::set<T>::iterator iterator;

    ~CObserverBase(){
      for( iterator it=base_type::begin(); it!=base_type::end(); it=base_type::begin() )
        (*it)->Detach(static_cast<T2>(this));
    }

    bool Attach(T p){
      if (find(p)!=base_type::end())
        return false;
      insert(p);
      p->Attach(static_cast<T2>(this));
      return true;
    }

    bool Detach(T p){
      if (find(p)==base_type::end())
        return false;
      erase(p);
      p->Detach(static_cast<T2>(this));
      return true;
    }
  };

  
}

#endif // INC_DEFINEMENTS_H
