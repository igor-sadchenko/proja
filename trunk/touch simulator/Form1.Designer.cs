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
			this.btnLoad = new System.Windows.Forms.Button();
			this.btnSave = new System.Windows.Forms.Button();
			this.chkNotifyChildren = new System.Windows.Forms.CheckBox();
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
			this.frameTrackbar.BackColor = System.Drawing.SystemColors.Window;
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
			// btnLoad
			// 
			this.btnLoad.Location = new System.Drawing.Point(203, 22);
			this.btnLoad.Name = "btnLoad";
			this.btnLoad.Size = new System.Drawing.Size(41, 20);
			this.btnLoad.TabIndex = 4;
			this.btnLoad.Text = "Load";
			this.btnLoad.UseVisualStyleBackColor = true;
			this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
			// 
			// btnSave
			// 
			this.btnSave.Location = new System.Drawing.Point(242, 22);
			this.btnSave.Name = "btnSave";
			this.btnSave.Size = new System.Drawing.Size(44, 20);
			this.btnSave.TabIndex = 5;
			this.btnSave.Text = "Save";
			this.btnSave.UseVisualStyleBackColor = true;
			this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
			// 
			// chkNotifyChildren
			// 
			this.chkNotifyChildren.AutoSize = true;
			this.chkNotifyChildren.BackColor = System.Drawing.SystemColors.Window;
			this.chkNotifyChildren.Location = new System.Drawing.Point(111, 0);
			this.chkNotifyChildren.Name = "chkNotifyChildren";
			this.chkNotifyChildren.Size = new System.Drawing.Size(86, 17);
			this.chkNotifyChildren.TabIndex = 6;
			this.chkNotifyChildren.Text = "Notify Childs";
			this.chkNotifyChildren.UseVisualStyleBackColor = false;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.Add(this.chkNotifyChildren);
			this.Controls.Add(this.btnSave);
			this.Controls.Add(this.btnLoad);
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
		private System.Windows.Forms.Button btnLoad;
		private System.Windows.Forms.Button btnSave;
		private System.Windows.Forms.CheckBox chkNotifyChildren;
	}
}

