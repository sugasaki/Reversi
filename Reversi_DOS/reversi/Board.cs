using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace reversi
{

    /// <summary>
    /// 盤クラス
    /// </summary>
    public class Board
    {


        private const int BOARD_SIZE = 8;

        /// <summary>
        /// ボードの状態[10×10]
        /// </summary>
        private Color[,] RawBoard = new Color[BOARD_SIZE + 2, BOARD_SIZE + 2]; //2= 端の壁


        /// <summary>
        /// discで指定された座標に、disc.colorの色の石を打てるかどうか、
        /// また、どの方向に石を裏返せるかを判定する。
        /// 石を裏返せる方向にフラグが立った整数値が返る。
        /// </summary>
        /// <param name="disc"></param>
        /// <returns></returns>
        private Direction checkMobility(Disc disc)
        {
            int x = 0, y = 0;
            var dir = Direction.NONE;

            if (RawBoard[disc.x, disc.y] != Color.EMPTY) return dir;


            #region 各方向探索

            //上方向探索
            if ((int)RawBoard[disc.x, disc.y - 1] == -(int)disc.color)
            {
                x = disc.x; y = disc.y - 2;
                while ((int)RawBoard[disc.x, disc.y] == -(int)disc.color) { y--; }//disc.colorを反転した色が続く間ループする
                if (RawBoard[disc.x, disc.y] == disc.color) dir |= Direction.UPPER; //端がdiscと同じ色ならば、裏返せるのでフラグを立てる
            }


            //下方向探索
            if ((int)RawBoard[disc.x, disc.y + 1] == -(int)disc.color)
            {
                x = disc.x; y = disc.y + 2;
                while ((int)RawBoard[disc.x, disc.y] == -(int)disc.color) { y++; }//disc.colorを反転した色が続く間ループする
                if (RawBoard[disc.x, disc.y] == disc.color) dir |= Direction.LOWER; //端がdiscと同じ色ならば、裏返せるのでフラグを立てる
            }


            //左方向探索
            if ((int)RawBoard[disc.x-1, disc.y] == -(int)disc.color)
            {
                x = disc.x - 2; y = disc.y;
                while ((int)RawBoard[disc.x, disc.y] == -(int)disc.color) { x--; }//disc.colorを反転した色が続く間ループする
                if (RawBoard[disc.x, disc.y] == disc.color) dir |= Direction.LEFT; //端がdiscと同じ色ならば、裏返せるのでフラグを立てる
            }


            //右方向探索
            if ((int)RawBoard[disc.x + 1, disc.y] == -(int)disc.color)
            {
                x = disc.x + 2; y = disc.y;
                while ((int)RawBoard[disc.x, disc.y] == -(int)disc.color) { x++; }//disc.colorを反転した色が続く間ループする
                if (RawBoard[disc.x, disc.y] == disc.color) dir |= Direction.RIGHT; //端がdiscと同じ色ならば、裏返せるのでフラグを立てる
            }


            //右上方向探索
            if ((int)RawBoard[disc.x + 1, disc.y - 1] == -(int)disc.color)
            {
                x = disc.x + 2; y = disc.y - 2;
                while ((int)RawBoard[disc.x, disc.y] == -(int)disc.color) { x++; y--; }//disc.colorを反転した色が続く間ループする
                if (RawBoard[disc.x, disc.y] == disc.color) dir |= Direction.UPPER_RIGHT; //端がdiscと同じ色ならば、裏返せるのでフラグを立てる
            }


            //左上方向探索
            if ((int)RawBoard[disc.x - 1, disc.y - 1] == -(int)disc.color)
            {
                x = disc.x - 2; y = disc.y - 2;
                while ((int)RawBoard[disc.x, disc.y] == -(int)disc.color) { x--; y--; }//disc.colorを反転した色が続く間ループする
                if (RawBoard[disc.x, disc.y] == disc.color) dir |= Direction.UPPER_LEFT; //端がdiscと同じ色ならば、裏返せるのでフラグを立てる
            }


            //左下方向探索
            if ((int)RawBoard[disc.x - 1, disc.y + 1] == -(int)disc.color)
            {
                x = disc.x - 2; y = disc.y + 2;
                while ((int)RawBoard[disc.x, disc.y] == -(int)disc.color) { x--; y++; }//disc.colorを反転した色が続く間ループする
                if (RawBoard[disc.x, disc.y] == disc.color) dir |= Direction.LOWER_LEFT; //端がdiscと同じ色ならば、裏返せるのでフラグを立てる
            }


            //右下方向探索
            if ((int)RawBoard[disc.x + 1, disc.y + 1] == -(int)disc.color)
            {
                x = disc.x + 2; y = disc.y + 2;
                while ((int)RawBoard[disc.x, disc.y] == -(int)disc.color) { x++; y++; }//disc.colorを反転した色が続く間ループする
                if (RawBoard[disc.x, disc.y] == disc.color) dir |= Direction.LOWER_RIGHT; //端がdiscと同じ色ならば、裏返せるのでフラグを立てる
            }

            #endregion

            return dir;
        }


    }
}
