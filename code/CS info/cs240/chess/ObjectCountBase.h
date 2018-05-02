

#ifndef OBJECT_COUNT_BASE_H
#define OBJECT_COUNT_BASE_H


class ObjectCountBase {
 private:
  static int global_creations;
  static int global_deletions;
  
 public:
  ObjectCountBase() {
    ++global_creations;
  }

  ~ObjectCountBase() {
    ++global_deletions;
  }

  static int GetGlobalCreations() {
    return global_creations;
  }

  static int GetGlobalDeletions() {
    return global_deletions;
  }

  static int GetGlobalObjectCount() {
    return (global_creations - global_deletions);
  }
};


#endif
