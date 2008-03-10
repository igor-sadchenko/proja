using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
namespace TouchworkSDK
{
	public class Touchwork
	{
		static public void NotifyOnTouch(ITouchable control)
		{
			WndProcHooker.HookWndProc(control);
		}


		static public int TouchworkWindowProc(
			ITouchable touchable, IntPtr hwnd, uint msg, uint wParam, uint lParam, ref bool handled)
		{
			TouchEventArgs e; // This event should be filled
			switch ((TouchMessage)msg)
			{
				case TouchMessage.WM_TOUCHDOWN:
					e = new TouchEventArgs(lParam, wParam);
					handled = touchable.OnTouchDown(touchable, e);
					break;
				case TouchMessage.WM_TOUCHMOVE:
					e = new TouchEventArgs(lParam, wParam);
					handled = touchable.OnTouchMove(touchable, e);
					break;
				case TouchMessage.WM_TOUCHUP:
					e = new TouchEventArgs(lParam, wParam);
					handled = touchable.OnTouchUp(touchable, e);
					break;

			}
			return 0;

		}
	}
}