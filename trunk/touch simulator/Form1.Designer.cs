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
            this.frameTrackbar = new System.Windows.Forms.TrackBar();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.btnLoad = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            this.chkNotifyChildren = new System.Windows.Forms.CheckBox();
            this.txtMonitor = new System.Windows.Forms.TextBox();
            this.chk_send_Mouse = new System.Windows.Forms.CheckBox();
            this.btnSelectWnd = new System.Windows.Forms.Button();
            this.chk_ScreenCoord = new System.Windows.Forms.CheckBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnRun = new System.Windows.Forms.CheckBox();
            this.btn_Clear = new System.Windows.Forms.Button();
            this.num_TimePerFrame = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.pictureBox = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.frameTrackbar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.num_TimePerFrame)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // frameTrackbar
            // 
            this.frameTrackbar.BackColor = System.Drawing.SystemColors.Window;
            this.frameTrackbar.Location = new System.Drawing.Point(0, 27);
            this.frameTrackbar.Maximum = 1;
            this.frameTrackbar.Name = "frameTrackbar";
            this.frameTrackbar.Size = new System.Drawing.Size(145, 45);
            this.frameTrackbar.TabIndex = 2;
            this.frameTrackbar.Scroll += new System.EventHandler(this.frameTrackbar_Scroll);
            this.frameTrackbar.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // btnLoad
            // 
            this.btnLoad.Location = new System.Drawing.Point(285, 29);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(40, 23);
            this.btnLoad.TabIndex = 4;
            this.btnLoad.Text = "Load";
            this.btnLoad.UseVisualStyleBackColor = true;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(329, 29);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(40, 23);
            this.btnSave.TabIndex = 5;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // chkNotifyChildren
            // 
            this.chkNotifyChildren.AutoSize = true;
            this.chkNotifyChildren.BackColor = System.Drawing.SystemColors.Window;
            this.chkNotifyChildren.Checked = true;
            this.chkNotifyChildren.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkNotifyChildren.Location = new System.Drawing.Point(158, 4);
            this.chkNotifyChildren.Name = "chkNotifyChildren";
            this.chkNotifyChildren.Size = new System.Drawing.Size(90, 17);
            this.chkNotifyChildren.TabIndex = 6;
            this.chkNotifyChildren.Text = "Notify Childs?";
            this.chkNotifyChildren.UseVisualStyleBackColor = false;
            // 
            // txtMonitor
            // 
            this.txtMonitor.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.txtMonitor.Location = new System.Drawing.Point(0, 463);
            this.txtMonitor.Multiline = true;
            this.txtMonitor.Name = "txtMonitor";
            this.txtMonitor.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtMonitor.Size = new System.Drawing.Size(676, 73);
            this.txtMonitor.TabIndex = 7;
            // 
            // chk_send_Mouse
            // 
            this.chk_send_Mouse.AutoSize = true;
            this.chk_send_Mouse.BackColor = System.Drawing.SystemColors.Window;
            this.chk_send_Mouse.Location = new System.Drawing.Point(158, 22);
            this.chk_send_Mouse.Name = "chk_send_Mouse";
            this.chk_send_Mouse.Size = new System.Drawing.Size(82, 17);
            this.chk_send_Mouse.TabIndex = 8;
            this.chk_send_Mouse.Text = "Mouse too?";
            this.chk_send_Mouse.UseVisualStyleBackColor = false;
            this.chk_send_Mouse.CheckedChanged += new System.EventHandler(this.chk_send_Mouse_CheckedChanged);
            // 
            // btnSelectWnd
            // 
            this.btnSelectWnd.Location = new System.Drawing.Point(375, 4);
            this.btnSelectWnd.Name = "btnSelectWnd";
            this.btnSelectWnd.Size = new System.Drawing.Size(82, 23);
            this.btnSelectWnd.TabIndex = 9;
            this.btnSelectWnd.Text = "Find Window";
            this.btnSelectWnd.UseVisualStyleBackColor = true;
            this.btnSelectWnd.Click += new System.EventHandler(this.btnSelectWnd_Click);
            // 
            // chk_ScreenCoord
            // 
            this.chk_ScreenCoord.AutoSize = true;
            this.chk_ScreenCoord.BackColor = System.Drawing.SystemColors.Window;
            this.chk_ScreenCoord.Location = new System.Drawing.Point(158, 40);
            this.chk_ScreenCoord.Name = "chk_ScreenCoord";
            this.chk_ScreenCoord.Size = new System.Drawing.Size(125, 17);
            this.chk_ScreenCoord.TabIndex = 10;
            this.chk_ScreenCoord.Text = "Screen Coordinates?";
            this.chk_ScreenCoord.UseVisualStyleBackColor = false;
            this.chk_ScreenCoord.CheckedChanged += new System.EventHandler(this.chk_ScreenCoord_CheckedChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.SystemColors.Window;
            this.label1.Location = new System.Drawing.Point(50, 47);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(46, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "Timeline";
            // 
            // btnRun
            // 
            this.btnRun.Appearance = System.Windows.Forms.Appearance.Button;
            this.btnRun.Location = new System.Drawing.Point(285, 4);
            this.btnRun.Name = "btnRun";
            this.btnRun.Size = new System.Drawing.Size(84, 23);
            this.btnRun.TabIndex = 3;
            this.btnRun.Text = "Run from here";
            this.btnRun.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.btnRun.UseVisualStyleBackColor = true;
            this.btnRun.CheckedChanged += new System.EventHandler(this.btnRun_CheckedChanged);
            // 
            // btn_Clear
            // 
            this.btn_Clear.Location = new System.Drawing.Point(375, 29);
            this.btn_Clear.Name = "btn_Clear";
            this.btn_Clear.Size = new System.Drawing.Size(82, 23);
            this.btn_Clear.TabIndex = 13;
            this.btn_Clear.Text = "Clear";
            this.btn_Clear.UseVisualStyleBackColor = true;
            this.btn_Clear.Click += new System.EventHandler(this.btn_Clear_Click);
            // 
            // num_TimePerFrame
            // 
            this.num_TimePerFrame.Increment = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.num_TimePerFrame.Location = new System.Drawing.Point(95, 3);
            this.num_TimePerFrame.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.num_TimePerFrame.Name = "num_TimePerFrame";
            this.num_TimePerFrame.Size = new System.Drawing.Size(43, 20);
            this.num_TimePerFrame.TabIndex = 14;
            this.num_TimePerFrame.Value = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.num_TimePerFrame.ValueChanged += new System.EventHandler(this.num_TimePerFrame_ValueChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.SystemColors.Window;
            this.label2.Location = new System.Drawing.Point(5, 5);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(89, 13);
            this.label2.TabIndex = 15;
            this.label2.Text = "Time(m.s.)/Frame";
            // 
            // pictureBox
            // 
            this.pictureBox.BackColor = System.Drawing.Color.White;
            this.pictureBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBox.Location = new System.Drawing.Point(0, 0);
            this.pictureBox.Name = "pictureBox";
            this.pictureBox.Size = new System.Drawing.Size(676, 536);
            this.pictureBox.TabIndex = 0;
            this.pictureBox.TabStop = false;
            this.pictureBox.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseMove);
            this.pictureBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseDown);
            this.pictureBox.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBox_Paint);
            this.pictureBox.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseUp);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(676, 536);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.num_TimePerFrame);
            this.Controls.Add(this.btn_Clear);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.chk_ScreenCoord);
            this.Controls.Add(this.btnSelectWnd);
            this.Controls.Add(this.chk_send_Mouse);
            this.Controls.Add(this.txtMonitor);
            this.Controls.Add(this.chkNotifyChildren);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.btnLoad);
            this.Controls.Add(this.btnRun);
            this.Controls.Add(this.frameTrackbar);
            this.Controls.Add(this.pictureBox);
            this.HelpButton = true;
            this.Name = "Form1";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Touchwork Simulator";
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseUp);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseDown);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseMove);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            ((System.ComponentModel.ISupportInitialize)(this.frameTrackbar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.num_TimePerFrame)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

        private System.Windows.Forms.TrackBar frameTrackbar;
		private System.Windows.Forms.Timer timer1;
		private System.Windows.Forms.Button btnLoad;
		private System.Windows.Forms.Button btnSave;
		private System.Windows.Forms.CheckBox chkNotifyChildren;
		private System.Windows.Forms.TextBox txtMonitor;
		private System.Windows.Forms.CheckBox chk_send_Mouse;
		private System.Windows.Forms.Button btnSelectWnd;
        private System.Windows.Forms.CheckBox chk_ScreenCoord;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox btnRun;
        private System.Windows.Forms.Button btn_Clear;
        private System.Windows.Forms.NumericUpDown num_TimePerFrame;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.PictureBox pictureBox;
	}
}

