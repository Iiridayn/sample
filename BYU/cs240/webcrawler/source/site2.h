// site2.h by Ged Crimsonclaw 11/7/2002
// header for site2.cpp

#ifndef SITE2_H
#define SITE2_H

#include "../lib/ObjectCount.h"
#include <string>

class site2 : public ObjectCount<site2> {
  public:
  site2(string, string);
  site2* next;
  string location;
  string name;
  bool operator ==(site2);
};

#endif
