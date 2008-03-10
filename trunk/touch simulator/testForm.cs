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
		}


		#region ITouchable Members

		bool ITouchable.OnTouchDown(object sender, TouchEventArgs e)
		{
			using (Graphics g = this.CreateGraphics())
			using (Font font = new Font("Tahoma", 10))
			using (Brush brush = new SolidBrush(Color.Black))
			{
				g.DrawString("DN", font, brush, e.point);
			}
			return false;
		}

		bool ITouchable.OnTouchUp(object sender, TouchEventArgs e)
		{
			using (Graphics g = this.CreateGraphics())
			using (Font font = new Font("Tahoma", 10))
			using (Brush brush = new SolidBrush(Color.Black))
			{
				g.DrawString("Up", font, brush, e.point);
			}
			return false;
		}

		bool ITouchable.OnTouchMove(object sender, TouchEventArgs e)
		{
			using (Graphics g = this.CreateGraphics())
			using (Font font = new Font("Tahoma", 10))
			using (Brush brush = new SolidBrush(Color.Black))
			{
				g.DrawString("MV", font, brush, e.point);
			}
			return false;
		}

		#endregion
	}
}