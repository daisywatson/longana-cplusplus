#ifndef TILES_H
#define TILES_H

# pragma once

#include "Round.h"
#include "Tournament.h"
#include "Player.h"
#include "Layout.h"

#include <vector>
#include <string>

using namespace std;

class Tiles
{
	friend class Computer;
	friend class Human;
	friend class Layout;
public:
	Tiles();

	//Initialize tiles for a new game
	void newGame(Round myRound, Tournament myTourney, Player &myPlayer, Layout &theBoard);

	//Load game from file.
	void loadGame(Round &myRound, Tournament &myTourney, Layout &theBoard);

	//Plays the engine if hasn't alreayd been played
	//i.e. from a loaded file
	void playEngine(Round myRound, Tournament myTourney, Player &myPlayer, Layout &theBoard);

	//Save the current game
	//nextPlayer = Computer or Human
	void saveGame(Round myRound, Tournament myTourney, string nextPlayer, Layout theBoard);

	//Return true if file loaded successfully
	bool successLoad();

	//Returns whether the computer is playing first when the game begins 
	bool isCompTurn();

	//Set if it's the computer's turn or not
	void setCompTurn();

	//Check if there are any tiles left in the players' hand
	//whichPlayer = human, anything else is computer
	bool isHandEmpty(string whichPlayer);

	//How many tiles in the stock left
	unsigned howManyStock();

	//Print the player's tiles to the screen
	//whichPlayer = human. Anything else will display computer's hand
	void printPlayerHand(string whichPlayer);

	//Print the stock. Purely for testing purposes: 
	void printStock();

	//Checks if there are any playable tiles in hand
	//whichPlayer = human anything else is computer
	//pass = pass, anything else doesn't count
	bool anyPlayableTile(string whichPlayer, string pass, Layout theBoard);

	//Check if there are any tiles left in the stock
	bool isStockEmpty();

	//This will draw one tile from the stock and add it to the player's hand 
	//whichPlayer = human, anything else is computer
	void drawFromStock(string whichPlayer);

	//Check if the tile to be played by user is a tile that exists in hand
	bool validTile(int lhs, int rhs);

	//Play user tile
	//r is for right side, anything else is left side
	void playUserTile(string side, int lhs, int rhs, Layout &theBoard);

	//Play computer tile
	//r is for right side, anything else is left side
	void playCompTile(string side, int lhs, int rhs, Layout &theBoard);

	//Calculates the score of a player's hand:
	//whichPlayer = human, anything else is computer
	int handScore(string whichPlayer);

	//Determine and print who won
	void whoWon(Tournament &myTourney);
	
	
private:
	//Create the left hand side pips of the initialized tiles
	vector<int> tileLeft;
	//Create the right hand side pips of the initialized tiles
	vector<int> tileRight;
	//Random order (shuffled) of tiles to start the round
	vector<int> randTiles;

	//Whether or not it's the computer's turn at the beginning of a game
	bool compTurn;

	//If a loaded file was successfully opened
	bool openSuccess;

	//Tiles in hand of computer/human
	vector<int> compLeft;
	vector<int> compRight;
	vector<int> humanLeft;
	vector<int> humanRight;
	//Tiles in the stock:
	vector<int> stockLeft;
	vector<int> stockRight;

};
#endif
