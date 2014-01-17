using murix_utils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace DroneUdpSendJoystick
{
    class Program
    {
        static void Main(string[] args)
        {

            while (true)
            {
                try
                {
                    Gamepad_State_SlimDX joy = new Gamepad_State_SlimDX(SlimDX.XInput.UserIndex.One);
                    UdpClient udpClient = new UdpClient();

                    udpClient.Connect("192.168.1.1", 3000);

                    Int16 pitch_now = 0;
                    Int16 roll_now = 0;
                    Int16 yaw_now = 0;
                    Int16 throttle_now = 0;
                    Int16 emergency = 0;
                    Int16 takeoff = 0;

                    int tx_count = 0;
                    int tx_timeout_count = 0;
                    double timeouts_perct = 0;

                    while (true)
                    {
                        joy.Update();

                        pitch_now = (Int16)(joy.LeftStick.Position.Y * 128);
                        roll_now = (Int16)(joy.LeftStick.Position.X * 128);
                        yaw_now = (Int16)(joy.RightStick.Position.X * 128);
                        throttle_now = (Int16)(joy.RightStick.Position.Y * 128);

                        emergency = 0;
                        if (joy.B || joy.RightTrigger > 0.5) emergency = 1;

                        takeoff = 0;
                        if (joy.A) takeoff = 1;

                        //
                        string cmd = pitch_now + "|" + roll_now + "|" + throttle_now + "|" + yaw_now + "|" + takeoff + "|" + emergency + "|";

                        //
                        //Console.WriteLine(cmd);

                        //
                        Byte[] sendBytes = Encoding.ASCII.GetBytes(cmd);
                        udpClient.Send(sendBytes, sendBytes.Length);
                        tx_count++;
                        DateTime start = DateTime.Now;

                        while (true)
                        {
                            TimeSpan timeItTook = DateTime.Now - start;
                            if (timeItTook.Milliseconds > 17) //1s/60Hz=16.66ms
                            {
                                tx_timeout_count++;
                                timeouts_perct = ((double)tx_timeout_count / (double)tx_count) * 100;
                                Console.WriteLine("pkt=" + tx_count + " rtt timeouts=" + tx_timeout_count + " (" + timeouts_perct + "%)");
                                break;
                            }
                            if (udpClient.Available > 0)
                            {
                                IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 0);
                                Byte[] receiveBytes = udpClient.Receive(ref RemoteIpEndPoint);
                                Console.WriteLine("pkt=" + tx_count + " rtt=" + timeItTook.TotalMilliseconds + " timeouts=" + tx_timeout_count + " (" + timeouts_perct + "%)");
                                break;
                            }
                        }
                    }

                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                }
            }
        }
    }
}
