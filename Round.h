#ifndef ROUND_H
#define ROUND_H

# pragma once

#include "Tournament.h"

class Round

{
public:
	Round();

	int getRounds();

	int getEngineNum();

	//If the game is loaded from the file, determine the engine num:
	int determineEngineNum();

	void setRounds(int inputRounds);

	void setEngineNum(int inputEngineNum);

	//This is called when a round ends. It sets the class variables accordingly
	void endRound();

	//Sets to true if the user or computer has passed
	void setUserPass();

	void setCompPass();
	
	//Sets to false if the round is not over yet
	void resetUserPass();

	void resetCompPass();

	//Get if the user or computer has passed
	bool getUserPass();

	bool getCompPass();

	//Return if a round is over
	bool isRoundOver();

	//Set the round as over
	void setRoundOver();

private:

	//Number of rounds
	int rounds;

	//The engine number
	int engineNum;

	//If the round is over:
	bool roundOver;

	//If the user or computer has passed last turn:
	bool userPass;
	bool compPass;

};
#endif
