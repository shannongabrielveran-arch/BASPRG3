#include "player.h"
#include "GameScene.h"

Player::~Player()
{
    // Scene handles deleting bullets.
}

void Player::start()
{
    texture = loadTexture("gfx/player.png");

    sound = SoundManager::loadSound(
        "sound/196914__dpoggioli__laser-gun.ogg"
    );

    x = 100;
    y = 100;
    width = 0;
    height = 0;

    defaultSpeed = 5;
    boostedSpeed = 10;
    currentSpeed = defaultSpeed;

    reloadTime = 4;
    currentReloadTime = 0;

    wingReloadTime = 4;
    currentWingReloadTime = 0;

    SDL_QueryTexture(
        texture, NULL, NULL, &width, &height
    );
}

void Player::update()
{
    if (currentReloadTime > 0)
    {
        currentReloadTime--;
    }

    if (currentWingReloadTime > 0)
    {
        currentWingReloadTime--;
    }

    // Main gun.
    if (
        app.keyboard[SDL_SCANCODE_F] &&
        currentReloadTime <= 0
        )
    {
        SoundManager::playSound(sound);

        Bullet* bullet = new Bullet(
            x + width - 5,
            y + height / 2 - 5,
            1,
            0,
            5
        );

        getScene()->addGameObject(bullet);

        currentReloadTime = reloadTime;
    }

    // Wing guns.
    if (
        app.keyboard[SDL_SCANCODE_G] &&
        currentWingReloadTime <= 0
        )
    {
        SoundManager::playSound(sound);

        Bullet* upperBullet = new Bullet(
            x, y, 1, 0, 5
        );

        Bullet* lowerBullet = new Bullet(
            x, y + height - 5, 1, 0, 5
        );

        getScene()->addGameObject(upperBullet);
        getScene()->addGameObject(lowerBullet);

        currentWingReloadTime = wingReloadTime;
    }

    if (app.keyboard[SDL_SCANCODE_LSHIFT])
    {
        currentSpeed = boostedSpeed;
    }

    if (app.keyboard[SDL_SCANCODE_BACKSPACE])
    {
        currentSpeed = defaultSpeed;
    }

    if (app.keyboard[SDL_SCANCODE_W])
    {
        y -= currentSpeed;
    }

    if (app.keyboard[SDL_SCANCODE_S])
    {
        y += currentSpeed;
    }

    if (app.keyboard[SDL_SCANCODE_A])
    {
        x -= currentSpeed;
    }

    if (app.keyboard[SDL_SCANCODE_D])
    {
        x += currentSpeed;
    }
}

void Player::draw()
{
    blit(texture, x, y);
}

int Player::getX()
{
    return x;
}

int Player::getY()
{
    return y;
}