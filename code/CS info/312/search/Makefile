####################################################
# Students' Makefile for the CS:APP Performance Lab
####################################################

# Change ROUTEY to be your ROUTE-Y id.
ROUTEY = mc467
VERSION = 2

##################################################
# You shouldn't need to modify anything below here
##################################################

# Submission information.
SENDMAIL = /usr/sbin/sendmail -bm
ADDRESS = cs312ta@cs.byu.edu

CC = g++
CFLAGS = -Wall -O2 -g
CPPFLAGS = $(CFLAGS)
LIBS = -lm

OBJS = n-ary-search.o search.o clock.o fcyc.o

all: n-ary-search

n-ary-search: $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o n-ary-search

handin: header.txt
	@test xbovik != x"$(ROUTEY)" \
		|| (echo Remember to set ROUTEY! && false)
	@test ! -e $(ROUTEY)-$(VERSION)-search.cc \
		|| (echo Remember to increase VERSION! && false)
	cp search.cc $(ROUTEY)-$(VERSION)-search.cc
	(cat header.txt; \
	 tar cjf - $(ROUTEY)-$(VERSION)-search.cc | \
	 uuencode handin/$(ROUTEY)-$(VERSION).tar.bz2) | \
	 $(SENDMAIL) $(ADDRESS)

header.txt:
	@echo Subject: n-ary-search handin > header.txt
	@echo >> header.txt

###########################################################
# Use these rules to switch back and forth between versions
# of kernels.c. E.g., "make naive" installs the naive solution 
# that is handed out to students; "make solution" installs
# some optimized solutions.
###########################################################

# Naive baseline version handed out to students
naive:
	rm -f search.{cc,o}; 
	ln -s search-naive.cc search.cc

# Example solutions
solution:
	rm -f search.{cc,o}; 
	ln -s search-solution.cc search.cc

clean: 
	-rm -f $(OBJS) n-ary-search core *~ *.o
