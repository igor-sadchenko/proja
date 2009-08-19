using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using Microsoft.Office.Interop.Outlook;

namespace GSync
{
    class OutlookManager
    {
        private string CALENDAR_NAME = "AUC";
        private Application oApp;
        private NameSpace oNS;
        private MAPIFolder oCalendar;
        private Items oItems;

        public OutlookManager()
        {

        }
        public void Open()
        {
            try
            {
                Console.WriteLine("OutlookManager Started...");

                oApp = new Application();
                oNS = oApp.GetNamespace("mapi");
                oNS.Logon(Missing.Value, Missing.Value, true, true);

                oCalendar = oNS.GetDefaultFolder(OlDefaultFolders.olFolderCalendar).Folders[CALENDAR_NAME];
                oItems = oCalendar.Items;
            }
            catch (System.Exception e)
            {
                System.Windows.Forms.MessageBox.Show(e.ToString());
            }
        }
        public void Close()
        {
            try
            {
                oNS.Logoff();

                oItems = null;
                oCalendar = null;
                oNS = null;
                oApp = null;

                Console.WriteLine("OutlookManager Stopped...");
            }
            catch (System.Exception e)
            {
                System.Windows.Forms.MessageBox.Show(e.ToString());
            }
        }
        public void AddAppointment(string subject, string body, DateTime start, DateTime end, string location)
        {
            try
            {
                AppointmentItem ai = oCalendar.Items.Add(OlItemType.olAppointmentItem) as AppointmentItem;
                ai.Subject = subject;
                ai.Body = body;
                ai.Start = start;
                ai.End = end;
                ai.Location = location;
                ai.Close(OlInspectorClose.olSave);

                Console.WriteLine("OutlookManager::Added - " + subject);
            }
            catch (System.Exception e)
            {
                System.Windows.Forms.MessageBox.Show(e.ToString());
            }
        }
    }
}
