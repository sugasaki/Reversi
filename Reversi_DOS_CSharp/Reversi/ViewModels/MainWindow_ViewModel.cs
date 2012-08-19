using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Gdf.Mvvm.Prism.ViewModel;
using Gdf.Mvvm.Prism.Commands;

namespace Reversi.ViewModels
{
    public class MainWindow_ViewModel : ViewModelBase
    {

        /// <summary>
        /// リバーシアルゴリズム
        /// </summary>
        private Reversi.Engine.Board _boardModel;

        /// <summary>
        /// コマンド
        /// </summary>
        public DelegateCommand undoCMD { get; set; }
        public DelegateCommand passCMD { get; set; }
        public DelegateCommand startCMD { get; set; }
       

        /// <summary>
        /// 盤のビューモデル
        /// </summary>
        public Board_ViewModel Board_ViewModel { get; set; }


        /// <summary>
        /// コンストラクタ
        /// </summary>
        public MainWindow_ViewModel()
        {
            _boardModel = new Reversi.Engine.Board();
            Board_ViewModel = new Board_ViewModel(_boardModel);

            undoCMD = new DelegateCommand(() =>Undo());
            passCMD = new DelegateCommand(() => Pass());
            startCMD = new DelegateCommand(() => Start());
        }



        /// <summary>
        /// ゲーム開始（リセット）
        /// </summary>
        private void Start()
        {
            _boardModel.init_member();
            Board_ViewModel.Reflesh();
        }


        /// <summary>
        /// Undo処理
        /// </summary>
        private void Undo()
        {
            _boardModel.undo();
            //_boardModel.undo();
            Board_ViewModel.Reflesh();
        }

        /// <summary>
        /// パス
        /// </summary>
        private void Pass()
        {
            _boardModel.pass();
        }

    }
}
