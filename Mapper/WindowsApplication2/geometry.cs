using System;
using System.Collections.Generic;
using System.Text;


namespace WindowsApplication2
{

	public enum VertexType
	{
		Merge,
		Split,
		Normal,
		Start,
		End
	}
	public class Geometry
	{
		public const float EPSILON = 0.00001f;
	}

	public class gPoint : IComparable<gPoint>
	{
		public VertexType type;
		public bool m_isVisited = false;
		public float X;
		public float Y;
		public List<gSegment> m_edges_in = new List<gSegment>();
		public List<gSegment> m_edges_out = new List<gSegment>();

		public gPoint() { }
		public gPoint(float x, float y)
		{
			X = x;
			Y = y;
		}
		public gPoint(System.Drawing.Point pt)
		{
			X = pt.X;
			Y = pt.Y;
		}

		public int GetWieghtIn()
		{
			int sum = 0;
			foreach(gSegment e in m_edges_in)
				sum += e.m_wieght;
			return sum;
		}

		public int GetWieghtOut()
		{
			int sum = 0;
			foreach(gSegment e in m_edges_out)
				sum += e.m_wieght;
			return sum;
		}

		public System.Drawing.Point ToPoint()
		{
			return new System.Drawing.Point((int)X, (int)Y);
		}

		public bool isConnectedTo(gPoint p)
		{
			foreach(gSegment edge in m_edges_out)
			{
				if (((object)p) == ((object)edge.P2))
					return true;
			}
//  			foreach (gSegment edge in p.m_edges_in)
//  			{
//  				if (((object)p) == ((object)edge.P1))
//  					return true;
//  			}
			return false;
		}

		public float GetDistance(gPoint p)
		{
			float dx = Math.Abs(this.X - p.X);
			float dy = Math.Abs(this.Y - p.Y);
			return (float)Math.Sqrt(dx * dx + dy * dy);
		}

		public bool isPointInBox(gPoint b1, gPoint b2)
		{
			gPoint p = this;
			return ((p.X >= Math.Min(b1.X, b2.X)) && (p.X <= Math.Max(b1.X, b2.X))
				&& (p.Y >= Math.Min(b1.Y, b2.Y)) && (p.Y <= Math.Max(b1.Y, b2.Y)));
		}

		static public float GetSignedTriangleArea(gPoint a, gPoint b, gPoint c)
		{
			return ((a.X * b.Y - a.Y * b.X + a.Y * c.X
				- a.X * c.Y + b.X * c.Y - c.X * b.Y) / 2.0f);
		}

		static public float GetTriangleArea(gPoint a, gPoint b, gPoint c)
		{
			return (Math.Abs(GetSignedTriangleArea(a, b, c)));
		}

		static public bool isCCW(gPoint a, gPoint b, gPoint c)
		{
			return (GetSignedTriangleArea(a, b, c) > Geometry.EPSILON);
		}

		static public bool isCW(gPoint a, gPoint b, gPoint c)
		{
			return (GetSignedTriangleArea(a, b, c) < Geometry.EPSILON);
		}

		static public bool isCollinear(gPoint a, gPoint b, gPoint c)
		{
			return (Math.Abs(GetSignedTriangleArea(a, b, c)) <= Geometry.EPSILON);
		}



		
		public int CompareTo(gPoint other)
		{


			if (Y > other.Y)
				return 1;
			else
			{
				if (Y == other.Y)
				{
					if (X > other.X)
						return 1;
					else if (X == other.X)
						return 0;
				}
				else
				{
					return -1;
				}
			}
			return -1;

		
		}

	}

	public class gLine
	{
		public float a;		/* x-coefficient */
		public float b;		/* y-coefficient */
		public float c;		/* constant term */

		public gLine(gPoint p1, gPoint p2)
		{
			if (p1.X == p2.X)
			{
				a = 1;
				b = 0;
				c = -p1.X;
			}
			else
			{
				b = 1;
				a = -(p1.Y - p2.Y) / (p1.X - p2.X);
				c = -(a * p1.X) - (b * p1.Y);
			}
		}


		public bool isParallel(gLine other)
		{
			return (
				(Math.Abs(a - other.a) <= Geometry.EPSILON) &&
				(Math.Abs(b - other.b) <= Geometry.EPSILON)
				);
		}

		public bool isSameLine(gLine other)
		{
			return (isParallel(other) && (Math.Abs(c - other.c) <= Geometry.EPSILON));
		}


		public gPoint GetIntersection(gLine other)
		{
			gPoint p = new gPoint();
			if (isSameLine(other))
			{
				return null;
			}

			if (isParallel(other))
			{
				return null;
			}

			p.X = (other.b * c - b * other.c) / (other.a * b - a * other.b);

			if (Math.Abs(b) > Geometry.EPSILON)	/* test for vertical line */
				p.Y = -(a * (p.X) + c) / b;
			else
				p.Y = -(other.a * (p.X) + other.c) / other.b;

			return p;
		}



	}

	public class gSegment
	{
		public int m_wieght = 1;
		public bool m_isExtra = false;
		public gPoint P1;
		public gPoint P2;

		public gSegment(gPoint p1, gPoint p2)
		{
			P1 = p1;
			P2 = p2;
		}

		public gLine GetLine()
		{
			return new gLine(P1, P2);
		}
		static public bool GetIntersection(gSegment s1, gSegment s2, ref gPoint p)
		{
			gLine l1, l2;		/* lines containing the input segments */
			//gPoint p;		/* intersection point */

			l1 = s1.GetLine();
			l2 = s2.GetLine();

			if (l1.isSameLine(l2)) 	/* overlapping or disjoint segments */
				return (s1.P1.isPointInBox(s2.P1, s2.P2) ||
					s1.P2.isPointInBox(s2.P1, s2.P2) ||
					s2.P1.isPointInBox(s1.P1, s1.P2) ||
					s2.P1.isPointInBox(s1.P1, s1.P2));

			if (l1.isParallel(l2)) return (false);

			p = l1.GetIntersection(l2);

			return (p.isPointInBox(s1.P1, s1.P2) && p.isPointInBox(s2.P1, s2.P2));
		}

		static public double GetAngle(gSegment l1, gSegment l2)
		{
			double Angle, angle;
			float Tan = 0;
			float m1 = 0, m2 = 0;
			bool Is90 = false;

			if (l1.P1.X != l1.P2.X)
				m1 = (float)(l1.P2.Y - l1.P1.Y) / (l1.P2.X - l1.P1.X);
			if (l2.P1.X != l2.P2.X)
			{
				m2 = (float)(l2.P2.Y - l2.P1.Y) / (l2.P2.X - l2.P1.X);
			}
			else
			{
				Is90 = true;
			}

			if (m1 * m2 != -1 && !Is90)
			{
				Tan = (m1 + m2) / (1 + m1 * m2);
				angle = Math.Atan(Tan);
				Angle = angle * (180 / Math.PI);
			}
			else
			{
				if (l2.P2.Y > l1.P1.Y)
					Angle = 90;
				else
					Angle = 270;
			}
			if (!Is90)
			{
				if ((l2.P2.X < l1.P1.X) && (l2.P2.Y > l1.P1.Y))
					Angle = 180 + Angle;

				if ((l2.P2.X < l1.P1.X) && (l2.P2.Y < l1.P1.Y))
					Angle = 180 + Angle;

				if ((l2.P2.X > l1.P1.X) && (l2.P2.Y < l1.P1.Y))
					Angle = 360 + Angle;
			}
			return Angle;
		}

	}

	class gFace
	{
		public List<gSegment> m_edges = new List<gSegment>();
		
	}


	public class PointXComparer: IComparer<gPoint>
	{

		#region IComparer<gPoint> Members

		public int Compare(gPoint a, gPoint b)
		{

			if (a.X > b.X)
				return 1;
			else
			{
				if (a.X == b.X)
				{
					if (a.Y > b.Y)
						return 1;
					else if (a.Y == b.Y)
						return 0;
				}
				else
				{
					return -1;
				}
			}
			return -1;

		}

		#endregion
	}
	public class EdgeInComparer: IComparer<gSegment>
{
//anticlock ascending

public int  Compare(gSegment x, gSegment y)
{
	double a = gSegment.GetAngle(new gSegment(x.P2,new gPoint(x.P2.X,float.MaxValue)) , new gSegment(x.P2,x.P1));
	double b = gSegment.GetAngle(new gSegment(y.P2, new gPoint(x.P2.X, float.MaxValue)), new gSegment(y.P2, y.P1));
	return (int)(a - b);
	//IComparer<gPoint> c = new PointXComparer();
 	//return c.Compare(x.P1,y.P1);
}

}
		public class EdgeOutComparer: IComparer<gSegment>
{//clockwise--descending


public int  Compare(gSegment x, gSegment y)
{
	double a = gSegment.GetAngle(new gSegment(x.P1, new gPoint(x.P1.X, 0)), new gSegment(x.P1, x.P2));
	double b = gSegment.GetAngle(new gSegment(y.P1, new gPoint(x.P1.X, 0)), new gSegment(y.P1, y.P2));
	return (int)(a - b);
	//IComparer<gPoint> c = new PointXComparer();
	//return - c.Compare(x.P2, y.P2);
 	
}


		}
	public class TreeNode
	{
		TreeNode left = null;
		TreeNode right = null;
		public TreeNode(TreeNode l,TreeNode r)
		{
			left = l;
			right = r;
		}
		public void InsertLeft(TreeNode n)
		{
			if(left == null)
				left = n;
			else
				left.InsertLeft(n);

		}
	}

}

