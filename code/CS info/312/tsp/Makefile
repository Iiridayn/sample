####################################################
# Students' Makefile for the CS:APP Performance Lab
####################################################

# Change ROUTEY to be your ROUTE-Y id.
ROUTEY = mc467
VERSION = 1

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

OBJS = tsp.o

all: tsp

tsp: $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o tsp

handin: header.txt
	@test xbovik != x"$(ROUTEY)" \
		|| (echo Remember to set ROUTEY! && false)
	@test ! -e $(ROUTEY)-$(VERSION)-tsp.cc \
		|| (echo Remember to increase VERSION! && false)
	cp tsp.cc $(ROUTEY)-$(VERSION)-tsp.cc
	(cat header.txt; \
	 tar cjf - $(ROUTEY)-$(VERSION)-tsp.cc | \
	 uuencode handin/$(ROUTEY)-$(VERSION).tar.bz2) | \
	 $(SENDMAIL) $(ADDRESS)

header.txt:
	@echo Subject: tsp handin > header.txt
	@echo >> header.txt

###########################################################
# Use these rules to switch back and forth between versions
# of kernels.c. E.g., "make naive" installs the naive solution 
# that is handed out to students; "make solution" installs
# some optimized solutions.
###########################################################

clean: 
	-rm -f $(OBJS) tsp core *~ *.o
