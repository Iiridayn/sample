
#include "HTTPInputStream.h"
#include "CS240Exception.h"

#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>


HTTPInputStream::HTTPInputStream(const string & url) {
  host = "";
  port = 80;
  path = "/";
  sockfd = -1;
  contentLength = -1;
  numRead = 0;

  ParseURL(url);
  //***cout << "[host=" << host << ", port=" << port << ", path=" << path << "]" << endl;
  OpenConnection();
  //***cout << "[connection established]" << endl;
  SendRequest();
  ParseHTTPHeader();
}


HTTPInputStream::~HTTPInputStream() {
  Close();
}


bool HTTPInputStream::IsOpen() {
  return (sockfd >= 0);
}


int HTTPInputStream::Read() {
  if (!IsOpen()) {
    throw IllegalStateException("stream is not open");
  }
  
  char c;
  int nread = read(sockfd, &c, 1);
  if (nread == 1) {
    ++numRead;
    return c;
  }
  else if (nread == 0) {
    //***cout << "[end of stream - contentLength: " << contentLength << ", numRead: " << numRead << "]" << endl;
    if (contentLength >= 0 && numRead != contentLength) {
      throw NetworkException("number of bytes read differs from content length");
    }
    return -1;
  }
  else {
    throw NetworkException("error occurred reading HTTP response");
  }
}


void HTTPInputStream::Close() {
  if (IsOpen()) {
    close(sockfd);
    sockfd = -1;
  }
}


void HTTPInputStream::ParseURL(const string & url) {
  int i;
  const string prefix = "http://";

  string::const_iterator p = url.begin();
  for (i=0; i < prefix.length(); ++i) {
    ++p;
  }

  while (p != url.end() && *p != ':' && *p != '/') {
    host.push_back(*p);
    ++p;
  }

  if (host == "") {
    throw InvalidURLException(url);
  }

  if (p == url.end()) {
    return;
  }

  if (*p == ':') {
    string portStr = "";
    while (p != url.end() && isdigit(*p)) {
      portStr.push_back(*p);
      ++p;
    }
    port = atoi(portStr.c_str());
 }

  if (p == url.end()) {
    return;
  }

  if (*p != '/') {
    throw InvalidURLException(url);
  }

  int idx = p - url.begin();
  path = url.substr(idx); 
}


void HTTPInputStream::OpenConnection() {
  struct hostent *hostData = gethostbyname(host.c_str());
  if (hostData == NULL || hostData->h_addr == NULL) {
    throw NetworkException(string("could not resolve host name ") + host);
  }
  
  struct sockaddr_in hostAddr;
  bzero(&hostAddr, sizeof(hostAddr));
  hostAddr.sin_family = AF_INET;
  hostAddr.sin_port = htons(port);
  memcpy(&hostAddr.sin_addr, hostData->h_addr, hostData->h_length);

  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    throw NetworkException("could not create socket");
  }

  if (connect(s, (struct sockaddr *)&hostAddr, sizeof(hostAddr)) < 0) {
    close(s);
    throw NetworkException(string("could not connect to host ") + host);
  }

  sockfd = s;
}


void HTTPInputStream::SendRequest() {
  stringstream request;
  request << "GET " << path << " HTTP/1.0\r\nHost: " << host << ":" << port << "\r\n\r\n";
  string str = request.str();
  //***cout << str;
  if (write(sockfd, str.c_str(), str.length()) != str.length()) {
    throw NetworkException("could not send HTTP request");
  }
}


void HTTPInputStream::ParseHTTPHeader() {
  while (true) {
    string line;
    ReadHeaderLine(line);

    if (line == "") {
      break;
    }

    if (line.find("Content-Length:") == 0) {
      ParseContentLength(line);
    }
  }
}


void HTTPInputStream::ParseContentLength(const string & line) {
  int i;
  const string prefix = "Content-Length:";

  string::const_iterator p = line.begin();
  for (i=0; i < prefix.length(); ++i) {
    ++p;
  }

  while (p != line.end() && isspace(*p)) {
    ++p;
  }

  if (p == line.end()) {
    throw NetworkException(string("invalid HTTP content length header: ") + line);
  }

  string length;
  while (p != line.end() && isdigit(*p)) {
    length.push_back(*p);
    ++p;
  }

  if (p != line.end()) {
    Close();
    throw NetworkException(string("invalid HTTP content length header: ") + line);
  }

  contentLength = atoi(length.c_str());
}


void HTTPInputStream::ReadHeaderLine(string & line) {
  bool gotCR = false;
  while (true) {
    char c;
    int nread = read(sockfd, &c, 1);
    if (nread == 1) {
      if (gotCR) {
	if (c == '\n') {
	  return;
	}
	else {
	  throw NetworkException("invalid HTTP header");
	}
      }
      else if (c == '\r') {
	gotCR = true;
      }
      else if (c == '\n') {
	throw NetworkException("invalid HTTP header");	
      }
      else {
	line.push_back(c);
      }
    }
    else {
      throw NetworkException("invalid HTTP header");
    }
  }
}


void HTTPInputStream::DumpResponse() {
  int c;
  while ((c = Read()) != -1) {
    cout << (char)c;
  }
}


