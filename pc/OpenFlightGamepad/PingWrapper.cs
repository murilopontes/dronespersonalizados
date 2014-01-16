using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace murix_utils
{
    class Ping_Monitor
    {
        public static int rtt = 0;

        private static void work_ping() { 
            while(true){
                rtt = (int) ping("192.168.1.1");
                Thread.Sleep(1000);
            }
        }

        static Thread fd;

        public static void Start() {
            fd = new Thread(work_ping);
            fd.Start();
        }

        public static void Stop() {
            fd.Abort();
        }

        private static long ping(string destination_ip) {

            //ping timeout
            int timeout = 100;

            try
            {
                //ping class
                Ping pingSender = new Ping();

                //options of ping
                PingOptions options = new PingOptions();
                options.DontFragment = true;

                //trash data for test
                string data = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
                byte[] buffer = Encoding.ASCII.GetBytes(data);


                //send it
                PingReply reply = pingSender.Send(destination_ip, timeout, buffer, options);

                //report it
                if (reply.Status == IPStatus.Success)
                {
                    if (reply.RoundtripTime > 100)
                    {
                        return 100;
                    }
                    return reply.RoundtripTime;
                }
                else
                {
                    return timeout;
                }
            }
            catch (Exception) {
                return timeout;
            }
        
        }
    }
}
