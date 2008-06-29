using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace TouchworkSDK
{
	internal class WndProcHooker
	{
		/// <summary>
		/// The callback used when a hooked window's message map contains the
		/// hooked message
		/// </summary>
		/// <param name="hwnd">The handle to the window for which the message
		/// was received</param>
		/// <param name="wParam">The message's parameters (part 1)</param>
		/// <param name="lParam">The message's parameters (part 2)</param>
		/// <param name="handled">The invoked function sets this to true if it
		/// handled the message. If the value is false when the callback
		/// returns, the next window procedure in the wndproc chain is
		/// called</param>
		/// <returns>A value specified for the given message in the MSDN
		/// documentation</returns>
		public delegate int WndProcCallback(
			IntPtr hwnd, uint msg, uint wParam, uint lParam, ref bool handled);

		/// <summary>
		/// This is the global list of all the window procedures we have
		/// hooked. The key is an hwnd. The value is a HookedProcInformation
		/// object which contains a pointer to the old wndproc and a map of
		/// messages/callbacks for the window specified. Controls whose handles
		/// have been created go into this dictionary.
		/// </summary>
		private static Dictionary<IntPtr, HookedProcInformation> hwndDict =
			new Dictionary<IntPtr, HookedProcInformation>();
		/// <summary>
		/// See <see>hwndDict</see>. The key is a control and the value is a
		/// HookedProcInformation. Controls whose handles have not been created
		/// go into this dictionary. When the HandleCreated event for the
		/// control is fired the control is moved into <see>hwndDict</see>.
		/// </summary>
		private static Dictionary<Control, HookedProcInformation> ctlDict =
			new Dictionary<Control, HookedProcInformation>();


        /// <summary>
        /// Makes a connection between a message on a specified window handle
        /// and the callback to be called when that message is received. If the
        /// window was not previously hooked it is added to the global list of
        /// all the window procedures hooked.
        /// </summary>
        /// <param name="ctl"></param>
		public static void HookWndProc(
			ITouchable ctl)
		{
			HookWndProc((Control)ctl, (ITouchable)ctl);
		}

		/// <summary>
		/// Makes a connection between a message on a specified window handle
		/// and the callback to be called when that message is received. If the
		/// window was not previously hooked it is added to the global list of
		/// all the window procedures hooked.
		/// </summary>
		/// <param name="ctl">The control whose wndproc we are hooking</param>
		/// <param name="callback">The method to call when the specified
		/// message is received for the specified window</param>
		/// <param name="msg">The message we are hooking.</param>
		public static void HookWndProc(
			Control ctl, ITouchable callback)
		{
			HookedProcInformation hpi = null;
			if (ctlDict.ContainsKey(ctl))
				hpi = ctlDict[ctl];
			else if (hwndDict.ContainsKey(ctl.Handle))
				hpi = hwndDict[ctl.Handle];
			if (hpi == null)
			{
				// We havne't seen this control before. Create a new
				// HookedProcInformation for it
				hpi = new HookedProcInformation(ctl,
					new Win32.WndProc(WndProcHooker.WindowProc),callback);
				ctl.HandleCreated += new EventHandler(ctl_HandleCreated);
				ctl.HandleDestroyed += new EventHandler(ctl_HandleDestroyed);
				ctl.Disposed += new EventHandler(ctl_Disposed);

				// If the handle has already been created set the hook. If it
				// hasn't been created yet, the hook will get set in the
				// ctl_HandleCreated event handler
				if (ctl.Handle != IntPtr.Zero)
					hpi.SetHook();
			}

			// stick hpi into the correct dictionary
			if (ctl.Handle == IntPtr.Zero)
				ctlDict[ctl] = hpi;
			else
				hwndDict[ctl.Handle] = hpi;
		}

		/// <summary>
		/// The event handler called when a control is disposed.
		/// </summary>
		/// <param name="sender">The object that raised this event</param>
		/// <param name="e">The arguments for this event</param>
		static void ctl_Disposed(object sender, EventArgs e)
		{
			Control ctl = sender as Control;
			if (ctlDict.ContainsKey(ctl))
				ctlDict.Remove(ctl);
			else
				System.Diagnostics.Debug.Assert(false);
		}

		/// <summary>
		/// The event handler called when a control's handle is destroyed.
		/// We remove the HookedProcInformation from <see>hwndDict</see> and
		/// put it back into <see>ctlDict</see> in case the control get re-
		/// created and we still want to hook its messages.
		/// </summary>
		/// <param name="sender">The object that raised this event</param>
		/// <param name="e">The arguments for this event</param>
		static void ctl_HandleDestroyed(object sender, EventArgs e)
		{
			// When the handle for a control is destroyed, we want to
			// unhook its wndproc and update our lists
			Control ctl = sender as Control;
			if (hwndDict.ContainsKey(ctl.Handle))
			{
				HookedProcInformation hpi = hwndDict[ctl.Handle];
				UnhookWndProc(ctl, false);
			}
			else
				System.Diagnostics.Debug.Assert(false);
		}

		/// <summary>
		/// The event handler called when a control's handle is created. We
		/// call SetHook() on the associated HookedProcInformation object and
		/// move it from <see>ctlDict</see> to <see>hwndDict</see>.
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		static void ctl_HandleCreated(object sender, EventArgs e)
		{
			Control ctl = sender as Control;
			if (ctlDict.ContainsKey(ctl))
			{
				HookedProcInformation hpi = ctlDict[ctl];
				hwndDict[ctl.Handle] = hpi;
				ctlDict.Remove(ctl);
				hpi.SetHook();
			}
			else
				System.Diagnostics.Debug.Assert(false);
		}

		/// <summary>
		/// This is a generic wndproc. It is the callback for all hooked
		/// windows. If we get into this function, we look up the hwnd in the
		/// global list of all hooked windows to get its message map. If the
		/// message received is present in the message map, its callback is
		/// invoked with the parameters listed here.
		/// </summary>
		/// <param name="hwnd">The handle to the window that received the
		/// message</param>
		/// <param name="msg">The message</param>
		/// <param name="wParam">The message's parameters (part 1)</param>
		/// <param name="lParam">The messages's parameters (part 2)</param>
		/// <returns>If the callback handled the message, the callback's return
		/// value is returned form this function. If the callback didn't handle
		/// the message, the message is forwarded on to the previous wndproc.
		/// </returns>
		private static int WindowProc(
			IntPtr hwnd, uint msg, uint wParam, uint lParam)
		{
			if (hwndDict.ContainsKey(hwnd))
			{
				HookedProcInformation hpi = hwndDict[hwnd];
				bool handled = false;
				int retval = Touchwork.TouchworkWindowProc(hpi.touchable,hpi.Control,hwnd,msg,wParam,lParam,ref handled, hpi.mTouches);
				if (handled)
					return retval;

				// if we didn't hook the message passed or we did, but the
				// callback didn't set the handled property to true, call
				// the original window procedure
				return hpi.CallOldWindowProc(hwnd, msg, wParam, lParam);
			}
			System.Diagnostics.Debug.Assert(
				false, "Touchwork recieved a message for a hwnd we don't know about");
			return Win32.DefWindowProc(hwnd, msg, wParam, lParam);
		}

		/// <summary>
		/// Restores the previous wndproc for the specified window.
		/// </summary>
		/// <param name="ctl">The control whose wndproc we no longer want to
		/// hook</param>
		/// <param name="disposing">if true we remove don't readd the
		/// HookedProcInformation
		/// back into ctlDict</param>
		public static void UnhookWndProc(Control ctl, bool disposing)
		{
			HookedProcInformation hpi = null;
			if (ctlDict.ContainsKey(ctl))
				hpi = ctlDict[ctl];
			else if (hwndDict.ContainsKey(ctl.Handle))
				hpi = hwndDict[ctl.Handle];
			if (hpi == null)
				throw new ArgumentException("this control never subscribed to Touch events");

			// If we found our HookedProcInformation in ctlDict and we are
			// disposing remove it from ctlDict
			if (ctlDict.ContainsKey(ctl) && disposing)
				ctlDict.Remove(ctl);

			// If we found our HookedProcInformation in hwndDict, remove it
			// and if we are not disposing stick it in ctlDict
			if (hwndDict.ContainsKey(ctl.Handle))
			{
				hpi.Unhook();
				hwndDict.Remove(ctl.Handle);
				if (!disposing)
					ctlDict[ctl] = hpi;
			}
		}

        /// <summary>
        /// Helper function to get the HookedProInformation of a given Control
        /// </summary>
        /// <param name="ctrl"></param>
        /// <returns></returns>
        public static HookedProcInformation GetInfoForControl(Control ctrl)
        {
            return ctlDict[ctrl]; 
        }

		/// <summary>
		/// This class remembers the old window procedure for the specified
		/// window handle and alsow provides the message map for the messages
		/// hooked on that window.
		/// </summary>
		public class HookedProcInformation
		{
			public ITouchable touchable;
			/// <summary>
			/// The old window procedure for the window
			/// </summary>
			private IntPtr oldWndProc;
			/// <summary>
			/// The delegate that gets called in place of this window's
			/// wndproc.
			/// </summary>
			private Win32.WndProc newWndProc;
			/// <summary>
			/// Control whose wndproc we are hooking
			/// </summary>
			private Control control;

			public Control Control
			{
				get { return control; }
			}

            /// <summary>
            /// The list of all fingers on a given control
            /// </summary>
            public List<Touch> mTouches; 

			/// <summary>
			/// Constructs a new HookedProcInformation object
			/// </summary>
			/// <param name="ctl">The handle to the window being hooked</param>
			/// <param name="wndproc">The window procedure to replace the
			/// original one for the control</param>
			public HookedProcInformation(Control ctl, Win32.WndProc wndproc , ITouchable _touchable)
			{
				control = ctl;
				newWndProc = wndproc;
				touchable = _touchable;
                mTouches = new List<Touch>(); 
			}

			/// <summary>
			/// Replaces the windows procedure for <see>control</see> with the
			/// one specified in the constructor.
			/// </summary>
			public void SetHook()
			{
				IntPtr hwnd = control.Handle;
				if (hwnd == IntPtr.Zero)
					throw new InvalidOperationException(
						"Handle for control has not been created");

				oldWndProc = Win32.SetWindowLong(hwnd, Win32.GWL_WNDPROC,
					Marshal.GetFunctionPointerForDelegate(newWndProc));
			}
			/// <summary>
			/// Restores the original window procedure for the control.
			/// </summary>
			public void Unhook()
			{
				IntPtr hwnd = control.Handle;
				if (hwnd == IntPtr.Zero)
					throw new InvalidOperationException(
						"Handle for control has not been created");

				Win32.SetWindowLong(hwnd, Win32.GWL_WNDPROC, oldWndProc);
			}
			/// <summary>
			/// Calls the original window procedure of the control with the
			/// arguments provided.
			/// </summary>
			/// <param name="hwnd">The handle of the window that received the
			/// message</param>
			/// <param name="msg">The message</param>
			/// <param name="wParam">The message's arguments (part 1)</param>
			/// <param name="lParam">The message's arguments (part 2)</param>
			/// <returns>The value returned by the control's original wndproc
			/// </returns>
			public int CallOldWindowProc(
				IntPtr hwnd, uint msg, uint wParam, uint lParam)
			{
				return Win32.CallWindowProc(
					oldWndProc, hwnd, msg, wParam, lParam);
			}
		}
	}
}
