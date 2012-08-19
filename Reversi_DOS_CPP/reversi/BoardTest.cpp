#include "Board.h"

#include <iostream>

#include <cstdlib>   //rand()を使うために必要

using namespace std;



//DOS用ボードクラス
class ConsoleBoard : public Board
{
public:
	void print()
	{
		cout << "  a b c d e f g h " << endl;
		for(int y=1; y<=8; y++)
		{
			cout << " " << y;
			for(int x=1; x<=8; x++)
			{
				switch(getColor(Point(x, y)))
				{
				case BLACK:
					cout << "●";
					break;
				case WHITE:
					cout << "○";
					break;
				default:
					cout << "　";
					break;
				}
			}
			cout << endl;
		}
	}
};



ostream& operator<<(ostream& os, const Point& p)
{
	string s = p;
	os << s;
	return os;
}


int main()
{
	ConsoleBoard board;

	while(true)
	{
		cout << "黒石" << board.countDisc(BLACK) << " ";
		cout << "白石" << board.countDisc(WHITE) << " ";
		cout << "空マス" << board.countDisc(EMPTY) << endl;
		
		//cout << board.getCurrentColor() == BLACK ? "黒手番 " : "白手番 ";
		

		vector<Point> movables = board.getMovablePos();

		
		if (board.getCurrentColor() == WHITE) {
			//乱数を得る
			int max = movables.size(); int po =  rand() % max ;
			Point p = movables[po];
			board.move(p);
			cout << "PC思考中: ";
			cout << p << " ";

			continue;
		}


		//盤を表示
		board.print();

		for(unsigned i=0; i<movables.size(); i++)
		{
			cout << movables[i] << " ";
		}
		cout << endl << endl;


		cout << "手を入力して下さい: ";
		Point p;
		
		string in;
		cin >> in;
		
		if(in == "p")
		{
			// パス
			if(!board.pass()) cerr << "パスできません！" << endl;
			continue;
		}
		
		if(in == "u")
		{
			// undo
			board.undo();
			board.undo();
			continue;
		}

		try
		{
			Point parse(in);
			p = parse;
		}catch(invalid_argument e){
			cerr << "リバーシ形式の手を入力して下さい！" << endl;
			continue;
		}
		
		if(board.move(p) == false)
		{
			cerr << "そこには置けません！" << endl;
			continue;
		}

		if(board.isGameOver())
		{
			cout << "----------------ゲーム終了----------------" << endl;
			return 0;
		}
	}

	return 0;
}


