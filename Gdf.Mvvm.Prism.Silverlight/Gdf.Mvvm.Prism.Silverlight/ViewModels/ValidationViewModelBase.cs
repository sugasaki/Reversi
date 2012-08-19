using System;
using System.ComponentModel;
using System.Diagnostics.CodeAnalysis;
using System.Linq.Expressions;
using System.Collections;
using System.Collections.Generic;

using System.ComponentModel.DataAnnotations;


namespace Gdf.Mvvm.Prism.ViewModel
{


    [System.Runtime.Serialization.DataContract]
    public abstract class ValidationViewModelBase : Gdf.Mvvm.Prism.ViewModel.ViewModelBase, System.ComponentModel.INotifyDataErrorInfo
    {


        #region INotifyDataErrorInfo メンバー


        protected Dictionary<string, List<string>> _errors = new Dictionary<string, List<string>>();

        protected void AddError(string propertyName, string message, bool isWarning)
        {
            //	ディクショナリに当該プロパティのエラーが無かったら、ディクショナリのエントリを作成します。
            if (!this._errors.ContainsKey(propertyName))
                this._errors.Add(propertyName, new List<string>());
            //	ディクショナリの当該プロパティのエントリ内に同一のメッセージが無かったら追加します。
            if (!this._errors[propertyName].Contains(message))
            {
                //	警告メッセージは追加、エラーは挿入します。（エラーメッセージを上位にする。）
                if (isWarning)
                    this._errors[propertyName].Add(message);
                else
                    this._errors[propertyName].Insert(0, message);
                //	エラー状態が変更されたことを通知します。
                this.OnErrorsChanged(propertyName);
            }
        }

        protected void RemoveError(string propertyName, string message)
        {
            //	ディクショナリの当該プロパティのエントリ内に同一のメッセージが存在したら、メッセージを削除します。
            if ((this._errors.ContainsKey(propertyName)) &&
                (this._errors[propertyName].Contains(message)))
            {
                //	メッセージを削除します。
                this._errors[propertyName].Remove(message);
                //	メッセージが無くなったら（=警告もエラーもない）、ディクショナリからエントリを削除します。
                if (this._errors[propertyName].Count == 0)
                    this._errors.Remove(propertyName);
                //	エラー状態が変更されたことを通知します。
                this.OnErrorsChanged(propertyName);
            }
        }

        public event EventHandler<DataErrorsChangedEventArgs> ErrorsChanged;
        protected void OnErrorsChanged(string propertyName)
        {
            EventHandler<DataErrorsChangedEventArgs> handlers = this.ErrorsChanged;
            if (handlers != null)
                handlers(this, new DataErrorsChangedEventArgs(propertyName));
        }

        public IEnumerable GetErrors(string propertyName)
        {
            return (this._errors.ContainsKey(propertyName)) ? this._errors[propertyName] : null;
        }

        [Display(AutoGenerateField = false)]
        public bool HasErrors
        {
            get { return (this._errors.Count > 0); }
        }
        #endregion


        #region DataAnnotation サポート

        /// <summary>
        /// 
        /// </summary>
        /// <param name="value"></param>
        /// <param name="propertyName"></param>
        /// <returns></returns>
        protected bool IsValid(object value, string propertyName)
        {
            //	検証結果用のICollection
            List<ValidationResult> errors = new List<ValidationResult>();
            // 検証用のコンテキスト
            ValidationContext context = new ValidationContext(this, null, null)
            {
                MemberName = propertyName,
            };
            // データ注釈による検証
            bool isValid = Validator.TryValidateProperty(value, context, errors);

            // 既存のメッセージを削除
            IEnumerable<string> messages = this.GetErrors(propertyName) as IEnumerable<string>;
            if (messages != null)
            {
                foreach (string message in messages)
                    this.RemoveError(propertyName, message);
            }

            // 検証結果の追加
            if (errors.Count != 0)
            {
                foreach (ValidationResult error in errors)
                    this.AddError(propertyName, error.ErrorMessage, false);
            }

            return isValid;
        }



        #endregion



    }
}
