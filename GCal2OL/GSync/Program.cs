using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Threading;

namespace GSync
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Console.GSyncConsole.Show();
            Console.WriteLine("GSync Started...");

            SyncMain main = new SyncMain();
            Thread th = new Thread(new ThreadStart(main.Start));
            th.IsBackground = true;
            th.Start();
            Application.ApplicationExit+=new EventHandler(main.Shutdown);
            Application.Run();
        }
    }
}
