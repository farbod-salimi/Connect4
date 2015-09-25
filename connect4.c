#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ROW 6
#define COLUMN 7

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_GRAY    "\e[0;37m"

int grid[ROW][COLUMN];

int main ()
{
    // Choose a player
    srand(time(0));
    int player = myRand(1,2);

    // Create and Initialize the grid
    gridInitializer();
    system("clear");

    printf("\n<---------START--------->\n");
    printf(" %sFirst player is : %d%s",ANSI_COLOR_GREEN , player, ANSI_COLOR_RESET);
    printf("\n<----------------------->\n\n");

    // Play game and find the winner
    int winner = 0;
    while(1) {
        if (player == 1) 
        {
            // Player one start            
            play(1);

            // Check the winner
            if (checkWinner() == 1)
            {
                winner  = checkWinner();
                break;                
            }

            // Change the player
            player = 2;
        }
        else
        {
            // Player two start
            play(2);

            // Check the winner
            if (checkWinner() == 2)
            {
                winner  = checkWinner();
                break;                
            }

            // Change the player
            player = 1;
        }
    }
    
    // Print the grid
    gridShow();

    // Print the winner
    printf("\n<---------WINNER-------->\n");
    if (winner == 1)
    {
        printf(" %sPLAYER %d%s",ANSI_COLOR_YELLOW , winner, ANSI_COLOR_RESET);
    }
    else if (winner == 2)
    {
        printf(" %sPLAYER %d%s",ANSI_COLOR_RED , winner, ANSI_COLOR_RESET);
    }
    printf("\n<----------------------->\n\n");
    
    return 0;
}

int play(int _player) {
    int rndCol = myRand(0,6);

    // Check row if empty
    int i;
    for (i = 0; i < ROW; i++) {
        if (grid[i][rndCol] == 0) {
            grid[i][rndCol] = _player;
            break;
        }
    }

    return 0;
}

int checkWinner() {
    int result = 0;
    int i,j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {

            if (grid[i][j] != 0)
            {
                // Check line
                if (checkLine(i,j) != 0)
                {
                    result = checkLine(i,j);
                    break;
                // Check Diagonal
                }else if (checkDiagonal(i,j) != 0) {
                    result = checkDiagonal(i,j);
                    break;
                }
            }
        }
    }

    return result;
}

int checkLine(int _i, int _j) {

    if ((_j + 3) <= COLUMN)
    {
        int cell = grid[_i][_j];
        int cell_1 = grid[_i][_j + 1];
        int cell_2 = grid[_i][_j + 2];
        int cell_3 = grid[_i][_j + 3];

        if (cell_1 == cell && cell_2 == cell && cell_3 == cell)
        {
            // Return the winner number
            return cell;
        }
    }

    return 0;
}

int checkDiagonal(int _i, int _j) {

    if (((_i + 3) <= ROW) && ((_j + 3) <= COLUMN))
    {
        int cell = grid[_i][_j];
        int cell_1 = grid[_i + 1][_j + 1];
        int cell_2 = grid[_i + 2][_j + 2];
        int cell_3 = grid[_i + 3][_j + 3];

        if (cell_1 == cell && cell_2 == cell && cell_3 == cell)
        {
            // Return the winner number
            return cell;
        }
    }

    return 0;
}

int gridInitializer() {
    int i,j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            grid[i][j] = 0;
        }
    }
    
    return 0;
}

int gridShow() {
    int tpmGrid[ROW][COLUMN];
    int i,j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            //printf("%d ", grid[i][j]);
            tpmGrid[ROW - i - 1][j] = grid[i][j];
            //printf("a[%d][%d] = %d\n", i,j, grid[i][j] );
        }
        //printf("\n");
    }

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            if (tpmGrid[i][j] == 1)
            {
                printf("%s%d%s ", ANSI_COLOR_YELLOW, tpmGrid[i][j], ANSI_COLOR_RESET);
            }
            else if (tpmGrid[i][j] == 2)
            {
                printf("%s%d%s ", ANSI_COLOR_RED, tpmGrid[i][j], ANSI_COLOR_RESET);
            }
            else if (tpmGrid[i][j] == 0)
            {
                printf("%s%d%s ", ANSI_COLOR_GRAY, tpmGrid[i][j], ANSI_COLOR_RESET);
            }
        }
        printf("\n");
    }

    return 0;
}

int myRand(int low, int high) {
   //srand(time(0));
   return rand() % (high - low + 1) + low;
}