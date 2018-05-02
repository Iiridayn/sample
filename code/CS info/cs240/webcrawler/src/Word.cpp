#include "Word.h"

void Word::insert(string url, string title) {
  if(head != 0) {
    Webpage* temp = head;
    while(temp->next != 0) {
      if(temp->url == url)
	return;
      temp = temp->next;
    }
    if(temp->url == url)
      return;
    temp->next = new Webpage(url, title);
  }
  else
    head = new Webpage(url, title); 
}

Webpage* Word::pop() { //caller must delete object...
  if(head == 0) return 0;
  Webpage* temp = head;
  head = head->next;
  return temp;
}

Word::~Word() {
  Webpage* temp = head;
  while(temp != 0) {
    head = temp;
    temp = temp->next;
    delete head;
  }
  head = 0;
}
