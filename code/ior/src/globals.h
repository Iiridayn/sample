#ifndef GLOBALS_H
#define GLOBALS_H

#define DEFAULT_PORT 31337
#define MAX_QUEUED_CLIENTS 10

#define NO_GENDER 0
#define MALE_GENDER 1
#define FEMALE_GENDER 2

typdef struct player_t {
	int socket;
	char* name;
	character* playing;
	struct player_t* next;
} player;

typedef struct character_t {
	char* name;
	long exp;
	char gender;
	room* in;
	struct character_t* next;
} character;

#endif

