// webcrawler.cpp by Ged Crimsonclaw 11/7/2002
// contains the general functions of the program

//#define TEST

#include <iostream>
#include <fstream>
#include <string>
#include "word.h"
#include "site2.h"
#include "site.h"
#include "../lib/URLConnection.h"
#include "../lib/InputStream.h"
#include "../lib/StringUtil.h"
#include "../lib/CS240Exception.h"

int crawl(string);
string getLinks(string &);
bool validLink(string &);
void stripTags(string &);
void getWords(string, string, string, string &);
bool toStop(string, string);
void output(string);
void makeIndex(string);
void startpages(string);
void endpages(string);
void makeWord(string, word);

string DOMAIN;
string DIR;

int main(int argc, char *argv[]) { //parse input, control flow
  string OUTPUT_DIR;
  string STOPWORDSFILE;
  
  if(argc < 4) {
    cout << "USAGE: webcrawler startpage outputdir stopwordsfile" << endl;
    return 1;
  }
  else {
    DOMAIN = argv[1]; //modified
    site::insert(DOMAIN);
    DOMAIN = DOMAIN.substr(0, DOMAIN.find_last_of("/")+1);
    DIR = "";
    //now mod domain (last /, substr to there)
#ifdef TEST
    cout << DOMAIN << endl;
#endif
    OUTPUT_DIR = argv[2];
    OUTPUT_DIR += '/';
    STOPWORDSFILE = argv[3];
  }
  int ret = 0;
  ret = crawl(STOPWORDSFILE);
  if(ret == 0)
    output(OUTPUT_DIR);

  //return codes here
  if(ret == 2) cerr << "Network error.  Try your crawling later." << endl;
  
  word::del();
  site::dele();
  cout << "Stuff: " << ObjectCountBase::GetGlobalObjectCount() << endl;
  return ret;
} // end main

int crawl(string stopfile) {
  for(string url = site::next(); url != ""; url = site::next()) {
#ifdef TEST
    cout << url << endl;
#endif    
    InputStream* temp;
    try {
      temp = URLConnection::Open(url);
    }
    catch(InvalidURLException& e){site::del(url); continue;} //skip rest
    catch(NetworkException& n){return 2;} //skip rest
    catch(CS240Exception& e){ cerr << "Error!" << endl;}
    string page = "";
    int c;
    while ((c = temp->Read()) != -1) {
       page += (char)c;
    }
    int substr1 = DOMAIN.length();
    int substr2 = url.find_last_of('/')+1;
    //cout << url.length() << " " << substr1 << " " << substr2 << endl;
    if((substr2 - substr1) > 0)
      DIR = url.substr(substr1, substr2-substr1); //filename...  //Why not working?
    else DIR = "";
    //cout << DIR << endl;
    string title = getLinks(page); //all by ref, so changed as you go
    //got past here
/*  no need?
#ifdef TEST
    cout << page << endl;
#endif
*/
    StringUtil::ToLower(page);
    stripTags(page); //can make lowercase before here
#ifdef TEST
    cout << page << endl;
#endif
    getWords(stopfile, url, title, page);
    //page.substr(-1, -1); //break it here
  }
  return 0;
}

string getLinks(string& page) {
  //must check for lowercase too...  HOW?  And vairable case...
  string temppage = page;
  StringUtil::ToLower(temppage);
  int front = temppage.find("<title>")+7; //substr <TITLE>, and </TITLE>, pull middle
  int back = temppage.find("</title>");
  string title = page.substr(front, back-front);
  string url = "";
  int backup = 0;
  for(front = temppage.find("href=", temppage.find("<a")+1)+5, back = temppage.find(">", front); front < page.length() && front > backup; backup = front, front = temppage.find("href=", temppage.find("<a", back)+1)+5, back = temppage.find(">", front)) {
    //now infinate loop... -_-;
    //find substr <A href=, then grab till whitespace or closing "  //find href seperate - done
    if(temppage[front] == '\"') { //whitespace doesn't matter
      front++;
      back = temppage.find('\"', front);
      url = page.substr(front, back-front);
    }
    else { //whitespace matters
      back = temppage.find(" ", front);
      url = page.substr(front, back-front);
    }
#ifdef TEST
    cout << "url: " << url << endl;
#endif
    if(validLink(url)) //needed? //should I accept releative urls?
      site::insert(url);
    //end loop
    url = "";
  }
  return title;
}

bool validLink(string& link) { //check if link is valid  //should I say yes to relative urls and fix them?  YES.  not work right
  //cout << "inurl: " << link << endl;
  if(!StringUtil::IsSuffix(link, ".html") && !StringUtil::IsSuffix(link, ".htm"))
    return false;
  if(!StringUtil::IsPrefix(link, "http://") && !StringUtil::IsPrefix(link, "file:/")) {
    if(link[0] == '/') //fix relative?
      link = DOMAIN + DIR + link.substr(1, link.length());
    else
      link = DOMAIN + DIR  + link;
  }
  if(link.find(DOMAIN) == -1)
    return false;
#ifdef TEST
  cout << "goodurl: " << link << endl;
#endif
  return true;
}

void stripTags(string& page) { //is lowercase now
  //head - works
  page = page.substr(page.find("</head>")+7, page.length()); //length?
  //comments - works
  int front, back;
  for(front = page.find("<!--"), back = page.find("-->", front)+3; front < page.length(); front = page.find("<!--"), back = page.find("-->", front)+3)
  {
    // I kill each, so can start search from start
    //cout << front << " " << back << endl;
    //cout << page.substr(front, back-front) << endl;
    if(front != -1) { //stops pages with no comments from making a mess
      page.replace(front, (back - front), " ");
      //cout << endl << page << endl;
    }
  }
  //after HTML - works
  page.erase(page.find("</html>"));
  //note: if prollems with links in comments, get links here.
  //tags - works?
  for(front = page.find("<"), back = page.find(">", front)+1; front < page.length(); front = page.find("<", front+1), back = page.find(">", front)+1)
  { //check if is follwed by a char or a '/'
    if((front != -1)&&(isalpha(page[front+1]) || page[front+1] == '/'))
      page.replace(front, (back - front), " ");
  }
  //strip funky stuff -- works?
  for(int loc = page.find("&"); loc < page.length(); loc = page.find("&", loc+1)) {
    if((page[loc+1] == '#')&&isdigit(page[loc+2])&&isdigit(page[loc+3])&&isdigit(page[loc+4])&&(page[loc+5] == ';')) page.replace(loc, 6, " ");
    else {
      int i = 1;
      while(!isspace(page[loc+i])) {
        if(page[loc+i] == ';') {
          page.replace(loc, i+1, " ");
          break;
	}
        i++;
      }
    }
  }//done w/funky
  //strip punct ?-ispuct does what want?
  for(int i = 0; i < page.length(); i++) {
    if(ispunct(page[i])) page[i] = ' ';
  }  
}

void getWords(string stopfile, string url, string title, string& page) {
  string word = "";
  for(int i = 0; i < page.length(); i++) {
    if(isalpha(page[i])) {
      while(!isspace(page[i])) {
        word += page[i];
        i++;
      }
      //cout << word << endl;
      if(toStop(stopfile, word))
        word::insert(word, url, title); //check syntax
      word = "";
    }
  }
}

bool toStop(string stopwords, string word) {
  ifstream stopfile(stopwords.c_str());
  while(stopfile.good()) {
    string temp;
    getline(stopfile, temp);
    if(word == temp) return false;
  }
  return true;
}

void output(string dir) {
  //cout << "All your base are belong to us!" << endl;
  //word::printList();
  //cout << "How are you gentlemen?" << endl;
  word::sort(); //word
  //cout << "Someone set up us the bomb!" << endl;
  makeIndex(dir);
  //cout << "We get signal!" << endl;
  startpages(dir);
  //cout << "Main screen turn on!" << endl;
  for(word* temp = word::next(); temp != 0; temp = word::next()) { //word
    char tempc = toupper(temp->value[0]);
    ofstream outfile(string(dir + tempc + ".html").c_str(), ios::app); //append only!
    outfile << "<A href=" << temp->value << ".htm>" << temp->value << "</A><BR>" << endl;
    //cout << "What?!" << endl;
    makeWord(dir,*temp);
    outfile.close();
  }
  //cout << "It's you!" << endl;
  endpages(dir);
} // end output

void makeIndex(string dir) {
  ofstream index(string(dir+"index.html").c_str());
  index << "<HTML><HEAD><TITLE>Ged's Page of Links!</TITLE></HEAD><BODY>"
  << "<P>Here Ged has compiled, for your conveinience, a list of the first letters"
  << " in the words you will be looking for.  Simply click the letter to be transported"
  << " to the page with all the words starting with that letter on it.</P>" << endl;
  for(char c = 'A'; c <= 'Z'; c++) {
    index << "<A href=" << c << ".html>" << c << "</A><BR>" << endl;
  } //rest of file
  index << "</BODY></HTML>" << endl;
  index.close();
}

void startpages(string dir) {
  for(char c ='A'; c <= 'Z'; c++) {
    ofstream temp(string(dir + c + ".html").c_str());
    temp << "<HTML><HEAD><TITLE>Things that start with the letter " << c << "</TITLE></HEAD><BODY>"
	 << "<P>This is the " << c << " page!  It contains links to all the pages that start with the letter "
	 << c << ".  Please have fun browsing the various words found in these pages.</P>" << endl;
    temp.close();
  }
}

void endpages(string dir) {
  for(char c = 'A'; c <= 'Z'; c++) {
    ofstream temp(string(dir + c + ".html").c_str(), ios::app); //append!
    temp << "</BODY></HTML>" << endl;
    temp.close();
  }
}

void makeWord(string dir, word w) {
  //cout << "MakeWord(" << dir << ", " << w.value << ")" << endl;
  ofstream output(string(dir + w.value + ".htm").c_str());
  output << "<HTML><HEAD><TITLE>Pages that contain " << w.value << "</TITLE></HEAD><BODY>"
  << "<P>This page contains links to pages that have the word " << w.value << "
in them. "
  << " Please enjoy your browsing experence.</P>" << endl;
  //now for each site make a line
  //cout << "sites:" << endl;
  for(site2* temp = w.nextSite(); temp != 0; temp = w.nextSite()) {
    //cout << temp->location << " " << temp->name << endl;
    output << "<A href=" << temp->location << ">" << temp->name << "</A><BR>" << endl;
  }
  output << "</BODY></HTML>" << endl;
  output.close();
}
