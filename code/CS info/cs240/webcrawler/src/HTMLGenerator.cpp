#include "HTMLGenerator.h"
#include <fstream>
#include <iostream>
#include <cctype>

HTMLGenerator::HTMLGenerator(WordTree* inwords, string inout) {
  outdir = inout;
  if(outdir.find("/") == -1) outdir.append("/");
  words = inwords;
  ofstream outindex((outdir + "index.html").c_str());
  outindex << "<html><head><title>" << endl
	   << "Ged Crimsonclaw's page of Letters" << endl
	   << "</title></head><body>" << endl
	   << "Here Ged has compiled, for your enjoyment "
	   << "a list of the first letters of all the "
	   << "words you may wish to discover. <br>" << endl
	   << "Please enjoy browsing the various links "
	   << "found on these pages. <br><br>" << endl;
  Word* temp = words->pop();
  for(char i = 'a'; i <= 'z'; i++) {
    outindex << "<a href=" << i << ".html>" << (char)toupper(i)
	     << "</a><br>" << endl;
    ofstream outword((outdir + i + ".html").c_str());
    outword << "<html><head><title>" << endl
	    << "Ged Crimsonclaw's " << (char)toupper(i) << " page"
	    << endl << "</title></head><body>" << endl
	    << "Here Ged has compiled, for your enjoyment "
	    << "a list of all the words starting with the "
	    << "letter " << (char)toupper(i) << ".<br>" << endl
	    << "Please enjoy browsing the various " << endl
	    << "links found on these pages.<br><br>" <<endl;
    while(temp != 0 && temp->word[0] == i) {
      addWord(temp, outword);
      delete temp;
      temp = words->pop();
    }//while
    outword << "</body></html>" << endl;
  }//for
  outindex << "</body></html>" << endl;
}

void HTMLGenerator::addWord(Word* word, ostream &outword) {
  outword << "<a href=" << word->word << ".htm" << ">"
	  << word->word << "</a><br>" << endl;
  ofstream outpage((outdir + word->word + ".htm").c_str());
  outpage << "<html><head><title>" << endl
	  << "Ged Crimsonclaw's " << word->word << " page"
	  << endl << "</title></head><body>" << endl
	  << "Here Ged has compiled, for your enjoyment "
	  << "a list of all the webpages containing " <<endl
	  << "the word " << word->word << ".<br>" << endl
	  << "Please enjoy browsing the various " << endl
	  << "links found on these pages.<br><br>" << endl;
  Webpage* temp = word->pop();
  while(temp != 0) {
    addPage(temp, outpage);
    delete temp;
    temp = word->pop();
  }
  outpage << "</body></html>" << endl;
}

void HTMLGenerator::addPage(Webpage* page, ostream &outpage) {
  outpage << "<a href=" << page->url << ">"
	  << page->title << "</a><br>" << endl;
}
