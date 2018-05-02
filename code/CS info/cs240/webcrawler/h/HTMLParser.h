#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include "ObjectCount.h"
#include "WordTree.h"
#include "PageQueue.h"
#include "StopWords.h"
#include <string>
#include <iostream>

class HTMLParser : public ObjectCount<HTMLParser> {
 public:
  HTMLParser(string inURL, WordTree* inwords, PageQueue* inpages, StopWords* instops);
  ~HTMLParser() { words = 0; pages = 0; stops = 0;}
  void Print(ostream &os);
  static bool Test(ostream &os);
 private:
  void ripTitle();
  void ripLinks();
  void ripTags();
  void ripWords();
  void ripComments();
  WordTree* words;
  PageQueue* pages;
  StopWords* stops;
  string URL;
  string Title;
  string page;
  string lower;
};

#endif
