#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

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
#define ANSI_BG_WHITE      "\x1b[47m"

// Buffer
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t can_referee;
    pthread_cond_t can_play;
} buffer_t;

int board[ROW][COLUMN];
int last_move[4][2];
int started = 0;
pthread_t cons1, cons2, refer;

void* referee(void *arg);
void* consumer1(void *arg);
void* consumer2(void *arg);

int main(int argc, char *argv[]) {
    
    srand(time(0));

    buffer_t buffer = {
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .can_referee = PTHREAD_COND_INITIALIZER,
        .can_play = PTHREAD_COND_INITIALIZER,
    };

    pthread_create(&refer, NULL, referee, (void*)&buffer);
    pthread_create(&cons1, NULL, consumer1, (void*)&buffer);
    pthread_create(&cons2, NULL, consumer2, (void*)&buffer);

    pthread_join(refer, NULL);
    pthread_join(cons1, NULL);
    pthread_join(cons2, NULL);
    
    return 0;
}

// Player 1
void* consumer1(void *arg) {
    buffer_t *buffer = (buffer_t*)arg;

    while(1) {
        pthread_mutex_lock(&buffer->mutex);

        // Play player 1
        pthread_cond_wait(&buffer->can_play, &buffer->mutex);
        play(1);

        // signal
        pthread_cond_signal(&buffer->can_referee);
        pthread_mutex_unlock(&buffer->mutex);      
    }

    // never reached
    return NULL;
}

// Player 2
void* consumer2(void *arg) {
    buffer_t *buffer = (buffer_t*)arg;

    while(1) {
        pthread_mutex_lock(&buffer->mutex);

        // Play player 2
        pthread_cond_wait(&buffer->can_play, &buffer->mutex);
        play(2);

        // signal
        pthread_cond_signal(&buffer->can_referee);
        pthread_mutex_unlock(&buffer->mutex);
    }

    // never reached
    return NULL;
}

// Referee
void* referee(void *arg) {
    
    buffer_t *buffer = (buffer_t*)arg;
    
    if (started == 0)
    {
        pthread_mutex_lock(&buffer->mutex);

        // Create and Initialize the grid
        initializeBoard();

        // signal
        pthread_cond_signal(&buffer->can_play);
        pthread_mutex_unlock(&buffer->mutex);

        started = 1;
    }

    int winner = 0;
    while(1) {
        pthread_mutex_lock(&buffer->mutex);

        // Check the winner
        if (checkWinner() == 1 || checkWinner() == 2)
        {
            winner  = checkWinner();
            showBoard(winner);

            pthread_cancel(refer);
            pthread_cancel(cons1);
            pthread_cancel(cons2);
            quick_exit();
        }
        else 
        {
        // signal the fact that new items may be consumed
        pthread_cond_signal(&buffer->can_play);
        pthread_mutex_unlock(&buffer->mutex);
        }
    }

    return NULL;
}

int play(int _player) {
    int rndCol = myRand(0,6);

    // Check row if empty
    int i;
    for (i = 0; i < ROW; i++) {
        if (board[i][rndCol] == 0) {
            board[i][rndCol] = _player;
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

            if (board[i][j] != 0)
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

    if ((_j + 3) <= (COLUMN - 1))
    {
        int cell = board[_i][_j];
        int cell_1 = board[_i][_j + 1];
        int cell_2 = board[_i][_j + 2];
        int cell_3 = board[_i][_j + 3];

        if (cell_1 == cell && cell_2 == cell && cell_3 == cell)
        {

            last_move[0][0] = _i; 
            last_move[0][1] = _j; 

            last_move[1][0] = _i;
            last_move[1][1] = _j + 1;

            last_move[2][0] = _i;
            last_move[2][1] = _j + 2;

            last_move[3][0] = _i;
            last_move[3][1] = _j + 3;

            // Return the winner number
            return cell;
        }
    }

    return 0;
}

int checkDiagonal(int _i, int _j) {

    if (((_i + 3) <= (ROW - 1)) && ((_j + 3) <= (COLUMN - 1)))
    {
        int cell = board[_i][_j];
        int cell_1 = board[_i + 1][_j + 1];
        int cell_2 = board[_i + 2][_j + 2];
        int cell_3 = board[_i + 3][_j + 3];

        if (cell_1 == cell && cell_2 == cell && cell_3 == cell)
        {
            last_move[0][0] = _i; 
            last_move[0][1] = _j; 

            last_move[1][0] = _i + 1;
            last_move[1][1] = _j + 1;

            last_move[2][0] = _i + 2;
            last_move[2][1] = _j + 2;

            last_move[3][0] = _i + 3;
            last_move[3][1] = _j + 3;

            // Return the winner number
            return cell;
        }
    }

    return 0;
}

int initializeBoard() {

    system("clear");
    printf("\n<---------START--------->\n\n");

    int i,j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            board[i][j] = 0;
        }
    }
    
    return 0;
}

int showBoard(int _winner) {
    int board_tmp[ROW][COLUMN];
    int i,j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            if (highter(i,j) == 1)
            {
                // Mark the reason of winning
                board_tmp[ROW - i - 1][j] = 3;
            }
            else
            {
                board_tmp[ROW - i - 1][j] = board[i][j];
            }
        }
    }

    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COLUMN; j++) {
            if (board_tmp[i][j] == 1)
            {
                printf("%s%d%s ", ANSI_COLOR_YELLOW, board_tmp[i][j], ANSI_COLOR_RESET);
            }
            else if (board_tmp[i][j] == 2)
            {
                printf("%s%d%s ", ANSI_COLOR_RED, board_tmp[i][j], ANSI_COLOR_RESET);
            }
            else if (board_tmp[i][j] == 0)
            {
                printf("%s%d%s ", ANSI_COLOR_GRAY, board_tmp[i][j], ANSI_COLOR_RESET);
            }
            else if (board_tmp[i][j] == 3)
            {
                if (_winner == 1)
                {
                    printf("%s%s%d%s ", ANSI_BG_WHITE, ANSI_COLOR_YELLOW, _winner, ANSI_COLOR_RESET);
                }

                if (_winner == 2)
                {
                    printf("%s%s%d%s ", ANSI_BG_WHITE, ANSI_COLOR_RED, _winner, ANSI_COLOR_RESET);
                }
            }
        }
        printf("\n");
    }

    // Print the winner
    printf("\n<---------WINNER-------->\n");
    if (_winner == 1)
    {
        printf(" %sPLAYER %d%s",ANSI_COLOR_YELLOW , _winner, ANSI_COLOR_RESET);
    }
    else if (_winner == 2)
    {
        printf(" %sPLAYER %d%s",ANSI_COLOR_RED , _winner, ANSI_COLOR_RESET);
    }
    printf("\n<----------------------->\n\n");

    return 0;
}

int highter(int _i, int _j) {

    int i;
    for (i = 0; i <= 3; i++)
    {
        if (last_move[i][0] == _i && last_move[i][1] == _j)
        {
            return 1;
        } 
    }

    return 0;
}

int myRand(int low, int high) {
   return rand() % (high - low + 1) + low;
}