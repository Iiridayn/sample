
#ifndef FILE_INPUT_STREAM_H
#define FILE_INPUT_STREAM_H

#include "InputStream.h"
#include "ObjectCount.h"

#include <string>
#include <fstream>
using namespace std;


class FileInputStream : public InputStream, public ObjectCount<FileInputStream> {
 public:
  FileInputStream(const string & url);
  ~FileInputStream();

  bool IsOpen();
  int Read();
  void Close();

 private:
  string fileName;
  ifstream file;
  off_t fileSize;
  off_t numRead;

  void ParseURL(const string & url);
  void OpenFile();
};


#endif
