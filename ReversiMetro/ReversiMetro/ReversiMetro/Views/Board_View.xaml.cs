using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Data;

using ReversiMetro.ViewModels;

namespace ReversiMetro.Views
{
    public sealed partial class Board_View
    {
        public Board_View()
        {
            InitializeComponent();
        }

        private void ListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            //var o = sender as System.Windows.Controls.ListView;
            //o.SelectedItem = null;
        }

        private void Border_Tapped(object sender, Windows.UI.Xaml.Input.TappedEventArgs e)
        {
            var border = sender as Windows.UI.Xaml.Controls.UserControl;

            var board_viewmodel = this.DataContext as Board_ViewModel;
            if (board_viewmodel == null) return;

            var click_item_viewmodel = border.DataContext as Square_ViewModel;
            if (click_item_viewmodel == null) return;

            board_viewmodel.ClickEvent(click_item_viewmodel);
        }


    }
}
