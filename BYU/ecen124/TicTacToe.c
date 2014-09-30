#include <stdio.h>

PrintX(int square);
PrintO(int square);
PrintBoard();
nextTurn(int turn);
WinChecker();

char[36][36] board;
int[3][3] small;

main()
{
    srand(time(NULL)); /* Needed for random nums */

    play =  1;

    do
        {
        for(int y = 0; y < 36; y++)
        {

            for(int x = 0; x < 36; x++)
            {
                if ((y%11 == 0)&&(x%11 == 0)&&(x != 0)&&(y != 0))
                    board[x][y] = '+';
                else if((y%11 == 0)&&(y != 0))
                    board[x][y] = '-';
                else if ((x%11 == 0)&&(x != 0))
                    board[x][y] = '|';
            }
        }

        int turn = rand()%2;
        int square;
        int wins = 0;

        do
            {
            PrintBoard();
            printf("Player %n, please use the number pad to select where to go.\n", (turn+1));
            do{
                scanf("%d", &square);
                if(square > 9 || square < 1)
                {
                    int go = 0;
                    printf("\nIllegal value.  Try again please.\n");
                }
                else
                    int go = 1;
            }
            while(go == 0);

            if(turn == 0)
                PrintX(square);
            else
                PrintO(square);

            wins = WinChecker();
        }
        while(wins != 0)

        }
        while(play == 1);
}

PrintX(int square)
{
    int x, y,a,b;
    switch(square)
    {
    case 1:
        x = y = a = b = 0;
        break;
    case 2:
        a = 1;
        x = 14;
        y = b = 0;
        break;
    case 3:
        a = 2;
        x = 26;
        y = b = 0;
        break;
    case 4:
        x = a = 0;
        b = 1;
        y = 14;
        break;
    case 5:
        a = b = 1;
        x = y = 14;
        break;
    case 6:
        a = 2;
        b = 1;
        x = 26;
        y = 14;
        break;
    case 7:
        x = a = 0;
        b = 2;
        y = 26;
        break;
    case 8:
        a = 1;
        b = 2;
        x = 14;
        y = 26;
        break;
    case 9:
        a = b = 2;
        x = y = 26;
    }

    board[x+1][y+1] = 'X';
    board[x+7][y+1] = 'X';
    board[x+2][y+2] = 'X';
    board[x+6][y+2] = 'X';
    board[x+3][y+3] = 'X';
    board[x+5][y+3] = 'X';
    board[x+4][y+4] = 'X';
    board[x+5][y+5] = 'X';
    board[x+3][y+5] = 'X';
    board[x+2][y+6] = 'X';
    board[x+6][y+6] = 'X';
    board[x+7][y+7] = 'X';
    board[x+1][y+7] = 'X';
    small[a][b] = 1;
}

PrintO(int square)
{
    int x, y,a,b;
    switch(square)
    {
    case 1:
        x = y = a = b = 0;
        break;
    case 2:
        a = 1;
        x = 14;
        y = b = 0;
        break;
    case 3:
        a = 2;
        x = 26;
        y = b = 0;
        break;
    case 4:
        x = a = 0;
        b = 1;
        y = 14;
        break;
    case 5:
        a = b = 1;
        x = y = 14;
        break;
    case 6:
        a = 2;
        b = 1;
        x = 26;
        y = 14;
        break;
    case 7:
        x = a = 0;
        b = 2;
        y = 26;
        break;
    case 8:
        a = 1;
        b = 2;
        x = 14;
        y = 26;
        break;
    case 9:
        a = b = 2;
        x = y = 26;
    }

    board[x+4][y+1] = 'O';
    board[x+5][y+1] = 'O';
    board[x+6][y+1] = 'O';
    board[x+4][y+5] = 'O';
    board[x+5][y+5] = 'O';
    board[x+6][y+5] = 'O';
    board[x+3][y+2] = 'O';
    board[x+3][y+3] = 'O';
    board[x+3][y+4] = 'O';
    board[x+6][y+2] = 'O';
    board[x+6][y+3] = 'O';
    board[x+6][y+4] = 'O';
    small[a][b] = 2;
}

PrintBoard()
{
    for(int i = 0; i < 37; i++)
    {
        printf("\n");
        for(int j = 0; j < 37; j++)
            printf("%c", board[i][j])
            }
        }

        nextTurn(int turn)
        {
            turn = (++turn)%2;
            return turn;
        }

WinChecker()
{

}
