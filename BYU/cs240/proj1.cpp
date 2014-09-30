#include <fstream.h>
#include <string>
#include <iostream>
#include <cstring>
#include <cctype>
#include <stdlib.h>

int parse(const char*);
int math(int, char, int);

main(int numargs, char* args[])
{
  fstream infile(args[1], ios::in);
  fstream outfile("results.txt", ios::out);
  int result;

  while(!infile.eof())
  {
    cout << "New line" << endl;
    string line;
    getline(infile, line);
    const char *temp;
    temp = line.c_str();
    result = parse(temp);
    outfile << result << endl;
  }
}

int indexAt = 0;

int parse(const char* input)
{
  int result = 0;
  bool foundInt = false;
  int temp = 0;
  char op = ' ';
  int size = strlen(input);

  while(indexAt < size)
  {
    cout << input[indexAt] << endl;
    if(input[indexAt] == '(')
    {
      indexAt++;
      result = math(result, op, parse(input));
    }
    else if(input[indexAt] == ')')
    {
      indexAt++;
      return result;
    }
    else if((input[indexAt] == '+') || (input[indexAt] == '-') || (input[indexAt] =='*') || (input[indexAt] =='/'))
    {
      op = input[indexAt];
      indexAt++;
    }
    if(isdigit(input[indexAt]))
    {
      foundInt = true;
      temp *= 10;
      temp += atoi(&input[indexAt]);
      indexAt++;
    }
    else if(op == ' ' && foundInt == false && temp != 0 && result == 0)
    {
      result = temp;
      temp = 0;
    }
    else if(op != ' ' && foundInt == false && temp != 0)
    {
      result = math(result, op, temp);
      temp = 0;
      op = ' ';
    }
    else if(input[indexAt] == ' ')
    {
      foundInt = false;
      indexAt++;
    }//end if set


  }//end while
  return result;
}//end parse

int math(int first, char op, int second)
{
  if(op == '+')
    return first + second;
  else if(op == '-')
    return first - second;
  else if(op == '*')
    return first * second;
  else if(op == '/')
    return first / second;
  else
  {
    cout << "Bad math" << endl;
    return 0;
  }
}
