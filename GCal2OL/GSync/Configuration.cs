using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Reflection;
using System.Xml.Serialization;
using System.Drawing;

namespace GSync
{
    /// <summary>
    /// Computer Configuration Settings. Use 'Configuration.Instance' to access the singleton instance ...
    /// </summary>
    public class Configuration
    {
        #region Singleton Code
        private static Configuration m_Configuration = null;

        [XmlIgnore]
        public static Configuration Instance
        {
            get
            {
                if (m_Configuration == null)
                {
                    m_Configuration = new Configuration();
                    //LoadConfiguration();
                }

                return Configuration.m_Configuration;
            }
            set { Configuration.m_Configuration = value; }
        }
        #endregion

        private string applicationName = "MyGSync-1.0b";
        private string username = "a.shaker.axmo@gmail.com";
        private string password = "qwertyui";
        private DateTime lastSync;

        public static string ApplicationName
        {
            get { return m_Configuration.applicationName; }
            set { m_Configuration.applicationName = value; }
        }
        public static string Username
        {
            get { return m_Configuration.username; }
            set { m_Configuration.username = value; }
        }
        public static string Password
        {
            get { return m_Configuration.password; }
            set { m_Configuration.password = value; }
        }
        public static DateTime LastSync
        {
            get { return m_Configuration.lastSync; }
            set { m_Configuration.lastSync = value; }
        }

        private Configuration()
        {
        }

        #region Serilaization Code
        
        private const string m_FileName = "..\\..\\..\\config.txt";

        public static void SaveConfiguration()
        {
            StreamWriter sw = new StreamWriter(m_FileName);
            PropertyInfo[] props = Instance.GetType().GetProperties();

            foreach (PropertyInfo p in props)
            {
                object[] o = p.GetCustomAttributes(false);
                if (o != null && o.Length > 0 && o[0] is XmlIgnoreAttribute)
                    continue;

                sw.WriteLine(String.Format("{0}={1}", p.Name, p.GetValue(Instance, null)));
            }
            sw.Close();
            Console.WriteLine("ConfigurationFile Saved...");
        }
        public static void LoadConfiguration()
        {
            if (!File.Exists(m_FileName))
            {
                StreamWriter sw = File.CreateText(m_FileName);
                sw.Close();
            }
            else
            {
                Type type = Instance.GetType();
                string propertyName, value;
                string[] temp;
                char[] splitChars = new char[] { '=' };
                PropertyInfo propertyInfo;

                string[] settings = File.ReadAllLines(m_FileName);
                foreach (string str in settings)
                {
                    temp = str.Split(splitChars, StringSplitOptions.RemoveEmptyEntries);
                    if (temp.Length == 2)
                    {
                        propertyName = temp[0];
                        value = temp[1];
                        propertyInfo = type.GetProperty(propertyName);
                        if (propertyInfo != null)
                            SetProperty(propertyInfo, value);
                    }
                }
            }
            Console.WriteLine("ConfigurationFile Loaded...");
        }

        private static void SetProperty(PropertyInfo propertyInfo, object value)
        {
            switch (propertyInfo.PropertyType.Name)
            {
                case "Int32":
                    propertyInfo.SetValue(Instance, Convert.ToInt32(value), null);
                    break;
                case "String":
                    propertyInfo.SetValue(Instance, value.ToString(), null);
                    break;
                case "DateTime":
                    propertyInfo.SetValue(Instance, Convert.ToDateTime(value), null);
                    break;
            }
        }
        #endregion
    }
}
