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

OBJS = desert.o compute_food_units.o clock.o fcyc.o

all: desert

desert: $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o desert

handin: header.txt
	@test xbovik != x"$(ROUTEY)" \
		|| (echo Remember to set ROUTEY! && false)
	@test ! -e $(ROUTEY)-$(VERSION)-compute_food_units.c \
		|| (echo Remember to increase VERSION! && false)
	cp compute_food_units.cc $(ROUTEY)-$(VERSION)-compute_food_units.cc
	(cat header.txt; \
	 tar cjf - $(ROUTEY)-$(VERSION)-compute_food_units.cc | \
	 uuencode handin/$(ROUTEY)-$(VERSION).tar.bz2) | \
	 $(SENDMAIL) $(ADDRESS)

header.txt:
	@echo Subject: desert handin > header.txt
	@echo >> header.txt

###########################################################
# Use these rules to switch back and forth between versions
# of kernels.c. E.g., "make naive" installs the naive solution 
# that is handed out to students; "make solution" installs
# some optimized solutions.
###########################################################

# Naive baseline version handed out to students
naive:
	rm -f compute_food_units.{cc,o}; 
	ln -s compute_food_units-naive.cc compute_food_units.cc

# Example solutions
solution:
	rm -f compute_food_units.{cc,o}; 
	ln -s compute_food_units-solution.cc compute_food_units.cc

clean: 
	-rm -f $(OBJS) desert core *~ *.o
