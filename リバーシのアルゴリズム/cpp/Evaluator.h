#ifndef EVALUATOR_H_INCLUDED
#define EVALUATOR_H_INCLUDED

#include "Board.h"

class Evaluator
{
public:
	virtual int evaluate(const Board& board) = 0;
};

class WLDEvaluator : public Evaluator
{
public:
	static const int WIN  =  1;
	static const int DRAW =  0;
	static const int LOSE = -1;

	int evaluate(const Board& board)
	{
		int discdiff
			= board.getCurrentColor()
			* (board.countDisc(BLACK) - board.countDisc(WHITE));
		
		if(discdiff > 0) return WIN;
		else if(discdiff < 0) return LOSE;
		else return DRAW;
	}
};

class PerfectEvaluator : public Evaluator
{
public:
	int evaluate(const Board& board)
	{
		int discdiff
			= board.getCurrentColor()
			* (board.countDisc(BLACK) - board.countDisc(WHITE));
		
		return discdiff;
	}
};

#endif