
#ifndef URL_CONNECTION_H
#define URL_CONNECTION_H

#include "ObjectCount.h"
#include "InputStream.h"

#include <string>
using namespace std;


class URLConnection : public ObjectCount<URLConnection> {
 public:
  static InputStream * Open(const string & url);
};


#endif
