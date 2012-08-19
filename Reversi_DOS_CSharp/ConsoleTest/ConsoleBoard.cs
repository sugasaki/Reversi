using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Reversi.Engine;

namespace ConsoleTest
{
    internal class ConsoleBoard : Reversi.Engine.Board
    {

        public void print()
        {

            Console.WriteLine("  a b c d e f g h ");

            for (int y = 1; y <= 8; y++)
            {
                Console.Write( " " + y);
                for (int x = 1; x <= 8; x++)
                {
                    switch (base.getColor(x, y))
                    {
                        case Disc.BLACK:
                            Console.Write( "●");
                            break;
                        case Disc.WHITE:
                            Console.Write("○");
                            break;
                        default:
                            Console.Write("　");
                            break;
                    }
                }
                Console.WriteLine();
            }

        }
    }
}
