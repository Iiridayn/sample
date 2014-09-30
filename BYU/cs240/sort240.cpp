#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>

int reader(istream &input);
bool setBool(char option[]);

class stringNumComp {
	public:
  stringNumComp(bool);
  bool operator()(const string &,const string &);
};

bool rbool = false, ibool = false, nbool = false; //all on defaults
int field;

int main(int argc, char* argv[]){ //parse command line
  if(argc > 1) { //if anything other than name on line

    int argl; //keep track of so know where we are

    for(argl = 1; argl < argc && argv[argl][0] == '-'; argl++) { //get options
      if(!setBool(argv[argl])) {
        cerr << "OPTIONS: -[rin]" << endl;
        return 2;
      }
    } //done w/options

    if(argl < argc) { //there is something after options

      if(!isdigit(argv[argl][0])) { //note: can only take one digit!
        cerr << "No field number to sort!" << endl;
        return 1;
      } //another error; implicit else :P
      field = atoi(argv[argl]);
      //now look if there is a file

      argl++; //the array location after the last operated on
      if(argl < argc) { //there is a file
        ifstream temp(argv[argl]);
        if(temp)
          return reader(temp);
        else {
          cerr << "Failure to open file" << endl;
          return 3;
        }
      }
      else { //there is no spoon
        return reader(cin);  //let them have cin
      }
    }
    else { //see below
      cerr << "No field number to sort!" << endl;
      return 1;
    }
  }
  cerr << "No field number to sort!" << endl;
  return 1;
}

bool setBool(char option[]) {
  for(int i = 1; option[i] != 0; i++) { //should start w/'-'.
    switch(option[i]) { //does this for each char
      case 'r':
        rbool = true;
        break;
      case 'i':
        ibool = true;
        break;
      case 'n':
        nbool = true;
        break;
      default: return false;
    }
  }// no bad options in block, all set
  return true;
}

int reader(istream &infile) { //operate on file (should exist)
  string buffer, temp;
  int rows = 1, cols = 0;

  //first line we'll count the number of colums
  //how do I know I am @ end of line?
  getline(infile, temp);
  istringstream temp2(temp);

  while(temp2.good()) { //temp has stuff
    temp2 >> temp;
    buffer += temp + " ";
    cols++;
  }

  //if cols is less than field, return 4, error
  if(cols < field) {
    cerr << "Specified field does not exist!" << endl;
    return 4;
  }

  while(!infile.eof()) { //read the rest
    getline(infile, temp);
    buffer += '\n' + temp;
    rows++;
  }

  //okay...  how?  last big question I think.
  string** file = new string*[rows]; //make an array rows*colums
  for(int i = 0; i < rows; i++) {
    file[i] = new string[cols];
  }

  istringstream temp3(buffer);
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      temp3 >> file[i][j];
    }
  }

  //how propogate change?  Can take my [][]?

  //now do the sort
  if(nbool) {
    stringNumComp compar(rbool);
    sort(file, file+rows, compar);
  }
  else if(ibool);
    //file = sort(file, file+rows, stringCompCase(rbool));
  else;
    //file = sort(file, file+rows, stringComp(rbool));

  //output
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      cout << file[i][j] << '\t';
    }
    cout << endl;
  }
 //is all bogus
  return 0;
}

class stringComp {
  bool reverse;
  public:
  stringComp(bool temp) {
    reverse = temp;
  }

  bool operator()(string a, string b) {
    int i;
    for(i = 0; a[i] != 0 && b[i] != 0; i++) {
      if(!reverse) {
        if(a[i] < b[i]) return true;
        else if(a[i] > b[i]) return false;
      }
      else {
      if(a[i] > b[i]) return true;
      else if(a[i] < b[i]) return false;
      }
    }//for
    if(!reverse) {
      if(a[i] != 0) return true;
      else if(b[i] != 0) return false;
    }
    else {
      if(a[i] != 0) return false;
      else if(b[i] != 0) return true;
    }
  } // operator()
}; // stringComp

class stringCompCase {
  bool reverse;
  public:
  stringCompCase(bool temp) {
    reverse = temp;
  }

  bool operator()(string a, string b) {
    int i;
    string atemp;
    string btemp;

    for(int j = 0; a[j] != 0; j++) { //throwing a to upper
      atemp += toupper(a[j]);
    }
    for(int j = 0; a[j] != 0; j++) { //throwing b to upper
      btemp += toupper(b[j]);
    }

    for(i = 0; atemp[i] != 0 && btemp[i] != 0; i++) {
      if(!reverse) {
        if(atemp[i] < btemp[i]) return true;
        else if(atemp[i] > btemp[i]) return false;
      }
      else {
      if(atemp[i] > btemp[i]) return true;
      else if(atemp[i] < btemp[i]) return false;
      }
    }//for
    if(!reverse) {
      if(a[i] != 0) return true;
      else if(b[i] != 0) return false;
    }
    else {
      if(a[i] != 0) return false;
      else if(b[i] != 0) return true;
    }
  } // operator()
}; // stringCompCase

class stringNumComp { //prolly too much
  bool reverse;
  public:
  stringNumComp(bool temp) {
    reverse = temp;
  }

  bool operator()(string * a, string * b) {
    char* tempaa;
    strcpy(tempaa, a.c_str());
    string tempa = strtok(tempaa, " ");
    for(int i = 2; i < field; i++)
      tempa = strtok(NULL, " ");

    char* tempbb;
    strcpy(tempbb, b.c_str());
    string tempb = strtok(tempbb, " ");
    for(int i = 2; i < field; i++)
      tempb = strtok(NULL, " ");

    float atemp = atof(tempa.c_str());
    float btemp = atof(tempb.c_str());
    if(!reverse)
      return (atemp < btemp);
    else
      return (atemp > btemp);
  } // operator()
}; // stringNumComp
