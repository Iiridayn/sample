#ifndef STOPWORDS_H
#define STOPWORDS_H

#include "WordTree.h"
#include "ObjectCount.h"
#include <string>
#include <iostream>

class StopWords : public ObjectCount<StopWords> {
 public:
  StopWords(string infilename);
  bool isStop(string word) { return words->isin(word); }
  ~StopWords() { delete words; }
  void Print(ostream &os);
  static bool Test(ostream &os);
 private:
  WordTree* words;
};

#endif
