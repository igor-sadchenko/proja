using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace twFormDemo
{
    public partial class TestTouchableControl : TouchableControl
    {
        GraphicsPath path;
        System.Drawing.Region region;
        Form parentForm;

        public TestTouchableControl(Form parentForm)
        {
            InitializeComponent();
            this.parentForm = parentForm;
            this.BackColor = Color.SteelBlue;

            Width = parentForm.Width - 20;
            Height = parentForm.Height - 20;

            path = new GraphicsPath();
            path.StartFigure();
            path.AddLine(0, 0, 100, 0);
            path.AddLine(100, 0, 150, 50);
            path.AddLine(150, 50, 50, 200);
            path.CloseFigure();

            Matrix m = new Matrix();
            m.Rotate(465);
            path.Transform(m);
            parentForm.Text = path.GetBounds().Left + " " + path.GetBounds().Top;

            m = new Matrix();
            m.Translate(-path.GetBounds().Left, -path.GetBounds().Top);
            path.Transform(m);
            parentForm.Text += path.GetBounds().Left + " " + path.GetBounds().Top;

            region = new Region(path);

            this.Region = region;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            e.Graphics.FillPath(new SolidBrush(Color.Red), path);
        }
    }
}
