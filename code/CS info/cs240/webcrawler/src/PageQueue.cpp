#include "PageQueue.h"

void PageQueue::insert(string URL, string title) {
  if(head == 0) {
    head = new Webpage(URL, title);
    current = head;
  }
  else {
    Webpage* temp = head;
    while(temp->next != 0) {
      if(temp->url == URL)
	return;
      temp = temp->next;
    }
    if(temp->url == URL) return;
    temp->next = new Webpage(URL, title);
    if(current == 0) current = temp;
  }
}

string PageQueue::getNext() { //caller better not del, so is string
  if(current == 0) return 0;
  string temp = current->url;
  current = current->next;
  return temp;
}

PageQueue::~PageQueue() {
  Webpage* temp = head;
  while(temp != 0) {
    current = temp;
    temp = temp->next;
    delete current;
  }
  current = 0;
  head = 0;
}
