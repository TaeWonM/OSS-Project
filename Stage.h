#define MAX_SIZE1 12
#define MAX_SIZE2 15
#define MAX_SIZE3 21

void moveGhost_player(int player_row, int player_col, int ghost_index);
void GotoXY(int x, int y);
void print_mazeGame(char maze[][MAX_SIZE3], int row);
int p_block(char maze[][MAX_SIZE3], int row, int col);
int m_block_p(char maze[][MAX_SIZE3], int row, int col);
void move_maze(char maze[][MAX_SIZE3], int* row, int* col);
void CursorView(char show);
void printTimeElapsed();
int fileopen();
int checkGameOver(int player_row, int player_col);
void initializeGhosts();
char * stage1 (int);

char * stage3(int);
void stageprint ();
void textcolor (int);
void clear ();
void phase1move ();
void Setlife ();
void makeclearstring ();
int skill1(int);
int skill2();
int skill3();
void phase1(int);
void phase3 ();
void phase2();
char * end(int);
void colobject (void *);
void rawobject (void *);
char * Achivement (int);
void stage3gotoxy(int x, int y);


void title(void); 
void reset(void); 
void reset_main(void); 
void reset_main_cpy(void); 
void draw_map(void);  
void draw_main(void); 
void new_block(void);  
int check_key(void);  
void drop_block(void);  
int check_crush(int bx, int by, int rotation);  
void move_block(int dir);
void check_line(void);  
void check_level_up(void);  
int check_game_over(void); 
void pause(void);
char * stage2(int);
void gotoxy(int x,int y);