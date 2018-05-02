#ifndef HTMLGENERATOR_H
#define HTMLGENERATOR_H

#include "ObjectCount.h"
#include <string>
#include "WordTree.h"
#include "Word.h"
#include "Webpage.h"
#include <iostream>

class HTMLGenerator : public ObjectCount<HTMLGenerator> {
 public:
  HTMLGenerator(WordTree* inwords, string inout);
  void Print(ostream &os);
  static bool Test(ostream &os);
 private:
  void addWord(Word* word, ostream &outword);
  void addPage(Webpage* page, ostream &outpage);
  WordTree* words;
  string outdir;
};

#endif
