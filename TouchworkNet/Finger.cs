using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace TouchworkSDK
{
    public class Finger
    {
        public int mID;
        public Point mPosition;
        public Point mOldPosition;
        //public bool isMoved; 
        public Finger(int id, Point position)
        {
            mID = id;
            mPosition = position;
            mOldPosition = Point.Empty;
        }
        public void Move(Point newPoint)
        {
            mOldPosition = mPosition;
            mPosition = newPoint;
            //isMoved = true;
        }
        public static double GetDistance(Point p1, Point p2)
        {
            int dy = p1.Y - p2.Y;
            int dx = p1.X - p2.X;

            return Math.Sqrt(((dx * dx) + (dy * dy)));
        }
        public static Point GetMidPoint(Point p1, Point p2)
        {
            int x = Math.Abs(p1.X - p2.X);
            x = x / 2;
            x += Math.Min(p1.X, p2.X);

            int y = Math.Abs(p1.Y - p2.Y);
            y = y / 2;
            y += Math.Min(p1.Y, p2.Y);

            return new Point(x, y);
        }
    }
}
