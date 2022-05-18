#include <stdio.h>  
#include <windows.h> 
#include <conio.h>
#include <time.h> 
#include <stdlib.h> 

 
#define LEFT 75  
#define RIGHT 77 
#define UP 72 
#define DOWN 80
#define SPACE 32
#define p 112
#define P 80
#define ESC 27 

#define false 0
#define true 1 

#define ACTIVE_BLOCK -2
#define CEILLING -1
#define EMPTY 0
#define WALL 1
#define INACTIVE_BLOCK 2

#define TITLE_X_SET 5
#define TITLE_Y_SET 5
#define MAIN_X 11 
#define MAIN_Y 23   
#define MAIN_X_SET 3 
#define MAIN_Y_SET 1

#define STATUS_X_SET MAIN_X_SET+MAIN_X+1

int title_name[5][30]={{9,9,9,9,9,0,9,9,9,0,9,9,9,9,9,0,9,9,9,0,0,9,9,9,0,9,9,9,9,9},
					   {0,0,9,0,0,0,9,0,0,0,0,0,9,0,0,0,9,0,0,9,0,0,9,0,0,9,0,0,0,0},
					   {0,0,9,0,0,0,9,9,9,0,0,0,9,0,0,0,9,9,9,0,0,0,9,0,0,9,9,9,9,9},
					   {0,0,9,0,0,0,9,0,0,0,0,0,9,0,0,0,9,0,9,0,0,0,9,0,0,0,0,0,0,9},
					   {0,0,9,0,0,0,9,9,9,0,0,0,9,0,0,0,9,0,0,9,0,9,9,9,0,9,9,9,9,9}};

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
};

int b_type;  
int b_rotation; 
int b_type_next;

int main_org[MAIN_Y][MAIN_X];
int main_cpy[MAIN_Y][MAIN_X];

int bx,by;
int dummy = 100;
 
int key;

int speed;
int cnt;  
int score = 0;

int new_block_on = 0;
int crush_on = 0;
int space_key_on = 0;

void title(void);
void reset(void); 
void reset_main(void);
void reset_main_cpy(void);
void draw_map(void);
void draw_main(void); 
void new_block(void);
void check_key(void); 
void drop_block(void);
int check_crush(int bx, int by, int rotation); 
void move_block(int dir); 
void check_line(void); 
void check_level_up(void);
void check_game_over(void); 
void pause(void); 

/*For cursor switch to x, y*/
void gotoxy(int x,int y) { 
    COORD pos={2*x,y}; 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

/*For hide cursor*/
void CursorView(char show) 
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(hConsole , &ConsoleCursor);
}



int main() {
	int i;
	
	srand((unsigned)time(NULL));
	CursorView(0);
	
	title(); 
	reset();
	
	while(1){
        for(i=0;i<5;i++){  
            check_key(); 
            draw_main(); 
            Sleep(speed); 
            if(crush_on&&check_crush(bx,by+1, b_rotation)==false) Sleep(100);
            if(space_key_on==1) {  
                space_key_on=0;
                break;
            }
        }
        drop_block(); 
        check_game_over(); 
        if(new_block_on==1) new_block(); 
    }
}

/*Draw title name*/
void title(void){
	int i, j;
	int cnt;
	
	for(i=0; i<5; i++){
		for(j=0; j<30; j++){ 
			gotoxy(TITLE_X_SET+j, TITLE_Y_SET+i);
                switch(title_name[i][j]){
                    case 0: 
                        printf("  ");
                        break;
                    case 9: 
                        printf("¢Æ");
                        break;  
                }    
            }
        }
    gotoxy(17, 15);printf("PRESS ENTER");
  
    for(cnt=0;;cnt++){
    	if(kbhit()) break;
    	sleep(10);
		}

	while(kbhit()) getch(); 
} 

/*reset gameboard*/
void reset(void){
	
	key = 0; 
	cnt = 0;
	speed = 100;
	
	system("cls");
	reset_main();
	draw_map(); 
	draw_main(); 
	b_type_next=rand()%7;
    new_block(); 
}

/*void blinking frame*/
void reset_main(void){  
	int i,j;
	
	for(i=0; i < MAIN_Y; i++){
		for(j=0; j < MAIN_X; j++){
			main_org[i][j] = 0;
			main_cpy[i][j] = dummy; 
		}
	} 
	for(j = 1; j < MAIN_X; j++)  
		main_org[3][j] = CEILLING; 


	for(i = 1; i < MAIN_Y-1; i++){ 
		main_org[i][0] = WALL;
		main_org[i][MAIN_X-1] = WALL; 
	}
	for(j=0; j < MAIN_X; j++){ 
		main_org[MAIN_Y-1][j] = WALL;
	}
}

/*draw optional infomation*/
void draw_map(void){  
	int y =3;
	gotoxy(STATUS_X_SET, y); printf("   S C O R E");     
    gotoxy(STATUS_X_SET, y+1); printf("     %6d", score);
	gotoxy(STATUS_X_SET, y+2); printf("+-  N E X T  -+ ");
    gotoxy(STATUS_X_SET, y+3); printf("|             | ");
    gotoxy(STATUS_X_SET, y+4); printf("|             | ");
    gotoxy(STATUS_X_SET, y+5); printf("|             | ");
    gotoxy(STATUS_X_SET, y+6); printf("|             | ");
    gotoxy(STATUS_X_SET, y+7); printf("+-------------+ "); 
    gotoxy(STATUS_X_SET, y+9); printf("SPACE : Hard Drop");
    gotoxy(STATUS_X_SET, y+11); printf("P   : Pause");
    gotoxy(STATUS_X_SET, y+13); printf("ESC  : Quit");
    gotoxy(STATUS_X_SET, y+15); printf("   ¡è   : Rotation");     
    gotoxy(STATUS_X_SET, y+16); printf(" ¡ç  ¡æ : Left / Right");     
    gotoxy(STATUS_X_SET, y+17); printf("   ¡é   : Soft Drop");
	
}

/*Draw main gameboard*/
void draw_main(void){ 
	int i,j;
	
	for(j=1; j<MAIN_X-1; j++){
		if(main_org[3][j] == EMPTY)
			main_org[3][j] = CEILLING;
	} 
	
	for(i=0; i<MAIN_Y; i++){
		for(j=0; j<MAIN_X; j++){
			if(main_cpy[i][j] != main_org[i][j]) { 
												   
			gotoxy(MAIN_X_SET+j,MAIN_Y_SET+i); 
                switch(main_org[i][j]){
                    case EMPTY:  
                        printf("  ");
                        break;
                    case CEILLING: 
                        printf(". ");
                        break;
                    case WALL:  
                        printf("¢Ì"); 
                        break;
                    case INACTIVE_BLOCK:  
                        printf("¡à");
                        break;
                    case ACTIVE_BLOCK:
                        printf("¡á");
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

/*Make new block*/
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
            if(blocks[b_type][b_rotation][i][j]==1)
				main_org[by+i][bx+j]=ACTIVE_BLOCK;
        }
    }
    for(i=1;i<3;i++){  
        for(j=0;j<4;j++){
            if(blocks[b_type_next][0][i][j]==1) {
                gotoxy(STATUS_X_SET+2+j,i+6);
                printf("¡á");
            }
            else{
                gotoxy(STATUS_X_SET+2+j,i+6);
                printf("  ");
            }
        }
    }
}

/*Drop block automatically in board*/
void drop_block(void){
    int i,j;
    
    if(crush_on&&check_crush(bx,by+1, b_rotation)==true)
		crush_on=0; 
    if(crush_on&&check_crush(bx,by+1, b_rotation)==false){
        for(i=0;i<MAIN_Y;i++){ 
            for(j=0;j<MAIN_X;j++){
                if(main_org[i][j]==ACTIVE_BLOCK)
					main_org[i][j]=INACTIVE_BLOCK;
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

/*Recieve key value*/
void check_key(void){
    key=0;
    
    if(kbhit()){   
        key=getch();
        if(key==224){  
            do{key=getch();} while(key==224); 
            switch(key){
                case LEFT:   
                    if(check_crush(bx-1,by,b_rotation)==true) move_block(LEFT); 
                    break; 
                case RIGHT: 
                    if(check_crush(bx+1,by,b_rotation)==true) move_block(RIGHT);
                    break;
                case DOWN: 
                    if(check_crush(bx,by+1,b_rotation)==true) move_block(DOWN);
                    break;
                case UP: 
                    if(check_crush(bx,by,(b_rotation+1)%4)==true) {
						move_block(UP);}
                    
                    else if(crush_on==1&&check_crush(bx,by-1,(b_rotation+1)%4)==true){
						move_block(100);
						}
        	}
		}
        else{  
            switch(key){
                case SPACE: 
                    space_key_on=1; 
                    while(crush_on==0){ 
                        drop_block();
                        
                    }
                    break;
                case P: 
                case p: 
                    pause();
                    break;
                case ESC:
                    system("cls");
                    exit(0); 
            }
        }
    }
    while (kbhit()) getch();
}

/*Checking block if crushing line*/
int check_crush(int bx, int by, int b_rotation){
    int i,j;
    
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){ 
            if(blocks[b_type][b_rotation][i][j]==1&&main_org[by+i][bx+j]>0) 
				return false;
        }
    }    
    return true; 
};

/*Confirm whether blocks make line. If blocks make line delete the line.*/
void check_line(void){
    int i, j, k, l;
    
    int block_amount; 
    
    for(i=MAIN_Y-2;i>3;){
        block_amount=0; 
        for(j=1;j<MAIN_X-1;j++){ 
            if(main_org[i][j]>0) block_amount++;
        }
        if(block_amount==MAIN_X-2){  
            score+=100;
            gotoxy(STATUS_X_SET, 4); printf("     %6d", score);
            for(k=i;k>1;k--){  
                for(l=1;l<MAIN_X-1;l++){
                    if(main_org[k-1][l]!=CEILLING) main_org[k][l]=main_org[k-1][l];
                    if(main_org[k-1][l]==CEILLING) main_org[k][l]=EMPTY;
                }
            }
        }

        else i--;
    }
   
}			 

/*Moving Block according to key value*/
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

/*Reset main_cpy*/
void reset_main_cpy(void){  
    int i, j;
    
    for(i=0;i<MAIN_Y;i++){
        for(j=0;j<MAIN_X;j++){ 
            main_cpy[i][j]=100;
        }
    }    
}

/*Check if game is over*/
void check_game_over(void){ 
    int i;
    
    int x=5;
    int y=5;
    
    for(i=1;i<MAIN_X-2;i++){
        if(main_org[3][i]>0){
            gotoxy(x-1,y+6); printf("++++++++++++++++++");
            gotoxy(x,y+7); printf("G A M E O V E R");
            gotoxy(x-1,y+8); printf("++++++++++++++++++");
            Sleep(1000);
            while (kbhit()) getch();
            key=getch();
            reset();
            
        }
    }
}

/*When user press 'p'*/
void pause(void){
    int i,j;
    
    int x=5;
    int y=5;
    
    for(i=1;i<MAIN_X-2;i++){ 
            gotoxy(x,y+6); printf("¢È¢È¢È¢È¢È¢È¢È");
			gotoxy(x,y+7); printf("¢È P A U S E¢È");
			gotoxy(x,y+8); printf("¢È¢È¢È¢È¢È¢È¢È");
            }
    getch(); 
    
    system("cls"); 
    reset_main_cpy();  
    draw_main();
    draw_map(); 
 
    for(i=1;i<3;i++){ 
        for(j=0;j<4;j++){
            if(blocks[b_type_next][0][i][j]==1) {
                gotoxy(MAIN_X+MAIN_X_SET+3+j,i+6);
                printf("¡á");
            }
            else{
                gotoxy(MAIN_X+MAIN_X_SET+3+j,i+6);
                printf("  ");
            }
        }
    }    
} 
