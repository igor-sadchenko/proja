using System;
using System.Collections.Generic;
using System.Text;

namespace Touchwork
{
	public interface ITouchable
	{
		bool OnTouchDown(TouchEventArgs e);
		bool OnTouchUp(TouchEventArgs e);
		bool OnTouchMove(TouchEventArgs e);
	}
}
