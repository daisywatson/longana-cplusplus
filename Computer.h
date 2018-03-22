#ifndef COMPUTER_H
#define COMPUTER_H

# pragma once

#include "Player.h"
#include "Tiles.h"
#include "Tournament.h"
#include "Round.h"
#include "Layout.h"
#include "LayoutView.h"

class Computer : public Player
{
public:
	//User quit is if the user quit during their previous turn
	void play(Tiles &gameTiles, Round &myRound, Tournament myTourney, Player &myPlayer, Layout &theBoard, LayoutView myView);

private:

};

#endif