#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"

// The apple. Sits on one grid cell and gently pulses so it's easy to spot.
class Food : public GameObject
{
public:
	Food();
	void start();
	void update();
	void draw();

	void setCell(int x, int y);
	SDL_Point getCell() const;

private:
	SDL_Point cell;
	SDL_Texture* texture;
	float pulseTimer;
};
