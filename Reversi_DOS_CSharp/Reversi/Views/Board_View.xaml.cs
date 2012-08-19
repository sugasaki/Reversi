using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Reversi.Views
{
    /// <summary>
    /// Board_View.xaml の相互作用ロジック
    /// </summary>
    public partial class Board_View : UserControl
    {
        public Board_View()
        {
            InitializeComponent();
        }


        private void Square_View_MouseDown(object sender, MouseButtonEventArgs e)
        {
            var border = sender as System.Windows.Controls.Border;
            var board_viewmodel = this.DataContext as Reversi.ViewModels.Board_ViewModel;
            if (board_viewmodel == null) return;
            var click_item_viewmodel = border.DataContext as Reversi.ViewModels.Square_ViewModel;
            if (click_item_viewmodel == null) return;
            board_viewmodel.ClickEvent(click_item_viewmodel);
        }


        private void ListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var o = sender as System.Windows.Controls.ListView;
            o.SelectedItem = null;
        }


    }
}
