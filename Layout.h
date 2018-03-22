#ifndef LAYOUT_H
#define LAYOUT_H

#include "Round.h"
#include <vector>
using namespace std; 

class Layout {
	friend class Tiles;
	friend class Computer;
	friend class Human;
	friend class LayoutView;
public:

	//Checks if the engine has been played yet or not
	//Returns true if it has been played
	bool enginePlayed(Round myRound);

	//Check if a tile can be played on the layout
	//whichPlayer = human, or pass which means the last player passed,
	//anything else is computer
	bool playableTile(string whichPlayer, int lhs, int rhs);

	//Check if the double tile the user wants to play can be played 
	//on the chosen side. Right side is r, anything else is left
	bool playableSide(string side, int lhs, int rhs);

	//Checks if the tile needs to be rotated when it's played/
	//added to the layout
	//side = r for right, anything else is for left
	bool rotateTile(string side, int lhs, int rhs);

private:
	//Played tiles
	vector<int> playedLeft;
	vector<int> playedRight;
};
#endif
