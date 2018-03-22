#ifndef LAYOUTVIEW_H
#define LAYOUTVIEW_H

#include "Layout.h"

class LayoutView
{
	friend class Layout;
public:

	//Print the layout/game board
	void printLayout(Layout theBoard);

};

#endif
