#include <iostream>
#include <fstream>
#include <string.h>
#include <ctype.h>

using namespace::std;

void grep(istream &infile, char* pattern);
void usage();
bool checkpattern(const char * pattern);
void stringcat(char* &total, char* add, bool );
void readline(istream &infile, char* &dest);
bool cmp(char one, char two);

bool I = false;

int main(int argc, char* argv[]) {
  if(argc < 2) {
    usage();
    return 1;
  }
  else if(argc == 2) //have pattern
    grep(cin, argv[1]);
  else if(argc == 3) { //options or filename
    if(argv[1][0] == '-') { //options
      if(argv[1][1] == 'i') {
	I = true;
	grep(cin, argv[2]);
      }
      else
	usage();
    }
    else { //must be filename
      ifstream infile(argv[2]);
      if(infile)
	grep(infile, argv[1]);
      else {
	cerr << "Failure to open file." << endl;
	return 2;
      }
    }
  } //end else if 3
  else if(argc == 4) { //have both
    if(argv[1][0] == '-' && argv[1][1] == 'i')
      I = true;
    else {
      usage();
      return 1;
    }
    
    ifstream infile(argv[3]);
    if(infile)
      grep(infile, argv[2]);
    else {
      usage();
      return 1;
    }
  }
  else {
    usage();
    return 1;
  }
}

void grep(istream &infile, char* pattern) {
  if(!checkpattern(pattern)) return;
  char* all = "";
  while(!infile.eof()) {
    char* line = "";
    readline(infile, line);
    //cout << line << endl;
    //cout << pattern << endl;
    int length = strlen(line);
    int pos = 0;
    int patlen = strlen(pattern) - 1;
    bool found = false;
    int firstMatch = -1;
    for(int i = 0; i < length && found != true; i++) {
      if(pattern[pos] == '*') {
	if(patlen == pos) // is length one longer?  guess: yes
	  found = true;
	else if(pattern[pos+1] == '\\')
	  pos++;
	else {
	  pos++;
	  while(i < length && !cmp(line[i], pattern[pos])) { //char comp
	    i++;
	  }
	  if((pos == patlen)&&i!=length)
	    found = true;
	}
      } //main * if
      else if(pattern[pos] == '\\') {
	if(pos == patlen){
	  cerr << "Bad pattern." << endl;
	  return;
	}
	pos++;
	if(!cmp(line[i], pattern[pos]) && pos != 1) {
	  i = firstMatch + 1;
	  firstMatch = -1;
	  pos = 0;
	}
	else if(cmp(line[i], pattern[pos])) {
	  if(pos==patlen) found = true;
	  if(firstMatch == -1) firstMatch = i;
	  pos++;
	}
	else if(pos == 1) {
	  while(i < length && !cmp(line[i], pattern[pos])) {
	    i++;
	  }
	  if((pos == patlen)&&i!=length) found = true;
	  if(i!=length&&firstMatch == -1) firstMatch = i;
	  pos++;
	}
      }
      else if(!cmp(pattern[pos], line[i])) {
	if(pos != 0) {
	  i = firstMatch + 1;
	  firstMatch = -1;
	}
	pos = 0;
      }
      else { //are ==
	if(firstMatch != -1) firstMatch = i;
	if(pos == patlen) found = true;
	else pos++;
      }
    } //main line loop
    //cout << "Found: " << found << endl;
    if(found)
      stringcat(all, line, false);
  } //main loop
  //cout << endl;
  cout << all << endl;
  //delete all;
}

void usage() {
  cout << "USAGE: grep240 [options] pattern [file]" << endl;
}

bool checkpattern(const char * pattern) {
  int length = strlen(pattern);
  bool one = false;
  for(int i = 0; i < length; i++) {
    if(pattern[i] == '\\')
      i++;
    else if(pattern[i] == '*') {
      if(one)
	return false;
      else
	one = true;
    }
  }
}

void stringcat(char* &total, char* add, bool nl = true) {
  int a = strlen(total);
  int b = strlen(add);
  //cout << "Total: " << total << endl;
  //cout << "Add: " << add << endl;
  char* newtotal = new char[a + b + (nl?2:1)];
  strcpy(newtotal, total);
  if(nl) strcat(newtotal, "\n");
  strcat(newtotal, add);
  total = newtotal;
  //cout << total << endl;
}

void readline(istream &infile, char* &dest) {
  for(char temp = infile.get(); !infile.eof() && temp != '\n'; temp = infile.get()) {
    char tempstr[2];
    tempstr[0] = temp;
    tempstr[1] = '\0';
    //cout << temp << endl;
    stringcat(dest, tempstr, false);
  } //main loop
  char tempstr[2];
  tempstr[0] = '\n';
  tempstr[1] = '\0';
  stringcat(dest, tempstr, false);
}

bool cmp(char one, char two) {
  if(I)
    return (toupper(one)==toupper(two));
  else
    return (one == two);
}
