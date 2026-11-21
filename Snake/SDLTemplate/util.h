#pragma once
#include "math.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

extern int checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
extern void calcSlope(int x1, int y1, int x2, int y2, float* outDx, float* outDy);