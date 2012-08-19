#include "Board.h"
#include "Reversi.h"


Board::Board()
{
	init();
}


//初期化
void Board::init()
{
	// 全マスを空きマスに設定
	for(int x=1; x <= BOARD_SIZE; x++)
	{
		for(int y=1; y <= BOARD_SIZE; y++)
		{
			RawBoard[x][y] = EMPTY;
		}
	}
	
	// 壁の設定（縦）
	for(int y=0; y < BOARD_SIZE + 2; y++)
	{
		RawBoard[0][y] = WALL;
		RawBoard[BOARD_SIZE+1][y] = WALL;
	}

	// 壁の設定（横）
	for(int x=0; x < BOARD_SIZE + 2; x++)
	{
		RawBoard[x][0] = WALL;
		RawBoard[x][BOARD_SIZE+1] = WALL;
	}


	// 初期配置
	RawBoard[4][4] = WHITE;
	RawBoard[5][5] = WHITE;
	RawBoard[4][5] = BLACK;
	RawBoard[5][4] = BLACK;
	

	// 石数の初期設定
	Discs[BLACK] = 2;
	Discs[WHITE] = 2;
	Discs[EMPTY] = BOARD_SIZE*BOARD_SIZE - 4;

	Turns = 0; // 手数は0から数える
	CurrentColor = BLACK; // 先手は黒
	
	// updateを全て消去
	UpdateLog.clear();

	//
	initMovable();
}



//石が置けるかどうか判定する
unsigned Board::checkMobility(const Disc& disc) const
{
	// 既に石があったら置けない
	if(RawBoard[disc.x][disc.y] != EMPTY) return NONE;

	int x, y;
	unsigned dir = NONE;

	// 上
	if(RawBoard[disc.x][disc.y-1] == -disc.color)
	{
		x = disc.x; y = disc.y-2;
		while(RawBoard[x][y] == -disc.color) { y--; }
		if(RawBoard[x][y] == disc.color) dir |= UPPER;
	}

	// 下
	if(RawBoard[disc.x][disc.y+1] == -disc.color)
	{
		x = disc.x; y = disc.y+2;
		while(RawBoard[x][y] == -disc.color) { y++; }
		if(RawBoard[x][y] == disc.color) dir |= LOWER;
	}

	// 左
	if(RawBoard[disc.x-1][disc.y] == -disc.color)
	{
		x = disc.x-2; y = disc.y;
		while(RawBoard[x][y] == -disc.color) { x--; }
		if(RawBoard[x][y] == disc.color) dir |= LEFT;
	}

	// 右
	if(RawBoard[disc.x+1][disc.y] == -disc.color)
	{
		x = disc.x+2; y = disc.y;
		while(RawBoard[x][y] == -disc.color) { x++; }
		if(RawBoard[x][y] == disc.color) dir |= RIGHT;
	}


	// 右上
	if(RawBoard[disc.x+1][disc.y-1] == -disc.color)
	{
		x = disc.x+2; y = disc.y-2;
		while(RawBoard[x][y] == -disc.color) { x++; y--; }
		if(RawBoard[x][y] == disc.color) dir |= UPPER_RIGHT;
	}

	// 左上
	if(RawBoard[disc.x-1][disc.y-1] == -disc.color)
	{
		x = disc.x-2; y = disc.y-2;
		while(RawBoard[x][y] == -disc.color) { x--; y--; }
		if(RawBoard[x][y] == disc.color) dir |= UPPER_LEFT;
	}

	// 左下
	if(RawBoard[disc.x-1][disc.y+1] == -disc.color)
	{
		x = disc.x-2; y = disc.y+2;
		while(RawBoard[x][y] == -disc.color) { x--; y++; }
		if(RawBoard[x][y] == disc.color) dir |= LOWER_LEFT;
	}

	// 右下
	if(RawBoard[disc.x+1][disc.y+1] == -disc.color)
	{
		x = disc.x+2; y = disc.y+2;
		while(RawBoard[x][y] == -disc.color) { x++; y++; }
		if(RawBoard[x][y] == disc.color) dir |= LOWER_RIGHT;
	}


	return dir;

}


//pointで指定された位置に石を打つ。
//成功したら return true
//及び、次のターンの計算結果を調査開始する。
bool Board::move(const Point& point)
{
	//石が打てる位置か判定する
	if(point.x <= 0 || point.x > BOARD_SIZE) return false;
	if(point.y <= 0 || point.y > BOARD_SIZE) return false;
	if(MovableDir[Turns][point.x][point.y] == NONE) return false; //checkMobilityの結果。既に石があるか、置けない場所を指定したなら falseを返す

	//石を打つ、変更点追跡、石裏返す、石数カウント、UpdateLog記録
	flipDiscs(point);

	//手数をインクリメント
	Turns++;
	CurrentColor = -CurrentColor;//白→黒　or　黒→白

	//MovableDirとMovablePosを調べ直す。（再計算）
	initMovable(); //現在の局面で、現在の番手(黒or白)が置ける場所を計算する

	return true;
}


//現在の局面で、現在の番手(黒or白)が置ける場所を計算する
//MovableDirとMovablePosを再計算する
void Board::initMovable()
{
	Disc disc(0, 0, CurrentColor);//構造体のインスタンスを現在色で作成 

	int dir;

	//ポジションクリア
	MovablePos[Turns].clear();

	//盤の全ての目において、置けるかどうかを調べる。
	for(int x=1; x<=BOARD_SIZE; x++)//x=0の時は壁なので1から開始する
	{
		disc.x = x;
		for(int y=1; y<=BOARD_SIZE; y++)
		{
			disc.y = y;
			
			//石を置ける場所か判定
			dir = checkMobility(disc); //discの位置に置けるなら、discから見て石が返る方向を表したビットを取得
			if(dir != NONE)
			{
				// 置ける
				MovablePos[Turns].push_back(disc); //末尾に格納（discのコピーが格納される？）
			}

			MovableDir[Turns][x][y] = dir; //置ける方向を表したビット（NONEでもNONE以外でも結果を格納）
		}
	}


}


//石を打つ、変更点追跡、石裏返す、石数カウント、UpdateLog記録
void Board::flipDiscs(const Point& point)
{
	int x, y;

	//pointから見て、石が反転する方向をビットで取得
	int dir = MovableDir[Turns][point.x][point.y];

	//今オペレーションの操作ログ記録用
	std::vector<Disc> update;

	//置いた場所＝pointを置いた色にする
	RawBoard[point.x][point.y] = CurrentColor;

	//ログ保存用構造体。（返った石の座標と色）
	Disc operation(point.x, point.y, CurrentColor); // 行った操作を表す石
	
	//最初に置いた石をログ記録
	update.push_back(operation);//構造体なので都度コピーがpushされる

	// 上
	if(dir & UPPER)
	{
		y = point.y;
		operation.x = point.x;
		while(RawBoard[point.x][--y] != CurrentColor)
		{
			RawBoard[point.x][y] = CurrentColor;
			operation.y = y;
			update.push_back(operation);
		}
	}

	// 下
	if(dir & LOWER)
	{
		y = point.y;
		operation.x = point.x;
		while(RawBoard[point.x][++y] == -CurrentColor)
		{
			RawBoard[point.x][y] = CurrentColor;
			operation.y = y;
			update.push_back(operation);
		}
	}

	// 左

	if(dir & LEFT)
	{
		x = point.x;
		operation.y = point.y;
		while(RawBoard[--x][point.y] == -CurrentColor)
		{
			RawBoard[x][point.y] = CurrentColor;
			operation.x = x;
			update.push_back(operation);
		}
	}

	// 右
	if(dir & RIGHT)
	{
		x = point.x;
		operation.y = point.y;
		while(RawBoard[++x][point.y] == -CurrentColor)
		{
			RawBoard[x][point.y] = CurrentColor;
			operation.x = x;
			update.push_back(operation);
		}
	}

	// 右上
	if(dir & UPPER_RIGHT)
	{
		x = point.x;
		y = point.y;
		while(RawBoard[++x][--y] == -CurrentColor)
		{
			RawBoard[x][y] = CurrentColor;
			operation.x = x;
			operation.y = y;
			update.push_back(operation);
		}
	}

	// 左上
	if(dir & UPPER_LEFT)
	{
		x = point.x;
		y = point.y;
		while(RawBoard[--x][--y] == -CurrentColor)
		{
			RawBoard[x][y] = CurrentColor;
			operation.x = x;
			operation.y = y;
			update.push_back(operation);
		}
	}

	// 左下
	if(dir & LOWER_LEFT)
	{
		x = point.x;
		y = point.y;
		while(RawBoard[--x][++y] == -CurrentColor)
		{
			RawBoard[x][y] = CurrentColor;
			operation.x = x;
			operation.y = y;
			update.push_back(operation);
		}
	}

	// 右下
	if(dir & LOWER_RIGHT)
	{
		x = point.x;
		y = point.y;
		while(RawBoard[++x][++y] == -CurrentColor)
		{
			RawBoard[x][y] = CurrentColor;
			operation.x = x;
			operation.y = y;
			update.push_back(operation);
		}
	}


	x = point.x;
	y = point.y;


	// 石の数を更新
	unsigned discdiff = update.size();

	Discs[CurrentColor]  += discdiff;
	Discs[-CurrentColor] -= discdiff -1;
	Discs[EMPTY]--;
	
	//今回の 行った操作ログに追加
	UpdateLog.push_back(update);


}



//ゲーム終了していればTrue、終了していなければfalse
bool Board::isGameOver() const
{
	// 60手に達していたらゲーム終了
	if(Turns == MAX_TURNS) return true;
	
	// 打てる手があるならゲーム終了ではない
	if(MovablePos[Turns].size() != 0) return false;
	
	//	現在の手番と逆の色が打てるかどうか調べる
	Disc disc;
	disc.color = -CurrentColor;
	for(int x=1; x<=BOARD_SIZE; x++)
	{
		disc.x = x;
		for(int y=1; y<=BOARD_SIZE; y++)
		{
			disc.y = y;
			// 置ける箇所が1つでもあればゲーム終了ではない
			if(checkMobility(disc) != NONE) return false;
		}
	}
	
	return true;
}


//パスする。パスできない場合=false
bool Board::pass()
{
	// 打つ手があればパスできない
	if(MovablePos[Turns].size() != 0) return false;
	
	// ゲームが終了しているなら、パスできない
	if(isGameOver()) return false;
	
	//色を反転する
	CurrentColor = -CurrentColor;
	
	// 空のupdateを挿入しておく
	std::vector<Disc> nullupdate;
	UpdateLog.push_back(nullupdate);
	
	//MovableDirとMovablePosを再計算する
	initMovable();
	
	return true;
}


//undo　もとに戻せない＝一手も打っていない場合false
bool Board::undo()
{
	// ゲーム開始地点ならもう戻れない
	if(Turns == 0) return false;

	CurrentColor = -CurrentColor;
	
	const std::vector<Disc> &update = UpdateLog.back();

	// 前回がパスかどうかで場合分け
	if(update.empty()) // 前回はパス
	{
		// MovablePos及びMovableDirを再構築。passしたという事は置ける場所が無いから、全てNONEにする
		MovablePos[Turns].clear();
		for(unsigned x=1; x<=BOARD_SIZE; x++)
		{
			for(unsigned y=1; y<=BOARD_SIZE; y++)
			{
				MovableDir[Turns][x][y] = NONE;
			}
		}
	}
	else // 前回はパスでない
	{
		//一手戻す
		Turns--;

		//update[0] = 前回打った場所
		int x = update[0].x;
		int y = update[0].y;

		// 石を元に戻す
		RawBoard[x][y] = EMPTY; //前回打った場所を空にする
		for(unsigned i=1; i<update.size(); i++)
		{
			RawBoard[update[i].x][update[i].y] = -update[i].color;
		}

		// 石数の更新
		unsigned discdiff = update.size();
		Discs[CurrentColor] -= discdiff;
		Discs[-CurrentColor] += discdiff -1;
		Discs[EMPTY]--;
	}

	// 不要になったupdateを1つ削除
	UpdateLog.pop_back();

	return true;
}


