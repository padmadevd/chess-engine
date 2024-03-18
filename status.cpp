#include "status.hpp"
#include "chess.hpp"
#include "raylib/raylib.h"
#include <cstdint>

extern Board board;

#define S_COLORW Color{200, 200, 220, 255}
#define S_COLORB Color{75,   75, 80, 255}
#define S_WIDTH 60

#define S_OFFSETX 40
#define S_OFFSETY 75

#define SCREEN_WIDTH 560
#define SCREEN_HEIGHT 630

#define STATE_NORMAL 0
#define STATE_MENU 1
#define STATE_EXCHANGE_PAWN 2
#define STATE_STATUS 3

#define BG Color{35,   20,  5, 255}
#define BG_GREEN Color{100,   175,  80, 255}
#define FG Color{230, 230, 250, 255}

extern uint8_t gameState;

extern Texture2D texturePieces;
extern Rectangle tc[12];

extern Font font2;
extern Font font;

Rectangle btnMenu2;

bool staleMate;
extern bool whiteTurn;

void InitStatusScene(){

	btnMenu2 = Rectangle{20, SCREEN_HEIGHT - 60, 150, 50};
}

void ProcessStatusScene(double delta){

	PollInputEvents();

	if((board.IsInCheck(COLOR_W) && whiteTurn) || (!whiteTurn && board.IsInCheck(COLOR_B)))
		staleMate = false;
	else
		staleMate = true;

	if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
		if(CheckCollisionPointRec(GetMousePosition(), btnMenu2)){
			gameState = STATE_MENU;
			return;
		}
	}
}

void DrawStatusScene(){

	ClearBackground(BG);

	for (uint8_t s = 0; s < 64; ++s)
	{
		Color sColor = S_COLORB;
		if((s%8 + s/8)%2 == 0)
			sColor = S_COLORW;

		DrawRectangle(s%8*S_WIDTH + S_OFFSETX, s/8*S_WIDTH + S_OFFSETY, S_WIDTH, S_WIDTH, sColor);
		uint8_t current = board.At(s);
		if(current != EMPTY && current != INVALID)
			DrawTexturePro(texturePieces, tc[TypeOf(current)+6*(ColorOf(current)-COLOR_B)], Rectangle{s%8*(float)S_WIDTH + S_OFFSETX, s/8*(float)S_WIDTH + S_OFFSETY, S_WIDTH, S_WIDTH}, Vector2{0, 0}, 0, WHITE);
	}

	DrawRectangle(S_OFFSETX, S_OFFSETY, S_WIDTH*8, S_WIDTH*8, Color{0, 0, 0, 150});

	if(staleMate)
		DrawTextEx(font2, "stalemate!", Vector2{SCREEN_WIDTH/2.f - 85, 200}, 64, 1, FG);
	else
		DrawTextEx(font2, "checkmate!", Vector2{SCREEN_WIDTH/2.f - 95, 200}, 64, 1, FG);

	DrawTextEx(font2, "by", Vector2{SCREEN_WIDTH/2.f - 20, 260}, 64, 1, FG);

	if(whiteTurn)
		DrawTextEx(font2, "black", Vector2{SCREEN_WIDTH/2.f -45, 320}, 64, 1, FG);
	else
		DrawTextEx(font2, "white", Vector2{SCREEN_WIDTH/2.f -45, 320}, 64, 1, FG);

	DrawRectangleRounded(btnMenu2, .3f, 5, BG_GREEN);
	DrawTextEx(font2, "menu", Vector2{btnMenu2.x+25, btnMenu2.y-13}, 64, 2, FG);
}