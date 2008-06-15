using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using TouchworkSDK;

namespace touch_simulator
{
	public partial class testForm :Form,ITouchable
	{
		public testForm()
		{
			InitializeComponent();
			Touchwork.NotifyOnTouch(this);
			Touchwork.NotifyOnTouch(pictureBox1,this);
		}


		#region ITouchable Members

		void ITouchable.OnTouchDown(object sender, TouchEventArgs e)
		{
// 			Control ctrl = sender as Control;
// 			using (Graphics g = ctrl.CreateGraphics())
// 			using (Font font = new Font("Tahoma", 10))
// 			using (Brush brush = new SolidBrush(Color.Black))
// 			{
// 				g.DrawString("DN", font, brush, e.point);
// 			}
		}

		void ITouchable.OnTouchUp(object sender, TouchEventArgs e)
		{
// 			Control ctrl = sender as Control;
// 			using (Graphics g = ctrl.CreateGraphics())
// 			using (Font font = new Font("Tahoma", 10))
// 			using (Brush brush = new SolidBrush(Color.Black))
// 			{
// 				g.DrawString("Up", font, brush, e.point);
// 			}
		}

		void ITouchable.OnTouchMove(object sender, TouchEventArgs e)
		{
// 			Control ctrl = sender as Control;
// 			using (Graphics g = ctrl.CreateGraphics())
// 			using (Font font = new Font("Tahoma", 10))
// 			using (Brush brush = new SolidBrush(Color.Black))
// 			{
// 				g.DrawString("MV", font, brush, e.point);
// 			}
		}

		#endregion

		private void testForm_MouseDown(object sender, MouseEventArgs e)
		{
			Control ctrl = sender as Control;
			using (Graphics g = ctrl.CreateGraphics())
			using (Font font = new Font("Tahoma", 10))
			using (Brush brush = new SolidBrush(Color.Black))
			{
				g.DrawString("dn", font, brush, e.X,e.Y);
			}
		}

		private void testForm_MouseMove(object sender, MouseEventArgs e)
		{
			Control ctrl = sender as Control;
			using (Graphics g = ctrl.CreateGraphics())
			using (Font font = new Font("Tahoma", 10))
			using (Brush brush = new SolidBrush(Color.Black))
			{
				g.DrawString("mv", font, brush, e.X,e.Y);
			}
		}

		private void testForm_MouseUp(object sender, MouseEventArgs e)
		{
			Control ctrl = sender as Control;
			using (Graphics g = ctrl.CreateGraphics())
			using (Font font = new Font("Tahoma", 10))
			using (Brush brush = new SolidBrush(Color.Black))
			{
				g.DrawString("up", font, brush, e.X,e.Y);
			}
		}

	}
}