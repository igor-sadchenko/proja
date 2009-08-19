using System;
using System.Collections.Generic;
using System.Text;

namespace GSync
{
    class SyncMain
    {
        public SyncMain()
        {
        }

        public void Start()
        {
            GCalManager gCal = new GCalManager(new OutlookManager());
            gCal.Open();
        }
    }
}
