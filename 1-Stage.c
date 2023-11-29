#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define XPOS 50
#define YPOS 5
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define MAX_SIZE1 12
#define MAX_SIZE2 15
#define MAX_SIZE3 20

typedef struct {
    int row;
    int col;
} Ghost;

Ghost ghosts[MAX_SIZE3 * MAX_SIZE3];
int num_ghosts = 1;

Ghost initial_ghost_positions[][3] = {
    {{7, 7}},           // 레벨 1
    {{5, 5}, {15, 15}}, // 레벨 2
    {{5, 5}, {15, 15}, {10, 10}} // 레벨 3
};

int MAX_SIZE;
int flag[MAX_SIZE3][MAX_SIZE3] = { 1 };
char maze[MAX_SIZE3][MAX_SIZE3];
int count = 0;
int ghost_row = 7, ghost_col = 7;
int game_timer;
int game_level = 0;
time_t start_time;

void GotoXY(int x, int y);
void print_mazeGame(char maze[][MAX_SIZE3], int row);
int p_block(char maze[][MAX_SIZE3], int row, int col);
int m_block(char maze[][MAX_SIZE3], int row, int col);
void move_maze(char maze[][MAX_SIZE3], int* row, int* col);
void moveGhost_player(int player_row, int player_col);
void moveGhost_random(int ghost_index);
void CursorView(char show);
void printTimeElapsed();
int fileopen();
void addGhost();
void initializeGhosts();

int main(void) {
    int row = 1, col = 1;

    printf("난이도 선택 (1, 2, 3): ");
    scanf("%d", &game_level);

    switch (game_level) {
    case 1:
        MAX_SIZE = MAX_SIZE1;
        num_ghosts = 1;
        game_timer = 60;
        break;
    case 2:
        MAX_SIZE = MAX_SIZE2;
        num_ghosts = 2;
        game_timer = 60;
        break;
    case 3:
        MAX_SIZE = MAX_SIZE3;
        num_ghosts = 3;
        game_timer = 60;
        break;
    default:
        printf("잘못된 입력.\n");
        return 1;
    }
    fileopen();
    CursorView(0);
    time(&start_time);

    initializeGhosts();

    while (1) {
        print_mazeGame(maze, MAX_SIZE);
        move_maze(maze, &row, &col);

        if (game_level >= 1) {
            for (int i = 0; i < num_ghosts; i++) {
                if (game_level == 1)
                    moveGhost_player(row, col);
                else if (game_level == 2) {
                    moveGhost_random(i);
                    moveGhost_player(row, col);
                }
                else if (game_level == 3) {
                    moveGhost_random(i);
                    moveGhost_random(i + num_ghosts);
                    moveGhost_player(row, col);
                }
            }
        }

        printTimeElapsed();

        Sleep(100);

        time_t current_time;
        time(&current_time);
        if (difftime(current_time, start_time) >= game_timer) {
            GotoXY(XPOS - 3, YPOS - 2);
            printf("게임 시간 초과");
            exit(0);
        }
    }
    return 0;
}

int fileopen() {
    char fileName[20];
    sprintf(fileName, "maze_%d.txt", game_level);
    FILE* fp = fopen(fileName, "r");

    if (fp == NULL) {
        printf("파일을 열 수 없습니다.\n");
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

    if (game_level > 1) {
        addGhost();
    }

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

int p_block(char maze[][MAX_SIZE3], int i, int j) {
    int clear_count = 0;
    switch (game_level) {
    case 1:
        clear_count = 63;
        break;
    case 2:
        clear_count = 104;
        break;
    case 3:
        clear_count = 3;
        break;
    }

    if (maze[i][j] == '1')
        return 1;
    else if (count == clear_count && maze[i][j] == '0' && flag[i][j] == 0) {
        GotoXY(XPOS - 3, YPOS - 2);
        printf("game clear");
        exit(0);
    }
    else if (flag[i][j] == 0 && maze[i][j] == '0' && flag[i][j] != 1) {
        flag[i][j] = 1;
        count++;
        return 0;
    }
    else
        return 0;
}

int m_block_r(char maze[][MAX_SIZE3], int i, int j) {
    if (i >= 0 && i < MAX_SIZE3 && j >= 0 && j < MAX_SIZE3) {
        return (maze[i][j] == '1');
    }
    else {
        return 1;
    }
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

void moveGhost_player(int player_row, int player_col) {
    int random_direction = rand() % 4;

    int new_ghost_row = ghost_row;
    int new_ghost_col = ghost_col;

    switch (random_direction) {
    case 0:
        if (!(m_block_p(maze, ghost_row - 1, ghost_col))) {
            if (maze[ghost_row][ghost_col] != '*')
                maze[ghost_row][ghost_col] = '0';

            new_ghost_row--;

            if (new_ghost_row == player_row && new_ghost_col == player_col) {
                GotoXY(XPOS - 3, YPOS - 2);
                printf("게임 종료: 유령과 충돌");
                exit(0);
            }
            else if (maze[new_ghost_row][new_ghost_col] != 'x')
                maze[new_ghost_row][new_ghost_col] = 'y';
        }
        break;

    case 1:
        if (!(m_block_p(maze, ghost_row + 1, ghost_col))) {
            if (maze[ghost_row][ghost_col] != '*')
                maze[ghost_row][ghost_col] = '0';

            new_ghost_row++;

            if (new_ghost_row == player_row && new_ghost_col == player_col) {
                GotoXY(XPOS - 3, YPOS - 2);
                printf("게임 종료: 유령과 충돌");
                exit(0);
            }
            else if (maze[new_ghost_row][new_ghost_col] != 'x')
                maze[new_ghost_row][new_ghost_col] = 'y';
        }
        break;

    case 2:
        if (!(m_block_p(maze, ghost_row, ghost_col - 1))) {
            if (maze[ghost_row][ghost_col] != '*')
                maze[ghost_row][ghost_col] = '0';

            new_ghost_col--;

            if (new_ghost_row == player_row && new_ghost_col == player_col) {
                GotoXY(XPOS - 3, YPOS - 2);
                printf("게임 종료: 유령과 충돌");
                exit(0);
            }
            else if (maze[new_ghost_row][new_ghost_col] != 'x')
                maze[new_ghost_row][new_ghost_col] = 'y';
        }
        break;

    case 3:
        if (!(m_block_p(maze, ghost_row, ghost_col + 1))) {
            if (maze[ghost_row][ghost_col] != '*')
                maze[ghost_row][ghost_col] = '0';

            new_ghost_col++;

            if (new_ghost_row == player_row && new_ghost_col == player_col) {
                GotoXY(XPOS - 3, YPOS - 2);
                printf("게임 종료: 유령과 충돌");
                exit(0);
            }
            else if (maze[new_ghost_row][new_ghost_col] != 'x')
                maze[new_ghost_row][new_ghost_col] = 'y';
        }
        break;
    }

    ghost_row = new_ghost_row;
    ghost_col = new_ghost_col;
}

void moveGhost_random(int ghost_index) {
    int random_direction = rand() % 4;

    int current_ghost_row = ghosts[ghost_index].row;
    int current_ghost_col = ghosts[ghost_index].col;

    switch (random_direction) {
    case 0:
        if (!(m_block_r(maze, current_ghost_row - 1, current_ghost_col))) {
            if (maze[current_ghost_row - 1][current_ghost_col] != '*')
                maze[current_ghost_row][current_ghost_col] = '0';

            ghosts[ghost_index].row--;

            if (ghosts[ghost_index].row == ghost_row && ghosts[ghost_index].col == ghost_col) {
                GotoXY(XPOS - 3, YPOS - 2);
                printf("게임 종료: 유령과 충돌");
                exit(0);
            }
            else if (maze[ghosts[ghost_index].row][ghosts[ghost_index].col] != 'x')
                maze[ghosts[ghost_index].row][ghosts[ghost_index].col] = 'y';
        }
        break;

    case 1:
        if (!(m_block_r(maze, current_ghost_row + 1, current_ghost_col))) {
            if (maze[current_ghost_row + 1][current_ghost_col] != '*')
                maze[current_ghost_row][current_ghost_col] = '0';

            ghosts[ghost_index].row++;

            if (ghosts[ghost_index].row == ghost_row && ghosts[ghost_index].col == ghost_col) {
                GotoXY(XPOS - 3, YPOS - 2);
                printf("게임 종료: 유령과 충돌");
                exit(0);
            }
            else if (maze[ghosts[ghost_index].row][ghosts[ghost_index].col] != 'x')
                maze[ghosts[ghost_index].row][ghosts[ghost_index].col] = 'y';
        }
        break;

    case 2:
        if (!(m_block_r(maze, current_ghost_row, current_ghost_col - 1))) {
            if (maze[current_ghost_row][current_ghost_col - 1] != '*')
                maze[current_ghost_row][current_ghost_col] = '0';

            ghosts[ghost_index].col--;

            if (ghosts[ghost_index].row == ghost_row && ghosts[ghost_index].col == ghost_col) {
                GotoXY(XPOS - 3, YPOS - 2);
                printf("게임 종료: 유령과 충돌");
                exit(0);
            }
            else if (maze[ghosts[ghost_index].row][ghosts[ghost_index].col] != 'x')
                maze[ghosts[ghost_index].row][ghosts[ghost_index].col] = 'y';
        }
        break;

    case 3:
        if (!(m_block_r(maze, current_ghost_row, current_ghost_col + 1))) {
            if (maze[current_ghost_row][current_ghost_col + 1] != '*')
                maze[current_ghost_row][current_ghost_col] = '0';

            ghosts[ghost_index].col++;

            if (ghosts[ghost_index].row == ghost_row && ghosts[ghost_index].col == ghost_col) {
                GotoXY(XPOS - 3, YPOS - 2);
                printf("게임 종료: 유령과 충돌");
                exit(0);
            }
            else if (maze[ghosts[ghost_index].row][ghosts[ghost_index].col] != 'x')
                maze[ghosts[ghost_index].row][ghosts[ghost_index].col] = 'y';
        }
        break;
    }
}

void printTimeElapsed() {
    time_t current_time;
    time(&current_time);
    double elapsed_time = difftime(current_time, start_time);

    GotoXY(XPOS - 3, YPOS - 4);
    printf("남은 시간: %.0lf초", game_timer - elapsed_time);
}

void addGhost() {
    for (int i = 0; i < num_ghosts; i++) {
        ghosts[i].row = initial_ghost_positions[game_level - 1][i].row;
        ghosts[i].col = initial_ghost_positions[game_level - 1][i].col;
    }
}

void initializeGhosts() {
    for (int i = 0; i < num_ghosts; i++) {
        ghosts[i].row = initial_ghost_positions[game_level - 1][i].row;
        ghosts[i].col = initial_ghost_positions[game_level - 1][i].col;
    }
}