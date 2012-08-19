using System;
using System.ComponentModel;
using System.Diagnostics.CodeAnalysis;
using System.Linq.Expressions;
using System.Collections;
using System.Collections.Generic;

using System.ComponentModel.DataAnnotations;


using Microsoft.Practices.Prism.ViewModel;

namespace Gdf.Mvvm.Prism.ViewModel
{

    /// <summary>
    /// NotificationObjectを実装したViewModelのベースクラスです。
    /// Prism.ViewModel.NotificationObjectをベースにしています。
    /// </summary>
    /// <remarks>
    /// このクラスは、 <see cref="INotifyPropertyChanged"/> インタフェースを実装する基本的なサポートを提供しています
    /// UIスレッドにマーシャリングします
    /// </remarks>
#if SILVERLIGHT
    [System.Runtime.Serialization.DataContract]
#else
    [Serializable]
#endif
    public abstract class ViewModelBase : NotificationObject
    {

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public ViewModelBase():base() { }


        /// <summary>
        /// オブジェクト開放
        /// </summary>
        public virtual void Cleanup() { }



    }
}
