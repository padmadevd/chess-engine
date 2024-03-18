#include <cstdint>
#include <exception>
#include <stdio.h>
#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <sys/types.h>
#include <vector>
using std::vector;

#include "chess.hpp"

#include "game.hpp"
#include "menu.hpp"
#include "status.hpp"

#define STATE_NORMAL 0
#define STATE_MENU 1
#define STATE_EXCHANGE_PAWN 2
#define STATE_STATUS 3

#define SCREEN_WIDTH 560
#define SCREEN_HEIGHT 630

Board board;
PieceInfo sliding;
vector<Move> moves;
uint8_t underCheck = COLOR_NONE;

bool whiteTurn = true;
bool twoPlayer = true;
bool isPlayerWhite = true;

uint8_t gameState = STATE_MENU;
int exchangeMove;

Rectangle exchangeRec;

int main(){

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess (by padmadev)");
	InitGameScene();
	InitMenuScene();
	InitStatusScene();

	double FPS = 60;
	double SPF = 1/FPS;
	double lastTime = GetTime();
	double delta = 0;

	// board.canCastleB = true;
	// board.canCastleQSB = true;
	// board.canCastleW = false;
	// board.canCastleQSW = false;

	// for(int d = 1; d <= 10; d++){
	// 	printf("depth %d : Nodes %llu\n", d, board.Perft(true, d));
	// }

	while (!WindowShouldClose()) {

		if(gameState == STATE_NORMAL || gameState == STATE_EXCHANGE_PAWN)
			ProcessGameScene(delta);
		if(gameState == STATE_MENU)
			ProcessMenuScene(delta);
		if(gameState == STATE_STATUS)
			ProcessStatusScene(delta);

		BeginDrawing();

			ClearBackground(BLACK);

			if(gameState == STATE_NORMAL || gameState == STATE_EXCHANGE_PAWN)
				DrawGameScene();
			if(gameState == STATE_MENU)
				DrawMenuScene();
			if(gameState == STATE_STATUS)
				DrawStatusScene();

		EndDrawing();

		SwapScreenBuffer();
 
		fflush(stdout);

		delta = GetTime() - lastTime;
		lastTime = GetTime();
		WaitTime(SPF - delta);
	}
}
