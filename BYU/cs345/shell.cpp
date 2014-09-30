#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
using namespace std;

int main() {
	bool cont = true;

	// wrap all in big loop
	do {
		//print prompt
		cout << "$ " << flush; //need to flush it prolly

		//get input line
		string input;
		if(getline(cin, input).eof()) { //should work
			cout << "logout" << endl;
			exit(0); // one of the end conditions is eof
		}
		input += '\n';

		//parse out args, special chars, multiple exes, etc :P
		//loop on it until empty?
		char** args = 0;
		int argCount = 0;
		string accume; // temp accumulation string
		bool quote = false;
		bool ampersand = false;
		FILE *infile, *outfile;
		short instream = false, outstream = false;
		int pipeFiles[2];
		bool pipeOut = false, pipeIn = false;
		int pipeInFile = 0, pipeOutFile = 1;

		for(int i = 0; i < input.length(); i++) {
			switch(input[i]) {
				case '&':
					// what are the special rules for this?
					// will be last, but let \n handle self
					ampersand = true;
					cout << "ampersand on" << endl;
					break;
				case '\n':
					//cout << "we have now entered case '\\n'" << endl;

					if(argCount == 0) // no args, only thing on line
						if(accume == "exit" || accume == "logout")
							exit(0);

					if(instream == 1) {
						infile = fopen(accume.c_str(), "r");
						if(!infile)
							cout << "Failed to open input file: " << accume << endl;
						instream = -1;
						accume.erase();
					}
					else if(outstream == 1) {
						outfile = fopen(accume.c_str(), "w");
						if(!outfile)
							cout << "Failed to open output file: " << accume << endl;
						outstream = -1;
						accume.erase();
					}
					else if(!accume.empty()) {
					// I have the last arg in accume, need to insert into array
					{
					argCount++;
					char** tmp = (char **) malloc((argCount+1)*sizeof(char));
					for(int j = 0; j < argCount-1; j++)
						tmp[j] = args[j];
					tmp[argCount-1] = (char *) malloc(accume.length()*sizeof(char));
					for(int j = 0; j < accume.length(); j++) {
						tmp[argCount-1][j] = accume[j];
					}
					accume.erase();
					tmp[argCount] = 0;
					free(args);
					args = tmp;
					}
					//cout << "done making arg, now have " << (argCount-1) << " args" << endl;
					//copied from below
					}

					if(argCount == 0) break;

					//done with all line, execute current command
					if(fork() == 0) { //child
						//execv(char* path, char* argv[]); or
						/*
						cout << "Trying to execute: " << args[0] << " with args:" << endl;
						for(int j = 1; j < argCount; j++)
								cout << args[j] << endl;
						*/
						if(instream) {
							close(0);
							dup2(fileno(infile), 0);
							close(fileno(infile));
						}
						if(outstream) {
							close(1);
							dup2(fileno(outfile), 1);
							close(fileno(outfile));
						}
						if(pipeOut) {
							close(1);
							dup2(pipeOutFile, 1);
							close(pipeOutFile);
						}
						if(pipeIn) {
							close(0);
							dup2(pipeInFile, 0);
							close(pipeInFile);
						}

						execvp(args[0], args); //will search as shell
						cout << "Program not found (or other error)." << endl;
						exit(0); // is the child, so should exit
						// argv[0] = filename, argv[i] = 0;
						//if it returns, is bad
					}
					else { //parent
						if(instream)
							close(fileno(infile));
						instream = false;
						if(outstream)
							close(fileno(outfile));
						outstream = false;
						if(pipeIn)
							close(pipeInFile);
						pipeIn = false;
						if(pipeOut) {
							close(pipeOutFile);
							pipeIn = true;
							pipeInFile = pipeFiles[0];
						}
						pipeOut = false;

						if(!ampersand) //should have this at the bottom
							wait(0); // block, waiting for child to exit
					}
					break;
				case '<': //input
					if(instream != -1) instream = true;
					break;
				case '>': //output
					if(outstream != -1)	outstream = true;
					break;
				case '|':
					//we have a complete specification if one of these is alone
					//so call the process and reset the lot.  Handle the special
					//junk for each too
					pipeOut = true;
					pipe(pipeFiles);
					pipeOutFile = pipeFiles[1];

					//now a copy of \n's code :P
					if(instream == 1) {
						infile = fopen(accume.c_str(), "r");
						if(!infile)
							cout << "Failed to open input file: " << accume << endl;
						instream = -1;
						accume.erase();
					}
					else if(outstream == 1) {
						outfile = fopen(accume.c_str(), "w");
						if(!outfile)
							cout << "Failed to open output file: " << accume << endl;
						outstream = -1;
						accume.erase();
					}
					else if(!accume.empty()) {
					// I have the last arg in accume, need to insert into array
						{
							argCount++;
							char** tmp = (char **) malloc((argCount+1)*sizeof(char));
							for(int j = 0; j < argCount-1; j++)
								tmp[j] = args[j];
							tmp[argCount-1] = (char *) malloc(accume.length()*sizeof(char));
							for(int j = 0; j < accume.length(); j++) {
									tmp[argCount-1][j] = accume[j];
							}
							accume.erase();
							tmp[argCount] = 0;
							free(args);
							args = tmp;
						}
						//cout << "done making arg, now have " << (argCount-1) << " args" << endl;
						//copied from below
					}

					if(argCount == 0) break;

					//done with all line, execute current command
					if(fork() == 0) { //child
						//execv(char* path, char* argv[]); or
						/*
						cout << "Trying to execute: " << args[0] << " with args:" << endl;
						for(int j = 1; j < argCount; j++)
								cout << args[j] << endl;
						*/
						if(instream) {
							close(0);
							dup2(fileno(infile), 0);
							close(fileno(infile));
						}
						if(outstream) {
							close(1);
							dup2(fileno(outfile), 1);
							close(fileno(outfile));
						}
						if(pipeOut) {
							close(1);
							dup2(pipeOutFile, 1);
							close(pipeOutFile);
						}
						if(pipeIn) {
							close(0);
							dup2(pipeInFile, 0);
							close(pipeInFile);
						}

						execvp(args[0], args); //will search as shell
						cout << "Program not found (or other error)." << endl;
						exit(0); // is the child, so should exit
						// argv[0] = filename, argv[i] = 0;
						//if it returns, is bad
					}
					else { //parent
						if(instream)
							close(fileno(infile));
						instream = false;
						if(outstream)
							close(fileno(outfile));
						outstream = false;
						if(pipeIn)
							close(pipeInFile);
						pipeIn = false;
						if(pipeOut) {
							close(pipeOutFile);
							pipeIn = true;
							pipeInFile = pipeFiles[0];
						}
						pipeOut = false;
						
						for(int j = 0; j < argCount; j++)
							free(args[j]);
						free(args);
						args = 0;
						argCount = 0;

						if(!ampersand) //should have this at the bottom
							wait(0); // block, waiting for child to exit
					}
					break;
				case '\"':
				case '\'':
				//currently no distingushing between the types, may need to fix
					quote = !quote;
					//cout << "quote toggled" << endl;
					break;
				case ' ':
				case '\t':
					//cout << "entered whitespace section" << endl;
					if(quote) {
     						accume += input[i];
     						break;
					}
					//cout << "not a quote" << endl;
					if(accume.empty()) break; // strip the whitespace
					//cout << "not stripping whitespace" << endl;

					if(instream == 1) {
						infile = fopen(accume.c_str(), "r");
						if(!infile)
							cout << "Failed to open input file: " << accume << endl;
						instream = -1;
						accume.erase();
						break;
					}
					if(outstream == 1) {
						outfile = fopen(accume.c_str(), "w");
						if(!outfile)
							cout << "Failed to open output file: " << accume << endl;
						outstream = -1;
						accume.erase();
						break;
					}

					{
					argCount++;
					char** tmp = (char **) malloc((argCount+1)*sizeof(char));
					for(int j = 0; j < argCount-1; j++)
						tmp[j] = args[j];
					tmp[argCount-1] = (char *) malloc(accume.length()*sizeof(char));
					for(int j = 0; j < accume.length(); j++) {
						tmp[argCount-1][j] = accume[j];
					}
					accume.erase();
					tmp[argCount] = 0;
					free(args);
					args = tmp;
					}
					//cout << "done making arg, now have " << (argCount-1) << " args" << endl;
					//i++;
					break;
				default:
					//cout << "default section" << endl;
					accume += input[i];
     				break;
			}
		}

		// special chars: &, <, >, |
		// \n needs to be okay, logout, exit, and ^d all quit

		/*

		*/

		//every command, try to reap children left
		int waitres, progstat;
		while((waitres = waitpid(-1, &progstat, WNOHANG)) != 0 && waitres != -1)
			cout << "Process " << waitres << " exited " << ((WIFEXITED(progstat))?"":"un") << "successfully." << endl;
	} while(cont);
}
