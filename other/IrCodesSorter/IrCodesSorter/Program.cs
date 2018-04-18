using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace IrCodesSorter
{
    class Program
    {
        static List<string> ListOfBrands = new List<string>(new string[] {    "NEC",
		                                    "SONY",
		                                    "PANASONIC",
		                                    "JVC",
		                                    "SAMSUNG",
		                                    "WHYNTER",
		                                    "AIWA_RC_T501",
		                                    "LG",
		                                    "SANYO",
		                                    "MITSUBISHI",
		                                    "DISH",
		                                    "SHARP",
		                                    "DENON",
		                                    "PRONTO",
		                                    "LEGO_PF",
                                       });

        public enum TvRemoteCommands : int
        {
	        TVREMOTE_KEY_UNKNOWN = -1,
	        TVREMOTE_KEY_NONE = 0,
	        TVREMOTE_KEY_POWER,
	        TVREMOTE_KEY_MUTE,
	        TVREMOTE_KEY_VOLUMEUP,
	        TVREMOTE_KEY_VOLUMEDOWN,
	        TVREMOTE_KEY_CHANNELUP,
	        TVREMOTE_KEY_CHANNELDOWN,
	        TVREMOTE_KEY_0,
	        TVREMOTE_KEY_1,
	        TVREMOTE_KEY_2,
	        TVREMOTE_KEY_3,
	        TVREMOTE_KEY_4,
	        TVREMOTE_KEY_5,
	        TVREMOTE_KEY_6,
	        TVREMOTE_KEY_7,
	        TVREMOTE_KEY_8,
	        TVREMOTE_KEY_9,
            /*
	        TVREMOTE_KEY_SELECT,
	        TVREMOTE_KEY_UP,
	        TVREMOTE_KEY_DOWN,
	        TVREMOTE_KEY_LEFT,
	        TVREMOTE_KEY_RIGHT,
	        TVREMOTE_KEY_PLAY,
	        TVREMOTE_KEY_STOP,
	        TVREMOTE_KEY_PREV,
	        TVREMOTE_KEY_NEXT,
	        TVREMOTE_KEY_INFO,
	        TVREMOTE_KEY_GUIDE,
	        TVREMOTE_KEY_MENU,
	        TVREMOTE_KEY_JUMP,
             */
        };

        static Dictionary<string, Dictionary<long, TvRemoteCommands>> Results = new Dictionary<string, Dictionary<long, TvRemoteCommands>>();

        static void Main(string[] args)
        {
            ProcessDirectory(new DirectoryInfo(@"C:\Projects\Robots\SSF-PublicLibrary-Robot2018\other\codes"), "");

            StreamWriter writer = new StreamWriter("output_" + DateTime.Now.ToString("yyyyMMddHHmmss") + ".cpp");
            writer.Write("static int decodeTvCommand(decode_results *results)" + Environment.NewLine);
            writer.Write("{" + Environment.NewLine);
            writer.Write("\tuint8_t d = results->decode_type;" + Environment.NewLine);
            writer.Write("\tuint16_t x = results->value;" + Environment.NewLine);
            foreach (var i in Results)
            {
                writer.Write("\t#if DECODE_" + i.Key + Environment.NewLine);
                writer.Write("\tif (d == " + i.Key + ")" + Environment.NewLine);
                writer.Write("\t{" + Environment.NewLine);
                Dictionary<TvRemoteCommands, List<long>> list = new Dictionary<TvRemoteCommands, List<long>>();
                foreach (var j in i.Value)
                {
                    if (list.ContainsKey(j.Value) == false)
                    {
                        list.Add(j.Value, new List<long>());
                    }
                    if (list[j.Value].Contains(j.Key) == false)
                    {
                        list[j.Value].Add(j.Key);
                    }
                }
                foreach (var j in list)
                {
                    writer.Write("\t\tif (0" + Environment.NewLine);
                    foreach (var x in j.Value)
                    {
                        writer.Write("\t\t\t|| x == 0x" + x.ToString("X4") + Environment.NewLine);
                    }
                    writer.Write("\t\t) {" + Environment.NewLine);
                    string key = Enum.GetName(typeof(TvRemoteCommands), j.Key);
                    writer.Write("\t\t\treturn " + key + ";" + Environment.NewLine);
                    writer.Write("\t\t}" + Environment.NewLine);
                }

                writer.Write("\t}" + Environment.NewLine);
                writer.Write("\t#endif // DECODE_" + i.Key + Environment.NewLine);
            }
            writer.Write("\treturn TVREMOTE_KEY_UNKNOWN;" + Environment.NewLine);
            writer.Write("}" + Environment.NewLine);

            writer.Close();

            Console.ReadKey();
        }

        static void ProcessDirectory(DirectoryInfo dnfo, string brand)
        {
            if (string.IsNullOrWhiteSpace(brand) && ListOfBrands.Contains(dnfo.Name.ToUpper()))
            {
                brand = dnfo.Name.ToUpper();
            }
            DirectoryInfo[] dirs = dnfo.GetDirectories();
            foreach (var i in dirs)
            {
                ProcessDirectory(i, brand);
            }

            FileInfo[] files = dnfo.GetFiles();
            foreach (var f in files)
            {
                if (Path.GetExtension(f.FullName).ToLower() != ".csv")
                {
                    continue;
                }
                ProcessFile(f.FullName, brand);
            }
        }

        private static void ProcessFile(string p, string brand)
        {
            string[] lines = File.ReadAllLines(p);

            long? power = null;

            string brand2 = null;

            bool tooManyBits = false;

            for (int i = 1; i < lines.Length; i++)
            {
                string[] columns = lines[i].Split(',');
                if (string.IsNullOrWhiteSpace(brand2))
                {
                    brand2 = columns[1].Trim().ToUpper();
                    if (brand2 != "RC5" && brand2 != "RC6")
                    {
                        string brand3 = brand2.Trim('0', '1', '2', '3', '4', '5', '6', '7', '8', '9').Trim();
                        string numBitsStr = brand2.Substring(brand3.Length);
                        int numBits;
                        if (int.TryParse(numBitsStr, out numBits))
                        {
                            if (numBits > 16)
                            {
                                tooManyBits = true;
                            }
                        }
                    }
                }
            }

            if (tooManyBits)
            {
                return;
            }

            if (string.IsNullOrWhiteSpace(brand) && string.IsNullOrWhiteSpace(brand2))
            {
                return;
            }

            if (string.IsNullOrWhiteSpace(brand2) == false)
            {
                if (ListOfBrands.Contains(brand2))
                {
                    brand = brand2;
                }
            }

            if (ListOfBrands.Contains(brand) == false)
            {
                return;
            }

            if (Results.ContainsKey(brand) == false)
            {
                Results.Add(brand, new Dictionary<long, TvRemoteCommands>());
            }

            var cmdList = Results[brand];

            Console.WriteLine(brand);

            for (int i = 1; i < lines.Length; i++)
            {
                string[] columns = lines[i].Split(',');
                string name = columns[0].ToLower().Trim();
                long x;
                if (long.TryParse(columns[4], out x) == false)
                {
                    continue;
                }
                if (x <= 0)
                {
                    continue;
                }
                if (name.Contains("power"))
                {
                        power = x;
                }
                if (name.Contains("pwr") && power.HasValue == false)
                {
                        power = x;
                }
                if (name.Contains("mute"))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_POWER;
                }
                if (name.Contains("vol") && (name.Contains("up") || name.Contains("+") || name.Contains("plus")))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_VOLUMEUP;
                }
                if (name.Contains("vol") && (name.Contains("down") || name.Contains("-") || name.Contains("dn") || name.Contains("minus")))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_VOLUMEDOWN;
                }
                if (name.Contains("chan") && (name.Contains("up") || name.Contains("+") || name.Contains("plus")))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_CHANNELUP;
                }
                if (name.Contains("chan") && (name.Contains("down") || name.Contains("-") || name.Contains("dn") || name.Contains("minus")))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_CHANNELDOWN;
                }
                for (int n = 0; n < 10; n++)
                {
                    if (name == n.ToString() || ((name.Contains("key") || name.Contains("num")) && name.Contains(n.ToString())))
                    {
                            TvRemoteCommands cmd = (TvRemoteCommands)((int)TvRemoteCommands.TVREMOTE_KEY_0 + (int)n);
                            cmdList[x] = cmd;
                    }
                }
                /*
                if (name.Contains("info"))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_INFO;
                }
                if (name.Contains("menu"))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_MENU;
                }
                if (name.Contains("guide"))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_GUIDE;
                }
                if (name.Contains("jump"))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_JUMP;
                }
                if (name.Contains("select") || name.Contains("center") || name.Contains("middle"))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_SELECT;
                }
                if (name.Contains("up") && name.Contains("vol") == false && name.Contains("chan") == false)
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_UP;
                }
                if (name.Contains("down") && name.Contains("vol") == false && name.Contains("chan") == false)
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_DOWN;
                }
                if (name.Contains("left"))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_LEFT;
                }
                if (name.Contains("right"))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_RIGHT;
                }
                if (name.Contains("play"))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_PLAY;
                }
                if (name.Contains("stop"))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_STOP;
                }
                if (name.Contains("prev"))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_PREV;
                }
                if (name.Contains("next"))
                {
                        cmdList[x] = TvRemoteCommands.TVREMOTE_KEY_NEXT;
                }
                */
            }

            if (power.HasValue)
            {
                cmdList[power.Value] = TvRemoteCommands.TVREMOTE_KEY_POWER;
            }
        }
    }
}
