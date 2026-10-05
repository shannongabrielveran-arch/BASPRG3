#pragma once

#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"

class Explosion : public GameObject
{
public:
    Explosion(
        int positionX,
        int positionY
    );

    void start() override;
    void update() override;
    void draw() override;

private:
    int x;
    int y;

    int width;
    int height;

    int lifeTime;
    int currentLifeTime;

    SDL_Texture* texture;
    Mix_Chunk* sound;
};