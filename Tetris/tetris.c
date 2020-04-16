#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <string.h>
//
typedef struct tetris{
	int x;
	int y;
	int cube[4][2];
}tetris_t;
tetris_t Tetris;
typedef struct gamescore{
	char name[6];
	int diem;
} gamescore_t;
gamescore_t highscore[4];
int game[22][12];
int score;
int speed;
int modTime;
int timeMove;
int countaddScore;
int line;
//
int key;
int test;
//

void readhighscore(){
	FILE *f;
	int i;
	char s[20];
	f = fopen("highscore.txt", "r");
	for(i = 0; i < 3; i++){
		fgets(s, 20, f);
		memcpy(highscore[i].name, s+1, 6 * sizeof(char));
		highscore[i].diem = atoi(s + 7);
	}
	fclose(f);
	
}
void writehighscore(){
	FILE *f;
	int i;
	f = fopen("highscore.txt", "w");
	for(i = 0; i < 3; i++){
		fprintf(f, "%d%6s%6d\n", i + 1, highscore[i].name, highscore[i].diem);
	}
	fclose(f);
	
}
void resizeConsole(int width, int height){
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}
void gotoxy(int x, int y)   {
    static HANDLE h = NULL; 
    if(!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { x + 18, y + 4}; 
    SetConsoleCursorPosition(h,c);
}
//start game
void Startgame(){
	readhighscore();
	srand(time(NULL));
	resizeConsole(600, 600);
}
//main1
void setCOLOR(int n){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
}
void newTetris(){
	key = rand() % 7;
	Tetris.x = 5;
	Tetris.y = 1;
	switch(key){
		case 0: Tetris.cube[0][1] = 1; Tetris.cube[0][0] = 1; 
				Tetris.cube[1][1] = 1; Tetris.cube[1][0] = 0; 
				Tetris.cube[2][1] = 0; Tetris.cube[2][0] = 1; 
				Tetris.cube[3][1] = 0; Tetris.cube[3][0] = 0; 
				break;
		case 1: Tetris.cube[0][1] = -1; Tetris.cube[0][0] = 1; 
				Tetris.cube[1][1] = -1; Tetris.cube[1][0] = 0; 
				Tetris.cube[2][1] = 0; Tetris.cube[2][0] = 0; 
				Tetris.cube[3][1] = 1; Tetris.cube[3][0] = 0; 
				break;
		case 2: Tetris.cube[0][1] = -1; Tetris.cube[0][0] = 0; 
				Tetris.cube[1][1] = 0; Tetris.cube[1][0] = 0; 
				Tetris.cube[2][1] = 1; Tetris.cube[2][0] = 0; 
				Tetris.cube[3][1] = 1; Tetris.cube[3][0] = 1; 
				break;
		case 3: Tetris.cube[0][1] = -1; Tetris.cube[0][0] = 0; 
				Tetris.cube[1][1] = 0; Tetris.cube[1][0] = 0; 
				Tetris.cube[2][1] = 1; Tetris.cube[2][0] = 0; 
				Tetris.cube[3][1] = 2; Tetris.cube[3][0] = 0; 
				break;
		case 4: Tetris.cube[0][1] = -1; Tetris.cube[0][0] = 0; 
				Tetris.cube[1][1] = 0; Tetris.cube[1][0] = 0; 
				Tetris.cube[2][1] = 0; Tetris.cube[2][0] = 1; 
				Tetris.cube[3][1] = 1; Tetris.cube[3][0] = 0; 
				break;
		case 5: Tetris.cube[0][1] = -1; Tetris.cube[0][0] = 1; 
				Tetris.cube[1][1] = 0; Tetris.cube[1][0] = 1; 
				Tetris.cube[2][1] = 0; Tetris.cube[2][0] = 0; 
				Tetris.cube[3][1] = 1; Tetris.cube[3][0] = 0; 
				break;
		case 6: Tetris.cube[0][1] = -1; Tetris.cube[0][0] = 0; 
				Tetris.cube[1][1] = 0; Tetris.cube[1][0] = 0; 
				Tetris.cube[2][1] = 0; Tetris.cube[2][0] = 1; 
				Tetris.cube[3][1] = 1; Tetris.cube[3][0] = 1; 
				break;
		default: break;
	}
}
void In(){
	int i, j;
	system("cls");
	printf("\n\n\n\n\t\t  ");
	
	for(i = 0; i < 22; i++){
		for(j = 0; j < 12; j++){
			
			switch(game[i][j]){
				case -1: setCOLOR(119); printf("[]"); break;
				case 0: setCOLOR(7); printf("  "); break;
				
				case -2: setCOLOR(238); printf("[]"); break;
				default: setCOLOR(7); printf("  ");break;
			}
		}
		setCOLOR(7);
		printf("\n\t\t  ");
	}
	printf("\tScore : %d\n\t\t  ", score);
	printf("\tLine:   %d\n\t\t  ", line);
	printf("\tLevel : %d\n", line / 15 + 1);
}
void reset(){
	test = 0;
	int i, j;
	for(i = 0; i < 22; i++){
		for(j = 0; j < 12; j++){
			if((i % 21 == 0) || (j % 11 == 0)){
				game[i][j] = -1;
			}
			else game[i][j] = 0;
		}
	}
	line = 0;
	score = 0;
	speed = 400;
	modTime = 0;
	timeMove = 100;
	newTetris();
	In();
}

void clearRow(int i){
	int dem, j;
	for(j = 1; j < 11; j++){
		game[i][j] = 0;
	}
	for(dem = i - 1; dem > 0; dem--){
		for(j = 1; j < 11; j++){
			game[dem + 1][j] = game[dem][j];
			game[dem][j] = 0; 
		}
	}
	
	countaddScore++;
	line++;
}
void addScore(){
	int i, j;
	countaddScore = 0;
	for(i = 1; i < 21; i++){
		j = 1;
		while(j < 11 && game[i][j] == -2)j++;
		if(j == 11){
			clearRow(i);
		}
	}
	switch(countaddScore){
		case 0: break;
		case 1: score+= 100; break;
		case 2: score+= 300; break;
		case 3: score+= 600; break;
		case 4: score+= 1000; break;
		default: break;
	}
	if(line < 60){
		speed = 400 - 50 * (line / 15);
	}
	else {
		speed = 200;
		timeMove = 70;
	}
	In();
}
void clearTetris(){
	int i;
	setCOLOR(7);
	for(i = 0; i < 4; i++){
		game[Tetris.cube[i][0] + Tetris.y][Tetris.cube[i][1] + Tetris.x] = 0;
		gotoxy((Tetris.cube[i][1] + Tetris.x)*2, Tetris.cube[i][0] + Tetris.y);
		printf("  ");
	}
	setCOLOR(7);
}
void displayTetris(){
	int i;
	switch(key/ 2){
		case 0: setCOLOR(17); break;
		case 1: setCOLOR(34); break;
		case 2: setCOLOR(51); break;
		case 3: setCOLOR(85); break;
		default: break;						
	}
	for(i = 0; i < 4; i++){
		game[Tetris.cube[i][0] + Tetris.y][Tetris.cube[i][1] + Tetris.x] = 1;
		gotoxy((Tetris.cube[i][1] + Tetris.x)*2, Tetris.cube[i][0] + Tetris.y);
		printf("[]");
		
	}
	setCOLOR(7);
}
void blockTetris(){
	int i;
	setCOLOR(238);
	for(i = 0; i < 4; i++){
		game[Tetris.cube[i][0] + Tetris.y][Tetris.cube[i][1] + Tetris.x] = -2;
		gotoxy((Tetris.cube[i][1] + Tetris.x)*2, Tetris.cube[i][0] + Tetris.y);
		printf("[]");
	}
	setCOLOR(7);
	addScore();
	//getch();
}
int left(){
	int i;
	for(i = 0; i < 4; i++){
		if(game[Tetris.cube[i][0] + Tetris.y][Tetris.cube[i][1] + Tetris.x - 1] < 0){
			return 0;
		}
	}
	return 1;
}
int right(){
	int i;
	for(i = 0; i < 4; i++){
		if(game[Tetris.cube[i][0] + Tetris.y][Tetris.cube[i][1] + Tetris.x + 1] < 0){
			return 0;
		}
	}
	return 1;
}
int down(){
	int i;
	for(i = 0; i < 4; i++){
		if(game[Tetris.cube[i][0] + Tetris.y + 1][Tetris.cube[i][1] + Tetris.x] < 0){
			return 0;
		}
	}
	return 1;
}
void taitao(){
	int i, tmp;
	for(i = 0; i < 4; i++){
		tmp = Tetris.cube[i][0];
		Tetris.cube[i][0] = Tetris.cube[i][1];//(-2, 1) -> (1, 2) 
		Tetris.cube[i][1] = -tmp;	
	}
}
void xoay(){
	int i, tmp;
	clearTetris();
	
	for(i = 0; i < 4; i++){
		tmp = Tetris.cube[i][0];
		Tetris.cube[i][0] = - Tetris.cube[i][1];//(1, 2) -> (-2, 1) 
		Tetris.cube[i][1] = tmp;	
	}
	for(i = 0; i < 4; i++){
		if(game[Tetris.cube[i][0] + Tetris.y][Tetris.cube[i][1] + Tetris.x] < 0){
			taitao();
			return;
		}	
	}
}

int Run(){
	if(down() == 1){clearTetris(); Tetris.y++;}
	else{
		if(Tetris.y > 2){blockTetris();newTetris(); return test;}
		else{
			int i = 3;
			
			system("cls");
			while(score > highscore[i-1].diem && i >= 1){
				highscore[i].diem = highscore[i-1].diem;
				strcpy(highscore[i].name, highscore[i-1].name);
				i--;
			}
			if(i < 3){
				highscore[i].diem = score;
				printf("nhap ten cua ban: ");
				gets(highscore[i].name);
				writehighscore();	
			}
		
			gotoxy(10, 5);
			printf("Game over!\n\n\t\t");
			printf("An phim bat ki de quay ve menu...");
			getch();
			return 1;
		}
	}
	
}

void pause(){
	char p;
	system("cls");
	printf("\n\n\tPause:");
	printf("\n\n\n\t\tM : Return to menu\n");
	printf("\t\tC : Continue\n");
	printf("\t\tE : Exit game\n");
	while(1){
		p = getch();
		switch(p){
			case 'm': test = 1; return;
			case 'c': In(); displayTetris(); return;
			case 'e': exit(1);
			default: break;
		}
	}
}
//newgame
void newgame(){
	
	
	char c ;
	int k;
	reset();
	
	while(test != 1){
		
        
        displayTetris();
        for(k = 0; k <= (speed + modTime) / timeMove; k++){
        	Sleep(timeMove);
        	if(kbhit()){
        		c = getch();	
    			switch(c){
            		case 'p' : pause(); if(test == 1) return; break; 
           			case 'r' : reset(); break;
           			case 's' : while(down() == 1) Run(); break;
            		case 'w' : xoay(); break;
            		case 'a' : if(left() == 1){clearTetris(); Tetris.x--; } break;
            		case 'd' : if(right() == 1){clearTetris(); Tetris.x++; } break;
            		default: break;
        		}
        		displayTetris();
    		}

		}
		modTime = (speed + modTime) % 100;
		
        test = Run();
		
	}
}
//menu1
void displayhighscore(){
	int i; 
	system("cls");
	printf("\n\n\t\tHigh Score:\n\n");
	for(i = 0; i < 3; i++){
		printf("\t\t%d\t%6s\t%6d\n", i+1, highscore[i].name, highscore[i].diem);
	}
	getch();
}
//menu2
void guide(){
	system("cls");
	printf("\tControl:\n\n");
	printf("\t\tA : Left\n");
	printf("\t\tD : Right\n");
	printf("\t\tW : Up\n");
	printf("\t\tA : Down\n");
	printf("\t\tR : Reset and play new game\n");
	printf("\t\tP : Pause and option\n\n");
	printf("\t\t\tAn phim bat ki de quay ve menu...");
	getch();
}
//menu3
void Menu(){
	
	while(1){
		char c;
		system("cls");
		printf("\t\tTetris 1.0\n\n");
		printf("\t\t\t\tmake by datbk206\n\n\n");
		printf("\t\t1.Tro choi moi\n");
		printf("\t\t2.Diem cao\n");
		printf("\t\t3.Huong dan\n");
		printf("\t\t4.Thoat\n");
		c = getch();
		switch(c){
			case '1': newgame(); break;
			case '2': displayhighscore(); break;
			case '3': guide(); break;
			case '4': exit(0); break;
			default: printf("Invalid input!\n"); exit(1); break;
		}
	}
}
//main2
int main(){
	Startgame();//giaodien
	Menu();//menu game
}
