#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 12
#define XPOS 50
#define YPOS 5

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

char maze[MAX_SIZE][MAX_SIZE];
int flag[MAX_SIZE][MAX_SIZE] = { 1 };
int count = 0;

int ghost_row = 5, ghost_col = 6;

void GotoXY(int x, int y);
void print_mazeGame(char maze[][MAX_SIZE], int row);
int is_block(char maze[][MAX_SIZE], int row, int col);
void move_maze(char maze[][MAX_SIZE], int* row, int* col);
void moveGhost(int player_row, int player_col);

void CursorView(char show);
int fileopen();


int main(void)
{
    int row = 1, col = 1;
    int y_row = 2, y_col = 2;

    fileopen();
    CursorView(0);

    while (1)
    {
        print_mazeGame(maze, 12);
        move_maze(maze, &row, &col);
        moveGhost(row, col);

        if (row == ghost_row && col == ghost_col)
        {
            GotoXY(XPOS - 3, YPOS - 2);
            printf("게임 종료");
            exit(0);
        }
        Sleep(100);
    }

    return 0;
}


int fileopen()
{
    FILE* fp = fopen("maze_1.txt", "r");

    for (int i = 0; i < MAX_SIZE; i++)
    {
        for (int j = 0; j < MAX_SIZE; j++)
        {
            fscanf(fp, " %c", &maze[i][j]);
            if (maze[i][j] != '0')
                flag[i][j] = 0;
        }
    }
    fclose(fp);
}

void CursorView(char show)
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void GotoXY(int x, int y)
{
    COORD Pos;
    Pos.X = x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

int GetKey()
{
    if (_kbhit() != 0)
    {
        return _getch();
    }

    return 0;
}

void print_mazeGame(char maze[][MAX_SIZE], int row)
{
    for (int i = 0; i < row; i++)
    {
        GotoXY(XPOS, YPOS + i);
        for (int j = 0; j < MAX_SIZE; j++)
        {
            if (maze[i][j] == '1')
                printf("■");
            else if (maze[i][j] == 'y')
                printf("e");
            else if (maze[i][j] == '0' && flag[i][j] == 0)
                printf("*");
            else if (maze[i][j] == '0' && flag[i][j] == 1)
                printf(" ");
            else if (maze[i][j] == 'x')
                printf("●");
        }
        puts("");
    }
}

int p_block(char maze[][MAX_SIZE], int i, int j)
{
    if (maze[i][j] == '1')
        return 1;
    else if (count == 63 && maze[i][j] == '0' && flag[i][j] == 0)
    {
        GotoXY(XPOS - 3, YPOS - 2);
        printf("game clear");
        exit(0);
    }
    else if (flag[i][j] == 0 && maze[i][j] == '0' && flag[i][j] != 1)
    {
        flag[i][j] = 1;
        count++;
        return 0;
    }
    else
        return 0;
}

void move_maze(char maze[][MAX_SIZE], int* row, int* col)
{
    int chr;
    int i = *row;
    int j = *col;

    chr = GetKey();

    if (chr == 0 || chr == 0xe0)
    {
        chr = GetKey();
        switch (chr)
        {
        case UP:
            i--;
            if (!(p_block(maze, i, j)))
            {
                maze[*row][j] = '0';
                maze[i][j] = 'x';
                *row -= 1;
            }
            break;

        case DOWN:
            i++;
            if (!(p_block(maze, i, j)))
            {
                maze[*row][j] = '0';
                maze[i][j] = 'x';
                *row += 1;
            }
            break;

        case LEFT:
            j--;
            if (!(p_block(maze, i, j)))
            {
                maze[i][*col] = '0';
                maze[i][j] = 'x';
                *col -= 1;
            }
            break;

        case RIGHT:
            j++;
            if (!(p_block(maze, i, j)))
            {
                maze[i][*col] = '0';
                maze[i][j] = 'x';
                *col += 1;
            }
            break;
        }
    }
}


void moveGhost(int player_row, int player_col)
{
    int random_direction = rand() % 4;

    switch (random_direction)
    {
    case 0:
        if (!(is_block(maze, ghost_row - 1, ghost_col)))
        {
            if (maze[ghost_row][ghost_col] != '*')
                maze[ghost_row][ghost_col] = '0';

            maze[ghost_row - 1][ghost_col] = 'y';
            ghost_row--;
        }
        break;

    case 1:
        if (!(is_block(maze, ghost_row + 1, ghost_col)))
        {
            if (maze[ghost_row][ghost_col] != '*')
                maze[ghost_row][ghost_col] = '0';

            maze[ghost_row + 1][ghost_col] = 'y';
            ghost_row++;
        }
        break;

    case 2:
        if (!(is_block(maze, ghost_row, ghost_col - 1)))
        {
            if (maze[ghost_row][ghost_col] != '*')
                maze[ghost_row][ghost_col] = '0';

            maze[ghost_row][ghost_col - 1] = 'y';
            ghost_col--;
        }
        break;

    case 3:
        if (!(is_block(maze, ghost_row, ghost_col + 1)))
        {
            if (maze[ghost_row][ghost_col] != '*')
                maze[ghost_row][ghost_col] = '0';

            maze[ghost_row][ghost_col + 1] = 'y';
            ghost_col++;
        }
        break;
    }
}
