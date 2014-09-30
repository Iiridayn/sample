#include <stdio.h>

typedef struct board_t {
	int ttt[3][3];
	struct board_t* next;
} board;

int compareBoard(board* a, board* b);
void genList(board* head);
void rotateBoard(board* toRot);
int countBoards(board* head);
board* copyBoard(board* copy);

int main() {
	board* head;
	board *loop,*loop2,*temp,*prev;
	int i;
	genList(head);
	for(loop = head;loop;loop = loop->next) {
		temp = copyBoard(loop);
		for(i=1;i<4;i++) { //run three times; 90,180,270
			prev = 0;
			rotateBoard(temp);
			for(loop2=head;loop2;prev=loop2,loop2=loop2->next) {
				if(compareBoard(loop2,temp)) {
					if(compareBoard(loop,loop2)) break;
					//should never be deleting head (we hope)
					//or loop itself (same as deleting head, really)
					//hope this will work here
					prev->next = loop2->next;
					free(loop2);
					break; //only one will be found
				}//if
			}//for loop2
		}//for i
		free(temp);
	}//for loop
	printf("There are %d unique games of tic-tac-toe",countBoards(head));
}

//compares two boards, returns non-zero if they are the same
int compareBoard(board* a, board* b) {
	int i,j;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			if(a->ttt[i][j] != b->ttt[i][j])
				return 0;
	return 1;
}

//generate the board list
//not nearly done ;P
//all but this compiles, worry later.
void genList(board* head) {
	/*
	int i,j,k;
	
	for(i=0;i<3;i++) //for each box
		for(j=0;j<3;j++)
			for(k=0;k<3;k++)
				//ttt[i][j] = k;
				break;
	
	head = malloc(sizeof(board));
	head->ttt = {{0,0,0},{0,0,0},{0,0,0}}; //null board
	*/
	return;
}

//rotate a board 90 degrees
void rotateBoard(board* toRot) {
	int i,j;
	int internal[3][3];
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			internal[i][j] = toRot->ttt[i][j];
	
	/* Possible new way to do rotate
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			toRot->ttt[i][j] = internal[2-j][i];
	*/
	
	//old top row
	toRot->ttt[2][2] = internal[0][2];
	toRot->ttt[2][1] = internal[1][2];
	toRot->ttt[2][0] = internal[2][2];
	
	//old middle row
	toRot->ttt[1][2] = internal[0][1];
	toRot->ttt[1][1] = internal[1][1];
	toRot->ttt[1][0] = internal[2][1];
	
	//old bottom row
	toRot->ttt[0][2] = internal[0][0];
	toRot->ttt[0][1] = internal[1][0];
	toRot->ttt[0][0] = internal[2][0];
}

//count the number of boards in the linked list
int countBoards(board* head) {
	int count = 0;
	board* temp;
	for(temp=head;temp;temp=temp->next)
		count++;
	return count;
}

//returns a board w/the ttt array the same
board* copyBoard(board* copy) {
	board* output = malloc(sizeof(board));
	int i,j;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			output->ttt[i][j] = copy->ttt[i][j];
	output->next = 0;
	return output;
}

