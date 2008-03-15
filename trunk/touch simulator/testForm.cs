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
			Control ctrl = sender as Control;
			using (Graphics g = ctrl.CreateGraphics())
			using (Font font = new Font("Tahoma", 10))
			using (Brush brush = new SolidBrush(Color.Black))
			{
				g.DrawString("DN", font, brush, e.point);
			}
		}

		void ITouchable.OnTouchUp(object sender, TouchEventArgs e)
		{
			Control ctrl = sender as Control;
			using (Graphics g = ctrl.CreateGraphics())
			using (Font font = new Font("Tahoma", 10))
			using (Brush brush = new SolidBrush(Color.Black))
			{
				g.DrawString("Up", font, brush, e.point);
			}
		}

		void ITouchable.OnTouchMove(object sender, TouchEventArgs e)
		{
			Control ctrl = sender as Control;
			using (Graphics g = ctrl.CreateGraphics())
			using (Font font = new Font("Tahoma", 10))
			using (Brush brush = new SolidBrush(Color.Black))
			{
				g.DrawString("MV", font, brush, e.point);
			}
		}

		#endregion

	}
}