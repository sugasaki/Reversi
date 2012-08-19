using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Reversi.Engine
{

    /// <summary>
    /// 石 クラス
    /// </summary>
    public class Disc : Point
    {
	    public const int BLACK = 1;
        public const int EMPTY = 0;
        public const int WHITE = -1;
        public const int WALL = 2;

        public int color { get; set; }

        #region コンストラクタ

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public Disc():base(0,0)
        {
            this.color = EMPTY;
        }


        public Disc(int x, int y, int color)
            : base(x, y)
        {
            this.color = color;
        }


        #endregion

    }




    /// <summary>
    /// 石リスト
    /// </summary>
    public class DisctList : List<Disc>
    {
    }


}
