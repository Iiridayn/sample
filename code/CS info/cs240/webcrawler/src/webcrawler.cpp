#include <iostream>
#include "StopWords.h"
#include "HTMLParser.h"
#include "HTMLGenerator.h"
#include "WordTree.h"
#include "PageQueue.h"
#include "Webpage.h"
#include "ObjectCount.h"
#include "print.h"

int usage() { cout << "USAGE: webcrawler startpage outputdir stopwordsfile" << endl; return 1;}

int main(int argc, char* argv[]) {
  PageQueue* pages;
  StopWords* stops;
  string outdir;
  if(argc < 4)
    return usage();
  else {
    string tempw = argv[1];
    if(Webpage::isValid(tempw))
      Webpage::getDomain(tempw);
    else {
      cout << "Invalid webpage." << endl;
      return 2;
    }
    pages = new PageQueue();
    pages->insert(argv[1]);

    outdir = argv[2];

    stops = new StopWords(argv[3]);
  }

  WordTree* words = new WordTree();
  while(!pages->isEmpty()) {
    HTMLParser* temp = new HTMLParser(pages->getNext(), words, pages, stops);
    delete temp;
  }
  delete pages;
  delete stops;
  HTMLGenerator* gen = new HTMLGenerator(words, outdir);
  delete gen;
  delete words;

  cout << "Stuff: " << ObjectCountBase::GetGlobalObjectCount() << endl;
}
