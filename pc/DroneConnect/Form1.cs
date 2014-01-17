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
using System.Net.Sockets;
using System.Net;
using murix_utils;
using AvionicsInstrumentControlDemo;
using System.Globalization;

namespace DroneConnect
{
    public partial class Form1 : Form
    {
        public static int ping_timeout = 50;

        public Form1()
        {
            InitializeComponent();
        }

        public static void joystick(){

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
                        Thread.Yield();

                        joy.Update();

                        pitch_now = (Int16)(joy.LeftStick.Position.Y * 512);
                        roll_now = (Int16)(joy.LeftStick.Position.X * 512);
                        yaw_now = (Int16)(joy.RightStick.Position.X * 512);
                        throttle_now = (Int16)(joy.RightStick.Position.Y * 512);

                        emergency = 0;
                        if (joy.B || joy.RightTrigger > 0.5) emergency = 1;

                        takeoff = 0;
                        if (joy.A) takeoff = 1;

                        //
                        string cmd = throttle_now + "|" + pitch_now + "|" + roll_now + "|" + yaw_now + "|" + takeoff + "|" + emergency + "|";

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

        private void Form1_Load(object sender, EventArgs e)
        {
            this.backgroundWorker_angle.RunWorkerAsync();

            new Thread(joystick).Start();
            
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
                chart1.ChartAreas[0].AxisY.Maximum = ping_timeout;
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
            int timeout = ping_timeout;

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
                    if (reply.RoundtripTime > timeout)
                    {
                        return timeout;
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

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            Environment.Exit(0);
        }

        double pitch = 0.0;
        double roll = 0.0;
        double yaw = 0.0;
        private void backgroundWorker_angle_DoWork(object sender, DoWorkEventArgs e)
        {
   

            while (true)
            {

                try
                {
                    UdpClient udpClient = new UdpClient();
                    udpClient.Connect("192.168.1.1", 4000);
                    string cmd = "get";
                    Byte[] sendBytes = Encoding.ASCII.GetBytes(cmd);
                    udpClient.Send(sendBytes, sendBytes.Length);
                    DateTime start = DateTime.Now;
                    while (true)
                    {
                        //Thread.Sleep(17);
                        TimeSpan timeItTook = DateTime.Now - start;
                        if (timeItTook.Milliseconds > 17) //1s/60Hz=16.66ms
                        {
                            break;
                        }
                        if (udpClient.Available > 0)
                        {
                            IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 0);
                            Byte[] receiveBytes = udpClient.Receive(ref RemoteIpEndPoint);
                            string returnData = Encoding.ASCII.GetString(receiveBytes);
                            string[] aa = returnData.Split('|');
                            if (aa.Length >= 3) {
                                pitch = double.Parse(aa[0], CultureInfo.InvariantCulture);
                                roll = double.Parse(aa[1], CultureInfo.InvariantCulture);
                                yaw = double.Parse(aa[2], CultureInfo.InvariantCulture);
                                this.backgroundWorker_angle.ReportProgress(1);
                            }

                            break;
                        }
                        Thread.Sleep(10);
                    }
                }
                catch (Exception ex) { 
                
                }
            }
        }

        private void backgroundWorker_angle_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            this.horizonInstrumentControl1.SetAttitudeIndicatorParameters(pitch, roll);
            this.attitudeIndicatorInstrumentControl1.SetAttitudeIndicatorParameters(0, -yaw);

        }

    }
}
