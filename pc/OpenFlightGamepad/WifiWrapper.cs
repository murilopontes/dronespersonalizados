using SimpleWifi;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace murix_utils
{
    class Wifi_Monitor
    {

        public static int rssi = 0;
        private static Wifi wifi = new Wifi();
        static AutoResetEvent autoEvent;

        private static void verify_and_connect()
        {
            while (run)
            {
                try
                {
                    //get list of access points
                    IEnumerable<AccessPoint> accessPoints = wifi.GetAccessPoints();
                    //scan for ardrone
                    foreach (AccessPoint ap in accessPoints)
                    {
                        if (ap.Name.StartsWith("ardrone_"))
                        {
                            rssi = (int)ap.SignalStrength;

                            //verify connection
                            if (!ap.IsConnected)
                            {
                                //connect if not connected
                                AuthRequest authRequest = new AuthRequest(ap);
                                ap.Connect(authRequest);

                                autoEvent = new AutoResetEvent(false);
                                autoEvent.WaitOne(20000);
                            }
                        }
                    }


                    Thread.Sleep(250);

                }
                catch (Exception ex)
                {
                    Console.WriteLine("update_rssi ex=" + ex.Message);
                }
            }
        }

        static bool run = true;


        static void wifi_ConnectionStatusChanged(object sender, WifiStatusEventArgs e)
        {
            autoEvent.Set();
        }

        static Thread fd;

        public static void Start()
        {
            wifi.ConnectionStatusChanged += wifi_ConnectionStatusChanged;
            fd = new Thread(verify_and_connect);
            fd.Start();
        }

        public static void Stop()
        {
            wifi.Disconnect();
            run = false;
            fd.Abort();
        }




    }
}
