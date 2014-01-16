using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace murix_utils
{
    class UDP_TX_Gamepad
    {


        private static bool run = true;

        private static void gamepad_work()
        {
            //ardrone connection
            Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            IPAddress ardrone_ip = IPAddress.Parse("192.168.1.1");
            IPEndPoint ep = new IPEndPoint(ardrone_ip, 11000);

            Gamepad_State_SlimDX joy = new Gamepad_State_SlimDX(SlimDX.XInput.UserIndex.One);

            float pitch_pre = 0;
            float roll_pre = 0;
            float yaw_pre = 0;
            float throttle_pre = 0;

            float pitch_now = 0;
            float roll_now = 0;
            float yaw_now = 0;
            float throttle_now = 0;

            while (run)
            {
                //
                pitch_pre = pitch_now;
                roll_pre = roll_now;
                yaw_pre = yaw_now;
                throttle_pre = throttle_now;
                //
                joy.Update();
                //
                pitch_now = joy.LeftStick.Position.Y;
                roll_now = joy.LeftStick.Position.X;
                yaw_now = joy.RightStick.Position.X;
                throttle_now = joy.RightStick.Position.Y;
                //
                if(
                    pitch_pre!=pitch_now ||
                    roll_pre!=roll_now ||
                    yaw_pre!=yaw_now ||
                    throttle_pre!=throttle_now 
                    ){
                        //pitch, roll, yaw, throttle
                        String cmd_txt = pitch_now + "|" + roll_now + "|" + yaw_now + "|" + throttle_now;
                        Console.WriteLine(cmd_txt);
                        //convert to binary
                        byte[] cmd_bin = System.Text.Encoding.ASCII.GetBytes(cmd_txt);
                        s.SendTo(cmd_bin, ep);
                
                }


                
            }
        }


        static Thread fd;

        public static void Start()
        {
            fd = new Thread(gamepad_work);
            fd.Start();
        }

        public static void Stop() {
            run = false;
        }

    }
}
