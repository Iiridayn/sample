#include <iostream> //normal input
#include <string> //always include, is fun ;P

#include <fstream> //files, IO?

int main()
{
  cout << "Hello World!" << endl;
  //cin
  //or 'flush'
  //ofstream, ifstream(can do filename),fstream infile("filename", ios::(in,out - overwrite,app - append))
  //infile >> varname; -- one word

  char arr[64]; // standard line length
  ifstream infile("filename",ios::in);
  infile.getLine(arr, 64); //get 64 chars, stops @ end of line
  
  return 0; //not really needed though
}
