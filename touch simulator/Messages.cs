using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Drawing;

namespace touch_simulator
{

	public struct TWMessagesType
	{
		public static uint WM_NONE = 0;
		public static uint WM_TOUCH_FIRST = 0x8000;
        public static uint WM_TOUCHUP;
		public static uint WM_TOUCHDOWN;
		public static uint WM_TOUCHMOVE;
		//------
		public static uint WM_MOUSE_FIRST = 0x0200;
		public static uint WM_MOUSEMOVE = 0x0200;
		public static uint WM_LBUTTONDOWN = 0x0201;
		public static uint WM_LBUTTONUP = 0x0202;
        public static uint WM_MOUSE_LAST = 0x0202;

        public static void InitializeTouchMessages()
        {
            WM_TOUCHUP = Messages.RegisterWindowMessage("WM_TOUCH_UP");
            WM_TOUCHDOWN = Messages.RegisterWindowMessage("WM_TOUCH_DOWN");
            WM_TOUCHMOVE = Messages.RegisterWindowMessage("WM_TOUCH_MOVE");
        }
	}

	public class Messages
	{
        
		public static bool send_Mouse_Messages = false;  // change the message box if u changed this or add a loader
        public static bool send_Screen_Coordinates = false;  // change the message box if u changed this or add a loader
		public static int targetWindow = 0;

		public static uint TouchToMouse(uint msg)
		{
			uint ret = TWMessagesType.WM_NONE;
			if(msg == TWMessagesType.WM_TOUCHDOWN)
			{
					ret = TWMessagesType.WM_LBUTTONDOWN;
			}
			else
			if(msg == TWMessagesType.WM_TOUCHMOVE)
			{
					ret = TWMessagesType.WM_MOUSEMOVE;
			}
			else
			if(msg == TWMessagesType.WM_TOUCHUP)
			{
					ret = TWMessagesType.WM_LBUTTONUP;
			}
			
			return ret;
		}

		public static void SendToNextWindow(Form sender,Control pointContainer ,Blob blob)
		{
			if (blob.type >= TWMessagesType.WM_TOUCH_FIRST )
			{
				//find the window to send to .. mostly obsecured by the simulator
				//here i can hide the simulator .. query for window at point .. send .. then unhide the simulator
				IntPtr HWnd;
                POINT ScreenPos = new POINT(blob.center.X, blob.center.Y);
                ClientToScreen(sender.Handle, ref ScreenPos);

				if (Messages.targetWindow == 0)
					HWnd = GetWindow(sender.Handle, (uint)GetWindow_Cmd.GW_HWNDNEXT);
				else
					HWnd = (IntPtr)Messages.targetWindow;
				//map the points
				POINT p = new POINT(blob.center.X, blob.center.Y);

                if (send_Screen_Coordinates)
                    ClientToScreen(sender.Handle, ref p);
                else
                    MapWindowPoints(sender.Handle, HWnd, ref p, 1);

                uint Msg = (uint)blob.type;
                uint LParam = (uint)((p.Y << 16) | (p.X & 0x0000FFFF));
                uint WParam = (uint)((blob.Pressure << 16) | (blob.id & 0x0000FFFF));

                Win32.SetCursorPos(ScreenPos.X, ScreenPos.Y);
                PostMessage(HWnd, Msg, WParam, LParam);

				if (send_Mouse_Messages)
				{
					uint WParam2  = 0;
					if(blob.type == TWMessagesType.WM_TOUCHMOVE)
						WParam2 = 0x0001;
					uint mouse_msg = TouchToMouse(blob.type);
					PostMessage(HWnd, mouse_msg, WParam2, LParam);
				}
			}
		}

		public static IntPtr SendToChildWindows(Control sender, Blob blob, POINT previousPosition)
		{
			if (blob.type >= TWMessagesType.WM_TOUCH_FIRST)
			{
				//find the window to send to .. mostly obsecured by the simulator
				//here i can hide the simulator -- done by the caller
				//.. query for window at point .. send .. 
				IntPtr HWnd ;
                POINT ScreenPos = new POINT(blob.center.X, blob.center.Y);
                ClientToScreen(sender.Handle, ref ScreenPos);

				if (Messages.targetWindow == 0)
				{
					HWnd = WindowFromPoint(ScreenPos);
				}
				else
					HWnd = (IntPtr)Messages.targetWindow;

                StringBuilder strBuilder = new StringBuilder();
                GetWindowText(HWnd, strBuilder, 200);
                Form1.s_txtMonitor.AppendText(HWnd + " - "+ strBuilder.ToString() +"\r\n");

				//map the points
				POINT p = new POINT(blob.center.X, blob.center.Y);

                if(send_Screen_Coordinates)
                    ClientToScreen(sender.Handle, ref p);
                else
                    MapWindowPoints(sender.Handle, HWnd, ref p, 1);

				uint Msg = (uint)blob.type;
				uint LParam = (uint)((p.Y << 16) | (p.X & 0x0000FFFF));
				uint WParam = (uint)((blob.Pressure << 16) | (blob.id & 0x0000FFFF));
                
                Win32.SetCursorPos(ScreenPos.X, ScreenPos.Y);
				PostMessage(HWnd, Msg, WParam, LParam);
				if(send_Mouse_Messages)
                {
                    uint WParam2 = 0;

                    //Modified for rigid.. send fake down in case of drag & up
                    if (blob.type == TWMessagesType.WM_TOUCHMOVE)
                    {
                        p = new POINT(previousPosition.X, previousPosition.Y);
                        if (p.X - blob.center.X != 0 || p.Y - blob.center.Y != 0)
                        {
                            ClientToScreen(sender.Handle, ref p);
                            uint FakeLParam = (uint)((p.Y << 16) | (p.X & 0x0000FFFF));
                            Win32.SetCursorPos(p.X, p.Y);
                            PostMessage(HWnd, TWMessagesType.WM_LBUTTONDOWN, WParam2, FakeLParam);
                            WParam2 = 0x0001;
                        }
                    }
                    else if (blob.type == TWMessagesType.WM_TOUCHUP)
                    {
                        PostMessage(HWnd, TWMessagesType.WM_LBUTTONDOWN, WParam2, LParam);
                    }

                    uint mouse_msg = TouchToMouse(blob.type);
                    PostMessage(HWnd, mouse_msg, WParam2, LParam);
                }
                //Win32.ReleaseCapture();
                return HWnd;
			}
            return IntPtr.Zero;
        }

        #region User32 DLL Importing
        [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = true)]
		static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);
		
		[DllImport("user32.dll", SetLastError = true)]
		public extern static int SendMessage(IntPtr hwnd, uint msg, uint wParam, uint lParam);

		[return: MarshalAs(UnmanagedType.Bool)]
		[DllImport("user32.dll", SetLastError = true)]
		static extern bool PostMessage(IntPtr hWnd, uint Msg, uint wParam,uint lParam);

		[DllImport("user32.dll", SetLastError = true)]
		public extern static IntPtr GetNextWindow(IntPtr hwnd, uint wCmd);

		[DllImport("user32.dll", SetLastError = true)]
		static extern IntPtr GetWindow(IntPtr hWnd, uint uCmd);

		[DllImport("user32.dll")]
		static extern IntPtr WindowFromPoint(POINT Point);

		[DllImport("user32.dll")]
		static extern bool ClientToScreen(IntPtr hWnd, ref POINT lpPoint);

        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern uint RegisterWindowMessage(string lpString);

		[DllImport("user32.dll", SetLastError = true)]
		public static extern int MapWindowPoints(IntPtr hwndFrom, IntPtr hwndTo, ref POINT lpPoints, [MarshalAs(UnmanagedType.U4)] int cPoints);

		[StructLayout(LayoutKind.Sequential)]
		public struct POINT
		{
			public int X;
			public int Y;

			public POINT(int x, int y)
			{
				this.X = x;
				this.Y = y;
			}

			public static implicit operator System.Drawing.Point(POINT p)
			{
				return new System.Drawing.Point(p.X, p.Y);
			}

			public static implicit operator POINT(System.Drawing.Point p)
			{
				return new POINT(p.X, p.Y);
			}
		}

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
        #endregion
    }
}
