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

	//point���w�肵�ĐF���擾
	Color getColor(const Point& p) const
	{
		return RawBoard[p.x][p.y];
	}

	//�^�[�����w�肵��
	const std::vector<Point>& getMovablePos() const
	{
		return MovablePos[Turns];
	}

	//���݂̎�Ԃ̐F��Ԃ�
	Color getCurrentColor() const
	{
		return CurrentColor;
	}


private:

	//������\���萔
	static const unsigned NONE        =   0;
	static const unsigned UPPER       =   1;
	static const unsigned UPPER_LEFT  =   2;
	static const unsigned LEFT        =   4;
	static const unsigned LOWER_LEFT  =   8;
	static const unsigned LOWER       =  16;
	static const unsigned LOWER_RIGHT =  32;
	static const unsigned RIGHT       =  64;
	static const unsigned UPPER_RIGHT = 128;



	Color RawBoard[BOARD_SIZE+2][BOARD_SIZE+2];//�Ղ̃}�X��\���B���[�̕ǂ��܂ނ̂�+2
	unsigned Turns;
	Color CurrentColor; //���݂̎�ԁi��or���j


	unsigned Board::checkMobility(const Disc& disc) const;

	//�ύX�_���i�[����R���N�V����
	//C#�ł́A���Ƃ��� �ȉ��悤�ɂȂ邾�낤
	//	DiscList : List<Disc>
	//	List<DiscList> UpdateLog
	std::vector<std::vector<Disc>> UpdateLog;


	//checkMobility�̌��ʊi�[�p�B�S�Ă̖ځiPoint�j�ɂ����Ēu���������ێ�����B�^�[�����I���΃N���A����Čv�Z����
	//C#�ł́A���Ƃ��� �ȉ��悤�ɂȂ邾�낤
	//	unsigned MovableDir_XY[BOARD_SIZE+2][BOARD_SIZE+2];
	//	List<MovableDir_XY> MovableDir
	unsigned MovableDir[MAX_TURNS+1][BOARD_SIZE+2][BOARD_SIZE+2];


	//�łĂ�ʒu�̍��W�Q
	//C#�ł́A���Ƃ��� �ȉ��悤�ɂȂ邾�낤
	//	PointList: List<Point>
	//	PointList MovablePos[MAX_TURNS+1];
	//  �܂��́AList<PointList> MovablePos;
	std::vector<Point> MovablePos[MAX_TURNS+1];

	//���ꂼ��̐΂̐F����ێ�����I�u�W�F�N�g
	ColorStorage<unsigned> Discs; //ex Discs[BLACK] = 2;, Discs[WHITE] = 2;

	//point�Ŏw�肳�ꂽ�ʒu�ɐ΂�łB
	void flipDiscs(const Point& point);


	//MovableDir��MovablePos���Čv�Z����
	void initMovable();

};

#endif
