# A simple makefile, by Ged Crimsonclaw.

# Will run this if make is run with no args
# Complies the main executable, and all of its dependancies
bin: lab7.o	
	 gcc -o lab7 lab7.o

# I like this idea...  Very cool.  Didn't know vars were supported
# For each .o file as a dependancy, this will make it from its .c file
%.o:    %.c
	gcc -c $<

# Clean everything.  Deletes unneccisary files.
clean:  
	rm -f *.o lab7 core