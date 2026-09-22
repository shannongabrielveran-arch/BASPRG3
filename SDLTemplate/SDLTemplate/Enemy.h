#pragma once
#include "GameObject.h"
#include "common.h"
#include "SoundManager.h"

class Enemy : public GameObject
{
public:
    Enemy(int positionX, int positionY);

    void start() override;
    void update() override;
    void draw() override;

private:
    int x, y;
    int width = 0;
    int height = 0;

    int directionY = 1;
    int speed = 2;

    int reloadTime = 60;
    int currentReloadTime = 60;

    int directionChangeTime = 180;
    int currentDirectionChangeTime = 0;

    SDL_Texture* texture = nullptr;
    Mix_Chunk* sound = nullptr;
};