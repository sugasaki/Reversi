using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Reversi.Engine
{

    /// <summary>
    /// 位置座標 クラス
    /// </summary>
    public class Point
    {
        public short? au_ord;

        public int x;
        public int y;


        #region コンストラクタ

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public Point() : this(0, 0) { }

        /// <summary>
        /// コンストラクタ（X,Y指定）
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        public Point(int x, int y)
        {
            this.x = x;
            this.y = y;
        }


        #endregion



        /// <summary>
        /// 文字列から、X,Y変換し、Pointを取得する
        /// 変換失敗したらNullを返す
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        public static Point GetPoint(string coordstr)
        {
            if (coordstr == null || coordstr.Length < 2) return null;
            int x = coordstr[0] - 'a' + 1;
            int y = coordstr[1] - '1' + 1;
            return new Point(x, y);
        }




        /// <summary>
        /// A1方式へ文字列変換
        /// </summary>
        /// <returns></returns>
        public string StringA1
        {
            get
            {
                string coord = string.Empty;

                coord += (char)('a' + this.x - 1);
                coord += (char)('1' + this.y - 1);

                return coord;
            }
        }


    }


    /// <summary>
    /// 位置座標 リスﾄ
    /// </summary>
    public class PointList : List<Point>
    {
    }


}
