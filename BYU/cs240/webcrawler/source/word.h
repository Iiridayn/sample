// word.h made by Ged Crimsonclaw 11-7-2002
// header file for word.cpp

#ifndef WORD_H
#define WORD_H

#include "../lib/ObjectCount.h"
#include <string>
#include "siteList.h"

class word : public ObjectCount<word> {
  private:
  static word* head;
  siteList* slist;
  word* nextw;
  static word* mergeSort(word*);
  static word* split(word*);
  static word* merge(word*, word*);
  static word* isin(word &);
  void dele();

  public:
  word(string, string, string);
  ~word();
  static void insert(string, string, string); //url,title
  bool operator ==(word &);
  static word* next();
  bool operator <=(word &);
  bool operator >(word &);
  static void sort();
  site2* nextSite();
  string value;
  static void printList(word *);
  static void del();
};

#endif
