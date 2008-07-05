using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Interop; 

//for any touch element
using NUIGroup.MultiTouchFramework;

using System.Diagnostics;
//dditional information: Cannot create instance of 'TouchCanvas' defined in assembly 'WPF_MTPhotoApp, Version=1.0.3085.1865, Culture=neutral, PublicKeyToken=null'. Exception has been thrown by the target of an invocation.  Error at object 'WPF_MTPhotoApp.PhotoApp' in markup file 'WPF_MTPhotoApp;component/photoapp.xaml' Line 7 Position 4.

using TouchworkSDK;


namespace WPF_MTPhotoApp
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>

    public partial class PhotoApp : System.Windows.Window
    {
        //TouchCanvas myCanvas = new TouchCanvas();

        System.Windows.Interop.WindowInteropHelper interopHelper;
        HwndSource hwndSource;
        
        public PhotoApp()
        {
            
            InitializeComponent();
            //this.Background = new LinearGradientBrush(Color.FromRgb(204, 204, 204), Color.FromRgb(51, 51, 51), 90);
            this.Background = new LinearGradientBrush(Colors.Black , Color.FromRgb(100, 100, 100), 90);

            //myCanvas.Background
            //this.AddChild(myCanvas);
            ImageSourceConverter converter = new ImageSourceConverter();
            ImageSource source = (ImageSource)converter.ConvertFromString("Sunset.jpg");
            
            TouchImage img = new TouchImage();
            img.Source = source;
            img.Height = 400;
            img.Width = 400;
            img.Name = "Sunset";
            Canvas.SetLeft(img, 100);
            Canvas.SetTop(img, 100);
            //you can extend the event handler just like you would normally do
            img.fingerDown += new TouchImage.fingerDownHandler(img_fingerDown);
            
            //you can set this property, so that the image cannot be dragged ,but still rotated/scaled.
            //img.drag = false;

            //you can set this property, so that the image cannot be scaled or roated, but still dragged.
            //img.rotatescale = false;

            source = (ImageSource)converter.ConvertFromString("Blue Hills.jpg");

            TouchImage img2 = new TouchImage();
            img2.Source = source;
            img2.Height = 400;
            img2.Width = 400;
            img2.Name = "Hills";
            Canvas.SetLeft(img2, 600);
            Canvas.SetTop(img2, 100);

            source = (ImageSource)converter.ConvertFromString("Winter.jpg");

            TouchImage img3 = new TouchImage();
            img3.Source = source;
            img3.Height = 400;
            img3.Width = 400;
            img3.Name = "Winter";
            Canvas.SetLeft(img3, 400);
            Canvas.SetTop(img3, 500);


            myMenu.Items.Add("Open"); 
            
            

            myCanvas.Children.Add(img);
            myCanvas.Children.Add(img2);
            myCanvas.Children.Add(img3);
            
            

            //Button startButton = new Button();
            //startButton.Content = "start tracking";
            //startButton.Width = 100;
            //startButton.Height = 100;
            //Canvas.SetLeft(startButton, 500);
            //Canvas.SetTop(startButton, 500);
            //startButton.Click += new RoutedEventHandler(startButton_Click);

            //myCanvas.Children.Add(startButton);

            


            Loaded += new RoutedEventHandler(PhotoApp_Loaded);
            TouchworkSDK.TouchMessage.InitializeTouchMessages(); 
        }

        void PhotoApp_Loaded(object sender, RoutedEventArgs e)
        {
            interopHelper = new System.Windows.Interop.WindowInteropHelper(this);

            hwndSource = System.Windows.Interop.HwndSource.FromHwnd(interopHelper.Handle);
            hwndSource.AddHook(new System.Windows.Interop.HwndSourceHook(WndProc));
        }

        public IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            TouchworkSDK.TouchEventArgs e;

            if ((uint)msg == TouchworkSDK.TouchMessage.WM_IS_TOUCHABLE)
            {
                return (IntPtr)5;
            }

            if ((uint)msg == TouchworkSDK.TouchMessage.WM_TOUCH_DOWN)
            {
                e = new TouchworkSDK.TouchEventArgs((uint)lParam, (uint)wParam);
                myCanvas.OnTouchDown(null, e);
                handled = e.handled;
            }
            else if ((uint)msg == TouchworkSDK.TouchMessage.WM_TOUCH_MOVE)
            {
                e = new TouchworkSDK.TouchEventArgs((uint)lParam, (uint)wParam);
                myCanvas.OnTouchMove(null, e);
                handled = e.handled;
            }
            else if ((uint)msg == TouchworkSDK.TouchMessage.WM_TOUCH_UP)
            {
                e = new TouchworkSDK.TouchEventArgs((uint)lParam, (uint)wParam);
                myCanvas.OnTouchUp(null, e);
                handled = e.handled;
            }
            return (IntPtr)0;
        }



        //you can add to the event handlers like this
        void img_fingerDown(object sender, NUIGroup.MultiTouchFramework.TouchEventArgs e)
        {
            //Debug.WriteLine("finger down in main app. Finger ID: " + e.data.ID);
        }

        void startButton_Click(object sender, RoutedEventArgs e)
        {
            //run this to start finger tracking on the TouchCanvas
            //if this is run in the main method, everything is not initialized
            //i'm looking into fix for that
            myCanvas.startTracking();
            this.Close();
        }

        
    }
}