/*  site.cpp made by Ged Crimsonclaw 11/4/2002
    stores info about a site and manages a linked
    list of them                                */

#include "site.h"

site* site::head = 0;

site::site(string link) {
  url = link;
  nexts = 0;
  visited = false;
}

void site::insert(string si) {
  site* s = new site(si);
  if(site::head == 0) { //empty list
    head = s;
  }
  else {
    site* temp;
    for(temp = site::head; temp->nexts != 0; temp = temp->nexts) {
      if(*temp == *s)
        return;
    }
    temp->nexts = s;
  }
} //end insert

bool site::operator ==(site &s) {
  if(s.url == this->url)
    return true;
  return false;
} //end operator ==

string site::next() {
  site* temp = head;
  while(temp->visited) {
    temp = temp->nexts;
    if(temp == 0) return "";
  }
  temp->visited = true;
  return temp->url;
} //end next

void site::dele() {
  for(site* temp = site::head; temp != 0; temp = temp->nexts) {
    delete temp;
  }
}

void site::del(string url) {
  if(site::head == 0) return;
  site* temp2 = 0;
  for(site * temp = site::head; temp != 0; temp2 = temp, temp = temp->nexts) {
    if(temp->url == url) {
      if(temp2 == 0) {
        head = temp->nexts;
      }
      else {
        temp2->nexts = temp->nexts;
        delete temp;
      }
    }
  }
}
