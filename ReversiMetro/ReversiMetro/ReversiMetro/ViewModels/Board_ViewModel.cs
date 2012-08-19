using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.ObjectModel;

using Gdf.Mvvm.Prism.ViewModel;
using Gdf.Mvvm.Prism.Commands;
using Windows.UI.Xaml;

namespace ReversiMetro.ViewModels
{
    public class Board_ViewModel : ViewModelBase
    {
        /// <summary>
        /// リバーシアルゴリズム
        /// </summary>
        private Reversi.Engine.Board _boardModel;

        /// <summary>
        /// 石リスト
        /// </summary>
        public ObservableCollection<Square_ViewModel> SquareList { get; set; }

        /// <summary>
        /// BOARD_SIZEアクセサ
        /// </summary>
        public int BOARD_SIZE { get { return Reversi.Engine.Reversi.BOARD_SIZE; } }


        /// <summary>
        /// コンストラクタ
        /// </summary>
        public Board_ViewModel(Reversi.Engine.Board boardModel)
        {
            _boardModel = boardModel;

            SquareList = new ObservableCollection<Square_ViewModel>();

            for (int x = 1; x <= Reversi.Engine.Reversi.BOARD_SIZE; x++)
            {
                for (int y = 1; y <= Reversi.Engine.Reversi.BOARD_SIZE; y++)
                {
                    int color = _boardModel.getColor(x, y);
                    var item = new Square_ViewModel();
                    item.x = x; item.y = y;
                    SquareList.Add(item);
                }
            }

            Reflesh();

        }


        /// <summary>
        /// 盤のクリック
        /// </summary>
        /// <param name="click_item"></param>
        public void ClickEvent(Square_ViewModel click_item)
        {
            _boardModel.move(new Reversi.Engine.Point(click_item.x, click_item.y));

            #region CPUの番だったらランダムに手を打つ
            if (_boardModel.CurrentColor == Reversi.Engine.Disc.WHITE)
            {
                //CPUの手番
                var cpu_p = _boardModel.RondomFlip();
                if (cpu_p == null)
                {
                    _boardModel.pass(); //パス
                }
                else
                {
                    _boardModel.move(cpu_p); //ランダムな手を打つ
                }
            }
            #endregion


            Reflesh();

            CheckGameOver();

        }


        /// <summary>
        /// 画面のリフレッシュ
        /// </summary>
        public void Reflesh()
        {
            foreach (var item in SquareList)
            {
                int x = item.x;
                int y = item.y;

                int color = _boardModel.getColor(x, y);

                if (color == Reversi.Engine.Disc.EMPTY) { item.BlackVisibility = Visibility.Collapsed; item.WhiteVisibility = Visibility.Collapsed; }
                if (color == Reversi.Engine.Disc.BLACK) { item.BlackVisibility = Visibility.Visible; item.WhiteVisibility = Visibility.Collapsed; }
                if (color == Reversi.Engine.Disc.WHITE) { item.BlackVisibility = Visibility.Collapsed; item.WhiteVisibility = Visibility.Visible; }


                item.IsSelectedVisibility = (_boardModel.IsOkeru(x, y)) ? Visibility.Visible : Visibility.Collapsed;
            }
        }


        /// <summary>
        /// ゲーム終了か判定し、終了ならばメッセージを表示する
        /// </summary>
        private void CheckGameOver()
        {

            if (_boardModel.isGameOver() == false) return;


            string message = "ゲーム終了です。";


            if (_boardModel.IsDraw())
            {
                message += "引き分けです";
            }
            else
            {
                message += _boardModel.Winer() == Reversi.Engine.Disc.BLACK ? "あなた" : "CPU";
                message += "の勝ちです";
            }

            //System.Windows.MessageBox.Show(message);


        }



    }

}
