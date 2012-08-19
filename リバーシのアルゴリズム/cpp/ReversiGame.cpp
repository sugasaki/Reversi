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
			// パス
			cout << "あなたはパスです。" << endl;
			board.pass();
			return;
		}

		while(true)
		{
			cout << "手を\"f5\"のように入力、もしくは(U:取消/X:終了)を入力してください:";
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
				cout << "正しい形式で入力してください！" << endl;
				continue;
			}
			
			if(!board.move(p))
			{
				cout << "そこには置けません！" << endl;
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
		cout << "コンピュータが思考中...";
		Ai->move(board);
		cout << "完了" << endl;
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


int main(int argc, char* argv[])
{
	auto_ptr<Player> player[2];
	int current_player = 0;
	ConsoleBoard board;
	bool reverse = false;

	if(argc > 1)
	{
		// コマンドラインオプション -r が与えられるとコンピュータ先手にする
		if(strcmp(argv[1], "-r") == 0) reverse = true;
	}

	// 先手・後手の設定
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
			cout << "ゲーム終了" << endl;
			cout << "黒石" << board.countDisc(BLACK) << " ";
			cout << "白石" << board.countDisc(WHITE) << endl;

			return 0;
		}

		// プレイヤーの交代
		current_player = ++current_player % 2;
	}

}

