#pragma once

#define STRNCPY(dest, src, n) strncpy(dest, src, n); dest[n - 1] = '\0'
#define WINDOW_NAME				"SNAKE"
#define FPS                      60
#define MAX_KEYBOARD_KEYS        350
#define MAX_NAME_LENGTH          32
#define MAX_LINE_LENGTH          1024

#define SCREEN_WIDTH             1280
#define SCREEN_HEIGHT            720

#define MAX_SND_CHANNELS         8

#define GLYPH_HEIGHT             28
#define GLYPH_WIDTH              18

// ---- Snake settings ----
// One grid cell = one 40x40 sprite
#define CELL_SIZE                40
// Bar at the top of the screen for the score
#define HUD_HEIGHT               40
// The board: 32 columns x 17 rows
#define GRID_COLS                (SCREEN_WIDTH / CELL_SIZE)
#define GRID_ROWS                ((SCREEN_HEIGHT - HUD_HEIGHT) / CELL_SIZE)
// How long the snake is when a level starts
#define START_LENGTH             3
// How many frames the snake waits between moves (60 frames = 1 second)
#define START_MOVE_DELAY         9
#define MIN_MOVE_DELAY           4
// The snake speeds up by 1 frame every this many apples
#define APPLES_PER_SPEEDUP       5
// Frames to wait on the game over screen before a restart key is accepted
#define RESTART_DELAY_FRAMES     30
// The best score is saved here so it is kept after closing the game
#define HIGH_SCORE_FILE          "highscore.txt"

enum
{
	TEXT_LEFT,
	TEXT_CENTER,
	TEXT_RIGHT
};
