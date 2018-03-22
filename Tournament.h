#ifndef TOURNAMENT_H
#define TOURNAMENT_H

# pragma once

#include <string>

using namespace std;

class Tournament
{
public:
	Tournament();

	//This checks if the game is over by comparing the player scores to the 
	//final tournament score
	bool isTourneyOver();

	int getTourneyScore();

	int getHumanScore();

	int getCompScore();

	void setTourneyScore(int inputScore);

	void setCompScore(int inputScore);

	void setHumanScore(int inputScore);

	//Print who won the tournament:
	void whoWon();

private:
	//Final goal tournament score:
	int tourneyScore;

	int humanScore;

	int compScore;

};

#endif
