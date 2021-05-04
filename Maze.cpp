#include "Maze.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

Maze::Maze(): xsize(MAZEX), ysize(MAZEY), numin(0) {}

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

void Maze::render(SDL_Renderer* renderer, Uint8 opacity) {

	for(int y = 1; y < ysize-1; y++) {
		for(int x = 1; x < xsize-1; x++) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 205, opacity);
			SDL_Rect fillRect = {BX + x*GAP, BY + TEXT_GAP + y*GAP, GAP, GAP};
			SDL_RenderFillRect(renderer, &fillRect);
			if(MAZE[x][y].left) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, opacity);
				SDL_Rect fillRect = {BX + x*GAP, BY + TEXT_GAP + y*GAP, WALL_THICKNESS, GAP};
				SDL_RenderFillRect(renderer, &fillRect);
			}
			if(MAZE[x][y].up) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, opacity);
				SDL_Rect fillRect = {BX + x*GAP, BY + TEXT_GAP + y*GAP, GAP, WALL_THICKNESS};
				SDL_RenderFillRect(renderer, &fillRect);
			}
		}
	}
}

bool Maze::check_wall(int xx, int yy) {
	if(xx <= BX+GAP || xx >= BX+(xsize-1)*GAP || yy <= BY+TEXT_GAP+GAP || yy >= BY+TEXT_GAP+(ysize-1)*GAP) return true;

	for(int y = 1; y < ysize-1; y++) {
		for(int x = 1; x < xsize-1; x++) {
			int xxx = xx - BX - x*GAP, yyy = yy - BY - TEXT_GAP - y*GAP;
			if(MAZE[x][y].left) {
				if(xxx >= -TOLERANCE && yyy >= -TOLERANCE && xxx <= WALL_THICKNESS+TOLERANCE && yyy <= GAP+TOLERANCE) return true;
			}
			if(MAZE[x][y].up) {
				if(xxx >= -TOLERANCE && yyy >= -TOLERANCE && xxx <= GAP+TOLERANCE && yyy <= WALL_THICKNESS+TOLERANCE) return true;
			}
		}
	}

	return false;
}

