using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace TouchworkSDK
{
	public class Touchwork
	{
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
			switch ((TouchMessage)msg)
			{
				case TouchMessage.WM_TOUCHDOWN:
					e = new TouchEventArgs(lParam, wParam);
					touchable.OnTouchDown(ctrl, e);
					handled = e.handled;
					break;
				case TouchMessage.WM_TOUCHMOVE:
					e = new TouchEventArgs(lParam, wParam);
					touchable.OnTouchMove(ctrl, e);
					handled = e.handled;
					break;
				case TouchMessage.WM_TOUCHUP:
					e = new TouchEventArgs(lParam, wParam);
					touchable.OnTouchUp(ctrl, e);
					handled = e.handled;
					break;

			}
			return 0;

		}
	}
}