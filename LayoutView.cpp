#include "LayoutView.h"
#include <iostream>
using namespace std;

void LayoutView::printLayout(Layout theBoard)
{
	//Print the layout in 3 rows to represent the cross-wise double tiles
	//Top row:
	//Two spaces for L
	cout << "  ";
	for (unsigned i = 0; i < theBoard.playedLeft.size(); i++)
	{
		if (theBoard.playedLeft[i] == theBoard.playedRight[i])
		{
			cout << theBoard.playedLeft[i] << " ";
		}
		else
		{
			cout << "    ";
		}
	}
	cout << endl;
	//Middle row:
	cout << "L ";
	for (unsigned j = 0; j < theBoard.playedLeft.size(); j++)
	{
		if (theBoard.playedLeft[j] == theBoard.playedRight[j])
		{
			cout << "| ";
		}
		else
		{
			cout << theBoard.playedLeft[j] << "-" << theBoard.playedRight[j] << " ";
		}
	}
	cout << "R" << endl;

	//Bottom row:
	//Two spaces for L
	cout << "  ";
	for (unsigned k = 0; k < theBoard.playedLeft.size(); k++)
	{
		if (theBoard.playedLeft[k] == theBoard.playedRight[k])
		{
			cout << theBoard.playedLeft[k] << " ";
		}
		else
		{
			cout << "    ";
		}
	}
	cout << endl;
}