#include "Webpage.h"

string Webpage::Domain;

bool Webpage::isValid(string &link, string current) {
  if(link.find("..") != -1) return false;
  if(link.find("./") != -1) link.replace(link.find("./"), 2, "");
  if(!StringUtil::IsSuffix(link, ".html") && !StringUtil::IsSuffix(link, ".htm"))
    return false;
  if(!StringUtil::IsPrefix(link, "http://") && !StringUtil::IsPrefix(link, "file:/")) {
    string DIR = getDir(current);
    if(link[0] == '/') //fix relative?
      link = Domain + DIR + link.substr(1, link.length());
    else
      link = Domain + DIR  + link;
  }
  if(link.find(Domain) == -1) 
    return false;
  return true;
}
