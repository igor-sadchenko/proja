using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace TouchworkSDK
{
    public delegate void TouchEventHandler(Object sender, TouchEventArgs e);
   
    public partial class TouchworkForm : Form
    {
		public TouchworkForm()
        {
            InitializeComponent();
        }

        public event TouchEventHandler TouchDown;
        public event TouchEventHandler TouchUp;
        public event TouchEventHandler TouchMove;

        protected virtual bool OnTouchDown()
        {
            return true;
        }
        protected virtual bool OnTouchUp()
        {
            return true; 
        }
        protected virtual bool OnTouchMove()
        {
            return true;
        }
        
        //virtual bool OnTouch(Message m)
        //{
        //    TouchEventArgs e;
        //    if (m.Msg == 5000)
        //    {
        //        // e.Id = .............
        //        OnTouchDown(e); 
        //    }
        //    return true;
        //}
        protected override void WndProc(ref Message m)
        {
            TouchEventArgs e; // This event should be filled
            bool flag = true;
            
            if (  (int)TouchMessage.WM_TOUCH_DOWN ==  m.Msg )
            {
                if (OnTouchDown())
                {
					e = new TouchEventArgs((uint)m.LParam.ToInt32(), (uint)m.WParam.ToInt32()); 
                    TouchDown(this, e);
                    base.WndProc(ref m);
                }
                else
                    flag = false; 
            }
            else if ( (int)TouchMessage.WM_TOUCH_MOVE == m.Msg )
            {
                if (OnTouchMove())
                {
					e = new TouchEventArgs((uint)m.LParam.ToInt32(), (uint)m.WParam.ToInt32()); 
                    TouchMove(this, e);
                    base.WndProc(ref m);
                }
                else
                    flag = false; 
            }
            else if ((int)TouchMessage.WM_TOUCH_UP == m.Msg )
            {
                if (OnTouchUp())
                {
					e = new TouchEventArgs((uint)m.LParam.ToInt32(), (uint)m.WParam.ToInt32()); 
                    TouchUp(this, e);
                    base.WndProc(ref m);
                }
                else
                    flag = false; 
            }

            if (flag)
                base.WndProc(ref m);  

            
        }
        

    }
}