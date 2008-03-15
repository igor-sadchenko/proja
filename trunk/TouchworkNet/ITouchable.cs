using System;
using System.Collections.Generic;
using System.Text;

namespace TouchworkSDK
{
	public interface ITouchable
	{
		void OnTouchDown(object sender,TouchEventArgs e);
		void OnTouchUp(object sender,TouchEventArgs e);
		void OnTouchMove(object sender,TouchEventArgs e);
	}
}
