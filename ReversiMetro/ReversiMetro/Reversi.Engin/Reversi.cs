using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Reversi.Engine
{
    public static class Reversi
    {
        public static int BOARD_SIZE = 8;
        public static int MAX_TURNS = 60;

    }



    /// <summary>
    /// 進行方向
    /// </summary>
    public static class Direction
    {
        //方向を表す定数（ビット演算子用）
        public static uint NONE = 0;
        public static uint UPPER = 1;
        public static uint UPPER_LEFT = 2;
        public static uint LEFT = 4;
        public static uint LOWER_LEFT = 8;
        public static uint LOWER = 16;
        public static uint LOWER_RIGHT = 32;
        public static uint RIGHT = 64;
        public static uint UPPER_RIGHT = 128;
    }



}
