using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace reversi
{

    /// <summary>
    /// 座標クラス
    /// </summary>
    public class Point
    {

        public int x { get; set; }
        public int y { get; set; }


        public Point()
        {
            //Point(0, 0);
            this.x = 0;
            this.y = 0;
        }


        public Point(int x, int y)
        {
            this.x = x;
            this.y = y;
        }



    }
}
