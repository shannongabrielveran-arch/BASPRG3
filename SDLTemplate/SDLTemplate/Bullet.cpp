#include "Bullet.h"

Bullet::Bullet(
    int positionX,
    int positionY,
    int directionX,
    int directionY,
    int speed,
    bool enemyBullet
)
{
    this->x = positionX;
    this->y = positionY;

    this->directionX = directionX;
    this->directionY = directionY;

    this->speed = speed;

    this->enemyBullet = enemyBullet;

    texture = nullptr;

    width = 0;
    height = 0;
}

void Bullet::start()
{
    // Enemy bullets use alienBullet.png.
    // Player bullets use playerBullet.png.
    texture = loadTexture(
        enemyBullet
        ? "gfx/alienBullet.png"
        : "gfx/playerBullet.png"
    );

    SDL_QueryTexture(
        texture,
        NULL,
        NULL,
        &width,
        &height
    );
}

void Bullet::update()
{
    x += directionX * speed;
    y += directionY * speed;

    // Destroy bullet when it leaves the screen.
    if (
        x > SCREEN_WIDTH ||
        x + width < 0 ||
        y > SCREEN_HEIGHT ||
        y + height < 0
        )
    {
        destroy();
    }
}

void Bullet::draw()
{
    blit(texture, x, y);
}

int Bullet::GetX()
{
    return x;
}

int Bullet::GetY()
{
    return y;
}

int Bullet::GetWidth()
{
    return width;
}

int Bullet::GetHeight()
{
    return height;
}

bool Bullet::IsEnemyBullet()
{
    return enemyBullet;
}