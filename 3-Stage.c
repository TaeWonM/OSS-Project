#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <process.h>

float stagexv = 0;
float stageyv = 0;
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
void stageprint ();
void gotoxy(int x, int y);
void Setconsole();
void textcolor (int color);
void clear ();
void phase1move ();
void Setlife ();
void makeclearstring ();
int skill1();
int skill2();
int skill3();
void phase1();
void phase3 ();
void phase2();
void end();
void colobject (void *);
void rawobject ();

int main () {
    HANDLE hThrd;
    DWORD threadId;
    Setconsole();
    Setlife ();
    setlocale(LC_ALL,"");
    textcolor(15);
    makeclearstring();
    stageprint ();
    gotoxy (stage3x,stage3y);
    printf ("@");
    /*start = clock();
    phasetime = start;
    srand(time(NULL));
    phase1();
    if (life<=0){
        end();
        return 0;
    }
    system ("cls");
    start = clock();
    phasetime = start;
    phase2();
    phase1();
    if (life<=0){
        end();
        return 0;
    }*/
    phase3();
}

void gotoxy(int x, int y){
    COORD pos = {x-1, y-1};
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Setconsole() {
    system ("title Stage - 3");
    system ("mode con: cols=50 lines=100");
    CONSOLE_CURSOR_INFO ConsoleCursor;
    ConsoleCursor.bVisible = 1;
    ConsoleCursor.dwSize = 1;
}

void textcolor (int color){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void stageprint () {
    for (int j = 0; j<stage3mapmaxx+2; j++) {
            gotoxy (printx+j,printy);
            wprintf(L"■");
    }
    for (int i = 1; i<stage3mapmaxy+1; i++) {
        for (int j = 0; j<stage3mapmaxx+2; j++) {
            gotoxy (printx+j,printy+i);
            if ( j== 0 || j == stage3mapmaxx+1) wprintf(L"■");
        }
    }
    for (int j = 0; j<stage3mapmaxx+2; j++) {
            gotoxy (printx+j,printy+stage3mapmaxy+1);
            wprintf(L"■");
    }
};

void clear (){
    for (int i = 1; i<=stage3mapmaxy; i++){
        gotoxy(printx+1,printy+i);
        printf("%s",cleartring);
    }
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
        gotoxy(prestage3x,prestage3y);
        printf (" ");
        gotoxy (stage3x,stage3y);
        printf ("@");
    }
    else {
        textcolor(15);
        gotoxy (stage3x,stage3y);
        printf ("@");
    }
};

void Setlife (){
    gotoxy(printx + stage3mapmaxx +2 ,10);
    printf ("Life = %d",life); 
};

void makeclearstring () {
    strcpy(cleartring,smallstring);
    for (int i = 1; i<stage3mapmaxx; i++){
        strcat(cleartring,smallstring);
    }
};

int skill1 () {
        phase1move();
        textcolor(3);
        int col = rand()%(stage3mapmaxy);
        for (int i = 0; i<stage3mapmaxx; i++){
            gotoxy(printx+1+i,printy+1+col);
            printf("-");
        }
        int raw = rand()%(stage3mapmaxx);
        for (int i = 0; i<stage3mapmaxy; i++){
            gotoxy(printx+1+raw,printy+1+i);
            printf("|");
        }
        gotoxy(printx+1+raw,printy+1+col);
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
            gotoxy(printx+1,printy+i);
            wprintf(L"%s",skill2halfstring);
        }
        for (int i = stage3mapmaxy/2+1; i<=stage3mapmaxy; i++){
            gotoxy(printx+1,printy+i);
            wprintf(L"%s",skill2string);
        }
        textcolor(15);
        gotoxy(stage3x,stage3y);
        printf("@");
        skillstart = clock();
        while ((double)(clock() - skillstart) / CLOCKS_PER_SEC <=0.5){
            phase1move();
            Sleep (30);
        }
        if (stage3x < printx + stage3mapmaxx/2 || stage3y > printy + stage3mapmaxy/2){
            life--;
            Setlife();
        }
        break;
    case 2:
        for (int i = 1; i<=stage3mapmaxy/2; i++){
            gotoxy(printx+1,printy+i);
            wprintf(L"%s",skill2string);
        }
        for (int i = stage3mapmaxy/2+1; i<=stage3mapmaxy; i++){
            gotoxy(printx+1,printy+i);
            wprintf(L"%s",skill2halfstring);
        }
        textcolor(15);
        gotoxy(stage3x,stage3y);
        printf("@");
        skillstart = clock();
        while ((double)(clock() - skillstart) / CLOCKS_PER_SEC <=1){
            phase1move();
            Sleep (30);
        }
        if (stage3x < printx + stage3mapmaxx/2 || stage3y < printy + stage3mapmaxy/2){
            life--;
            Setlife();
        }
        break;
    case 3:
        for (int i = 1; i<=stage3mapmaxy/2; i++){
            gotoxy(printx+1+stage3mapmaxx/2,printy+i);
            wprintf(L"%s",skill2halfstring);
        }
        for (int i = stage3mapmaxy/2+1; i<=stage3mapmaxy; i++){
            gotoxy(printx+1,printy+i);
            wprintf(L"%s",skill2string);
        }
        textcolor(15);
        gotoxy(stage3x,stage3y);
        printf("@");
        skillstart = clock();
        while ((double)(clock() - skillstart) / CLOCKS_PER_SEC <=0.5){
            phase1move();
            Sleep (30);
        }
        if (stage3x > printx + stage3mapmaxx/2 || stage3y > printy + stage3mapmaxy/2){
            life--;
            Setlife();
        }
        break;
    case 4:
        for (int i = 1; i<=stage3mapmaxy/2; i++){
            gotoxy(printx+1,printy+i);
            wprintf(L"%s",skill2string);
        }
        for (int i = stage3mapmaxy/2+1; i<=stage3mapmaxy; i++){
            gotoxy(printx+1+stage3mapmaxx/2,printy+i);
            wprintf(L"%s",skill2halfstring);
        }
        textcolor(15);
        gotoxy(stage3x,stage3y);
        printf("@");
        skillstart = clock();
        while ((double)(clock() - skillstart) / CLOCKS_PER_SEC <=0.5){
            phase1move();
            Sleep (30);
        }
        if (stage3x > printx + stage3mapmaxx/2 || stage3y < printy + stage3mapmaxy/2){
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
        gotoxy(printx+1,printy+i);
        wprintf(L"%s",skill3string);
    }
    textcolor(15);
    gotoxy(stage3x,stage3y);
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

void phase1(){
    while ((double)(clock() - phasetime) / CLOCKS_PER_SEC <=30){
        phase1move();
        Sleep (20);
        if ((double)(clock() - start) / CLOCKS_PER_SEC >=1){
            int num = rand()%3;
            if (num==0) {
                if (skill1()) break;
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
    gotoxy (stage3x,stage3y);
    printf ("@");
}



void end(){
    system ("cls");
    gotoxy (50,15);
    printf ("END");
    Sleep(30);
    gotoxy (44,17);
    printf ("Press ESC to out");
    while (1){
        if (GetAsyncKeyState(VK_ESCAPE)) return;
    }
}