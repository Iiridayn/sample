#include <iostream>
#include <string>
#include <fstream.h>

int main(int size, char* args[])
{
  fstream infile(args[1], ios::in);
  while(!infile.eof())
  {
    string stringname;
    getline(infile, stringname);
    cout << stringname << endl;
  }
}
