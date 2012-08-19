using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;

using Gdf.Mvvm.Prism.ViewModel;
using Gdf.Mvvm.Prism.Commands;

namespace Reversi.ViewModels
{
    public class Square_ViewModel : ViewModelBase
    {

        public int x { get; set; }
        public int y { get; set; }


        /// <summary>
        /// 黒石を表示・非表示
        /// </summary>
        private System.Windows.Visibility _blackVisibility;
        public System.Windows.Visibility BlackVisibility
        {
            get { return _blackVisibility; }
            set
            {
                _blackVisibility = value;
                this.RaisePropertyChanged(() => BlackVisibility);
            }
        }

        /// <summary>
        /// 白石を表示・非表示
        /// </summary>
        private System.Windows.Visibility _whiteVisibility;
        public System.Windows.Visibility WhiteVisibility
        {
            get { return _whiteVisibility; }
            set
            {
                _whiteVisibility = value;
                this.RaisePropertyChanged(() => WhiteVisibility);
            }
        }

        /// <summary>
        /// 置ける場所ガイドを表示・非表示
        /// </summary>
        private System.Windows.Visibility _isSelectedVisibility;
        public System.Windows.Visibility IsSelectedVisibility
        {
            get { return _isSelectedVisibility; }
            set
            {
                _isSelectedVisibility = value;
                this.RaisePropertyChanged(() => IsSelectedVisibility);
            }
        }

        
    }
}
