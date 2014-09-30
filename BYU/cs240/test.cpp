#include <iostream>
#include <string>
#include <fstream>

int main()
{
  cout << "Type the text to echo: " << flush;
  string temp;
  cin >> temp;
  ofstream outfile("filename", ios::out);
  outfile << temp << endl;
  ifstream infile("filename", ios::in);
  getline(infile, temp);
  cout << temp << endl;
}
