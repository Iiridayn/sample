#ifndef WORD_H
#define WORD_H

#include <string>
#include "ObjectCount.h"
#include "Webpage.h"
#include <iostream>

class Word : public ObjectCount<Word> {
 public:
  Word* left;
  Word* right;
  string word;
  
  Word(string inword) {
    word = inword;
    left = 0;
    right = 0;
    head = 0;
  }
  void insert(string url, string title);
  Webpage* pop(); //caller deletes
  ~Word();
  void Print(ostream &os, bool verb = false);
  static bool Test(ostream &os);
 private:
  Webpage* head;
};

#endif
