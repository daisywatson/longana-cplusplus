#include "Tournament.h"

#include <iostream>

using namespace std;

Tournament::Tournament() {
	tourneyScore = 200;
	humanScore = 0;
	compScore = 0;
}


bool Tournament::isTourneyOver()
{
	if (humanScore >= tourneyScore)
	{
		return true;
	}
	else if (compScore >= tourneyScore)
	{
		return true;
	}
	else return false; 
}


int Tournament::getTourneyScore()
{
	return tourneyScore;
}

int Tournament::getHumanScore()
{
	return humanScore;
}

int Tournament::getCompScore()
{
	return compScore;
}


void Tournament::setTourneyScore(int inputScore)
{
	tourneyScore = inputScore;
}

void Tournament::setCompScore(int inputScore)
{
	compScore = inputScore;
}

void Tournament::setHumanScore(int inputScore)
{
	humanScore = inputScore;
}

void Tournament::whoWon()
{
	if (humanScore > compScore)
	{
		cout << "The human won!" << endl;
	}
	else if (compScore > humanScore)
	{
		cout << "The computer won!" << endl;
	}
	else {
		cout << "It's a tie!" << endl;
	}
}