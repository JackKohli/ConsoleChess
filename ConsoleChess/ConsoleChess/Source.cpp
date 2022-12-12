#include<iostream>
using namespace std; //bad practice...

struct coord {
	int x;
	int y;
	coord(int file, int rank) {
		x = file;
		y = rank;
	}
	coord() {
		x = 0, y = 0;
	};
};


class Piece {//this probably needs a rewrite but i decided to stick with it 
public://this could probably just be a struct at this point...
	char Colour = 0, Symbol = 0; //w=white b=black
	bool HasMoved = 0;
	bool CanEnpassantLeft = 0;
	bool CanEnpassantRight = 0;
};


void clearBoard(Piece board[8][8]) {//initialis(z)e board. Unnecessary but would be used if multiple games in one run was implimented.
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j].Symbol = 0;
			board[i][j].Colour = 0;
			board[i][j].HasMoved = 0;
		}
	}
};
void newBoard(Piece board[8][8]) {//set up the board in the starting position
	int i, j;
	for (j = 0; j < 8; j++) {
		if (j == 0) {//white pieces
			for (i = 0; i < 8; i++) {
				board[i][j].Colour = 'W';
				if (i == 0 || i == 7)//rooks
				board[i][j].Symbol = 'r';
				if (i == 1 || i == 6)//knights
				board[i][j].Symbol = 'n', board[i][j].HasMoved = 1;
				if (i == 2 || i == 5)//bishops
				board[i][j].Symbol = 'b', board[i][j].HasMoved = 1;
				if (i == 3)//queen
				board[i][j].Symbol = 'q', board[i][j].HasMoved = 1;
				if (i == 4)//king
				board[i][j].Symbol = 'k';
			};
		}
		else if (j == 1) {//white pawns
			for (i = 0; i < 8; i++) {
				board[i][j].Colour = 'W';
				board[i][j].Symbol = 'p';
			};
		}
		else if (j > 1 && j < 6) {
			for (i = 0; i < 8; i++) {
				board[i][j].HasMoved = 1;
			}
		}
		else if (j == 6) {//black pawns
			for (i = 0; i < 8; i++) {
				board[i][j].Colour = 'B';
				board[i][j].Symbol = 'p';
			};
		}
		else if (j==7){
			for (i = 0; i < 8; i++) {
				board[i][j].Colour = 'B';
				if (i == 0 || i == 7)//rooks
				board[i][j].Symbol = 'r';
				if (i == 1 || i == 6)//knights
				board[i][j].Symbol = 'n', board[i][j].HasMoved = 1;
				if (i == 2 || i == 5)//bishops
				board[i][j].Symbol = 'b', board[i][j].HasMoved = 1;
				if (i == 3)//queen
				board[i][j].Symbol = 'q', board[i][j].HasMoved = 1;
				if (i == 4)//king
				board[i][j].Symbol = 'k';
			};
		}
	}
};
//will call getMyColour() at the start of a turn and store it to avoid having to pass the function repeatedly
char getMyColour(int turn) {
	if (turn % 2 == 0)
		return 'W';
	else return 'B';
}
char getOpponentColour(int turn) {
	if (turn % 2 == 0)
		return 'B';
	else return 'W';
}
coord choosePiece(Piece board[8][8], char colour) { //player selects the coordinates of the piece they want to move, this returns coord
	int rank;
	char file;
	coord square;
choose:
	cout << "Choose the file (lower case letter) and then rank (number) of the piece you would like to move: ";
	cin.clear();
	cin >> file >> rank;
	square.x = file - (int)'a';
	square.y = rank - 1;
	if (square.x < 0 || square.x >7 || square.y < 0 || square.y >7) {
		cout << "That square is not on the board.";
		goto choose;
	}
	if (board[square.x][square.y].Symbol != 0 && board[square.x][square.y].Colour == colour)
		return square;
	else if (board[square.x][square.y].Symbol == 0)
		cout << "There is no piece at " << char(square.x + (int)'a') << square.y + 1 << endl;
	else cout << "The piece at " << char(square.x + (int)'a') << square.y + 1 << " does not  belong to you" << endl;
	goto choose;
};
coord movePiece(Piece board[8][8], char colour) {//select coordinates to move piece to
	coord square;
	char file;
	int rank;
move:
	cout << "Choose the file (lower case letter) and then rank (number) that you would like to move this piece to: ";
	cin.clear();
	cin >> file >> rank;
	square.x = int(file - 'a');
	square.y = rank - 1;
	if (square.x < 0 || square.x >7 || square.y < 0 || square.y >7) {
		cout << "That square is not on the board." << endl;
		goto move;
	}
	else if (board[square.x][square.y].Colour == colour) {
		cout << "You can't move your piece onto a square already occupied by one of your pieces." << endl;
		goto move;
	}
	return square;
};
bool clearLine(Piece board[8][8], coord square, coord destination) { //check if a line along x or y is clear between piece position and destination. probably not the most efficient.
	bool isClear = 1;
	if (square.x < destination.x) {
		for (int i = square.x + 1; i < destination.x; i++) {
			if (board[i][square.y].Symbol != 0) isClear = 0;
		}
	}
	else if (square.x > destination.x) {
		for (int i = square.x - 1; i > destination.x; i--) {
			if (board[i][square.y].Symbol != 0) isClear = 0;
		}
	}
	else if (square.y < destination.y) {
		for (int i = square.y + 1; i < destination.y; i++) {
			if (board[square.x][i].Symbol != 0) isClear = 0;
		}
	}
	else if (square.y > destination.y) {
		for (int i = square.y - 1; i > destination.y; i--) {
			if (board[square.x][i].Symbol != 0) isClear = 0;
		}
	}
	return isClear;
};
bool clearDiag(Piece board[8][8], coord square, coord destination) {// check if diagonal is clear between start and finish point. also probably not most efficient
	bool isClear = 1;//is clear by default
	int i, j;
	if (square.x < destination.x) {
		if (square.y < destination.y) {
			for (i = square.x + 1, j = square.y + 1; i < destination.x; i++, j++) {//right, up
				if (board[i][j].Symbol != 0) isClear = 0;
			}
		}
		else for (i = square.x + 1, j = square.y - 1; i < destination.x; i++, j--) {//right, down
			if (board[i][j].Symbol != 0) isClear = 0;
		}
	}
	else if (square.x > destination.x) {
		if (square.y < destination.y) {
			for (i = square.x - 1, j = square.y + 1; i > destination.x; i--, j++) {//left, up
				if (board[i][j].Symbol != 0) isClear = 0;
			}
		}
		else for (i = square.x - 1, j = square.y - 1; i > destination.x; i--, j--) {//left, down
			if (board[i][j].Symbol != 0) isClear = 0;
		}
	}
	return isClear;
};
//ValidMove() should probably be integrated getPossibleMoves(), checking user input destination against possible moves, maybe later and maybe moved out of the class
bool validMove(Piece board[8][8], coord square, coord destination) {
	if (board[square.x][square.y].Symbol == 'p') {//pawn
		if (board[square.x][square.y].Colour == 'W' && board[destination.x][destination.y].Colour != 'W') {
			if (destination.y == square.y + 1) {
				return (square.x == destination.x && board[destination.x][destination.y].Symbol == 0)//simple move
					|| (destination.x == square.x + 1 && ((board[destination.x][destination.y].Symbol != 0) || board[square.x][square.y].CanEnpassantRight == 1))//captures
					|| (destination.x == square.x - 1 && ((board[destination.x][destination.y].Symbol != 0) || board[square.x][square.y].CanEnpassantLeft == 1));
			}
			else if (destination.y == square.y + 2) {//double move
				return board[square.x][square.y].HasMoved == 0 && board[square.x][square.y + 1].Symbol == 0 && board[square.x][square.y + 2].Symbol == 0 && square.x == destination.x;
			}
		}
		else if (board[square.x][square.y].Colour == 'B' && board[destination.x][destination.y].Colour != 'B') {
			if (destination.y == square.y - 1) {
				return ((square.x == destination.x && board[destination.x][destination.y].Symbol == 0))//simple move
					|| (destination.x == square.x + 1 && ((board[destination.x][destination.y].Symbol != 0) || board[square.x][square.y].CanEnpassantRight == 1))//captures
					|| (destination.x == square.x - 1 && ((board[destination.x][destination.y].Symbol != 0) || board[square.x][square.y].CanEnpassantLeft == 1));
			}
			else if (destination.y == square.y - 2) {//double move
				return board[square.x][square.y].HasMoved == 0 && board[square.x][square.y - 1].Symbol == 0 && board[square.x][square.y - 2].Symbol == 0 && square.x == destination.x;
			}
		}
	}
	else if (board[square.x][square.y].Symbol == 'b') {//bishop
		return  board[destination.x][destination.y].Colour != board[square.x][square.y].Colour && (destination.x - square.x == destination.y - square.y || (destination.x - square.x == -(destination.y - square.y))) && clearDiag(board, square, destination);//dx=dy||dx=-dy and check path is clear
	}
	else if (board[square.x][square.y].Symbol == 'n') {//knight
		return board[destination.x][destination.y].Colour != board[square.x][square.y].Colour && (((destination.x == square.x + 2 || destination.x == square.x - 2) && (destination.y == square.y + 1 || destination.y == square.y - 1)) || //knight can move across 2 and up/down 1
			((destination.x == square.x + 1 || destination.x == square.x - 1) && (destination.y == square.y + 2 || destination.y == square.y - 2)));// or up/down 2 and across 1
	}
	else if (board[square.x][square.y].Symbol == 'r') {//rook
		return clearLine(board, square, destination) && (destination.x == square.x || destination.y == square.y) && board[destination.x][destination.y].Colour != board[square.x][square.y].Colour;
	}
	else if (board[square.x][square.y].Symbol == 'q') {//queen
		return board[destination.x][destination.y].Colour != board[square.x][square.y].Colour && ((clearLine(board, square, destination) && (destination.x == square.x || destination.y == square.y))
			|| ((clearDiag(board, square, destination) && (destination.x - square.x == destination.y - square.y || destination.x - square.x == -(destination.y - square.y)))));
	}
	else if (board[square.x][square.y].Symbol == 'k') {//king
		return board[destination.x][destination.y].Colour != board[square.x][square.y].Colour && (square.x - destination.x<2 && square.x - destination.x >-2) && (square.y - destination.y<2 && square.y - destination.y >-2)
			|| (board[square.x][square.y].HasMoved==0 && ((destination.x == square.x + 2 && clearLine(board,square, coord(7, square.y))&& board[7][square.y].HasMoved==0) || (destination.x == square.x - 2 && clearLine(board, square, coord(0, square.y)) && board[0][square.y].HasMoved == 0)));
	}
	return 0;
}
// this is for checking if a square is checked by your opponent as if there's a piece on that square, whether there actually is or not (important for pawns when validtating castling)
//we iterate across all squares and if a square contains an opposite colour piece, see if the square we are checking is a valid move for that piece =
bool squareIsCheck(Piece board[8][8], coord square, char colour) {
	int i, j;
	Piece pieceOld = board[square.x][square.y];//we need to test the square while it is not empty so we store all of the data about the square
	board[square.x][square.y].Symbol = 't', board[square.x][square.y].Colour = 't';//set symbol and colour to arbitrary non-zero char 't'
	char checkingColour;//this is the colour of piece that can check the square, i.e. the opposite colour to current player's
	if (colour == 'W') { checkingColour = 'B'; }
	else checkingColour = 'W';
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (board[i][j].Colour == checkingColour) {
				if (validMove(board, coord(i,j), square)) {
					board[square.x][square.y] = pieceOld;//reset the piece
					return 1;
				}
			}
		}
	}
	board[square.x][square.y] = pieceOld;//reset the piece
	return 0;	
};
coord myKingLocation(Piece board[8][8], char colour) {
	int i, j;
	coord kingPosition;
	for (i = 0; i < 8; i++) {//find king's location
		for (j = 0; j < 8; j++) {
			if (board[i][j].Symbol == 'k' && board[i][j].Colour == colour) {
				return coord(i, j);
			}//this returns a warning; not all control paths return a value but both kings are always on board so this always returns the king's position
		}	 //could impliment some error code.
	}
};
void promote(Piece board[8][8], coord square) {//for move function if (Symbol == 'p' && (destination == 7 || destination == 0)) call this function to change symbol to chosen symbol.
functionStart:
	char targetPiece;
	cout << "Please choose which piece you would like to promote to; b(ishop), (k)n(ight), r(ook), q(ueen) ";
	cin >> targetPiece;
	if (targetPiece == 'b' || targetPiece == 'n' || targetPiece == 'r' || targetPiece == 'q') {
		board[square.x][square.y].Symbol = targetPiece;
	}
	else {
		cout << targetPiece << " is not a valid type to promote a pawn." << endl;
		goto functionStart;
	}
}
void clearEnpassant(Piece board[8][8], char colour) {
	int i;
	for (i = 0; i < 8; i++) {//after the current player's move set CanEnpassant to 0 for rank 4 and 5, for black and white respectively
		if (colour == 'W')//at the end of a white turn
			board[i][4].CanEnpassantLeft = 0, board[i][4].CanEnpassantRight = 0;
		else if (colour == 'B')
			board[i][3].CanEnpassantLeft = 0, board[i][3].CanEnpassantRight = 0;
	}
};
//for the isCheckmate() function we will declare an array (with 27 entries(max possible for a queen)) of coord variables. use an int function to fill for a piece at a square and return number of moves
coord possibleMoves[27];// this is a global variable and probably bad practice... but it is required for current implimentation of canMove() function
//this is needed to save hundreds of if statements in isCheckmate() and isStalemate(). this whole function is basically for the sake of operation reduction...
bool moveIsGood(Piece board[8][8], coord square, coord destination, char colour) {
	if (validMove(board, square, destination)) {
		Piece positionOld = board[square.x][square.y], destinationOld = board[destination.x][destination.y];//store old squares
		board[destination.x][destination.y] = positionOld;//set piece new square
		board[square.x][square.y].Colour = 0, board[square.x][square.y].Symbol = 0;//empty piece previous square
		if (squareIsCheck(board, myKingLocation(board, colour), colour) == 0) {//if king not in check return true
			board[square.x][square.y] = positionOld, board[destination.x][destination.y] = destinationOld;
			return 1;
		}
		else {
			board[square.x][square.y] = positionOld, board[destination.x][destination.y] = destinationOld;
			return 0;
		}
	}
	else return 0;
}
//for the sake of the canMove() function call king, then ascending maximum possible moves (p,n,b,r,q), sometimes maybe reduces operations, could use statistics for order maybe
int getPossibleMoves(Piece board[8][8], coord square) {
	int i, j, k = 0;//the index of the coordinate of a valid move will be written to possibleMoves[k] will use k return to limit iteration on possibleMoves[] in canMove to avoid extra checks outside
			 //appropriate range. maybe bad practice to use an int function like this
	if (board[square.x][square.y].Symbol == 0) {
		return 0;
	}
	else if (board[square.x][square.y].Symbol == 'k') {
		for (i = -1; i < 2; i++) {
			if (square.x + i < 0) {
				continue;
			}
			if (square.x + i > 7) {
				break;
			}
			for (j = -1; j < 2; j++) {
				if (i == 0 && j == 0) {
					continue;
				}
				if (square.y + j < 0) {
					continue;
				}
				if (square.y + j > 7) {
					break;
				}
				if (moveIsGood(board, square, coord(square.x + i, square.y + j), board[square.x][square.y].Colour)) {
					possibleMoves[k] = coord(square.x + i, square.y + j);
					k++;
				}
			}
		}
		if (board[square.x][square.y].HasMoved == 0) {//redundancy with validMove() but small, could remove there
			if (!squareIsCheck(board, square, board[square.x][square.y].Colour)) {//can't castle while in check
				if (!squareIsCheck(board, coord(square.x + 1, square.y), board[square.x][square.y].Colour)) {//can't castle through a checked square
					if (moveIsGood(board, square, coord(square.x + 2, square.y), board[square.x][square.y].Colour)) {//castle kingside
						possibleMoves[k] = coord(square.x + 2, square.y);
						k++;
					}
				}
				if (!squareIsCheck(board, coord(square.x - 1, square.y), board[square.x][square.y].Colour)) {//can't castle through a checked square
					if (moveIsGood(board, square, coord(square.x - 2, square.y), board[square.x][square.y].Colour)) {//catle queenside
						possibleMoves[k] = coord(square.x - 2, square.y);
						k++;
					}
				}
			}
		}
		return k;
	}
	else if (board[square.x][square.y].Symbol == 'p') {
		if (board[square.x][square.y].Colour == 'W') {//for pawns you need to check colour because they can only move forward (up for white, down for black)
			for (i = -1; i < 2; i++) {//if valid move or enpassant and the new x coordinate is between 0 and 7 inclusive
				if (square.x + i < 0) {
					continue;
				}
				if (square.x + i > 7) {
					break;
				}
				if (moveIsGood(board, square, coord(square.x + i, square.y + 1), board[square.x][square.y].Colour)) {
					possibleMoves[k] = coord(square.x + i, square.y + 1);
					k++;
				}
			}
			if (moveIsGood(board, square, coord(square.x, square.y + 2), board[square.x][square.y].Colour)) {
				possibleMoves[k] = coord(square.x, square.y + 2);
				k++;
			}
			return k;
		}
		else {
			for (i = -1; i < 2; i++) {
				if (square.x + i < 0) {
					continue;
				}
				if (square.x + i > 7) {
					break;
				}
				if (moveIsGood(board, square, coord(square.x + i, square.y - 1), board[square.x][square.y].Colour)) {
					possibleMoves[k] = coord(square.x + i, square.y - 1);
					k++;
				}
			}
			if (moveIsGood(board, square, coord(square.x, square.y - 2), board[square.x][square.y].Colour)) {
				possibleMoves[k] = coord(square.x, square.y - 2);
				k++;
			}
			return k;
		}
	}
	else if (board[square.x][square.y].Symbol == 'n') {
		for (i = -2; i < 3; i++) {
			if (square.x + i < 0) {
				continue;
			}
			if (square.x + i > 7) {
				break;
			}
			if (i == 0) {
				continue;
			}
			for (j = -2; j < 3; j++) {
				if (j == 0) {
					continue;
				}
				if (j == i || j == -i) {
					continue;
				}
				if (square.y + j < 0) {
					continue;
				}
				if (square.y + j > 7) {
					break;
				}
				if (moveIsGood(board, square, coord(square.x + i, square.y + j), board[square.x][square.y].Colour)) {
					possibleMoves[k] = coord(square.x + i, square.y + j);
					k++;

				}
			}
		}
		return k;
	}
	else if (board[square.x][square.y].Symbol == 'b') {//feels like a light rewrite of ClearDiag()
		for (i = 1; square.x + i < 8; i++) {
			j = i;//right, up
			if (square.y + j < 8) {
				if (moveIsGood(board, square, coord(square.x + i, square.y + j), board[square.x][square.y].Colour)) {
					possibleMoves[k] = coord(square.x + i, square.y + j);
					k++;

				}
			}
			j = -i;//right, down
			if (square.y + j > -1) {
				if (moveIsGood(board, square, coord(square.x + i, square.y + j), board[square.x][square.y].Colour)) {
					possibleMoves[k] = coord(square.x + i, square.y + j);
					k++;
				}
			}
			
		}
		for (i = -1; square.x + i > -1; i--) {
			j = i;//left, down
			if (square.y + j > -1) {
				if (moveIsGood(board, square, coord(square.x + i, square.y + j), board[square.x][square.y].Colour)) {
					possibleMoves[k] = coord(square.x + i, square.y + j);
					k++;
				}
			}
			j = -i;//left, up
			if (square.y + j < 8) {
				if (moveIsGood(board, square, coord(square.x + i, square.y + j), board[square.x][square.y].Colour)) {
					possibleMoves[k] = coord(square.x + i, square.y + j);
					k++;
				}
			}
		}
		return k;
	}
	else if (board[square.x][square.y].Symbol == 'r') {
		for (i = -square.x; square.x + i < 8; i++) {//from left to right
			if (moveIsGood(board, square, coord(square.x + i, square.y), board[square.x][square.y].Colour)) {
				possibleMoves[k] = coord(square.x + i, square.y);
				k++;
			}
		}
		for (i = -square.y; square.y + i < 8; i++) {//from bottom to top
			if (moveIsGood(board, square, coord(square.x, square.y + i), board[square.x][square.y].Colour)) {
				possibleMoves[k] = coord(square.x, square.y + i);
				k++;
			}
		}
		return k;
	}
	else if (board[square.x][square.y].Symbol == 'q') {//combine bishop and rook moves for queen
		//rook code
		for (i = -square.x; square.x + i < 8; i++) {//from left to right
			if (moveIsGood(board, square, coord(square.x + i, square.y), board[square.x][square.y].Colour)) {
				possibleMoves[k] = coord(square.x + i, square.y);
				k++;
			}
		}
		for (i = -square.y; square.y + i < 8; i++) {//from bottom to top
			if (moveIsGood(board, square, coord(square.x, square.y + i), board[square.x][square.y].Colour)) {
				possibleMoves[k] = coord(square.x, square.y + i);
				k++;
			}
		}
		//bishop code
		for (i = 1; square.x + i < 8; i++) {
			j = i;//right, up
			if (square.y + j < 8) {
				if (moveIsGood(board, square, coord(square.x + i, square.y + j), board[square.x][square.y].Colour)) {
					possibleMoves[k] = coord(square.x + i, square.y + j);
					k++;

				}
			}
				j = -i;//right, down
				if (square.y + j > -1) {

					if (moveIsGood(board, square, coord(square.x + i, square.y + j), board[square.x][square.y].Colour)) {
						possibleMoves[k] = coord(square.x + i, square.y + j);
						k++;
					}

				}
			}
		for (i = -1; square.x + i > -1; i--) {
			j = i;//left, down
			if (square.y + j > -1) {
				if (moveIsGood(board, square, coord(square.x + i, square.y + j), board[square.x][square.y].Colour)) {
					possibleMoves[k] = coord(square.x + i, square.y + j);
					k++;
				}

			}
			j = -i;//left, up
			if (square.y + j < 8) {

				if (moveIsGood(board, square, coord(square.x + i, square.y + j), board[square.x][square.y].Colour)) {
					possibleMoves[k] = coord(square.x + i, square.y + j);
					k++;
				}

			}
		}
		return k;
	}
}

//define game endings, checkmate, stalemate, 3-fold repetition, 50moverule

//for every piece of player's colour check if there's a possible move (doesn't result in check and valid move for that piece type)
//if this returns 0, checkmate or stalemate. determine by finding if player's king is in check
//there are optimisations one could make like scanning every rank bottom to top for white and top to bottom for black. offers a reduction in operations most of the game
bool canMove(Piece board[8][8], char colour) {
	int i, j;
	int numberPossibleMoves;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (board[i][j].Colour == colour) {
				numberPossibleMoves = getPossibleMoves(board, coord(i, j));
				if (numberPossibleMoves > 0) {
					return 1;
				}
			}
		}
	}
	return 0;
}

//will call threefold after the move is played and update move100 beforehand i.e. increment if no capture occured/reset if pawn move or capture
//in main we will store the whole board state for up to 100 moves (50 each player). there is likely another way to find repetitions but this is simple
// isThreefold will only be called if there have been 8 or more (move100>7) turns since last capture or pawn move
//iterate through all entries in record we start iteration at j = i+4 because it's the earliest possible duplicate position
bool isThreefold(Piece record[8][8][100], int move100) {//pass turnNumber because only current turn player's moves can lead to a repetition
	int i, j, k, l;
	for (i =  (move100+1) % 2; i < move100 - 5; i+=2) {//8 is minimum number of moves for a position repetition, starts from end of turn of last capture/pawn move for current player's turn only. Due to castling/enpassant rights this is -5 instead of -7
		int repeats = 0;//reset repeats every time the board you're comparing against is changed
		for (j = i + 4; j < move100; j += 2) {//must be on the same player's turn i.e. current player moves into the 3rd repetition of the position while it is their turn
			bool matches = 1;//we say the records match until we find one that doesn't
			for (k = 0; k < 8; k++) {
				for (l = 0; l < 8; l++) {
					if (record[k][l][i].Symbol != record[k][l][j].Symbol || record[k][l][i].Colour != record[k][l][j].Colour || record[k][l][i].CanEnpassantLeft != record[k][l][j].CanEnpassantLeft ||
						record[k][l][i].CanEnpassantRight != record[k][l][j].CanEnpassantRight || record[k][l][i].HasMoved != record[k][l][j].HasMoved) {
						matches = 0;
						break;
					}
				}
				if (matches == 0)break;//saves MANY iterations at the cost of an extra if statement for the first non-match, breaks to the for loop in j
			}
			if (matches == 1) {//if matches is still true after iterating through k and l, the records match
				repeats++;
				if (repeats == 2) {
					return 1;
				}
			}
		}
	}
	return 0;
}
//50 move rule (1 turn for each player 50 times so 100 total since last capture or pawn move returns true
bool isFiftyMoves(int count) {
	if (count == 99)//move100 can take values from 0 to 99, if 99 end the game
		return 1;
	else
		return 0;
}
//there is insufficient material for a checkmate if no pawns remain and a player has only 2 knights or fewer or 2 bishops on the same colour square (possible but unlikely)
//if any rooks or queens remain there is sufficient material, if a player has 2 bishops(on opposite coloured squares) or a bishop and knight it is possible to checkmate
//scan board and check for any pawns, rooks or queens, if they exist, returns false, else counts bishops and knights of each colour, if bishops >=2 and bishopsOppositeSquares = true returns false
//if bishops + knights >= 2, returns false else if knights <=2 or bishops >=2 but bishopsOppositeSquares = false or bishops+knights < 2 returns true if the opponent only has a king because
//some possible checkmates exist if the king avoids stalemate by moving another piece allowing for a checkmate with 2 knights or 2 same coloured square bishops (maybe)
//it is technically possible although unlikely to checkmate with 1 knight each, 1 bishop (opposite square colour but not same) each and 1 knight vs 1 bishop
bool isInsufficientMaterial(Piece board[8][8]) {
	int wBishopsOnWhite=0, wBishopsOnBlack=0;
	int bBishopsOnWhite=0, bBishopsOnBlack=0;
	int	wKnights=0, bKnights=0, i, j;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (board[i][j].Symbol == 'p' || board[i][j].Symbol == 'r' || board[i][j].Symbol == 'q') {
				return 0;
			}
			else if (board[i][j].Symbol == 'b') {
				if (board[i][j].Colour == 'W') {
					if ((i % 2 + j % 2) % 2 == 0) {//find if square index is even or odd (square is black or white)
						wBishopsOnBlack++;
					}
					else {
						wBishopsOnWhite++;
					}
				}
				else if ((i % 2 + j % 2) % 2 == 0) {//find if square index is even or odd (square is black or white)
						wBishopsOnBlack++;
					}
					else {
						wBishopsOnWhite++;
					}
			}
			else if (board[i][j].Symbol == 'n') {
				if (board[i][j].Colour == 'W') {
					wKnights++;
				}
				else bKnights++;
			}
		}
	}
	if ((wBishopsOnBlack > 0 && wBishopsOnWhite > 0) || (bBishopsOnBlack > 0 && bBishopsOnWhite > 0)){//2 or more bishops on opposite colour squares for 1 player
		return 0;
	}
	if (((wBishopsOnBlack > 0 || wBishopsOnWhite > 0) && wKnights > 0) || ((bBishopsOnBlack > 0 || bBishopsOnWhite > 0) && bKnights > 0)) {//bishop and knight for 1 player
		return 0;
	}
	if ((wKnights > 0 && (bKnights > 0 || bBishopsOnBlack>0 ||bBishopsOnWhite>0))||(bKnights>0&&(wKnights>0||wBishopsOnBlack>0||wBishopsOnWhite>0))) {//technically possible to checkmate with 1 knight (or more) if your opponent has at least 1 knight or bishop. aslo covers 1 bishop against knight
		return 0;
	}
	if ((wBishopsOnBlack > 0 && bBishopsOnWhite > 0) || (wBishopsOnWhite > 0 && bBishopsOnBlack > 0)) {//technically possible to checkmate with 1 bishop against an opposite square bishop
		return 0;
	}
	return 1;
}
//this ui is bad but simple and works just enough, could use ascii drawings
void drawBoard(Piece board[8][8]) { // draw board from top left to bottom right in rows i.e. array element 0,7 to array element 7,7 then 0,6 to 7,6 etc.
	//k will be used to draw the lines. Also black squares have coordinates that add up to even e.g. (1,1), (1,3), (5,3) etc. and white odd.
	////draw function will use (char)254 for white squares and (char) 255 for black and draw a square of 4x3 (due to spacing in the console). unfortunately even is bad for symmetry
	for (int j = 7; j > -1; j--) {//board orientation with white at the bottom. could 'flip' (rotate) it for black's turn
		for(int k=0;k<3;k++){
			for (int i = 0; i < 8; i++) {
				if (i == 0 && k == 1) {
					cout << j + 1 << " ";//add labels for rank
				}
				if (i==0&&k!=1) {
					cout << "  ";
				}
				if (k == 1 && board[i][j].Symbol!=0) {
					if ((i + j) % 2 == 0)
						cout << (char)255 << board[i][j].Colour << board[i][j].Symbol << (char)255;
					else cout << (char)254 << board[i][j].Colour << board[i][j].Symbol << (char)254;
				}
				else if ((i + j) % 2 == 0)
					cout << (char)255 << (char)255 << (char)255 << (char)255;
				else cout << (char)254 << (char)254 << (char)254 << (char)254;
			}
			cout << endl;
		}
	}
	//cout << " ";//add labels for file
	for (int i = 0; i < 8; i++) {
		cout << "   " << char(i + int('a'));
	}
	cout << endl;
};

//VSC suggested stack was too big (~100kiB) so move these out of main() to allocate in heap (particularly record which is BIG)
Piece board[8][8];
Piece record[8][8][100];//reserve a record of up to 100 moves for threefold repetition draw.
//record could technically be a dynamic array instead of static but would need to be updated every turn, less memory used at the cost of more operations

int main() {
	clearBoard(board);
	newBoard(board);
	char colour = 'W';//white starts
	bool gameIsOver = 0;//small redundancy, could just make while condition true
	int turnNumber = 0, move100 = 0;
	while (gameIsOver == 0) {//start gameloop
	choosePiece:
		drawBoard(board);
		if (colour == 'W') {
			cout << "White ";
		}
		else {
			cout << "Black ";
		}
		cout << "to move" << endl;
		coord startSquare = choosePiece(board, colour);
		int numberMoves = getPossibleMoves(board, startSquare);//need to add castling into getPossibleMoves()
		if (numberMoves == 0) {
			cout << "The piece at " << char(startSquare.x + int('a')) << startSquare.y + 1 << " cannot move" << endl;
			goto choosePiece;
		}
		cout << "The possible moves are: ";
		for (int i = 0; i < numberMoves; i++) {
			cout << char(possibleMoves[i].x + int('a')) << possibleMoves[i].y + 1 << " ";
		}
		cout << endl;
	chooseTargetSquare:
		coord targetSquare = movePiece(board, colour);
		for (int i = 0; i < numberMoves; i++) {
			if (possibleMoves[i].x == targetSquare.x && possibleMoves[i].y == targetSquare.y) {
				goto targetSquareGood;
			}
		}
		cout << "You cannot move that piece to " << char(targetSquare.x + int('a')) << targetSquare.y + 1 << endl;
		goto chooseTargetSquare;
	targetSquareGood://if targetSquare is a possible move, execute the move
		char confirm;
		cout << "Would you like to move " << char(startSquare.x+'a') << startSquare.y+1 << " to " << char(targetSquare.x+'a') << targetSquare.y+1 << "?(y/n)";
		cin >> confirm;
		if (confirm == 'y') {
			goto moveConfirmed;
		}
		else if (confirm != 'n') {
			goto targetSquareGood;
		}
		else {
			goto choosePiece;
		}
	moveConfirmed:
		if (board[targetSquare.x][targetSquare.y].Symbol != 0 || board[startSquare.x][startSquare.y].Symbol == 'p') {//if capture or pawn move reset 50 move rule
			move100 = 0;
		}
		if (board[startSquare.x][startSquare.y].Symbol == 'p') {//set enpassants 
			if (targetSquare.y == startSquare.y + 2 || targetSquare.y == startSquare.y - 2) {//don't need to check HasMoved, covered in getPossibleMoves()
				if (board[targetSquare.x + 1][targetSquare.y].Symbol == 'p' && board[targetSquare.x + 1][targetSquare.y].Colour != colour) {
					board[targetSquare.x + 1][targetSquare.y].CanEnpassantLeft = 1;
				}
				if (board[targetSquare.x - 1][targetSquare.y].Symbol == 'p' && board[targetSquare.x - 1][targetSquare.y].Colour != colour) {
					board[targetSquare.x - 1][targetSquare.y].CanEnpassantRight = 1;
				}
			} //remove the pawn when enpassant is played
			if (targetSquare.x == startSquare.x - 1 && board[startSquare.x][startSquare.y].CanEnpassantLeft == 1) {//only ever occurs if target square is empty after double pawn push from rank 2 or 7
				board[startSquare.x-1][startSquare.y].Symbol = 0, board[startSquare.x-1][startSquare.y].Colour = 0;
			}
			if (targetSquare.x == startSquare.x + 1 && board[startSquare.x][startSquare.y].CanEnpassantRight == 1) {//only ever occurs if target square is empty after double pawn push from rank 2 or 7
				board[startSquare.x +1][startSquare.y].Symbol = 0, board[startSquare.x + 1][startSquare.y].Colour = 0;
			}//call promote() when they reach rank 1 or 8
			if (targetSquare.y == 7 || targetSquare.y == 0) {//this is weird because it is promoted then moved but the move has already been confirmed and don't need to check symbol again
				promote(board, startSquare);
			}
		}
		clearEnpassant(board, colour);//set enpassant to 0 so it doesn't carry over to the new square, also if pawn taken was between 2, the other pawn will also lose it's canEnpassant flag
		board[startSquare.x][startSquare.y].HasMoved = 1;//this sets for both squares after the piece moves
		board[targetSquare.x][targetSquare.y] = board[startSquare.x][startSquare.y];//copying the piece to the new square
		board[startSquare.x][startSquare.y].Symbol = 0, board[startSquare.x][startSquare.y].Colour = 0;//deleting the piece from the old square
		if (board[targetSquare.x][targetSquare.y].Symbol == 'k') {//castling moving the rook
			if (startSquare.x - 2 == targetSquare.x) {//castling queenside
				board[0][targetSquare.y].HasMoved = 1;
				board[targetSquare.x + 1][targetSquare.y] = board[0][targetSquare.y];
				board[targetSquare.x + 1][targetSquare.y].HasMoved= 1;
				board[0][targetSquare.y].Symbol = 0;
				board[0][targetSquare.y].Colour = 0;
				board[0][targetSquare.y].HasMoved = 1;
			}
			else if (startSquare.x + 2 == targetSquare.x) {//castling kingside
				board[7][targetSquare.y].Symbol = 0;
				board[targetSquare.x - 1][targetSquare.y] = board[7][targetSquare.y];
				board[targetSquare.x - 1][targetSquare.y].HasMoved = 1;
				board[7][targetSquare.y].Symbol = 0;
				board[7][targetSquare.y].Colour = 0;
				board[7][targetSquare.y].HasMoved = 1;
			}
		}
		for (int i = 0; i < 8;i++) {//copy every square on the board to record
			for (int j = 0; j < 8;j++) {
				record[i][j][move100] = board[i][j];
			}
		}
		move100++;
		if (isThreefold(record, move100)) {//check if 3rd repetition of a position has occurred
			drawBoard(board);
			gameIsOver = 1;
			cout << "This exact position has been repeated 3 times. Draw by repetition." << endl;
			goto gameEnd;
		}
		turnNumber++;
		colour = getMyColour(turnNumber);
		if (canMove(board, colour) == 0) {
			drawBoard(board);
			gameIsOver = 1;
			if (squareIsCheck(board, myKingLocation(board, colour), colour)){
				if (colour == 'B') {
					cout << "Checkmate! White wins." << endl;
					goto gameEnd;
				}
				else {
					cout << "Checkmate! Black wins." << endl;
					goto gameEnd;
				}
			}
			else {
				cout << "The game ends in a draw by stalemate." << endl;
				goto gameEnd;
			}
		}
		if (isFiftyMoves(move100)) {
			drawBoard(board);
			gameIsOver = 1;
			cout << "Each player has played 50 moves without a capture or pawn move. Draw by 50 move rule." << endl;
			goto gameEnd;
		}
		if (isInsufficientMaterial(board)) {
			drawBoard(board);
			gameIsOver = 1;
			cout << "No checkmate is possible with the pieces on the board. Draw due to insufficient material." << endl;
			goto gameEnd;
		}
	}
	gameEnd:
	return 0;
}
