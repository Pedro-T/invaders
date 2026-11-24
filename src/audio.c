#include "raylib.h"


struct Sounds {
    Sound playerFireSound;
    Sound gameOver;
    Sound playerHit;
    Sound playerDestruction;
} Sounds;

void initAudio()
{
    InitAudioDevice();
    Sounds.playerFireSound = LoadSound("sounds/fire.mp3");
    Sounds.gameOver = LoadSound("sounds/game-over.mp3");
    Sounds.playerHit = LoadSound("sounds/hit.mp3");
    Sounds.playerDestruction = LoadSound("sounds/dead.mp3");
}

void playFireSound()
{
    PlaySound(Sounds.playerFireSound);
}

void playGameOver()
{
    PlaySound(Sounds.gameOver);
}

void playHit()
{
    PlaySound(Sounds.playerHit);
}

void playPlayerDestruction()
{
    PlaySound(Sounds.playerDestruction);
}

void unloadAudio()
{
    UnloadSound(Sounds.playerFireSound);
    UnloadSound(Sounds.gameOver);
    UnloadSound(Sounds.playerHit);
    UnloadSound(Sounds.playerDestruction);
}