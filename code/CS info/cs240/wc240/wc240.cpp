#include <iostream>
#include <fstream>
#include <cctype>

void reader(istream &input);

int wordc = 0, linec = 1, bytec = -1; //for eof

int main(int argc, char* argv[]) {
  bool word = true, line = true, byte = true;

  if(argc > 1) { //if there is anything on the command line
    if(argv[1][0] == '-') { //if there are options
      word = false, line = false, byte = false;
      for(int i = 1; argv[1][i] != 0; i++) {
        switch(argv[1][i]) {
          case 'l':
            line = true;
            break;
          case 'w':
            word = true;
            break;
          case 'b':
            byte = true;
            break;
          default:
            cerr << "Incorrent use of command line options" << endl;
            cerr << "USAGE: wc240 [-lwb] [file]" << endl;
            return 1; //return inforamtive error
        }
      }// done with switches

      if(argc > 2) { //argv[2] is filename if exist
        ifstream temp(argv[2]);
        if(temp)
          reader(temp);
        else {
          cerr << "Failure to open file" << endl;
          return 2;
        }
      }// make file
    } //parse command line options
    else { //argv[1] is a filename
      ifstream temp(argv[1]);
      if(temp)
        reader(temp);
      else {
          cerr << "Failure to open file" << endl;
          return 2;
        }
    } //make file
  } //top if
  else { //nothing on command line
    reader(cin); //so simpol ;P.
  }//make file

  if(line)
    cout << linec << " lines ";
  if(word)
    cout << wordc << " words ";
  if(byte)
    cout << bytec << " bytes";
  cout << endl;
  //^^^ "So simple, I could even doing it myself"!
}//main

void reader(istream &infile) { //operate on file (should exist)
  char input;
  bool isWord = false;
  while(!infile.eof()) { //main loop
    input = infile.get(); //get a char
    bytec++; //increment numBytes
    if(!isWord && isalpha(input)) {
      isWord = true;
      wordc++;
    } //If char is letter after no word increment word count
    else if(input == '\n') { //If char is a '\n', increment lines
      linec++;
      isWord = false;
    } //  (turn off isWord too)
    else if(isspace(input)) {
      isWord = false;
    } //If char is whitespace after word (of any
      // sort) isWord is off
  }//while file has stuff in
}
