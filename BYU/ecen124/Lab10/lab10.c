/*
 * Lab10.c by Michael Clark, created on 11-19-2003
 * TA: Scott	RYid: mc467
 * 
 * Plays the game of Nim with the user.  Fun fun fun.
 */

#include <stdio.h>
#include <stdlib.h> 
#define MAX_STIX 30

int getNum(char *, int);
int playNim(int, int);
int compTurn(int);

int main() {
	srand(time(NULL)); //set random number generator
	int cont = 0;
	do {
		printf("***********************\n");
		printf("*** The Game of Nim ***\n");
		printf("***********************\n\n");

		int players = getNum("One or Two player mode (%d = One, 2 = Two): ",1);
		while((players != 1)&&(players != 2)) {
			printf("That is not a valid number of players.  Please enter a valid number.\n");
			players = getNum("One or Two player mode (%d = One, 2 = Two): ",1);
		}
		printf("\n");
		
		int sticks = getNum("Enter sticks in the pile (5-%d): ", MAX_STIX);
		while((sticks < 5)||(sticks > MAX_STIX)) {
			printf("That is not a valid number of sticks.  Please enter a valid number.\n");
			sticks = getNum("Enter sticks in the pile (5-%d): ", MAX_STIX);
		}
		printf("\n");
		
		int looser = playNim(players, sticks);
		printf("\nPlayer %d lost by picking up the last stick!\n", looser+1);
		
		int contloop = 1;
		while(contloop) {
			printf("\nWould you like to play again? (y/n): ");
			getchar(); //why do I need to do this to make it work?
			char ans = getchar();
			switch(ans) {
				case 'Y':
				case 'y':
					cont = 1;
					contloop = 0;
					break;
				case 'N':
				case 'n':
					cont = 0;
					contloop = 0;
					break;
				default:
					while(getchar() != '\n'); //clear buffer
					printf("Invalid response.  Please enter a 'y' or an 'n'.\n");
					contloop = 1;
			}
		}
		if(cont) printf("\n\n\n");
	} while(cont);
}

int getNum(char *prompt, int extra) {
	int temp;
	printf(prompt, extra);
	while (!scanf("%d",&temp))
	{
		printf("Not a number!  Please enter a valid number.\n");
    while(getchar() != '\n');
		printf(prompt, extra);
	}
	return temp;
}

int playNim(int players, int sticks) {
	int turn = rand()%2; //player 1 is always user let's say
	while(sticks > 0) {
		turn = (++turn)%2; //can do here, as is random value anyway, preserves looser
		printf("**************************\n");
		printf("**  Sticks in pile: %2d ***\n", sticks);
		printf("**************************\n");
		
		int grab;
		if((players == 2)||(turn==0)) {
			grab = getNum("\nEnter sticks to pick up (1, 2, or 3)?\nPlayer %d: ", turn+1);
			while( ((grab < 1)||(grab > 3)) || (grab>sticks)) {
				printf("That is not a valid number of sticks.  Please enter a valid number.\n");
				grab = getNum("\nEnter sticks to pick up (1, 2, or 3)?\nPlayer %d: ", turn+1);
			}
			printf("\n");
		}
		else { //comp's turn
			grab = compTurn(sticks);
			printf("Enter sticks to pick up (1, 2, or 3)?\nComputer: %d\n\n", grab);
		}
		sticks -= grab;
	}
	return turn;
}

int compTurn(int sticks) {
	sticks %= 4;
	switch(sticks) {
		case 0:
			return 3;
		case 1:
			return (1+rand()%3); //prolly loose
		default:
			return (sticks - 1);
	}
}

