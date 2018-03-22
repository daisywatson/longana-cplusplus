#ifndef HUMAN_H
#define HUMAN_H

# pragma once

#include "Player.h"
#include "Tiles.h"
#include "Tournament.h"
#include "Round.h"
#include "Layout.h"
#include "LayoutView.h"

class Human : public Player
{
public:

	void play(Tiles &gameTiles, Round &myRound, Tournament myTourney, Player &myPlayer, Layout &theBoard, LayoutView myView);

	//If the computer passed last turn, tilesToCheck indicates that 
	//the user can play non-double tiles on either side
	//tilesToCheck = human or pass if the computer passed last turn
	void help(string tilesToCheck, Tiles gameTiles, Layout theBoard);

private:
};
#endif