#include <iostream>
#include <string>
#include <memory>

#include "Board.h"
#include "AI.h"

using namespace std;

class Player
{
public:
	virtual void onTurn(Board& board) = 0;
};

class UndoException
{
};

class ExitException
{
};

class GameOverException
{
};

class HumanPlayer : public Player
{
public:
	void onTurn(Board& board)
	{
		if(board.getMovablePos().empty())
		{
			// �p�X
			cout << "���Ȃ��̓p�X�ł��B" << endl;
			board.pass();
			return;
		}

		while(true)
		{
			cout << "���\"f5\"�̂悤�ɓ��́A��������(U:���/X:�I��)����͂��Ă�������:";
			string in;
			cin >> in;
			
			if(in == "U" || in == "u") throw UndoException();
			
			if(in == "X" || in == "x") throw ExitException();
			
			Point p;
			try{
				p = Point(in);
			}
			catch(invalid_argument& e)
			{
				cout << "�������`���œ��͂��Ă��������I" << endl;
				continue;
			}
			
			if(!board.move(p))
			{
				cout << "�����ɂ͒u���܂���I" << endl;
				continue;
			}
			
			if(board.isGameOver()) throw GameOverException();
			
			break;
		}
	}
};

class AIPlayer : public Player
{

	AI* Ai;

public:

	AIPlayer() : Ai(NULL)
	{
		Ai = new AlphaBetaAI();
	}
	
	~AIPlayer()
	{
		delete Ai;
	}

	void onTurn(Board& board)
	{
		cout << "�R���s���[�^���v�l��...";
		Ai->move(board);
		cout << "����" << endl;
		if(board.isGameOver()) throw GameOverException();
	}
};

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


int main(int argc, char* argv[])
{
	auto_ptr<Player> player[2];
	int current_player = 0;
	ConsoleBoard board;
	bool reverse = false;

	if(argc > 1)
	{
		// �R�}���h���C���I�v�V���� -r ���^������ƃR���s���[�^���ɂ���
		if(strcmp(argv[1], "-r") == 0) reverse = true;
	}

	// ���E���̐ݒ�
	if(reverse)
	{
		player[1].reset(new HumanPlayer());
		player[0].reset(new AIPlayer());
	}
	else
	{
		player[0].reset(new HumanPlayer());
		player[1].reset(new AIPlayer());
	}

	while(true)
	{
		board.print();

		try{
			player[current_player]->onTurn(board);
		}
		catch(UndoException& e)
		{
			do
			{
				board.undo(); board.undo();
			} while(board.getMovablePos().empty());
			continue;
		}
		catch(ExitException& e)
		{
			return 0;
		}
		catch(GameOverException& e)
		{
			cout << "�Q�[���I��" << endl;
			cout << "����" << board.countDisc(BLACK) << " ";
			cout << "����" << board.countDisc(WHITE) << endl;

			return 0;
		}

		// �v���C���[�̌��
		current_player = ++current_player % 2;
	}

}

