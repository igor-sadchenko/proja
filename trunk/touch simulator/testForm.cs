using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using twForm;

namespace touch_simulator
{
	public partial class testForm : twForm.twForm
	{
		public testForm()
		{
			InitializeComponent();
		}

		private void testForm_TouchDown(object sender, TouchEventArgs e)
		{
			using(Graphics g = this.CreateGraphics())
			using(Font font = new Font("Tahoma",10))
			using(Brush brush = new SolidBrush(Color.Black))
			{
				g.DrawString("DN",font,brush,e.point);
			}

		}

		private void testForm_TouchMove(object sender, TouchEventArgs e)
		{
			using (Graphics g = this.CreateGraphics())
			using (Font font = new Font("Tahoma", 10))
			using (Brush brush = new SolidBrush(Color.Black))
			{
				g.DrawString("MV", font, brush, e.point);
			}

		}

		private void testForm_TouchUp(object sender, TouchEventArgs e)
		{
			using (Graphics g = this.CreateGraphics())
			using (Font font = new Font("Tahoma", 10))
			using (Brush brush = new SolidBrush(Color.Black))
			{
				g.DrawString("Up", font, brush, e.point);
			}

		}
	}
}