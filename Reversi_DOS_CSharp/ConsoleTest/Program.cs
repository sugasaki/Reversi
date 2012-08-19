using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Reversi.Engine;

namespace ConsoleTest
{
    class Program
    {
        static void Main(string[] args)
        {

            var board = new ConsoleBoard();

            while (true)
            {

                Console.Write(string.Format("黒石:{0}  白石:{0}  あなた=>黒", board.CountDisc(Disc.BLACK), board.CountDisc(Disc.WHITE)));
                Console.Write(string.Format("  空マス{0} ", board.CountDisc(Disc.EMPTY)));
                Console.WriteLine();

                board.print();
                Console.WriteLine();


                #region CPUの番だったらランダムに手を打つ
                if (board.CurrentColor == Disc.WHITE)
                {
                    Console.Write("PC思考中: => ");
                    //CPUの手番
                    var cpu_p = board.RondomFlip();

                    board.move(cpu_p); //ランダムな手を打つ
                    Console.Write(cpu_p);
                    Console.WriteLine();

                    continue;
                }
                #endregion


                Console.Write("手を入力して下さい( ");

                #region 置ける場所を表示
                var movables = board.MovablePos; //置ける場所を取得
                for (int i = 0; i < movables.Count(); i++)
                {
                    Console.Write(string.Format("{0} ", movables[i].StringA1));
                }
                Console.Write("):");
                #endregion


                //ユーザー入力
                string input_string;
                input_string = Console.ReadLine();//入力させる


                #region パス
                if (string.Compare(input_string, "p", true) == 0)
                {
                    if (!board.pass())
                    {
                        Console.WriteLine("パスできません！");
                    }
                    continue;
                }
                #endregion

                #region やりなおし
                if (string.Compare(input_string, "u", true) == 0)
                {
				    board.undo();
				    continue;
                }
                #endregion


                #region 入力値チェック
                Point p = Point.GetPoint(input_string);
                if (p==null)
                {
                    Console.WriteLine();
                    Console.WriteLine("リバーシ形式の手を入力して下さい！");
                    Console.WriteLine();
                    continue;
                }
                #endregion


                #region 石配置
                if (board.move(p) == false)
                {
                    Console.WriteLine();
                    Console.WriteLine("そこには置けません！");
                    Console.WriteLine();
                    continue;
                }
                #endregion


                #region ゲームオーバー判定
                if (board.isGameOver())
                {
                    Console.WriteLine("----------------ゲーム終了----------------");
                    return;
                }
                #endregion



            }


        }
    }
}
