#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>
#include "Stage.h"
 
#define LEFT 75 // 좌측 이동
#define RIGHT 77 // 우측 이동
#define UP 72 // 회전 (블록 방향)
#define DOWN 80 // 약한 하강
#define SPACE 32 // 강한 하강
#define p 112 // 일시 정지 
#define P 80 // 일시 정지 
#define ESC 27 // 게임 종료
 
#define false 0
#define true 1
 
#define ACTIVE_BLOCK -2 // 블록 상태 (회전에 따른 분류)
#define CEILLING -1 // 블록이 움직일 수 없는 공간 (천장)    
#define EMPTY 0 // 블록이 움직일 수 있는 공간        
#define WALL 1 // 블록이 움직일 수 없는 공간 (벽)
#define INACTIVE_BLOCK 2 // 고정된 블록 (바닥에 쌓여 조작 불가능한 블록) 
 
#define MAIN_X 11 // 보드 수평 길이 
#define MAIN_Y 23 // 보드 수직 길이
#define MAIN_X_ADJ 3 // 보드 위치 고정 X 좌표 
#define MAIN_Y_ADJ 1 // 보드 위치 고정 Y 좌표
 
#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 // 정보 표시 구역 위치 좌표 
 
int STATUS_Y_GOAL; // Goal (다음 Lv 로 넘어가기까지 남은 조건) 표시 위치 Y 좌표 
int STATUS_Y_LEVEL; // Lv 표시 위치 Y 좌표
int STATUS_Y_SCORE; // Score 표시 위치 Y 좌표
char stage2achievement [4] = {'X','X','X','\0'}; // 업적 상태 저장 배열
 
int blocks[7][4][4][4]={
{{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
 {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}},
{{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}},
{{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0},
 {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0}},
{{0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0},{0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0}},
{{0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0},{0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0},{0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}}
}; // 블록 모양 저장 blocks[블록 종류][블록 회전][i][j]
 
int b_type; // 블록 종류
int b_rotation; // 블록 회전 (에 따른 모양 저장) 
int b_type_next; // 다음 블록 저장
int b_dummy; // 가짜 다음 블록 (난이도 2)
 
int main_org[MAIN_Y][MAIN_X]; // 게임판 정보 저장  
int main_cpy[MAIN_Y][MAIN_X]; // 출력되기 전의 게임판 정보 (org 과 비교하여 변경점만 출력하도록 설정) 
                              
int bx,by; // 조작하는 블록의 X, Y 좌표
 
int key; // 입력 받은 값 저장

int step; // 난이도 (메인 스테이지에서 입력 받은 숫자 - 1, 2, 3 저장)
 
int speed; // 블록 하강 속도
int level; // 현재 Lv
int level_goal; // 다음 레벨로 넘어가기 위한 조건 
int cnt; // 현재 Lv 에서 제거한 줄 수 
int score; // 현재 점수
int last_score=0; // 마지막으로 플레이한 게임 점수
int best_score=0; // 최고 점수
 
int new_block_on=0; 
int crush_on=0;
int level_up_on=0; 
int space_key_on=0; 

 
void gotoxy(int x,int y) { 
    COORD pos={2*x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
 
char *stage2(int diffi){
    step = diffi;
    int i;
    setlocale(LC_ALL,"");
    srand((unsigned)time(NULL)); 
    title(); 
    reset(); 
    
    while(1){
        for(i=0;i<5;i++){  
            if (check_key()) return stage2achievement; 
            draw_main(); 
            Sleep(speed); 
            if(crush_on&&check_crush(bx,by+1, b_rotation)==false) Sleep(100);
                                                 
            if(space_key_on==1) { 
                space_key_on=0;
                break;
            }
        }
        drop_block(); 
        check_level_up(); 
        if (check_game_over()) return stage2achievement;
        if(new_block_on==1) new_block(); 
    }
}
 
void title(void){
int x=5;  
int y=4; 
int cnt;   
    
    gotoxy(x+10,y+2);printf("T   E   T   R   I   S");
    gotoxy(x+10,y+7);printf("Press Enter to Start.");
    gotoxy(x+10,y+9); wprintf(L"  △   : Shift");     
    gotoxy(x+10,y+10); wprintf(L"◁  ▷ : Left / Right");     
    gotoxy(x+10,y+11); wprintf(L"  ▽   : Soft Drop");
    gotoxy(x+10,y+12); printf(" SPACE : Hard Drop"); 
    gotoxy(x+10,y+13); printf("   P   : Pause"); 
    gotoxy(x+10,y+14); printf("  ESC  : Quit");  
    gotoxy(x+10,y+16); printf("BONUS FOR HARD DROPS / COMBOS"); 

    while (1) {
        if (GetAsyncKeyState(VK_RETURN)) {
            Sleep(100);
            break;
        }
    }
 
}
 
void reset(void){
 
    FILE *file=fopen("score.dat", "rt");  
    if(file==0){best_score=0;} 
    else {
        fscanf(file,"%d", &best_score); 
        fclose(file);  
    }
        
    level=1; 
    score=0;
    level_goal=1000;
    key=0;
    crush_on=0;
    cnt=0;
    speed=100;
    
    system("cls"); 
    reset_main();  
    draw_map(); 
    draw_main(); 
 
    b_type_next=rand()%7;  
    new_block();   
    
}
 
void reset_main(void){  
    int i,j;
    
    for(i=0;i<MAIN_Y;i++){   
        for(j=0;j<MAIN_X;j++){
            main_org[i][j]=0;
            main_cpy[i][j]=100;
        }
    }    
    for(j=1;j<MAIN_X;j++){  
        main_org[3][j]=CEILLING;
    }
    for(i=1;i<MAIN_Y-1;i++){   
        main_org[i][0]=WALL;
        main_org[i][MAIN_X-1]=WALL;
    }
    for(j=0;j<MAIN_X;j++){  
        main_org[MAIN_Y-1][j]=WALL;
    }
}
 
void reset_main_cpy(void){  
    int i, j;
    
    for(i=0;i<MAIN_Y;i++){          
        for(j=0;j<MAIN_X;j++){   
            main_cpy[i][j]=100;
        }
    }    
}
 
void draw_map(void){ 
int y=3;             
                    
    gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL=y); printf(" LEVEL : %5d", level); 
    gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL=y+1); printf(" GOAL  : %5d", 10-cnt);

    switch (step) {
        case 2 :
            gotoxy(STATUS_X_ADJ, y+2); printf("+--------  N E X T  --------+ ");
            gotoxy(STATUS_X_ADJ, y+3); printf("|                           | ");
            gotoxy(STATUS_X_ADJ, y+4); printf("|                           | ");
            gotoxy(STATUS_X_ADJ, y+5); printf("|                           | ");
            gotoxy(STATUS_X_ADJ, y+6); printf("|                           | ");
            gotoxy(STATUS_X_ADJ, y+7); printf("+--------  -  -  -  --------+ "); 
            break;
        default :
            gotoxy(STATUS_X_ADJ, y+2); printf("+-  N E X T  -+ ");
            gotoxy(STATUS_X_ADJ, y+3); printf("|             | ");
            gotoxy(STATUS_X_ADJ, y+4); printf("|             | ");
            gotoxy(STATUS_X_ADJ, y+5); printf("|             | ");
            gotoxy(STATUS_X_ADJ, y+6); printf("|             | ");
            gotoxy(STATUS_X_ADJ, y+7); printf("+-- -  -  - --+ "); 
            break;
    };

    gotoxy(STATUS_X_ADJ, y+8); printf(" YOUR SCORE :");     
    gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE=y+9); printf("        %6d", score); 
    gotoxy(STATUS_X_ADJ, y+10); printf(" LAST SCORE :");     
    gotoxy(STATUS_X_ADJ, y+11); printf("        %6d", last_score);     
    gotoxy(STATUS_X_ADJ, y+12); printf(" BEST SCORE :");     
    gotoxy(STATUS_X_ADJ, y+13); printf("        %6d", best_score);     
    gotoxy(STATUS_X_ADJ, y+15); wprintf(L"  △   : Shift        SPACE : Hard Drop");     
    gotoxy(STATUS_X_ADJ, y+16); wprintf(L"◁  ▷ : Left / Right   P   : Pause");     
    gotoxy(STATUS_X_ADJ, y+17); wprintf(L"  ▽   : Soft Drop     ESC  : Quit");
}
 
void draw_main(void){ 
    int i, j;    
    
    for(j=1;j<MAIN_X-1;j++){  
        if(main_org[3][j]==EMPTY) main_org[3][j]=CEILLING;
    }
    for(i=0;i<MAIN_Y;i++){
        for(j=0;j<MAIN_X;j++){
            if(main_cpy[i][j]!=main_org[i][j]){ 
                                                
            gotoxy(MAIN_X_ADJ+j,MAIN_Y_ADJ+i); 
                switch(main_org[i][j]){
                    case EMPTY:  
                        printf("  ");
                        break;
                    case CEILLING: 
                        printf(". ");
                        break;
                    case WALL:  
                        wprintf(L"▩"); 
                        break;
                    case INACTIVE_BLOCK:   
                        wprintf(L"□");
                        break;
                    case ACTIVE_BLOCK:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), b_type + 1);  
                        wprintf(L"■");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                        break;    
                }    
            }
        }
    }
    for(i=0;i<MAIN_Y;i++){   
        for(j=0;j<MAIN_X;j++){
            main_cpy[i][j]=main_org[i][j];
        }
    }
}
 
void new_block(void){   
    int i, j;    
    
    bx=(MAIN_X/2)-1; 
    by=0;   
    b_type=b_type_next;  
    b_type_next=rand()%7;  
    b_rotation=0;   
    
    new_block_on=0;   
    
    for(i=0;i<4;i++){   
        for(j=0;j<4;j++){
            if(blocks[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=ACTIVE_BLOCK;
        }
    }
    switch (step) {
        case 1:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), b_type_next + 1);
            for(i=1;i<3;i++){ 
                for(j=0;j<4;j++){
                    if(blocks[b_type_next][0][i][j]==1) {
                        gotoxy(STATUS_X_ADJ+2+j,i+6);
                        wprintf(L"■");
                    }
                    else{
                        gotoxy(STATUS_X_ADJ+2+j,i+6);
                        printf("  ");
                    }
                }
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            break;
        case 2:
            b_dummy=rand()%7;
   
            if (b_type_next > b_dummy) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), b_type_next + 1);
                for(i=1;i<3;i++){ 
                    for(j=0;j<4;j++){
                        if(blocks[b_type_next][0][i][j]==1) {
                            gotoxy(STATUS_X_ADJ+2+j,i+6);
                            wprintf(L"■");
                        }
                        else{
                            gotoxy(STATUS_X_ADJ+2+j,i+6);
                            printf("  ");
                        }
                    }
                }
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
     
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), b_dummy + 1);
                for(i=1;i<3;i++){ 
                    for(j=0;j<4;j++){
                        if(blocks[b_dummy][0][i][j]==1) {
                            gotoxy(STATUS_X_ADJ+10+j,i+6);
                            wprintf(L"■");
                        }
                        else{
                            gotoxy(STATUS_X_ADJ+10+j,i+6);
                            printf("  ");
                        }
                    }
                }
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), b_type_next + 1);
                for(i=1;i<3;i++){ 
                    for(j=0;j<4;j++){
                        if(blocks[b_type_next][0][i][j]==1) {
                            gotoxy(STATUS_X_ADJ+10+j,i+6);
                            wprintf(L"■");
                        }
                        else{
                            gotoxy(STATUS_X_ADJ+10+j,i+6);
                            printf("  ");
                        }
                    }
                }
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
     
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), b_dummy + 1);
                for(i=1;i<3;i++){ 
                    for(j=0;j<4;j++){
                        if(blocks[b_dummy][0][i][j]==1) {
                            gotoxy(STATUS_X_ADJ+2+j,i+6);
                            wprintf(L"■");
                        }
                        else{
                            gotoxy(STATUS_X_ADJ+2+j,i+6);
                            printf("  ");
                        }
                    }
                }
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            break;
        case 3:
            break;
    };
}
 
int check_key(void){
    key=0;   
    if (GetAsyncKeyState(VK_LEFT) && check_crush(bx-1,by,b_rotation)==true)move_block(LEFT); 
    else if (GetAsyncKeyState(VK_RIGHT) && check_crush(bx+1,by,b_rotation)==true)move_block(RIGHT);
    else if (GetAsyncKeyState(VK_DOWN) && check_crush(bx,by+1,b_rotation)==true)move_block(DOWN);
    else if (GetAsyncKeyState(VK_UP)){
        if(check_crush(bx,by,(b_rotation+1)%4)==true) move_block(UP);             
        else if(crush_on==1&&check_crush(bx,by-1,(b_rotation+1)%4)==true) move_block(100);   
    }
    else if (GetAsyncKeyState(VK_SPACE)){
        space_key_on=1; 
        while(crush_on==0){  
            drop_block();
            score+=level;
            gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);   
        }
    }
    else if (GetAsyncKeyState(0x50))pause(); 
    else if (GetAsyncKeyState(VK_ESCAPE)) {
        system("cls");  
        return 1;
    }
    return 0;
}
 
void drop_block(void){
    int i,j;
    
    if(crush_on&&check_crush(bx,by+1, b_rotation)==true) crush_on=0;  
    if(crush_on&&check_crush(bx,by+1, b_rotation)==false){ 
        for(i=0;i<MAIN_Y;i++){  
            for(j=0;j<MAIN_X;j++){
                if(main_org[i][j]==ACTIVE_BLOCK) main_org[i][j]=INACTIVE_BLOCK;
            }
        }
        crush_on=0; 
        check_line();  
        new_block_on=1;    
        return; 
    }
    if(check_crush(bx,by+1, b_rotation)==true) move_block(DOWN);  
    if(check_crush(bx,by+1, b_rotation)==false) crush_on++; 
}
 
int check_crush(int bx, int by, int b_rotation){  
    int i,j;
    
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){ 
            if(blocks[b_type][b_rotation][i][j]==1&&main_org[by+i][bx+j]>0) return false;
        }
    }    
    return true;  
};
 
void move_block(int dir){ 
    int i,j;
 
    switch(dir){
        case LEFT:  
            for(i=0;i<4;i++){  
                for(j=0;j<4;j++){
                    if(blocks[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=EMPTY;
                }
            }
            for(i=0;i<4;i++){  
                for(j=0;j<4;j++){
                    if(blocks[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j-1]=ACTIVE_BLOCK;
                }
            }
            bx--;  
            break;
        
        case RIGHT:    
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=EMPTY;
                }
            }
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j+1]=ACTIVE_BLOCK;
                }
            }
            bx++;        
            break;
        
        case DOWN:    
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=EMPTY;
                }
            }
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[b_type][b_rotation][i][j]==1) main_org[by+i+1][bx+j]=ACTIVE_BLOCK;
                }
            }
            by++;        
            break;
        
        case UP:  
            for(i=0;i<4;i++){ 
                for(j=0;j<4;j++){
                    if(blocks[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=EMPTY;
                }
            }
            b_rotation=(b_rotation+1)%4;
            for(i=0;i<4;i++){  
                for(j=0;j<4;j++){
                    if(blocks[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=ACTIVE_BLOCK;
                }
            }
            break;
            
        case 100:  
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[b_type][b_rotation][i][j]==1) main_org[by+i][bx+j]=EMPTY;
                }
            }
            b_rotation=(b_rotation+1)%4;
            for(i=0;i<4;i++){
                for(j=0;j<4;j++){
                    if(blocks[b_type][b_rotation][i][j]==1) main_org[by+i-1][bx+j]=ACTIVE_BLOCK;
                }
            }
            by--;
            break;
    }
}
 
void check_line(void){
    int i, j, k, l;
    
    int    block_amount; 
    int combo=0;  
    
    for(i=MAIN_Y-2;i>3;){  
        block_amount=0;
        for(j=1;j<MAIN_X-1;j++){ 
            if(main_org[i][j]>0) block_amount++;
        }
        if(block_amount==MAIN_X-2){ 
            if(level_up_on==0){  
                score+=100*level;  
                cnt++;  
                combo++;   
            }
            for(k=i;k>1;k--){ 
                for(l=1;l<MAIN_X-1;l++){
                    if(main_org[k-1][l]!=CEILLING) main_org[k][l]=main_org[k-1][l];
                    if(main_org[k-1][l]==CEILLING) main_org[k][l]=EMPTY; 

                }
            }
        }
        else i--;
    }
    if(combo){   
        if(combo>1){ 
            gotoxy(MAIN_X_ADJ+(MAIN_X/2)-1,MAIN_Y_ADJ+by-2);printf("%d COMBO!",combo);
            Sleep(500);
            score+=(combo*level*100);
            reset_main_cpy();  
        }
        gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", (cnt<=10)?10-cnt:0);
        gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);    
    }
}
 
void check_level_up(void){
    int i, j;
    
    if(cnt>=10){  
        draw_main();
        level_up_on=1; 
        level+=1; 
        cnt=0;    
    
        for(i=0;i<4;i++){
            gotoxy(MAIN_X_ADJ+(MAIN_X/2)-3,MAIN_Y_ADJ+4);
            printf("             ");
            gotoxy(MAIN_X_ADJ+(MAIN_X/2)-2,MAIN_Y_ADJ+6);
            printf("             ");
            Sleep(200);
            
            gotoxy(MAIN_X_ADJ+(MAIN_X/2)-3,MAIN_Y_ADJ+4);
            wprintf(L"☆LEVEL UP!☆");
            gotoxy(MAIN_X_ADJ+(MAIN_X/2)-2,MAIN_Y_ADJ+6);
            wprintf(L"☆SPEED UP!☆");
            Sleep(200);
        }
        reset_main_cpy(); 
       
        switch(level){  
            case 2:
                speed=50;
                break;
            case 3:
                speed=25;
                break;
            case 4:
                speed=10;
                break;
            case 5:
                speed=5;
                break;
            case 6:
                speed=4;
                break;
            case 7:
                speed=3;
                break;
            case 8:
                speed=2;
                break;
            case 9:
                speed=1;
                break;
            case 10:
                speed=0;
                break;
        }    
        level_up_on=0; 
        
        gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level); 
        gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10-cnt); 
    
    }
}
 
int check_game_over(void){ 
    int i;
    
    int x=5;
    int y=5;
    
    for(i=1;i<MAIN_X-2;i++){
        if(main_org[3][i]>0){ 
            gotoxy(x,y+0); wprintf(L"▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");   
            gotoxy(x,y+1); wprintf(L"▤                              ▤");
            gotoxy(x,y+2); wprintf(L"▤  +-----------------------+   ▤");
            gotoxy(x,y+3); wprintf(L"▤  |  G A M E  O V E R..   |   ▤");
            gotoxy(x,y+4); wprintf(L"▤  +-----------------------+   ▤");
            gotoxy(x,y+5); wprintf(L"▤   YOUR SCORE: %6d         ▤",score);
            gotoxy(x,y+6); wprintf(L"▤                              ▤");
            gotoxy(x,y+7); wprintf(L"▤  Press Enter to restart..    ▤");
            gotoxy(x,y+8); wprintf(L"▤                              ▤");
            gotoxy(x,y+9); wprintf(L"▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
            last_score=score;  
            
            if(score>best_score){ 
                FILE* file=fopen("score.dat", "wt");                 
                
                gotoxy(x,y+6); wprintf(L"▤  ★★★ BEST SCORE! ★★★   ▤  ");
 
                if(file==0){  
                    gotoxy(0,0); 
                    printf("FILE ERROR: SYSTEM CANNOT WRITE BEST SCORE ON \"SCORE.DAT\"");
                }
                else{
                    fprintf(file,"%d", score);
                    fclose(file);
                }
            }
            while (1) {
                if (GetAsyncKeyState(VK_RETURN)) {
                    Sleep(100);
                    return 1;
                }
            }
        }
    }
    return 0;
}
 
void pause(void){ 
    int i,j;
    system("cls");
    int x=5;
    int y=5;
    
    for(i=1;i<MAIN_X-2;i++) { 
            gotoxy(x,y+0); wprintf(L"▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
            gotoxy(x,y+1); wprintf(L"▤                              ▤");
            gotoxy(x,y+2); wprintf(L"▤  +-----------------------+   ▤");
            gotoxy(x,y+3); wprintf(L"▤  |       P A U S E       |   ▤");
            gotoxy(x,y+4); wprintf(L"▤  +-----------------------+   ▤");
            gotoxy(x,y+5); wprintf(L"▤  Press Enter to resume..     ▤");
            gotoxy(x,y+6); wprintf(L"▤                              ▤");
            gotoxy(x,y+7); wprintf(L"▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
    }

    while (1) {
        if (GetAsyncKeyState(VK_RETURN)) {
            Sleep(100);
            break;
        }
    }

    system("cls"); 
    reset_main_cpy();  
    draw_main();
    draw_map(); 
 
    if (b_type_next > b_dummy) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), b_type_next + 1);
        for(i=1;i<3;i++){ 
            for(j=0;j<4;j++){
                if(blocks[b_type_next][0][i][j]==1) {
                    gotoxy(STATUS_X_ADJ+2+j,i+6);
                    wprintf(L"■");
                }
                else{
                    gotoxy(STATUS_X_ADJ+2+j,i+6);
                    printf("  ");
                }
            }
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
     
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), b_dummy + 1);
        for(i=1;i<3;i++){ 
            for(j=0;j<4;j++){
                if(blocks[b_dummy][0][i][j]==1) {
                    gotoxy(STATUS_X_ADJ+10+j,i+6);
                    wprintf(L"■");
                }
                else{
                    gotoxy(STATUS_X_ADJ+10+j,i+6);
                    printf("  ");
                }
            }
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
    else {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), b_type_next + 1);
        for(i=1;i<3;i++){ 
            for(j=0;j<4;j++){
                if(blocks[b_type_next][0][i][j]==1) {
                    gotoxy(STATUS_X_ADJ+10+j,i+6);
                    wprintf(L"■");
                }
                else{
                    gotoxy(STATUS_X_ADJ+10+j,i+6);
                    printf("  ");
                }
            }
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
     
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), b_dummy + 1);
        for(i=1;i<3;i++){ 
            for(j=0;j<4;j++){
                if(blocks[b_dummy][0][i][j]==1) {
                    gotoxy(STATUS_X_ADJ+2+j,i+6);
                    wprintf(L"■");
                }
                else{
                    gotoxy(STATUS_X_ADJ+2+j,i+6);
                    printf("  ");
                }
            }
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);   
    }
}

