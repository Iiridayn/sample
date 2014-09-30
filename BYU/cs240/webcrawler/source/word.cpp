// word.cpp made by Ged Crimsonclaw 11/7/2002
// contains information about a Word, and methods
// for managing a linked list of them

#include "word.h"
#include <string>

word* word::head = 0;

word::word(string in_value, string in_url, string in_title) {
  value = in_value;
  nextw = 0;
  slist = new siteList(in_url, in_title); //grab here?
} //end constructor

word::~word() { //how about lists of sites?
  delete slist;
} //end destructor

void word::del() {
  for(word* temp = word::head; temp != 0; temp = temp->nextw) {
    temp->dele();
    delete temp;
  }
}

void word::dele() {
  slist->del();
  delete slist;
}

void word::insert(string wword, string url, string title) {
  //cout << "word::insert(" << wword << ", " << url << ", " << title <<")" << endl;
  word* w = new word(wword, url, title);
  if(word::head == 0) { //empty list
    head = w; //url
  }
  else {
    word* temp = isin(*w);
    //cout << "isin() = " << temp << endl;
    if(temp != 0)
      temp->slist->insert(url, title);
    else {
      w->nextw = head;
      head = w;
    }
  }
}// end insert

word* word::isin(word& w) {
  word *temp = head;
  while(temp != 0) {
    //cout << temp->value << " == " << w.value << " = " << (*temp == w) << endl;
    if(*temp == w)
      return temp;
    temp = temp->nextw;
  }
  return 0;
}

void word::printList(word* thead = head)
{
  word* temp = thead;
  while(temp != 0) {
    cout << '\t'  << temp->value << endl;
    if(temp == temp->nextw) {
      cout << "Infinate loop.  Terminated." << endl;
      exit(10);
    }
    temp = temp->nextw;
  }
}

bool word::operator ==(word& w) {
  if(w.value == this->value)
    return true;
  return false;
} // end operator ==

word* word::next() {
  if(head == 0) return 0;
  word* temp = head;
  head = head->nextw;
  return temp;
} //end next

bool word::operator <=(word &w) {
  if(w.value <= this->value)
    return true;
  return false;
} // end operator <

bool word::operator >(word &w) {
  if(w.value > this->value)
    return true;
  return false;
} // end operator >

void word::sort() {
  //now mergeSort
  head = mergeSort(head);
}

word* word::mergeSort(word* list) {
  word* list2;
  //cout << "mergeSort(" << list->value  << ")" << list->nextw  << endl;
  if(list == 0) return 0;
  else if(list->nextw == 0) return list;
  else {
    //cout << (int)list->nextw << endl;
    list2 = split(list);
    return merge(mergeSort(list), mergeSort(list2));
  }
}

word* word::split(word* list) {
  word* list2;
  //cout << "split()" << endl;
  if(list == 0) return 0;
  else if(list->nextw == 0) return 0;
  else {
    list2 = list->nextw;
    list->nextw = list2->nextw;
    list2->nextw = split(list2->nextw);
    return list2;
  }
}

word* word::merge(word* list1, word* list2) {
  //cout << "merge()" << endl;
  //cout << "merge(" << list1->value << ", " << list2->value  << ")" << endl;
  if(list1 == 0) return list2;
  else if(list2 == 0) return list1;
  else if(*list1 > *list2) {
    //cout << list1->value  << " <= " << list2->value  << endl;
    //printList(list1);
    //cout <<"<SUB>" << endl;
    list1->nextw = merge(list1->nextw, list2);
    //cout << "</SUB>"  << endl;
    //printList(list1);
    return list1;
  }
  else {
    //cout << list1->value  << ">" << list2->value  << endl;
    //printList(list2);
    //cout << "<SUB>" << endl;
    list2->nextw = merge(list1, list2->nextw);
    //cout << "</SUB>" << endl;
    //printList(list2);
    return list2;
  }
}

site2* word::nextSite() {
  return slist->pull();
}
