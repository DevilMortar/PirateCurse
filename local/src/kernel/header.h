#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "config.h"

typedef struct TEXTURE TEXTURE;
struct TEXTURE
{
    SDL_Texture * texture;
    SDL_Rect dstrect;
};

typedef struct SPRITE SPRITE;
struct SPRITE
{
    SDL_Texture * texture;
    SDL_Rect dstrect;
    SDL_Rect srcrect;
    int frame;
    int max;
    int srcsize;
};

typedef struct POSITION POSITION;
struct POSITION
{
    int x;
    int y;
    int direction;
};

typedef struct OBSTACLE OBSTACLE;
struct OBSTACLE
{
    POSITION position;
    OBSTACLE * previous;
    OBSTACLE * next;
    int frame;
    int warning;
};

typedef struct LIST_OBSTACLE LIST_OBSTACLE;
struct LIST_OBSTACLE
{
    OBSTACLE * first;
    OBSTACLE * last;
    SPRITE sprite;
    SPRITE warning;
};

typedef struct PLAYER PLAYER;
struct PLAYER
{
    POSITION position;
    SPRITE sprite;
    int skin;
    
};

typedef struct COIN COIN;
struct COIN
{
    POSITION position;
    SPRITE sprite;  
};

// SDL
void SDL_ExitWithError(const char * message); // Quitter 




