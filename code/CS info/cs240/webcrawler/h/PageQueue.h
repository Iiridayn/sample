#ifndef PAGEQUEUE_H
#define PAGEQUEUE_H

#include "ObjectCount.h"
#include "Webpage.h"
#include <iostream>

class PageQueue : public ObjectCount<PageQueue> {
 public:
  PageQueue() { head = 0; current = 0; return; }
  void insert(string URL, string title = "");
  bool isEmpty() { return (current==0); }
  string getNext(); //caller !del
  ~PageQueue();
  void Print(ostream &os);
  static bool Test(ostream &os);
 private:
  Webpage* head;
  Webpage* current;
};

#endif
