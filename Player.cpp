#include "Player.h"

Player::Player() {
	quit = false;
}

Player::~Player() {
}

void Player::play()
{
}

void Player::setQuit()
{
	quit = true;
}

bool Player::getQuit()
{
	return quit;
}

