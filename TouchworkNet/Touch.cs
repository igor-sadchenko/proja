using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace TouchworkSDK
{
    public class Touch
    {
        public int mID;
        public Point mPosition;
        public Point mOldPosition;
        //public bool isMoved; 

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="id"></param>
        /// <param name="position"></param>
        public Touch(int id, Point position)
        {
            mID = id;
            mPosition = position;
            mOldPosition = Point.Empty;
        }

        /// <summary>
        /// This function us used to update the positions
        /// </summary>
        /// <param name="newPoint"></param>
        public void Move(Point newPosition)
        {
            mOldPosition = mPosition;
            mPosition = newPosition;
            //isMoved = true;
        }

        /// <summary>
        /// Helper function to get the distance between two points.
        /// </summary>
        /// <param name="p1"></param>
        /// <param name="p2"></param>
        /// <returns></returns>
        public static double GetDistance(Point p1, Point p2)
        {
            int dy = p1.Y - p2.Y;
            int dx = p1.X - p2.X;

            return Math.Sqrt(((dx * dx) + (dy * dy)));
        }

        /// <summary>
        /// Helper function to get the mid-point on the line joining two points.
        /// </summary>
        /// <param name="p1"></param>
        /// <param name="p2"></param>
        /// <returns></returns>
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
