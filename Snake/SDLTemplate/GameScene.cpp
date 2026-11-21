#include "GameScene.h"
#include <fstream>
#include <string>

// Colours used by the board and the text
static const SDL_Color GRASS_LIGHT = { 170, 215, 81, 255 };
static const SDL_Color GRASS_DARK = { 162, 209, 73, 255 };
static const SDL_Color HUD_GREEN = { 74, 117, 44, 255 };
static const SDL_Color WHITE = { 255, 255, 255, 255 };
static const SDL_Color LIGHT_GREY = { 220, 220, 220, 255 };
static const SDL_Color RED = { 255, 90, 90, 255 };
static const SDL_Color GOLD = { 255, 230, 90, 255 };

static void setDrawColor(const SDL_Color& color)
{
	SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
}

static void drawTextColored(int x, int y, const SDL_Color& color, int align, const std::string& text)
{
	drawText(x, y, color.r, color.g, color.b, align, "%s", text.c_str());
}

GameScene::GameScene()
{
	// Register and add game objects on constructor
	snake = new Snake();
	this->addGameObject(snake);

	food = new Food();
	this->addGameObject(food);

	appleIcon = NULL;
	state = STATE_READY;
	score = 0;
	highScore = 0;
	newHighScore = false;
	moveTimer = 0;
	moveDelay = START_MOVE_DELAY;
	gameOverTimer = 0;
	won = false;
}

GameScene::~GameScene()
{
	// The Scene deletes its game objects (snake and food)
}

void GameScene::start()
{
	Scene::start();
	// Initialize any scene logic here
	initFonts();
	appleIcon = loadTexture("gfx/apple.png");
	loadHighScore();
	resetGame();
}

// ---------------------------------------------------------------------------
// Game flow
// ---------------------------------------------------------------------------

// Restarts the level: snake back to the start, new apple, score back to 0
void GameScene::resetGame()
{
	snake->reset();
	spawnFood();

	score = 0;
	newHighScore = false;
	moveTimer = 0;
	moveDelay = START_MOVE_DELAY;
	gameOverTimer = 0;
	won = false;
	state = STATE_READY;
}

// Puts the apple on a random empty cell. Returns false if the board is full.
bool GameScene::spawnFood()
{
	// Mark every cell the snake is on (one pass over the snake)
	std::vector<bool> occupied(GRID_COLS * GRID_ROWS, false);
	for (const SDL_Point& part : snake->getBody())
	{
		occupied[part.y * GRID_COLS + part.x] = true;
	}

	// Collect the free cells and pick one at random
	std::vector<SDL_Point> emptyCells;
	emptyCells.reserve(GRID_COLS * GRID_ROWS);
	for (int y = 0; y < GRID_ROWS; y++)
	{
		for (int x = 0; x < GRID_COLS; x++)
		{
			if (!occupied[y * GRID_COLS + x])
			{
				SDL_Point c = { x, y };
				emptyCells.push_back(c);
			}
		}
	}

	if (emptyCells.empty()) return false;

	SDL_Point pick = emptyCells[rand() % emptyCells.size()];
	food->setCell(pick.x, pick.y);
	return true;
}

// One step of the snake: check what's in front, then move
void GameScene::stepSnake()
{
	SDL_Point next = snake->getNextHead();
	SDL_Point apple = food->getCell();

	bool hitWall = next.x < 0 || next.x >= GRID_COLS || next.y < 0 || next.y >= GRID_ROWS;
	bool eating = (next.x == apple.x && next.y == apple.y);
	// If we're not eating, the tail moves away this step, so the head can go where the tail is now
	bool hitSelf = snake->isOnCell(next.x, next.y, !eating);

	if (hitWall || hitSelf)
	{
		endGame(false);
		return;
	}

	snake->move(eating);

	if (eating) eatFood();
}

void GameScene::eatFood()
{
	score++;

	// Save right away so the high score is kept even if the window is closed mid-game
	if (score > highScore)
	{
		highScore = score;
		newHighScore = true;
		saveHighScore();
	}

	// Speed up every few apples
	moveDelay = START_MOVE_DELAY - score / APPLES_PER_SPEEDUP;
	if (moveDelay < MIN_MOVE_DELAY) moveDelay = MIN_MOVE_DELAY;

	if (!spawnFood()) endGame(true);   // no space left = you filled the board!
}

void GameScene::endGame(bool playerWon)
{
	won = playerWon;
	state = STATE_GAME_OVER;
	gameOverTimer = 0;
	snake->setAlive(false);
}

// R restarts at any time. On the game over screen, Space or Enter also work.
bool GameScene::isRestartPressed()
{
	bool restart = app.keyboard[SDL_SCANCODE_R] != 0;
	if (state == STATE_GAME_OVER)
	{
		restart = restart || app.keyboard[SDL_SCANCODE_SPACE] || app.keyboard[SDL_SCANCODE_RETURN];
	}

	if (restart)
	{
		app.keyboard[SDL_SCANCODE_R] = 0;
		app.keyboard[SDL_SCANCODE_SPACE] = 0;
		app.keyboard[SDL_SCANCODE_RETURN] = 0;
	}
	return restart;
}

void GameScene::update()
{
	Scene::update();   // the snake reads the arrow keys here

	switch (state)
	{
	case STATE_READY:
		if (snake->wasKeyPressed())
		{
			state = STATE_PLAYING;
			moveTimer = moveDelay;   // take the first step right away
		}
		break;

	case STATE_PLAYING:
		if (isRestartPressed())
		{
			resetGame();
			break;
		}
		moveTimer++;
		if (moveTimer >= moveDelay)
		{
			moveTimer = 0;
			stepSnake();
		}
		break;

	case STATE_GAME_OVER:
		gameOverTimer++;
		// Small wait so a key you were already pressing doesn't restart instantly
		if (gameOverTimer > RESTART_DELAY_FRAMES && isRestartPressed())
		{
			resetGame();
		}
		break;
	}
}

// ---------------------------------------------------------------------------
// High score
// ---------------------------------------------------------------------------

void GameScene::loadHighScore()
{
	std::ifstream file(HIGH_SCORE_FILE);
	int saved = 0;
	if (file >> saved && saved > 0)
	{
		highScore = saved;
	}
}

void GameScene::saveHighScore()
{
	std::ofstream file(HIGH_SCORE_FILE);
	if (file)
	{
		file << highScore;
	}
}

// ---------------------------------------------------------------------------
// Drawing
// ---------------------------------------------------------------------------

// Checkerboard grass + the score bar
void GameScene::drawBoard()
{
	SDL_Rect hud = { 0, 0, SCREEN_WIDTH, HUD_HEIGHT };
	setDrawColor(HUD_GREEN);
	SDL_RenderFillRect(app.renderer, &hud);

	for (int y = 0; y < GRID_ROWS; y++)
	{
		for (int x = 0; x < GRID_COLS; x++)
		{
			setDrawColor((x + y) % 2 == 0 ? GRASS_LIGHT : GRASS_DARK);
			SDL_Rect cell = { x * CELL_SIZE, HUD_HEIGHT + y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
			SDL_RenderFillRect(app.renderer, &cell);
		}
	}
}

void GameScene::drawHud()
{
	if (appleIcon != NULL) blit(appleIcon, 8, 0);
	drawTextColored(56, 6, WHITE, TEXT_LEFT, std::to_string(score));
	drawTextColored(SCREEN_WIDTH / 2, 6, WHITE, TEXT_CENTER, "SNAKE");
	drawTextColored(SCREEN_WIDTH - 16, 6, WHITE, TEXT_RIGHT, "HIGH SCORE: " + std::to_string(highScore));
}

// See-through dark band behind the text messages
void GameScene::drawOverlay(int top, int height, Uint8 alpha)
{
	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, alpha);
	SDL_Rect band = { 0, top, SCREEN_WIDTH, height };
	SDL_RenderFillRect(app.renderer, &band);
	SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}

void GameScene::drawReadyScreen()
{
	int middle = HUD_HEIGHT + (SCREEN_HEIGHT - HUD_HEIGHT) / 2;
	drawOverlay(middle + 50, 90, 110);
	drawTextColored(SCREEN_WIDTH / 2, middle + 60, WHITE, TEXT_CENTER, "PRESS AN ARROW KEY TO START");
	drawTextColored(SCREEN_WIDTH / 2, middle + 100, LIGHT_GREY, TEXT_CENTER, "(OR W A S D)   R: RESTART");
}

void GameScene::drawGameOverScreen()
{
	int middle = HUD_HEIGHT + (SCREEN_HEIGHT - HUD_HEIGHT) / 2;
	drawOverlay(HUD_HEIGHT, SCREEN_HEIGHT - HUD_HEIGHT, 140);

	if (won)
		drawTextColored(SCREEN_WIDTH / 2, middle - 90, GOLD, TEXT_CENTER, "YOU WIN!");
	else
		drawTextColored(SCREEN_WIDTH / 2, middle - 90, RED, TEXT_CENTER, "GAME OVER");

	drawTextColored(SCREEN_WIDTH / 2, middle - 40, WHITE, TEXT_CENTER, "SCORE: " + std::to_string(score));
	drawTextColored(SCREEN_WIDTH / 2, middle, WHITE, TEXT_CENTER, "HIGH SCORE: " + std::to_string(highScore));
	if (newHighScore)
		drawTextColored(SCREEN_WIDTH / 2, middle + 40, GOLD, TEXT_CENTER, "NEW HIGH SCORE!");

	drawTextColored(SCREEN_WIDTH / 2, middle + 100, LIGHT_GREY, TEXT_CENTER, "PRESS SPACE OR R TO PLAY AGAIN");
}

void GameScene::draw()
{
	drawBoard();
	Scene::draw();   // snake and apple
	drawHud();

	if (state == STATE_READY) drawReadyScreen();
	else if (state == STATE_GAME_OVER) drawGameOverScreen();
}
