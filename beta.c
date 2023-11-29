#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <locale.h>
#include "Stage3.h"

FILE *fp;
char map[20][101];
int CAMMAXX = 60;
int CAMMAXY = 20;
int MAPMAXX,MOVE = 0;
int startx = 0;
int starty = 0;
float xv = 0;
float yv = 0;
int check = 0;
char stage3achi [5] = {'X','X','X','X','X'};
char stage2achi [4] = {'X','X','X','X'};
char stage1achi [4] = {'X','X','X','X'};
char character = 'W';

void mainstagegotoxy(int x, int y);
void Filescan ();
int Choose_1to3 ();

int main () {
    Filescan ();
    int prex=startx,curx;
    int prey=starty,cury;
    for (int i = 0; i<CAMMAXY; i++) {
            for (int j = 0; j<CAMMAXX; j++) printf ("%c",map[i][j]);
            printf("\n");
    }
    mainstagegotoxy(prex,prey);
    printf("%c",character);
    while (1){
        if (GetAsyncKeyState(VK_LEFT)){
            if (xv >= -1.5){
                xv-=0.3;
            }
        }
        if (GetAsyncKeyState(VK_RIGHT)){
            if (xv <= 1.5){
                xv+=0.3;
            }
        }
        if (GetAsyncKeyState(VK_UP) && prey>1 && map[prey-2][prex-1+MOVE]==' ' && map[prey][prex-1+MOVE] == '='){
            yv-=2;
        }
        else if (GetAsyncKeyState(VK_DOWN) && map[prey][prex-1+MOVE]!=' ' && map[prey][prex-1+MOVE]!='='){
            system ("cls");
            mainstagegotoxy(30,10);
            printf ("Press dificulty 1~3");
            int diffi = Choose_1to3 ();
            switch (map[prey][prex-1+MOVE])
            {
            case '1':
                
                break;
            case '3':
                system("cls");
                strcpy(stage3achi,stage3(diffi));
                system("cls");
                break;
            default:
                break;
            }
            setlocale(LC_ALL,"C");
        }
        
        xv-=0.1*xv;
        yv+=0.3;
        if (GetAsyncKeyState (VK_ESCAPE)){
            system ("cls");
            mainstagegotoxy(30,10);
            printf ("Press Option 1~3");
            mainstagegotoxy(30,11);
            printf(" 1 : EXIT");
            mainstagegotoxy(30,12);
            printf(" 2 : SAVE");
            mainstagegotoxy(30,13);
            printf(" 3 : Change Character");
            switch (Choose_1to3())
            {
            case 1:
                system ("cls");
                return 0;
            default:
                break;
            };
            setlocale( LC_ALL, ".ACP" );
        }
        Sleep(20);
        if (map[prey][prex-1+MOVE] != '=' && map[prey][prex-1+MOVE] != ' '){
            if (map[prey][prex-1+MOVE] == '1'){
                mainstagegotoxy(80,5);
                printf ("Stage 1 : Pac-Man");
                mainstagegotoxy(80,7);
                printf ("Press Down");
            }
            else if (map[prey][prex-1+MOVE] == '2'){
                mainstagegotoxy(80,5);
                printf ("Stage 2 : Tetris");
                mainstagegotoxy(80,7);
                printf ("Press Down");
            }
            else if (map[prey][prex-1+MOVE] == '3'){
                mainstagegotoxy(80,5);
                printf ("Stage 3 : Untertail");
                mainstagegotoxy(80,7);
                printf ("Press Down");
                mainstagegotoxy(80,9);
                if (stage3achi[0]=='O'){
                    printf("1 : Is it a start?");
                    mainstagegotoxy(80,10);
                    printf("Condition: Phase 1 passed");
                }
                else {
                    printf("1 : ???");
                }
                mainstagegotoxy(80,11);
                if (stage3achi[1]=='O'){
                    printf("2 : An Unperturbed Heart");
                    mainstagegotoxy(80,12);
                    printf("Condition: Phase 2 passed");
                }
                else {
                    printf("2 : ???");
                }
                mainstagegotoxy(80,13);
                if (stage3achi[2]=='O'){
                    printf("3 : Tough Bond");
                    mainstagegotoxy(80,14);
                    printf("Condition: Phase 3 passed");
                }
                else {
                    printf("3 : ???");
                }
                mainstagegotoxy(80,15);
                if (stage3achi[3]=='O'){
                    printf("4 : Perfect Clear");
                    mainstagegotoxy(80,16);
                    printf("Condition: Pass phases 1,2,3 with 3 lives");
                }
                else {
                    printf("4 : ???");
                }
                mainstagegotoxy(80,17);
                if (stage3achi[4]=='O'){
                    printf("5 : Master");
                    mainstagegotoxy(80,18);
                    printf("Condition: Clear with 3 difficulty levels");
                }
                else {
                    printf("5 : ???");
                }
            }
            check = 1;
        }
        else if (check) {
            system ("cls");
            check = 0;
        }
        curx = prex + (int)xv;
        if (map[prey-1][curx-1 + MOVE] != ' '){
            curx = prex;
        }
        cury = prey + (int)yv;
        if (cury > prey){
            for (int i = prey; i <= cury; i++) {
                if (map[i-1][curx-1 + MOVE] != ' ') {
                    cury = i-1;
                    prey = i-1;
                    yv = 0;
                    break;
                }
            }
        }
        if (cury < prey){
            for (int i = cury; i <= prey; i++) {
                if (map[i-1][curx-1 + MOVE] != ' ') {
                    cury = i-1;
                    prey = i-1;
                    yv = 0;
                    break;
                }
            }
        }
        if (prex>CAMMAXX*0.8 && MOVE+CAMMAXX != MAPMAXX) {
            MOVE+=1;
            curx--;
        }
        if (prex<CAMMAXX*0.2 && MOVE != 0) {
            MOVE-=1;
            curx++;
        }
        mainstagegotoxy(1,1);
        for (int i = 0; i<CAMMAXY; i++) {
            printf ("%.60s",&map[i][MOVE]);
            printf("\n");
        }
        if (prex != curx || prey!= cury){
        mainstagegotoxy (prex,prey);
        printf (" ");
        mainstagegotoxy (curx,cury);
        printf ("%c",character);
        }
        else {
            mainstagegotoxy (curx,cury);
            printf ("%c",character);
        }
        prex = curx;
        prey = cury;
    }
}
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

void mainstagegotoxy(int x, int y){
    COORD pos = {x-1, y-1};
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int Choose_1to3 (){
    while (1){
        if (GetAsyncKeyState (0x31)){
            return 1;
        }
        if (GetAsyncKeyState (0x32)){
            return 2;
        }
        if (GetAsyncKeyState (0x33)){
            return 3;
        }
    }
}
