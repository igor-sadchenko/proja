//----------------------------------------------------------------------------
// NAME: TouchWork
// DESCRIPTION: Enable controling WorldWind under touchwork
// DEVELOPER: TouchWork Team
// WEBSITE: http://www.mywebsite.com
//----------------------------------------------------------------------------
using System;
using System.Drawing;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using TouchworkSDK;

namespace WorldWind
{
	public class twPlugin: WorldWind.PluginEngine.Plugin
	{
		public override void Load()
		{
			Touchwork.NotifyOnTouch(ParentApplication.WorldWindow);
			base.Load();
		}
		public override void Unload()
		{
			base.Unload();
		}
	}



}