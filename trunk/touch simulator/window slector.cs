using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace touch_simulator
{
	public partial class window_slector : Form
	{
		public window_slector()
		{
			InitializeComponent();
		}
		public int hwnd;

		private void button1_Click(object sender, EventArgs e)
		{
			int.TryParse(textBox1.Text,out hwnd);
		}
	}
}