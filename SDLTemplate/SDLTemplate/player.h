#pragma once

#include "GameObject.h"
#include "common.h"

class Player : public GameObject
{
public:
    void start() override;
    void update() override;
    void draw() override;

private:
    SDL_Texture* texture = nullptr;
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    int movementSpeed = 1;
};