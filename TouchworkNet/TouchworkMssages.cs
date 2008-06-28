using System;
using System.Collections.Generic;
using System.Text;

namespace TouchworkSDK
{
    //public enum TouchMessage
    //{
    //    WM_TOUCHUP = 0x8000,
    //    WM_TOUCHDOWN,
    //    WM_TOUCHMOVE
    //}
    public struct TouchMessage
    {
        public static uint WM_TOUCH_UP;
        public static uint WM_TOUCH_DOWN;
        public static uint WM_TOUCH_MOVE;
        public static void InitializeTouchMessages()
        {
            WM_TOUCH_UP = Win32.RegisterWindowMessage("WM_TOUCH_UP");
            WM_TOUCH_DOWN = Win32.RegisterWindowMessage("WM_TOUCH_DOWN");
            WM_TOUCH_MOVE = Win32.RegisterWindowMessage("WM_TOUCH_MOVE");
        }
    }
}
