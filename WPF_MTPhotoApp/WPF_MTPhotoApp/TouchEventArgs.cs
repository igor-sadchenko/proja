using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;

namespace NUIGroup.MultiTouchFramework
{
    #region toucheventargs class
    /// <summary>
    /// Custon Event Arguments class for use in Multitouch Event handlers
    /// </summary>
    public class TouchEventArgs : RoutedEventArgs
    {

        private TouchData _data;


        public TouchEventArgs(TouchData data)
        {
            this._data = data;
        }

        public TouchData data
        {
            get { return _data; }
        }


    }
    #endregion
}
