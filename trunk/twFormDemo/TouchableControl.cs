using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using twForm;

namespace twFormDemo
{
    public enum TouchableContent { Text, Image, Video };

    public partial class TouchableControl : UserControl
    {
        private TouchableContent contentType;
        private bool isMouseDown = false;
        private Point prevMouseLocation;
        private Bitmap bmp;
        private GraphicsPath path;

        public int rot = 0;
        public float transX = 0;
        public float transY = 0;
        public TouchableContent ContentType
        {
            get { return contentType; }
            set { contentType = value; }
        }

        public TouchableControl()
        {
            InitializeComponent();
            //this.DoubleBuffered = true;
        }

        private void SetControlPath()
        {
            Point[] points = new Point[] { 
            new Point(ClientRectangle.Left, ClientRectangle.Top),
            new Point(ClientRectangle.Left + Width, ClientRectangle.Top),
            new Point(ClientRectangle.Left + Width, ClientRectangle.Top + Height),
            new Point(ClientRectangle.Left, ClientRectangle.Top + Height) };
            path = new GraphicsPath();
            path.StartFigure();
            path.AddPolygon(points);
            path.CloseFigure();
        }

        public void SetAsBitmap(Image bmp)
        {
            contentType = TouchableContent.Image;
            this.bmp = bmp as Bitmap;
            this.Width = bmp.Width + 10;
            this.Height = bmp.Height + 10;
            //this.BackColor = Color.Black;
            SetControlPath();
            this.Region = new Region(path);
        }
        public void Transform(Matrix matrix)
        {
            path.Transform(matrix);
            matrix = new Matrix();
            transX = -path.GetBounds().Left;
            transY = -path.GetBounds().Top;
            matrix.Translate(transX, transY);
            path.Transform(matrix);

            this.Width = (int) path.GetBounds().Width;
            this.Height = (int) path.GetBounds().Height;

            System.Drawing.Region reg = new Region(path);
            this.Region = reg;
            this.ParentForm.Text = path.GetBounds().Left + " " + path.GetBounds().Top;
        }
        public void Rotate(int angle)
        {
            System.Drawing.Region reg = this.Region.Clone();
            int oldW = this.Width;
            int oldH = this.Height;

            Matrix m = new Matrix();
            m.Rotate(angle);
            reg.Transform(m);
            m = new Matrix();
            transX = -1 * reg.GetBounds(Graphics.FromHwnd(this.Handle)).Left;
            transY = -1 * reg.GetBounds(Graphics.FromHwnd(this.Handle)).Top;
            m.Translate(transX, transY);
            reg.Transform(m);
            
            this.Region = reg;
            this.Width = (int)reg.GetBounds(Graphics.FromHwnd(this.Handle)).Width;
            this.Height = (int)reg.GetBounds(Graphics.FromHwnd(this.Handle)).Height;
            this.Left -= (this.Width - oldW) / 2;
            this.Top -= (this.Height - oldH) / 2;
            transX = (this.Width - oldW) / 2;
            transY = (this.Height - oldH) / 2;
            
            this.Invalidate();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Pen pen = new Pen(Color.Black, 2);

            e.Graphics.RotateTransform(rot);
            //e.Graphics.TranslateTransform(+transX/4, +transY/4);
            //base.OnPaint(e);

            switch (contentType)
            {
                case TouchableContent.Image:
                    e.Graphics.DrawImage(bmp, 5, 5);
                    break;
            }

            //Border
            //e.Graphics.DrawPath(pen, path);
            
            //e.Graphics.FillPath(new SolidBrush(Color.Red), path);
        }

        public event TouchEventHandler TouchDown;
        public event TouchEventHandler TouchUp;
        public event TouchEventHandler TouchMove;

        protected virtual bool OnTouchDown(object sender, TouchEventArgs e)
        {
            MouseEventArgs args = new MouseEventArgs(MouseButtons.Left, 1, e.point.X, e.point.Y, 0);
            TouchableControl_MouseDown(this, args);
            return true;
        }
        protected virtual bool OnTouchUp(object sender, TouchEventArgs e)
        {
            MouseEventArgs args = new MouseEventArgs(MouseButtons.Left, 1, e.point.X, e.point.Y, 0);
            TouchableControl_MouseDown(this, args);
            return true;
        }
        protected virtual bool OnTouchMove(object sender, TouchEventArgs e)
        {
            MouseEventArgs args = new MouseEventArgs(MouseButtons.Left, 1, e.point.X, e.point.Y, 0);
            TouchableControl_MouseDown(this, args);
            return true;
        }

        protected override void WndProc(ref Message m)
        {
            TouchEventArgs e; // This event should be filled
            e = new TouchEventArgs(m.LParam.ToInt32(), m.WParam.ToInt32());
            bool flag = true;
            if ((int)TWMessagesType.WM_TOUCHDOWN == m.Msg)
            {
                if (OnTouchDown(this, e))
                {
                    //TouchDown(this, e);
                    base.WndProc(ref m);
                }
                else
                    flag = false;
            }
            else if ((int)TWMessagesType.WM_TOUCHMOVE == m.Msg)
            {
                if (OnTouchMove(this, e))
                {
                    //TouchMove(this, e);
                    base.WndProc(ref m);
                }
                else
                    flag = false;
            }
            else if ((int)TWMessagesType.WM_TOUCHUP == m.Msg)
            {
                if (OnTouchUp(this, e))
                {
                    //TouchUp(this, e);
                    base.WndProc(ref m);
                }
                else
                    flag = false;
            }

            if (flag)
                base.WndProc(ref m);


        }

        #region MouseEvents
        private void TouchableControl_MouseDown(object sender, MouseEventArgs e)
        {
            this.BringToFront();
            isMouseDown = true;
            prevMouseLocation = Control.MousePosition;
        }
        private void TouchableControl_MouseMove(object sender, MouseEventArgs e)
        {
            if (isMouseDown)
            {
                int xDiff = Control.MousePosition.X - prevMouseLocation.X;
                int yDiff = Control.MousePosition.Y - prevMouseLocation.Y;
                this.Location = new Point(this.Location.X + xDiff, this.Location.Y + yDiff);
                prevMouseLocation = Control.MousePosition;
            }
        }
        private void TouchableControl_MouseUp(object sender, MouseEventArgs e)
        {
            isMouseDown = false;
        }
        #endregion
    }
}
