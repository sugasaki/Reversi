using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace reversi
{

    //using C = Console;

    using Color2 = Int16;

    public static class reversi
    {
        public const Color2 EMPTY = 0;
        public const Color2 WHITE = -1;
        public const Color2 BLACK = 1;
        public const Color2 WALL = 2;
    }



    /// <summary>
    /// セルの状態を表す
    /// </summary>
    public enum Color : int
    {
        EMPTY = 0,  //何も置かれていないセル
        WHITE = -1, //白石が置かれている
        BLACK = 1,  //黒石が置かれている
        WALL = 2,   //壁（はじっこ=石が置けない）
    }

    
    /// <summary>
    /// 方向を表す定数
    /// </summary>
    public enum Direction : uint
    {
        /// <summary>
        /// 方向無
        /// </summary>
        NONE = 0,           //方向無

        /// <summary>
        /// 上
        /// </summary>
        UPPER = 1,          //上

        /// <summary>
        /// 左上
        /// </summary>
        UPPER_LEFT = 2,     //左上

        /// <summary>
        /// 左
        /// </summary>
        LEFT = 4,           //左

        /// <summary>
        /// 左下
        /// </summary>
        LOWER_LEFT = 8,     //左下

        /// <summary>
        /// 下
        /// </summary>
        LOWER = 16,         //下

        /// <summary>
        /// 右下
        /// </summary>
        LOWER_RIGHT = 32,   //右下

        /// <summary>
        /// 右
        /// </summary>
        RIGHT = 64,         //右

        /// <summary>
        /// 右上
        /// </summary>
        UPPER_RIGHT = 128,  //右上
    }
    


}
