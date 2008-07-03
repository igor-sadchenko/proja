using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;

using System.Windows.Media;

using System.Diagnostics;

namespace NUIGroup.MultiTouchFramework
{
        
    class TouchImage : Image
    {
        #region Instance Variables
        //where drag starts
        Point origLocation;
        //original position of element when drag starts
        double origHorizOffset, origVertOffset;
        //the offset from edges of dragging element
        bool modifyLeftOffset, modifyTopOffset;
        Point _lastPosition;
        Double angle = 0;
        Point imgOrigin;
        //list of fingers on the image
        LinkedList<TouchData> fingerList = new LinkedList<TouchData>();

        //transform tools
        ScaleTransform scaleTrans = new ScaleTransform();
        RotateTransform rotateTrans = new RotateTransform();
        TransformGroup transGroup = new TransformGroup();

        //properties
        //these allow the user to set if an element can be
        //dragged/rotated/scaled from the main app.
        public bool drag = true;
        public bool rotatescale = true;
        
        //

        #endregion


        public static RoutedEvent fingerDownEvent;
        public static RoutedEvent fingerUpdateEvent;
        public static RoutedEvent fingerUpEvent;
        public delegate void fingerDownHandler(object sender, TouchEventArgs e);
        public delegate void fingerUpdateHandler(object sender, TouchEventArgs e);
        public delegate void fingerUpHandler(object sender, TouchEventArgs e);

        public TouchImage()
        {
            
             
            if (fingerDownEvent == null)
                fingerDownEvent = EventManager.RegisterRoutedEvent("fingerDown", RoutingStrategy.Bubble, typeof(fingerDownHandler), typeof(TouchImage));
            if (fingerUpdateEvent == null)
                fingerUpdateEvent = EventManager.RegisterRoutedEvent("fingerUpdate", RoutingStrategy.Bubble, typeof(fingerUpdateHandler), typeof(TouchImage));
            if (fingerUpEvent == null)
                fingerUpEvent = EventManager.RegisterRoutedEvent("fingerUp", RoutingStrategy.Bubble, typeof(fingerUpHandler), typeof(TouchImage));

            TouchCanvas theCanvas = TouchCanvas.Instance;
            theCanvas.fingerDown += new TouchCanvas.fingerDownHandler(theCanvas_fingerDown);
            theCanvas.fingerUp += new TouchCanvas.fingerUpHandler(theCanvas_fingerUp);
            theCanvas.fingerUpdate += new TouchCanvas.fingerUpdateHandler(theCanvas_fingerUpdate);

            
            
        }

        #region Event Handlers
        void theCanvas_fingerUpdate(object sender, TouchEventArgs e)
        {

            if ((e.data.touchedElement == this) && (isInLIstByID(e.data)))
            {
                imgOrigin = new Point(Canvas.GetLeft(this) + this.Width / 2, Canvas.GetTop(this) + this.Height / 2);
                if ((this.fingerList.Count == 1) && drag)
                {
                    double newHorizontalOffset, newVerticalOffset;

                    if (this.modifyLeftOffset)
                        newHorizontalOffset = this.origHorizOffset + (e.data.X - this.origLocation.X);
                    else
                        newHorizontalOffset = this.origHorizOffset - (e.data.X - this.origLocation.X);

                    if (this.modifyTopOffset)
                        newVerticalOffset = this.origVertOffset + (e.data.Y - this.origLocation.Y);
                    else
                        newVerticalOffset = this.origVertOffset - (e.data.Y - this.origLocation.Y);



                    if (this.modifyLeftOffset)
                        Canvas.SetLeft(this, newHorizontalOffset);
                    else
                        Canvas.SetRight(this, newHorizontalOffset);

                    if (this.modifyTopOffset)
                        Canvas.SetTop(this, newVerticalOffset);
                    else
                        Canvas.SetBottom(this, newVerticalOffset);
                }//drag stuff end
                else if (this.fingerList.Count == 2)
                {
                    //start rotatescale
                    
                    //this following lines gets the last finger, the last finger on the image
                    //kind of controls the rotate/scale operation
                    LinkedListNode<TouchData> lastFingerNode;
                    lastFingerNode = fingerList.Last;
                    TouchData lastFinger = lastFingerNode.Value;

                    if (findFingerByID(e.data).ID == lastFinger.ID)
                    {
                        transGroup.Children.Clear();
                        //too rotate around the image's center point
                        this.RenderTransformOrigin = new Point(0.5, 0.5);
                        
                        double num = 57.295779513082323;
                        if (rotatescale)
                        {
                            double num2 = Math.Atan2(this._lastPosition.Y - this.imgOrigin.Y, this._lastPosition.X - this.imgOrigin.X) * num;
                            double num3 = Math.Atan2(e.data.Y - this.imgOrigin.Y, e.data.X - this.imgOrigin.X) * num;
                            
                            angle += (num3 - num2);
                            rotateTrans.Angle = angle;
                            
                            double num4 = Math.Sqrt(Math.Pow(this._lastPosition.Y - this.imgOrigin.Y, 2) + Math.Pow(this._lastPosition.X - this.imgOrigin.X, 2));
                            double num5 = Math.Sqrt(Math.Pow(e.data.Y - this.imgOrigin.Y, 2) + Math.Pow(e.data.X - this.imgOrigin.X, 2));
                            
                            scaleTrans.ScaleX *= (num5 / num4);
                            scaleTrans.ScaleY *= (num5 / num4);

                            transGroup.Children.Add(scaleTrans);
                            transGroup.Children.Add(rotateTrans);

                            this.RenderTransform = transGroup;
                        }
                        _lastPosition = new Point(e.data.X, e.data.Y);
                    }
                    
                }


                //pass the event to main app extenders
                TouchData data = new TouchData(e.data.touchedElement, e.data.ID, e.data.tagID, e.data.X, e.data.Y, e.data.angle, e.data.area, e.data.height, e.data.width, e.data.dX, e.data.dY);
                TouchEventArgs args = new TouchEventArgs(data);
                args.RoutedEvent = fingerUpdateEvent;
                RaiseEvent(args);
            }
            
        }

        void theCanvas_fingerUp(object sender, TouchEventArgs e)
        {
            if (e.data.touchedElement == this)
            {
                
                RemoveFingerByID(e.data);
                //pass the event to main app extenders
                TouchData data = new TouchData(e.data.touchedElement, e.data.ID, e.data.tagID, e.data.X, e.data.Y, e.data.angle, e.data.area, e.data.height, e.data.width, e.data.dX, e.data.dY);
                TouchEventArgs args = new TouchEventArgs(data);
                args.RoutedEvent = fingerUpEvent;
                RaiseEvent(args);
            }
            else if (isInLIstByID(e.data))
            {
                //this is called when the finger is lost off the control
                RemoveFingerByID(e.data);
                TouchData data = new TouchData(null, e.data.ID, e.data.tagID, e.data.X, e.data.Y, e.data.angle, e.data.area, e.data.height, e.data.width, e.data.dX, e.data.dY);
                TouchEventArgs args = new TouchEventArgs(data);
                args.RoutedEvent = fingerUpEvent;
                RaiseEvent(args);
            }

        }

        void theCanvas_fingerDown(object sender, TouchEventArgs e)
        {
            if (e.data.touchedElement == this)
            {
                imgOrigin = new Point(Canvas.GetLeft(this) + this.Width / 2, Canvas.GetTop(this) + this.Height / 2);
                this.fingerList.AddLast(e.data);
                
                if (this.fingerList.Count == 1)
                {
                    //drag stuff start
                    this.origLocation = new Point((e.data.X), (e.data.Y));

                    //because left is shorter than 'Canvas.GetLeft(activeTouchElement)'
                    double left = Canvas.GetLeft(this);
                    double right = Canvas.GetRight(this);
                    double top = Canvas.GetTop(this);
                    double bottom = Canvas.GetBottom(this);

                    //finds edge offsets
                    this.origHorizOffset = ResolveOffset(left, right, out this.modifyLeftOffset);
                    this.origVertOffset = ResolveOffset(top, bottom, out this.modifyTopOffset);
                    
                    //drag stuff end
                }
                else if (this.fingerList.Count == 2)
                {
                    
                    _lastPosition = new Point(e.data.X, e.data.Y);
                    
                    
                    //put anything you want to happen when 2 fingers are pressed on an image here
                }


                //pass the event to main app extenders
                TouchData data = new TouchData(e.data.touchedElement, e.data.ID, e.data.tagID, e.data.X, e.data.Y, e.data.angle, e.data.area, e.data.height, e.data.width, e.data.dX, e.data.dY);
                TouchEventArgs args = new TouchEventArgs(data);
                args.RoutedEvent = fingerDownEvent;
                RaiseEvent(args);
            }
        }
        #endregion

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

        public bool noFingers()
        {
            if (fingerList.Count == 0)
                return true;
            else return false;
        }

        //find which side to use
        private static double ResolveOffset(double side1, double side2, out bool useSide1)
        {
            useSide1 = true;
            double result;
            if (Double.IsNaN(side1))
            {
                if (Double.IsNaN(side2))
                {
                    result = 0;
                }
                else
                {
                    result = side2;
                    useSide1 = false;
                }
            }
            else
            {
                result = side1;
            }
            return result;
        }

        
        bool isInLIstByID(TouchData data)
        {
            if (fingerList.Count != 0)
            {
                foreach (TouchData elem in fingerList)
                {
                    if (data.ID == elem.ID)
                    {
                        return true;
                        //break;
                    }
                }
                return false;
            }
            else return false;

        }

        TouchData getLowestID()
        {
            int lowest = 99999999;
            TouchData carry = null;
            foreach (TouchData data in fingerList)
            {
                if (data.ID < lowest)
                {
                    lowest = data.ID;
                    carry = data;
                }

            }
            return carry;

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

        
        #endregion
    }
}
