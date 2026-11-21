#include "Food.h"

// How fast and how much the apple pulses
static const float PULSE_SPEED = 0.12f;
static const float PULSE_AMOUNT = 0.08f;

Food::Food()
{
	cell.x = 0;
	cell.y = 0;
	texture = NULL;
	pulseTimer = 0;
}

void Food::start()
{
	texture = loadTexture("gfx/apple.png");
}

void Food::update()
{
	pulseTimer += PULSE_SPEED;
}

void Food::draw()
{
	if (texture == NULL) return;

	// Grow and shrink a little around the middle of the cell
	float scale = 1.0f + PULSE_AMOUNT * sinf(pulseTimer);
	int size = (int)(CELL_SIZE * scale);

	SDL_Rect dest;
	dest.w = size;
	dest.h = size;
	dest.x = cell.x * CELL_SIZE + (CELL_SIZE - size) / 2;
	dest.y = HUD_HEIGHT + cell.y * CELL_SIZE + (CELL_SIZE - size) / 2;

	SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void Food::setCell(int x, int y)
{
	cell.x = x;
	cell.y = y;
	pulseTimer = 0;
}

SDL_Point Food::getCell() const
{
	return cell;
}
