#include "Snake.h"

Snake::Snake()
{
	keyPressed = false;
	alive = true;
	direction.x = 1;
	direction.y = 0;

	headUp = headDown = headLeft = headRight = NULL;
	tailUp = tailDown = tailLeft = tailRight = NULL;
	bodyVertical = bodyHorizontal = NULL;
	bodyTopLeft = bodyTopRight = bodyBottomLeft = bodyBottomRight = NULL;
}

void Snake::start()
{
	headUp = loadTexture("gfx/head_up.png");
	headDown = loadTexture("gfx/head_down.png");
	headLeft = loadTexture("gfx/head_left.png");
	headRight = loadTexture("gfx/head_right.png");

	tailUp = loadTexture("gfx/tail_up.png");
	tailDown = loadTexture("gfx/tail_down.png");
	tailLeft = loadTexture("gfx/tail_left.png");
	tailRight = loadTexture("gfx/tail_right.png");

	bodyVertical = loadTexture("gfx/body_vertical.png");
	bodyHorizontal = loadTexture("gfx/body_horizontal.png");
	bodyTopLeft = loadTexture("gfx/body_topleft.png");
	bodyTopRight = loadTexture("gfx/body_topright.png");
	bodyBottomLeft = loadTexture("gfx/body_bottomleft.png");
	bodyBottomRight = loadTexture("gfx/body_bottomright.png");

	reset();
}

void Snake::reset()
{
	body.clear();
	int startX = GRID_COLS / 4;
	int startY = GRID_ROWS / 2;
	for (int i = 0; i < START_LENGTH; i++)
	{
		SDL_Point part = { startX - i, startY };
		body.push_back(part);
	}

	direction.x = 1;
	direction.y = 0;
	turnQueue.clear();
	keyPressed = false;
	alive = true;
}

void Snake::update()
{
	if (!alive) return;

	// Arrow keys or WASD
	handleKey(SDL_SCANCODE_UP, SDL_SCANCODE_W, 0, -1);
	handleKey(SDL_SCANCODE_DOWN, SDL_SCANCODE_S, 0, 1);
	handleKey(SDL_SCANCODE_LEFT, SDL_SCANCODE_A, -1, 0);
	handleKey(SDL_SCANCODE_RIGHT, SDL_SCANCODE_D, 1, 0);
}

// If either key is down, queue a turn. The keys are set back to 0 so one press = one turn
void Snake::handleKey(SDL_Scancode arrowKey, SDL_Scancode letterKey, int dx, int dy)
{
	if (app.keyboard[arrowKey] || app.keyboard[letterKey])
	{
		app.keyboard[arrowKey] = 0;
		app.keyboard[letterKey] = 0;
		queueTurn(dx, dy);
	}
}

void Snake::queueTurn(int dx, int dy)
{
	keyPressed = true;

	// Compare with the last turn waiting in line (or the current direction)
	SDL_Point last = turnQueue.empty() ? direction : turnQueue.back();

	// Already going that way
	if (last.x == dx && last.y == dy) return;
	// Can't turn straight back into your own body
	if (last.x == -dx && last.y == -dy) return;

	// Remember at most 2 turns so quick presses aren't lost
	if (turnQueue.size() < 2)
	{
		SDL_Point turn = { dx, dy };
		turnQueue.push_back(turn);
	}
}

SDL_Point Snake::getMoveDirection() const
{
	// The next turn in line is used on the next step
	if (!turnQueue.empty()) return turnQueue.front();
	return direction;
}

SDL_Point Snake::getNextHead() const
{
	SDL_Point dir = getMoveDirection();
	SDL_Point next = { body.front().x + dir.x, body.front().y + dir.y };
	return next;
}

void Snake::move(bool grow)
{
	SDL_Point next = getNextHead();
	direction = getMoveDirection();
	if (!turnQueue.empty()) turnQueue.pop_front();

	// New head at the front
	body.push_front(next);

	// Drop the tail unless we just ate
	if (!grow) body.pop_back();
}

bool Snake::isOnCell(int x, int y, bool ignoreTail) const
{
	int count = (int)body.size();
	if (ignoreTail) count--;

	for (int i = 0; i < count; i++)
	{
		if (body[i].x == x && body[i].y == y) return true;
	}
	return false;
}

bool Snake::wasKeyPressed()
{
	bool result = keyPressed;
	keyPressed = false;
	return result;
}

void Snake::setAlive(bool isAlive)
{
	alive = isAlive;
}

int Snake::getLength() const
{
	return (int)body.size();
}

const std::deque<SDL_Point>& Snake::getBody() const
{
	return body;
}

// Picks the right sprite for each part by looking at its neighbours
SDL_Texture* Snake::getPartTexture(int index) const
{
	int last = (int)body.size() - 1;

	// Head: faces the way it's moving (away from the part behind it)
	if (index == 0)
	{
		int dx = (last > 0) ? body[0].x - body[1].x : direction.x;
		int dy = (last > 0) ? body[0].y - body[1].y : direction.y;
		if (dx == 1) return headRight;
		if (dx == -1) return headLeft;
		if (dy == -1) return headUp;
		return headDown;
	}

	// Tail: points away from the part in front of it
	if (index == last)
	{
		int dx = body[last].x - body[last - 1].x;
		int dy = body[last].y - body[last - 1].y;
		if (dx == 1) return tailRight;
		if (dx == -1) return tailLeft;
		if (dy == -1) return tailUp;
		return tailDown;
	}

	// Middle parts: where are the part in front (a) and the part behind (b)?
	SDL_Point a = { body[index - 1].x - body[index].x, body[index - 1].y - body[index].y };
	SDL_Point b = { body[index + 1].x - body[index].x, body[index + 1].y - body[index].y };

	if (a.x == b.x) return bodyVertical;     // one above, one below
	if (a.y == b.y) return bodyHorizontal;   // one left, one right

	// Corner: connects two sides
	bool up = (a.y == -1 || b.y == -1);
	bool left = (a.x == -1 || b.x == -1);
	if (up && left) return bodyTopLeft;
	if (up) return bodyTopRight;
	if (left) return bodyBottomLeft;
	return bodyBottomRight;
}

void Snake::draw()
{
	// Tint the snake red when it dies
	Uint8 green = (Uint8)(alive ? 255 : 110);
	Uint8 blue = (Uint8)(alive ? 255 : 110);

	// Draw from tail to head so the head is on top
	for (int i = (int)body.size() - 1; i >= 0; i--)
	{
		SDL_Texture* texture = getPartTexture(i);
		if (texture == NULL) continue;

		SDL_SetTextureColorMod(texture, 255, green, blue);
		blit(texture, body[i].x * CELL_SIZE, HUD_HEIGHT + body[i].y * CELL_SIZE);
	}
}
