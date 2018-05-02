#include "StopWords.h"
#include <string>
#include <iostream>
#include <fstream>
#include "CS240Exception.h"
#include <cctype>

StopWords::StopWords(string infilename) {
  words = new WordTree();
  ifstream infile(infilename.c_str());
  if(!infile)
    throw InvalidArgumentException("bad file");
  string tempW = "";
  while(!infile.eof()) {
    char tempc = infile.get();
    if(!isspace(tempc))
      tempW += tempc;
    else {
      words->insert(tempW);
      tempW = "";
    }
  }
}
