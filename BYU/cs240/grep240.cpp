#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int reader(istream &infile);
int usage(void);

bool ibool = false;
string pattern = "";

int main(int argc, char* argv[]) {
  if(argc > 2) { //if args (more than required)
    if(argv[1][0] == '-') { //if options
      if(argv[1][1] == 'i') //only one to check for
        ibool = true;
      pattern = argv[2];
      if(argc > 3) { //there is a file too
        ifstream temp(argv[3]);
        if(temp)
          return reader(temp);
        else {
          cerr << "Failure to open file" << endl;
          return 1;
        }
      }
      else // 1 arg, options
        return reader(cin);
    } //end is option -i
    else { //no options, 1 arg
      pattern = argv[1];
      ifstream temp(argv[2]);
      if(temp)
        return reader(temp);
      else {
        cerr << "Failure to open file" << endl;
        return 1;
      }
    }
  }
  else if(argc > 1) { //no args, read from command line
    pattern = argv[1];
    return reader(cin);
  }
  else
    return usage();
} //end main

int reader(istream &infile) { //main now works (30 mins)
  //use state machine, parse pattern
  int i = 0;
  string line;
  bool found = false;
  int max = pattern.length();
  ostringstream output;
  bool star = false;
  bool starf = false;
  bool star2 = false;
  char look;
  bool normal = false;
  int test = 1;
  string idata;
  while(!infile.eof()) {
    look = pattern[i++];
    getline(infile, line);
    int strlength = line.length();
    //cout << "Line: " << line << endl;
    for(int j = 0; j < strlength; j++) { //parse for each line?  whatever.
      if(look != '*' || normal == true) {
        if(!star) {
          if(look == '\\' && normal == false){
            look = pattern[i++]; // next char treated normal anyway
            normal = true;
            test = i;
          } // case insenitive if ibool is true
          //cout << "Compare1: " << line[j] << " " <<look << endl;
          if((ibool)?toupper(line[j]) == toupper(look):line[j] == look) {
           if(i >= max) {
              found = true;
              break;
            }
            else{
              look = pattern[i++];
              normal = false;
            }
          }//not ==
          else if(test < i) break;
        } // not star
        else { //star is true
          if(i >= max) {
            if(look != '\0') { // case insensitive if ibool is true
            if((ibool)?toupper(line[j]) == toupper(look):line[j] == look) {
              found = true; //only star, all is found
              break;
            }
           }
           else {
             found = true; //only is star...
             break;
           }
          } // case insensitive if ibool is true
          //cout << "Compare2: " << line[j] << " " <<look << endl;
          if(look == '\\') {
            look = pattern[i++];
            test = i;
            normal = true;
          }
          if((ibool)?toupper(line[j]) == toupper(look):line[j] == look) {
            star = false;
            look = pattern[i++];
            test = i;
            normal = false;
          }
        }
      }//end not *
      else {
        if(starf != true) {
          star = true;
          starf = true;
          look = pattern[i++];
        }
        else {
          star2 = true;
          break;
        }
      }
    }//for
    if(found) output << line << endl;
    if(star2 == true) {
      cerr << "may not have two *'s in pattern" << endl;
      return 3;
    }
    found = false;
    i = 0;
    normal = false;
    test = 1;
    star = false;
    star2 = false;
    starf = false;
  }//while
  cout << output.str() << endl;
  return 0;
}

int usage(void) {
  cerr << "USAGE: grep240 [options] pattern [file]" << endl;
  return 2;
}
