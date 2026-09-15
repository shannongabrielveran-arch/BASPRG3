#include "Bullet.h"


Bullet::Bullet(int positionX, int positionY, int directionX, int directionY, int speed)
{
	this->x = positionX;
	this->y = positionY;
	this->directionX = directionX;
	this->directionY = directionY;
	this->speed = speed;
}

void Bullet::start()
{
	// Load texture
	// This only supports jpeg, png, and bitmaps
	texture = loadTexture("gfx/playerBullet.png");

	// Initialize to avoid garbage values
	width = 3;
	height = 1;

	// Query the texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void Bullet::update()
{
	x += directionX * speed; 5;
	y += directionY * speed; 5;
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