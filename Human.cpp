#include "Human.h"
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;


void Human::play(Tiles &gameTiles, Round &myRound, Tournament myTourney, Player &myPlayer, Layout &theBoard, LayoutView myView)
{
	//If the user decided to quit their previous turn, quit the game
	if (!myPlayer.getQuit())
	{
		//If computer has already passed and user has no playable tiles:
		if (myRound.getCompPass() == true && myRound.getUserPass() == true && gameTiles.isStockEmpty())
		{
			myRound.setRoundOver();
			return;
		}

		//If the computer or user's hand is empty, end the round:
		if (gameTiles.isHandEmpty("human") || gameTiles.isHandEmpty("computer"))
		{
			myRound.setRoundOver();
			return;
		}

		cout << endl;
		cout << "It's your turn." << endl;

		cout << "There are " << gameTiles.howManyStock() << " tile(s) left in the stock." << endl;

		//Print the layout of the board:
		myView.printLayout(theBoard);

		//Print the user's hand:
		cout << "These are the tiles in your hand: " << endl;
		gameTiles.printPlayerHand("human");

		//If the computer passed last turn,
		//non-doubles can be played
		string pass = "";
		if (myRound.getCompPass())
		{
			pass = "pass";
		}

		//If the user passed last turn but they can play a tiles this turn, let them play the tile
		if (gameTiles.anyPlayableTile("human", pass, theBoard))
		{
			myRound.resetUserPass();
		}

		//Check if the player can play any of the tiles in their hand
		if (gameTiles.anyPlayableTile("human", pass, theBoard) == false)
		{
			//Check if the stock is empty
			if (gameTiles.isStockEmpty())
			{
				//cout << "The stock is empty." << endl;
				cout << "There are no playable tiles in your hand." << endl;
				cout << "You pass this turn." << endl;
				myRound.setUserPass();
			}
			else {
				//Draw one tile from the boneyard
				cout << "You draw a tile from the stock because you have no playable tiles in your hand." << endl;
				gameTiles.drawFromStock("human");

				//If the user passed last turn but they can play a tiles this turn after drawing a tile, let them play the tile
				myRound.resetUserPass();

				//Print the user's hand:
				cout << "These are the tiles in your hand: " << endl;
				gameTiles.printPlayerHand("human");

				//Check again if there are any playable tiles in the 
				//user's hand after drawing one tile from the stock
				if (gameTiles.anyPlayableTile("human", pass, theBoard) == false)
				{
					cout << "There are still no playable tiles in your hand." << endl;
					cout << "You pass this turn." << endl;
					myRound.setUserPass();
				}
			}
		}

		if (myRound.getUserPass())
		{
			//Ask the user if they want to save the game after their turn:
			string saveGame1;
			do {
				cout << "Would you like to save the game? (y/n) ";
				cin >> saveGame1;
			} while (saveGame1 != "y" && saveGame1 != "n");
			if (saveGame1 == "y")
			{
				gameTiles.saveGame(myRound, myTourney, "Computer", theBoard);
				myPlayer.setQuit();
			}
		}

		//The round can end if:
		//1. Both players have no playable tiles in hand and the stock is empty
		//aka both players have passed and the stock is empty
		//2. The player's hand is empty 	

		//If the user isn't passing this turn, let them play:
		if (!myRound.getUserPass())
		{
			//Get the user input to play a tile or get help first
			string userInput;
			do {
				cout << "Enter p to play a tile or h for help: " << endl;
				cin >> userInput;
			} while (userInput != "p" && userInput != "h");

			//If the computer passed last turn, the user
			//can play non-double tiles on either side
			string tilesToCheck = "human";
			if (myRound.getCompPass())
			{
				cout << "The computer passed last turn. You can play non-doubles on either side of the engine this turn." << endl;
				tilesToCheck = "pass";
			}

			//If the user wants help, call the help function
			if (userInput == "h")
			{
				this->help(tilesToCheck, gameTiles, theBoard);
			}

			bool tilePlayed = false;
			int lhs;
			int rhs;
			string userInputLeft;
			string userInputRight;
			//Get the user input for which tile to play:
			bool validTile = false;
			do {
				cout << "Enter the left hand side of the tile you want to play (the number only): ";
				cin >> userInputLeft;
				cout << "Enter the right hand side of the tile you want to play: ";
				cin >> userInputRight;
				//Check if the user input is a number or not:
				size_t found = userInputLeft.find_first_not_of("0123456789");
				size_t found2 = userInputRight.find_first_not_of("0123456789");
				if (found == string::npos && found2 == string::npos)
				{
					//Convert to int:
					lhs = atoi(userInputLeft.c_str());
					rhs = atoi(userInputRight.c_str());
					//Check if the tile exists in the hand:
					if (gameTiles.validTile(lhs, rhs) != true)
					{
						cout << "That's not a valid tile. Please try again." << endl;
						continue;
					}
					//Check if the tile can be played on the layout
					if (theBoard.playableTile(tilesToCheck, lhs, rhs) == true)
					{
						validTile = true;
						break;
					}
					else {
						cout << "That is not a tile that can be played. Please try again. " << endl;
					}
				}
				else {
					cout << "Invalid input. Please enter a number." << endl;
				}
			} while (validTile == false);

			//Check if the tile is a double tile
			bool isDouble = false;
			if (lhs == rhs) isDouble = true;

			//User can play on either side with a double tile
			//Or if the computer passed last turn
			string whichSide;
			if (isDouble == true || tilesToCheck == "pass")
			{
				bool validSide = false;
				do {
					do {
						cout << "Which side would you like to play this double tile on?: (l/r)" << endl;
						cin >> whichSide;
						if (whichSide != "l" && whichSide != "r")
						{
							cout << "That is not valid input, please try again." << endl;
						}
					} while (whichSide != "l" && whichSide != "r");
					//Check if legal side (tile can be played as is or transposed): 
					if (theBoard.playableSide(whichSide, lhs, rhs) == true || theBoard.playableSide(whichSide, rhs, lhs))
					{
						validSide = true;
					}
					else {
						cout << "That is not a valid side, please try again." << endl;
					}
				} while (validSide == false);
			}

			//Play the tile
			cout << "The user played " << lhs << "-" << rhs << endl;
			gameTiles.playUserTile(whichSide, lhs, rhs, theBoard);

			//If the user's hand is empty, end the round:
			if (gameTiles.isHandEmpty("human"))
			{
				cout << "The user's hand is empty." << endl;
				myRound.setRoundOver();
				return;
			}

			//Ask the user if they want to save the game at the end of this turn:
			if (!myRound.isRoundOver())
			{
				string saveGame;
				do {
					cout << "Would you like to save the game? (y/n) ";
					cin >> saveGame;
				} while (saveGame != "y" && saveGame != "n");
				if (saveGame == "y")
				{
					gameTiles.saveGame(myRound, myTourney, "Computer", theBoard);
					myPlayer.setQuit();
				}
			}
		}

	}
}

void Human::help(string tilesToCheck, Tiles gameTiles, Layout theBoard)
{
	//Computer finds playable tiles fo the user:
	vector<int> playableTileLeft;
	vector<int> playableTileRight;
	for (unsigned i = 0; i < gameTiles.humanLeft.size(); i++)
	{
		int lhs = gameTiles.humanLeft[i];
		int rhs = gameTiles.humanRight[i];
		if (theBoard.playableTile(tilesToCheck, lhs, rhs))
		{
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
	if (theBoard.playableSide("l", playedLHS, playedRHS))
	{
		whichSide = "left";
	}
	else if (theBoard.playableSide("l", playedRHS, playedLHS))
	{
		whichSide = "left";
		rotate = "transposed";
	}
	else if (theBoard.playableSide("r", playedLHS, playedRHS))
	{
		whichSide = "right";
	}
	else {
		whichSide = "right";
		rotate = "transposed";
	}

	//This is the computer's suggested strategy
	string strategy = "It has the highest number of total pips among playable tiles.";
	if (playableTileLeft.size() < 2)
	{
		strategy = "It is the only playable tile.";
	}

	//Output what tile the computer recommends
	cout << "The computer recommends playing " << playedLHS << "-" << playedRHS;
	cout << " " << rotate << " on the " << whichSide << " side of the engine. " << endl;
	cout << strategy << endl;

}
