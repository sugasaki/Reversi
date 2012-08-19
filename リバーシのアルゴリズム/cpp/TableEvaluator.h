#ifndef TABLEEVALUATOR_H_INCLUDED
#define TABLEEVALUATOR_H_INCLUDED

#include "Evaluator.h"

class TableEvaluator : public Evaluator
{
private:

	static const int Table[8][8];

public:

	int evaluate(const Board& board)
	{
		int e = 0;
		Point p;
		for(unsigned x=1; x<=BOARD_SIZE; x++)
		{
			p.x = x;
			for(unsigned y=1; y<=BOARD_SIZE; y++)
			{
				p.y = y;
				e += board.getColor(p) * Table[x][y];
			}
		}
		
		return board.getCurrentColor() * e;
	}
};


#endif

