using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using twForm ;
using System.Drawing.Drawing2D; 

namespace twFormDemo
{
    public partial class MainForm : twForm.twForm
    {
        TouchableControl ctrl;
        
        public MainForm()
        {
            InitializeComponent();

            this.DoubleBuffered = true;
            this.WindowState = FormWindowState.Maximized;

            GenerateMockObjects();
        }

        private void GenerateMockObjects()
        {
            //ctrl = new TouchableControl();
            //ctrl.Location = new Point(10, 10);
            //ctrl.BackColor = Color.LightSalmon;
            //ctrl.SetAsBitmap(Bitmap.FromFile(@"C:\Documents and Settings\All Users\Documents\My Pictures\Sample Pictures\Blue hills.jpg"));
            //this.Controls.Add(ctrl);

            ctrl = new TouchableControl();
            ctrl.Location = new Point(30, 30);
            ctrl.BackColor = Color.Black;
            ctrl.SetAsBitmap(Bitmap.FromFile(@"C:\Documents and Settings\All Users\Documents\My Pictures\Sample Pictures\Sunset.jpg"));
            this.Controls.Add(ctrl);

            //ctrl = new TouchableControl();
            //ctrl.Location = new Point(0, 0);
            //ctrl.Width = Screen.PrimaryScreen.WorkingArea.Width;
            //ctrl.Height = Screen.PrimaryScreen.WorkingArea.Height;
            //ctrl.BackColor = Color.LightSteelBlue;
            //ctrl.SetAsBitmap(Bitmap.FromFile(@"C:\Documents and Settings\All Users\Documents\My Pictures\Sample Pictures\Water lilies.jpg"));
            //this.Controls.Add(ctrl);
        }

        private void Form1_TouchDown(object sender, TouchEventArgs e)
        {
            //MessageBox.Show("Touch Down");
        }

        private void Form1_TouchMove(object sender, TouchEventArgs e)
        {
            //MessageBox.Show("Touch Move");
        }

        private void Form1_TouchUp(object sender, TouchEventArgs e)
        {
            //MessageBox.Show("Touch Up");
        }
        
        int rot = 0;
        private void btnAction_Click(object sender, EventArgs e)
        {
            rot += 10;
            Matrix m = new Matrix();
            m.Rotate(rot);
      //      ctrl.Transform(m);
            ctrl.Rotate(10);
            ctrl.rot = rot;
        }
    }
}