#ifndef MAIN_H
#define MAIN_H
#pragma once

#define BX 6
#define BY 6
#define GAP 40

#define WALL_THICKNESS 3

#define MAZEX 20
#define MAZEY 10

#define TEXT_GAP 10

#define TOLERANCE 0

//Screen dimension constants
const int SCREEN_WIDTH = 2*BX + MAZEX*GAP;
const int SCREEN_HEIGHT = 2*BY + MAZEY*GAP + 2*TEXT_GAP;

#define HEALTH_DROP_INCREMENT 1000

const int MAX_SIZE = 1000;

#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Maze.h"
#include "Timer.h"
#include "Texture.h"
#include "Tank.h"
#include "Health.h"
#include "Player.h"
#include "Bullet.h"

#endif