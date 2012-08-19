#ifndef MIDEVALUATOR_H_INCLUDED
#define MIDEVALUATOR_H_INCLUDED

#include "Evaluator.h"

class MidEvaluator : public Evaluator
{
private:

	struct EdgeParam
	{
		EdgeParam &operator +=(const EdgeParam &src)
		{
			stable += src.stable;
			wing += src.wing;
			mountain += src.mountain;
			Cmove += src.Cmove;

			return *this;
		}

		short stable; // 確定石の個数
		short wing; // ウイングの個数
		short mountain; // 山の個数
		short Cmove; // 危険なC打ちの個数
	};

	struct CornerParam
	{
		short corner; // 隅にある石の数
		short Xmove;  // 危険なX打ちの個数
	};

	// 重み係数を規定する構造体
	struct Weight
	{
		int mobility_w;
		int liberty_w;
		int stable_w;
		int wing_w;
		int Xmove_w;
		int Cmove_w;
	} EvalWeight;
	
	typedef ColorStorage<EdgeParam> EdgeStat;
	typedef ColorStorage<CornerParam> CornerStat;
	
	static const unsigned TABLE_SIZE = 6561; // 3^8
	static EdgeStat EdgeTable[TABLE_SIZE];
	static bool TableInit;

	void generateEdge(Color line[], unsigned index);
	EdgeParam evalEdge(Color line[], Color color) const;
	CornerStat evalCorner(const Board &) const;

	inline unsigned idxTop(const Board &) const;
	inline unsigned idxBottom(const Board &) const;
	inline unsigned idxRight(const Board &) const;
	inline unsigned idxLeft(const Board &) const;
	
	ColorStorage<unsigned> countLiberty(const Board& board) const;

	unsigned idxLine(Color l[])
	{
		return 3*(3*(3*(3*(3*(3*(3*(l[0]+1)+l[1]+1)+l[2]+1)+l[3]+1)+l[4]+1)+l[5]+1)+l[6]+1)+l[7]+1;
	}

public:

	MidEvaluator();
	int evaluate(const Board& board);

};


#endif // MIDEVALUATOR_H_INCLUDED
