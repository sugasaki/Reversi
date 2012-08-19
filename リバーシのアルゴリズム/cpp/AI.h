#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

#include "Board.h"
#include <functional>

class Evaluator;

class AI
{
public:

	AI() : presearch_depth(3), normal_depth(5), wld_depth(15), perfect_depth(13),
		Eval(NULL), nodes(0), leaves(0)
	{
	}

	unsigned presearch_depth;
	unsigned normal_depth;
	unsigned wld_depth;
	unsigned perfect_depth;

	unsigned nodes;
	unsigned leaves;

	virtual void move(Board&)=0;
protected:

	Evaluator* Eval;

	struct Move : public Point
	{
		int eval;
		Move() : Point(0, 0), eval(0) {}
		Move(int x, int y, int e) : Point(x, y), eval(e) {}
	};
	
	struct MoveGreater
	{
		bool operator() (const Move& lhs, const Move& rhs)
		{
			return (lhs.eval > rhs.eval);
		}
	};

private:
	// コピー・代入は許可しない
	AI(const AI&){}
	AI& operator=(const AI&){return *this;}

};

class MinimaxAI : public AI
{
public:
	MinimaxAI();
	void move(Board& board);
private:
	int negaMax(Board& board, int limit);
};

class AlphaBetaAI : public AI
{
public:
	AlphaBetaAI();
	void move(Board& board);
private:
	void sort(Board& board, std::vector<Point>&, int limit);
	int alphabeta(Board& board, int limit, int alpha, int beta);
};

class NegaScoutAI : public AI
{
public:
	NegaScoutAI();
	void move(Board& board);
private:
	void sort(Board& board, std::vector<Point>&, int limit);
	int negaScout(Board& board, int limit, int alpha, int beta);
};

#endif
