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
            OutlookManager olMgr = new OutlookManager();
            GCalManager gCal = new GCalManager(olMgr);

            olMgr.Open();
            //olMgr.AddAppointment("ABC", "Hiiii", "ASDASDASDASDA", DateTime.Now, DateTime.Now.AddDays(1), "Here");
            //olMgr.FindAppointment("ABD");
            gCal.Open();
            olMgr.Close();
        }

        public void Shutdown(object obj, EventArgs args)
        {
            //Configuration.SaveConfiguration();
        }
    }
}
