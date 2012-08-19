#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include "Reversi.h"
#include <vector>


class Board
{

public:
	Board();
	
	void init();
	bool move(const Point& point);
	bool pass();
	bool undo();
	bool isGameOver() const;

	
	unsigned countDisc(Color color) const
	{
		return Discs[color];
	}

	//pointを指定して色を取得
	Color getColor(const Point& p) const
	{
		return RawBoard[p.x][p.y];
	}

	//ターンを指定して
	const std::vector<Point>& getMovablePos() const
	{
		return MovablePos[Turns];
	}

	//現在の手番の色を返す
	Color getCurrentColor() const
	{
		return CurrentColor;
	}


private:

	//方向を表す定数
	static const unsigned NONE        =   0;
	static const unsigned UPPER       =   1;
	static const unsigned UPPER_LEFT  =   2;
	static const unsigned LEFT        =   4;
	static const unsigned LOWER_LEFT  =   8;
	static const unsigned LOWER       =  16;
	static const unsigned LOWER_RIGHT =  32;
	static const unsigned RIGHT       =  64;
	static const unsigned UPPER_RIGHT = 128;



	Color RawBoard[BOARD_SIZE+2][BOARD_SIZE+2];//盤のマスを表す。両端の壁を含むので+2
	unsigned Turns;
	Color CurrentColor; //現在の手番（黒or白）


	unsigned Board::checkMobility(const Disc& disc) const;

	//変更点を格納するコレクション
	//C#では、たとえば 以下ようになるだろう
	//	DiscList : List<Disc>
	//	List<DiscList> UpdateLog
	std::vector<std::vector<Disc>> UpdateLog;


	//checkMobilityの結果格納用。全ての目（Point）において置ける方向を保持する。ターンが終わればクリアされ再計算する
	//C#では、たとえば 以下ようになるだろう
	//	unsigned MovableDir_XY[BOARD_SIZE+2][BOARD_SIZE+2];
	//	List<MovableDir_XY> MovableDir
	unsigned MovableDir[MAX_TURNS+1][BOARD_SIZE+2][BOARD_SIZE+2];


	//打てる位置の座標群
	//C#では、たとえば 以下ようになるだろう
	//	PointList: List<Point>
	//	PointList MovablePos[MAX_TURNS+1];
	//  または、List<PointList> MovablePos;
	std::vector<Point> MovablePos[MAX_TURNS+1];

	//それぞれの石の色数を保持するオブジェクト
	ColorStorage<unsigned> Discs; //ex Discs[BLACK] = 2;, Discs[WHITE] = 2;

	//pointで指定された位置に石を打つ。
	void flipDiscs(const Point& point);


	//MovableDirとMovablePosを再計算する
	void initMovable();

};

#endif
