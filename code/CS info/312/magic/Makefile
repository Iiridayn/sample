####################################################
# Students' Makefile 
####################################################

# Change ROUTEY to be your ROUTE-Y id.
ROUTEY=mc467
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

OBJS = msquare.o

all: msquare

msquare: $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o msquare

handin: header.txt
	@test xbovik != x"$(ROUTEY)" \
		|| (echo Remember to set ROUTEY! && false)
	@test ! -e $(ROUTEY)-$(VERSION)-msquare.cc \
		|| (echo Remember to increase VERSION! && false)
	cp msquare.cc $(ROUTEY)-$(VERSION)-msquare.cc
	(cat header.txt; \
	 tar cjf - $(ROUTEY)-$(VERSION)-msquare.cc | \
	 uuencode handin/$(ROUTEY)-$(VERSION).tar.bz2) | \
	 $(SENDMAIL) $(ADDRESS)

header.txt:
	@echo Subject: msquare handin > header.txt
	@echo >> header.txt

###########################################################
# Use these rules to switch back and forth between versions
# of kernels.c. E.g., "make naive" installs the naive solution 
# that is handed out to students; "make solution" installs
# some optimized solutions.
###########################################################

clean: 
	-rm -f $(OBJS) msquare core *~ *.o
