using System;
using System.Collections.Generic;
using System.Text;

namespace TouchworkSDK
{
    public struct TouchMessage
    {
        public static uint WM_TOUCH_UP;
        public static uint WM_TOUCH_DOWN;
        public static uint WM_TOUCH_MOVE;
        public static uint WM_IS_TOUCHABLE;
        public static void InitializeTouchMessages()
        {
            WM_TOUCH_UP = Win32.RegisterWindowMessage("WM_TOUCH_UP");
            WM_TOUCH_DOWN = Win32.RegisterWindowMessage("WM_TOUCH_DOWN");
            WM_TOUCH_MOVE = Win32.RegisterWindowMessage("WM_TOUCH_MOVE");
            WM_IS_TOUCHABLE = Win32.RegisterWindowMessage("WM_IS_TOUCHABLE"); 
        }
    }
}
