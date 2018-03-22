#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

using namespace std;

class Player {


public:
	Player();
	~Player();

	virtual void play();

	//This sets quit to true
	void setQuit();

	//Return value of quit
	bool getQuit();


private:
	//If the user decides to quit the game
	bool quit;
};

#endif