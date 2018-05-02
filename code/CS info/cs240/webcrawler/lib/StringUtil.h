
#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include "ObjectCount.h"

#include <string>
using namespace std;


class StringUtil : public ObjectCount<StringUtil> {
 public:
  static bool IsPrefix(const string & str, const string & prefix);
  static bool IsSuffix(const string & str, const string & suffix);
  static void ToLower(string & str);
};


#endif
