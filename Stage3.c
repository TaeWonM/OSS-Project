#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <process.h>
#include "Stage.h"

int printx = 20, printy = 5;
int stage3mapmaxx = 18, stage3mapmaxy = 14;
clock_t start,skillstart;
clock_t phasetime;
char smallstring[] = " ";
int life = 3;
int prestage3x;
int prestage3y;
int stage3x = 29;
int stage3y = 12;
char cleartring[50];
char stage3achieve[6] = {'X','X','X','X','X','\0'};
int difficulty;
short stage3phase;
HANDLE threadId;
char * oldsetting;

char *stage3(int Difficulty) {
    life = 3;
    stage3mapmaxx = 18;
    stage3mapmaxy = 14;
    difficulty = Difficulty;
    Setlife ();
    setlocale(LC_ALL,"");
    textcolor(15);
    makeclearstring();
    stageprint ();
    stage3gotoxy (stage3x,stage3y);
    printf ("@");
    start = clock();
    phasetime = start;
    srand(time(NULL));
    phase1(1);
    if (life<=0){
        return end(0);
    }
    system ("cls");
    start = clock();
    phasetime = start;
    phase2();
    phase1(2);
    if (life<=0){
        return end(1);
    }
    phase3();
    if (life<=0){
        return end(2);
    }
    else {
        return end(3);
    }
}


void textcolor (int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    return;
}

void stageprint () {
    for (int j = 0; j<stage3mapmaxx+2; j++) {
            stage3gotoxy (printx+j,printy);
            wprintf(L"■");
    }
    for (int i = 1; i<stage3mapmaxy+1; i++) {
        for (int j = 0; j<stage3mapmaxx+2; j++) {
            stage3gotoxy (printx+j,printy+i);
            if ( j== 0 || j == stage3mapmaxx+1) wprintf(L"■");
        }
    }
    for (int j = 0; j<stage3mapmaxx+2; j++) {
            stage3gotoxy (printx+j,printy+stage3mapmaxy+1);
            wprintf(L"■");
    }
    return;
};

void clear (){
    for (int i = 1; i<=stage3mapmaxy; i++){
        stage3gotoxy(printx+1,printy+i);
        printf("%s",cleartring);
    }
    return;
};

void phase1move (){
    prestage3x = stage3x;
    prestage3y = stage3y;
    if (GetAsyncKeyState(VK_LEFT) && stage3x-printx-1 > 0){
        stage3x--;
    }
    if (GetAsyncKeyState(VK_RIGHT) && stage3x-printx+1 <= stage3mapmaxx){
        stage3x++;
    }
    if (GetAsyncKeyState(VK_UP)&& stage3y-printy-1 > 0){
        stage3y--;
    }
    if (GetAsyncKeyState(VK_DOWN)&& stage3y-printy+1 <= stage3mapmaxy){
        stage3y++;
    }
    if (prestage3x != stage3x || prestage3y != stage3y){
        textcolor(15);
        stage3gotoxy(prestage3x,prestage3y);
        printf (" ");
        stage3gotoxy (stage3x,stage3y);
        printf ("@");
    }
    else {
        textcolor(15);
        stage3gotoxy (stage3x,stage3y);
        printf ("@");
    }
    return;
};

void Setlife (){
    stage3gotoxy(printx + stage3mapmaxx +2 ,10);
    printf ("Life = %d",life); 
    return;
};

void makeclearstring () {
    strcpy(cleartring,smallstring);
    for (int i = 1; i<stage3mapmaxx; i++){
        strcat(cleartring,smallstring);
    }
    return;
};

int skill1 (int ph) {
        phase1move();
        textcolor(3);
        int col,raw;
        if (ph==1) col = rand()%(stage3mapmaxy);
        else if (ph==2) col = stage3y-printy-1;
        if (ph==1) raw = rand()%(stage3mapmaxx);
        else if (ph==2) raw = stage3x-printx-1;
        for (int i = 0; i<stage3mapmaxx; i++){
            stage3gotoxy(printx+1+i,printy+1+col);
            printf("-");
        }
        for (int i = 0; i<stage3mapmaxy; i++){
            stage3gotoxy(printx+1+raw,printy+1+i);
            printf("|");
        }
        stage3gotoxy(printx+1+raw,printy+1+col);
        printf ("+");
        skillstart = clock();
        while ((double)(clock() - skillstart) / CLOCKS_PER_SEC <=0.7){
            phase1move();
            Sleep (30);
        }
        if (stage3y == printy+1+col || stage3x == printx+1+raw){
            life--;
            if (life==0) return 1;
            Setlife();
        }
    clear();
    return 0;
}

int skill2(){
    wchar_t skill2smallstring[] = L"■";
    wchar_t skill2string[50] = L"■";
    for (int i = 1; i <stage3mapmaxx; i++){
        wcscat(skill2string,skill2smallstring);
    }
    wchar_t skill2halfstring[25] = L"■";
    for (int i = 1; i <stage3mapmaxx/2; i++){
        wcscat(skill2halfstring,skill2smallstring);
    }
    textcolor(3);
    srand(time(NULL));
    switch (rand()%4)
    {
    case 0:
        for (int i = 1; i<=stage3mapmaxy/2; i++){
            stage3gotoxy(printx+1,printy+i);
            wprintf(L"%s",skill2halfstring);
        }
        for (int i = stage3mapmaxy/2+1; i<=stage3mapmaxy; i++){
            stage3gotoxy(printx+1,printy+i);
            wprintf(L"%s",skill2string);
        }
        textcolor(15);
        stage3gotoxy(stage3x,stage3y);
        printf("@");
        skillstart = clock();
        while ((double)(clock() - skillstart) / CLOCKS_PER_SEC <=0.7){
            phase1move();
            Sleep (30);
        }
        if (stage3x < printx + stage3mapmaxx/2 || stage3y >= printy + stage3mapmaxy/2){
            life--;
            Setlife();
        }
        break;
    case 2:
        for (int i = 1; i<=stage3mapmaxy/2; i++){
            stage3gotoxy(printx+1,printy+i);
            wprintf(L"%s",skill2string);
        }
        for (int i = stage3mapmaxy/2+1; i<=stage3mapmaxy; i++){
            stage3gotoxy(printx+1,printy+i);
            wprintf(L"%s",skill2halfstring);
        }
        textcolor(15);
        stage3gotoxy(stage3x,stage3y);
        printf("@");
        skillstart = clock();
        while ((double)(clock() - skillstart) / CLOCKS_PER_SEC <=0.7){
            phase1move();
            Sleep (30);
        }
        if (stage3x < printx + stage3mapmaxx/2 || stage3y <= printy + stage3mapmaxy/2){
            life--;
            Setlife();
        }
        break;
    case 3:
        for (int i = 1; i<=stage3mapmaxy/2; i++){
            stage3gotoxy(printx+1+stage3mapmaxx/2,printy+i);
            wprintf(L"%s",skill2halfstring);
        }
        for (int i = stage3mapmaxy/2+1; i<=stage3mapmaxy; i++){
            stage3gotoxy(printx+1,printy+i);
            wprintf(L"%s",skill2string);
        }
        textcolor(15);
        stage3gotoxy(stage3x,stage3y);
        printf("@");
        skillstart = clock();
        while ((double)(clock() - skillstart) / CLOCKS_PER_SEC <=0.7){
            phase1move();
            Sleep (30);
        }
        if (stage3x > printx + stage3mapmaxx/2 || stage3y >= printy + stage3mapmaxy/2){
            life--;
            Setlife();
        }
        break;
    case 4:
        for (int i = 1; i<=stage3mapmaxy/2; i++){
            stage3gotoxy(printx+1,printy+i);
            wprintf(L"%s",skill2string);
        }
        for (int i = stage3mapmaxy/2+1; i<=stage3mapmaxy; i++){
            stage3gotoxy(printx+1+stage3mapmaxx/2,printy+i);
            wprintf(L"%s",skill2halfstring);
        }
        textcolor(15);
        stage3gotoxy(stage3x,stage3y);
        printf("@");
        skillstart = clock();
        while ((double)(clock() - skillstart) / CLOCKS_PER_SEC <=0.7){
            phase1move();
            Sleep (30);
        }
        if (stage3x > printx + stage3mapmaxx/2 || stage3y <= printy + stage3mapmaxy/2){
            life--;
            Setlife();
        }
        break;
    default:
        break;
    }
    clear();
    if (life==0) return 1;
    return 0;
}

int skill3(){
    int moveorstop;
    wchar_t skill3smallstring[] = L"■";
    wchar_t skill3string[50] = L"■";
    for (int i = 1; i <stage3mapmaxx; i++){
        wcscat(skill3string,skill3smallstring);
    }
    srand(time(NULL));
    moveorstop = rand()%2;
    switch (moveorstop){
        case 0 :
            textcolor (12); //red
            break;
        case 1 :
            textcolor (9); //blue
            break;
        case 2 :
            textcolor(15);
            break;
    }
    for (int i = 1; i<=stage3mapmaxy; i++){
        stage3gotoxy(printx+1,printy+i);
        wprintf(L"%s",skill3string);
    }
    textcolor(15);
    stage3gotoxy(stage3x,stage3y);
    printf("@");
    int skill3x = stage3x;
    int skill3y = stage3y;
    skillstart = clock();
    while ((double)(clock() - skillstart) / CLOCKS_PER_SEC <=1){
        phase1move();
        Sleep (30);
    }
    clear();
    if (moveorstop == 1){
        if (skill3x == stage3x && skill3y == stage3y){
            life--;
            Setlife();
        }
    }
    if (moveorstop == 0){
        if (skill3x != stage3x || skill3y != stage3y){
            life--;
            Setlife();
        }
    }
    if (life==0) return 1;
    return 0;
}

void phase1(int dif){
    while ((double)(clock() - phasetime) / CLOCKS_PER_SEC <=10 + 20 * difficulty){
        phase1move();
        Sleep (20);
        if ((double)(clock() - start) / CLOCKS_PER_SEC >=(float)2/difficulty){
            srand(time(NULL));
            int num = rand()%3;
            if (num==0) {
                if (skill1(dif)) break;
            }
            else if (num==1) {
                if (skill2()) break;
            }
            else if (num==2) {
                if (skill3()) break;
            }
            start = clock();
        }
    }
    return;
}

void phase2() {
    int raw = rand()%36;
    while (raw<=stage3mapmaxx || raw%2!=0) {
        raw = rand()%36;
    }
    int col =  rand()%20;
    while (col<=stage3mapmaxy || col%2!=0) {
        col = rand()%20;
    }
    printx -= (raw-stage3mapmaxx)/2;
    printy -= (col-stage3mapmaxy)/2;
    stage3mapmaxx = raw;
    stage3mapmaxy = col;
    system("cls");
    textcolor(15);
    makeclearstring();
    stageprint ();
    Setlife();
    stage3gotoxy (stage3x,stage3y);
    printf ("@");
    return;
}

void phase3 () {
    stage3mapmaxx = 10;
    stage3mapmaxy = 8;
    stage3phase = 0;
    system("cls");
    textcolor(15);
    makeclearstring();
    stageprint ();
    Setlife();
    stage3x = printx + stage3mapmaxx/2;
    stage3y = printy + stage3mapmaxy/2;
    stage3gotoxy (stage3x,stage3y);
    printf ("@");
    clock_t object = clock();
    clock_t stage3phase3start = clock();
    while ((double)(clock() - stage3phase3start) / CLOCKS_PER_SEC <= 10 + 15 * difficulty){
        if ((double)(clock() - object) / CLOCKS_PER_SEC >1){
            srand(time(NULL));
            switch (rand()%2)
            {
            case 0:
                _beginthread(colobject, 0,(void *)1);
                break;
            case 1:
                _beginthread(rawobject, 0,(void *)1);
            default:
                break;
            }
            object = clock();
        }
        if (threadId == 0) {
            threadId = _beginthread(phase1move, 0,(void *)1);
            WaitForSingleObject(threadId,INFINITE);
            threadId = 0;
        }
        Sleep(50);
        if (life <= 0) {
            stage3phase = 1;
            return;
        }
    }
    stage3phase = 1;
    return;
}

void colobject (void * n) {
    int x = printx+stage3mapmaxx;
    int y = printy+1;
    int tmpprintx = printx;
    srand(time(NULL));
    int tmpy = rand()%stage3mapmaxy;
    int tmpprinty = printy;
    y+=tmpy;
    clock_t startcolobject;
    while (x>tmpprintx){
        while (1) if (threadId == 0) {
            threadId = 1;
            stage3gotoxy(x,y);
            printf("*");
            threadId = 0;
            break;
        }
        startcolobject = clock();
        while ((double)(clock() - startcolobject) / CLOCKS_PER_SEC <=1){
            if (x == stage3x && y == stage3y) {
                life--;
                while (1) if (threadId == 0) { Setlife(); return;}
            }
        }
        while (1) if (threadId == 0) {
            threadId = 1;
            stage3gotoxy(x,y);
            printf(" ");
            threadId = 0;
            break;
        }
        if (stage3phase){
            while (1) if (threadId == 0) {
                threadId = 1;
                stage3gotoxy(x,y);
                printf(" ");
                threadId = 0;
                break;
            }
            return;
        }
        x--;
    }
    return;
}

void rawobject (void * n) {
    int x = printx+1;
    int y = printy+stage3mapmaxy;
    int tmpprintx = printx;
    srand(time(NULL));
    int tmpx = rand()%stage3mapmaxx;
    int tmpprinty = printy;
    x+=tmpx;
    clock_t startcolobject;
    while (y>tmpprinty){
        while (1) if (threadId == 0) {
            threadId = 1;
            stage3gotoxy(x,y);
            printf("*");
            threadId = 0;
            break;
        }
        startcolobject = clock();
        while ((double)(clock() - startcolobject) / CLOCKS_PER_SEC <=1){
            if (x == stage3x && y == stage3y) {
                life--;
                while (1) if (threadId == 0) { Setlife(); return;}
            }
        }
        while (1) if (threadId == 0) {
            threadId = 1;
            stage3gotoxy(x,y);
            printf(" ");
            threadId = 0;
            break;
        }
        if (stage3phase){
            while (1) if (threadId == 0) {
                threadId = 1;
                stage3gotoxy(x,y);
                printf(" ");
                threadId = 0;
                break;
            }
            return;
        }
        y--;
    }
    return;
}

char *end(int phase){
    system ("cls");
    stage3gotoxy(30,10);
    if (phase == 3) printf("Game Clear");
    else printf("Game End");
    stage3gotoxy(30,11);
    printf ("Press Enter to Return");
    setlocale( LC_ALL, "English" );
    while (1) {
        if (GetAsyncKeyState(VK_RETURN)) break;
    }
    return Achivement (phase);
}

char *Achivement (int phase){
    if (phase >= 1){
        stage3achieve[0] = 'O';
    }
    if (phase >= 2){
        stage3achieve[1] = 'O';
    }
    if (phase >= 3){
        stage3achieve[2] = 'O';
    }
    if (life == 3 && phase == 3){
        stage3achieve[3] = 'O';
    }
    if (difficulty == 3 && phase >= 3) {
        stage3achieve[4] = 'O';
    }
    return stage3achieve;
}

void stage3gotoxy(int x, int y){
    COORD pos = {x-1, y-1};
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), pos);
}