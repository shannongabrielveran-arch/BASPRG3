#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include "SoundManager.h"
#include "Bullet.h"
#include <vector>

class Player : public GameObject
{
public:
	~Player();
	void start() override;
	void update() override;
	void draw() override;
	int getX();
	int getY();
private:
	int x;
	int y;
	int width;
	int height;
	SDL_Texture* texture;
	int currentSpeed;

	Mix_Chunk* sound;

	int defaultSpeed;
	int boostedSpeed;

	float reloadTime;
	float currentReloadTime;

	float wingReloadTime;
	float currentWingReloadTime;

	std::vector<Bullet*> bullets;
};