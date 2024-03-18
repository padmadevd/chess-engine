#include "chess.hpp"

#define S_COLORW Color{200, 200, 220, 255}
#define S_COLORB Color{75,   75, 80, 255}
#define S_WIDTH 70
double TIME = 0.5;

extern Texture2D texturePieces;
extern Rectangle tc[12];

void DebugDrawBoard(Board &board){

	PollInputEvents();

	if(IsKeyPressed(KEY_DOWN))
		TIME -= 0.125;
	if(IsKeyPressed(KEY_UP))
		TIME += 0.25;


	BeginDrawing();

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

	EndDrawing();

	SwapScreenBuffer();

	WaitTime(TIME);
}

void DebugPieceType(uint8_t type){

	switch (type) {
		case PAWN:
			printf("PAWN ");
			break;
		case KNIGHT:
			printf("KNIGHT ");
			break;
		case ROOK:
			printf("ROOK ");
			break;
		case BISHOP:
			printf("BISHOP ");
			break;
		case QUEEN:
			printf("QUEEN ");
			break;
		case KING:
			printf("KING ");
			break;
		case PIECE_NONE:
			printf("PIECE NONE ");
			break;
	}

	fflush(stdout);
	return;
}
void DebugPieceColor(uint8_t color){

	switch (color) {
		case COLOR_B:
			printf("BLACK ");
			break;
		case COLOR_W:
			printf("WHITE ");
			break;
		case COLOR_ALL:
			printf("COLOR ALL ");
			break;
		case COLOR_NONE:
			printf("COLOR NONE ");
			break;
	}

	fflush(stdout);
	return;
}
void DebugPiece(uint8_t piece){

	DebugPieceType(TypeOf(piece));
	DebugPieceColor(ColorOf(piece));

	fflush(stdout);
	return;
}
void DebugMoveType(uint8_t type){

	switch (type) {
		case NORMAL:
			printf("MOVE-NORMAL ");
			break;
		case TWOSTEP:
			printf("MOVE-TWOSTEP ");
			break;
		case CASTLING:
			printf("MOVE-CASTLING ");
			break;
		case ENPASSANT:
			printf("MOVE-ENPASSANT ");
			break;
		case PROMOTION:
			printf("MOVE-PROMOTION ");
			break;
	}

	fflush(stdout);
	return;
}

void DebugMove(Move move){

	DebugMoveType(move.type);
	printf("FROM ");
	DebugSquare(move.start);
	printf("END ");
	DebugSquare(move.end);
	printf("INSERTED ");
	DebugPiece(move.inserted);
	printf("DELETED ");
	DebugPiece(move.deleted);

	fflush(stdout);
	return;
}
void DebugSquare(uint8_t square){

	if(IsValidSquare(square)){

		uint8_t x = square%8;
		uint8_t y = square/8;
		printf("(%c%c) ", x+'a', '8'-y);
	}else {
		printf("SQUARE-NONE ");
	}

	fflush(stdout);
	return;
}

void DebugBoard(Board &board){

	for(int y = 0; y < 8; y++){
		for (int x = 0; x < 8; ++x)
		{
			printf("%d ", board.At(x, y));
		}
		printf("\n");
	}

	fflush(stdout);
}

uint8_t North(uint8_t square){

	if(square != SQUARE_NONE && square/8 > 0)
		return square - 8;
	else
		return SQUARE_NONE;
}
uint8_t South(uint8_t square){

	if(square != SQUARE_NONE && square/8 < 7)
		return square + 8;
	else
		return SQUARE_NONE;
}
uint8_t East(uint8_t square){

	if(square != SQUARE_NONE && square%8 < 7)
		return square + 1;
	else
		return SQUARE_NONE;
}
uint8_t West(uint8_t square){

	if(square != SQUARE_NONE && square%8 > 0)
		return square - 1;
	else
		return SQUARE_NONE;
}

uint8_t ColorOf(uint8_t piece){

	if(piece >= RW && piece <= PW)
		return COLOR_W;
	else if(piece >= RB && piece <= PB)
		return COLOR_B;
	else
		return COLOR_NONE;
}

uint8_t TypeOf(uint8_t piece){

	if(piece == RW || piece == RB)
		return ROOK;
	if(piece == NW || piece == NB)
		return KNIGHT;
	if(piece == BW || piece == BB)
		return BISHOP;
	if(piece == QW || piece == QB)
		return QUEEN;
	if(piece == KW || piece == KB)
		return KING;
	if(piece == PW || piece == PB)
		return PAWN;

	return PIECE_NONE;
}


Board::Board(){

	uint8_t positions[64] = {
		 RB, NB, BB, QB, KB, BB, NB, RB,
		 PB, PB, PB, PB, PB, PB, PB, PB,
		  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,
		 PW, PW, PW, PW, PW, PW, PW, PW,
		 RW, NW, BW, QW, KW, BW, NW, RW
	};

	// rnB2k1r/pp2bppp/2p5/8/2B5/1P6/P1P1NnPP/qNBQK2R b K - 1 8
	// POSITION 1
	// uint8_t positions[64] = {
	// 	 RB, NB, BB, QB,  0, KB,  0, RB,
	// 	 PB, PB,  0, PW, BB, PB, PB, PB,
	// 	  0,  0, PB,  0,  0,  0,  0,  0,
	// 	  0,  0,  0,  0,  0,  0,  0,  0,
	// 	  0,  0, BW,  0,  0,  0,  0,  0,
	// 	  0,  0,  0,  0,  0,  0,  0,  0,
	// 	 PW, PW, PW,  0, NW, NB, PW, PW,
	// 	 RW, NW, BW, QW, KW,  0,  0, RW
	// };

	// r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/P1N2Q2/1PPBBPpP/R3K2R w KQkq -
	// POSITION 2
	// uint8_t positions[64] = {
	// 	 RB,  0,  0,  0, KB,  0,  0, RB,
	// 	 PB,  0, PB, PB, QB, PB, BB,  0,
	// 	 BB, NB,  0,  0, PB, NB, PB,  0,
	// 	  0,  0,  0, PW, NW,  0,  0,  0,
	// 	  0, PB,  0,  0, PW,  0,  0,  0,
	// 	  0,  0, NW,  0,  0, QW,  0, PB,
	// 	 PW, PW, PW, BW, BW, PW, PW, PW,
	// 	 RW,  0,  0,  0, KW,  0,  0, RW
	// };

	// POSITION 3 - SUCCESS
	// uint8_t positions[64] = {
	// 	  0,  0,  0,  0,  0,  0,  0,  0,
	// 	  0,  0, PB,  0,  0,  0,  0,  0,
	// 	  0,  0,  0, PB,  0,  0,  0,  0,
	// 	 KW, PW,  0,  0,  0,  0,  0, RB,
	// 	  0, RW,  0,  0,  0, PB,  0, KB,
	// 	  0,  0,  0,  0,  0,  0,  0,  0,
	// 	  0,  0,  0,  0, PW,  0, PW,  0,
	// 	  0,  0,  0,  0,  0,  0,  0,  0
	// };

	// POSITION 4 - SUCCESS
	// uint8_t positions[64] = {
	// 	 RB,  0,  0,  0, KB,  0,  0, RB,
	// 	 PW, PB, PB, PB,  0, PB, PB, PB,
	// 	  0, BB,  0,  0,  0, NB, BB, NW,
	// 	 NB, PW,  0,  0,  0,  0,  0,  0,
	// 	 BW, BW, PW,  0, PW,  0,  0,  0,
	// 	 QB,  0,  0,  0,  0, NW,  0,  0,
	// 	 PW, PB,  0, PW,  0,  0, PW, PW,
	// 	 RW,  0,  0, QW,  0, RW, KW,  0
	// };

	for (uint8_t i = 0; i < 64; ++i)
	{

		squares[i] = positions[i];

		if(squares[i] == KW)
			KWSquare = i;
		if(squares[i] == KB)
			KBSquare = i;
	}
}

void Board::Reset(){

	uint8_t positions[64] = {
		 RB, NB, BB, QB, KB, BB, NB, RB,
		 PB, PB, PB, PB, PB, PB, PB, PB,
		  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,
		 PW, PW, PW, PW, PW, PW, PW, PW,
		 RW, NW, BW, QW, KW, BW, NW, RW
	};

	for (uint8_t i = 0; i < 64; ++i)
	{

		squares[i] = positions[i];

		if(squares[i] == KW)
			KWSquare = i;
		if(squares[i] == KB)
			KBSquare = i;
	}

	unsigned int unCastleMoveQSW = 0;
	unsigned int unCastleMoveQSB = 0;
	unsigned int unCastleMoveW = 0;
	unsigned int unCastleMoveB = 0;
	bool canCastleQSW = true;
	bool canCastleQSB = true;
	bool canCastleW = true;
	bool canCastleB = true;

	moveHistory.clear();
	moveCount = 0;
}

uint8_t Board::At(uint8_t square){
	if(IsValidSquare(square))
		return squares[square];
	else
		return INVALID;
}

uint8_t Board::At(uint8_t x, uint8_t y){
	if(x < 0 || x > 7 || y < 0 || y > 7)
		return INVALID;
	else
		return squares[y*8+x];
}

void Board::Set(uint8_t square, uint8_t piece){

	if (!IsValidSquare(square) || !IsValid(piece))
		return;

	// DebugSquare(square);
	// DebugPiece(piece);
	// printf("\n");
	// fflush(stdout);

	if(piece == KB)
		KBSquare = square;
	if(piece == KW)
		KWSquare = square;

	squares[square] = piece;
}

uint8_t Board::GetKing(uint8_t color){

	if(color != COLOR_B && color != COLOR_W)
		return SQUARE_NONE;

	// for(uint8_t s = 0; s < 64; s++){

	// 	if(At(s) == KW && color == COLOR_W)
	// 		return s;
	// 	if(At(s) == KB && color == COLOR_B)
	// 		return s;
	// }

	if(color == COLOR_B)
		return KBSquare;
	if(color == COLOR_W)
		return KWSquare;

	return SQUARE_NONE;
}

bool Board::IsInAttack(uint8_t square, bool whiteTurn){

	uint8_t piece = At(square);
	uint8_t color = ColorOf(piece);

	if(!IsValidSquare(square))
		return false;

	if(!IsValid(piece))
		return false;

	if(IsEmpty(piece)){
		if(whiteTurn)
			color = COLOR_W;
		else
		 	color = COLOR_B;
	}

	uint8_t d, s;

	// EAST
	s = East(square);
	d = 1;
	while (IsValidSquare(s)) {

		if(!IsEmpty(At(s))){

			if(ColorOf(At(s)) == color)
				break;
			else{
				uint8_t type = TypeOf(At(s));
				if(type == ROOK || type == QUEEN || (type == KING && d == 1)){
					return true;
				}
				else
					break;
			}
		}
		s = East(s);
		d += 1;
	}

	// WEST
	s = West(square);
	d = 1;
	while (IsValidSquare(s)) {

		if(!IsEmpty(At(s))){

			if(ColorOf(At(s)) == color)
				break;
			else{
				uint8_t type = TypeOf(At(s));
				if(type == ROOK || type == QUEEN || (type == KING && d == 1)){
					return true;
				}
				else
					break;
			}
		}
		s = West(s);
		d += 1;
	}

	
	// SOUTH
	s = South(square);
	d = 1;
	while (IsValidSquare(s)) {

		if(!IsEmpty(At(s))){

			if(ColorOf(At(s)) == color)
				break;
			else{
				uint8_t type = TypeOf(At(s));
				if(type == ROOK || type == QUEEN || (type == KING && d == 1)){
					return true;
				}
				else
					break;
			}
		}
		s = South(s);
		d += 1;
	}

	// NORTH
	s = North(square);
	d = 1;
	while (IsValidSquare(s)) {

		if(!IsEmpty(At(s))){

			if(ColorOf(At(s)) == color)
				break;
			else{
				uint8_t type = TypeOf(At(s));
				if(type == ROOK || type == QUEEN || (type == KING && d == 1)){
					return true;
				}
				else
					break;
			}
		}
		s = North(s);
		d += 1;
	}

	// NORTH - EAST
	s = East(North(square));
	d = 1;
	while (IsValidSquare(s)) {

		if(!IsEmpty(At(s))){

			if(ColorOf(At(s)) == color)
				break;
			else{
				uint8_t type = TypeOf(At(s));
				if(type == BISHOP || type == QUEEN || (type == KING && d == 1) || (IsBlack(At(s)) && type == PAWN && d == 1)){
					return true;
				}
				else
					break;
			}
		}
		s = East(North(s));
		d += 1;
	}

	// NORTH - WEST
	s = West(North(square));
	d = 1;
	while (IsValidSquare(s)) {

		if(!IsEmpty(At(s))){

			if(ColorOf(At(s)) == color)
				break;
			else{
				uint8_t type = TypeOf(At(s));
				if(type == BISHOP || type == QUEEN || (type == KING && d == 1) || (IsBlack(At(s)) && type == PAWN && d == 1)){
					return true;
				}
				else
					break;
			}
		}
		s = West(North(s));
		d += 1;
	}

	//SOUTH - EAST
	s = East(South(square));
	d = 1;
	while (IsValidSquare(s)) {

		if(!IsEmpty(At(s))){

			if(ColorOf(At(s)) == color)
				break;
			else{
				uint8_t type = TypeOf(At(s));
				if(type == BISHOP || type == QUEEN || (type == KING && d == 1) || (IsWhite(At(s)) && type == PAWN && d == 1)){
					return true;
				}
				else
					break;
			}
		}
		s = East(South(s));
		d += 1;
	}

	//SOUTH - WEST
	s = West(South(square));
	d = 1;
	while (IsValidSquare(s)) {

		if(!IsEmpty(At(s))){

			if(ColorOf(At(s)) == color)
				break;
			else{
				uint8_t type = TypeOf(At(s));
				if(type == BISHOP || type == QUEEN || (type == KING && d == 1) || (IsWhite(At(s)) && type == PAWN && d == 1)){
					return true;
				}
				else
					break;
			}
		}
		s = West(South(s));
		d += 1;
	}

	int xL[] = {2, 2, -2, -2, 1, 1, -1, -1};
	int yL[] = {1, -1, 1, -1, 2, -2, 2, -2};
	for (uint8_t i = 0; i < 8; ++i)
	{
		uint8_t x = square%8 + xL[i];
		uint8_t y = square/8 + yL[i];
		// printf("%d %d\n", x, y);
		uint8_t type = TypeOf(At(x, y));
		if((y >= 0 && y < 8) && (x >= 0 && x < 8) && type == KNIGHT && ColorOf(At(x, y)) != color){
				return true;
		}
	}
	// fflush(stdout);
	// printf("bishop check\n");
	// fflush(stdout);
	// DebugBoard(*this);
	return false;
}

bool Board::IsInCheck(uint8_t color){

	return IsInAttack(GetKing(color), false);
}

void Board::MakeMove(Move move){

	if(IsValidSquare(move.start) && IsValidSquare(move.end) && move.start != move.end){

		moveCount += 1;

		if(canCastleB && (At(move.start) == KB || move.start == H8 || move.end == H8)){
			canCastleB = false;
			unCastleMoveB = moveCount;
		}
		if(canCastleW && (At(move.start) == KW || move.start == H1 || move.end == H1)){
			canCastleW = false;
			unCastleMoveW = moveCount;
		}
		if(canCastleQSB && (At(move.start) == KB || move.start == A8 || move.end == A8)){
			canCastleQSB = false;
			unCastleMoveQSB = moveCount;
		}
		if(canCastleQSW && (At(move.start) == KW || move.start == A1 || move.end == A1)){
			canCastleQSW = false;
			unCastleMoveQSW = moveCount;
		}

		if(move.type == NORMAL || move.type == TWOSTEP){

			move.deleted = At(move.end);

			Set(move.end, At(move.start));
			Set(move.start, EMPTY);

			if(At(move.start) == KB)
				KBSquare = move.end;
			if(At(move.start) == KW)
				KWSquare = move.end;
		}
		if(move.type == CASTLING){

			move.deleted = EMPTY;

			Set(move.end, At(move.start));
			Set(move.start, EMPTY);
			if(move.end == G8 || move.end == G1){
				Set(East(move.start), At(East(move.end)));
				Set(East(move.end), EMPTY);
			}else{
				Set(West(move.start), At(West(West(move.end))));
				Set(West(West(move.end)), EMPTY);
			}

			if(At(move.start) == KB)
				KBSquare = move.end;
			if(At(move.start) == KW)
				KWSquare = move.end;
		}
		if(move.type == ENPASSANT){

			if(IsWhite(At(move.start))){
				Set(move.end, At(move.start));
				Set(move.start, EMPTY);
				Set(South(move.end), EMPTY);
			}else{
				Set(move.end, At(move.start));
				Set(move.start, EMPTY);
				Set(North(move.end), EMPTY);
			}
		}
		if (move.type == PROMOTION)
		{

			move.deleted = At(move.end);

			Set(move.end, move.inserted);
			Set(move.start, EMPTY);
		}
		
		moveHistory.push_back(move);
	}

	// fflush(stdout);
	// printf("Make Move %d\n", moveCount);
	// DebugMove(move);
	// printf("\n");
	// fflush(stdout);
	// DebugDrawBoard(*this);
}

void Board::UnMakeMove(){

	if(moveHistory.size() > 0){

		Move &move = moveHistory.back();

		if(!IsValidSquare(move.start) || !IsValidSquare(move.end))
			return;

		if(!canCastleB && moveCount == unCastleMoveB){
			canCastleB = true;
		}
		if(!canCastleW && moveCount == unCastleMoveW){
			canCastleW = true;
		}
		if(!canCastleQSB && moveCount == unCastleMoveQSB){
			canCastleQSB = true;
		}
		if(!canCastleQSW && moveCount == unCastleMoveQSW){
			canCastleQSW = true;
		}

		if(move.type == NORMAL || move.type == TWOSTEP){

			Set(move.start, At(move.end));
			Set(move.end, move.deleted);

			if(At(move.end) == KB)
				KBSquare = move.start;
			if(At(move.end) == KW)
				KWSquare = move.start;
		}
		else if(move.type == CASTLING){

			Set(move.start, At(move.end));
			Set(move.end, EMPTY);

			if(move.end == G8 || move.end == G1){
				Set(East(move.end), At(East(move.start)));
				Set(East(move.start), EMPTY);

				if(IsBlack(At(move.start)))
					canCastleB = true;
				else
					canCastleW = true;
			}else{
				Set(West(West(move.end)), At(West(move.start)));
				Set(West(move.start), EMPTY);

				if(IsBlack(At(move.start)))
					canCastleQSB = true;
				else
					canCastleQSW = true;
			}

			if(At(move.end) == KB)
				KBSquare = move.start;
			if(At(move.end) == KW)
				KWSquare = move.start;
		}
		else if(move.type == ENPASSANT){

			if(IsWhite(At(move.end))){
				Set(move.start, At(move.end));
				Set(South(move.end), move.deleted);
				Set(move.end, EMPTY);
			}else{
				Set(move.start, At(move.end));
				Set(North(move.end), move.deleted);
				Set(move.end, EMPTY);
			}
		}
		else if(move.type == PROMOTION){

			if(IsBlack(move.inserted)){
				Set(move.start, PB);
				Set(move.end, move.deleted);
			}else{
				Set(move.start, PW);
				Set(move.end, move.deleted);
			}
		}

		moveHistory.pop_back();
		moveCount -= 1;

		// fflush(stdout);
		// printf("Unmake Move %d\n", moveCount);
		// DebugMove(move);
		// printf("\n");
		// fflush(stdout);
		// DebugDrawBoard(*this);
	}
}

vector<Move> Board::GetPseudoLegalMoves(uint8_t s){

	vector<Move> moves;

	if(s == SQUARE_NONE)
		return moves;

	uint8_t piece = At(s);
	uint8_t type = TypeOf(piece);
	uint8_t color = ColorOf(piece);

	uint8_t x, y, d;

	if(type == PAWN){
		x = s%8;
		y = s/8;

		if((IsWhite(At(s)) && y == 1) || (IsBlack(At(s)) && y == 6)){
			Move move;
			move.start = s;
			if(IsWhite(At(s))){
				x = s%8+1;
				y = s/8-1;
				if (x >= 0 && x < 8 && y >= 0 && y < 8 && IsBlack(At(x, y)))
				{
					move.end = y*8+x;
					move.deleted = At(x, y);
					move.type = PROMOTION;
					move.inserted = BW;
					moves.push_back(move);
					move.inserted = QW;
					moves.push_back(move);
					move.inserted = RW;
					moves.push_back(move);
					move.inserted = NW;
					moves.push_back(move);
				}
				x = s%8-1;
				y = s/8-1;
				if (x >= 0 && x < 8 && y >= 0 && y < 8 && IsBlack(At(x, y)))
				{
					move.end = y*8+x;
					move.deleted = At(x, y);
					move.type = PROMOTION;
					move.inserted = BW;
					moves.push_back(move);
					move.inserted = QW;
					moves.push_back(move);
					move.inserted = RW;
					moves.push_back(move);
					move.inserted = NW;
					moves.push_back(move);
				}
				x = s%8;
				y = s/8-1;
				if (x >= 0 && x < 8 && y >= 0 && y < 8 && At(x, y) == EMPTY)
				{
					move.end = y*8+x;
					move.deleted = EMPTY;
					move.type = PROMOTION;
					move.inserted = BW;
					moves.push_back(move);
					move.inserted = QW;
					moves.push_back(move);
					move.inserted = RW;
					moves.push_back(move);
					move.inserted = NW;
					moves.push_back(move);
				}
				return moves;
			}else{
				x = s%8+1;
				y = s/8+1;
				if (x >= 0 && x < 8 && y >= 0 && y < 8 && IsWhite(At(x, y)))
				{
					move.end = y*8+x;
					move.deleted = At(x, y);
					move.type = PROMOTION;
					move.inserted = BB;
					moves.push_back(move);
					move.inserted = QB;
					moves.push_back(move);
					move.inserted = RB;
					moves.push_back(move);
					move.inserted = NB;
					moves.push_back(move);
				}
				x = s%8-1;
				y = s/8+1;
				if (x >= 0 && x < 8 && y >= 0 && y < 8 && IsWhite(At(x, y)))
				{
					move.end = y*8+x;
					move.deleted = At(x, y);
					move.type = PROMOTION;
					move.inserted = BB;
					moves.push_back(move);
					move.inserted = QB;
					moves.push_back(move);
					move.inserted = RB;
					moves.push_back(move);
					move.inserted = NB;
					moves.push_back(move);
				}
				x = s%8;
				y = s/8+1;
				if (x >= 0 && x < 8 && y >= 0 && y < 8 && At(x, y) == EMPTY)
				{
					move.end = y*8+x;
					move.deleted = At(x, y);
					move.type = PROMOTION;
					move.inserted = BB;
					moves.push_back(move);
					move.inserted = QB;
					moves.push_back(move);
					move.inserted = RB;
					moves.push_back(move);
					move.inserted = NB;
					moves.push_back(move);
				}
				return moves;
			}
		}

		if(color == COLOR_W && y == 6 && At(s-16) == EMPTY && At(s-8) == EMPTY){

			Move move;
			move.start = s;
			move.end = s-16;
			move.type = TWOSTEP;
			moves.push_back(move);
		}
		if(color == COLOR_B && y == 1 && At(s+16) == EMPTY && At(s+8) == EMPTY){

			Move move;
			move.start = s;
			move.end = s+16;
			move.type = TWOSTEP;
			moves.push_back(move);
		}
		if(moveHistory.size() > 0){
			Move prevMove = moveHistory.back();
			if(prevMove.type == TWOSTEP && ColorOf(At(prevMove.end)) != color){
				if((x > 0 && s-1 == prevMove.end) || (x < 7 && s+1 == prevMove.end)){
					if(color == COLOR_B){
						Move move;
						move.start = s;
						move.end = prevMove.end+8;
						move.deleted = At(prevMove.end);
						move.type = ENPASSANT;
						moves.push_back(move);
					}else{
						Move move;
						move.start = s;
						move.end = prevMove.end-8;
						move.deleted = At(prevMove.end);
						move.type = ENPASSANT;
						moves.push_back(move);
					}
				}
			}
		}
	}

	// L - MOVES
	if(type == KNIGHT){

		int xL[] = {2, 2, -2, -2, 1, 1, -1, -1};
		int yL[] = {1, -1, 1, -1, 2, -2, 2, -2};

		for (uint8_t i = 0; i < 8; ++i)
		{
			x = s%8 + xL[i];
			y = s/8 + yL[i];
			if((y >= 0 && y < 8) && (x >= 0 && x < 8) && ColorOf(At(x, y)) != color){
				Move move;
				move.start = s;
				move.end = y*8+x;
				move.type = NORMAL;
				moves.push_back(move);
			}
		}
	}

	if(type == KING){
		if((color == COLOR_B && canCastleB) || (color == COLOR_W && canCastleW)){
			if(!IsInAttack(s, IsWhite(piece)) && At(s+1) == EMPTY && !IsInAttack(s+1, IsWhite(piece)) && At(s+2) == EMPTY && !IsInAttack(s+2, IsWhite(piece))){
				Move move;
				move.start = s;
				move.end = s+2;
				move.type = CASTLING;
				moves.push_back(move);
			}
		}
		if((color == COLOR_B && canCastleQSB) || (color == COLOR_W && canCastleQSW)){
			if(!IsInAttack(s, IsWhite(piece))  && At(s-1) == EMPTY && !IsInAttack(s-1, IsWhite(piece)) && At(s-2) == EMPTY && !IsInAttack(s-2, IsWhite(piece)) && At(s-3) == EMPTY){
				Move move;
				move.start = s;
				move.end = s-2;
				move.type = CASTLING;
				moves.push_back(move);
			}
		}
	}

	// EAST
	x = s%8 + 1;
	y = s/8;
	d = 1;
	while ((type == ROOK || type == QUEEN || (type == KING && d == 1)) && (x >= 0 && x < 8)) {

		if(At(x, y) != EMPTY){

			if(ColorOf(At(x, y)) == color)
				break;
			else{
				Move move;
				move.start = s;
				move.end = y*8+x;
				move.type = NORMAL;
				moves.push_back(move);
				break;
			}
		}else{
			Move move;
			move.start = s;
			move.end = y*8+x;
			move.type = NORMAL;
			moves.push_back(move);
		}
		x += 1;
		d += 1;
	}

	// WEST
	x = s%8 - 1;
	y = s/8;
	d = 1;
	while ((type == ROOK || type == QUEEN || (type == KING && d == 1)) && (x >= 0 && x < 8)) {

		if(At(x, y) != EMPTY){

			if(ColorOf(At(x, y)) == color)
				break;
			else{
				Move move;
				move.start = s;
				move.end = y*8+x;
				move.type = NORMAL;
				moves.push_back(move);
				break;
			}
		}else{
			Move move;
			move.start = s;
			move.end = y*8+x;
			move.type = NORMAL;
			moves.push_back(move);
		}
		x -= 1;
		d += 1;
	}

	// SOUTH
	x = s%8;
	y = s/8 + 1;
	d = 1;
	while ((type == ROOK || type == QUEEN || (type == KING && d == 1) || (type == PAWN && d == 1 && color == COLOR_B)) && (y >= 0 && y < 8)) {

		if(At(x, y) != EMPTY){

			if(ColorOf(At(x, y)) == color || type == PAWN)
				break;
			else{
				Move move;
				move.start = s;
				move.end = y*8+x;
				move.type = NORMAL;
				moves.push_back(move);
				break;
			}
		}else{
			Move move;
			move.start = s;
			move.end = y*8+x;
			move.type = NORMAL;
			moves.push_back(move);
		}
		y += 1;
		d += 1;
	}

	// NORTH
	x = s%8;
	y = s/8 - 1;
	d = 1;
	while ((type == ROOK || type == QUEEN || (type == KING && d == 1) || (type == PAWN && d == 1 && color == COLOR_W)) && (y >= 0 && y < 8)) {

		if(At(x, y) != EMPTY){

			if(ColorOf(At(x, y)) == color || type == PAWN)
				break;
			else{
				Move move;
				move.start = s;
				move.end = y*8+x;
				move.type = NORMAL;
				moves.push_back(move);
				break;
			}
		}else{
			Move move;
			move.start = s;
			move.end = y*8+x;
			move.type = NORMAL;
			moves.push_back(move);
		}
		y -= 1;
		d += 1;
	}

	// NORTH-EAST
	x = s%8 + 1;
	y = s/8 - 1;
	d = 1;
	while ((type == BISHOP || type == QUEEN || (type == KING && d == 1) || (type == PAWN && d == 1 && color == COLOR_W)) && (x >= 0 && x < 8) && (y >= 0 && y < 8)) {

		if(At(x, y) != EMPTY){

			if(ColorOf(At(x, y)) == color)
				break;
			else{
				Move move;
				move.start = s;
				move.end = y*8+x;
				move.type = NORMAL;
				moves.push_back(move);
				break;
			}
		}else{
			if(type != PAWN){
				Move move;
				move.start = s;
				move.end = y*8+x;
				move.type = NORMAL;
				moves.push_back(move);
			}
		}
		x += 1;
		y -= 1;
		d += 1;
	}

	// NORTH-WEST
	x = s%8 - 1;
	y = s/8 - 1;
	d = 1;
	while ((type == BISHOP || type == QUEEN || (type == KING && d == 1) || (type == PAWN && d == 1 && color == COLOR_W)) && (x >= 0 && x < 8) && (y >= 0 && y < 8)) {

		if(At(x, y) != EMPTY){

			if(ColorOf(At(x, y)) == color)
				break;
			else{
				Move move;
				move.start = s;
				move.end = y*8+x;
				move.type = NORMAL;
				moves.push_back(move);
				break;
			}
		}else{
			if(type != PAWN){
				Move move;
				move.start = s;
				move.end = y*8+x;
				move.type = NORMAL;
				moves.push_back(move);
			}
		}
		x -= 1;
		y -= 1;
		d += 1;
	}

	// SOUTH-EAST
	x = s%8 + 1;
	y = s/8 + 1;
	d = 1;
	while ((type == BISHOP || type == QUEEN || (type == KING && d == 1) || (type == PAWN && d == 1 && color == COLOR_B)) && (x >= 0 && x < 8) && (y >= 0 && y < 8)) {

		if(At(x, y) != EMPTY){

			if(ColorOf(At(x, y)) == color)
				break;
			else{
				Move move;
				move.start = s;
				move.end = y*8+x;
				move.type = NORMAL;
				moves.push_back(move);
				break;
			}
		}else{
			if(type != PAWN){
				Move move;
				move.start = s;
				move.end = y*8+x;
				move.type = NORMAL;
				moves.push_back(move);
			}
		}
		x += 1;
		y += 1;
		d += 1;
	}

	// SOUTH-WEST
	x = s%8 - 1;
	y = s/8 + 1;
	d = 1;
	while ((type == BISHOP || type == QUEEN || (type == KING && d == 1) || (type == PAWN && d == 1 && color == COLOR_B)) && (x >= 0 && x < 8) && (y >= 0 && y < 8)) {

		if(At(x, y) != EMPTY){

			if(ColorOf(At(x, y)) == color)
				break;
			else{
				Move move;
				move.start = s;
				move.end = y*8+x;
				move.type = NORMAL;
				moves.push_back(move);
				break;
			}
		}else{
			if(type != PAWN){
				Move move;
				move.start = s;
				move.end = y*8+x;
				move.type = NORMAL;
				moves.push_back(move);
			}
		}
		x -= 1;
		y += 1;
		d += 1;
	}

	return moves;
}


vector<Move> Board::GetAllPseudoLegalMoves(uint8_t playerColor){

	vector<Move> moves;

	if(playerColor == COLOR_NONE || playerColor == COLOR_ALL)
		return moves;

	for(uint8_t s = 0; s < 64; s++){

		uint8_t piece = At(uint8_t(s));
		uint8_t color = ColorOf(piece);
		if(color == playerColor){
			vector<Move> pseudoMoves = GetPseudoLegalMoves(uint8_t(s));
			moves.insert(moves.end(), pseudoMoves.begin(), pseudoMoves.end());
		}
	}

	return moves;
}

vector<Move> Board::GetLegalMoves(uint8_t s){

	uint8_t piece = At(s);
	uint8_t color = ColorOf(piece);

	vector<Move> pseudoMoves = GetPseudoLegalMoves(s);
	vector<Move> moves;

	for(int i = 0; i < pseudoMoves.size(); i++){
		Move m = pseudoMoves[i];
		MakeMove(m);
		if(!IsInCheck(color))
			moves.push_back(m);
		UnMakeMove();
	}

	return moves;
}

vector<Move> Board::GetAllLegalMoves(uint8_t playerColor){

	vector<Move> moves;

	if(playerColor == COLOR_NONE || playerColor == COLOR_ALL)
		return moves;

	for(uint8_t s = 0; s < 64; s++){

		uint8_t piece = At(uint8_t(s));
		uint8_t color = ColorOf(piece);
		if(color == playerColor){
			vector<Move> m = GetLegalMoves(uint8_t(s));
			moves.insert(moves.end(), m.begin(), m.end());
		}
	}

	return moves;
}

Move Board::GetBestMove(vector<Move> moves, bool whiteTurn){

	if(moves.size() == 0){
		return Move();
	}

	int bestScore, bestMove = 0;
	if(whiteTurn)
		bestScore = -500000;
	else
		bestScore = +500000;

	for(int m = 0; m < moves.size(); m++){

		int score;

		if(whiteTurn)
			score = Search(moves[m], whiteTurn, 6, 500000, -500000);
		else
			score = Search(moves[m], !whiteTurn, 6, -500000, 500000);

		if(whiteTurn && score > bestScore){
			bestScore = score;
			bestMove = m;
		}
		if(!whiteTurn && score < bestScore){
			bestScore = score;
			bestMove = m;
		}
	}

	return moves[bestMove];
}

int Board::Evaluate(){

	int score = 0;

	if(IsInCheck(COLOR_W))
		return -100000;
	if(IsInCheck(COLOR_B))
		return 100000;

	for(int s = 0; s < 64; s++){

		switch (At(s)) {
			case PB:
				score -= 50;
				break;
			case NB:
				score -= 300;
				break;
			case RB:
				score -= 400;
				break;
			case BB:
				score -= 600;
				break;
			case QB:
				score -= 1000;
				break;
			case PW:
				score += 50;
				break;
			case NW:
				score += 300;
				break;
			case RW:
				score += 400;
				break;
			case BW:
				score += 600;
				break;
			case QW:
				score += 1000;
				break;
		}
	}

	return score;
}

int Board::Search(Move move, bool whiteTurn, uint8_t depth, int alpha, int beta){

	if(depth == 0){

		MakeMove(move);
		int eval = Evaluate();
		UnMakeMove();
		return eval;
	}

	MakeMove(move);

	if (whiteTurn)
	{
		vector<Move> moves = GetAllLegalMoves(COLOR_B);

		for (int m = 0; m < moves.size(); ++m)
		{

			int eval = Search(moves[m], !whiteTurn, depth-1, beta, alpha);

			if(eval <= beta){
				UnMakeMove();
				return beta;
			}

			if(eval < alpha)
				alpha = eval;
		}

		UnMakeMove();
		return alpha;
	}
	else
	{
		vector<Move> moves = GetAllLegalMoves(COLOR_W);

		for (int m = 0; m < moves.size(); ++m)
		{

			int eval = Search(moves[m], !whiteTurn, depth-1, alpha, beta);

			if(eval >= beta){
				UnMakeMove();
				return beta;
			}

			if(eval > alpha)
				alpha = eval;
		}

		UnMakeMove();
		return alpha;
	}

	// if(depth == 0){

	// 	MakeMove(move);
	// 	int eval = Evaluate();
	// 	UnMakeMove();
	// 	return eval;
	// }

	// MakeMove(move);

	// if (whiteTurn)
	// {
	// 	vector<Move> moves = GetAllLegalMoves(COLOR_B);
	// 	int score = 500000;
	// 	for (int m = 0; m < moves.size(); ++m)
	// 	{

	// 		int eval = Search(moves[m], !whiteTurn, depth-1, beta, alpha);
	// 		if(eval < score)
	// 			score = eval;
	// 	}
	// 	UnMakeMove();
	// 	return score;
	// }
	// else
	// {
	// 	vector<Move> moves = GetAllLegalMoves(COLOR_W);
	// 	int score = -500000;
	// 	for (int m = 0; m < moves.size(); ++m)
	// 	{

	// 		int eval = Search(moves[m], !whiteTurn, depth-1, beta, alpha);
	// 		if(eval > score)
	// 			score = eval;
	// 	}
	// 	UnMakeMove();
	// 	return score;
	// }
}

unsigned long long int Board::MoveTest(bool whiteTurn, int depth){

	if(depth == 0){
		return 1;
	}

	uint8_t color = COLOR_B;
	vector<Move> moves;

	if(whiteTurn)
		color = COLOR_W;

	moves = GetAllPseudoLegalMoves(color);

	unsigned long long int moveCount = 0;
	for(int m = 0; m < moves.size(); m++){
		MakeMove(moves[m]);
		if(!IsInCheck(color)){
			unsigned long long count = MoveTest(!whiteTurn, depth-1);
			// if(depth == 1){
			// 	printf("\t");
			// 	DebugSquare(moves[m].start);
			// 	DebugSquare(moves[m].end);
			// 	printf(" : %llu \n", count);
			// 	fflush(stdout);
			// }
			moveCount += MoveTest(!whiteTurn, depth-1);
		}
		UnMakeMove();
	}

	return moveCount;
}

unsigned long long int Board::Perft(bool whiteTurn, int depth){

	if(depth == 0){
		return 1;
	}

	uint8_t color = COLOR_B;
	vector<Move> moves;

	if(whiteTurn)
		color = COLOR_W;

	moves = GetAllPseudoLegalMoves(color);

	unsigned long long int moveCount = 0;
	for(int m = 0; m < moves.size(); m++){
		MakeMove(moves[m]);
		if(!IsInCheck(color)){
			unsigned long long count = MoveTest(!whiteTurn, depth-1);
			// if(depth == 1){
			// 	DebugSquare(moves[m].start);
			// 	DebugSquare(moves[m].end);
			// 	printf(" : %llu \n", count);
			// 	fflush(stdout);
			// }
			moveCount += MoveTest(!whiteTurn, depth-1);
		}
		UnMakeMove();
	}

	return moveCount;
}