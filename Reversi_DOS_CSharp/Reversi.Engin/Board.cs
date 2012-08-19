using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Reversi.Engine
{
    public class Board
    {

        #region フィールド

        //それぞれの石の色数を保持するオブジェクト
        private Dictionary<int, int> _discs = null;


        private int[,] _rawBoard = null;//盤のマスを表す。両端の壁を含むので+2

        private int _turns = 0; //現在のターン数
        private int _currentColor; //現在の手番（黒or白）



        //変更点を格納するコレクション
        Stack<DisctList> UpdateLog = null;

        //checkMobilityの結果格納用。全ての目（Point）において置ける方向を保持する。ターンが終わればクリアされ再計算する
        private uint[, ,] MovableDir = new uint[Reversi.MAX_TURNS + 1, Reversi.BOARD_SIZE + 2, Reversi.BOARD_SIZE + 2];


	    //打てる位置の座標群
        PointList[] _movablePos = new PointList[Reversi.MAX_TURNS+1];


        #endregion


        /// <summary>
        /// コンストラクタ
        /// </summary>
        public Board()
        {
            // Vectorの配列を初期化
            for (int i = 0; i <= Reversi.MAX_TURNS; i++)
            {
                _movablePos[i] = new PointList();
            }

            init_member();
        }


        #region private

        /// <summary>
        /// 全てのメンバ初期化
        public void init_member()
        {

            _rawBoard = new int[Reversi.BOARD_SIZE+2, Reversi.BOARD_SIZE+2];//盤のマスを表す。両端の壁を含むので+2

	        // 全マスを空きマスに設定
            for (int x = 1; x <= Reversi.BOARD_SIZE; x++)
	        {
                for (int y = 1; y <= Reversi.BOARD_SIZE; y++)
		        {
                    _rawBoard[x, y] = Disc.EMPTY;
		        }
	        }


            // 壁の設定（縦）
            for (int y = 0; y < Reversi.BOARD_SIZE + 2; y++)
            {
                _rawBoard[0, y] = Disc.WALL;
                _rawBoard[Reversi.BOARD_SIZE + 1, y] = Disc.WALL;
            }

            // 壁の設定（横）
            for (int x = 0; x < Reversi.BOARD_SIZE + 2; x++)
            {
                _rawBoard[x, 0] = Disc.WALL;
                _rawBoard[x, Reversi.BOARD_SIZE + 1] = Disc.WALL;
            }


            // 初期配置
            _rawBoard[4, 4] = Disc.WHITE;
            _rawBoard[5, 5] = Disc.WHITE;
            _rawBoard[4, 5] = Disc.BLACK;
            _rawBoard[5, 4] = Disc.BLACK;


            // 石数の初期設定
            _discs = new Dictionary<int, int>();
            _discs[Disc.BLACK] = 2;
            _discs[Disc.WHITE] = 2;
            _discs[Disc.EMPTY] = Reversi.BOARD_SIZE * Reversi.BOARD_SIZE - 4;

            _turns = 0; // 手数は0から数える
            _currentColor = Disc.BLACK; // 先手は黒

            // updateを全て消去
            if (UpdateLog != null) UpdateLog.Clear();
            UpdateLog = new Stack<DisctList>();

            //
            initMovable();

        }


        /// <summary>
        /// 現在の局面で、現在の番手(黒or白)が置ける場所を計算する
        /// MovableDirと_movablePosを再計算する</summary>
        private void initMovable()
        {
	        Disc disc = new Disc(0, 0, _currentColor);//構造体のインスタンスを現在色で作成 

	        uint dir;

	        //ポジションクリア
	        _movablePos[_turns].Clear();

	        //盤の全ての目において、置けるかどうかを調べる。
	        for(int x=1; x<=Reversi.BOARD_SIZE; x++)//x=0の時は壁なので1から開始する
	        {
		        for(int y=1; y<=Reversi.BOARD_SIZE; y++)
		        {
                    disc = new Disc(x, y, _currentColor);
			
			        //石を置ける場所か判定
			        dir = checkMobility(disc); //discの位置に置けるなら、discから見て石が返る方向を表したビットを取得
			        if(dir != Direction.NONE)
			        {
				        // 置ける
				        _movablePos[_turns].Add(disc); //末尾に格納（discのコピーが格納される？）
			        }

			        MovableDir[_turns, x, y] = dir; //置ける方向を表したビット（Direction.NONEでもDirection.NONE以外でも結果を格納）
		        }
	        }
        }


        
        //石が置けるかどうか判定する
        private uint checkMobility(Disc disc) 
        {
	        // 既に石があったら置けない
            if (_rawBoard[disc.x, disc.y] != Disc.EMPTY) return Direction.NONE;

	        int x, y;
            uint dir = Direction.NONE;


	        // 上
            if (_rawBoard[disc.x, disc.y - 1] == -disc.color)
            {
		        x = disc.x; y = disc.y-2;
		        while(_rawBoard[x,y] == -disc.color) { y--; }
		        if(_rawBoard[x,y] == disc.color) dir |= Direction.UPPER;
            }

            // 下
            if (_rawBoard[disc.x, disc.y + 1] == -disc.color)
            {
                x = disc.x; y = disc.y + 2;
                while (_rawBoard[x, y] == -disc.color) { y++; }
                if (_rawBoard[x, y] == disc.color) dir |= Direction.LOWER;
            }

            // 左
            if (_rawBoard[disc.x - 1, disc.y] == -disc.color)
            {
                x = disc.x - 2; y = disc.y;
                while (_rawBoard[x, y] == -disc.color) { x--; }
                if (_rawBoard[x, y] == disc.color) dir |= Direction.LEFT;
            }

            // 右
            if (_rawBoard[disc.x + 1, disc.y] == -disc.color)
            {
                x = disc.x + 2; y = disc.y;
                while (_rawBoard[x, y] == -disc.color) { x++; }
                if (_rawBoard[x, y] == disc.color) dir |= Direction.RIGHT;
            }


            // 右上
            if (_rawBoard[disc.x + 1, disc.y - 1] == -disc.color)
            {
                x = disc.x + 2; y = disc.y - 2;
                while (_rawBoard[x, y] == -disc.color) { x++; y--; }
                if (_rawBoard[x, y] == disc.color) dir |= Direction.UPPER_RIGHT;
            }

            // 左上
            if (_rawBoard[disc.x - 1, disc.y - 1] == -disc.color)
            {
                x = disc.x - 2; y = disc.y - 2;
                while (_rawBoard[x, y] == -disc.color) { x--; y--; }
                if (_rawBoard[x, y] == disc.color) dir |= Direction.UPPER_LEFT;
            }

            // 左下
            if (_rawBoard[disc.x - 1, disc.y + 1] == -disc.color)
            {
                x = disc.x - 2; y = disc.y + 2;
                while (_rawBoard[x, y] == -disc.color) { x--; y++; }
                if (_rawBoard[x, y] == disc.color) dir |= Direction.LOWER_LEFT;
            }

            // 右下
            if (_rawBoard[disc.x + 1, disc.y + 1] == -disc.color)
            {
                x = disc.x + 2; y = disc.y + 2;
                while (_rawBoard[x, y] == -disc.color) { x++; y++; }
                if (_rawBoard[x, y] == disc.color) dir |= Direction.LOWER_RIGHT;
            }


	        return dir;

        }



        //石を打つ、変更点追跡、石裏返す、石数カウント、UpdateLog記録
        private void flipDiscs(Point point)
        {
	        int x, y;

	        //pointから見て、石が反転する方向をビットで取得
	        uint dir = MovableDir[_turns,point.x,point.y];

	        //今オペレーションの操作ログ記録用
	        var update = new DisctList();

	        //置いた場所＝pointを置いた色にする
	        _rawBoard[point.x,point.y] = _currentColor;
	
	        //最初に置いた石をログ記録
            update.Add(new Disc(point.x, point.y, _currentColor));

	        // 上
	        if ((dir & Direction.UPPER) != 0)
	        {
		        y = point.y;
		        while(_rawBoard[point.x,--y] == -_currentColor)
		        {
			        _rawBoard[point.x,y] = _currentColor;
                    update.Add(new Disc(point.x, y, _currentColor));
                }
	        }

            // 下
            if ((dir & Direction.LOWER) != Direction.NONE)
            {
                y = point.y;
                while (_rawBoard[point.x, ++y] != _currentColor)
                {
                    _rawBoard[point.x, y] = _currentColor;
                    update.Add(new Disc(point.x, y, _currentColor));
                }
            }

            // 左
            if ((dir & Direction.LEFT) != Direction.NONE)
            {
                x = point.x;
                while (_rawBoard[--x, point.y] != _currentColor)
                {
                    _rawBoard[x, point.y] = _currentColor;
                    update.Add(new Disc(x, point.y, _currentColor));
                }
            }

            // 右
            if ((dir & Direction.RIGHT) != Direction.NONE)
            {
                x = point.x;
                while (_rawBoard[++x, point.y] != _currentColor)
                {
                    _rawBoard[x, point.y] = _currentColor;
                    update.Add(new Disc(x, point.y, _currentColor));
                }
            }

            // 右上
            if ((dir & Direction.UPPER_RIGHT) != Direction.NONE)
            {
                x = point.x;
                y = point.y;
                while (_rawBoard[++x, --y] != _currentColor)
                {
                    _rawBoard[x, y] = _currentColor;
                    update.Add(new Disc(x, y, _currentColor));
                }
            }

            // 左上
            if ((dir & Direction.UPPER_LEFT) != Direction.NONE)
            {
                x = point.x;
                y = point.y;
                while (_rawBoard[--x, --y] != _currentColor)
                {
                    _rawBoard[x, y] = _currentColor;
                    update.Add(new Disc(x, y, _currentColor));
                }
            }

            // 左下
            if ((dir & Direction.LOWER_LEFT) != Direction.NONE)
            {
                x = point.x;
                y = point.y;
                while (_rawBoard[--x, ++y] != _currentColor)
                {
                    _rawBoard[x, y] = _currentColor;
                    update.Add(new Disc(x, y, _currentColor));
                }
            }

            // 右下
            if ((dir & Direction.LOWER_RIGHT) != Direction.NONE)
            {
                x = point.x;
                y = point.y;
                while (_rawBoard[++x, ++y] != _currentColor)
                {
                    _rawBoard[x, y] = _currentColor;
                    update.Add(new Disc(x, y, _currentColor));
                }
            }


	        // 石の数を更新
	        int discdiff = update.Count();

	        this._discs[_currentColor]  += discdiff;
            this._discs[-_currentColor] -= discdiff - 1;
            this._discs[Disc.EMPTY]--;
	
	        //今回の 行った操作ログに追加
            this.UpdateLog.Push(update);

        }

        #endregion


        #region public メソッド


        /// <summary>
        /// pointで指定された位置に石を打つ。
        /// 成功したら return true
        /// 及び、次のターンの計算結果を調査開始する。
        /// </summary>
        /// <param name="?"></param>
        /// <returns></returns>
        public bool move(Point point)
        {
            //石が打てる位置か判定する
            if (IsOkeru(point.x, point.y) == false) return false;
            //if (point.x <= 0 || point.x > Reversi.BOARD_SIZE) return false;
            //if (point.y <= 0 || point.y > Reversi.BOARD_SIZE) return false;
            //if (MovableDir[_turns, point.x, point.y] == Direction.NONE) return false; //checkMobilityの結果。既に石があるか、置けない場所を指定したなら falseを返す

            //石を打つ、変更点追跡、石裏返す、石数カウント、UpdateLog記録
            flipDiscs(point);

            //手数をインクリメント
            this._turns++;
            this._currentColor = -_currentColor;//白→黒　or　黒→白

            //MovableDirと_movablePosを調べ直す。（再計算）
            initMovable(); //現在の局面で、現在の番手(黒or白)が置ける場所を計算する

            return true;
        }

        
        //パスする。パスできない場合=false
        public bool pass()
        {
	        // 打つ手があればパスできない
	        if(_movablePos[_turns].Count() != 0) return false;
	
	        // ゲームが終了しているなら、パスできない
	        if(isGameOver()) return false;
	
	        //色を反転する
	        _currentColor = -_currentColor;
	
	        // 空のupdateを挿入しておく
	        var nullupdate = new DisctList();
            UpdateLog.Push(nullupdate);
	
	        //MovableDirと_movablePosを再計算する
	        initMovable();
	
	        return true;
        }


        //undo　もとに戻せない＝一手も打っていない場合false
        public bool undo()
        {
	        // ゲーム開始地点ならもう戻れない
	        if(_turns == 0) return false;

	        _currentColor = -_currentColor;
	
	        var update = UpdateLog.Peek();//最後に格納したオブジェクトを取得

	        // 前回がパスかどうかで場合分け
	        if(update.Count==0) // 前回はパス
	        {
		        // _movablePos及びMovableDirを再構築。passしたという事は置ける場所が無いから、全てDirection.NONEにする
		        _movablePos[_turns].Clear();
		        for(int x=1; x<= Reversi.BOARD_SIZE; x++)
		        {
                    for (int y = 1; y <= Reversi.BOARD_SIZE; y++)
			        {
				        MovableDir[_turns,x,y] = Direction.NONE;
			        }
		        }
	        }
	        else // 前回はパスでない
	        {
		        //一手戻す
		        _turns--;

		        //update[0] = 前回打った場所
		        int x = update[0].x;
		        int y = update[0].y;

		        // 石を元に戻す
                _rawBoard[x, y] = Disc.EMPTY; //前回打った場所を空にする
		        for(int i=1; i<update.Count(); i++)
		        {
			        _rawBoard[update[i].x,update[i].y] = -(int)(update[i].color);
		        }

		        // 石数の更新
		        int discdiff = update.Count();
		        _discs[_currentColor] -= discdiff;
		        _discs[-_currentColor] += discdiff -1;
                _discs[Disc.EMPTY]--;
	        }

	        // 不要になったupdateを1つ削除
            UpdateLog.Pop();

	        return true;
        }


        #endregion



        #region public

        //ゲーム終了していればTrue、終了していなければfalse
        public bool isGameOver()
        {
            // 60手に達していたらゲーム終了
            if (_turns == Reversi.MAX_TURNS) return true;

            // 打てる手があるならゲーム終了ではない
            if (_movablePos[_turns].Count() != 0) return false;

            //	現在の手番と逆の色が打てるかどうか調べる
            var disc = new Disc();
            disc.color = -_currentColor;
            for (int x = 1; x <= Reversi.BOARD_SIZE; x++)
            {
                disc.x = x;
                for (int y = 1; y <= Reversi.BOARD_SIZE; y++)
                {
                    disc.y = y;
                    // 置ける箇所が1つでもあればゲーム終了ではない
                    if (checkMobility(disc) != Direction.NONE) return false;
                }
            }

            return true;
        }


        /// <summary>
        /// 石が打てる位置か判定する
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        public bool IsOkeru(int x, int y)
        {
            if (x <= 0 || x > Reversi.BOARD_SIZE) return false;
            if (y <= 0 || y > Reversi.BOARD_SIZE) return false;
            if (MovableDir[_turns, x, y] == Direction.NONE) return false; //checkMobilityの結果。既に石があるか、置けない場所を指定したなら falseを返す
            return true;
        }


        /// <summary>
        /// 石が多い番手を返す。1=先手、2＝後手
        /// </summary>
        /// <param name="color"></param>
        /// <returns></returns>
        public int Winer()
        {
            return this.CountDisc(Disc.BLACK) > this.CountDisc(Disc.WHITE) ? Disc.BLACK : Disc.WHITE;
        }

        /// <summary>
        /// 引き分けかどうか
        /// </summary>
        /// <returns></returns>
        public bool IsDraw()
        {
            return this.CountDisc(0) == this.CountDisc(1) ? true : false;
        }


        #endregion


        
        #region プロパティ



        /// <summary>
        /// Pointを指定して、その場所の色を返す
        /// </summary>
        /// <param name="p"></param>
        /// <returns></returns>
        public int getColor(Point p) 
	    {
		    return _rawBoard[p.x, p.y];
	    }

        /// <summary>
        /// X,Yを指定して、その場所の色を返す
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        public int getColor(int x, int y)
        {
            return _rawBoard[x, y];
        }


        /// <summary>
        /// 石の数を返す
        /// </summary>
        /// <param name="color"></param>
        /// <returns></returns>
        public int CountDisc(int color)
        {
            return _discs[color];
        }


        /// <summary>
        /// 現在の置ける場所を取得
        /// </summary>
        public PointList MovablePos { get { return _movablePos[_turns]; } }


        /// <summary>
        /// 現在の手番の色を返す
        /// </summary>
        public int CurrentColor { get { return _currentColor; } }



        #endregion


        #region 人口無能

        /// <summary>
        /// // 現在の置ける場所群からランダムに選択し置ける場所を返す
        /// </summary>
        /// <returns></returns>
        public Point RondomFlip()
        {
            // 現在の置ける場所を取得
            var movables = this.MovablePos;

            //置ける個所が１つしかなかったら、即座に一番目を返す
            if (movables.Count() == 1) return movables.First();

            //置ける個所がなかったらエラー
            if (movables.Count() == 0) return null;

            //ランダム数値取得
            var rnd = new Random();
            int i = rnd.Next(movables.Count());

            return movables[i];
        }

        #endregion

    }
}
