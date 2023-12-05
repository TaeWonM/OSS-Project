#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <locale.h>
#include "Stage.h"
#include <process.h>

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
int watchend = 1;
int cooldown = 1;
char stage3achi [6] = {'X','X','X','X','X','\0'};
char stage2achi [4] = {'X','X','X','\0'};
char stage1achi [4] = {'X','X','X','\0'};
char clearstage3achi [6] = {'O','O','O','O','O','\0'};
char clearstage2achi [5] = {'O','O','O','\0'};
char clearstage1achi [4] = {'O','O','O','\0'};
char character = 'W';
int charactnum = 0;
char characters[4] = {'W','e','L','I'};
HANDLE starthThread;
DWORD startthreadId;

void mainstagegotoxy(int x, int y); // 커서 이동 함수
void Filescan (); // 메인 스테이지 맵 읽기 함수
int Choose_1to3 (); // 1-3의 입력을 받는 함수
void save(); // save.txt 파일 출력 함수
void saveread(); // save.txt 파일 읽기 함수
void stage3Achievementprint (); // stage 3 업적 출력 함수
void stage2Achievementprint (); // stage 2 업적 출력 함수
void stage1Achievementprint (); // stage 1 업적 출력 함수
void characterselect (); // 캐릭터 변경 화면 출력
void startscreen(); // 시작 화면 출력 함수
void endscreen(); // 엔딩 크레딧 출력 함수
void Enterscreen(void *); // 깜빡이는 스크린
void Cooldownset(void *); // stage 들어가는 쿨타임 설정

int main () {
    Filescan ();
    saveread();
    setlocale(LC_ALL,"");
    startscreen();
    setlocale(LC_ALL,"C");
    system("cls");
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
        if (GetAsyncKeyState(VK_UP) && prey>1 && map[prey][prex-1+MOVE] != ' '){
            yv-=2;
        }
        else if (GetAsyncKeyState(VK_DOWN) && map[prey][prex-1+MOVE]!=' ' && map[prey][prex-1+MOVE]!='=' && cooldown == 1){
            system ("cls");
            mainstagegotoxy(30,10);
            printf ("Press dificulty 1~3");
            int diffi = Choose_1to3 ();
            switch (map[prey][prex-1+MOVE])
            {
            case '1':
                system("cls");
                char stage1tmp[4];
                strcpy(stage1tmp,stage1(diffi));
                for (int i = 0; i < 3; i++) if (stage1tmp[i]=='O') stage1achi[i] = 'O';
                system("cls");
                break;
            case '2':
                system("cls");
                stage2(diffi);
                system("cls");
                break;
            case '3':
                system("cls");
                char stage3tmp[6];
                strcpy(stage3tmp,stage3(diffi));
                for (int i = 0; i < 5; i++) if (stage3tmp[i]=='O') stage3achi[i] = 'O';
                system("cls");
                break;
            default:
                break;
            }
            setlocale(LC_ALL,"C");
            if (watchend == 1 && strcmp(stage1achi,clearstage1achi) == 0 && strcmp(stage2achi,clearstage2achi) == 0 && strcmp(stage3achi,clearstage3achi) == 0 ){
                setlocale(LC_ALL,"");
                Sleep(50);
                endscreen();
                Sleep(3000);
                setlocale(LC_ALL,"C");
                watchend = 0;
                system("cls");
            }
            _beginthread(Cooldownset, 0,(void *)1);
            Sleep(100);
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
            case 2: 
                save();
                break;
            case 3:
                characterselect();
                break;
            };
        }
        Sleep(20);
        if (map[prey][prex-1+MOVE] != '=' && map[prey][prex-1+MOVE] != ' '){
            if (map[prey][prex-1+MOVE] == '1'){
                mainstagegotoxy(80,5);
                printf ("Stage 1 : Pac-Man");
                mainstagegotoxy(80,7);
                printf ("Press Down");
                stage1Achievementprint();
            }
            else if (map[prey][prex-1+MOVE] == '2'){
                mainstagegotoxy(80,5);
                printf ("Stage 2 : Tetris");
                mainstagegotoxy(80,7);
                printf ("Press Down");
                stage2Achievementprint();
            }
            else if (map[prey][prex-1+MOVE] == '3'){
                mainstagegotoxy(80,5);
                printf ("Stage 3 : Untertail");
                mainstagegotoxy(80,7);
                printf ("Press Down");
                stage3Achievementprint();
                
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

void save(){
    fp = fopen("save.txt","w");
    fprintf (fp,"%s\n",stage1achi);
    fprintf (fp,"%s\n",stage2achi);
    fprintf (fp,"%s\n",stage3achi);
    fclose(fp);
}

void saveread(){
    fp = fopen("save.txt","r");
    fscanf(fp,"%s",stage1achi);
    fscanf(fp,"%s",stage2achi);
    fscanf(fp,"%s",stage3achi);
    fclose(fp);
}

void stage3Achievementprint (){
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

void stage2Achievementprint (){
    mainstagegotoxy(80,9);
    if (stage2achi[0]=='O'){
        printf("1 : Block World...");
        mainstagegotoxy(80,10);
        printf("Condition: Difficulty 1 passed");
    }
    else {
        printf("1 : ???");
    }
    mainstagegotoxy(80,11);
    if (stage2achi[1]=='O'){
        printf("2 : I guess I can see something in vain..");
        mainstagegotoxy(80,12);
        printf("Condition: Difficulty 2 passed");
    }
    else {
        printf("2 : ???");
    }
    mainstagegotoxy(80,13);
    if (stage2achi[2]=='O'){
        printf("3 : I can't see it anymore..");
        mainstagegotoxy(80,14);
        printf("Condition: Difficulty 3 passed");
    }
    else {
        printf("3 : ???");
    }
}
void stage1Achievementprint (){
    mainstagegotoxy(80,9);
    if (stage1achi[0]=='O'){
        printf("1 : First Start");
        mainstagegotoxy(80,10);
        printf("Condition: Difficulty 1 passed");
    }
    else {
        printf("1 : ???");
    }
    mainstagegotoxy(80,11);
    if (stage1achi[1]=='O'){
        printf("2 : An Unexpected Move");
        mainstagegotoxy(80,12);
        printf("Condition: Difficulty 2 passed");
    }
    else {
        printf("2 : ???");
    }
    mainstagegotoxy(80,13);
    if (stage1achi[2]=='O'){
        printf("3 : Busy. Busy. Modern society");
        mainstagegotoxy(80,14);
        printf("Condition: Difficulty 3 passed");
    }
    else {
        printf("3 : ???");
    }
}

void characterselect(){
    while(1){
        system("cls");
        mainstagegotoxy(47,4);
        printf("%d",charactnum+1);
        mainstagegotoxy(42,5);
        printf("===========");
        mainstagegotoxy(47,7);
        printf("%c",characters[charactnum]);
        mainstagegotoxy(42,9);
        printf("===========");
        if (charactnum==0){
            mainstagegotoxy(35,11);
            printf ("Character : Original Character");
            mainstagegotoxy(37,12);
            printf ("Feature : Quite Practical");
            mainstagegotoxy(36,14);
            printf ("Condition : No Condition");
            mainstagegotoxy(54,7);
            printf(">");
        }
        else if (charactnum==1 && stage1achi[2] == 'O'){
            mainstagegotoxy(35,11);
            printf ("Character : Pac-man Character");
            mainstagegotoxy(15,12);
            printf ("Feature : It reminds me of a certain alphabet, but it's Pac-Man anyway");
            mainstagegotoxy(28,14);
            printf ("Condition : Passed Stage 1, Difficulty 3");
            mainstagegotoxy(54,7);
            printf(">");
            mainstagegotoxy(37,7);
            printf("<");
        }
        else if (charactnum==1 && stage1achi[2] != 'O'){
            mainstagegotoxy(35,11);
            printf ("Character : ???");
            mainstagegotoxy(37,12);
            printf ("Feature : ???");
            mainstagegotoxy(54,7);
            printf(">");
            mainstagegotoxy(37,7);
            printf("<");
            mainstagegotoxy (47,7);
            printf("?");
        }
        else if (charactnum==2 && stage2achi[3] == 'O'){
            mainstagegotoxy(35,11);
            printf ("Character : Tetreis Character");
            mainstagegotoxy(15,12);
            printf ("Feature : The block that reminds me of Tetris... No, it's a character.");
            mainstagegotoxy(28,14);
            printf ("Condition : Passed Stage 2, Difficulty 3");
            mainstagegotoxy(54,7);
            printf(">");
            mainstagegotoxy(37,7);
            printf("<");
        }
        else if (charactnum==2 && stage2achi[3] != 'O'){
            mainstagegotoxy(35,11);
            printf ("Character : ???");
            mainstagegotoxy(37,12);
            printf ("Feature : ???");
            mainstagegotoxy(54,7);
            printf(">");
            mainstagegotoxy(37,7);
            printf("<");
            mainstagegotoxy (47,7);
            printf("?");
        }
        else if (charactnum==3 && stage3achi[4] == 'O'){
            mainstagegotoxy(35,11);
            printf ("Character : Tetreis Character");
            mainstagegotoxy(15,12);
            printf ("Feature : A bone that reminds me of Sans... No, it's a character...");
            mainstagegotoxy(28,14);
            printf ("Condition : Passed Stage 3, Difficulty 5");
            mainstagegotoxy(37,7);
            printf("<");
        }
        else if (charactnum==3 && stage3achi[4] != 'O'){
            mainstagegotoxy(35,11);
            printf ("Character : ???");
            mainstagegotoxy(37,12);
            printf ("Feature : ???");
            mainstagegotoxy(37,7);
            printf("<");
            mainstagegotoxy (47,7);
            printf("?");
        }
        Sleep(500);
        while (1){
            if (GetAsyncKeyState(VK_RETURN)) {
                if ((charactnum==3 && stage2achi[4] == 'O')||(charactnum==2 && stage2achi[3] == 'O')||(charactnum==1 && stage1achi[2] == 'O')||charactnum==0){
                character = characters[charactnum];
                system("cls");
                return;
                }
                
            }
            else if (GetAsyncKeyState(VK_LEFT) && charactnum>0) {
                charactnum--;
                break;
            }
            else if (GetAsyncKeyState(VK_RIGHT) && charactnum<3) {
                charactnum++;
                break;
            }
        }
    }
}

void startscreen(){
    for (int i = 20; i<=70; i++){
        mainstagegotoxy(i,5);
        wprintf(L"■");
    }
    for (int i = 6; i<=16; i++){
        mainstagegotoxy(20,i);
        wprintf(L"■");
        mainstagegotoxy(70,i);
        wprintf(L"■");
    }
    for (int i = 20; i<=70; i++){
        mainstagegotoxy(i,17);
        wprintf(L"■");
    }
    mainstagegotoxy(40,10);
    wprintf(L"오락실 게임");
    starthThread = _beginthread(Enterscreen, 0,(void *)1);
    Sleep(50);
    mainstagegotoxy(34,12);
    printf("Made by CBNU OSS class");
    while (1) {
        if (GetAsyncKeyState(VK_RETURN)) break;
    }
    TerminateThread(starthThread,startthreadId);
    return;
}

void Enterscreen(void * n){
    while (1){
    mainstagegotoxy(35,11);
    printf("Press Enter to start");
    Sleep(500);
    mainstagegotoxy(35,11);
    printf("                       ");
    Sleep(500);
    }
}

void endscreen(){
    for (int i = 20; i<=70; i++){
        mainstagegotoxy(i,5);
        wprintf(L"■");
    }
    for (int i = 6; i<=16; i++){
        mainstagegotoxy(20,i);
        wprintf(L"■");
        mainstagegotoxy(70,i);
        wprintf(L"■");
    }
    for (int i = 20; i<=70; i++){
        mainstagegotoxy(i,17);
        wprintf(L"■");
    }
    mainstagegotoxy(37,10);
    wprintf(L"클리어를 축하합니다");
    mainstagegotoxy(33,11);
    wprintf(L"플레이를 해주셔서 감사합니다");
    mainstagegotoxy(37,12);
    wprintf(L"-수원서 조 드림-");
    return;
}

void Cooldownset(void * n){
    cooldown = 0;
    Sleep(1500);
    cooldown = 1;
    return;
}