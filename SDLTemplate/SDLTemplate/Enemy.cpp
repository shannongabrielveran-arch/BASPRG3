#include "Enemy.h"
#include "Bullet.h"
#include "Scene.h"
#include "SoundManager.h"

Enemy::Enemy(int positionX, int positionY)
    : x(positionX), y(positionY)
{
}

void Enemy::start()
{
    texture = loadTexture("gfx/enemy.png");

    SDL_QueryTexture(
        texture,
        NULL,
        NULL,
        &width,
        &height
    );

    sound = SoundManager::loadSound(
        "sound/334227__jradcoolness__laser.ogg"
    );

    // Randomly choose whether enemy starts moving
    // upward or downward.
    directionY = (rand() % 2) ? 1 : -1;

    directionChangeTime =
        180 + rand() % 300;

    // Prevent enemy from spawning below the screen.
    if (y + height > SCREEN_HEIGHT)
    {
        y = SCREEN_HEIGHT - height;
    }
}

void Enemy::update()
{
    // Move enemy toward the left.
    x -= speed;

    // Move enemy vertically.
    y += directionY * speed;

    // Occasionally change vertical direction.
    currentDirectionChangeTime++;

    if (
        currentDirectionChangeTime >=
        directionChangeTime
        )
    {
        directionY = -directionY;

        currentDirectionChangeTime = 0;

        directionChangeTime =
            180 + rand() % 300;
    }

    // Keep enemy inside the top of the screen.
    if (y < 0)
    {
        y = 0;
        directionY = 1;
    }

    // Keep enemy inside the bottom of the screen.
    if (y + height > SCREEN_HEIGHT)
    {
        y = SCREEN_HEIGHT - height;
        directionY = -1;
    }

    // Destroy enemy after it fully leaves the left side.
    if (x + width < 0)
    {
        destroy();
        return;
    }

    // Enemy shooting timer.
    currentReloadTime--;

    if (
        currentReloadTime <= 0 &&
        x < SCREEN_WIDTH
        )
    {
        Bullet* bullet = new Bullet(
            x,
            y + height / 2,
            -1,
            0,
            5,
            true
        );

        getScene()->addGameObject(bullet);

        SoundManager::playSound(sound);

        currentReloadTime = reloadTime;
    }
}

void Enemy::draw()
{
    blit(texture, x, y);
}

int Enemy::getX()
{
    return x;
}

int Enemy::getY()
{
    return y;
}

int Enemy::getWidth()
{
    return width;
}

int Enemy::getHeight()
{
    return height;
}