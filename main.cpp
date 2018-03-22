//************************************************************
//* Name: Daisy Watson										 *
//* Project : C++ Longana						             *
//* Date : 10/17/17					                          *
//************************************************************

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>

#include "Tiles.h"
#include "Round.h"
#include "Tournament.h"
#include "Player.h"
#include "Human.h"
#include "Computer.h"
#include "Layout.h"
#include "LayoutView.h"

using namespace std;

int main()
{	
	//User input for new game or load a game
	string loadGame;
	do {
		cout << "Would you like to load a game from a file? (y/n): ";
		cin >> loadGame;
	} while (loadGame != "y" && loadGame != "n");

	//Create tournament, round, tiles, player
	//layout, layoutView objects
	Tournament myTourney;
	Round myRound;
	Tiles gameTiles;
	Player myPlayer;
	Layout theBoard;
	LayoutView myView;

	//Load the game or else create a new game. 
	if (loadGame == "y")
	{
		gameTiles.loadGame(myRound, myTourney, theBoard);
		//Play the engine if it hasn't been played yet:
		if (!theBoard.enginePlayed(myRound) && gameTiles.successLoad())
		{
			gameTiles.playEngine(myRound, myTourney, myPlayer, theBoard);
		}
	}
	else {	
		cout << "You're playing a new tournament!" << endl;
		bool validTourneyScore = true;
		string userNum;
		do {
			cout << "Enter a valid tournament score ( >0, <1000): ";
			cin >> userNum;
			//Convert to an int:
			int uNum = atoi(userNum.c_str());
			//Check if the user input is a number or not:
			size_t found = userNum.find_first_not_of("0123456789");
			if (found != string::npos)
			{
				cout << "That is not a valid tournament score, please try again." << endl;
				validTourneyScore = false;
			}
			else if (uNum < 0 || uNum > 1000)
			{
				cout << "That tournament score is too low/high, please try again." << endl;
				validTourneyScore = false;
			}
			else {
				validTourneyScore = true;
			}
		} while (validTourneyScore == false);
		int tourneyScore;
		tourneyScore = atoi(userNum.c_str());
		//Set the tourney score:
		myTourney.setTourneyScore(tourneyScore);
		//Create a new game:
		gameTiles.newGame(myRound, myTourney, myPlayer, theBoard);
	}

	//If the file isn't opened successfully, end the program 
	if (loadGame == "y" && !gameTiles.successLoad())
	{
		cout << "Goodbye." << endl;
		return 0;
	}

	//If the user quit already, end the game here
	if (myPlayer.getQuit())
	{
		cout << "Goodbye." << endl;
		return 0;
	}

	//Initialize the players:
	Human hPlayer;
	Computer cPlayer;
	//Loop for playing a tournament:
	do {
		//Loop for playing a round:
		if (gameTiles.isCompTurn() == true)
		{
			//If it's the computer's turn, the computer goes first
			do {
				cPlayer.play(gameTiles, myRound, myTourney, myPlayer, theBoard, myView);
				hPlayer.play(gameTiles, myRound, myTourney, myPlayer, theBoard, myView);
				//Loop until round is over or the player quits
			} while (myRound.isRoundOver() == false && !myPlayer.getQuit());
		}
		else {
			do {
				hPlayer.play(gameTiles, myRound, myTourney, myPlayer, theBoard, myView);
				cPlayer.play(gameTiles, myRound, myTourney, myPlayer, theBoard, myView);
			} while (myRound.isRoundOver() == false && !myPlayer.getQuit());
		}
		//At the end of a round (if the round is over):
		if (!myPlayer.getQuit())
		{
			//Print out who won the round
			gameTiles.whoWon(myTourney);
			//Set up for the next round:
			myRound.endRound();
			//Create new tiles for a new game if the game isn't over
			if (!myTourney.isTourneyOver()) gameTiles.newGame(myRound, myTourney, myPlayer, theBoard);
		}
	} while (myTourney.isTourneyOver() == false && !myPlayer.getQuit()); //quit if tournament over or user quits

	//If the tournament is over:
	if (myTourney.isTourneyOver())
	{
		cout << endl;
		cout << "The tournament is over!" << endl;
		//Print out who won the tournament
		myTourney.whoWon();
		//Print out scores:
		cout << "The computer's score is: " << myTourney.getCompScore() << endl;
		cout << "The human's score is: " << myTourney.getHumanScore() << endl;
	}

	cout << "Goodbye." << endl;
}