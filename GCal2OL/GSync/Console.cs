using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace GSync
{
    public partial class Console : Form
    {
        private static Console m_Console = null;
        private delegate void PrintDelegate(string text);

        public static Console GSyncConsole
        {
            get
            {
                if (m_Console == null)
                    m_Console = new Console();
                return Console.m_Console;
            }
            set { Console.m_Console = value; }
        }

        private Console()
        {
            InitializeComponent();
        }

        public static void WriteLine(string text)
        {
            GSyncConsole.Print(text);
        }
        public void Print(string text)
        {
            if (rtfConsole.InvokeRequired)
            {
                rtfConsole.Invoke(new PrintDelegate(WriteLine), text);
                return;
            }

            rtfConsole.AppendText("> " + text + "\n");
            rtfConsole.ScrollToCaret();
        }

        private void Console_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }
    }
}
