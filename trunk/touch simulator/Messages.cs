using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace touch_simulator
{

	public enum TWMessagesType :uint
    {
        WM_TOUCHUP = 0x8000 ,
        WM_TOUCHDOWN , 
        WM_TOUCHMOVE 
    }
	public class Messages
	{
		public static void SendToNextWindow(Form sender,Blob blob)
		{
			IntPtr HWnd = GetWindow(sender.Handle, (uint)GetWindow_Cmd.GW_HWNDNEXT);
			uint Msg = (uint)blob.type; 	
			uint LParam =(uint)((blob.center.Y<<16) | (blob.center.X& 0x0000FFFF));
			uint WParam = (uint)((1 << 16) | (blob.id & 0x0000FFFF));
			SendMessage(HWnd,Msg,WParam,LParam);
		}

		
		
		[DllImport("user32.dll", SetLastError = true)]
		public extern static int SendMessage(
			IntPtr hwnd, uint msg, uint wParam, uint lParam);

		[DllImport("user32.dll", SetLastError = true)]
		public extern static IntPtr GetNextWindow(
			IntPtr hwnd, uint wCmd);

		[DllImport("user32.dll", SetLastError = true)]
		static extern IntPtr GetWindow(IntPtr hWnd, uint uCmd);

		enum GetWindow_Cmd : uint
		{
			GW_HWNDFIRST = 0,
			GW_HWNDLAST = 1,
			GW_HWNDNEXT = 2,
			GW_HWNDPREV = 3,
			GW_OWNER = 4,
			GW_CHILD = 5,
			GW_ENABLEDPOPUP = 6
		}

	

	}
}
