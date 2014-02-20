using murix_utils;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StellarisXbox
{
    class Program
    {



        static int range_get(double scale, int min, int max) {
                return min + (int)((max - min) * scale);
        }

        static void Main(string[] args)
        {
            try
            {
                Gamepad_State_SlimDX joy = new Gamepad_State_SlimDX(SlimDX.XInput.UserIndex.One);
                SerialPort port = new SerialPort();
                port.PortName = "COM4";
                port.BaudRate = 115200;
                port.Open();

                while (true)
                {
                    joy.Update();


                    string cmd = "pilot(";
                    cmd += range_get(joy.LeftStick.Position.X,0,254);
                    cmd += ",";
                    cmd += range_get(joy.LeftStick.Position.Y,0,254);
                    cmd += ",";
                    cmd += range_get(joy.RightStick.Position.X,0,254);
                    cmd += ",";
                    cmd += range_get(joy.RightStick.Position.Y, 0, 254);
                    cmd += ",";

                    if (joy.A)
                    {
                        cmd += "1";
                    }
                    else
                    {
                        cmd += "0";
                    }

                    cmd += ",";

                    if (joy.B)
                    {
                        cmd += "1";
                    }
                    else
                    {
                        cmd += "0";
                    }

                    cmd += ")";

                    //Console.Write(cmd);

                    DateTime t_start = DateTime.Now;
                    port.DiscardOutBuffer();
                    port.DiscardInBuffer();
                    port.ReadTimeout = 30;
                    port.WriteLine(cmd + "\r");
                    try
                    {
                        string txt = port.ReadTo("ok");
                        Console.Write(txt.Replace("\r", "").Replace("\n", ""));
                        TimeSpan diff = DateTime.Now - t_start;
                         Console.WriteLine(" dt="+diff.TotalMilliseconds);
                    }
                    catch (Exception) {
                        TimeSpan diff = DateTime.Now - t_start;
                        Console.WriteLine(" dt=" + diff.TotalMilliseconds);
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                Console.ReadKey();
            }
        }
    }
}
