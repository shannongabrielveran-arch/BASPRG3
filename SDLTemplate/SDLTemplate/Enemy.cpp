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
        texture, NULL, NULL, &width, &height
    );

    sound = SoundManager::loadSound(
        "sound/334227__jradcoolness__laser.ogg"
    );

    directionY = (rand() % 2) ? 1 : -1;
    directionChangeTime = 180 + rand() % 300;

    if (y + height > SCREEN_HEIGHT)
    {
        y = SCREEN_HEIGHT - height;
    }
}

void Enemy::update()
{
    // Move left and vertically.
    x -= speed;
    y += directionY * speed;

    // Occasionally reverse vertical direction.
    currentDirectionChangeTime++;

    if (currentDirectionChangeTime >= directionChangeTime)
    {
        directionY = -directionY;
        currentDirectionChangeTime = 0;
        directionChangeTime = 180 + rand() % 300;
    }

    // Stay inside the top and bottom edges.
    if (y < 0)
    {
        y = 0;
        directionY = 1;
    }

    if (y + height > SCREEN_HEIGHT)
    {
        y = SCREEN_HEIGHT - height;
        directionY = -1;
    }

    // Remove the enemy after it leaves the screen.
    if (x + width < 0)
    {
        destroy();
        return;
    }

    // Fire left every 60 frames.
    currentReloadTime--;

    if (currentReloadTime <= 0 && x < SCREEN_WIDTH)
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