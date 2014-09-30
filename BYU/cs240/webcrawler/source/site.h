/* Site.h made by Ged Crimsonclaw 11/4/2002
   header file for site.cpp                 */

#ifndef SITE_H
#define SITE_H

#include "../lib/ObjectCount.h"
#include <string>

class site : public ObjectCount<site> {
  private:
  static site* head;
  string url;
  bool visited;
  site* nexts;

  public:
  site(string);
  static void insert(string);
  bool operator ==(site &);
  static string next();
  static void del(string);
  static void dele();
};

#endif
