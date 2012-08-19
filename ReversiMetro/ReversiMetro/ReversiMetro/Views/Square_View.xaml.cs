using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Data;

namespace ReversiMetro.Views
{
    public sealed partial class Square_View
    {
        public Square_View()
        {
            InitializeComponent();
        }

        private void Image_Tapped(object sender, Windows.UI.Xaml.Input.TappedEventArgs e)
        {
            var img = sender as Image;
            img.Visibility = Visibility.Visible;
        }
    }
}
