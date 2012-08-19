using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Data;

namespace ReversiMetro
{
    partial class MainPage
    {
        public MainPage()
        {
            InitializeComponent();

            this.DataContext = new ViewModels.MainWindow_ViewModel();
        }
    }
}
