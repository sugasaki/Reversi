using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace reversi
{
    class Program
    {
        static void Main(string[] args)
        {


            while (true)
            {

                Console.WriteLine("手を入力してください");
                var str = Console.ReadLine(); // ユーザーの入力した文字列を1行読み込む

                Console.WriteLine(string.Format("YOU => {0}",str));
                Console.WriteLine("");
            }

        }
    }
}
