#include "HTMLGenerator.h"
#include "HTMLParser.h"
#include "PageQueue.h"
#include "StopWords.h"
#include "WordTree.h"
#include "Word.h"
#include "Webpage.h"
#include <iostream>
#include "print.h"

int main(int argc, char* argv[]) {
  if(!Webpage::Test(cout))
    cout << "Error in Webpage" << endl;
  else if(!Word::Test(cout))
    cout << "Error in Word" << endl;
  else if(!WordTree::Test(cout))
    cout << "Error in WordTree" << endl;
  else if(!StopWords::Test(cout))
    cout << "Error in StopWords" << endl;
  else if(!PageQueue::Test(cout))
    cout << "Error in PageQueue" << endl;
  else if(!HTMLGenerator::Test(cout))
    cout << "Error in HTMLGenerator" << endl;
  else if(!HTMLParser::Test(cout))
    cout << "Error in HTMLParser" << endl;
  else
    cout << "All tests successful" << endl;
  return 0;
}

bool Webpage::Test(ostream &os) {
  os << "Webpage:" << endl;
  bool good = true;
  getDomain(string("http://faculty.cs.byu.edu/~rodham/cs240/index.html"));
  os << "Domain: " << Domain << endl;
  string temp = "http://faculty.cs.byu.edu/~rodham/cs240/webcrawler/index.html";  if(!isValid(temp))
    good = false;
  temp = "http://www.google.com/index.html";
  if(isValid(temp))
    good = false;
  temp = "index.html";
  if(!isValid(temp, "http://faculty.cs.byu.edu/~rodham/cs240/webcrawler/"))
    good = false;
  temp = "/index.html";
  if(!isValid(temp, "http://faculty.cs.byu.edu/~rodham/cs240/webcrawler/"))
    good = false;
  temp = "fragged.html";
  if(!isValid(temp, "http://faculty.cs.byu.edu/~rodham/cs240/webcrawler/") || !(temp == Domain + getDir("http://faculty.cs.byu.edu/~rodham/cs240/webcrawler/") + "fragged.html"))
    good = false;
  return good;
}

bool Word::Test(ostream &os) {
  os << "Word:" << endl;
  Word* temp = new Word("All your base are belong to us");
  temp->insert("http://www.google.com/", "Google!");
  temp->insert("http://www.google.com/", "Google!");
  temp->insert("http://www.google.com/", "Google!");
  temp->insert("http://www.yahoo.com/", "Yahoo!");
  temp->insert("http://www.yahoo.com/", "Yahoo!");
  temp->insert("http://www.yahoo.com/", "Yahoo!");
  Webpage* page = temp->pop();
  int tot = 0;
  while(page != 0) {
    delete page;
    tot++;
    page = temp->pop();
  }
  delete temp;
  os << "Objects: " << ObjectCountBase::GetGlobalObjectCount() << endl;
  if(tot != 2)
    return false;
  return true;
}

bool WordTree::Test(ostream &os) {
  os << "WordTree:" << endl;
  WordTree* temp = new WordTree();
  temp->insert("bob");
  temp->insert("bob");
  temp->insert("cat");
  temp->insert("cat");
  temp->insert("all");
  temp->insert("bob");
  temp->insert("fred");
  temp->insert("all");
  Word* word = temp->pop();
  word->Print(os);
  delete word;
  word = temp->pop();
  word->Print(os);
  delete word;
  word = temp->pop();
  word->Print(os);
  delete word;
  word = temp->pop();
  word->Print(os);
  delete word;
  bool ret = true;
  if(!temp->isEmpty()) ret = false;
  delete temp;
  os << "Objects: " << ObjectCountBase::GetGlobalObjectCount() << endl;
  return ret;
}

bool StopWords::Test(ostream &os) {
  os << "StopWords:" << endl;
  StopWords* temp = new StopWords("stopword.txt");
  bool ret = true;
  if(!temp->isStop("too"))
    ret = false;
  if(!temp->isStop("u"))
    ret = false;
  if(!temp->isStop("i"))
    ret = false;
  if(temp->isStop("thou"))
    ret = false;
  if(temp->isStop("where"))
    ret = false;
  delete temp;
  os << "Objects: " << ObjectCountBase::GetGlobalObjectCount() << endl;
  return ret;
}

bool PageQueue::Test(ostream &os) {
  os << "PageQueue:" << endl;
  PageQueue* temp = new PageQueue();
  temp->insert("lala");
  temp->insert("lala");
  temp->insert("foot");
  temp->insert("lala");
  temp->insert("foot");
  string what = temp->getNext();
  os << what << endl;
  what = temp->getNext();
  os << what << endl;
  bool ret = temp->isEmpty();
  delete temp;
  os << "Objects: " << ObjectCountBase::GetGlobalObjectCount() << endl;
  return ret;
}

bool HTMLGenerator::Test(ostream &os) {
  os << "HTMLGenerator:" << endl;
  os << "Can't test right now..." << endl;
  os << "Objects: " << ObjectCountBase::GetGlobalObjectCount() << endl;
}

bool HTMLParser::Test(ostream &os) {
  os << "HTMLParser:" << endl;
  os << "Can't test right now..." << endl;
  os << "Objects: " << ObjectCountBase::GetGlobalObjectCount() << endl;
}
