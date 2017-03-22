/**
 * Unit Tests for Piezas
**/

#include <gtest/gtest.h>
#include "Piezas.h"
#include <iostream>
 
class PiezasTest : public ::testing::Test
{
	protected:
		PiezasTest(){} //constructor runs before each test
		virtual ~PiezasTest(){} //destructor cleans up after tests
		virtual void SetUp(){} //sets up before each test (after constructor)
		virtual void TearDown(){} //clean up after each test, (before destructor) 
};

TEST(PiezasTest, sanityCheck)
{
	ASSERT_TRUE(true);
}

/* Constructor sanity check. Try-Catch for weird segfault stuff. */
TEST(PiezasTest, constructerCheck)
{
	try
	{
		SetUp();
		Piezas game;
		ASSERT_TRUE(true);
		TearDown();
	} 
	catch (std::exception& e)
	{
		std::cerr << "Exception thrown:  " << e.what() << std::endl;
		ASSERT_TRUE(false);
	}
	/* Dev's log: March the 20th of 2017. The try-catch attempt still didn't stop
	 * the seg fault from happening. Fortunately, we were able to avert crisis by
	 * fixing the constructor to the proper specification. Next time we may not
	 * be so lucky */
}

/* Reset implicitly tested with constructor. Explicitly testing that it blanks the board */
/* Run through the locations on the board and check if they are blank. If one is not, 
 * fail the test
 */
TEST(PiezasTest, resetCheck)
{
	Piezas game;
	char testPiece = ' ';
	unsigned short i, j;
	i = 0;
	j = 0;
	while (i < BOARD_ROWS && testPiece == ' ')
	{
		for (j = 0; j < 4; j++)
		{
			if ((char)game.pieceAt(i, j) != ' ')
				testPiece = (char)game.pieceAt(i, j);
		}
		i++;
	}
	if (i != BOARD_ROWS)
	{
		std::cerr << "Something goofed. i,j: " << i << "," << j << std::endl;
		std::cerr << "Piece at point: " << (char)game.pieceAt(i,j)
			  << std::endl;
	}
	SetUp();
	ASSERT_TRUE(testPiece == ' ');
	TearDown();
}

/* Trying to test all invalid instances of pieceAt(row, col) */
TEST(PiezasTest, invalidPieceAts)
{
	short i, j;
	Piezas game;
	char testPiece = '?';
	for (i = -1; i <= 3 && testPiece == '?'; i++)
	{
		for (j = -1; j <= 4 && testPiece == '?'; j += 5)
		{
			if ((char)game.pieceAt(i, j) != '?')
			{
				testPiece = (char)game.pieceAt(i,j);
			}
		}
	}
	SetUp();
	ASSERT_EQ('?', testPiece);
	TearDown();
}

/* Test dropPiece. See if it returns an X when dropped into 0,0  */
TEST(PiezasTest, checkXReturn)
{
	Piezas game;
	SetUp();
	ASSERT_EQ(game.dropPiece(0), 'X');
	TearDown();
}


/* Test dropPiece. See if it drops an X into 0,0  */
TEST(PiezasTest, dropXIntoZeroZero)
{
	Piezas game;
	char testPiece = (char)game.dropPiece(0);
	SetUp();
	ASSERT_EQ(testPiece, game.pieceAt(0,0));
	TearDown();
}


/* Drop an X. Then See if O drops. */
TEST(PiezasTest, checkOReturn)
{
	Piezas game;
	game.dropPiece(0);
	SetUp();
	ASSERT_EQ(game.dropPiece(0), 'O');
	TearDown();
}

/* Drop an X. Drop an O. See if O is on (1, 0) */
TEST(PiezasTest, dropOintoZeroZero)
{
	Piezas game;
	game.dropPiece(0);
	char testPiece = (char)game.dropPiece(0);
	SetUp();
	ASSERT_EQ(testPiece, game.pieceAt(1,0));
	TearDown();
}

/* Test trying to insert a piece into a full column.
 * This test fills all the columns and then   */
TEST(PiezasTest, testAllColumnsTop)
{
	Piezas game;
	int i = 0;
	char refPiece = ' ';

	/* Drop all the pieces in all the columns */
	for (i = 0; i < 4 && refPiece == ' '; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			game.dropPiece(i);
		}
		/* Try dropping a piece on the column. Should return invalid. */
		refPiece = (char)game.dropPiece(i);
	}

	if (refPiece != ' ')
		std::cerr << "Overflow on column " << i << std::endl;

	SetUp();
	ASSERT_EQ(refPiece, ' ');
	TearDown();
}

/* Test dropPiece on out of bounds cases */
TEST(PiezasTest, dropPieceOutOfBoundsTest)
{
	Piezas game;
	char testPiece = '?';
	int i = -1;
	for (i = -1; i <= 4 && testPiece == '?'; i += 5)
	{
		testPiece = (char)game.dropPiece(i);
	}
	SetUp();
	ASSERT_EQ(testPiece, '?');
	TearDown();
}

/* Test turn-skipping when dropping into a full column. X and O
 * will always drop into the same column four times, which means
 * that O will lose their turn on the fourth try. End result should
 * be:
 *	[X|X|X|X]
 *	[O|O|O|O]
 *	[X|X|X|X]
 *
 * NB: This test was written before the gameStatus function
 * was implemented.*/
TEST(PiezasTest, turnSkipTest)
{
	Piezas game;
	char testColumn[3] = {'X', 'O', 'X'};
	int i, j; // j = rows, i = cols
	bool testCorrect = true;
	for (i = 0; i < 4 && testCorrect; i++)
	{
		for (j = 0; j < 4; j++)
		{
			/*std::cout << "Dropping Piece: " << (char)game.dropPiece(i)
				  << " " << i << "," << j << std::endl;*/
			game.dropPiece(i); // Above code was for testing purposes
		}

		for (j = 0; j < 3 && testCorrect; j++)
		{
			testCorrect = (game.pieceAt(j, i) == testColumn[j]);
		}
	}


	SetUp();
	if (j < 3 || i < 4)
		std::cerr << "Test failed at: " << j << "," << i << std::endl;
	ASSERT_TRUE(testCorrect);
	TearDown();
}

TEST(PiezasTest, gameNotOver)
{
	Piezas game;
	SetUp();
	ASSERT_EQ((char)game.gameState(), '?');
	TearDown();
}

/**
 * Test X victory by filling the bottom row with Xs. Should Be:
 *
 *	[o][o][x][o]
 *	[o][o][x][o]
 *	[x][x][x][x]
 */
TEST(PiezasTest, gameXWinRow)
{
	Piezas game;
	game.dropPiece(0); // X
	game.dropPiece(0); // O
	game.dropPiece(1); // X
	game.dropPiece(0); // O
	game.dropPiece(2); // X
	game.dropPiece(1); // O
	game.dropPiece(3); // X
	game.dropPiece(1); // O
	game.dropPiece(2); // X
	game.dropPiece(3); // O
	game.dropPiece(2); // X
	game.dropPiece(3); // O

	SetUp();
	ASSERT_EQ((char)game.gameState(), 'X');
	TearDown();
}

/**
 * Test X victory by filling the bottom row with Xs. Should Be:
 *
 *	[x5]  [x11] [o12] [o10]
 *	[x3]  [o6]  [o8]  [x9]
 *	[x1]  [o2]  [x7]  [o4]
 */
TEST(PiezasTest, gameXWinCol)
{
	Piezas game;
	game.dropPiece(0); // X
	game.dropPiece(1); // O
	game.dropPiece(0); // X
	game.dropPiece(3); // O
	game.dropPiece(0); // X
	game.dropPiece(1); // O
	game.dropPiece(2); // X
	game.dropPiece(2); // O
	game.dropPiece(3); // X
	game.dropPiece(3); // O
	game.dropPiece(1); // X
	game.dropPiece(2); // O
	SetUp();
	ASSERT_EQ((char)game.gameState(), 'X');
	TearDown();
}

/*
 * Test that O can win by Row Victory.
 * Board should look like:
 *
 * 	2 [x7] [x9] [o12][x11]
 * 	1 [o4] [o6] [o8] [o10]
 * 	0 [x1] [x3] [x5] [o2]
 *	   0    1    2    3
 */

TEST(PiezasTest, gameOWinRow)
{
	Piezas game;
	game.dropPiece(0); // X
	game.dropPiece(3); // O
	game.dropPiece(1); // X
	game.dropPiece(0); // O
	game.dropPiece(2); // X
	game.dropPiece(1); // O
	game.dropPiece(0); // X
	game.dropPiece(2); // O
	game.dropPiece(1); // X
	game.dropPiece(3); // O
	game.dropPiece(3); // X
	game.dropPiece(2); // O
	SetUp();
	ASSERT_EQ((char)game.gameState(), 'O');
	TearDown();
}

/**
 * Test that O can win by Columns
 * Board should look like this
 *
 * 	2 [o12][x7][x11][o10]
 * 	1 [x5][o6][x9][o4]
 * 	0 [x1][x3][o8][o2]
 *	   0   1   2   3
 */
TEST(PiezasTest, gameOWinCol)
{
	Piezas game;
	game.dropPiece(0); // X
	game.dropPiece(3); // O
	game.dropPiece(1); // X
	game.dropPiece(3); // O
	game.dropPiece(0); // X
	game.dropPiece(1); // O
	game.dropPiece(1); // X
	game.dropPiece(2); // O
	game.dropPiece(2); // X
	game.dropPiece(3); // O
	game.dropPiece(2); // X
	game.dropPiece(0); // O
	SetUp();
	ASSERT_EQ((char)game.gameState(), 'O');
	TearDown();
}

/*
 * Drop pieces in checkboard pattern. Should return tie.
 * Board should look like:
 *
 * 	2 [x][o][x][o]
 * 	1 [o][x][o][x]
 * 	0 [x][o][x][o]
 *	   0  1  2  3
 *
 */
TEST(PiezasTest, gameCheckerBoardDraw)
{
	Piezas game;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			game.dropPiece(j);
		}
	}
	SetUp();
	ASSERT_EQ((char)game.gameState(), ' ');
	TearDown();
}

/*
 * Drop Pieces so that the end result is two rows X, one row O
 * Board should look like:
 *
 * 	2 [x][x][x][x]
 * 	1 [o][o][o][o]
 * 	0 [x][x][x][x]
 *
 */
TEST(PiezasTest, differentNumberPiecesDraw1)
{
	Piezas game;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			game.dropPiece(j);
		}
	}
	SetUp();
	ASSERT_EQ((char)game.gameState(), ' ');
	TearDown();
}

/*
 * Drop Pieces so that the end result is two rows X, one row O
 * Board should look like:
 *
 * 	2 [o][o][o][o]
 * 	1 [x][x][x][x]
 * 	0 [o][o][o][o]
 *
 */
TEST(PiezasTest, differentNumberPiecesDraw2)
{
	Piezas game;
	game.dropPiece(-1);
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			game.dropPiece(j);
		}
	}
	SetUp();
	ASSERT_EQ((char)game.gameState(), ' ');
	TearDown();
}

/**
 * Drop Pieces to achieve a column tie
 * Board should look like:
 *
 * 	2 [x][x][o][o]
 * 	1 [x][x][o][o]
 * 	0 [x][x][o][o]
 *	   0  1  2  3
 *
 */
TEST(PiezasTest, columnsDraw)
{
	Piezas game;
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			game.dropPiece(j);
			game.dropPiece(3-j);
		}
	}
	SetUp();
	ASSERT_EQ((char)game.gameState(), ' ');
	TearDown();
}
