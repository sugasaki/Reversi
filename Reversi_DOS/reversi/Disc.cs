using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace reversi
{

    /// <summary>
    /// 石クラス
    /// </summary>
    public class Disc : Point
    {

        public Color color;


        public Disc()
            : base(0, 0)
        {
            color = Color.EMPTY;
        }

        public Disc(int x, int y, Color color)
            : base(x, y)
        {
            this.color = color;
        }


    }
}
