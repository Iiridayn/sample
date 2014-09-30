
#include "FileInputStream.h"
#include "CS240Exception.h"

#include <sys/types.h>
#include <sys/stat.h>


FileInputStream::FileInputStream(const string & url) {
  fileSize = 0;
  numRead = 0;

  ParseURL(url);
  OpenFile();
}


FileInputStream::~FileInputStream() {
  Close();
}


bool FileInputStream::IsOpen() {
  return file.is_open();
}


int FileInputStream::Read() {
  if (numRead == fileSize) {
    Close();
    return -1;
  }
  else {
    char c;
    file.get(c);

    if (file.fail()) {
      throw FileException(string("error reading from file ") + fileName);
    }

    ++numRead;
    return c;
  }
}


void FileInputStream::Close() {
  if (IsOpen()) {
    file.close();
  }
}


void FileInputStream::ParseURL(const string & url) {
  const string prefix = "file:";
  fileName = url.substr(prefix.length());
}


void FileInputStream::OpenFile() {
  struct stat buf;
  if (stat(fileName.c_str(), &buf) < 0) {
    throw FileException(string("could not determine size of file ") + fileName);
  }
  fileSize = buf.st_size;

  file.open(fileName.c_str());
  if (!IsOpen() || file.fail()) {
    throw FileException(string("could not open file ") + fileName);
  }
}

