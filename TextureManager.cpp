#include "Game.h"

TextureManager::takeTexture(const char* link){
    SDL_Surface* bg = IMG_Load(link);
    if (!bg)
        printf("IMG_Load: %s\n", IMG_GetError());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bg);
    SDL_FreeSurface(bg);
    return texture;
}

TextureManager::drawImage(const char* link, SDL_Rect src, SDL_Rect des){
    SDL_RenderCopy(renderer,takeTexture(link, renderer),&src,&des);
}
