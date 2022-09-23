#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

char user;
int x, y, cycles, deathcount, warpcount, oldx;

//2 Robots being added randomly at beginning
//Debug mode shows at least 1 isn't being drawn by draw code.
//It was for loops counting i to 20 instead of 19, ask dadoo for explanation.

//Add:
//Diagonal Movement
//Scoring
//Game 1 is 5 robots, Game 2 is 6 robots, Game 3 is 7 robots, and so on
//Limit warps to 1 per 10 moves

int main(void){
	cycles = 0;

	//Setup Ncurses
	initscr();
	noecho();
	curs_set(0);

	deathcount = 0;
	//Set robot and robot death coordinates offscreen.
	int robotX[20] = {COLS + 5};
	int robotY[20] = {COLS + 5};
	int deathX[30] = {COLS + 10};
	int deathY[30] = {COLS + 10};

	srand(time(NULL));
	x = COLS / 2;
	y = LINES / 2;
	while(1){
		clear();
		//Debug Info
		/*for(int i = 0; i <= 24; i++){
			mvprintw(i, 1, "X %d, Y %d", robotX[i], robotY[i]);
		}*/
		/*for(int i = 0; i <= 19; i++){
			for(int j = 0; j <= 19; j++){
				if(robotX[i] == robotX[j] && i != j && robotX[i] != COLS + 5){
					for(int k = 0; k <= 19; k++){
						for(int l = 0; l <= 19; l++){
							if(robotY[k] == robotY[l] && k != l && robotY[k] != COLS + 5){
								mvaddch(robotY[k], robotX[i], '%');
								//Set position to offscreen
								deathX[deathcount] = robotX[i];
								deathY[deathcount] = robotY[k];
								deathcount = deathcount + 1;
								robotX[i] = COLS + 5;
								robotY[k] = COLS + 5;
								robotX[j] = COLS + 5;
								robotY[l] = COLS + 5;
							}
						}
					}
				}
			}
		}*/
		//Robot Kill Code
		for(int i = 0; i <= 19; i++){
			for(int j = 0; j <= 19; j++){
				if(robotX[i] == robotX[j] && robotY[i] == robotY[j] && i != j && robotX[i] != COLS + 5){
					//Add a trash pile
					mvaddch(robotY[i], robotX[i], '%');
					//Set a location that will kill robots if they intersect with a trash pile
					deathX[deathcount] = robotX[i];
					deathY[deathcount] = robotY[i];
					//Make sure death coordinates dont intersect
					deathcount++;
					//Set the 2 robots that died to offscreen
					robotX[i] = COLS + 5;
					robotX[j] = COLS + 5;
					robotY[i] = COLS + 5;
					robotY[j] = COLS + 5;
				}
			}
		}
		//Draw trash piles
		for(int i = 0; i <= 29; i++){
			if(deathX[i] != COLS + 10){
				mvaddch(deathY[i], deathX[i], '%');
			}
		}
		//Spawn robot every 20 moves
		if(cycles % 20 == 0){
			for(int i = 0; i <= 19; i++){
				if(robotX[i] == COLS + 5){
					//Black magic fuckery with rand (ask dadoo how it works)
					robotX[i] = rand() % ((COLS - 2) - 1 + 1) + 1;
					robotY[i] = rand() % ((LINES - 2) - 1 + 1) + 1;
					break;
				}
			}
		}
		//Make sure robot X can hit player X (set even to odd and vice versa if needed)
		/*for(int i = 0; i <= 20; i++){
			if(robotX[i] != COLS + 5){
				if(x % 2 == 0){
					if(robotX[i] % 2 != 0){
						robotX[i] = robotX[i] - 1;
					}
				}
				else{
					if(robotX[i] % 2 == 0){
						robotX[i] = robotX[i] - 1;
					}
				}
			}
		}*/


		//Make sure robot can hit player
		for(int i = 0; i <= 19; i++){
			if(robotX[i] != COLS + 5){
				if(x % 2 == 0 && robotX[i] % 2 != 0){
					robotX[i]--;
				}
				else if(x % 2 != 0 && robotX[i] % 2 == 0){
					robotX[i]++;
				}
			}
		}
		//Kill robots if in contact with dead robots
		for(int i = 0; i <= 19; i++){
			for(int j = 0; j <= 49; j++){
				if(robotX[i] == deathX[j] && robotY[i] == deathY[j]){
					robotX[i] = COLS + 5;
					robotY[i] = COLS + 5;
				}
			}
		}
		//Move Robots & Kill Player
		for(int i = 0; i <=19; i++){
			if(robotX[i] != COLS + 5 && robotY[i] != COLS + 5){
				//Movement code
				if(robotX[i] < x){
					robotX[i] = robotX[i] + 2;
				}
				else if(robotX[i] > x){
					robotX[i] = robotX[i] - 2;
				}
				if(robotY[i] < y){
					robotY[i]++;
				}
				else if(robotY[i] > y){
					robotY[i]--;
				}
				//Kill player if in contact with robot
				if(robotX[i] == x && robotY[i] == y){
                                	clear();
                                	mvprintw(LINES / 2, (COLS / 2) - 5, "Game Over!");
                                	refresh();
                                	sleep(3);
                                	echo();
                                	curs_set(2);
                                	endwin();
                                	return(0);
                                }
			}
		}

		//Draw robots
		for(int i = 0; i <= 19; i++){
			mvaddch(robotY[i], robotX[i], '&');
		}
		refresh();
		//Draw top/bottom border
		for(int i = 0; i < COLS; i++){
			mvaddch(0, i, '-');
			mvaddch(LINES - 1, i, '-');
		}
		//Draw left/right border
		for(int i = 0; i < LINES; i++){
			mvaddch(i, 0, '|');
			mvaddch(i, COLS - 1, '|');
		}
			
		//Prevent out of bounds
		if(y <= 0){
			y = y + 1;
		}
		else if(LINES - 1 <= y){
			y = y - 1;
		}
		else if(x <= 0){
			x = x + 2;
		}
		else if(COLS <= x){
			x = x - 2;
		}
		for(int i = 0; i <= 19; i++){
			if(robotX[i] != COLS + 5){
				if(robotY[i] <= 1){
					robotY[i] = robotY[i] + 1;
				}
				else if(LINES - 2 <= robotY[i]){
					robotY[i] = robotY[i] - 1;
				}
				else if(robotX[i] <= 1){
					robotX[i] = robotX[i] + 2;
				}
				else if(COLS - 2 <= robotX[i]){
					robotX[i] = robotX[i] - 2;
				}
			}
		}
		//Draw Character
		mvaddch(y, x, '#');
		refresh();
		//User input
		for(int i = 0; i < 1;){
			user = getch();
			if(user == 'a'){
				x = x - 2;
				refresh();
				i++;
			}
			else if(user == 'd'){
				x = x + 2;
				refresh();
				i++;
			}
			else if(user == 'w'){
				y--;
				refresh();
				i++;
			}
			else if(user == 's'){
				y++;
				refresh();
				i++;
			}
			else if(user == 'r'){
				y--;
				x = x - 2;
				i++;
			}
			else if(user == 'y'){
				y--;
				x = x + 2;
				i++;
			}
			else if(user == 'v'){
				y++;
				x = x - 2;
				i++;
			}
			else if(user == 'n'){
				y++;
				x = x + 2;
				i++;
			}
			else if(user == 'g'){
				if(warpcount < 5){
					oldx = x;
					y = rand() % ((LINES - 5) + 1 - 1) + 1;
					x = rand() % ((COLS - 5) + 1 - 1) + 1;
					if(oldx % 2 == 0 && x != 0){
						x++;
					}
					else if(oldx % 2 != 0 && x % 2 == 0){
						x--;
					}
					for(int j = 0; j <= 19; j++){
						if(x == robotX[j] && y == robotY[j]){
							x = x + 2;
							y++;
							break;
						}
					}
					i++;
					warpcount++;
				}
				else{
					mvprintw(1, (COLS - 7) / 2, "Out of warps!");
				}
			}
			else if(user == 'q'){
				endwin();
				return(0);
			}
		}
		cycles++;
	}
}
