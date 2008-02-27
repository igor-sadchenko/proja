namespace touch_simulator
{
	partial class Form1
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.pictureBox = new System.Windows.Forms.PictureBox();
			this.frameTrackbar = new System.Windows.Forms.TrackBar();
			this.btnRun = new System.Windows.Forms.CheckBox();
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			this.chkAutoProgress = new System.Windows.Forms.CheckBox();
			((System.ComponentModel.ISupportInitialize)(this.pictureBox)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.frameTrackbar)).BeginInit();
			this.SuspendLayout();
			// 
			// pictureBox
			// 
			this.pictureBox.BackColor = System.Drawing.Color.White;
			this.pictureBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pictureBox.Location = new System.Drawing.Point(0, 0);
			this.pictureBox.Name = "pictureBox";
			this.pictureBox.Size = new System.Drawing.Size(292, 273);
			this.pictureBox.TabIndex = 0;
			this.pictureBox.TabStop = false;
			this.pictureBox.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseMove);
			this.pictureBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseDown);
			this.pictureBox.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBox_Paint);
			this.pictureBox.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseUp);
			// 
			// frameTrackbar
			// 
			this.frameTrackbar.Location = new System.Drawing.Point(12, 0);
			this.frameTrackbar.Maximum = 1;
			this.frameTrackbar.Name = "frameTrackbar";
			this.frameTrackbar.Size = new System.Drawing.Size(104, 42);
			this.frameTrackbar.TabIndex = 2;
			this.frameTrackbar.Scroll += new System.EventHandler(this.frameTrackbar_Scroll);
			this.frameTrackbar.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
			// 
			// btnRun
			// 
			this.btnRun.Appearance = System.Windows.Forms.Appearance.Button;
			this.btnRun.AutoSize = true;
			this.btnRun.Location = new System.Drawing.Point(203, 0);
			this.btnRun.Name = "btnRun";
			this.btnRun.Size = new System.Drawing.Size(83, 23);
			this.btnRun.TabIndex = 3;
			this.btnRun.Text = "run from here";
			this.btnRun.UseVisualStyleBackColor = true;
			this.btnRun.CheckedChanged += new System.EventHandler(this.btnRun_CheckedChanged);
			// 
			// timer1
			// 
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// chkAutoProgress
			// 
			this.chkAutoProgress.AutoSize = true;
			this.chkAutoProgress.Location = new System.Drawing.Point(192, 25);
			this.chkAutoProgress.Name = "chkAutoProgress";
			this.chkAutoProgress.Size = new System.Drawing.Size(94, 17);
			this.chkAutoProgress.TabIndex = 4;
			this.chkAutoProgress.Text = "Auto Progress";
			this.chkAutoProgress.UseVisualStyleBackColor = true;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.Add(this.chkAutoProgress);
			this.Controls.Add(this.btnRun);
			this.Controls.Add(this.frameTrackbar);
			this.Controls.Add(this.pictureBox);
			this.Name = "Form1";
			this.Text = "Simulator";
			this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseUp);
			this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseDown);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseMove);
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
			((System.ComponentModel.ISupportInitialize)(this.pictureBox)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.frameTrackbar)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.PictureBox pictureBox;
		private System.Windows.Forms.TrackBar frameTrackbar;
		private System.Windows.Forms.CheckBox btnRun;
		private System.Windows.Forms.Timer timer1;
		private System.Windows.Forms.CheckBox chkAutoProgress;
	}
}

