// siteList.cpp by Ged Crimsonclaw 11/7/2002
// manages a list of site2s only ;P

#include "siteList.h"

siteList::siteList(string url, string title) {
  head = new site2(url, title);
} // end constructor

siteList::~siteList() {
  site2* temp = head;
  while(head != 0) {
    head = head->next;
    delete temp;
    temp = head;
  }
} // end destructor

void siteList::del() {
  site2* temp = head;
  while(head != 0) {
    head = head->next;
    delete temp;
    temp = head;
  }
}

void siteList::insert(string url, string title) {
  //cout << "siteList::insert(" << url << ", " << title << ")" << endl;
  //cout << "head = " << head;
  site2* s = new site2(url, title);
  if(head == 0) {
    cout << "to head" << endl;
    head = s;
    return;
  }
  site2* temp;
  for(temp = head; temp != 0; temp = temp->next) {
    //cout << temp->location << " == " << url << " = " << (*temp==*s) << endl;
    if(*temp == *s) {
      delete s;
      return;
    }
  }
  s->next = head;
  head = s;
}

site2* siteList::pull() {
  if(head != 0) {
    site2* temp = head;
    head = head->next;
    return temp;
  }
  return 0;
}
