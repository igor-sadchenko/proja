using System;
using System.Collections.Generic;
using System.Text;
using Google.GData.Calendar;
using Google.GData.Client;
using Google.GData.Extensions;

namespace GSync
{
    class GCalManager
    {
        private string FEED_URI = "http://www.google.com/calendar/feeds/default/private/full";
        private int CAL_ID = 0;
        private CalendarService m_calendarService;
        private OutlookManager m_outlookMgr;

        public GCalManager(OutlookManager outlookMgr)
        {
            m_outlookMgr = outlookMgr;
        }
        public void Open()
        {
            Console.WriteLine("GCalManager Started...");

            try
            {
                m_calendarService = new CalendarService(Configuration.ApplicationName);
                m_calendarService.setUserCredentials(Configuration.Username, Configuration.Password);

                CalendarQuery query = new CalendarQuery();
                query.Uri = new Uri("http://www.google.com/calendar/feeds/default/allcalendars/full");

                Console.WriteLine("GCalManager::Connecting to Google Calendar...");
                CalendarFeed resultFeed = m_calendarService.Query(query);

                //Console.WriteLine("\nYour calendars:");
                //if (resultFeed != null)
                //{
                //    foreach (CalendarEntry entry in resultFeed.Entries)
                //    {
                //        Console.WriteLine("# " + entry.Title.Text);//Id.AbsoluteUri);
                //    }
                //}
                string id = resultFeed.Entries[CAL_ID].Id.AbsoluteUri;
                id = id.Replace("http://www.google.com/calendar/feeds/default/allcalendars/full", "");
                id = id.Substring(1);

                Console.WriteLine("GCalManager::Fetching Calendar [" + resultFeed.Entries[CAL_ID].Title.Text + "]");

                EventQuery equery = new EventQuery();
                equery.Uri = new Uri("http://www.google.com/calendar/feeds/" + id + "/private/full");
                equery.StartTime = Configuration.LastSync;
                //equery.SingleEvents = true;
                equery.ExtraParameters = "showdeleted=true";

                Console.WriteLine("GCalManager::Fetching Events (updated after " + Configuration.LastSync.ToShortDateString() + ")");
                EventFeed eresultFeed = m_calendarService.Query(equery);

                DateTime lastSync = Configuration.LastSync;
                DateTime lastUpdate;

                while (eresultFeed != null && eresultFeed.Entries.Count > 0)
                {
                    foreach (EventEntry entry in eresultFeed.Entries)
                    {
                        if (entry.Times.Count > 0 && entry.Title.Text != "")
                        {
                            lastUpdate = ((Google.GData.Client.AtomEntry)(entry)).Updated;

                            if (entry.Status.Value == EventEntry.EventStatus.CANCELED_VALUE)
                            {
                                m_outlookMgr.DeleteAppointment(entry.EventId);
                            }
                            else
                            {
                                string strRecurrence = "";

                                if (entry.Recurrence != null)
                                {
                                    strRecurrence = entry.Recurrence.Value;
                                }

                                m_outlookMgr.AddUpdateAppointment(entry.EventId,
                                    entry.Title.Text,
                                    ((Google.GData.Client.AtomEntry)(entry)).Content.Content,
                                    entry.Times[0].StartTime,
                                    entry.Times[0].EndTime,
                                    entry.Locations[0].ValueString,
                                    strRecurrence);
                            }

                            entry.SyncEvent = new GCalSyncEvent();
                            entry.SyncEvent.Value = "true";
                            //entry.Update();
                            lastSync = DateTime.Now;
                        }
                    }

                    if (eresultFeed.NextChunk != null)
                    {
                        equery.Uri = new Uri(eresultFeed.NextChunk);
                        Console.WriteLine("GCalManager::Fetching Events (cont.)");
                        eresultFeed = m_calendarService.Query(equery);
                    }
                    else
                    {
                        eresultFeed = null;
                    }
                }

                Configuration.LastSync = lastSync;
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
