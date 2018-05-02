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

OBJS = lcs.o

all: lcs

lcs: $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o lcs

handin: header.txt
	@test xbovik != x"$(ROUTEY)" \
		|| (echo Remember to set ROUTEY! && false)
	@test ! -e $(ROUTEY)-$(VERSION)-lcs.cc \
		|| (echo Remember to increase VERSION! && false)
	cp lcs.cc $(ROUTEY)-$(VERSION)-lcs.cc
	(cat header.txt; \
	 tar cjf - $(ROUTEY)-$(VERSION)-lcs.cc | \
	 uuencode handin/$(ROUTEY)-$(VERSION).tar.bz2) | \
	 $(SENDMAIL) $(ADDRESS)

header.txt:
	@echo Subject: lcs handin > header.txt
	@echo >> header.txt

###########################################################
# Use these rules to switch back and forth between versions
# of kernels.c. E.g., "make naive" installs the naive solution 
# that is handed out to students; "make solution" installs
# some optimized solutions.
###########################################################

clean: 
	-rm -f $(OBJS) lcs core *~ *.o

