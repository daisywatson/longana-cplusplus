#include "Round.h"

#include <iostream>
using namespace std;

Round::Round() {
	rounds = 1;
	engineNum = 6;
	userPass = false;
	compPass = false;
	roundOver = false;
}

int Round::getRounds()
{
	return rounds;
}

int Round::getEngineNum()
{
	return engineNum;
}

int Round::determineEngineNum()
{
	int determinedNum = 6; 

	for (int i = 1; i < rounds; i++)
	{
		if (determinedNum == 0)
		{
			determinedNum = 6;
		}
		else {
			determinedNum--;
		}
	}

	return determinedNum;
}

void Round::setRounds(int inputRounds)
{
	rounds = inputRounds;
}

void Round::setEngineNum(int inputEngineNum)
{
	engineNum = inputEngineNum;
}

void Round::setUserPass()
{
	userPass = true;
}

void Round::setCompPass()
{
	compPass = true;
}

void Round::resetUserPass()
{
	userPass = false;
}

void Round::resetCompPass()
{
	compPass = false;
}

bool Round::getUserPass()
{
	return userPass;
}

bool Round::getCompPass()
{
	return compPass;
}

void Round::endRound()
{
	roundOver = false;
	userPass = false;
	compPass = false;
	rounds++;

	if (engineNum == 0)
	{
		engineNum = 6;
	}
	else {
		engineNum--;
	}

}

//Return if a round is over
bool Round::isRoundOver()
{
	return roundOver;
}

//Set the round as over
void Round::setRoundOver()
{
	roundOver = true;
}

