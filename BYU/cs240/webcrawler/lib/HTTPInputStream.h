
#ifndef HTTP_INPUT_STREAM_H
#define HTTP_INPUT_STREAM_H

#include "InputStream.h"
#include "ObjectCount.h"

#include <string>


class HTTPInputStream : public InputStream, public ObjectCount<HTTPInputStream> {
 public:
  HTTPInputStream(const string & url);
  ~HTTPInputStream();

  bool IsOpen();
  int Read();
  void Close();

 private:
  string host;
  int port;
  string path;
  int sockfd;
  int contentLength;
  int numRead;

  void ParseURL(const string & url);
  void OpenConnection();
  void SendRequest();
  void ParseHTTPHeader();
  void ParseContentLength(const string & line);
  void ReadHeaderLine(string & line);

  void DumpResponse();
};


#endif
