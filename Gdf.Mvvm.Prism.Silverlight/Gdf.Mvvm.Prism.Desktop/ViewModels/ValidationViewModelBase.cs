using System;
using System.ComponentModel;
using System.Diagnostics.CodeAnalysis;
using System.Linq.Expressions;
using System.Collections;
using System.Collections.Generic;

using System.ComponentModel.DataAnnotations;


namespace Gdf.Mvvm.Prism.ViewModel
{


    [Serializable]
    public abstract class ValidationViewModelBase : Gdf.Mvvm.Prism.ViewModel.ViewModelBase, System.ComponentModel.IDataErrorInfo
    {


        #region IDataErrorInfo Members


        public abstract string this[string columnName] { get; }


        public string Error
        {
            get { return null; }
        }


        #endregion // IDataErrorInfo Members



    }
}
