#include <stdio.h>
#include <windows.h>
FILE *fp;
char map[20][101];
int CAMMAXX = 60;
int CAMMAXY = 20;
int MAPMAXX,MOVE = 0;
int startx = 0;
int starty = 0;
int check = 0;

void Filescan () {
    fp = fopen ("mainmap.txt","r");
    for (int i = 0; i<CAMMAXY; i++) fgets (map[i],sizeof(char)*101,fp);
    fclose (fp);
    MAPMAXX = strlen(map[0])-1;
    for (int i = 0; i<CAMMAXY; i++){
        for (int j = 0; j<CAMMAXX; j++){
            if (map[i][j] == '*') {
                startx = j+1;
                starty = i+1;
                map[i][j]=' ';
                return;
            }
        }
    }
}

void gotoxy(int x, int y){
    COORD pos = {x-1, y-1};
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main () {
    Filescan ();
    int x=startx;
    int y=starty;
    for (int i = 0; i<CAMMAXY; i++) {
            for (int j = 0; j<CAMMAXX; j++) printf ("%c",map[i][j]);
            printf("\n");
    }
    while (1){
        if (GetAsyncKeyState(VK_LEFT) && x>1 && map[y-1][x-2+MOVE]==' '){
            x--;
        }
        if (GetAsyncKeyState(VK_RIGHT) && x<MAPMAXX && map[y-1][x+MOVE]==' '){
            x++;
        }
        if (GetAsyncKeyState(VK_UP) && y>1 && map[y-2][x-1+MOVE]==' '){
            y--;
        }
        if (GetAsyncKeyState(VK_DOWN) && y<CAMMAXY && map[y][x-1+MOVE]==' '){
            y++;
        }
        else if (GetAsyncKeyState(VK_DOWN) && map[y][x-1+MOVE]!=' ' && map[y][x-1+MOVE]!='='){
            system ("cls");
            gotoxy(30,10);
            printf ("LOADING");
        }
        Sleep(50);
        if (map[y][x-1+MOVE] != '=' && map[y][x-1+MOVE] != ' '){
            if (map[y][x-1+MOVE] == '1'){
                gotoxy(80,5);
                printf ("Stage 1 : Pac-Man");
                gotoxy(80,6);
                printf ("Dificulty : **");
                gotoxy(80,7);
                printf ("Press Down");
            }
            else if (map[y][x-1+MOVE] == '2'){
                gotoxy(80,5);
                printf ("Stage 2 : Tetris");
                gotoxy(80,6);
                printf ("Dificulty : ****");
                gotoxy(80,7);
                printf ("Press Down");
            }
            else if (map[y][x-1+MOVE] == '3'){
                gotoxy(80,5);
                printf ("Stage 3 : Untertail");
                gotoxy(80,6);
                printf ("Dificulty : ******");
                gotoxy(80,7);
                printf ("Press Down");
            }
            check = 1;
            }
        else if (check) {
            system ("cls");
            check = 0;
        }
        if (x>CAMMAXX*0.9 && MOVE+CAMMAXX != MAPMAXX) {
            MOVE+=1;
            x--;
        }
        if (x<CAMMAXX*0.1 && MOVE != 0) {
            MOVE-=1;
            x++;
        }
        gotoxy(1,1);
        for (int i = 0; i<CAMMAXY; i++) {
            printf ("%.60s",&map[i][MOVE]);
            printf("\n");
        }
        gotoxy (x,y);
        printf ("W");
    }
}