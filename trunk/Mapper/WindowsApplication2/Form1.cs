using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace WindowsApplication2
{
	public partial class Form1 : Form
	{
		gPoint[] trapozoid = new gPoint[4];
		gPoint[] rectangle = new gPoint[4];

		gPoint point = new gPoint(0,0);
		public Form1()
		{
			
			InitializeComponent();
			trapozoid[0]= new gPoint(40, 100);
			trapozoid[1]= new gPoint(165, 100);
			trapozoid[2] = new gPoint(200,5);
			trapozoid[3] =new gPoint(5,	5);

		}
		
		private void pictureBox1_Paint(object sender, PaintEventArgs e)
		{
			int r = 3;
			Graphics g = e.Graphics;
			Pen pen = new Pen(Color.Black);
			g.DrawLine(pen, trapozoid[0].ToPoint(), trapozoid[1].ToPoint());
			g.DrawLine(pen, trapozoid[1].ToPoint(), trapozoid[2].ToPoint());
			g.DrawLine(pen, trapozoid[2].ToPoint(), trapozoid[3].ToPoint());
			g.DrawLine(pen, trapozoid[3].ToPoint(), trapozoid[0].ToPoint());
			e.Graphics.DrawRectangle(pen, point.X - r, point.Y - r, 2 * r, 2 * r);
			pen.Dispose();
		}

		private void pictureBox2_Paint(object sender, PaintEventArgs e)
		{

			gLine left = new gLine(trapozoid[0], trapozoid[3]);
			gLine right = new gLine(trapozoid[1], trapozoid[2]);
			gLine top = new gLine(trapozoid[0], trapozoid[1]);
			gLine bottom = new gLine(trapozoid[3], trapozoid[2]);
			gPoint leftPoint = new gPoint(point.X - 5,point.Y);
			gPoint topPoint = new gPoint(point.X ,point.Y- 5);
			gLine horizental = new gLine( leftPoint, point);
			gLine vertical = new gLine(topPoint, point);

			gPoint u1 = left.GetIntersection(horizental);
			gPoint u2 = right.GetIntersection(horizental);
			float u = (point.X - u1.X) / (u2.X - u1.X);


			gPoint v1 = bottom.GetIntersection(vertical);
			gPoint v2 = top.GetIntersection(vertical);
			float v = (point.Y - v1.Y) / (v2.Y - v1.Y);

			Rectangle rect = new Rectangle(10,10,200,100);
			float x = rect.Left + rect.Width * u;
			float y = rect.Bottom - rect.Height * v;

			int r = 3;
			Pen pen = new Pen(Color.Black);
			e.Graphics.DrawRectangle( pen , rect);
			//MessageBox.Show(x + " " + y);
			e.Graphics.DrawRectangle( pen , x - r,y-r , 2*r, 2*r);
			pen.Dispose();


		}

		private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
		{
			point.X = e.X;
			point.Y = e.Y;
			
			pictureBox1.Invalidate();
			pictureBox2.Invalidate();
		}
	}
}