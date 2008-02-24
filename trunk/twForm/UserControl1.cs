using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace twForm
{
    public delegate void TouchEventHandler(Object sender, TouchEventArgs e);
    public enum TWMessagesType 
    {
        WM_TOUCHUP = 0x8000 ,
        WM_TOUCHDOWN , 
        WM_TOUCHMOVE 
    }
    public partial class twForm : Form
    {
        public twForm()
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
            if (  (int)TWMessagesType.WM_TOUCHDOWN ==  m.Msg )
            {
                if (OnTouchDown())
                {
                    e = new TouchEventArgs(m.LParam.ToInt32() , m.WParam.ToInt32()); 
                    TouchDown(this, e);
                    base.WndProc(ref m);
                }
                else
                    flag = false; 
            }
            else if ( (int)TWMessagesType.WM_TOUCHMOVE == m.Msg )
            {
                if (OnTouchMove())
                {
                    e = new TouchEventArgs(m.LParam.ToInt32() , m.WParam.ToInt32() ); 
                    TouchMove(this, e);
                    base.WndProc(ref m);
                }
                else
                    flag = false; 
            }
            else if ((int)TWMessagesType.WM_TOUCHUP == m.Msg )
            {
                if (OnTouchUp())
                {
                    e = new TouchEventArgs(m.LParam.ToInt32() , m.WParam.ToInt32() ); 
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