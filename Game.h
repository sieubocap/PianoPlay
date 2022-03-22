#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#pragma once
#include <iostream>
#include <SDL.h>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
using namespace std;//currently resources is not sync, change link variable

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 900;
const int GAME_WIDTH = 500;
const int GAME_HEIGHT = 900;

class Global;
class Camera
{
public:
    double y = 8, speed = 1.5;
    bool stop;
    void update()
    {
        if (stop)
            y = 0;
        else y = 8;
    }
};

class Text
{
private:
    SDL_Color textColor = {255,255,255};
    TTF_Font *gFont = TTF_OpenFont("PianoPlay/TTFfonts/Fine College.ttf", 100);
    string text;
    SDL_Texture* texture;
public:
    SDL_Rect desR;
    Text();
    Text(const string& _text, int x, int y, int w, int h);
    void updateText(const string& newText, int _w);
    void updateFont(int font);
    void updateColor(int color);
    void updateTexture();
    void show();
    string takeText() {
        return text;
    }
};

class Block
{
private:
    string name;
public:
    Text content;
    SDL_Rect bloR;

    Block();
    Block(const string& _name, int blox, int bloy, int blow, int bloh,
          const string& _text, int x, int y, int w, int h);
    void setText(int font, int color);
    void show();
    void update();
    void changePos(int x, int y);
    string getName() {
        return name;
    }
};

class PopUp
{
public:
    vector<Block> container;
    SDL_Rect desR;
    int limitMoveUp, limitMoveDown;

    PopUp(int x, int y, int w, int h);
    void addBlock(const string& _name, int blox, int bloy, int blow, int bloh,//block so voi Popup, text sv block
          const string& _text, int x, int y, int w, int h);
    void update();
    void show();
    bool visibleBlock(int i);
    int takeY_BasePopUp(int i);
};

class AudioManager
{
public:
    static Mix_Chunk* notesList[14][8];
    static Mix_Chunk* winnerChunk;
    static void playNote(const string& note, int channel, int time)
    {
        Mix_Chunk* sound;
        if (note[1] >= '0' && note[1] <= '9')
            sound = notesList[note[0]-'A'][note[1]-'0'];
        else
            sound =  notesList[note[0]-'A'+7][note[2]-'0'];
        if (time == 0)
            Mix_PlayChannel(channel, sound, 0);
        else
            Mix_PlayChannelTimed(channel, sound, 0, time);
    }
    static void addNote(const string& note)
    {
        string s = "PianoPlay/pianoHub/piano-mp3/" + note + ".wav";
        if (note[1] >= '0' && note[1] <= '9')
            notesList[note[0]-'A'][note[1]-'0'] = Mix_LoadWAV(&s[0]);
        else
            notesList[note[0]-'A'+7][note[2]-'0'] = Mix_LoadWAV(&s[0]);
    }
};

class TextureManager
{
public:
    static SDL_Texture* takeTexture(const char* link);
    static void drawImage(SDL_Texture* tex, SDL_Rect src, SDL_Rect des);
};

class Tile
{
private:
    int w, h, pos; static Uint32 curTick;
    const int channelCount = 4;
    bool touched = 0, runSecondTimeForChannel[8] = {0,0,0,0,0,0,0,0};//channelCount
public:
    SDL_Rect desR;
    string note[4][2];//~channelCount
    string bass[4][2];
    Tile(int width, int height, int stt, int prePos);
    void setNote(string _note, int channel, bool isSecond, int isBass);
    void show();
    void handleInput(int posInput, int& fail, PopUp& scoreTxt, PopUp& highScoreTxt,
                     PopUp& failPopUp);
    void update(int& fail, int gobackLength, PopUp& scoreTxt, PopUp& highScoreTxt,
                PopUp& failPopUp);
    int takePos(){
        return pos;
    }
    bool hadTouched(){
        return touched;
    }
};

void init(const char* title, int xpos, int ypos,
              int width, int height, bool fullscreen, bool& isRunning, int& fail);
void render(int& fail);
void handleInput(bool& isRunning, int& fail);
void update(bool& isRunning, int& fail);
void clean();

class Global
{
public:
    static SDL_Renderer* renderer;
    static int curTileID, tileCount, lastSeenID, score, highScore;
    static SDL_Texture *bg, *gameBg;
    static Camera camera;
    static int waitingTimeForSecondNote;
    static SDL_Rect wrongRect;
    static bool showWrongKey;
};
#endif // GAME_H_INCLUDED
