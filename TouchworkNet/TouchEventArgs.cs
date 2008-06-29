using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace TouchworkSDK
{
    public class TouchEventArgs : EventArgs
    {
        public Point point;
        public int pressure;
        public int ID;
		public bool handled = false;

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="lparam"></param>
        /// <param name="wparam"></param>
        public TouchEventArgs(uint lparam, uint wparam)
        {
            uint x = 0, y = 0;
            GetHighLow(lparam, ref y, ref x);

            uint id = 0, pressure = 0;
            GetHighLow(wparam, ref pressure, ref id);

            point.X = (int)x;
            point.Y = (int)y;
            this.pressure = (int)pressure;
            this.ID = (int)id; 
        }
        
        /// <summary>
        /// Private helper function  that takes unsigned int and return the High part and Low part 
        /// </summary>
        /// <param name="number"></param>
        /// <param name="h"></param>
        /// <param name="l"></param>
        private void GetHighLow(uint number, ref uint h, ref uint l)
        {
            uint temp = (uint)number;
            l = temp & 0x0000FFFF;
            
            temp = temp & 0xFFFF0000;
            h = temp >> 16;
        }
    }
}
