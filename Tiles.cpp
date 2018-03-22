#include "Tiles.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>      
#include <cstdlib> 
#include <fstream>
using namespace std;

Tiles::Tiles() {
	openSuccess = false;
	compTurn = false;
}


void Tiles::newGame(Round myRound, Tournament myTourney, Player &myPlayer, Layout &theBoard)
{
	cout << endl;
	cout << "A new game has started." << endl;

	//If there was previous round, reset the tiles:
	tileLeft.clear();
	tileRight.clear();
	randTiles.clear();
	compLeft.clear();
	compRight.clear();
	humanLeft.clear();
	humanRight.clear();
	stockLeft.clear();
	stockRight.clear();
	theBoard.playedLeft.clear();
	theBoard.playedRight.clear();

	//initialize random order of tiles:
	for (unsigned r = 0; r<28; ++r)randTiles.push_back(r);
	//shuffle:
	srand(unsigned(time(0)));
	random_shuffle(randTiles.begin(), randTiles.end());

	//initialize left side of the tiles
	unsigned count = 7;
	for (unsigned i = 0; i <= 6; i++)
	{
		for (unsigned j = 0; j < count; j++)
		{
			tileLeft.push_back(i);
		}
		count--;
	}

	//initialize right side of tiles
	unsigned kBegin = 0;
	while (kBegin <= 6)
	{
		for (unsigned k = kBegin; k <= 6; k++)
		{
			tileRight.push_back(k);
		}
		kBegin++;
	}


	//Draw the first 16 randomly shuffled tiles (according to randTiles) 
	//and put 8 of them into the computer hand
	for (unsigned c = 0; c < 8; c++)
	{
		compLeft.push_back(tileLeft[randTiles[c]]);
		compRight.push_back(tileRight[randTiles[c]]);
	}
	cout << endl;


	//Put 8 other tiles into human's hand
	//unsigned humanValid = 0; //for testing purposes
	for (unsigned h = 8; h < 16; h++)
	{
		humanLeft.push_back(tileLeft[randTiles[h]]);
		humanRight.push_back(tileRight[randTiles[h]]);
	}

	//Put them remaining tiles in the stock 
	//unsigned stockValid = 0;
	for (unsigned s = 16; s < 28; s++)
	{
		stockLeft.push_back(tileLeft[randTiles[s]]);
		stockRight.push_back(tileRight[randTiles[s]]);
	}

	this->playEngine(myRound, myTourney, myPlayer, theBoard);
}

void Tiles::playEngine(Round myRound, Tournament myTourney, Player &myPlayer, Layout &theBoard)
{
	//Get the engine number:
	unsigned engineNum = (unsigned)myRound.getEngineNum();
	compTurn = false;
	//Location of engine in hand:
	int engineLocation;
	//Draw from the stock if neither player has the engine 
	bool drawAgain = true;

	//Figure out which player has the engine
	for (unsigned e = 0; e < 8; e++)
	{
		//If the human player has the engine
		if (humanLeft[e] == engineNum && humanRight[e] == engineNum)
		{
			cout << "The user has the engine" << endl;
			drawAgain = false;
			engineLocation = e;
			break;
		}
		//if the computer has the engine
		else if (compLeft[e] == engineNum && compRight[e] == engineNum)
		{
			cout << "The computer has the engine." << endl;
			drawAgain = false;
			compTurn = true;
			engineLocation = e;
			break;
		}
	}

	cout << "Each player has drawn 8 tiles." << endl;

	//Ask if the user wants to save the game here, after both players have drawn 8 tiles
	string saveGame;
	do {
		cout << "Would you like to save the game? (y/n) ";
		cin >> saveGame;
	} while (saveGame != "y" && saveGame != "n");
	if (saveGame == "y")
	{
		//By default the next player is the human
		this->saveGame(myRound, myTourney, "Human", theBoard);
		myPlayer.setQuit();
		return;
	}

	//Number of tiles in hand:
	unsigned handNum = 9;
	//If neither player has the engine, keep drawing from the stock until one of them does
	while (drawAgain == true)
	{
		cout << "Neither player has the engine. Need to draw again. " << endl;

		//Add a value from the stock to each player's hand 
		//stockVal represents the position of the first tile in the stock
		unsigned stockVal = 0;
		//Add to human hand and erase from stock
		humanLeft.push_back(stockLeft[stockVal]);
		stockLeft.erase(stockLeft.begin());
		humanRight.push_back(stockRight[stockVal]);
		stockRight.erase(stockRight.begin());
		//Add to computer hand and erase from stock
		compLeft.push_back(stockLeft[stockVal]);
		stockLeft.erase(stockLeft.begin());
		compRight.push_back(stockRight[stockVal]);
		stockRight.erase(stockRight.begin());

		//Figure out which player has the engine:
		for (unsigned e = 0; e < handNum; e++)
		{
			//If the human player has the engine
			if (humanLeft[e] == engineNum && humanRight[e] == engineNum)
			{
				drawAgain = false;
				engineLocation = e;
				break;
			}
			//if the computer has the engine
			else if (compLeft[e] == engineNum && compRight[e] == engineNum)
			{
				drawAgain = false;
				compTurn = true;
				engineLocation = e;
				break;
			}
		}
		handNum++;
	}

	//Add the engine to the layout:
	theBoard.playedLeft.push_back(engineNum);
	theBoard.playedRight.push_back(engineNum);
	//Erase the engine tile from the player's hand:
	if (compTurn == true)
	{
		cout << "Computer has the engine. Computer plays the engine." << endl;
		//Remove the tile from the computer's hand:
		compLeft.erase(compLeft.begin() + engineLocation);
		compRight.erase(compRight.begin() + engineLocation);

		//The computer has played, so the next player is the user:
		compTurn = false;
	}
	else
	{
		cout << "These are the tiles in your hand: " << endl;
		this->printPlayerHand("human");

		//Force the player to play the engine
		cout << "You have the engine!" << endl;
		string pointlessInput;
		cout << "You must play the engine, type any key then press Enter to continue: " << endl;
		cin >> pointlessInput;

		//Erase the tile from the user's hand:
		humanLeft.erase(humanLeft.begin() + engineLocation);
		humanRight.erase(humanRight.begin() + engineLocation);

		//The user has played, so the next player is the computer:
		compTurn = true;
	}

	//Ask if the user wants to save the game here, after a player has played a tile
	string saveGame2;
	do {
		cout << "Would you like to save the game? (y/n) ";
		cin >> saveGame2;
	} while (saveGame2 != "y" && saveGame2 != "n");
	if (saveGame2 == "y")
	{
		if (compTurn)
		{
			this->saveGame(myRound, myTourney, "Computer", theBoard);
		}
		else {
			this->saveGame(myRound, myTourney, "Human", theBoard);
		}
		myPlayer.setQuit();
		return;
	}

}

void Tiles::loadGame(Round &myRound, Tournament &myTourney, Layout &theBoard)
{
	//False until a file is opened or user quits
	bool doneWithFile = false;
	string fileName;
	//Hold the data from the file:
	vector<string> fileText;
	do {
		
		cout << "What is the name of the file you would like to load the game from? (i.e. test1.txt): " << endl;
		cin >> fileName;

		ifstream openFile(fileName);
		if (!openFile)
		{
			cerr << "Can't open file!" << endl;
		}
		else {
			doneWithFile = true;
			openSuccess = true;
			//Read from the file:
			string line;
			while (getline(openFile, line)) {
				fileText.push_back(line);
			}
		}
		openFile.close();

		//If the file fails to open, the user can try to open it again
		if (doneWithFile == false)
		{
			string tryAgain;
			do {
				cout << "Would you like to try again? (y/n): " << endl;
				cin >> tryAgain;
			} while (tryAgain != "y" && tryAgain != "n");
			if (tryAgain == "n") doneWithFile = true;
		}
	} while (doneWithFile == false);

	if (openSuccess)
	{
		//----Parse data: 

		//Parsed data stored here:
		string parsedData[10];
		//Number of characters from the beginning of the line to the data desired:
		int dataSize[10] = { 18, 11, 9, 10, 9, 10, 4, 0, 24, 13 };
		//Counter for iterating through arrays:
		int parserCount = 0;
		string currentLine;
		for (unsigned i = 0; i < fileText.size(); i++)
		{
			currentLine = fileText[i];
			if (currentLine.size() == 0 && i == 15)
			{
				//If the boneyard is empty, continue iterating through the data array
				parserCount++;
				continue;
			}
			else if (currentLine.size() == 0)
			{
				//If it's an empty line, go to the next line
				continue;
			}
			else {
				//See if the current line is an unnecessary line
				//Skip if it is
				if (currentLine.find("Computer:") != -1 || currentLine.find("Human:") != -1 ||
					currentLine.find("Layout:") != -1 || currentLine.find("Boneyard:") != -1)
				{
					continue;
				}
				//If it's not an empty/unnecessary line, parse the data:
				//Position of where the line ends
				int lineEnd = currentLine.size();
				
				//Ignore the line if there's no data/the line is empty
				if (dataSize[parserCount] - currentLine.size() <= 1)
				{
					parserCount++;
					continue;
				}
				else {
					//Input data from the line into the parsedData array
					parsedData[parserCount] = currentLine.substr(dataSize[parserCount], lineEnd - dataSize[parserCount]);
				}
				parserCount++;
			}
		}

		//Assign the loaded data 
		//1. Tournament score
		int tourneyScore = 200;
		tourneyScore = atoi(parsedData[0].c_str());
		myTourney.setTourneyScore(atoi(parsedData[0].c_str()));

		//2. Number of rounds
		int loadedRounds = 1;
		loadedRounds = atoi(parsedData[1].c_str());
		myRound.setRounds(atoi(parsedData[1].c_str()));
		myRound.setEngineNum(myRound.determineEngineNum());

		//3. computer's hand
		string compHand = parsedData[2];
		//Alternate sides of the tile:
		bool leftSide = true;
		for (unsigned k = 0; k < compHand.size(); k++) {
			char tempChar = compHand[k];
			if (tempChar != '-' && tempChar != ' ')
			{
				if (leftSide == true)
				{
					compLeft.push_back(tempChar - '0');
					leftSide = false;
				}
				else {
					compRight.push_back(tempChar - '0');
					leftSide = true;
				}
			}
		}

		//4. computer's score
		int compScore = atoi(parsedData[3].c_str());
		myTourney.setCompScore(atoi(parsedData[3].c_str()));
		//5. human's hand
		string humanHand = parsedData[4];
		//Alternate sides of the tile:
		leftSide = true;
		for (unsigned m = 0; m < humanHand.size(); m++) {
			char tempChar = humanHand[m];
			if (tempChar != '-' && tempChar != ' ')
			{
				if (leftSide == true)
				{
					humanLeft.push_back(tempChar - '0');
					leftSide = false;

				}
				else {
					humanRight.push_back(tempChar - '0');
					leftSide = true;
				}
			}
		}

		//6. human's score
		int humanScore = atoi(parsedData[5].c_str());
		myTourney.setHumanScore(atoi(parsedData[5].c_str()));

		//7. Layout 
		string layout = parsedData[6];
		//Alternate sides of the tile:
		leftSide = true;
		for (unsigned n = 0; n < layout.size(); n++) {
			char tempChar = layout[n];
			if (tempChar != '-' && tempChar != ' ' && tempChar != 'R')
			{
				if (leftSide == true)
				{
					theBoard.playedLeft.push_back(tempChar - '0');
					leftSide = false;

				}
				else {
					theBoard.playedRight.push_back(tempChar - '0');
					leftSide = true;
				}
			}
		}

		//8. Boneyard
		string boneyard = parsedData[7];
		//Alternate sides of the tile:
		leftSide = true;
		for (unsigned m = 0; m < boneyard.size(); m++) {
			char tempChar = boneyard[m];
			if (tempChar != '-' && tempChar != ' ')
			{
				if (leftSide == true)
				{
					stockLeft.push_back(tempChar - '0');
					leftSide = false;

				}
				else {
					stockRight.push_back(tempChar - '0');
					leftSide = true;
				}
			}
		}

		//9. See after #10

		//10. Next player
		string nextPlayer = parsedData[9];
		if (nextPlayer == "Computer")
		{
			compTurn = true;
			//cout << "it's the computer's turn" << endl;
		}

		//9. Previous Player Passed:
		bool prevPlayerPass = false;
		string prevPlayer = parsedData[8];
		if (prevPlayer == "Yes") prevPlayerPass = true;
		if (nextPlayer == "Computer" && prevPlayerPass == true)
		{
			//The user previously passed
			myRound.setUserPass();
		}
		else if (prevPlayerPass == true)
		{
			//The computer previously passed
			myRound.setCompPass();
		}
	}
}

void Tiles::saveGame(Round myRound, Tournament myTourney, string nextPlayer, Layout theBoard)
{
	//Make a string that holds whether the previous player passed or not:
	string prevPlayerPass = "No";
	if (nextPlayer == "Computer" && myRound.getCompPass())
	{
		prevPlayerPass = "Yes";
	}
	else if (nextPlayer == "Human" && myRound.getUserPass())
	{
		prevPlayerPass = "Yes";
	}

	//Make a string that holds computer's hand tiles:
	string compTiles;
	for (unsigned i = 0; i < compLeft.size(); i++)
	{
		compTiles += to_string(compLeft[i]);
		compTiles += "-";
		compTiles += to_string(compRight[i]);
		compTiles += " ";
	}

	//Make string that holds human hand's tiles
	string userTiles;
	for (unsigned j = 0; j < humanLeft.size(); j++)
	{
		userTiles += to_string(humanLeft[j]);
		userTiles += "-";
		userTiles += to_string(humanRight[j]);
		userTiles += " ";
	}

	//Make a string that holds the layout's tiles
	string layoutTiles;
	for (unsigned k = 0; k < theBoard.playedLeft.size(); k++)
	{
		layoutTiles += to_string(theBoard.playedLeft[k]);
		layoutTiles += "-";
		layoutTiles += to_string(theBoard.playedRight[k]);
		layoutTiles += " ";
	}

	//Make a string to hold the stock's tiles
	string stockTiles;
	for (unsigned m = 0; m < stockLeft.size(); m++)
	{
		stockTiles += to_string(stockLeft[m]);
		stockTiles += "-";
		stockTiles += to_string(stockRight[m]);
		stockTiles += " ";
	}

	string userFileName; 
	cout << "What would like to name the file? (i.e. game1.txt): ";
	cin >> userFileName;
	ofstream userFile(userFileName);
	if (userFile.is_open())
	{
		userFile << "Tournament Score: ";
		userFile << myTourney.getTourneyScore();
		userFile << "\n";
		userFile << "Round No.: ";
		userFile << myRound.getRounds();
		userFile << "\n";
		userFile << "\n";
		userFile << "Computer:\n";
		userFile << "   Hand: ";
		userFile << compTiles;
		userFile << "\n";
		userFile << "   Score: ";
		userFile << myTourney.getCompScore();
		userFile << "\n";
		userFile << "\n";
		userFile << "Human:\n";
		userFile << "   Hand: ";
		userFile << userTiles;
		userFile << "\n";
		userFile << "   Score: ";		
		userFile << myTourney.getHumanScore();
		userFile << "\n";
		userFile << "\n";
		userFile << "Layout:\n";
		userFile << "  L ";
		userFile << layoutTiles;
		userFile << "R";
		userFile << "\n";
		userFile << "\n";
		userFile << "Boneyard:\n";
		userFile << stockTiles;
		userFile << "\n";
		userFile << "\n";
		userFile << "Previous Player Passed: ";
		userFile << prevPlayerPass;
		userFile << "\n";
		userFile << "\n";
		userFile << "Next Player: ";
		userFile << nextPlayer;
		userFile << "\n";
		userFile.close();
	}
	else cout << "Unable to open file.";
}


bool Tiles::successLoad()
{
	return openSuccess;
}

bool Tiles::isCompTurn()
{
	return compTurn;
}

void Tiles::setCompTurn()
{
	compTurn = true;
}

void Tiles::printPlayerHand(string whichPlayer)
{
	if (whichPlayer == "human")
	{
		for (unsigned i = 0; i < humanLeft.size(); i++)
		{
			cout << humanLeft[i] << "-" << humanRight[i] << " ";
		}
		cout << endl;
	}
	else {
		for (unsigned j = 0; j < compLeft.size(); j++)
		{
			cout << compLeft[j] << "-" << compRight[j] << " ";
		}
		cout << endl;
	}
}


void Tiles::printStock()
{
	for (unsigned i = 0; i < stockLeft.size(); i++)
	{
		cout << stockLeft[i] << "-" << stockRight[i] << " "; 
	}
	cout << endl;
}

bool Tiles::validTile(int lhs, int rhs)
{
	bool isValid = false;

	for (unsigned j = 0; j < humanLeft.size(); j++)
	{
		if (humanLeft[j] == lhs && humanRight[j] == rhs)
		{
			isValid = true;
			break;
		}
	}

	return isValid;
}


bool Tiles::anyPlayableTile(string whichPlayer, string pass, Layout theBoard)
{
	bool playableTileExists = false;
	if (whichPlayer == "human")
	{
		for (unsigned i = 0; i < humanLeft.size(); i++)
		{
			int lhs = humanLeft[i];
			int rhs = humanRight[i];
			if (pass == "pass")
			{
				if (theBoard.playableTile(pass, lhs, rhs) == true)
				{
					playableTileExists = true;
					break;
				}
			}
			else {
				if (theBoard.playableTile(whichPlayer, lhs, rhs) == true)
				{
					playableTileExists = true;
					break;
				}
			}
		}
	}
	else {
		for (unsigned j = 0; j < compLeft.size(); j++)
		{
			int lhs = compLeft[j];
			int rhs = compRight[j];
			if (pass == "pass")
			{
				if (theBoard.playableTile(pass, lhs, rhs) == true)
				{
					playableTileExists = true;
					break;
				}
			}
			else {
				if (theBoard.playableTile(whichPlayer, lhs, rhs) == true)
				{
					playableTileExists = true;
					break;
				}
			}
		}
	}
	return playableTileExists;
}





void Tiles::playUserTile(string side, int lhs, int rhs, Layout &theBoard)
{
	int tileLoc;
	//Find location of the tile in the vectors
	for (unsigned i = 0; i < humanLeft.size(); i++)
	{
		if (humanLeft[i] == lhs && humanRight[i] == rhs)
		{
			tileLoc = i;
			//cout << "tileloc: " << tileLoc << endl;
		}
	}
	if (side == "r")
	{
		//insert onto the right side of the layout
		if (!theBoard.rotateTile("r", lhs, rhs))
		{
			theBoard.playedLeft.push_back(lhs);
			theBoard.playedRight.push_back(rhs);
		}
		else {
			//Swap the pips on the lhs/rhs if 
			//the domino needs to be rotated
			theBoard.playedLeft.push_back(rhs);
			theBoard.playedRight.push_back(lhs);
		}
	}
	else {
		//The left side of the layout is the human side
		//Insert the tile into the beginning of playedTiles
		//aka the left hand side of the layout
		vector<int>::iterator it = theBoard.playedLeft.begin();
		vector<int>::iterator it2 = theBoard.playedRight.begin();
		if (!theBoard.rotateTile("l", lhs, rhs))
		{
			theBoard.playedLeft.insert(it, lhs);
			theBoard.playedRight.insert(it2, rhs);
		}
		else {
			//Swap the pips on the lhs/rhs if 
			//the domino needs to be rotated
			theBoard.playedLeft.insert(it, rhs);
			theBoard.playedRight.insert(it2, lhs);
		}
	}
	//delete from the hand
	humanLeft.erase(humanLeft.begin() + tileLoc);
	humanRight.erase(humanRight.begin() + tileLoc);
}

void Tiles::playCompTile(string side, int lhs, int rhs, Layout &theBoard)
{
	int tileLoc;
	//Find location of the tile in the vectors
	for (unsigned i = 0; i < compLeft.size(); i++)
	{
		if (compLeft[i] == lhs && compRight[i] == rhs)
		{
			tileLoc = i;
		}
	}

	if (side == "r")
	{
		//Insert onto the end of the layout
		if (!theBoard.rotateTile("r", lhs, rhs))
		{
			theBoard.playedLeft.push_back(lhs);
			theBoard.playedRight.push_back(rhs);
		}
		else {
			//Swap the pips on the lhs/rhs if 
			//the domino needs to be rotated
			theBoard.playedLeft.push_back(rhs);
			theBoard.playedRight.push_back(lhs);
		}
	}
	else {
		//Insert into the beginning of the layout
		vector<int>::iterator it = theBoard.playedLeft.begin();
		vector<int>::iterator it2 = theBoard.playedRight.begin();
		if (!theBoard.rotateTile("l", lhs, rhs))
		{
			theBoard.playedLeft.insert(it, lhs);
			theBoard.playedRight.insert(it2, rhs);
		}
		else {
			//Swap the pips on the lhs/rhs if 
			//the domino needs to be rotated
			theBoard.playedLeft.insert(it, rhs);
			theBoard.playedRight.insert(it2, lhs);
		}
	}
	//delete from the hand
	compLeft.erase(compLeft.begin() + tileLoc);
	compRight.erase(compRight.begin() + tileLoc);
}


void Tiles::drawFromStock(string whichPlayer)
{
	//stockVal represents the position of the first tile in the stock
	unsigned stockVal = 0;
	if (whichPlayer == "human")
	{
		//Add to human hand and erase from stock
		humanLeft.push_back(stockLeft[stockVal]);
		stockLeft.erase(stockLeft.begin());
		humanRight.push_back(stockRight[stockVal]);
		stockRight.erase(stockRight.begin());
	}
	else {
		//Add to computer hand and erase from stock
		compLeft.push_back(stockLeft[stockVal]);
		stockLeft.erase(stockLeft.begin());
		compRight.push_back(stockRight[stockVal]);
		stockRight.erase(stockRight.begin());
	}
}

bool Tiles::isStockEmpty()
{
	if (stockLeft.empty())
	{
		return true;
	}
	else {
		return false;
	}
}

bool Tiles::isHandEmpty(string whichPlayer)
{
	if (whichPlayer == "human")
	{
		if (humanLeft.empty())
		{
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (compLeft.empty())
		{
			return true;
		}
		else {
			return false;
		}

	}

}

int Tiles::handScore(string whichPlayer)
{
	int playerScore = 0;

	if (whichPlayer == "human")
	{
		for (unsigned i = 0; i < humanLeft.size(); i++)
		{
			playerScore += humanLeft[i];
			playerScore += humanRight[i];
		}
	}
	else {
		for (unsigned i = 0; i < compLeft.size(); i++)
		{
			playerScore += compLeft[i];
			playerScore += compRight[i];
		}
	}

	return playerScore;
}

void Tiles::whoWon(Tournament &myTourney)
{
	cout << endl;
	cout << "This round has ended!" << endl;

	//1. If one of the players' hand is empty, they automatically win
	//They get the score of total pips in the opposite player's hand
	if (isHandEmpty("human"))
	{
		cout << "The human player has won!" << endl;
		cout << "The human's score is: " << handScore("computer") << endl;
		myTourney.setHumanScore(handScore("computer"));
	}
	else if (isHandEmpty("computer"))
	{
		cout << "The computer has won!" << endl;
		cout << "The computer's score is: " << handScore("human") << endl;
		myTourney.setCompScore(handScore("human"));
	}
	else {
		//2. Otherwise calculate the score
		//Player with smaller number of pips in hand wins
		if (handScore("human") < handScore("computer"))
		{
			cout << "The human player has won!" << endl;
			cout << "The human's score is: " << handScore("computer") << endl;
			myTourney.setHumanScore(handScore("computer"));
		}
		else if (handScore("human") > handScore("computer"))
		{
			cout << "The computer has won!" << endl;
			cout << "The computer's score is: " << handScore("human") << endl;
			myTourney.setCompScore(handScore("human"));
		}
		else {
			cout << "It's a tie!" << endl;
		}
	}

	cout << "The tournament score so far is: " << endl;
	cout << "Computer: " << myTourney.getCompScore() << endl;
	cout << "Human: " << myTourney.getHumanScore() << endl;
}

unsigned Tiles::howManyStock()
{
	return stockLeft.size();
}