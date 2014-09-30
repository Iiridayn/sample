
#ifndef REFERENCE_H
#define REFERENCE_H

#include "ObjectCount.h"


template <class T>
class Reference : public ObjectCount< Reference<T> > { 
private:
  T* ptr;

public:
  Reference() {
    ptr = 0;
  }

  Reference(T* p) {
    ptr = p;
    if (ptr != 0) {
      ptr->AddRef();
    }
  }

  Reference(const Reference& other) {
    ptr = other.ptr;
    if (ptr != 0) {
      ptr->AddRef();
    }
  }

  ~Reference() {
    if (ptr != 0) {
      if (ptr->ReleaseRef() == 0) {
        delete ptr;
      }
    }
  }
  
  void operator =(T* p) {
    if (ptr != 0) {
      if (ptr->ReleaseRef() == 0) {
        delete ptr;
      }
    }
    ptr = p;
    if (ptr != 0) {
      ptr->AddRef();
    }
  }

  void operator =(const Reference<T>& other) {
    if (&other != this) {
      if (ptr != 0) {
        if (ptr->ReleaseRef() == 0) {
          delete ptr;
        }
      }
      ptr = other.ptr;
      if (ptr != 0) {
        ptr->AddRef();
      }
    }
  }

  bool operator ==(const Reference<T>& other) {
    return (ptr == other.ptr);
  }

  bool operator !=(const Reference<T>& other) {
    return !operator ==(other);
  }

  operator bool() {
    return (ptr != 0);
  }

  bool operator !() {
    return (ptr == 0);
  }

  bool IsNull() {
    return (ptr == 0);
  }

  bool NotNull() {
    return (ptr != 0);
  }

  operator T*() {
    return ptr;
  }

  T* operator ->() {
    return ptr;
  }

  T& operator *() {
    return *ptr;
  }
};


#endif
