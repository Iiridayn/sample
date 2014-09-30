/*
Need to flush buffers, but don't know teacher's way...
*/
#include <stdio.h>
#include <stdlib.h>
#define MAXSTICKS 30
#define MINSTICKS 5

int turn;

main()
{
  int players, sticks, pickUp, end;

  srand(time(NULL)); /* Needed, apparently */

  PrintBreak(); /* The intro Screen */
  printf("***  The Game of Nim  ***\n");
  PrintBreak();

  int yes = 0;
  do
  {
    printf("\nOne or two players? ");
    if(scanf("%d", &players) || RangeChecker(2, 1, players))
    {
      yes = 0;
      printf("Please enter a valid number\n");
    }
  }
  while(yes == 0);

  do
  {
    printf("\nPlease enter the desired number of sticks (5 - 30): ");
    if(scanf("%d", &sticks) || RangeChecker(MAXSTICKS, MINSTICKS, sticks))
    {
      yes = 0;
      printf("Please enter a valid number\n");
    }
  }
  while(yes == 0);

  if (players == 2)
  {
    turn = rand()%2;

    do
    {
      do /* prompt, checking */
      {
        PrintSticks(sticks);
        printf("Player %d, input a number between 1 and 3 inclusive", turn + 1);
        if(scanf("%d", &pickUp) || RangeChecker(3, 1, pickUp) || pickUp > sticks)
        {
          yes = 0;
          printf("Please enter a valid number\n");
        }
      }
      while(yes == 0);

      sticks -= pickUp;
      if(sticks == 0)
      {
        end = turn + 1;
      }
      else
        nextTurn();
    }
    while(end == 0);

    if(end == 1)
      printf("Player 1 loses.\n");
    else
      printf("Player 2 loses.\n");
  }
  else
  {
    turn = rand()%2;

    do
    {
      do /* prompt, checking */
      {
        PrintSticks(sticks);
        printf("Player %d, input a number between 1 and 3 inclusive", turn + 1);

        if(turn == 0) /*Player's turn*/
          if(scanf("%d", &pickUp) || RangeChecker(3, 1, pickUp) || pickUp > sticks)
          {
            yes = 0;
            printf("Please enter a valid number\n");
          }
        else
          pickUp = GetComputerNumber(sticks);
      }
      while(yes == 0);

      sticks -= pickUp;
      if(sticks == 0)
        end = turn + 1;
      else
        nextTurn();
    }
    while(end == 0);

    if(end == 1)
      printf("Player 1 loses.\n");
    else
      printf("Player 2 loses.\n");
  }

}

void PrintBreak () /* Prints a spacer */
{
  printf("**************************\n");
}

void PrintSticks(sticks) /* Prints the number of sticks */
{
  PrintBreak();
  printf("***        Sticks in pile:  %d        ***\n", sticks);
  PrintBreak();
}

int RangeChecker(max, min, num)
{
  if ((num > max) || (num < min))
    return 0;
  else
    return 1;
}

void nextTurn()
{
  turn = (++turn)%2;
}

int GetComputerNumber(sticks)
{
  int temp;
  int pickup;
  temp = sticks % 4;

  if (temp == 0)
  {
    pickup = 3;
  }
  else if (temp == 1)
  {
    pickup = 1 + rand() % 3;
  }
  else
  {
    pickup = temp - 1;
  }

  return pickup;
}
