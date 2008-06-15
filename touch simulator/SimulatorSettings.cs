using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;


namespace touch_simulator
{
	[Serializable]
	class SimulatorSettings
	{
		public List<LinkedList<Blob>> blobList;
		public int Max;
		public SimulatorSettings()
		{
		}


		public SimulatorSettings(List<LinkedList<Blob>> _list , int _max)
		{
			blobList = _list;
			Max = _max;
		}

		public void Save(string path)
		{
			FileStream fs = null;
			try
			{

				fs = new FileStream(path, FileMode.Create);
				BinaryFormatter bf = new BinaryFormatter();
				bf.Serialize(fs, this);

			}
			catch (Exception)
			{
			}
			finally
			{
				if(fs != null)
					fs.Close();
			}
		}

		static public SimulatorSettings Load(string path)
		{
			FileStream fs = null;
			try
			{
				fs = new FileStream(path, FileMode.Open);

				BinaryFormatter bf = new BinaryFormatter();
				SimulatorSettings ss = (SimulatorSettings)bf.Deserialize(fs);
				
				return ss;
			}
			catch(Exception)
			{
				return null;
			}
			finally
			{
				if(fs !=null)
					fs.Close();
			}
		}
	}
}
