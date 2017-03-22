#include "Piezas.h"
#include <vector>
#include <iostream>
/** CLASS Piezas
 * Class for representing a Piezas vertical board, which is roughly based
 * on the game "Connect Four" where pieces are placed in a column and 
 * fall to the bottom of the column, or on top of other pieces already in
 * that column. For an illustration of the board, see:
 *  https://en.wikipedia.org/wiki/Connect_Four
 *
 * Board coordinates [row,col] should match with:
 * [2,0][2,1][2,2][2,3]
 * [1,0][1,1][1,2][1,3]
 * [0,0][0,1][0,2][0,3]
 * So that a piece dropped in column 2 should take [0,2] and the next one
 * dropped in column 2 should take [1,2].
**/


/**
 * Constructor sets an empty board (default 3 rows, 4 columns) and 
 * specifies it is X's turn first
**/
Piezas::Piezas(){
	board.resize(BOARD_ROWS);
	for (int i = 0; i < BOARD_ROWS; i++)
	{
		board[i].resize(BOARD_COLS);
	}
	reset();
}

/**
 * Resets each board location to the Blank Piece value, with a board of the
 * same size as previously specified
**/
void Piezas::reset(){
	for (int i = 0; i < BOARD_ROWS; i++)
	{
		for (int j = 0; j < BOARD_COLS; j++)
		{
			board[i][j] = Blank;
		}
	}
	turn = X;
}

/**
 * Places a piece of the current turn on the board, returns what
 * piece is placed, and toggles which Piece's turn it is. dropPiece does 
 * NOT allow to place a piece in a location where a column is full.
 * In that case, placePiece returns Piece Blank value 
 * Out of bounds coordinates return the Piece Invalid value
 * Trying to drop a piece where it cannot be placed loses the player's turn
**/ 
Piece Piezas::dropPiece(int column)
{
	Piece returnPiece = turn;
	short i = 2;
	if (pieceAt(i, column) == Invalid)
		returnPiece = Invalid;
	else if (pieceAt(i, column) != Blank)
		returnPiece = Blank;
	else
	{
		while (i >= 0 && (pieceAt(i, column) == Blank))
		{
			if (pieceAt(i, column) == Blank)
				i--;
		}
		board[i+1][column] = turn;

	}

	/* toggle turns */
	if (turn == X)
		turn = O;
	else
		turn = X;

	return returnPiece;
}

/**
 * Returns what piece is at the provided coordinates, or Blank if there
 * are no pieces there, or Invalid if the coordinates are out of bounds
**/
Piece Piezas::pieceAt(int row, int column){
	if (row < 0 || row >= BOARD_ROWS || column < 0 || column >= BOARD_COLS)
	{
		return Invalid;
	}
	return board[row][column];
}

/**
 * Returns which Piece has won, if there is a winner, Invalid if the game
 * is not over, or Blank if the board is filled and no one has won ("tie").
 * For a game to be over, all locations on the board must be filled with X's 
 * and O's (i.e. no remaining Blank spaces). The winner is which player has
 * the most adjacent pieces in a single line. Lines can go either vertically
 * or horizontally. If both X's and O's have the same max number of pieces in a
 * line, it is a tie.
**/
Piece Piezas::gameState()
{
	int xMax, oMax;
	int blankCount = 12;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ((char)pieceAt(i,j) != ' ')
				blankCount--;
		}
	}
	if (blankCount != 0)
		return Invalid;
	xMax = countPiece(X);
	oMax = countPiece(O);
/*	std::cout << "xMax: " << xMax << std::endl;
	std::cout << "oMax: " << oMax << std::endl;
	std::cout << (char)pieceAt(2,0) << (char)pieceAt(2,1) << (char)pieceAt(2,2) << (char)pieceAt(2,3) << std::endl;
	std::cout << (char)pieceAt(1,0) << (char)pieceAt(1,1) << (char)pieceAt(1,2) << (char)pieceAt(1,3) << std::endl;
	std::cout << (char)pieceAt(0,0) << (char)pieceAt(0,1) << (char)pieceAt(0,2) << (char)pieceAt(0,3) << std::endl;
*/
	if (xMax > oMax)
		return X;
	else if (oMax > xMax)
		return O;
	return Blank;
}

/**
 * Counts the maximum number of adjacent pieces of same type in a row or column
 * Private Function only used for determining winner.
 */
int Piezas::countPiece(Piece piece)
{

	int ColCheck = 0;
	int ColMax = 0;
	int RowCheck = 0;
	int RowMax = 0;

	if (piece != X && piece != O)
		return -1;
	// check rows
	for (int i = 0; i < 3; i++)
	{
		RowCheck = 0;
		for (int j = 0; j < 4; j++)
		{
			RowCheck = RowCheck << 1;
			if (pieceAt(i, j) == piece)
				RowCheck++; // Add 1 to the bit-shifted value.
			/*std::cout << i << ", " << j << ':' << (char)pieceAt(i,j)
				  << std::endl << "RowCheck: " << RowCheck << std::endl;*/

		}
		/* To make sense of this case statement: convert
		 * All the case values into binary. */
		switch (RowCheck)
		{
			case 1:
			case 2:
			case 4:
			case 5:
			case 8:
			case 9:
			case 10: if (RowMax < 1)
					RowMax = 1;
				 break;
			case 3:
			case 6:
			case 11:
			case 12:
			case 13: if (RowMax < 2)
					RowMax = 2;
			         break;
			case 7:
			case 14: if (RowMax < 3)
				 	RowMax = 3;
				 break;
			case 15: if (RowMax < 4)
					 RowMax = 4;
				 break;
			default: if (RowMax == 0)
				 	RowMax = 0;
				 break;
		}
	}

	/* Check Columns */
	for (int j = 0; j < 4; j++)
	{
	ColCheck = 0;
		for (int i = 0; i  < 3; i++)
		{
			ColCheck = ColCheck << 1;
			if (pieceAt(i,j) == piece)
				ColCheck++;
		}
		switch (ColCheck)
		{
			case 1:
			case 2:
			case 4:
			case 5: if (ColMax < 1)
					ColMax = 1;
				break;
			case 3:
			case 6: if (ColMax < 2)
					ColMax = 2;
				break;
			case 7: if (ColMax < 3)
					ColMax = 3;
				break;
			default: if(ColMax == 0)
				 	ColMax = 0;
				 break;
		}
	}
/*	std::cout << (char)piece << " RowMax == " << RowMax << std::endl;
	std::cout << (char)piece << " ColMax == " << ColMax << std::endl;
*/

	return (RowMax > ColMax) ? RowMax : ColMax;
}

