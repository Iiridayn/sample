
#include "StringUtil.h"
#include <ctype.h>


bool StringUtil::IsPrefix(const string & str, const string & prefix) {
#if (__GNUC__ > 2)
  return (str.length() >= prefix.length() &&
	  str.compare(0, prefix.length(), prefix) == 0);
#else
  return (str.length() >= prefix.length() &&
	  str.compare(prefix, 0, prefix.length()) == 0);
#endif
}


bool StringUtil::IsSuffix(const string & str, const string & suffix) {
#if (__GNUC__ > 2)
  return (str.length() >= suffix.length() &&
	  str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0);
#else
  return (str.length() >= suffix.length() &&
	  str.compare(suffix, str.length() - suffix.length(), suffix.length()) == 0);
#endif
}


void StringUtil::ToLower(string & str) {
  string::iterator p = str.begin();
  while (p != str.end()) {
    *p = tolower(*p);
    ++p;
  }
}
