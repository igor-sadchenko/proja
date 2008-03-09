using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace touch_simulator
{
	[Serializable]
	public class Blob:IEquatable<Blob>
	{
		public static int radius = 10;
		public Point center;
		public bool isSelected = false;
		public bool isVisible = true;
		public int id;
		public TWMessagesType type;

		public Blob(){}
		public Blob(Point _center , int _id)
		{
			center = _center;
			id = _id;
		}
		public Blob(Blob b)
		{
			center = b.center;
			id = b.id;
		}
		public void Draw(Graphics g, Brush brush)
		{
			g.FillEllipse(brush, center.X-radius, center.Y - radius, 2*radius , 2*radius);
		}

		public bool ContainsPoint(Point p)
		{
			
			int dx = Math.Abs(center.X - p.X);
			int dy = Math.Abs(center.Y - p.Y);
			int dist =(int) Math.Sqrt(dx * dx + dy * dy);
			if (dist <= radius)
				return true;
			else
				return false;
		}



		#region IEquatable<Blob> Members

		public bool Equals(Blob other)
		{
			if (id == other.id)
				return true;
			return false;
		}

		#endregion
	}
}
