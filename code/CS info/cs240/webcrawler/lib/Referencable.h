
#ifndef REFERENCABLE_H
#define REFERENCABLE_H

#include "ObjectCount.h"


class Referencable : public ObjectCount<Referencable> {
private:
  int refCount;

public:
  Referencable() {
    refCount = 0;
  }

  int AddRef() {
    return ++refCount;
  }

  int ReleaseRef() {
    return --refCount;
  }
};


#endif
