#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>

void printMap(char** map, int x, int y, SDL_Renderer** renderer){
    SDL_Rect r = {0, 0, 16, 16};
    SDL_SetRenderDrawColor(*renderer, 0x00, 0x00, 0x00, 0xff);
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            if((*map)[i * x + j] == '*'){
            r.x = i * 16;
            r.y = j * 16;
            SDL_RenderFillRect(*renderer, &r);
         }
        } 
    }
    SDL_SetRenderDrawColor(*renderer, 0xff, 0xff, 0xff, 0xff);
}

int evalMap(char** map, int x, int y){ 
    int *toBeRemoved = malloc((x * y + 1) * sizeof(int));
    memset(toBeRemoved, 0, (x * y + 1) * sizeof(int));
    int *toBeCreated = malloc((x * y + 1) * sizeof(int));
    memset(toBeCreated, 0, (x * y + 1) * sizeof(int));
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            int nOfN = 0;
            //To the left
            if((j > 0) && ((*map)[i * x + (j-1)] == '*')) nOfN++;
            //To the right
            if((j < y) && ((*map)[i * x + (j+1)] == '*')) nOfN++;
            //Up
            if((i > 0) && ((*map)[(i-1) * x + j] == '*')) nOfN++;
            //Below
            if((i < x) && ((*map)[(i+1) * x + j] == '*')) nOfN++;
            //Top-Right
            if((j < y) && (i > 0) && ((*map)[(i-1) * x + (j+1)] == '*')) nOfN++;
            //Top-Left
            if((j > 0) && (i > 0) && ((*map)[(i-1) * x + (j-1)] == '*')) nOfN++;
            //Bottom-Right
            if((j < y) && (i < x) && ((*map)[(i+1) * x + (j+1)] == '*')) nOfN++;
            //Bottom-Left
            if((j > 0) && (i < x) && ((*map)[(i+1) * x + (j-1)] == '*')) nOfN++;
            if((nOfN < 2) && ((*map)[i * x + j] == '*')) toBeRemoved[i * x + j] = 1;
            if((nOfN > 3) && ((*map)[i * x + j] == '*')) toBeRemoved[i * x + j] = 1;
            if((nOfN == 3) && ((*map)[i * x + j] == '0')) toBeCreated[i * x +j] = 1;

        }
    }
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            if(toBeRemoved[i * x + j]) (*map)[i * x + j] = '0';
            if(toBeCreated[i * x + j]) (*map)[i * x + j] = '*';
        }
    }

    free(toBeCreated);
    free(toBeRemoved);
    
    int nOfAlive = 0;
    for(int i = 0; i < x; i++)
        for(int j = 0; j < y; j++)
            if((*map)[i * x + j] == '*') nOfAlive++;

    return nOfAlive;
}

int main(int argc, char* argv[]){
    SDL_Window *window;
    SDL_Renderer *renderer;
    char *map;
    int x, y, nOfRandom;
    printf("X: ");
    scanf("%d", &x);
    printf("Y: ");
    scanf("%d", &y);
    map = (char *)(malloc(((x + 1) * (y + 1)) * sizeof(char)));
    memset(map, '0', ((x + 1) * (y + 1)) * sizeof(char));
    printf("How many random cells to plot? ");
    scanf("%d", &nOfRandom);
    printf("Generating randomness! This might take a while... (Blame whoever made rand())\n");
    for(int i = 0; i < nOfRandom; i++)
        for(;;){
            int randx = rand() % x, randy = rand() % y;
            //srand(time(NULL));
            //sleep(1);
            if(map[x * randx + randy] == '0'){
                map[x * randx + randy] = '*';
                break;
            }
        }
        window = SDL_CreateWindow("Game Of Life C", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, y * 16, x * 16, SDL_WINDOW_SHOWN);
        if(!window) return -1;
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(!renderer) return -1;
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    int nOfAlive = 0;
    int quit = 0;
    SDL_Event e;
    do{  
        while(SDL_PollEvent(&e) != 0)
            if(e.type == SDL_QUIT)
                quit = 1;
        SDL_RenderClear(renderer);
        printMap(&map, x, y, &renderer);
        nOfAlive = evalMap(&map, x, y);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    }while(nOfAlive > 0 && !quit);
    free(map);
    return 0;
}
