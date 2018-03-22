#include "Computer.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


void Computer::play(Tiles &gameTiles, Round &myRound, Tournament myTourney, Player &myPlayer, Layout &theBoard, LayoutView myView)
{
	//If the user decided to quit their previous turn, quit the game
	if (!myPlayer.getQuit())
	{
		//If the computer's or user's hand is empty, end the round:
		if (gameTiles.isHandEmpty("computer") || gameTiles.isHandEmpty("human"))
		{
			myRound.setRoundOver();
			return;
		}

		//If the user has already passed on the previous turn
		//and the computer has no playable tiles, the round is over
		if (myRound.getCompPass() == true && myRound.getUserPass() == true && gameTiles.isStockEmpty())
		{			
			myRound.setRoundOver();
			return;
		}

		cout << endl;
		cout << "Computer's turn." << endl;

		cout << "There are " << gameTiles.howManyStock() << " tile(s) left in the stock." << endl;

		//Output the layout
		myView.printLayout(theBoard);

		//If the user passed last turn, non-doubles can be played
		string pass = "";
		if (myRound.getUserPass())
		{
			cout << "The user passed last turn. The computer can play non-doubles on either side of the engine this turn." << endl;
			pass = "pass";
		}

		//If the computer passed last turn but has a playable tile this turn, play it
		if (gameTiles.anyPlayableTile("computer", pass, theBoard))
		{
			myRound.resetCompPass();
		}

		//Computer checks if there are any playable tiles
		if (gameTiles.anyPlayableTile("computer", pass, theBoard) == false)
		{
			//Check if the stock is empty
			if (gameTiles.isStockEmpty())
			{
				//If there are no playable tiles, the computer passes
				//cout << "The stock is empty." << endl;
				cout << "There are no playable tiles in the computer's hand." << endl;
				cout << "The computer passes this turn." << endl;
				myRound.setCompPass();
			}
			else {
				//Draw one tile from the boneyard
				cout << "The computer has no playable tiles in hand so the computer draws one tile from the stock." << endl;
				gameTiles.drawFromStock("computer");

				//If the computer passed last turn but now has a playable tile after drawing, play it
				myRound.resetCompPass(); 

				//Check again if there are any playable tiles in the 
				//computer's hand after drawing one tile from the stock
				if (gameTiles.anyPlayableTile("computer", pass, theBoard) == false)
				{
					cout << "There are still no playable tiles in the computer's hand." << endl;
					cout << "The computer passes this turn." << endl;
					myRound.setCompPass();

				}
			}
		}

		if (myRound.getCompPass())
		{
			//If passing this turn, ask the user if they'd like to save the game
			string saveGame1;
			do {
				cout << "Would you like to save the game? (y/n) ";
				cin >> saveGame1;
			} while (saveGame1 != "y" && saveGame1 != "n");
			if (saveGame1 == "y")
			{
				gameTiles.saveGame(myRound, myTourney, "Human", theBoard);
				myPlayer.setQuit();
			}
		}
		
		//If there are playable tiles, the computer will play a tile:
		if (!myRound.getCompPass())
		{
			//If the user passed last turn, the computer
			//can play non-double tiles on either side
			string tilesToCheck = "computer";
			if (myRound.getUserPass())
			{
				tilesToCheck = "pass";
			}

			//Computer finds playable tiles:
			vector<int> playableTileLeft;
			vector<int> playableTileRight;
			for (unsigned i = 0; i < gameTiles.compLeft.size(); i++)
			{
				int lhs = gameTiles.compLeft[i];
				int rhs = gameTiles.compRight[i];
				if (theBoard.playableTile(tilesToCheck, lhs, rhs))
				{
					//cout << lhs << "-" << rhs << " ";
					playableTileLeft.push_back(lhs);
					playableTileRight.push_back(rhs);
				}
			}

			//Computer decides which tile to play
			//Computer prioritizes playing tiles with the highest pip number

			//First, it adds up the sum of the pips of each tile
			vector <int> tileSums;
			for (unsigned j = 0; j < playableTileLeft.size(); j++)
			{
				tileSums.push_back(playableTileLeft[j] + playableTileRight[j]);
			}

			//Then it finds the tile with the largest sum
			//Find the location of the largest sum in the vector
			vector<int>::iterator itSum = max_element(tileSums.begin(), tileSums.end());
			int largestLoc = distance(tileSums.begin(), itSum);

			//Initialize the tile to play to the tile with the largest sum of its pips:
			int playedLHS = playableTileLeft[largestLoc];
			int playedRHS = playableTileRight[largestLoc];


			//Figure out which side to play the tile on
			string whichSide;
			//Figure out whether the tile needs to be rotated or not
			string rotate = "";
			if (theBoard.playableSide("r", playedLHS, playedRHS))
			{
				whichSide = "right";
			}
			else if (theBoard.playableSide("r", playedRHS, playedLHS))
			{
				whichSide = "right";
				rotate = "transposed";
			}
			else if (theBoard.playableSide("l", playedLHS, playedRHS))
			{
				whichSide = "left";
			}
			else {
				whichSide = "left";
				rotate = "transposed";
			}

			//Convert the playSide to a format usable by the function to play the tile
			string playSide = "r";
			if (whichSide == "left") playSide = "l";
			//Play the tile:
			gameTiles.playCompTile(playSide, playedLHS, playedRHS, theBoard);

			//This is the computer's strategy
			string strategy = "It has the highest number of total pips among playable tiles.";
			if (playableTileLeft.size() < 2)
			{
				strategy = "It is the only playable tile.";
			}

			//Output what tile the computer played
			cout << "The computer played " << playedLHS << "-" << playedRHS;
			cout << " " << rotate << " on the " << whichSide << " side of the engine." << endl;
			cout << strategy << endl;

			//If the computer's hand is empty, end the round:
			if (gameTiles.isHandEmpty("computer"))
			{
				cout << "The computer's hand is empty." << endl;
				myRound.setRoundOver();
				return;
			}

			//Ask the user if they would like to save the game
			if (!myRound.isRoundOver())
			{
				string saveGame;
				do {
					cout << "Would you like to save the game? (y/n) ";
					cin >> saveGame;
				} while (saveGame != "y" && saveGame != "n");
				if (saveGame == "y")
				{
					gameTiles.saveGame(myRound, myTourney, "Human", theBoard);
					myPlayer.setQuit();
				}
			}
		}

	}
}