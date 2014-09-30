// siteList.h by Ged Crimsonclaw 11/7/2002
// header file for siteList.cpp

#ifndef SITELIST_H
#define SITELIST_H

#include "../lib/ObjectCount.h"
#include <string>
#include "site2.h"

class siteList : public ObjectCount<siteList> {
  private:
  site2* head;

  public:
  siteList(string, string); //url && title
  ~siteList();
  void insert(string, string); //see constructor
  site2* pull();
  void del();
};

#endif
