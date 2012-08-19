using System;
using System.Windows.Input;

namespace Gdf.Mvvm.Prism.Commands
{

    public class DelegateCommand : Microsoft.Practices.Prism.Commands.DelegateCommand
    {
        public DelegateCommand(Action executeMethod):base(executeMethod){}

        public DelegateCommand(Action executeMethod, Func<bool> canExecuteMethod)
            :base(executeMethod, canExecuteMethod) {}

    }

    public class DelegateCommand<T> : Microsoft.Practices.Prism.Commands.DelegateCommand
    {
        public DelegateCommand(Action executeMethod) : base(executeMethod) { }

        public DelegateCommand(Action executeMethod, Func<bool> canExecuteMethod)
            : base(executeMethod, canExecuteMethod) { }

    }


}
