using System;
using System.Collections.Generic;
using System.Text;

namespace TouchworkSDK
{
	public interface ITouchable
	{
		bool OnTouchDown(object sender,TouchEventArgs e);
		bool OnTouchUp(object sender,TouchEventArgs e);
		bool OnTouchMove(object sender,TouchEventArgs e);
	}
}
