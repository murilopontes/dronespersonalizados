using SimpleWifi;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections.ObjectModel;
using System.Windows.Forms.DataVisualization.Charting;
using System.Net.NetworkInformation;
using System.Threading;

namespace DroneConnect
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
            chart1.ChartAreas[0].AxisY.Title = "rtt(ms)";
            chart1.ChartAreas[0].AxisX.Title = "packet number";
            chart1.Series.Clear();
            ping_rtt_ms.ChartType = SeriesChartType.Line;
            chart1.Series.Add(ping_rtt_ms);

        }

        Series ping_rtt_ms = new Series("ping RTT (ms)");
        Wifi wifi = new Wifi();
        long packet = 0;

        private void timer1_Tick(object sender, EventArgs e)
        {
            try
            {
                
                //get list of access points
                IEnumerable<AccessPoint> accessPoints = wifi.GetAccessPoints();
                //scan for ardrone

                while (ping_rtt_ms.Points.Count > 10) {
                    ping_rtt_ms.Points.RemoveAt(0);
                }
                chart1.ChartAreas[0].AxisY.Maximum = 105;
                chart1.ChartAreas[0].AxisY.Minimum = 0;
                chart1.ChartAreas[0].RecalculateAxesScale();
                ping_rtt_ms.Points.AddXY(packet,(int)ping("192.168.1.1"));
                packet++;

                foreach (AccessPoint ap in accessPoints)
                {
                  

                    if (ap.Name.StartsWith("ardrone_"))
                    {
                        int rssi = (int)ap.SignalStrength;
                       

                        //verify connection
                        if (!ap.IsConnected)
                        {
                            //connect if not connected
                            AuthRequest authRequest = new AuthRequest(ap);
                            ap.Connect(authRequest);
                        }
                    }
                }

                dataGridView1.DataSource = accessPoints;
                dataGridView1.Update();
            }
            catch (Exception ex) { 
            
            }
        }


        private static long ping(string destination_ip)
        {

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
                string data = "aaaaaa";
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
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
                return timeout;
            }

        }

    }
}
