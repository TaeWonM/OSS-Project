#include <stdio.h>
#include<windows.h>
#include<conio.h>

#define MAX_SIZE 12
#define XPOS 50
#define YPOS 5

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

char maze[MAX_SIZE][MAX_SIZE] = { { '1','1','1','1','1','1','1','1','1','1','1','1' },
                                 { '1','x','0','0','0','0','0','0','0','0','0','1' },
                                 { '1','0','1','1','0','1','1','0','1','1','0','1' },
                                 { '1','0','1','1','0','1','1','0','1','1','0','1' },
                                 { '1','0','0','0','y','0','0','y','0','0','0','1' },
                                 { '1','0','1','1','0','1','1','0','1','1','0','1' },
                                 { '1','0','1','1','0','1','1','0','1','1','0','1' },
                                 { '1','0','0','0','y','0','0','y','0','0','0','1' },
                                 { '1','0','1','1','0','1','1','0','1','1','0','1' },
                                 { '1','0','1','1','0','1','1','0','1','1','0','1' },
                                 { '1','0','0','0','0','0','0','0','0','0','0','1' },
                                 { '1','1','1','1','1','1','1','1','1','1','1','1' },
};

void GotoXY(int x, int y);
void print_mazeGame(char maze[][MAX_SIZE], int row);
int is_block(char maze[][MAX_SIZE], int row, int col);
void move_maze(char maze[][MAX_SIZE], int* row, int* col);
void CursorView(char show);
void complete_exit();

int main(void)
{
    int row = 1, col = 1;

    CursorView(0);


    while (1)
    {
        print_mazeGame(maze, 12);
        move_maze(maze, &row, &col);
    }

    return 0;
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
            else if (maze[i][j] == '0')
                printf("□");
            else
                printf("●");
        }
        puts("");
    }
}

int is_block(char maze[][MAX_SIZE], int i, int j)
{

    if (maze[i][j] == '1' || maze[i][j] == 'y')
        return 1;
    else
        return 0;
}

int is_finish(char maze[][MAX_SIZE], int i, int j)
{

    if (maze[i][j] == 'y')
        return 1;
    else
        return 0;
}

void complete_exit()
{
    printf("fail\n");
    exit(0);
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
            if (!(is_block(maze, i, j)))
            {
                maze[*row][j] = '0';
                maze[i][j] = 'x';
                *row -= 1;
            }
            else if (is_finish(maze, i, j))
            {
                maze[*row][j] = '0';
                maze[i][j] = 'x';
                print_mazeGame(maze, 12);
                complete_exit();
            }
            break;

        case DOWN:
            i++;
            if (!(is_block(maze, i, j)))
            {
                maze[*row][j] = '0';
                maze[i][j] = 'x';
                *row += 1;
            }
            else if (is_finish(maze, i, j))
            {
                maze[*row][j] = '0';
                maze[i][j] = 'x';
                print_mazeGame(maze, 12);
                complete_exit();
            }
            break;

        case LEFT:
            j--;
            if (!(is_block(maze, i, j)))
            {
                maze[i][*col] = '0';
                maze[i][j] = 'x';
                *col -= 1;
            }
            else if (is_finish(maze, i, j))
            {
                maze[i][*col] = '0';
                maze[i][j] = 'x';
                print_mazeGame(maze, 12);
                complete_exit();
            }
            break;

        case RIGHT:
            j++;
            if (!(is_block(maze, i, j)))
            {
                maze[i][*col] = '0';
                maze[i][j] = 'x';
                *col += 1;
            }
            else if (is_finish(maze, i, j))
            {
                maze[i][*col] = '0';
                maze[i][j] = 'x';
                print_mazeGame(maze, 12);
                complete_exit();
            }
            break;
        }
    }

}