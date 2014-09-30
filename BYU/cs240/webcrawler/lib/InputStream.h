
#ifndef INPUT_STREAM_H
#define INPUT_STREAM_H

#include "ObjectCount.h"


class InputStream : public ObjectCount<InputStream> {
 public:
  virtual ~InputStream() {}
  virtual bool IsOpen() = 0;
  virtual int Read() = 0;
  virtual void Close() = 0;
};


#endif
