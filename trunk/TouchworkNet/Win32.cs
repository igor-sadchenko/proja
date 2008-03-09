using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Runtime.InteropServices;

namespace Touchwork
{
	// Contains managed wrappers and implementations of Win32
	// structures, delegates, constants and platform invokes
	// used by the GradientFill and Subclassing samples.

	public sealed class Win32
	{
		// A callback to a Win32 window procedure (wndproc):
		// Parameters:
		//   hwnd - The handle of the window receiving a message.
		//   msg - The message</param>
		//   wParam - The message's parameters (part 1).
		//   lParam - The message's parameters (part 2).
		//  Returns an integer as described for the given message in MSDN.
		public delegate int WndProc(IntPtr hwnd, uint msg, uint wParam, int lParam);

		[DllImport("user32.dll")]
		public extern static IntPtr SetWindowLong(
			IntPtr hwnd, int nIndex, IntPtr dwNewLong);
		public const int GWL_WNDPROC = -4;

		[DllImport("user32.dll")]
		public extern static int CallWindowProc(
			IntPtr lpPrevWndFunc, IntPtr hwnd, uint msg, uint wParam, int lParam);

		[DllImport("user32.dll")]
		public extern static int DefWindowProc(
			IntPtr hwnd, uint msg, uint wParam, int lParam);

		[DllImport("user32.dll", SetLastError = true)]
		public extern static int SendMessage(
			IntPtr hwnd, uint msg, uint wParam, uint lParam);

		// Helper function to convert a Windows lParam into a Point.
		//   lParam - The parameter to convert.
		// Rreturns a Point where X is the low 16 bits and Y is the
		// high 16 bits of the value passed in.
		public static Point LParamToPoint(int lParam)
		{
			uint ulParam = (uint)lParam;
			return new Point(
				(int)(ulParam & 0x0000ffff),
				(int)((ulParam & 0xffff0000) >> 16));
		}

		// Windows messages
		public const uint WM_PAINT = 0x000F;
		public const uint WM_ERASEBKGND = 0x0014;
		public const uint WM_KEYDOWN = 0x0100;
		public const uint WM_KEYUP = 0x0101;
		public const uint WM_MOUSEMOVE = 0x0200;
		public const uint WM_LBUTTONDOWN = 0x0201;
		public const uint WM_LBUTTONUP = 0x0202;
		public const uint WM_NOTIFY = 0x4E;
	}

}
