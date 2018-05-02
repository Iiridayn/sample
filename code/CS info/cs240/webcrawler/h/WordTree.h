#ifndef WORDTREE_H
#define WORDTREE_H

#include "ObjectCount.h"
#include "Word.h"
#include <string>
#include <iostream>

class WordTree : public ObjectCount<WordTree> {
 public:
  WordTree(){head = 0; return;}
  void insert(string word, string url = "", string title = "");
  bool isin(string word);
  ~WordTree() { destruct(head); head = 0;}
  bool isEmpty() { return (head==0);}
  Word* pop();
  void Print(ostream &os, bool verb = false);
  void Print(ostream &os, Word* temp, bool verb = false);
  static bool Test(ostream &os);
 private:
  Word* pop(Word* temp);
  void destruct(Word* temp);
  Word* head;
};

#endif
