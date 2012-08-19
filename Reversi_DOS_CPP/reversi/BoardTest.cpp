#include "Board.h"

#include <iostream>

#include <cstdlib>   //rand()���g�����߂ɕK�v

using namespace std;



//DOS�p�{�[�h�N���X
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
					cout << "��";
					break;
				case WHITE:
					cout << "��";
					break;
				default:
					cout << "�@";
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
		cout << "����" << board.countDisc(BLACK) << " ";
		cout << "����" << board.countDisc(WHITE) << " ";
		cout << "��}�X" << board.countDisc(EMPTY) << endl;
		
		//cout << board.getCurrentColor() == BLACK ? "����� " : "����� ";
		

		vector<Point> movables = board.getMovablePos();

		
		if (board.getCurrentColor() == WHITE) {
			//�����𓾂�
			int max = movables.size(); int po =  rand() % max ;
			Point p = movables[po];
			board.move(p);
			cout << "PC�v�l��: ";
			cout << p << " ";

			continue;
		}


		//�Ղ�\��
		board.print();

		for(unsigned i=0; i<movables.size(); i++)
		{
			cout << movables[i] << " ";
		}
		cout << endl << endl;


		cout << "�����͂��ĉ�����: ";
		Point p;
		
		string in;
		cin >> in;
		
		if(in == "p")
		{
			// �p�X
			if(!board.pass()) cerr << "�p�X�ł��܂���I" << endl;
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
			cerr << "���o�[�V�`���̎����͂��ĉ������I" << endl;
			continue;
		}
		
		if(board.move(p) == false)
		{
			cerr << "�����ɂ͒u���܂���I" << endl;
			continue;
		}

		if(board.isGameOver())
		{
			cout << "----------------�Q�[���I��----------------" << endl;
			return 0;
		}
	}

	return 0;
}


