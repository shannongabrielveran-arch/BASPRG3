#include "player.h"
#include "GameScene.h"

Player::~Player()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		delete bullets[i];
	}

	bullets.clear();
}

void Player::start()
{
	// Load texture
	// This only supports jpeg, png, and bitmaps
	texture = loadTexture("gfx/player.png");
	sound = SoundManager::loadSound("sound/196914__dpoggioli__laser-gun.ogg");

	// Initialize to avoid garbage values
	x = 100;
	y = 100;
	width = 0;
	height = 0;

	defaultSpeed = 5;
	boostedSpeed = 10;
	currentSpeed = defaultSpeed;

	reloadTime = 4; // About 0.067 seconds at 60 FPS
	currentReloadTime = 0;
// player.h (inside class Player, private:)
int defaultSpeed;
int boostedSpeed;

float reloadTime;
float currentReloadTime;
	
int wingReloadTime;    // <-- add this

std::vector<Bullet*> bullets;
	currentWingReloadTime = 0;

	// Query the texture to set our width and height
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

void Player::update()
{
	for (int i = 0; i < bullets.size();)
	{
		if (bullets[i]->GetX() > SCREEN_WIDTH)
		{
			Bullet* bulletToDelete = bullets[i];
			bullets.erase(bullets.begin() + i);
			delete bulletToDelete;
		}
		else
		{
			i++;
		}
	}

	if (currentReloadTime > 0)
	{
		currentReloadTime--;
	}
	if (currentWingReloadTime > 0)
	{
		currentWingReloadTime--;
	}

	if (app.keyboard[SDL_SCANCODE_F] && currentReloadTime <= 0)
	{
		SoundManager::playSound(sound);
		Bullet* bullet = new Bullet
		(
			x + width - 5,
			y + (height / 2) - 5,
			1,
			0,
			5
		);
		getScene()->addGameObject(bullet);
		bullets.push_back(bullet);

		currentReloadTime = reloadTime;
	}

	if (app.keyboard[SDL_SCANCODE_G] && currentWingReloadTime <= 0)
	{
		SoundManager::playSound(sound);
		// The ship faces right, with wingtips at its upper/lower rear corners.
		Bullet* upperBullet = new Bullet(x, y, 1, 0, 5);
		Bullet* lowerBullet = new Bullet(x, y + height - 5, 1, 0, 5);
		getScene()->addGameObject(upperBullet);
		getScene()->addGameObject(lowerBullet);
		bullets.push_back(upperBullet);
		bullets.push_back(lowerBullet);

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