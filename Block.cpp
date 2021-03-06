#include "Game.h"

Block::Block(){}
Block::Block(const string& _name, const SDL_Rect& rec, Color blockColor,
          const string& _text, int x, int y, int fontSize, int fontType, Color textColor, SDL_Renderer* Orenderer, double _ratio){
    renderer = Orenderer; ratio = _ratio;
    bloR = {int(rec.x * ratio), int(rec.y * ratio), int(rec.w * ratio), int(rec.h * ratio)};
    colorType = blockColor;
    content = Text(_text, (x * ratio + bloR.x)/ratio + 1, (y * ratio + bloR.y)/ratio + 1, fontSize, fontType, textColor, renderer, ratio);
    name = _name;
}

void Block::setText(Color colorType){
    content.updateColor(colorType);
}

void Block::show(){
    SDL_SetRenderDrawColor(renderer, colorList[colorType].r,
        colorList[colorType].g,
        colorList[colorType].b,
        colorList[colorType].a);
    SDL_RenderFillRect(renderer, &bloR);
    content.show();
}

void Block::setColor(Color newColor) {
    colorType = newColor;
}

void Block::update(Game* game){
    if (name == "score")
        content.updateText("Your score: " + to_string(game->score));
    else if (name == "scoreOnlyNum"){
        string test = to_string(game->score);
        content.updateText(test);
    }
    else if (name == "failTitle"){
        if (game->score == game->highScore && game->score > 0) {
            game->highScorePopUp.container[1].content.updateText("Best Score: " + to_string(game->highScore));
            content.updateText("High Score!!");
            setColor(blue); setText(white);
        }
        else {
            content.updateText("You lose!");
            setColor(darkGrey); setText(red);
        }
    }
}

void Block::changePos(int x, int y) {
    bloR.x += x; content.desR.x += x;
    bloR.y += y; content.desR.y += y;
}