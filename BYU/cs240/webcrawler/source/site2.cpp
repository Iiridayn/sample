// site2.cpp by Ged Crimsonclaw 11/7/2002
// does almost nothing anyway

#include "site2.h"

site2::site2(string url, string title) {
  location = url;
  name = title;
  next = 0;
} // end constructor

bool site2::operator ==(site2 s) {
  if(s.location == this->location)
    return true;
  return false;
} // end operator ==
