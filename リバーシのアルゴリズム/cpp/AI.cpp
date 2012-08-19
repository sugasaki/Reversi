
#include "AI.h"
#include "Board.h"
#include "MidEvaluator.h"
#include "BookManager.h"

#include <limits>
#include <algorithm>

using namespace std;

MinimaxAI::MinimaxAI() : AI()
{
}

void MinimaxAI::move(Board& board)
{
	BookManager book;
	vector<Point> movables = book.find(board);
	
	if(movables.empty())
	{
		// 打てる箇所がなければパスする
		board.pass();
		return;
	}
	
	if(movables.size() == 1)
	{
		// 打てる箇所が一カ所だけなら探索は行わず、即座に打って返る
		board.move(movables[0]);
		return;
	}
	
	int limit;
	Eval = new MidEvaluator();

	if(MAX_TURNS - board.getTurns() <= wld_depth)
	{
		delete Eval;
		limit = numeric_limits<int>::max();
		if(MAX_TURNS - board.getTurns() <= perfect_depth) Eval = new PerfectEvaluator();
		else Eval = new WLDEvaluator();
	}
	else
	{
		limit = normal_depth;
	}

	int eval, eval_max = numeric_limits<int>::min();
	Point p;
	for(unsigned i=0; i<movables.size(); i++)
	{
		board.move(movables[i]);
		eval = -negaMax(board, limit-1);
		board.undo();

		if(eval > eval_max) p = movables[i];
	}
	
	delete Eval;
	Eval = NULL;
	
	board.move(p);
}

int MinimaxAI::negaMax(Board& board, int limit)
{
	if(board.isGameOver() || limit == 0)
	{
		leaves++;
		return Eval->evaluate(board);
	}

	nodes++;

	const std::vector<Point>& movables = board.getMovablePos();
	int score, score_max = std::numeric_limits<int>::min();

	if(movables.empty())
	{
		board.pass();
		score = -negaMax(board, limit);
		board.undo();
		return score;
	}

	for(unsigned i=0; i<movables.size(); i++)
	{
		board.move(movables[i]);
		score = -negaMax(board, limit-1); // 次の相手の手
		board.undo();

		if(score > score_max)
		{
			// よりよい手が見つかった
			score_max = score;
		}
	}

	return score_max;

}

AlphaBetaAI::AlphaBetaAI() : AI()
{
}

void AlphaBetaAI::move(Board& board)
{
	BookManager book;
	vector<Point> movables = book.find(board);
	
	if(movables.empty())
	{
		// 打てる箇所がなければパスする
		board.pass();
		return;
	}
	
	if(movables.size() == 1)
	{
		// 打てる箇所が一カ所だけなら探索は行わず、即座に打って返る
		board.move(movables[0]);
		return;
	}
	
	int limit;
	Eval = new MidEvaluator();
	sort(board, movables, presearch_depth); // 事前に手を良さそうな順にソート

	if(MAX_TURNS - board.getTurns() <= wld_depth)
	{
		delete Eval;
		limit = numeric_limits<int>::max();
		if(MAX_TURNS - board.getTurns() <= perfect_depth) Eval = new PerfectEvaluator();
		else Eval = new WLDEvaluator();
	}
	else
	{
		limit = normal_depth;
	}

	int eval, alpha = numeric_limits<int>::min();
	int beta = numeric_limits<int>::max();
	Point p;
	for(unsigned i=0; i<movables.size(); i++)
	{
		board.move(movables[i]);
		eval = -alphabeta(board, limit-1, -beta, -alpha);
		board.undo();

		if(eval > alpha)
		{
			alpha = eval;
			p = movables[i];
		}
	}
	
	delete Eval;
	Eval = NULL;
	
	board.move(p);
}

void AlphaBetaAI::sort(Board& board, std::vector<Point>& movables, int limit)
{
	std::vector<Move> moves;
	
	for(unsigned i=0; i<movables.size(); i++)
	{
		int eval;

		board.move(movables[i]);
		eval = -alphabeta(board, limit-1, -INT_MAX, INT_MAX);
		board.undo();

		Move move(movables[i].x, movables[i].y, eval);
		moves.push_back(move);
	}
	
	// 評価値の大きい順にソート
	std::stable_sort(moves.begin(), moves.end(), MoveGreater());
	
	// 結果の書き戻し
	
	movables.clear();
	for(unsigned i=0; i<moves.size(); i++)
	{
		movables.push_back(moves[i]);
	}
	
	return;
}

int AlphaBetaAI::alphabeta(Board& board, int limit, int alpha, int beta)
{
	if(board.isGameOver() || limit == 0)
	{
		leaves++;
		return Eval->evaluate(board);
	}

	nodes++;

	int score;
	const std::vector<Point>& movables = board.getMovablePos();
	
	if(movables.empty())
	{
		board.pass();
		score = -alphabeta(board, limit, -beta, -alpha);
		board.undo();
		return score;
	}
	
	for(unsigned i=0; i<movables.size(); i++)
	{
		board.move(movables[i]);
		score = -alphabeta(board, limit-1, -beta, -alpha);
		board.undo();
		
		if(score >= beta) return score;

		alpha = std::max(alpha, score);

	}
	
	return alpha;
}


NegaScoutAI::NegaScoutAI() : AI()
{
}

void NegaScoutAI::move(Board& board)
{
	BookManager book;
	vector<Point> movables = book.find(board);
	
	if(movables.empty())
	{
		// 打てる箇所がなければパスする
		board.pass();
		return;
	}
	
	if(movables.size() == 1)
	{
		// 打てる箇所が一カ所だけなら探索は行わず、即座に打って返る
		board.move(movables[0]);
		return;
	}
	
	int limit;
	Eval = new MidEvaluator();
	sort(board, movables, presearch_depth); // 事前に手を良さそうな順にソート

	if(MAX_TURNS - board.getTurns() <= wld_depth)
	{
		delete Eval;
		limit = numeric_limits<int>::max();
		if(MAX_TURNS - board.getTurns() <= perfect_depth) Eval = new PerfectEvaluator();
		else Eval = new WLDEvaluator();
	}
	else
	{
		limit = normal_depth;
	}

	int eval, alpha = numeric_limits<int>::min();
	int beta = numeric_limits<int>::max();
	Point p;
	for(unsigned i=0; i<movables.size(); i++)
	{
		board.move(movables[i]);
		eval = -negaScout(board, limit-1, -beta, -alpha);
		board.undo();

		if(eval > alpha)
		{
			alpha = eval;
			p = movables[i];
		}
	}
	
	delete Eval;
	Eval = NULL;
	
	board.move(p);
}

void NegaScoutAI::sort(Board& board, std::vector<Point>& movables, int limit)
{
	std::vector<Move> moves;
	
	for(unsigned i=0; i<movables.size(); i++)
	{
		int eval;

		board.move(movables[i]);
		eval = -negaScout(board, limit-1, -INT_MAX, INT_MAX);
		board.undo();

		Move move(movables[i].x, movables[i].y, eval);
		moves.push_back(move);
	}
	
	// 評価値の大きい順にソート
	std::stable_sort(moves.begin(), moves.end(), MoveGreater());
	
	// 結果の書き戻し
	
	movables.clear();
	for(unsigned i=0; i<moves.size(); i++)
	{
		movables.push_back(moves[i]);
	}
	
	return;
}

int NegaScoutAI::negaScout(Board& board, int limit, int alpha, int beta)
{
	// 深さ制限に達したら評価値を返す
	if(board.isGameOver() || limit == 0)
	{
		leaves++;
		return Eval->evaluate(board);
	}

	nodes++;

	int a,b, t;
	a = alpha;
	b = beta;
	const std::vector<Point>& pos = board.getMovablePos();

	if(pos.empty())
	{
		// パス
		board.pass();
		t = -negaScout(board, limit, -beta, -alpha);
		board.undo();
		return t;
	}

	for(unsigned i=0; i< pos.size(); i++)
	{
		board.move(pos[i]);
		t = -negaScout(board, limit-1, -b, -a);
		if(t > a && t < beta && i>0 && limit <= 2)
		{
			// 再探索
			a = - negaScout(board, limit-1, -beta, -t);
		}
		board.undo();

		a = max(a, t);

		if(a >= beta)
		{
			// β刈り
			return a;
		}

		b = a+1; // 新しいnull windowを設定
	}

	return a;
}
