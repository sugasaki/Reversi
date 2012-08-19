#include "MidEvaluator.h"

bool MidEvaluator::TableInit = false;
MidEvaluator::EdgeStat MidEvaluator::EdgeTable[TABLE_SIZE];


MidEvaluator::MidEvaluator()
{
	if(!TableInit)
	{
		//
		//	初回起動時にテーブルを生成
		//

		Color line[BOARD_SIZE];
		generateEdge(line, 0);

		TableInit = true;
	}

	// 重み係数の設定 (全局面共通)

	EvalWeight.mobility_w = 67;
	EvalWeight.liberty_w  = -13;
	EvalWeight.stable_w   = 101;
	EvalWeight.wing_w     = -308;
	EvalWeight.Xmove_w    = -449;
	EvalWeight.Cmove_w    = -552;
}

int MidEvaluator::evaluate(const Board& board)
{
	EdgeStat edgestat;
	CornerStat cornerstat;
	int result;

	//
	//	辺の評価
	//

	edgestat  = EdgeTable[idxTop(board)];
	edgestat += EdgeTable[idxBottom(board)];
	edgestat += EdgeTable[idxRight(board)];
	edgestat += EdgeTable[idxLeft(board)];

	//
	//	隅の評価
	//

	cornerstat = evalCorner(board);

	// 確定石に関して、隅の石を2回数えてしまっているので補正。

	edgestat[BLACK].stable -= cornerstat[BLACK].corner;
	edgestat[WHITE].stable -= cornerstat[WHITE].corner;

	//
	//	パラメータの線形結合
	//

	result =
		  edgestat[BLACK].stable * EvalWeight.stable_w
		- edgestat[WHITE].stable * EvalWeight.stable_w
		+ edgestat[BLACK].wing * EvalWeight.wing_w
		- edgestat[WHITE].wing * EvalWeight.wing_w
		+ cornerstat[BLACK].Xmove * EvalWeight.Xmove_w
		- cornerstat[WHITE].Xmove * EvalWeight.Xmove_w
		+ edgestat[BLACK].Cmove * EvalWeight.Cmove_w
		- edgestat[WHITE].Cmove * EvalWeight.Cmove_w
		;

	// 開放度・着手可能手数の評価

	if(EvalWeight.liberty_w != 0)
	{
		ColorStorage<unsigned> liberty = countLiberty(board);
		result += liberty[BLACK] * EvalWeight.liberty_w;
		result -= liberty[WHITE] * EvalWeight.liberty_w;
	}

	// 現在の手番の色についてのみ、着手可能手数を数える
	result +=
		  board.getCurrentColor()
		* board.getMovablePos().size()
		* EvalWeight.mobility_w;

	return board.getCurrentColor() * result;

}

void MidEvaluator::generateEdge(Color edge[], unsigned count)
{
	if(count == BOARD_SIZE)
	{
		//
		//	このパターンは完成したので、局面のカウント
		//

		EdgeStat stat;

		stat[BLACK] = evalEdge(edge, BLACK);
		stat[WHITE] = evalEdge(edge, WHITE);

		EdgeTable[idxLine(edge)] = stat;

		return;
	}

	// 再帰的に全てのパターンを網羅

	edge[count] = EMPTY;
	generateEdge(edge, count +1);

	edge[count] = BLACK;
	generateEdge(edge, count +1);

	edge[count] = WHITE;
	generateEdge(edge, count +1);

	return ;

}

//
//	evalEdge
//	辺のパラメータを数える
//

MidEvaluator::EdgeParam MidEvaluator::evalEdge(Color line[], Color color) const
{
	EdgeParam edgeparam = {0, 0, 0, 0};

	unsigned x;

	//
	//	ウィング等のカウント
	//

	if(line[0] == EMPTY && line[7] == EMPTY)
	{
		x = 2;
		while(x <= 5)
		{
			if(line[x] != color) break;
			x++;
		}
		if(x == 6) // 少なくともブロックができている
		{
			if(line[1] == color && line[6] == EMPTY)
				edgeparam.wing = 1;
			else if(line[1] == EMPTY && line[6] == color)
				edgeparam.wing = 1;
			else if(line[1] == color && line[6] == color)
				edgeparam.mountain = 1;
		}
		else // それ以外の場合に、隅に隣接する位置に置いていたら
		{
			if(line[1] == color)
				edgeparam.Cmove++;
			if(line[6] == color)
				edgeparam.Cmove++;
		}
	}

	//
	//	確定石のカウント
	//

	// 左から右方向に走査
	for(x = 0; x < 8; x++)
	{
		if(line[x] != color) break;
		edgeparam.stable++;
	}

	if(edgeparam.stable < 8)
	{
		// 右側からの走査も必要
		for(x = 7; x > 0; x--)
		{
			if(line[x] != color) break;
			edgeparam.stable++;
		}
	}


	return edgeparam;

}

//
//	evalCorner
//	隅のパラメータを調べる。この関数は各評価時に使う。
//

MidEvaluator::CornerStat MidEvaluator::evalCorner(const Board &board) const
{
	CornerStat cornerstat;

	cornerstat[BLACK].corner=0; cornerstat[BLACK].Xmove=0;
	cornerstat[WHITE].corner=0; cornerstat[WHITE].Xmove=0;

	//	左上
	cornerstat[board.getColor(Point(1, 1))].corner++;
	if(board.getColor(Point(1, 1)) == EMPTY) cornerstat[board.getColor(Point(2, 2))].Xmove++;

	//	左下
	cornerstat[board.getColor(Point(1, 8))].corner++;
	if(board.getColor(Point(1, 8)) == EMPTY) cornerstat[board.getColor(Point(2, 7))].Xmove++;

	//	右下
	cornerstat[board.getColor(Point(8, 7))].corner++;
	if(board.getColor(Point(8, 8)) == EMPTY) cornerstat[board.getColor(Point(7, 7))].Xmove++;

	//	右上
	cornerstat[board.getColor(Point(8, 1))].corner++;
	if(board.getColor(Point(8, 1)) == EMPTY) cornerstat[board.getColor(Point(7, 2))].Xmove++;

	return cornerstat;

}

ColorStorage<unsigned> MidEvaluator::countLiberty(const Board &board) const
{
	ColorStorage<unsigned> liberty;

	liberty[BLACK] = 0; liberty[WHITE] = 0; liberty[EMPTY] = 0;

	for(unsigned int x = 1; x <= BOARD_SIZE; x++)
	{
		for(unsigned int y = 1; y <= BOARD_SIZE; y++)
		{
			Point p(x, y);
			liberty[board.getColor(p)] += board.getLiberty(p);
		}
	}

	return liberty;

}


////////////////////////////////////////////////////////////////////////
//
//	各箇所についてのインデックスの計算
//	パフォーマンス向上のため個別のインライン関数でベタに処理
//

unsigned MidEvaluator::idxTop(const Board &board) const
{
	unsigned index;

	index =
		2187 * (board.getColor(Point(1, 1)) + 1)
		+729 * (board.getColor(Point(2, 1)) + 1)
		+243 * (board.getColor(Point(3, 1)) + 1)
		+ 81 * (board.getColor(Point(4, 1)) + 1)
		+ 27 * (board.getColor(Point(5, 1)) + 1)
		+  9 * (board.getColor(Point(6, 1)) + 1)
		+  3 * (board.getColor(Point(7, 1)) + 1)
		+  1 * (board.getColor(Point(8, 1)) + 1);

	return index;
}

unsigned MidEvaluator::idxBottom(const Board &board) const
{
	unsigned index;

	index =
		2187 * (board.getColor(Point(1, 8)) + 1)
		+729 * (board.getColor(Point(2, 8)) + 1)
		+243 * (board.getColor(Point(3, 8)) + 1)
		+ 81 * (board.getColor(Point(4, 8)) + 1)
		+ 27 * (board.getColor(Point(5, 8)) + 1)
		+  9 * (board.getColor(Point(6, 8)) + 1)
		+  3 * (board.getColor(Point(7, 8)) + 1)
		+  1 * (board.getColor(Point(8, 8)) + 1);

	return index;

}

unsigned MidEvaluator::idxRight(const Board &board) const
{
	unsigned index;

	index =
		2187 * (board.getColor(Point(8, 1)) + 1)
		+729 * (board.getColor(Point(8, 2)) + 1)
		+243 * (board.getColor(Point(8, 3)) + 1)
		+ 81 * (board.getColor(Point(8, 4)) + 1)
		+ 27 * (board.getColor(Point(8, 5)) + 1)
		+  9 * (board.getColor(Point(8, 6)) + 1)
		+  3 * (board.getColor(Point(8, 7)) + 1)
		+  1 * (board.getColor(Point(8, 8)) + 1);

	return index;

}

unsigned MidEvaluator::idxLeft(const Board &board) const
{
	unsigned index;

	index =
		2187 * (board.getColor(Point(1, 1)) + 1)
		+729 * (board.getColor(Point(1, 2)) + 1)
		+243 * (board.getColor(Point(1, 3)) + 1)
		+ 81 * (board.getColor(Point(1, 4)) + 1)
		+ 27 * (board.getColor(Point(1, 5)) + 1)
		+  9 * (board.getColor(Point(1, 6)) + 1)
		+  3 * (board.getColor(Point(1, 7)) + 1)
		+  1 * (board.getColor(Point(1, 8)) + 1);

	return index;

}
