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
			MessageBox.Show("down");
		}
	}
}