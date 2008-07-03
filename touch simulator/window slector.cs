using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace touch_simulator
{
	public partial class window_slector : Form
	{
        private Image _finderHome;
        private Image _finderGone;
        private Cursor _cursorDefault;
        private Cursor _cursorFinder;
        private IntPtr _hPreviousWindow;
        public bool _capturing;
        public int _hwnd;

		public window_slector()
		{
			InitializeComponent();

            _cursorDefault = Cursor.Current;

            _cursorFinder = new Cursor("Finder.cur");
            _finderHome = (Image)touch_simulator.Properties.Resources.FinderHome1;
            _finderGone = (Image)touch_simulator.Properties.Resources.FinderGone1;

            _pictureBox.Image = _finderHome;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			int.TryParse(txt_Hwnd.Text,out _hwnd);
		}

        private void _pictureBox_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                this.CaptureMouse(true);
            }
        }

        protected override void WndProc(ref Message m)
        {
            switch (m.Msg)
            {
                case (int)Win32.WindowMessages.WM_LBUTTONUP:
                    this.CaptureMouse(false);
                    break;
                
                case (int)Win32.WindowMessages.WM_MOUSEMOVE:
                    this.HandleMouseMovements();
                    break;
            };

            base.WndProc(ref m);
        }

        private void CaptureMouse(bool captured)
        {
            if (captured)
            {
                Win32.SetCapture(this.Handle);
                Cursor.Current = _cursorFinder;
                _pictureBox.Image = _finderGone;
            }
            else
            {
                Win32.ReleaseCapture();
                Cursor.Current = _cursorDefault;
                _pictureBox.Image = _finderHome;

                if (_hPreviousWindow != IntPtr.Zero)
                {
                    Refresh(_hPreviousWindow);
                    _hPreviousWindow = IntPtr.Zero;
                }
            }
            _capturing = captured;
        }

        private void HandleMouseMovements()
        {
            // if nth is captured, return
            if (!_capturing)
                return;

            try
            {
                IntPtr hWnd = Win32.WindowFromPoint(Cursor.Position);

                if (_hPreviousWindow != IntPtr.Zero && _hPreviousWindow != hWnd)
                    Refresh(_hPreviousWindow);

                if (hWnd == IntPtr.Zero)
                {
                    txt_Hwnd.Text = null;
                }
                else
                {
                    _hPreviousWindow = hWnd;
                    txt_Hwnd.Text = hWnd.ToString("X");
                    HighlightWindow(hWnd);
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        public void HighlightWindow(IntPtr hWnd)
        {
            const float penWidth = 3;
            Win32.Rect rc = new Win32.Rect();
            Win32.GetWindowRect(hWnd, ref rc);

            IntPtr hDC = Win32.GetWindowDC(hWnd);
            if (hDC != IntPtr.Zero)
            {
                using (Pen pen = new Pen(Color.Black, penWidth))
                {
                    using (Graphics g = Graphics.FromHdc(hDC))
                    {
                        g.DrawRectangle(pen, 0, 0, rc.right - rc.left - (int)penWidth, rc.bottom - rc.top - (int)penWidth);
                    }
                }
            }
            Win32.ReleaseDC(hWnd, hDC);
        }

        /// Forces a window to refresh, to eliminate any highlighting
        public static void Refresh(IntPtr hWnd)
        {
            Win32.InvalidateRect(hWnd, IntPtr.Zero, 1);
            Win32.UpdateWindow(hWnd);
            Win32.RedrawWindow(hWnd, IntPtr.Zero, IntPtr.Zero, Win32.RDW_FRAME | Win32.RDW_INVALIDATE | Win32.RDW_UPDATENOW | Win32.RDW_ALLCHILDREN);
        }
	}
}