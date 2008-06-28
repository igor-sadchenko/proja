using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Collections ;

namespace TouchworkSDK
{
	public sealed class Touchwork
	{
        //Constructor
        public Touchwork()
        {
            TouchMessage.InitializeTouchMessages(); 
        }
        /// <summary>
        /// NotifyOnTouch Allows a given ITouchable control to recieve touch events.
        /// </summary>
        /// <param name="touchableControl"></param>
		static public void NotifyOnTouch(ITouchable touchableControl)
		{
			WndProcHooker.HookWndProc(touchableControl);
		}
        static public void NotifyOnTouch(Control control, ITouchable callback)
        {
            WndProcHooker.HookWndProc(control, callback);
        }

        /// <summary>
        /// UnsubscribeToTouchEvents prevent a control from receiving touch events.
        /// </summary>
        /// <param name="ctrl"></param>
		static public  void UnsubscribeToTouchEvents(Control ctrl)
		{
			WndProcHooker.UnhookWndProc(ctrl,false);
		}

		static public int TouchworkWindowProc(
			ITouchable touchable, Control ctrl,IntPtr hwnd, uint msg, uint wParam, uint lParam, ref bool handled, List<Finger> mFingers)
		{
			TouchEventArgs e; // This event should be filled
            Finger finger; 
            if (msg == TouchMessage.WM_TOUCH_DOWN)
            {
                e = new TouchEventArgs(lParam, wParam);
                touchable.OnTouchDown(ctrl, e);
                handled = e.handled;
                finger = new Finger(e.ID, e.point);
                mFingers.Add(finger); 
                
            }
            else if (msg == TouchMessage.WM_TOUCH_MOVE)
            {
                e = new TouchEventArgs(lParam, wParam);
                touchable.OnTouchMove(ctrl, e);
                handled = e.handled;
                for (int i = 0; i < mFingers.Count; i++)
                {
                    if (mFingers[i].mID == e.ID)
                    {
                        mFingers[i].mOldPosition = mFingers[i].mPosition;
                        mFingers[i].mPosition = e.point; 
                        break;
                    }
                }
                
            }
            else if (msg == TouchMessage.WM_TOUCH_UP)
            {
                e = new TouchEventArgs(lParam, wParam);
                touchable.OnTouchUp(ctrl, e);
                handled = e.handled;
                int i;
                for(i=0 ; i< mFingers.Count ; i++ )
                {
                    if (mFingers[i].mID == e.ID)
                        break; 
                }
                mFingers.RemoveAt(i); 

            }
			return 0;

		}

        /// <summary>
        /// Get a Control and return a List of the fingers on that control
        /// </summary>
        /// <param name="ctrl"></param>
        /// <returns></returns>
        static public List<Finger> GetFingersOnControl(Control ctrl)
        {
            return ((WndProcHooker.HookedProcInformation)WndProcHooker.GetInfoForControl(ctrl)).mFingers; 
        }
	}
}