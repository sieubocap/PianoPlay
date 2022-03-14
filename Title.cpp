#include "Game.h"

Tile::Tile(int width, int height, int stt, int prePos)
{
    w = width; h = height;
    pos = rand()%4;
    if (pos == prePos)
        pos = (pos + 1)%4;
    srcR.h = desR.h = h;
    srcR.w = desR.w = w;
    srcR.x = srcR.y = 0;
    desR.x = pos * w;
    desR.y = -h - stt * h;
}

void Tile::setNote(string _note, int channel, bool isSecond, int isBass)
{
    if (!isBass){
        if (!isSecond)
            note[channel][0] = _note;
        else note[channel][1] = _note;
    }
    else{
        if (!isSecond)
            bass[channel][0] = _note;
        else bass[channel][1] = _note;
    }
}

void Tile::show()
{
    if (!touched)
        TextureManager::drawImage(Global::unclick, srcR, desR);
    else
        TextureManager::drawImage(Global::click, srcR, desR);
}

void pauseAllChannel(bool type, int channelCount)
{
    int start, en;
    if (!type)
        start = 0, en = channelCount;
    else
        start = channelCount, en = channelCount * 2;
    for (int chan = start; chan < en; chan++)
        if (Mix_Playing(chan))
            Mix_Pause(chan);
}

void Tile::handleInput(int posInput, int& fail)
{
    cout << Global::curTileID << '\n';
    if (posInput == pos && desR.y + h >= 0){
    //if (desR.y > 500 && !touched){
        cout << SDL_GetTicks() - curTick << '\n';
        curTick = SDL_GetTicks();
        touched = 1, Global::curTileID++;
        for (int channel = 0; channel < channelCount; channel++){
            if (note[channel][0] != ""){
                if (note[channel][0] == "!")
                    pauseAllChannel(0, channelCount);
                else if (note[channel][1] == "")
                    AudioManager::playNote(note[channel][0], channel, 0);
                else
                    AudioManager::playNote(note[channel][0], channel,
                                           Global::waitingTimeForSecondNote / Global::camera.speed),
                    runSecondTimeForChannel[channel] = 1;
            }
            else if (note[channel][1] != "")
                runSecondTimeForChannel[channel] = 1;
            if (bass[channel][0] != ""){
                if (bass[channel][0] == "!")
                    pauseAllChannel(1, channelCount);
                else if (bass[channel][1] == "")
                    AudioManager::playNote(bass[channel][0], channel + channelCount, 0);
                else
                    AudioManager::playNote(bass[channel][0], channel + channelCount,
                                           Global::waitingTimeForSecondNote / Global::camera.speed),
                    runSecondTimeForChannel[channel + channelCount] = 1;
            }
            else if (bass[channel][1] != "")
                runSecondTimeForChannel[channel + channelCount] = 1;
        }
    }
    else{
        AudioManager::playNote("A0", 0, 0);
        fail = 1, cout << "You fail because of wrong key\n";
        Global::camera.stop = 1, Global::lastSeenID = Global::curTileID;
    }
}

void Tile::update(int& fail, int gobackLength)
{
    if (fail){
        desR.y -= gobackLength;
        return;
    }
    desR.y += int(Global::camera.y * Global::camera.speed);
    if (desR.y > WINDOW_HEIGHT && !touched){
        AudioManager::playNote("A0", 0, 0);
        fail = 1, cout << "You fail because of untouched\n";
        Global::camera.stop = 1, Global::lastSeenID = Global::curTileID;
        desR.y -= (gobackLength + int(Global::camera.y * Global::camera.speed));
    }
    for (int channel = 0; channel < channelCount * 2; channel++)
        if (runSecondTimeForChannel[channel]){
            if (!Mix_Playing(channel)){
                if (channel < channelCount){
                    if (note[channel][1] == "!")
                        pauseAllChannel(0, channelCount);
                    else
                        AudioManager::playNote(note[channel][1], channel, 0);
                    runSecondTimeForChannel[channel] = 0;
                }
                else{
                    if (bass[channel - channelCount][1] == "!")
                        pauseAllChannel(1, channelCount);
                    else
                        AudioManager::playNote(bass[channel - channelCount][1], channel, 0);
                    runSecondTimeForChannel[channel] = 0;
                }
            }
            else if (SDL_GetTicks() - curTick >= int(Global::waitingTimeForSecondNote / Global::camera.speed)){
                if (channel < channelCount && note[channel][0] == ""){
                    if (note[channel][1] == "!")
                        pauseAllChannel(0, channelCount);
                    else
                        AudioManager::playNote(note[channel][1], channel, 0);
                    runSecondTimeForChannel[channel] = 0;
                }
                else if (channel >= channelCount && bass[channel - channelCount][0] == ""){
                    if (bass[channel - channelCount][1] == "!")
                        pauseAllChannel(1, channelCount);
                    else
                        AudioManager::playNote(bass[channel - channelCount][1], channel, 0);
                    runSecondTimeForChannel[channel] = 0;
                }
            }
    }
}
