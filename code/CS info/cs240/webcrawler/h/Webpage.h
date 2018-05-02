#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <string>
#include "ObjectCount.h"
#include "StringUtil.h"
#include <iostream>

class Webpage : public ObjectCount<Webpage> {
 public:
  string url, title; //prefix?
  static string Domain;
  Webpage* next;

  Webpage(string inurl, string intitle = "") {
    url = inurl;
    title = intitle;
    next = 0;
  }
  
  static void getDomain(string link) {
    int back = link.find_last_of("/"); //?
    Domain = link.substr(0, back+1);
  }
  
  static string getDir(string link) {
    if(Domain != "") {
      int back = link.find_last_of("/")+1;
      int front = link.find(Domain)+Domain.length();
      if(back-front > 0)
	return link.substr(front, back-front);
      return "";
    }
    return "";
  }

  static bool isValid(string &link, string current = "");

  bool operator ==(Webpage &other) {
    if(other.url == url) return true;
    return false;
  }

  void Print(ostream &os);
  static bool Test(ostream &os);
};

#endif
