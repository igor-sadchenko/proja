namespace GSync
{
    partial class Console
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
            this.rtfConsole = new System.Windows.Forms.RichTextBox();
            this.SuspendLayout();
            // 
            // rtfConsole
            // 
            this.rtfConsole.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rtfConsole.Font = new System.Drawing.Font("Consolas", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rtfConsole.Location = new System.Drawing.Point(0, 0);
            this.rtfConsole.Name = "rtfConsole";
            this.rtfConsole.ReadOnly = true;
            this.rtfConsole.Size = new System.Drawing.Size(584, 464);
            this.rtfConsole.TabIndex = 0;
            this.rtfConsole.Text = "";
            // 
            // Console
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(584, 464);
            this.Controls.Add(this.rtfConsole);
            this.Name = "Console";
            this.Text = "GSync Console";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Console_FormClosed);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox rtfConsole;
    }
}

