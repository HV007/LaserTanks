#include "Maze.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define BX 6
#define BY 6
#define GAP 6

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

Maze::Maze(): xsize(20), ysize(20), numin(0) {}

Maze::Maze(int xsize, int ysize): xsize(xsize), ysize(ysize), numin(0) {}

void Maze::initialise() {
    for(int x = 0; x < xsize; x++) {
		for(int y = 0; y < ysize; y++) {
			MAZE[x][y].in = ( x == 0 || x == xsize - 1 || y == 0 || y == ysize - 1 ) ? 1 : 0;
			MAZE[x][y].up = ( x == 0 || x == xsize - 1 || y == 0 ) ? 0 : 1;
			MAZE[x][y].left = ( x == 0 || y == 0 || y == ysize - 1 ) ? 0 : 1;
		}
	}
}

void Maze::generate() {
    int xcur = 1, ycur = 1;
    MAZE[xcur][ycur].in = 1;
    numin++;
    int whichway;
    bool success;
    do {
        if(MAZE[xcur][ycur-1].in && MAZE[xcur][ycur+1].in && MAZE[xcur-1][ycur].in && MAZE[xcur+1][ycur].in) {
			do {
				success = 0;
				whichway = rand()%4;
				switch(whichway) {
				case UP:
					if(MAZE[xcur][ycur].up && ycur != 1){
						success = 1;
						MAZE[xcur][ycur].up = 0;
					}
					break;
				case DOWN:
					if(MAZE[xcur][ycur+1].up && ycur != ysize-2){
						success = 1;
						MAZE[xcur][ycur+1].up = 0;
					}
					break;
				case LEFT:
					if(MAZE[xcur][ycur].left && xcur != 1) {
						success = 1;
						MAZE[xcur][ycur].left = 0;
					}
					break;
				case RIGHT:
					if(MAZE[xcur+1][ycur].left && xcur != xsize-2) {
						success = 1;
						MAZE[xcur+1][ycur].left = 0;
					}
					break;
				}
			} while(!success);
		}

        while(MAZE[xcur][ycur-1].in && MAZE[xcur][ycur+1].in && MAZE[xcur-1][ycur].in && MAZE[xcur+1][ycur].in) {
            int xcur2 = MAZE[xcur][ycur].prevx;
            ycur = MAZE[xcur][ycur].prevy;
            xcur = xcur2;
		}

        do {
			success = 0;
			whichway = rand()%4;
			switch(whichway){
			case UP:
				if(!MAZE[xcur][ycur-1].in) {
					success = 1;
					MAZE[xcur][ycur].up = 0;
					MAZE[xcur][ycur-1].prevx = xcur;
					MAZE[xcur][ycur-1].prevy = ycur;
					ycur--;
				}
				break;
			case DOWN:
				if(!MAZE[xcur][ycur+1].in) {
					success = 1;
					MAZE[xcur][ycur+1].up = 0;
					MAZE[xcur][ycur+1].prevx = xcur;
					MAZE[xcur][ycur+1].prevy = ycur;
					ycur++;
				}
				break;
			case LEFT:
				if(!MAZE[xcur-1][ycur].in) {
					success = 1;
					MAZE[xcur][ycur].left = 0;
					MAZE[xcur-1][ycur].prevx = xcur;
					MAZE[xcur-1][ycur].prevy = ycur;
					xcur--;
				}
				break;
			case RIGHT:
				if(!MAZE[xcur+1][ycur].in) {
					success = 1;
					MAZE[xcur+1][ycur].left = 0;
					MAZE[xcur+1][ycur].prevx = xcur;
					MAZE[xcur+1][ycur].prevy = ycur;
					xcur++;
				}
				break;
			}
		} while(!success);
		MAZE[xcur][ycur].in = 1;
		numin++;
    } while(numin < (xsize-2)*(ysize-2));
}

void Maze::render(SDL_Renderer* renderer) {
    int width = (xsize-1)*2 - 1;
	int height = (ysize-1)*2 - 1;

    for(int y = 0; y <= height - 1; y++) {
		for(int x = 0; x <= width - 1; x++) {
			if(x%2 == 1 && y%2 == 1) {
                if(MAZE[x/2+1][y/2+1].in) SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
                else SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			} else if(x%2 == 0 && y%2 == 0) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			} else if(x%2 == 0 && y%2 == 1) {
				if(MAZE[x/2+1][y/2+1].left) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			} else if(x%2 == 1 && y%2 == 0) {
				if(MAZE[x/2+1][y/2+1].up) {std::cout << "HERE "; SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);}
                else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			}
            SDL_RenderDrawLine(renderer, BX + (x + 1)*GAP, BY + y*GAP, BX + (x + 1)*GAP, BY + (y + 1)*GAP);
		}
	}
}

