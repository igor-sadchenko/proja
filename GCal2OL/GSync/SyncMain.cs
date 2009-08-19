using System;
using System.Collections.Generic;
using System.Text;

namespace GSync
{
    class SyncMain
    {
        public SyncMain()
        {
            Configuration.LoadConfiguration();
        }

        public void Start()
        {
            GCalManager gCal = new GCalManager(new OutlookManager());
            gCal.Open();
        }

        public void Shutdown(object obj, EventArgs args)
        {
            //Configuration.SaveConfiguration();
        }
    }
}
