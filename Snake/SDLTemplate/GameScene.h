#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Snake.h"
#include "Food.h"
#include "text.h"
#include <vector>

enum GameState
{
	STATE_READY,      // waiting for the first arrow key
	STATE_PLAYING,
	STATE_GAME_OVER
};

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();
	void start();
	void draw();
	void update();
private:
	Snake* snake;
	Food* food;
	SDL_Texture* appleIcon;

	GameState state;
	int score;
	int highScore;
	bool newHighScore;  // true when this round beat the saved best
	int moveTimer;      // counts frames until the next snake step
	int moveDelay;      // frames between steps (smaller = faster)
	int gameOverTimer;
	bool won;

	// Game flow
	void resetGame();
	void stepSnake();
	void eatFood();
	bool spawnFood();
	void endGame(bool playerWon);
	bool isRestartPressed();

	// High score (saved to HIGH_SCORE_FILE)
	void loadHighScore();
	void saveHighScore();

	// Drawing
	void drawBoard();
	void drawHud();
	void drawOverlay(int top, int height, Uint8 alpha);
	void drawReadyScreen();
	void drawGameOverScreen();
};
