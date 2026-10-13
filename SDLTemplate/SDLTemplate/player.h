
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

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

    bool hasShield() const
    {
        return shieldCharges > 0;
    }

    bool consumeShield();
    bool receiveHit();

    void activatePowerUp(int type);

    int getHealth() const
    {
        return health;
    }

    int getMiniShipSeconds() const
    {
        return miniShipFrames / FPS;
    }

    int getShotgunSeconds() const
    {
        return shotgunFrames / FPS;
    }

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

    // Power-up variables.
    int shieldCharges = 0;
    int miniShipFrames = 0;
    int shotgunFrames = 0;
    int miniShipReload = 0;

    // Health and damage protection.
    int health = 3;
    int invulnerableFrames = 0;

    float wingReloadTime;
    float currentWingReloadTime;
};
