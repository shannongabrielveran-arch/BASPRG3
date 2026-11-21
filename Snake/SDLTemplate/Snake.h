#pragma once
#include "GameObject.h"
#include "common.h"
#include "draw.h"
#include <deque>

// The player's snake. It lives on a grid: every body part is one cell (x, y).
// GameScene decides WHEN the snake moves; the snake handles input, moving and drawing.
class Snake : public GameObject
{
public:
	Snake();
	void start();
	void update();
	void draw();

	// Puts the snake back at the start: START_LENGTH parts long, facing right
	void reset();
	// The cell the head will move into on the next step
	SDL_Point getNextHead() const;
	// Moves the snake one cell. If grow is true, the tail stays put so the snake gets 1 longer
	void move(bool grow);
	// True if any part of the snake is on cell (x, y). ignoreTail skips the last part
	bool isOnCell(int x, int y, bool ignoreTail) const;
	// True (once) if the player pressed a direction key since the last check
	bool wasKeyPressed();
	void setAlive(bool isAlive);
	int getLength() const;
	// All body parts, head first (read only)
	const std::deque<SDL_Point>& getBody() const;

private:
	std::deque<SDL_Point> body;       // body[0] is the head, body.back() is the tail
	SDL_Point direction;              // the way the snake moved last step
	std::deque<SDL_Point> turnQueue;  // turns pressed but not made yet (so fast double-turns work)
	bool keyPressed;
	bool alive;

	// Sprites
	SDL_Texture* headUp;
	SDL_Texture* headDown;
	SDL_Texture* headLeft;
	SDL_Texture* headRight;
	SDL_Texture* tailUp;
	SDL_Texture* tailDown;
	SDL_Texture* tailLeft;
	SDL_Texture* tailRight;
	SDL_Texture* bodyVertical;
	SDL_Texture* bodyHorizontal;
	SDL_Texture* bodyTopLeft;
	SDL_Texture* bodyTopRight;
	SDL_Texture* bodyBottomLeft;
	SDL_Texture* bodyBottomRight;

	void handleKey(SDL_Scancode arrowKey, SDL_Scancode letterKey, int dx, int dy);
	void queueTurn(int dx, int dy);
	SDL_Point getMoveDirection() const;
	SDL_Texture* getPartTexture(int index) const;
};
