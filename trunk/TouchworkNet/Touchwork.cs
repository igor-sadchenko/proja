using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace TouchworkSDK
{
	public class Touchwork
	{
        public Touchwork()
        {
            InititializeTouchMessages(); 
        }

        static public void InititializeTouchMessages()
        {
            TouchMessage.WM_TOUCH_UP = Win32.RegisterWindowMessage("WM_TOUCH_UP");
            TouchMessage.WM_TOUCH_DOWN = Win32.RegisterWindowMessage("WM_TOUCH_DOWN");
            TouchMessage.WM_TOUCH_MOVE = Win32.RegisterWindowMessage("WM_TOUCH_MOVE"); 
        }
		static public void NotifyOnTouch(ITouchable touchableControl)
		{
			WndProcHooker.HookWndProc(touchableControl);
		}

		static public  void UnsubscribeToTouchEvents(Control ctrl)
		{
			WndProcHooker.UnhookWndProc(ctrl,false);
		}

		static public void NotifyOnTouch(Control control, ITouchable callback)
		{
			WndProcHooker.HookWndProc(control,callback);
		}
		static public int TouchworkWindowProc(
			ITouchable touchable, Control ctrl,IntPtr hwnd, uint msg, uint wParam, uint lParam, ref bool handled)
		{
			TouchEventArgs e; // This event should be filled
            //switch (msg)
            //{
            //    case TouchMessage.WM_TOUCH_DOWN:
            //        e = new TouchEventArgs(lParam, wParam);
            //        touchable.OnTouchDown(ctrl, e);
            //        handled = e.handled;
            //        break;
            //    case TouchMessage.WM_TOUCH_MOVE:
            //        e = new TouchEventArgs(lParam, wParam);
            //        touchable.OnTouchMove(ctrl, e);
            //        handled = e.handled;
            //        break;
            //    case TouchMessage.WM_TOUCH_UP:
            //        e = new TouchEventArgs(lParam, wParam);
            //        touchable.OnTouchUp(ctrl, e);
            //        handled = e.handled;
            //        break;
            //}
            if (msg == TouchMessage.WM_TOUCH_DOWN)
            {
                e = new TouchEventArgs(lParam, wParam);
                touchable.OnTouchDown(ctrl, e);
                handled = e.handled;
            }
            else if (msg == TouchMessage.WM_TOUCH_MOVE)
            {
                e = new TouchEventArgs(lParam, wParam);
                touchable.OnTouchMove(ctrl, e);
                handled = e.handled;
            }
            else if (msg == TouchMessage.WM_TOUCH_UP)
            {
                e = new TouchEventArgs(lParam, wParam);
                touchable.OnTouchUp(ctrl, e);
                handled = e.handled;
            }
			return 0;

		}
	}
}