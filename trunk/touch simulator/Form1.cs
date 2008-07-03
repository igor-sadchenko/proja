using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace touch_simulator
{
	public partial class Form1 : Form
	{
		List<LinkedList<Blob>> m_blobs = new List<LinkedList<Blob>>();
		Blob m_currnetBlob = null;
		testForm test_form = new testForm();
		int m_currentID = 0;
		public static TextBox s_txtMonitor = null;
		SimulatorSettings settings;
		public Form1()
		{
			InitializeComponent();
            TWMessagesType.InitializeTouchMessages(); 
			s_txtMonitor = txtMonitor;
			frameTrackbar.Value = 0;
			m_blobs.Add(new LinkedList<Blob>());

			this.Opacity = 0.5;
			timer1.Interval = 1000;
			//test_form.Show();
		}

		private void frameTrackbar_Scroll(object sender, EventArgs e)
		{
			if(m_currnetBlob != null)
			{
				m_currnetBlob.isSelected = false;
				m_currnetBlob = null;
			}
			if (!btnRun.Checked && frameTrackbar.Value >= m_blobs.Count)
				AddFrame();
			pictureBox.Invalidate();
			pictureBox.Update();

		}

		private void btnRun_CheckedChanged(object sender, EventArgs e)
		{
			if (btnRun.Checked)
			{
				timer1.Start();
				btnRun.Text = "Stop";
			}
			else
			{
				timer1.Stop();
				btnRun.Text = "Start";
			}
		}

		private void SimulateFrame()
		{
			if (chkNotifyChildren.Checked)
			{
				if (Messages.targetWindow == 0)
					this.Visible = false;
                IntPtr hwnd = IntPtr.Zero;
				foreach (Blob b in m_blobs[frameTrackbar.Value])
				{
                    Point previousPos = GetPreviousPosition(b.id, frameTrackbar.Value);
                    hwnd = Messages.SendToChildWindows(this.pictureBox, b, previousPos);
                 }
                //Win32.RedrawWindow(hwnd, IntPtr.Zero, IntPtr.Zero, Win32.RDW_FRAME | Win32.RDW_INVALIDATE | Win32.RDW_UPDATENOW | Win32.RDW_ALLCHILDREN);
				if (Messages.targetWindow == 0)
					this.Visible = true;
			}
			else
			{
				foreach (Blob b in m_blobs[frameTrackbar.Value])
				{
					Messages.SendToNextWindow(this, pictureBox, b);
				}
			}
		}

        private Point GetPreviousPosition(int id, int frameno)
        {
            if (frameno == 0)
                return new Point(0, 0);
            foreach (Blob b in m_blobs[frameno - 1])
            {
                if (b.id == id)
                    return b.center;
            }
            return new Point(0,0);
        }

		private void timer1_Tick(object sender, EventArgs e)
		{
			SimulateFrame();
			if (frameTrackbar.Value < frameTrackbar.Maximum - 1)
			{
				frameTrackbar.Value++;
				pictureBox.Invalidate();
				pictureBox.Update();
			}
			else
			{
				timer1.Stop();
				btnRun.Checked = false;
			}
		}

		private void pictureBox_Paint(object sender, PaintEventArgs e)
		{
			Graphics g = e.Graphics;
			using (SolidBrush brush = new SolidBrush(Color.Yellow))
			{
				foreach (Blob b in m_blobs[frameTrackbar.Value])
				{

					if (!b.isVisible)
						brush.Color = Color.LightSteelBlue;
					if (b.isVisible)
						brush.Color = Color.Yellow;
					if (b.isSelected)
						brush.Color = Color.Red;
					b.Draw(g, brush);
				}
			}
		}

		void SelectBlobAtPoint(Point p)
		{
			m_currnetBlob = null;
			foreach (Blob b in m_blobs[frameTrackbar.Value])
			{
				if (b.ContainsPoint(p))
				{
					b.isSelected = true;
					m_currnetBlob = b;
				}
				else
					b.isSelected = false;
			}
		}

		private void Form1_MouseDown(object sender, MouseEventArgs e)
		{
			if (e.Button == MouseButtons.Left)
			{
				SelectBlobAtPoint(e.Location);
			}
			else
			{
				AddNewBlob(e);
			}
			pictureBox.Invalidate();
		}

		private void AddNewBlob(MouseEventArgs e)
		{
			Blob b = new Blob(e.Location, m_currentID++);
			b.type = TWMessagesType.WM_TOUCHDOWN;
			m_blobs[frameTrackbar.Value].AddLast(b);
			for (int i = frameTrackbar.Value + 1; i < frameTrackbar.Maximum; i++)
			{
				Blob b2 = new Blob(b);
				b2.type = TWMessagesType.WM_NONE;
				m_blobs[i].AddLast(b);
			}
		}

		private void Form1_MouseMove(object sender, MouseEventArgs e)
		{
			if (m_currnetBlob != null && e.Button == MouseButtons.Left)
			{
				m_currnetBlob.center = e.Location;
				m_currnetBlob.type = TWMessagesType.WM_TOUCHMOVE;
				pictureBox.Invalidate();
			}
		}

		void AddFrame()
		{
			m_blobs.Add(new LinkedList<Blob>());
			if (m_blobs.Count > 1)
			{
				foreach (Blob b in m_blobs[m_blobs.Count - 2])
				{
					if (b.isVisible)
					{
						Blob b2 = new Blob(b);
						b2.type = TWMessagesType.WM_TOUCHMOVE;
						m_blobs[m_blobs.Count - 1].AddLast(b2);
					}
				}
			}
			frameTrackbar.Maximum++;
		}

		private void Form1_MouseUp(object sender, MouseEventArgs e)
		{
			if (m_currnetBlob != null)
			{

				for (int i = frameTrackbar.Value + 1; i < frameTrackbar.Maximum; i++)
				{

					LinkedListNode<Blob> node = m_blobs[i].Find(m_currnetBlob);
					if (node == null)
						break;
					if (node.Value.type != TWMessagesType.WM_TOUCHUP)
					{
						node.Value.center = m_currnetBlob.center;
						node.Value.type = TWMessagesType.WM_TOUCHMOVE;
					}
				}
			}
		}


		private void Form1_KeyDown(object sender, KeyEventArgs e)
		{
			if (m_currnetBlob == null)
				return;
			if (e.KeyCode == Keys.Delete)
			{
				m_currnetBlob.isVisible = false;
				m_currnetBlob.type = TWMessagesType.WM_TOUCHUP;
				//now erase it form the other lists
				for (int i = frameTrackbar.Value + 1; i < frameTrackbar.Maximum; i++)
				{
					if (!m_blobs[i].Remove(m_currnetBlob))
						break;
				}
				pictureBox.Invalidate();
			}

			else if (e.KeyCode == Keys.Oemplus || e.KeyCode == Keys.Add)
			{
				m_currnetBlob.Pressure+= 2;
				pictureBox.Invalidate();
				for (int i = frameTrackbar.Value + 1; i < frameTrackbar.Maximum; i++)
				{
					LinkedListNode<Blob> node = m_blobs[i].Find(m_currnetBlob);
					if (node == null)
						break;
					node.Value.Pressure = m_currnetBlob.Pressure;
				}
			}
			else if (e.KeyCode == Keys.OemMinus || e.KeyCode == Keys.Subtract)
			{
				m_currnetBlob.Pressure -= 2;
				pictureBox.Invalidate();
				for (int i = frameTrackbar.Value + 1; i < frameTrackbar.Maximum; i++)
				{
					LinkedListNode<Blob> node = m_blobs[i].Find(m_currnetBlob);
					if (node == null)
						break;
					node.Value.Pressure = m_currnetBlob.Pressure;
				}
			}
		}

		private void btnLoad_Click(object sender, EventArgs e)
		{
			OpenFileDialog dlg = new OpenFileDialog();

			if(dlg.ShowDialog() == DialogResult.OK)
			{
				settings = SimulatorSettings.Load(dlg.FileName);
				m_blobs = settings.blobList;
				frameTrackbar.Maximum = settings.Max;
				pictureBox.Invalidate();
				pictureBox.Update();
			}
		}

		private void btnSave_Click(object sender, EventArgs e)
		{
			settings = new SimulatorSettings(m_blobs,frameTrackbar.Maximum);
			SaveFileDialog dlg = new SaveFileDialog();
			if(dlg.ShowDialog() == DialogResult.OK)
			{
				settings.Save(dlg.FileName);
			}
		}

		private void chk_send_Mouse_CheckedChanged(object sender, EventArgs e)
		{
			Messages.send_Mouse_Messages = chk_send_Mouse.Checked;
		}

		private void btnSelectWnd_Click(object sender, EventArgs e)
		{
			window_slector f = new window_slector();
			if (f.ShowDialog() == DialogResult.OK)
				Messages.targetWindow = f._hwnd;
		}

        private void chk_ScreenCoord_CheckedChanged(object sender, EventArgs e)
        {
            Messages.send_Screen_Coordinates = chk_ScreenCoord.Checked;
        }

        private void num_TimePerFrame_ValueChanged(object sender, EventArgs e)
        {
            timer1.Interval = (int) num_TimePerFrame.Value;
        }

        private void btn_Clear_Click(object sender, EventArgs e)
        {
            frameTrackbar.Value = 0;
            m_blobs = new List<LinkedList<Blob>>();
            m_blobs.Add(new LinkedList<Blob>());
            this.pictureBox.Invalidate();
        }
	}
}