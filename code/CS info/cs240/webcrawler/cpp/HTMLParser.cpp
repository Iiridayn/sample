#include "HTMLParser.h"
#include "InputStream.h"
#include "URLConnection.h"
#include "StringUtil.h"
#include "Webpage.h"
#include <cctype>
#include <iostream>
#include "CS240Exception.h"

HTMLParser::HTMLParser(string inURL, WordTree* inwords, PageQueue* inpages, StopWords* instops) {
  URL = inURL;
  words = inwords;
  pages = inpages;
  stops = instops;
  InputStream* temp;
  bool got = false;
  for(int i = 0; !got && i < 5; i++) {
    try {
      temp = URLConnection::Open(URL);
      got = true;
    }
    catch(InvalidURLException &e) { return; }
    catch(NetworkException &e) { got = false; }
    catch(CS240Exception &e) {
      cerr << "Error: " << e.GetMessage() << endl;
      return;
    }
  }
  int c;
  while((c = temp->Read()) != -1)
    page += (char)c;
  delete temp;
  lower = page;
  StringUtil::ToLower(lower);
  ripTitle();
  ripLinks();
  ripWords();
}

void HTMLParser::ripTitle() {
  int front = lower.find("<title>")+7;
  int back = lower.find("</title>");
  if(lower.find("<title></title>") != -1) 
    Title = "No Title";
  else
    Title = page.substr(front, back-front);
  //other cases?
  if(Title == "") {
    front = lower.find("<h1>")+4;
    back = lower.find("</h1>");
    Title = page.substr(front, back-front);
  }
  if(Title == "") Title = "No Title";
}

void HTMLParser::ripLinks() {
  ripComments();
  int backup = 0;
  int front = lower.find("href=", lower.find("<a")+1)+5; //what about src?
  int back = lower.find(">", front);
  string tempurl;
  while(front < page.length()&& front > backup) { //what is returned if !found
    if(lower[front] == '\"') {
      front++;
      back = lower.find("\"", front);
      tempurl = page.substr(front, back-front);
    }
    else {
      back = lower.find(" ", front);
      tempurl = page.substr(front, back-front);
    }
    if(Webpage::isValid(tempurl, URL))
      pages->insert(tempurl);
    backup = front;
    front = lower.find("href=", lower.find("<a", back)+1)+5;
    back = lower.find(">", front); //better to find " "?
  }//while
  //check algorhythim, replace w/white?
  front = lower.find("src=", lower.find("<a")+1)+5;
  back = lower.find(">", front);
  while(front < page.length()&& front > backup) { //what is returned if !found
    if(lower[front] == '\"') {
      front++;
      back = lower.find("\"", front);
      tempurl = page.substr(front, back-front);
    }
    else {
      back = lower.find(" ", front);
      tempurl = page.substr(front, back-front);
    }
    if(Webpage::isValid(tempurl, URL))
      pages->insert(tempurl);
    backup = front;
    front = lower.find("src=", lower.find("<a", back)+1)+5;
    back = lower.find(">", front); //better to find " "?
  }//while
}

void HTMLParser::ripComments() {
  int front = lower.find("<!--");
  int back = lower.find("-->", front)+3;
  while(front != -1 && front < lower.length() && back != -1) {
    lower.replace(front, (back-front), " ");
    page.replace(front, (back-front), " ");
    front = lower.find("<!--");
    back = lower.find("-->", front);
  }
}

void HTMLParser::ripTags() { //and funky, and punct
  lower = lower.substr(lower.find("<body>")+6, lower.length());
  lower.erase(lower.find("</html>"));
 
  //script is special
  int front = lower.find("<script>");
  int back = lower.find("</script>", front)+9;
  while(front != -1 && front < lower.length() && back != -1) {
    lower.replace(front, (back-front), " ");
    front = lower.find("<script>");
    back = lower.find("</script>", front);
  }

  front = lower.find("<");
  back = lower.find(">", front)+1;
  while(front < lower.length() && front != -1 && back != -1) {
    while(lower[front+1] == '<') front++; // fixes minor bug
    if((front != -1)&&((isalpha(lower[front+1]))||(lower[front+1] == '/'))) // strip front and back tags
      lower.replace(front, (back - front), " ");
    front = lower.find("<", back);
    back = lower.find(">", front)+1;
  }
  //copied
  for(int loc = lower.find("&"); loc < lower.length(); loc = lower.find("&", loc+1)) {
    if((lower[loc+1] == '#')&&isdigit(lower[loc+2])&&isdigit(lower[loc+3])&&isdigit(lower[loc+4])&&(lower[loc+5] == ';')) lower.replace(loc, 6, " ");
    else {
      int i = 1;
      while(!isspace(lower[loc+i])) {
        if(lower[loc+i] == ';') {
          lower.replace(loc, i+1, " ");
          break;
        }
        i++;
      }
    }
  }//done w/funky
  //strip punct ?-ispuct does what want?
  for(int i = 0; i < lower.length(); i++) {
    if(ispunct(lower[i])) lower[i] = ' ';
  }
}

void HTMLParser::ripWords() {
  ripTags();
  string word = "";
  for(int i = 0; i < lower.length(); i++) {
    if(isalpha(lower[i])) {
      while(!isspace(lower[i])) {
        word += lower[i];
        i++;
      }
      //cout << word << endl;
      if(!stops->isStop(word)) 
        words->insert(word, URL, Title); //check syntax
      word = "";
    }
  }
}
