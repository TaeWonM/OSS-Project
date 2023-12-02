#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <locale.h>
#include <wchar.h>

#define XPOS 50
#define YPOS 5
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define MAX_SIZE1 12
#define MAX_SIZE2 15
#define MAX_SIZE3 21

int MAX_SIZE;
int flag[MAX_SIZE3][MAX_SIZE3] = { 1 };
char maze[MAX_SIZE3][MAX_SIZE3];
int count = 0;
int ghost_row = 7, ghost_col = 7;
int game_timer;
int game_level = 0;
int clear_count = 99999;
time_t start_time;
int num_ghosts;
char achivemant[4] =  {'X','X','X','\0' }; //업적 개수

typedef struct {
    int row;
    int col;
    int direction;
} Ghost;

Ghost ghosts[MAX_SIZE3 * MAX_SIZE3];

void moveGhost_player(int player_row, int player_col, int ghost_index);
void GotoXY(int x, int y);
void print_mazeGame(char maze[][MAX_SIZE3], int row);
int p_block(char maze[][MAX_SIZE3], int row, int col);
int m_block_r(char maze[][MAX_SIZE3], int row, int col);
int m_block_p(char maze[][MAX_SIZE3], int row, int col);
void move_maze(char maze[][MAX_SIZE3], int* row, int* col);
void CursorView(char show);
void printTimeElapsed();
int fileopen();
int checkGameOver(int player_row, int player_col);
void initializeGhosts();

char *main(int diffi) {
    game_level = diffi;
    int row = 1, col = 1;
    setlocale(LC_ALL,"");

    switch (game_level) {
    case 1:
        MAX_SIZE = MAX_SIZE1;
        game_timer = 60;
        num_ghosts = 1;
        clear_count = 63;
        break;
    case 2:
        MAX_SIZE = MAX_SIZE2;
        game_timer = 60;
        num_ghosts = 2;
        clear_count = 104;
        break;
    case 3:
        MAX_SIZE = MAX_SIZE3;
        game_timer = 90;
        num_ghosts = 3;
        clear_count = 216;
        break;
    default:
        return 1;
    }

    fileopen();
    CursorView(0);
    time(&start_time);

    initializeGhosts();

    for (int i = 0; i < 4; i++)
        achivemant[i] = 'X';

    while (1) {
        print_mazeGame(maze, MAX_SIZE);
        move_maze(maze, &row, &col);

        if (game_level >= 1) {
            for (int i = 0; i < num_ghosts; i++) {
                if (game_level == 1)
                    moveGhost_player(row, col, i);
                else if (game_level == 2)
                    moveGhost_player(row, col, i);
                else if (game_level == 3)
                    moveGhost_player(row, col, i);
            }
        }

        if (checkGameOver(row, col)) {
                system("cls");
                GotoXY(XPOS - 3, YPOS - 2);
                wprintf(L"게임 오버: 유령과 부딪혔습니다.");
                GotoXY(XPOS - 3, YPOS - 3);
                printf("Press Enter To Return");
                while (1) if (GetAsyncKeyState(VK_RETURN)) return achivemant;
        }

        printTimeElapsed();

        Sleep(100);

        time_t current_time;
        time(&current_time);

        if (difftime(current_time, start_time) >= game_timer) {
                system("cls");
                GotoXY(XPOS - 3, YPOS - 2);
                wprintf(L"게임 시간 초과");
                GotoXY(XPOS - 3, YPOS - 3);
                printf("Press Enter To Return");
                while (1) if (GetAsyncKeyState(VK_RETURN)) return achivemant;
        }
    }
    return 0;
}


int fileopen() {
    char fileName[20];
    sprintf(fileName, "maze_%d.txt", game_level);
    FILE* fp = fopen(fileName, "r");

    if (fp == NULL) {
        wprintf(L"파일을 열 수 없습니다.\n");
        exit(1);
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            fscanf(fp, " %c", &maze[i][j]);
            if (maze[i][j] != '0')
                flag[i][j] = 0;
        }
    }
    fclose(fp);

    return 0;
}

void CursorView(char show) {
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void GotoXY(int x, int y) {
    COORD Pos;
    Pos.X = x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

int GetKey() {
    if (_kbhit() != 0) {
        return _getch();
    }

    return 0;
}

void print_mazeGame(char maze[][MAX_SIZE3], int row) {
    for (int i = 0; i < row; i++) {
        GotoXY(XPOS, YPOS + i);
        for (int j = 0; j < MAX_SIZE; j++) {
            if (maze[i][j] == '1')
                wprintf(L"■");
            else if (maze[i][j] == 'y')
                printf("e");
            else if (maze[i][j] == '0' && flag[i][j] == 0)
                printf(".");
            else if (maze[i][j] == '0' && flag[i][j] == 1)
                printf(" ");
            else if (maze[i][j] == 'x')
                wprintf(L"●");
        }
        puts("");
    }
}

int p_block(char maze[][MAX_SIZE3], int i, int j) {

    if (maze[i][j] == '1')
        return 1;
    else if (count == clear_count && maze[i][j] == '0' && flag[i][j] == 0) {
            system("cls");
            GotoXY(XPOS - 3, YPOS - 2);
            printf("game clear\n");
            GotoXY(XPOS - 3, YPOS - 3);
            printf("Press Enter To Return");
            while (1) if (GetAsyncKeyState(VK_RETURN)) return achivemant;
        switch (game_level) {
        case 1:
            achivemant[game_level - 1] = 'O';
            break;
        case 2:
            achivemant[game_level - 1] = 'O';
            break;
        case 3:
            achivemant[game_level - 1] = 'O';
            break;
        }
        return achivemant;
    }
    else if (flag[i][j] == 0 && maze[i][j] == '0' && flag[i][j] != 1) {
        flag[i][j] = 1;
        count++;
        return 0;
    }
    else
        return 0;
}

int m_block_p(char maze[][MAX_SIZE3], int i, int j) {
    if (i >= 0 && i < MAX_SIZE3 && j >= 0 && j < MAX_SIZE3) {
        return (maze[i][j] == '1');
    }
    else {
        return 1;
    }
}

void move_maze(char maze[][MAX_SIZE3], int* row, int* col) {
    int chr;
    int i = *row;
    int j = *col;

    chr = GetKey();

    if (chr == 0 || chr == 0xe0) {
        chr = GetKey();
        switch (chr) {
        case UP:
            i--;
            if (!(p_block(maze, i, j))) {
                maze[*row][j] = '0';
                maze[i][j] = 'x';
                *row -= 1;
            }
            break;

        case DOWN:
            i++;
            if (!(p_block(maze, i, j))) {
                maze[*row][j] = '0';
                maze[i][j] = 'x';
                *row += 1;
            }
            break;

        case LEFT:
            j--;
            if (!(p_block(maze, i, j))) {
                maze[i][*col] = '0';
                maze[i][j] = 'x';
                *col -= 1;
            }
            break;

        case RIGHT:
            j++;
            if (!(p_block(maze, i, j))) {
                maze[i][*col] = '0';
                maze[i][j] = 'x';
                *col += 1;
            }
            break;
        }
    }
}

void moveGhost_player(int player_row, int player_col, int ghost_index)
{
    int random_direction = rand() % 4;

    switch (random_direction)
    {
    case 0:
        if (!(m_block_p(maze, ghosts[ghost_index].row - 1, ghosts[ghost_index].col)))
        {
            if (maze[ghosts[ghost_index].row - 1][ghosts[ghost_index].col] != '*')
                maze[ghosts[ghost_index].row][ghosts[ghost_index].col] = '0';

            maze[ghosts[ghost_index].row - 1][ghosts[ghost_index].col] = 'y';
            ghosts[ghost_index].row--;
        }
        break;

    case 1:
        if (!(m_block_p(maze, ghosts[ghost_index].row + 1, ghosts[ghost_index].col)))
        {
            if (maze[ghosts[ghost_index].row + 1][ghosts[ghost_index].col] != '*')
                maze[ghosts[ghost_index].row][ghosts[ghost_index].col] = '0';

            maze[ghosts[ghost_index].row + 1][ghosts[ghost_index].col] = 'y';
            ghosts[ghost_index].row++;
        }
        break;

    case 2:
        if (!(m_block_p(maze, ghosts[ghost_index].row, ghosts[ghost_index].col - 1)))
        {
            if (maze[ghosts[ghost_index].row][ghosts[ghost_index].col - 1] != '*')
                maze[ghosts[ghost_index].row][ghosts[ghost_index].col] = '0';

            maze[ghosts[ghost_index].row][ghosts[ghost_index].col - 1] = 'y';
            ghosts[ghost_index].col--;
        }
        break;

    case 3:
        if (!(m_block_p(maze, ghosts[ghost_index].row, ghosts[ghost_index].col + 1)))
        {
            if (maze[ghosts[ghost_index].row][ghosts[ghost_index].col + 1] != '*')
                maze[ghosts[ghost_index].row][ghosts[ghost_index].col] = '0';

            maze[ghosts[ghost_index].row][ghosts[ghost_index].col + 1] = 'y';
            ghosts[ghost_index].col++;
        }
        break;
    }
}

void printTimeElapsed() {
    time_t current_time;
    time(&current_time);
    double elapsed_time = difftime(current_time, start_time);

    GotoXY(XPOS - 3, YPOS - 4);
    wprintf(L"남은 시간: %.0lf초", game_timer - elapsed_time);

}

void initializeGhosts() {
    if (game_level == 1) {
        ghosts[0].row = ghost_row;
        ghosts[0].col = ghost_col;
        ghosts[0].direction = 0;
    }
    else if (game_level == 2) {
        ghosts[0].row = ghost_row;
        ghosts[0].col = ghost_col;
        ghosts[0].direction = 0;
        ghosts[1].row = 10;
        ghosts[1].col = 10;
        ghosts[1].direction = 1;
    }
    else if (game_level == 3) {
        ghosts[0].row = ghost_row;
        ghosts[0].col = ghost_col;
        ghosts[0].direction = 0;
        ghosts[1].row = 13;
        ghosts[1].col = 13;
        ghosts[1].direction = 1;
        ghosts[2].row = 19;
        ghosts[2].col = 19;
        ghosts[2].direction = 2;
    }
}

int checkGameOver(int player_row, int player_col) {

    for (int i = 0; i < num_ghosts; i++) {
        if (player_row == ghosts[i].row && player_col == ghosts[i].col) {
            return 1;
        }
    }

    return 0;
}