#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"
#include "Bullet.h"

class Player : public GameObject
{
public:
    ~Player() override;

    void start() override;
    void update() override;
    void draw() override;

    int getX();
    int getY();

private:
    int x;
    int y;
    int width;
    int height;

    SDL_Texture* texture;
    Mix_Chunk* sound;

    int currentSpeed;
    int defaultSpeed;
    int boostedSpeed;

    float reloadTime;
    float currentReloadTime;

    float wingReloadTime;
    float currentWingReloadTime;
};