

#ifndef OBJECT_COUNT_H
#define OBJECT_COUNT_H


#include "ObjectCountBase.h"


template <class T>
class ObjectCount : virtual public ObjectCountBase {
 private:
  static int class_creations;
  static int class_deletions;

 public:
  ObjectCount() {
    ++class_creations;
  }

  ~ObjectCount() {
    ++class_deletions;
  }

  static int GetClassCreations() {
    return class_creations;
  }

  static int GetClassDeletions() {
    return class_deletions;
  }

  static int GetClassObjectCount() {
    return (class_creations - class_deletions);
  }
};


template <class T> int ObjectCount<T>::class_creations = 0;
template <class T> int ObjectCount<T>::class_deletions = 0;


#endif
