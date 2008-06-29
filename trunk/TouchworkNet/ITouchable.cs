using System;
using System.Collections.Generic;
using System.Text;

namespace TouchworkSDK
{
	public interface ITouchable
	{
        //Touchwork Events

        /// <summary>
        /// Event handler for Touch Down
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
		void OnTouchDown(object sender,TouchEventArgs e);

        /// <summary>
        /// Event handler for Touch Up
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
		void OnTouchUp(object sender,TouchEventArgs e);

        /// <summary>
        /// Event handler for Touch Move
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
		void OnTouchMove(object sender,TouchEventArgs e);
	}
}
