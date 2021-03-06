#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
using namespace std;

const int WINDOW_HEIGHT = 900;
const int WINDOW_WIDTH = WINDOW_HEIGHT / 0.9;
const int GAME_HEIGHT = WINDOW_HEIGHT;
const int GAME_WIDTH = GAME_HEIGHT / 1.8;


const SDL_Color colorList[] = {
    { 255, 255, 255, 255 }, 
    { 255, 13, 13, 255 },
    { 50, 50, 50, 255 },
    { 80, 80, 80, 255 },
    { 30, 35, 247, 255 },
    { 0, 0, 0, 0},
    { 0, 0, 0, 255},
    { 30, 35, 247, 180},
    { 245, 254, 6, 255}
};
enum Color {
    white, red, darkGrey, lightGrey, blue, transparent, black, blueTranparent, yellow
};

class Game;
class Camera
{
public:
    double y = 8, speed = 1.5;
    const double speedIncreasePerStep = 0.005;
    bool stop, autoSpeed = 0;
    void update(double ratio)
    {
        if (stop)
            y = 0;
        else y = 8 * ratio;
    }
};

class Text
{
    SDL_Color textColor = colorList[white];
    TTF_Font *gFont = nullptr;
    string text;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    double ratio;
public:
    SDL_Rect desR;
    Text();
    Text(const string& _text, int x, int y, int fontSize, int fontType, Color textColor, SDL_Renderer* Orenderer, double _ratio);
    void updateText(const string& newText);
    void updateFont(int fontSize, int type);
    void updateColor(Color type);
    void updateTexture();
    void show();
    string takeText() {
        return text;
    }
};

class Block
{
    string name;
    Color colorType;
    SDL_Renderer* renderer;
    double ratio;
public:
    Text content;
    SDL_Rect bloR;
    bool isShown = 0;

    Block();
    Block(const string& _name, const SDL_Rect& rec, Color blockColor,
          const string& _text, int x, int y, int fontSize, int fontType, Color textColor, SDL_Renderer* Orenderer, double _ratio);
    void setText(Color colorType);
    void setColor(Color newColor);
    void show();
    void update(Game* game);
    void changePos(int x, int y);
    string getName() {
        return name;
    }
};

class PopUp
{
    Color colorType;
    double ratio;
    int limitMoveUpValue, limitMoveDownValue;

public:
    vector<Block> container;
    SDL_Rect desR;
    bool isShown = 0;

    PopUp(){}
    PopUp(const SDL_Rect& rec, double _ratio);
    void addBlock(const string& _name, const SDL_Rect& block, Color blockColor,//block so voi Popup, text sv block
          const string& _text, int x, int y, int fontSize, int fontType, Color textColor, SDL_Renderer* Orenderer);
    void setLimit(int limitUp, int limitDown);
    void setColor(Color newColor);
    void update(Game* game);
    void show(SDL_Renderer* renderer);
    bool visibleBlock(int i);
    int takeY_BasePopUp(int i);
    int limitMoveUp() { return limitMoveUpValue;  }
    int limitMoveDown() { return limitMoveDownValue;  }
};

struct AudioManager
{
    static Mix_Chunk* notesList[14][8];
    static Mix_Chunk* winnerChunk;
    static Mix_Music* menuMusic;
    static void playNote(const string& note, int channel, int time);
    static void addNote(const string& note);
};

struct TextureManager
{
    static SDL_Texture* takeTexture(const char* link, SDL_Renderer* renderer);
    static void drawImage(SDL_Texture* tex, SDL_Rect src, SDL_Rect des, SDL_Renderer* renderer);
};

class Tile
{
    int w, h, pos; static Uint32 curTick;
    double ratio;
    int channelCount = 4;
    bool touched = 0;
    int runNextTimeForChannel[8] = { 4,4,4,4,4,4,4,4 };//channelCount
public:
    SDL_Rect desR;
    string note[4][4];//~channelCount
    string bass[4][4];

    Tile(int width, int height, int stt, int prePos, double _ratio);
    void setNote(string _note, int channel, int consecutiveNotes, int isBass);
    int duration(int channel, int curpos, bool isNote);
    void playNote(int channel, bool isNote, int noteLength, int notePos, const Game* game);
    void rightFirstNote(Game* game);
    void show(SDL_Renderer* renderer);
    void handleInput(int posInput, Game* game);
    void update(int gobackLength, int stt, Game* game);
    int takePos() { return pos; }
    bool hadTouched(){ return touched; }
};

class Game
{
    SDL_Window* window;
    SDL_Texture* bg, * gameBg;
    SDL_Rect srcRGame, desRGame, srcRBg, desRBg;

public:
    double ratio;
    SDL_Renderer* renderer;
    bool isRunning; int fail; bool showWrongKey = 0;
    int cur1stSongInList = 1;
    int curTileID, tileCount, lastSeenID, score, highScore;
    Camera camera;
    int waitingTimeForAQuarterNote = 90;
    SDL_Rect wrongRect;
    vector<Tile> tileList;
    int curSongId;
    PopUp scoreTxt, highScoreTxt, failPopUp;
    Block speedTxt, autoPlay;
    PopUp speedPopUp, chooseSongPopUp, menu, manual, highScorePopUp;

    void init(const char* title);
    void render();
    void exit();
    void handleInput();
    void update();
    void clean();
};
#endif // GAME_H_INCLUDED
