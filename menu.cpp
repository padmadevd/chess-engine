#include "menu.hpp"
#include "raylib/raylib.h"
#include "raylibX.hpp"
#include <cstdint>
#include "chess.hpp"

#define SCREEN_WIDTH 560
#define SCREEN_HEIGHT 630

#define STATE_NORMAL 0
#define STATE_MENU 1
#define STATE_EXCHANGE_PAWN 2
#define STATE_STATUS 3

#define CHECKER_WIDTH 150
#define CHECKER_BG Color{35,   20,  5, 255}
#define CHECKER_BG2 Color{50,   30,  15, 255}
#define BTN_BG_GREEN Color{100,   175,  80, 255}
#define BTN_BG Color{45,   45,  50, 255}
#define BTN_FG Color{230, 230, 250, 255}
#define BTN_BG_DISABLED Color{45,   45,  50, 150}
#define BTN_FG_DISABLED Color{230, 230, 250, 150}

extern Board board;
extern PieceInfo sliding;
extern vector<Move> moves;
extern uint8_t underCheck;

extern bool whiteTurn;
extern bool isPlayerWhite;
extern bool twoPlayer;

extern uint8_t gameState;
extern int exchangeMove;

extern Rectangle exchangeRec;

Texture2D textureIC;
Font font;
Font font2;

extern Texture2D texturePieces;
extern Rectangle tc[12];

Rectangle btnMode1;
Rectangle btnMode2;
Rectangle btnBlack;
Rectangle btnWhite;
Rectangle btnStart;
Rectangle btnExit;

uint8_t pieces[6];
float pieceRots[6];
Vector2 piecePos[6];
float pieceScale[6];

float bgOffX = -40;
float bgOffY = -60;
float bgSpeed = -100;

bool selTwoPlayer = true;
bool selBlack = true;

void InitMenuScene(){

	textureIC = LoadTexture("assets/ic.png");
	SetTextureFilter(textureIC, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(textureIC, TEXTURE_WRAP_CLAMP);
	font = LoadFontEx("assets/oswald.ttf", 36, 0, 0);
	font2 = LoadFontEx("assets/oswald.ttf", 64, 0, 0);

	btnExit = Rectangle{20, 20, 75, 40};
	btnMode1 = Rectangle{60, 250, 200, 60};
	btnMode2 = Rectangle{60+200+40, 250, 200, 60};
	btnBlack = Rectangle{200, 250+60+100, 60, 60};
	btnWhite = Rectangle{200+60+40, 250+60+100, 60, 60};
	btnStart = Rectangle{180, 250+60+100+60+50, 200, 75};

	for(int i = 0; i < 6; i++){
		pieces[i] = GetRandomValue(0, 11);
		pieceRots[i] = GetRandomValue(0, 100);
		pieceScale[i] = GetRandomValue(50, 75)/100.f;
		piecePos[i].x = GetRandomValue(50, SCREEN_WIDTH -50);
		piecePos[i].y = i*SCREEN_HEIGHT/6.f;
	}
}

void ProcessMenuScene(double delta){

	PollInputEvents();

	bgOffX += bgSpeed*delta;
	bgOffY += bgSpeed*delta;

	if(bgOffX <= -2*CHECKER_WIDTH)
		bgOffX = 0;
	if(bgOffY <= -2*CHECKER_WIDTH)
		bgOffY = 0;

	for(int i = 0; i < 6; i++){
		pieceRots[i] += 40*delta;
		piecePos[i].y += 100*delta;
		if(piecePos[i].y > SCREEN_HEIGHT+100){
			pieces[i] = GetRandomValue(0, 11);
			pieceRots[i] = GetRandomValue(0, 100);
			pieceScale[i] = GetRandomValue(50, 75)/100.f;
			piecePos[i].x = GetRandomValue(50, SCREEN_WIDTH -50);
			piecePos[i].y = -100;
		}
	}

	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){

		if(CheckCollisionPointRec(GetMousePosition(), btnExit))
			CloseWindow();
		if(CheckCollisionPointRec(GetMousePosition(), btnMode1))
			selTwoPlayer = false;
		if(CheckCollisionPointRec(GetMousePosition(), btnMode2))
			selTwoPlayer = true;
		if(!selTwoPlayer){
			if(CheckCollisionPointRec(GetMousePosition(), btnBlack))
				selBlack = true;
			if(CheckCollisionPointRec(GetMousePosition(), btnWhite))
				selBlack = false;
		}
		if(CheckCollisionPointRec(GetMousePosition(), btnStart)){
			board.Reset();
			sliding.piece = INVALID;

			if(selTwoPlayer)
				twoPlayer = true;
			else
				twoPlayer = false;

			if(selBlack)
				isPlayerWhite = false;
			else
				isPlayerWhite = true;

			whiteTurn = true;

			moves.clear();
			underCheck = COLOR_NONE;

			gameState = STATE_NORMAL;
		}
	}
}

void DrawMenuScene(){

	ClearBackground(CHECKER_BG);

	for(uint8_t x = 0; x < 6; x++){
		for (uint8_t y = 0; y < 7; ++y)
		{
			if((x+y)%2 == 0)
				DrawRectangle(x*CHECKER_WIDTH + bgOffX, y*CHECKER_WIDTH + bgOffY, CHECKER_WIDTH, CHECKER_WIDTH, CHECKER_BG2);
		}
	}

	for(int i = 0; i < 6; i++){

		Rectangle rec = Rectangle{piecePos[i].x, piecePos[i].y, tc[pieces[i]].width, tc[pieces[i]].height};
		DrawTexture(texturePieces, tc[pieces[i]], rec, pieceRots[i], pieceScale[i], Vector2{rec.width/2, rec.height/2}, WHITE);
	}


	DrawRectangleRounded(btnExit, .2f, 5, BTN_FG);
	DrawTextEx(font, "exit", Vector2{btnExit.x+17, btnExit.y}, 36, 2, BTN_BG);


	DrawTextEx(font2, "mode", Vector2{SCREEN_WIDTH/2.f-50, btnMode2.y-75}, 64, 2, BTN_FG);

	DrawRectangleRounded(btnMode1, .2f, 5, BTN_BG_GREEN);
	DrawTextEx(font, "vs computer", Vector2{btnMode1.x+35, btnMode1.y+11}, 36, 2, BTN_FG);
	// DrawTextureEx(textureIC, Vector2{btnMode1.x + 143, btnMode1.y+5}, 0, 0.3, WHITE);

	DrawRectangleRounded(btnMode2, .2f, 5, BTN_BG_GREEN);
	DrawTextEx(font, "two player", Vector2{btnMode2.x+10, btnMode2.y+11}, 36, 2, BTN_FG);
	Rectangle rec = btnMode2;
	// rec.height = rec.height*0.75;
	rec.width = rec.height;
	rec.y -= 3;
	rec.x += 110;
	DrawTexture(texturePieces, tc[PAWN+6], rec, 0, 1, Vector2{0, 0}, WHITE);
	rec.x += 40;
	DrawTexture(texturePieces, tc[PAWN], rec, 0, 1, Vector2{0, 0}, WHITE);


	if(selTwoPlayer)
		DrawRectangle(btnMode2.x+10, btnMode2.y+btnMode2.height+5, btnMode2.width-20, 5, Color{255, 255, 255, 150});
	else
		DrawRectangle(btnMode1.x+10, btnMode1.y+btnMode1.height+5, btnMode1.width-20, 5, Color{255, 255, 255, 150});


	if(!selTwoPlayer){

		DrawTextEx(font2, "i choose", Vector2{SCREEN_WIDTH/2.f-75, btnBlack.y-75}, 64, 2, BTN_FG);

		DrawRectangleRounded(btnBlack, .2f, 5, BTN_BG);
		// DrawTextEx(font, "black", Vector2{btnBlack.x+15, btnBlack.y+15}, 45, 2, BTN_FG);

		DrawRectangleRounded(btnWhite, .2f, 5, BTN_FG);
		// DrawTextEx(font, "white", Vector2{btnWhite.x+13, btnWhite.y+15}, 45, 2, BTN_BG);

		if(selBlack)
			DrawRectangleRoundedLines(btnBlack, .2f, 5, 3, BTN_BG_GREEN);
			// DrawRectangle(btnBlack.x+10, btnBlack.y+btnBlack.height+5, btnBlack.width-20, 5, BTN_BG_GREEN);
		else
			DrawRectangleRoundedLines(btnWhite, .2f, 5, 3, BTN_BG_GREEN);
			// DrawRectangle(btnWhite.x+10, btnWhite.y+btnWhite.height+5, btnWhite.width-20, 5, BTN_BG_GREEN);

	}else{

		DrawTextEx(font2, "i choose", Vector2{SCREEN_WIDTH/2.f-75, btnBlack.y-75}, 64, 2, BTN_FG_DISABLED);

		DrawRectangleRounded(btnBlack, .2f, 5, BTN_BG_DISABLED);
		// DrawTextEx(font, "black", Vector2{btnBlack.x+15, btnBlack.y+15}, 45, 2, BTN_FG_DISABLED);

		DrawRectangleRounded(btnWhite, .2f, 5, BTN_FG_DISABLED);
		// DrawTextEx(font, "white", Vector2{btnWhite.x+13, btnWhite.y+15}, 45, 2, BTN_BG_DISABLED);
	}

	DrawRectangleRounded(btnStart, .2f, 5, BTN_FG);
	DrawTextEx(font2, "start", Vector2{btnStart.x+55, btnStart.y+3}, 64, 2, BTN_BG);
}