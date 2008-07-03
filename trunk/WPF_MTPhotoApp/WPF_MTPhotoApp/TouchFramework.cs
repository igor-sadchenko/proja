using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
//for debug
using System.Diagnostics;
//for CsTI
using TouchlibWrapper;

namespace NUIGroup.MultiTouchFramework
{
    #region TouchFramework class
    /// <summary>
    /// The main class that gets the Touchlib events via CsTI and sends them
    /// to any class implements it
    /// 
    /// http://www.nuigroup.com
    /// 
    /// thread-safe singleton to avoid RoutedEvent from re-initializing itself
    /// </summary>
    public sealed class TouchFramework : Control
    {
        static TouchFramework instance = null;
        static readonly object padlock = new Object();
        Double screenHeight;
        Double screenWidth;

        public static TouchFramework Instance
        {
            get
            {
                lock (padlock)
                {
                    if (instance == null)
                        instance = new TouchFramework();
                    return instance;
                }
            }
        }

        public static RoutedEvent FrameworkfingerDownEvent;
        public static RoutedEvent FrameworkfingerUpEvent;
        public static RoutedEvent FrameworkfingerUpdateEvent;
        public delegate void FrameWorkfingerDownHandler(object sender, TouchEventArgs e);
        public delegate void FrameWorkfingerUpHandler(object sender, TouchEventArgs e);
        public delegate void FrameWorkfingerUpdateHandler(object sender, TouchEventArgs e);
        public TouchFramework()
        {
            screenHeight = SystemParameters.PrimaryScreenHeight;
            screenWidth = SystemParameters.PrimaryScreenWidth;

            //create csti here
            CsTI cs = CsTI.Instance();

            cs.fingerDown += new TouchlibWrapper.fingerDownHandler(cs_fingerDown);
            cs.fingerUpdate += new TouchlibWrapper.fingerUpdateHandler(cs_fingerUpdate);
            cs.fingerUp += new TouchlibWrapper.fingerUpHandler(cs_fingerUp);

            //event shit
            FrameworkfingerDownEvent = EventManager.RegisterRoutedEvent("FrameWorkfingerDown", RoutingStrategy.Bubble, typeof(FrameWorkfingerDownHandler), typeof(TouchFramework));
            FrameworkfingerUpEvent = EventManager.RegisterRoutedEvent("FrameWorkfingerUp", RoutingStrategy.Bubble, typeof(FrameWorkfingerUpHandler), typeof(TouchFramework));
            FrameworkfingerUpdateEvent = EventManager.RegisterRoutedEvent("FrameWorkfingerUpdate", RoutingStrategy.Bubble, typeof(FrameWorkfingerUpdateHandler), typeof(TouchFramework));



        }

        //the x and y coordinates multiplied by 1000 to get the pixel values
        void cs_fingerUp(int ID, int tagID, float X, float Y, float angle, float area, float height, float width, float dX, float dY)
        {
            //Y = (float)1.0 - Y;

            TouchData data = new TouchData(null, ID, tagID, (float)(X * screenWidth), (float)(Y * screenHeight), angle, area, height, width, dX, dY);
            TouchEventArgs args = new TouchEventArgs(data);
            args.RoutedEvent = FrameworkfingerUpEvent;
            //runs last
            RaiseEvent(args);
        }

        void cs_fingerUpdate(int ID, int tagID, float X, float Y, float angle, float area, float height, float width, float dX, float dY)
        {
            //Y = (float)1.0 - Y;
            TouchData data = new TouchData(null, ID, tagID, (float)(X * screenWidth), (float)(Y * screenHeight), angle, area, height, width, dX, dY);
            TouchEventArgs args = new TouchEventArgs(data);
            args.RoutedEvent = FrameworkfingerUpdateEvent;
            //runs last
            RaiseEvent(args);
        }



        public void cs_fingerDown(int ID, int tagID, float X, float Y, float angle, float area, float height, float width, float dX, float dY)
        {
            //Y = (float)1.0 - Y;
            TouchData data = new TouchData(null, ID, tagID, (float)(X * screenWidth), (float)(Y * screenHeight), angle, area, height, width, dX, dY);
            TouchEventArgs args = new TouchEventArgs(data);
            args.RoutedEvent = FrameworkfingerDownEvent;
            //if (args.Handled == false)
            RaiseEvent(args);
            //runs last
        }


        public event FrameWorkfingerDownHandler FrameWorkfingerDown
        {
            add { AddHandler(FrameworkfingerDownEvent, value); }
            remove { RemoveHandler(FrameworkfingerDownEvent, value); }
        }

        public event FrameWorkfingerUpHandler FrameWorkfingerUp
        {
            add { AddHandler(FrameworkfingerUpEvent, value); }
            remove { RemoveHandler(FrameworkfingerUpEvent, value); }
        }

        public event FrameWorkfingerUpdateHandler FrameWorkfingerUpdate
        {
            add { AddHandler(FrameworkfingerUpdateEvent, value); }
            remove { RemoveHandler(FrameworkfingerUpdateEvent, value); }
        }

        //this is still a problem, but it works for now
        //read the comment when this is called for what it is
        public void startTracking()
        {
            //MessageBox.Show("RUN");
            CsTI cs = CsTI.Instance();
            cs.startScreen();
        }
    }
    #endregion
}
