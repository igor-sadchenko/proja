using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices; 

namespace WPF_MTPhotoApp
{
    internal sealed class Win32
    {
        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern uint RegisterWindowMessage(string lpString);
    }
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
