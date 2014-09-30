
#include "StringUtil.h"


bool StringUtil::IsPrefix(const string & str, const string & prefix) {
  return (str.length() >= prefix.length() &&
	  str.compare(prefix, 0, prefix.length()) == 0);
}


bool StringUtil::IsSuffix(const string & str, const string & suffix) {
  return (str.length() >= suffix.length() &&
	  str.compare(suffix, str.length() - suffix.length(), suffix.length()) == 0);
}


void StringUtil::ToLower(string & str) {
  string::iterator p = str.begin();
  while (p != str.end()) {
    *p = tolower(*p);
    ++p;
  }
}
