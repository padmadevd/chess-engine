#include "game.hpp"
#include "chess.hpp"
#include "raylib/raylib.h"
#include <cstdio>

#define S_COLORW Color{200, 200, 220, 255}
#define S_COLORB Color{75,   75, 80, 255}
#define S_WIDTH 70

uint8_t SquareAt(int x, int y);

#define STATE_NORMAL 0
#define STATE_MENU 1
#define STATE_EXCHANGE_PAWN 2
#define STATE_STATUS 3

#define BG Color{35,   20,  5, 255}
#define BG_GREEN Color{100,   175,  80, 255}
#define FG Color{45,   45,  50, 255}

Rectangle btnMenu;

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

void ProcessGameScene(double delta){

	PollInputEvents();

	if(gameState == NORMAL){
		if(!twoPlayer && !isPlayerWhite == whiteTurn){
			if(!isPlayerWhite)
				moves = board.GetAllLegalMoves(COLOR_W);
			else
				moves = board.GetAllLegalMoves(COLOR_B);

			if(moves.size() == 0){
				gameState = STATE_STATUS;
				return;
			}

			Move move = board.GetBestMove(moves, !isPlayerWhite);
			board.MakeMove(move);
			moves.clear();
			whiteTurn = !whiteTurn;
			return;
		}

		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && (sliding.piece == EMPTY || sliding.piece == INVALID)){

			sliding.square = SquareAt(GetMouseX(), GetMouseY());
			sliding.piece = board.At(sliding.square);
			if(sliding.piece != EMPTY && sliding.piece != INVALID && IsWhite(sliding.piece) == whiteTurn){
				moves = board.GetLegalMoves(sliding.square);
			}
		}
		if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && sliding.piece != EMPTY && sliding.piece != INVALID){

			sliding.piece = INVALID;
			sliding.square = SQUARE_NONE;
			uint8_t end = SquareAt(GetMouseX(), GetMouseY());
			for(int i = 0; i < moves.size(); i++){

				if(IsWhite(board.At(moves[i].start)) != whiteTurn)
					return;
				if(moves[i].end == end){
					if(moves[i].type != PROMOTION){
						// printf("MAKE ");
						// DebugMove(moves[i]);
						board.MakeMove(moves[i]);
						whiteTurn = !whiteTurn;
						break;
					}
					else{
						exchangeMove = i;
						gameState = STATE_EXCHANGE_PAWN;
						// printf("%d\n", (int)moves.size());
						// fflush(stdout);
						return;
					}
				}
			}
			moves.clear();
		}

		if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
			moves = board.GetAllLegalMoves(COLOR_B);
			if(moves.size() == 0){
				gameState = STATE_STATUS;
				return;
			}
			moves = board.GetAllLegalMoves(COLOR_W);
			if(moves.size() == 0){
				gameState = STATE_STATUS;
				return;
			}
			moves.clear();
		}

		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			if(CheckCollisionPointRec(GetMousePosition(), btnMenu)){
				gameState = STATE_MENU;
				return;
			}
		}
		// if(IsKeyPressed(KEY_Z) && (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))){
		// 	if(board.moveHistory.size() > 0)
		// 		whiteTurn = !whiteTurn;

		// 	board.UnMakeMove();
		// }
		return;
	}
	if(gameState == STATE_EXCHANGE_PAWN){
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){

			int x = GetMouseX();
			int y = GetMouseY();

			if(x < exchangeRec.x || x > (exchangeRec.x+exchangeRec.width) || y < exchangeRec.y || y > (exchangeRec.y+exchangeRec.height)){
				gameState = STATE_NORMAL;
				// printf("%d\n", (int)moves.size());
				// fflush(stdout);
				moves.clear();
				return;
			}

			Rectangle inRec;
			inRec.x = exchangeRec.x;
			inRec.y = exchangeRec.y + 10;
			inRec.width = S_WIDTH*1.5;
			inRec.height = S_WIDTH*1.5;

			inRec.x += 10;
			if(x >= inRec.x && x <= (inRec.x+inRec.width) && y >= inRec.y && y <= (inRec.y+inRec.height)){
				// printf("MAKE ");
				// DebugMove(moves[exchangeMove]);
				// printf("%d\n", (int)moves.size());
				// fflush(stdout);
				board.MakeMove(moves[exchangeMove]);
				moves.clear();
				whiteTurn = !whiteTurn;
				gameState = STATE_NORMAL;
				return;
			}
			inRec.x += S_WIDTH*1.5f + 10;
			if(x >= inRec.x && x <= (inRec.x+inRec.width) && y >= inRec.y && y <= (inRec.y+inRec.height)){
				// printf("MAKE ");
				// DebugMove(moves[exchangeMove]);
				// printf("%d\n", (int)moves.size());
				// fflush(stdout);
				board.MakeMove(moves[exchangeMove+1]);
				moves.clear();
				whiteTurn = !whiteTurn;
				gameState = STATE_NORMAL;
				return;
			}
			inRec.x += S_WIDTH*1.5f + 10;
			if(x >= inRec.x && x <= (inRec.x+inRec.width) && y >= inRec.y && y <= (inRec.y+inRec.height)){
				// printf("MAKE ");
				// DebugMove(moves[exchangeMove]);
				// printf("%d\n", (int)moves.size());
				// fflush(stdout);
				board.MakeMove(moves[exchangeMove+2]);
				moves.clear();
				whiteTurn = !whiteTurn;
				gameState = STATE_NORMAL;
				return;
			}
			inRec.x += S_WIDTH*1.5f + 10;
			if(x >= inRec.x && x <= (inRec.x+inRec.width) && y >= inRec.y && y <= (inRec.y+inRec.height)){
				// printf("MAKE ");
				// DebugMove(moves[exchangeMove]);
				// printf("%d\n", (int)moves.size());
				// fflush(stdout);
				board.MakeMove(moves[exchangeMove+3]);
				moves.clear();
				whiteTurn = !whiteTurn;
				gameState = STATE_NORMAL;
				return;
			}
		}
	}

	return;
}

Texture2D texturePieces;
Rectangle tc[12];

extern Font font2;

void InitGameScene(){

	texturePieces = LoadTexture("assets/chessPiece.png");
	SetTextureFilter(texturePieces, TEXTURE_FILTER_BILINEAR);
	SetTextureWrap(texturePieces, TEXTURE_WRAP_CLAMP);

	tc[0] = Rectangle{0*151+1, 1, 149, 149};
	tc[1] = Rectangle{1*151+1, 1, 149, 149};
	tc[2] = Rectangle{2*151+1, 1, 149, 149};
	tc[3] = Rectangle{3*151+1, 1, 149, 149};
	tc[4] = Rectangle{4*151+1, 1, 149, 149};
	tc[5] = Rectangle{5*151+1, 1, 149, 149};

	tc[6] = Rectangle{0*151+1, 156, 149, 149};
	tc[7] = Rectangle{1*151+1, 156, 149, 149};
	tc[8] = Rectangle{2*151+1, 156, 149, 149};
	tc[9] = Rectangle{3*151+1, 156, 149, 149};
	tc[10] = Rectangle{4*151+1, 156, 149, 149};
	tc[11] = Rectangle{5*151+1, 156, 149, 149};

	exchangeRec.width = S_WIDTH*6 + 50;
	exchangeRec.height = S_WIDTH*1.5f + 20;
	exchangeRec.x = S_WIDTH*4 - exchangeRec.width/2;
	exchangeRec.y = S_WIDTH*4 - exchangeRec.height/2;

	btnMenu = Rectangle{20, S_WIDTH*8+10, 150, 50};
}

void DrawGameScene(){

	ClearBackground(BG);
	if(gameState == STATE_NORMAL){

		for (uint8_t s = 0; s < 64; ++s)
		{
			Color sColor = S_COLORB;
			if((s%8 + s/8)%2 == 0)
				sColor = S_COLORW;

			DrawRectangle(s%8*S_WIDTH, s/8*S_WIDTH, S_WIDTH, S_WIDTH, sColor);

			uint8_t current = board.At(s);
			if(sliding.piece != EMPTY && sliding.piece != INVALID && sliding.square == s){
				DrawRectangle(s%8*S_WIDTH, s/8*S_WIDTH, S_WIDTH, S_WIDTH, Color{50, 50, 255, 75});
			}
		}

		if(sliding.piece != EMPTY && sliding.piece != INVALID){
			for(int m = 0; m < moves.size(); m++){
				Move &move = moves[m];
				uint8_t s = move.end;
				DrawRectangle(s%8*S_WIDTH, s/8*S_WIDTH, S_WIDTH, S_WIDTH, Color{50, 255, 0, 75});
				if(move.type == PROMOTION)
					m += 4;
			}
		}

		for (uint8_t s = 0; s < 64; ++s)
		{
			uint8_t current = board.At(s);

			if(current != EMPTY && (sliding.piece != current || sliding.square != s)){
				DrawTexturePro(texturePieces, tc[TypeOf(current)+6*(ColorOf(current)-COLOR_B)], Rectangle{s%8*(float)S_WIDTH, s/8*(float)S_WIDTH, S_WIDTH, S_WIDTH}, Vector2{0, 0}, 0, WHITE);
			}
		}
		if(sliding.piece != EMPTY && sliding.piece != INVALID){
			int x = GetMouseX();
			int y = GetMouseY();
			DrawTexturePro(texturePieces, tc[TypeOf(sliding.piece)+6*(ColorOf(sliding.piece)-COLOR_B)], Rectangle{(float)x - S_WIDTH*1.5f/2.0f, (float)y - S_WIDTH*1.5f/2.0f, S_WIDTH*1.5f, S_WIDTH*1.5f}, Vector2{0, 0}, 0, Color{255, 255, 255, 150});
		}

		DrawRectangleRounded(btnMenu, .3f, 5, BG_GREEN);
		DrawTextEx(font2, "menu", Vector2{btnMenu.x+25, btnMenu.y-13}, 64, 2, FG);
	}
	if(gameState == STATE_EXCHANGE_PAWN){

		for (uint8_t s = 0; s < 64; ++s)
		{

			Color sColor = S_COLORB;
			if((s%8 + s/8)%2 == 0)
				sColor = S_COLORW;

			DrawRectangle(s%8*S_WIDTH, s/8*S_WIDTH, S_WIDTH, S_WIDTH, sColor);

			uint8_t current = board.At(s);
			if(current != EMPTY){
				DrawTexturePro(texturePieces, tc[TypeOf(current)+6*(ColorOf(current)-COLOR_B)], Rectangle{s%8*(float)S_WIDTH, s/8*(float)S_WIDTH, S_WIDTH, S_WIDTH}, Vector2{0, 0}, 0, WHITE);
			}
		}

		DrawRectangle(0, 0, S_WIDTH*8, S_WIDTH*8, Color{0, 0, 0, 150});

		DrawRectangleRounded(exchangeRec, 0.25, 10, S_COLORW);

		if(IsWhite(board.At(moves[exchangeMove].start))){
			DrawTexturePro(texturePieces, tc[BISHOP+6], Rectangle{exchangeRec.x+10+(S_WIDTH*1.5f+10)*0, exchangeRec.y+10, S_WIDTH*1.5f, S_WIDTH*1.5f}, Vector2{0, 0}, 0, WHITE);
			DrawTexturePro(texturePieces, tc[QUEEN+6], Rectangle{exchangeRec.x+10+(S_WIDTH*1.5f+10)*1, exchangeRec.y+10, S_WIDTH*1.5f, S_WIDTH*1.5f}, Vector2{0, 0}, 0, WHITE);
			DrawTexturePro(texturePieces, tc[ROOK+6], Rectangle{exchangeRec.x+10+(S_WIDTH*1.5f+10)*2, exchangeRec.y+10, S_WIDTH*1.5f, S_WIDTH*1.5f}, Vector2{0, 0}, 0, WHITE);
			DrawTexturePro(texturePieces, tc[KNIGHT+6], Rectangle{exchangeRec.x+10+(S_WIDTH*1.5f+10)*3, exchangeRec.y+10, S_WIDTH*1.5f, S_WIDTH*1.5f}, Vector2{0, 0}, 0, WHITE);
		}else{
			DrawTexturePro(texturePieces, tc[BISHOP], Rectangle{exchangeRec.x+10+(S_WIDTH*1.5f+10)*0, exchangeRec.y+10, S_WIDTH*1.5f, S_WIDTH*1.5f}, Vector2{0, 0}, 0, WHITE);
			DrawTexturePro(texturePieces, tc[QUEEN], Rectangle{exchangeRec.x+10+(S_WIDTH*1.5f+10)*1, exchangeRec.y+10, S_WIDTH*1.5f, S_WIDTH*1.5f}, Vector2{0, 0}, 0, WHITE);
			DrawTexturePro(texturePieces, tc[ROOK], Rectangle{exchangeRec.x+10+(S_WIDTH*1.5f+10)*2, exchangeRec.y+10, S_WIDTH*1.5f, S_WIDTH*1.5f}, Vector2{0, 0}, 0, WHITE);
			DrawTexturePro(texturePieces, tc[KNIGHT], Rectangle{exchangeRec.x+10+(S_WIDTH*1.5f+10)*3, exchangeRec.y+10, S_WIDTH*1.5f, S_WIDTH*1.5f}, Vector2{0, 0}, 0, WHITE);
		}
	}

}

uint8_t SquareAt(int x, int y){
	if(x < 0 || x > 8*S_WIDTH || y < 0 || y > 8*S_WIDTH)
		return SQUARE_NONE;
	x = x/S_WIDTH;
	y = y/S_WIDTH;
	return y*8+x;
}