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

                oCalendar.UserDefinedProperties.Add("EventID", OlUserPropertyType.olText, Type.Missing, Type.Missing);
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
                oCalendar.UserDefinedProperties["EventID"].Delete();

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
        public AppointmentItem FindAppointment(string id)
        {
            try
            {
                AppointmentItem ai = oCalendar.Items.Find("[EventID] = '" + id + "'") as AppointmentItem;
                return ai;
            }
            catch (System.Exception e)
            {
                System.Windows.Forms.MessageBox.Show(e.ToString());
                return null;
            }
        }
        public void AddUpdateAppointment(string id, string subject, string body, DateTime start, DateTime end, string location, string recurrence)
        {
            try
            {
                AppointmentItem ai = FindAppointment(id);
                bool found = true;

                if (ai == null)
                {
                    found = false;
                    ai = oCalendar.Items.Add(OlItemType.olAppointmentItem) as AppointmentItem;
                }

                if (recurrence != "")
                {
                    SetRecurrence(ai, recurrence);
                }

                ai.Subject = subject;
                ai.Body = body;
                if (!ai.IsRecurring)
                {
                    ai.Start = start;
                    ai.End = end;
                }
                ai.Location = location;
                ai.UserProperties.Add("EventID", OlUserPropertyType.olText, false, System.Reflection.Missing.Value);
                ai.UserProperties["EventID"].Value = id;

                ai.Close(OlInspectorClose.olSave);

                if (found)
                    Console.WriteLine("OutlookManager::Updated - " + subject);
                else
                    Console.WriteLine("OutlookManager::Added - " + subject);
            }
            catch (System.Exception e)
            {
                System.Windows.Forms.MessageBox.Show(e.ToString());
            }
        }
        public void DeleteAppointment(string id)
        {
            try
            {
                AppointmentItem ai = oCalendar.Items.Find("[EventID] = '" + id + "'") as AppointmentItem;
                
                if (ai != null)
                {
                    Console.WriteLine("OutlookManager::Deleted - " + ai.Subject);
                    ai.Delete();
                }
            }
            catch (System.Exception e)
            {
                System.Windows.Forms.MessageBox.Show(e.ToString());
            }
        }

        private void SetRecurrence(AppointmentItem ai, string recurrence)
        {
            RecurrencePattern rp = ai.GetRecurrencePattern();

            //recurrence = recurrence.Substring(recurrence.IndexOf("RRULE:") + 6);
            //recurrence = recurrence.Substring(0, recurrence.IndexOf("\r"));

            //FREQ=DAILY;UNTIL=20090826T080000Z;WKST=SU

            string[] lines = recurrence.Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries);
            string temp, temp2;
            string rule;
            DateTime dtStart, dtEnd;

            foreach (string line in lines)
            {
                if (line.IndexOf("DTSTART;") > -1) // Start Date
                {
                    temp = line.Substring(line.IndexOf("DTSTART;") + 8);
                    string[] tokens = temp.Split(':');
                    temp = tokens[1];
                    temp2 = "";

                    if (temp.IndexOf('T') > -1)
                    {
                        temp = temp.Substring(0, temp.IndexOf('T'));

                        temp2 = tokens[1].Substring(tokens[1].IndexOf('T') + 1);
                        
                        temp2 = temp2.Insert(4, ":");
                        temp2 = temp2.Insert(2, ":");
                    }

                    temp = temp.Insert(6, "/");
                    temp = temp.Insert(4, "/");

                    rp.StartTime = rp.PatternStartDate = DateTime.Parse(temp + " " + temp2);
                }
                else
                    if (line.IndexOf("DTEND;") > -1) // End Date
                    {
                        temp = line.Substring(line.IndexOf("DTEND;") + 6);
                        string[] tokens = temp.Split(':');
                        temp = tokens[1];
                        temp2 = "";

                        if (temp.IndexOf('T') > -1)
                        {
                            temp = temp.Substring(0, temp.IndexOf('T'));

                            temp2 = tokens[1].Substring(tokens[1].IndexOf('T') + 1);

                            temp2 = temp2.Insert(4, ":");
                            temp2 = temp2.Insert(2, ":");
                        }

                        temp = temp.Insert(6, "/");
                        temp = temp.Insert(4, "/");
                        
                        rp.EndTime = DateTime.Parse(temp + " " + temp2);
                    }
                    else
                        if (line.IndexOf("RRULE:") > -1) // Start Date
                        {
                            temp = line.Substring(line.IndexOf("RRULE:") + 6);

                            string[] fields = temp.Split(';');
                            Dictionary<string, string> kv = new Dictionary<string, string>();

                            foreach (string field in fields)
                            {
                                string[] tokens = field.Split('=');
                                kv.Add(tokens[0], tokens[1]);
                            }

                            if (kv.ContainsKey("FREQ"))
                            {
                                switch (kv["FREQ"])
                                {
                                    case "DAILY": rp.RecurrenceType = OlRecurrenceType.olRecursDaily; break;
                                    case "WEEKLY": rp.RecurrenceType = OlRecurrenceType.olRecursWeekly; break;
                                    case "MONTHLY": rp.RecurrenceType = OlRecurrenceType.olRecursMonthly; break;
                                    case "YEARLY": rp.RecurrenceType = OlRecurrenceType.olRecursYearly; break;
                                }
                            }

                            if (kv.ContainsKey("INTERVAL"))
                            {
                                int i = 1;
                                int.TryParse(kv["INTERVAL"], out i);
                                rp.Interval = i;
                            }

                            if (kv.ContainsKey("UNTIL"))
                            {
                                temp = kv["UNTIL"];
                                temp2 = "";

                                if (temp.IndexOf('T') > -1)
                                {
                                    temp = temp.Substring(0, temp.IndexOf('T'));

                                    temp2 = kv["UNTIL"].Substring(kv["UNTIL"].IndexOf('T') + 1);

                                    temp2 = temp2.Insert(4, ":");
                                    temp2 = temp2.Insert(2, ":");
                                }

                                temp = temp.Insert(6, "/");
                                temp = temp.Insert(4, "/");

                                rp.PatternEndDate = DateTime.Parse(temp + " " + temp2);
                            }

                            break;
                        }
            }

            //String recurData =
            //    "DTSTART;VALUE=DATE:20070501\r\n" +
            //    "DTEND;VALUE=DATE:20070502\r\n" +
            //    "RRULE:FREQ=WEEKLY;BYDAY=Tu;UNTIL=20070904\r\n";

            /*
             * "DTSTART;TZID=Africa/Cairo:20090824T110000\r\n
             * DTEND;TZID=Africa/Cairo:20090824T130000\r\n
             * 
             * RRULE:FREQ=DAILY;UNTIL=20090826T080000Z;WKST=SU\r\n
             * 
             * RRULE:FREQ=YEARLY;BYMONTH=4;BYDAY=-1FR\r\n
             * 
             * END:VTIMEZONE"*/
        }
    }
}
