#include "Layout.h"

#include <iostream>

using namespace std;

bool Layout::enginePlayed(Round myRound)
{
	bool found = false;

	//Get the engine number:
	int engineNum = myRound.getEngineNum();

	for (unsigned i = 0; i < playedLeft.size(); i++)
	{
		if (playedLeft[i] == engineNum)
		{
			found = true;
			break;
		}
	}

	return found;
}

bool Layout::playableTile(string whichPlayer, int lhs, int rhs)
{
	bool isPlayable = false;
	//Left pips of the tile on the lhs of the layout
	int playedLeftLeft = playedLeft[0];
	//Right pips of the tile on the rhs of the layout
	int playedRightRight = playedRight[playedRight.size() - 1];

	if (whichPlayer == "pass")
	{
		if (rhs == playedLeftLeft || lhs == playedLeftLeft
			|| lhs == playedRightRight || rhs == playedRightRight)
		{
			isPlayable = true;
		}
	}
	//If the tile is a double
	else if (lhs == rhs)
	{
		if (lhs == playedLeftLeft || lhs == playedRightRight)
		{
			isPlayable = true;
		}
	}
	//else check if the tile is playable as a non-double
	else {
		if (whichPlayer == "human")
		{
			//The user can only play tiles on the LHS of the layout
			if (rhs == playedLeftLeft || lhs == playedLeftLeft)
			{
				isPlayable = true;
			}
		}
		else {
			if (lhs == playedRightRight || rhs == playedRightRight)
			{
				//The computer can only play tiles on the RHS of the layout
				isPlayable = true;
			}
		}
	}

	return isPlayable;
}

bool Layout::playableSide(string side, int lhs, int rhs)
{
	bool playable = false;
	//Left pips of the tile on the lhs of the layout
	int playedLeftLeft = playedLeft[0];
	//Right pips of the tile on the rhs of the layout
	int playedRightRight = playedRight[playedRight.size() - 1];

	if (side == "r")
	{
		if (lhs == playedRightRight)
		{
			playable = true;
		}
	}
	else {
		if (rhs == playedLeftLeft)
		{
			playable = true;
		}
	}

	return playable;
}


bool Layout::rotateTile(string side, int lhs, int rhs)
{
	bool rotate = false;
	//Left pips of the tile on the lhs of the layout
	int playedLeftLeft = playedLeft[0];
	//Right pips of the tile on the rhs of the layout
	int playedRightRight = playedRight[playedRight.size() - 1];

	if (side == "r")
	{
		if (rhs == playedRightRight)
		{
			rotate = true;
		}
	}
	else {
		if (lhs == playedLeftLeft)
		{
			rotate = true;
		}
	}

	return rotate;
}

