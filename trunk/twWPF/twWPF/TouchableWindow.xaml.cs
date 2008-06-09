using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Interop;

using TouchworkSDK;
namespace twWPF
{
	/// <summary>
	/// Interaction logic for TouchableWindow.xaml
	/// </summary>
	public partial class TouchableWindow : Window
	{
		System.Windows.Interop.WindowInteropHelper interopHelper;
		HwndSource hwndSource;
		public TouchableWindow()
		{
			InitializeComponent();
			Loaded += new RoutedEventHandler(TouchableWindow_Loaded);
		}

		void TouchableWindow_Loaded(object sender, RoutedEventArgs e)
		{
			interopHelper = new System.Windows.Interop.WindowInteropHelper(this);

			hwndSource = System.Windows.Interop.HwndSource.FromHwnd(interopHelper.Handle);
			hwndSource.AddHook(new System.Windows.Interop.HwndSourceHook(WndProc));
		}

		void OnTouchDown(object sender, TouchEventArgs e)
		{
			InputEventArgs ievent = new InputEventArgs(null,DateTime.Now.Ticks);
			InputManager.Current.ProcessInput(ievent);
		}

		void OnTouchUp(object sender, TouchEventArgs e)
		{

		}

		void OnTouchMove(object sender, TouchEventArgs e)
		{

		}

		public IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
		{
			
			TouchEventArgs e; // This event should be filled
			switch ((TouchMessage)msg)
			{
				case TouchMessage.WM_TOUCHDOWN:
					e = new TouchEventArgs((uint)lParam, (uint)wParam);
					OnTouchDown(null, e);
					handled = e.handled;
					break;
				case TouchMessage.WM_TOUCHMOVE:
					e = new TouchEventArgs((uint)lParam, (uint)wParam);
					OnTouchMove(null, e);
					handled = e.handled;
					break;
				case TouchMessage.WM_TOUCHUP:
					e = new TouchEventArgs((uint)lParam, (uint)wParam);
					OnTouchUp(null, e);
					handled = e.handled;
					break;

			}
			
			return (IntPtr)0;
		}
	}
}
