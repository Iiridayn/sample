#include "HTMLGenerator.h"
#include "HTMLParser.h"
#include "PageQueue.h"
#include "StopWords.h"
#include "WordTree.h"
#include "Word.h"
#include "Webpage.h"
#include <iostream>

void Webpage::Print(ostream &os) {
  os << "url: " << url << endl;
  if(title != "")
    os << "title: " << title << endl;
  return;
}

void Word::Print(ostream &os, bool verb) {
  os << word << endl;
  Webpage* temp = head;
  if(verb)
    while(temp != 0) {
      temp->Print(os);
      temp = temp->next;
    }
  return;
}

void WordTree::Print(ostream &os, bool verb) {
  Print(os, head, verb);
  return;
}

void WordTree::Print(ostream &os, Word* temp, bool verb) {
  if(temp == 0) return;
  Print(os, temp->right, verb);
  temp->Print(os, verb);
  Print(os, temp->left, verb);
  return;
}

void StopWords::Print(ostream &os) {
  words->Print(os);
  return;
}

void PageQueue::Print(ostream &os) {
  Webpage* temp = head;
  bool print = false;
  while(temp != 0) {
    if(temp == current) {
      //os << "Current:";
      //temp->Print(os);
      print = true;
    }
    //else
    if(print)
      temp->Print(os);
    temp = temp->next;
  }
  return;
}

void HTMLGenerator::Print(ostream &os) {
  os << "outdir: " << outdir << endl;
  os << "WordTree: " << endl;
  words->Print(os);
}

void HTMLParser::Print(ostream &os) {
  os << "this' url: " << URL << endl;
  os << "The title: " << Title << endl;
  //os << "And for the rest, I may make a verbose option" << endl;
}
