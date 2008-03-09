using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
namespace Touchwork
{
	class Touchwork
	{
		void NotifyOnTouch(ITouchable control)
		{
			//WndProcHooker.HookWndProc(control, OnTouch, Win32.WM_LBUTTONDOWN);

			//WndProcHooker.UnhookWndProc(control, WindowsMessage.TOUCH);
		}


		public int OnTouch(
			IntPtr hwnd, uint msg, uint wParam, int lParam, ref bool handled)
		{
			return -1;//return -1 if not proccesed .. 0 if processed
		}


	}
}