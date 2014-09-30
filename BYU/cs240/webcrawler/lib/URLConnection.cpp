
#include "URLConnection.h"
#include "CS240Exception.h"
#include "HTTPInputStream.h"
#include "FileInputStream.h"
#include "StringUtil.h"


InputStream * URLConnection::Open(const string & url) {
  if (StringUtil::IsPrefix(url, "file:")) {
    return new FileInputStream(url);
  }
  else if (StringUtil::IsPrefix(url, "http://")) {
    return new HTTPInputStream(url);
  }
  else {
    throw InvalidURLException(url);
  }
}

