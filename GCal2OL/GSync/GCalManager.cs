using System;
using System.Collections.Generic;
using System.Text;
using Google.GData.Calendar;
using Google.GData.Client;

namespace GSync
{
    class GCalManager
    {
        private string USERNAME = "a.shaker.axmo@gmail.com";
        private string PASSWORD = "qwertyui";
        private string FEED_URI = "http://www.google.com/calendar/feeds/default/private/full";
        private string APP_NAME = "MyGSync-1.0b";
        private CalendarService m_calendarService;
        private OutlookManager m_outlookMgr;

        public GCalManager(OutlookManager outlookMgr)
        {
            m_outlookMgr = outlookMgr;
        }
        public void Open()
        {
            try
            {
                m_outlookMgr.Open();

                m_calendarService = new CalendarService(APP_NAME);
                m_calendarService.setUserCredentials(USERNAME, PASSWORD);

                CalendarQuery query = new CalendarQuery();
                query.Uri = new Uri("http://www.google.com/calendar/feeds/default/allcalendars/full");
                CalendarFeed resultFeed = m_calendarService.Query(query);
                //Console.WriteLine("\nYour calendars:");
                //if (resultFeed != null)
                //{
                //    foreach (CalendarEntry entry in resultFeed.Entries)
                //    {
                //        Console.WriteLine("# " + entry.Title.Text);//Id.AbsoluteUri);
                //    }
                //}
                string id = resultFeed.Entries[0].Id.AbsoluteUri;
                id = id.Replace("http://www.google.com/calendar/feeds/default/allcalendars/full", "");
                id = id.Substring(1);

                EventQuery equery = new EventQuery();
                equery.Uri = new Uri("http://www.google.com/calendar/feeds/" + id + "/private/full");
                equery.StartTime = DateTime.Now;
                equery.EndTime = DateTime.Now.AddMonths(1);

                EventFeed eresultFeed = m_calendarService.Query(equery);

                if (eresultFeed != null)
                {
                    Console.WriteLine("\nYour events:");

                    foreach (EventEntry entry in eresultFeed.Entries)
                    {
                        if (entry.Times.Count > 0 && entry.Title.Text != "")
                        {
                            Console.WriteLine("# " + entry.Title.Text + " : " + entry.Times[0].StartTime.ToString() + " -> " + entry.Times[0].EndTime.ToString());

                            m_outlookMgr.AddAppointment(
                                entry.Title.Text,
                                ((Google.GData.Client.AtomEntry)(entry)).Content.Content,
                                entry.Times[0].StartTime,
                                entry.Times[0].EndTime,
                                entry.Locations[0].ValueString);
                        }
                    }
                }
                m_outlookMgr.Close();
            }
            catch (Exception e)
            {
                System.Windows.Forms.MessageBox.Show(e.ToString());
            }
        }
        private void Close()
        {
        }
    }
}
