#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"

class Bullet : public GameObject
{
public:
    Bullet(
        int positionX,
        int positionY,
        int directionX,
        int directionY,
        int speed,
        bool enemyBullet = false
    );

    void start() override;
    void update() override;
    void draw() override;

    int GetX();
    int GetY();

private:
    bool enemyBullet;

    int x;
    int y;
    int width;
    int height;
    int speed;
    int directionX;
    int directionY;

    SDL_Texture* texture;
};