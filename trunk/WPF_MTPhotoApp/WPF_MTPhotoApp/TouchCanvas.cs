using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;

//for hit tests
using System.Windows.Media;

//for debugging
using System.Diagnostics;

using TouchworkSDK;



namespace NUIGroup.MultiTouchFramework 
{
    #region TouchCanvas Class
    public sealed class TouchCanvas : Canvas, ITouchable 
    {
        public LinkedList<UIElement> activeObjects = new LinkedList<UIElement>();
        public LinkedList<TouchData> fingerList = new LinkedList<TouchData>();

        public static RoutedEvent fingerDownEvent;
        public static RoutedEvent fingerUpdateEvent;
        public static RoutedEvent fingerUpEvent;
        public delegate void fingerDownHandler(object sender, TouchEventArgs e);
        public delegate void fingerUpdateHandler(object sender, TouchEventArgs e);
        public delegate void fingerUpHandler(object sender, TouchEventArgs e);
        TouchFramework frame = TouchFramework.Instance;
        static TouchCanvas _instance;
        static readonly object padlock = new Object();

        int newZindex = 10;

        UIElement touchElement;
        ScaleTransform scaleTrans = new ScaleTransform();
        RotateTransform rotateTrans = new RotateTransform();
        TransformGroup transGroup = new TransformGroup();

        Point _lastPosition;
        Double angle = 0;
        Point imgOrigin;

        

        //provide a global access point to the same instance that was created in the main app
        //there must be an easier way to do this...
        public static TouchCanvas Instance
        {
            get
            {
                lock (padlock)
                {
                    if (_instance == null)
                        _instance = new TouchCanvas();
                    return _instance;
                }
            }
        }

        public TouchCanvas()
        {
            int screenHeight = (int)SystemParameters.PrimaryScreenHeight;
            int screenWidth = (int)SystemParameters.PrimaryScreenWidth;
            imgOrigin = new Point(screenWidth / 2, screenHeight / 2);
            _instance = this;
            //if (fingerDownEvent == null)
                frame.FrameWorkfingerDown += new TouchFramework.FrameWorkfingerDownHandler(frame_FrameWorkfingerDown);
            //if (fingerUpdateEvent == null)
                frame.FrameWorkfingerUpdate += new TouchFramework.FrameWorkfingerUpdateHandler(frame_FrameWorkfingerUpdate);
            //if (fingerUpEvent == null)
                frame.FrameWorkfingerUp += new TouchFramework.FrameWorkfingerUpHandler(frame_FrameWorkfingerUp);

            if (fingerDownEvent == null)
                fingerDownEvent = EventManager.RegisterRoutedEvent("fingerDown", RoutingStrategy.Bubble, typeof(fingerDownHandler), typeof(TouchFramework));
            if (fingerUpdateEvent == null)
                fingerUpdateEvent = EventManager.RegisterRoutedEvent("fingerUpdate", RoutingStrategy.Bubble, typeof(fingerUpdateHandler), typeof(TouchFramework));
            if (fingerUpEvent == null)
                fingerUpEvent = EventManager.RegisterRoutedEvent("fingerUp", RoutingStrategy.Bubble, typeof(fingerUpHandler), typeof(TouchFramework));

        }

        public void startTracking()
        {
            frame.startTracking();
        }
       

        void frame_FrameWorkfingerDown(object sender, TouchEventArgs e)
        {
            //this.RenderTransform = new TranslateTransform(50, 50);
            touchElement = getTouchedElement(e.data.X, e.data.Y);

            if (touchElement != null)
            {
                if (!activeObjects.Contains(touchElement))
                    activeObjects.AddLast(touchElement);
                if (touchElement.GetType().ToString() == "NUIGroup.MultiTouchFramework.TouchImage")
                {
                    TouchData data = new TouchData(touchElement, e.data.ID, e.data.tagID, e.data.X, e.data.Y, e.data.angle, e.data.area, e.data.height, e.data.width, e.data.dX, e.data.dY);
                    Canvas.SetZIndex(touchElement, newZindex);
                    newZindex++;

                    TouchEventArgs args = new TouchEventArgs(data);
                    args.RoutedEvent = fingerDownEvent;

                    RaiseEvent(args);
                }
                if (touchElement.GetType().ToString() == "NUIGroup.MultiTouchFramework.TouchCanvas")
                {
                    
                }
            }
            else
            {
                fingerList.AddLast(e.data);
                _lastPosition = new Point(e.data.X, e.data.Y);
            }
            
            
        }

        void frame_FrameWorkfingerUp(object sender, TouchEventArgs e)
        {
            Debug.WriteLine("UP");
            UIElement releasedElement = getTouchedElement(e.data.X, e.data.Y);
            if (releasedElement != null)
            {
                
                if (releasedElement.GetType().ToString() == "NUIGroup.MultiTouchFramework.TouchImage")
                {

                    TouchData data = new TouchData(releasedElement, e.data.ID, e.data.tagID, e.data.X, e.data.Y, e.data.angle, e.data.area, e.data.height, e.data.width, e.data.dX, e.data.dY);
                    TouchEventArgs args = new TouchEventArgs(data);
                    args.RoutedEvent = fingerUpEvent;
                    //runs first
                    RaiseEvent(args);

                    if (activeObjects.Contains(releasedElement) && ((TouchImage)releasedElement).noFingers())
                        activeObjects.Remove(releasedElement);
                    
                }
                else if (releasedElement.GetType().ToString() == "NUIGroup.MultiTouchFramework.TouchCanvas")
                {
                    //remove finger from canvas touch
                    //RemoveFingerByID(e.data);
                }

            }
            else
            {
                //remove finger from canvas touch
                RemoveFingerByID(e.data);
            }
            
              
        }

        void frame_FrameWorkfingerUpdate(object sender, TouchEventArgs e)
        {
            touchElement = getTouchedElement(e.data.X, e.data.Y);

            if (touchElement != null)
            {
                if (touchElement.GetType().ToString() == "NUIGroup.MultiTouchFramework.TouchImage")
                {

                    TouchData data = new TouchData(touchElement, e.data.ID, e.data.tagID, e.data.X, e.data.Y, e.data.angle, e.data.area, e.data.height, e.data.width, e.data.dX, e.data.dY);
                    TouchEventArgs args = new TouchEventArgs(data);
                    args.RoutedEvent = fingerUpdateEvent;
                    RaiseEvent(args);
                }
            }
            else
            {
                

                TouchData data = new TouchData(null, e.data.ID, e.data.tagID, e.data.X, e.data.Y, e.data.angle, e.data.area, e.data.height, e.data.width, e.data.dX, e.data.dY);
                TouchEventArgs args = new TouchEventArgs(data);
                args.RoutedEvent = fingerUpEvent;
                RaiseEvent(args);

                //do canvas move
                
            }
        }

        #region Add/Remove Event Handlers
        public event fingerDownHandler fingerDown
        {
            add { AddHandler(fingerDownEvent, value); }
            remove { RemoveHandler(fingerDownEvent, value); }
        }

        public event fingerUpdateHandler fingerUpdate
        {
            add { AddHandler(fingerUpdateEvent, value); }
            remove { RemoveHandler(fingerUpdateEvent, value); }
        }

        public event fingerUpHandler fingerUp
        {
            add { AddHandler(fingerUpEvent, value); }
            remove { RemoveHandler(fingerUpEvent, value); }
        }
        #endregion

        #region Helper Methods
        UIElement getTouchedElement(double x, double y)
        {
            UIElement temp = null;
            //times 1000 to get actual pixel locations (i think)
            Point pt = new Point(Math.Floor(x), Math.Floor(y));
           // Debug.WriteLine(pt.ToString() + " AND ORIGINAL X<Y " + x + "," + y);
            //Debug.WriteLine(pt.ToString());

            //perform hit test, only get the topmost element on coordinate
            HitTestResult result = VisualTreeHelper.HitTest(this, pt);
            
            if (result != null)
            {
                //Debug.WriteLine(result.GetType());
                DependencyObject obj = (DependencyObject)result.VisualHit;
                //Debug.WriteLine("OBJ LEFT: " + ((Canvas.GetLeft((UIElement)obj)).ToString()));
                temp = (UIElement)obj;
                //Debug.WriteLine("TYPE: " + temp.GetType().ToString());
            }
            return temp;
        }


        private void RemoveFingerByID(TouchData remove)
        {
            foreach (TouchData data in fingerList)
            {
                if (remove.ID == data.ID)
                {
                    fingerList.Remove(data);
                    break;
                }
            }

        }

        private TouchData findFingerByID(TouchData find)
        {
            if (fingerList.Count != 0)
            {
                foreach (TouchData data in fingerList)
                {
                    if (find.ID == data.ID)
                    {
                        return data;

                    }
                }
            }
            return null;

        }

        bool isInList(TouchData find)
        {
            if (fingerList.Count != 0)
            {
                foreach (TouchData data in fingerList)
                {
                    if (find.ID != data.ID)
                    {
                        return true;

                    }
                    if (data.touchedElement == null)
                        Debug.WriteLine("NULL");
                }
            }
            return false;
        }
        #endregion


        #region ITouchable Members

        public void OnTouchDown(object sender, TouchworkSDK.TouchEventArgs e)
        {
            //MessageBox.Show("TouchDown");
            TouchEventArgs args = new TouchEventArgs(new TouchData(null, e.ID, 0, e.point.X,
                e.point.Y, 0.0f, (float)e.pressure, 0, 0, 0, 0));
            frame_FrameWorkfingerDown(sender, args);
        }

        public void OnTouchMove(object sender, TouchworkSDK.TouchEventArgs e)
        {
            //MessageBox.Show("TouchMove");
            TouchEventArgs args = new TouchEventArgs(new TouchData(null, e.ID, 0, e.point.X,
                e.point.Y, 0.0f, (float)e.pressure, 0, 0, 0, 0));
            frame_FrameWorkfingerUpdate(sender, args);
        }

        public void OnTouchUp(object sender, TouchworkSDK.TouchEventArgs e)
        {
            //MessageBox.Show("TouchUp");
            TouchEventArgs args = new TouchEventArgs(new TouchData(null, e.ID, 0, e.point.X,
                e.point.Y, 0.0f, (float)e.pressure, 0, 0, 0, 0));
            frame_FrameWorkfingerUp(sender, args);
        }

        #endregion
    }
    #endregion
}
