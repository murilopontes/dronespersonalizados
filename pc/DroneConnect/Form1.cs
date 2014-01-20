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
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.IO;

namespace DroneConnect
{
    public partial class Form1 : Form
    {

        #region main window

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //navboard
            navboard_init();
            this.backgroundWorker_navboard.RunWorkerAsync();

            //ping
            ping_monitor_init();
            this.backgroundWorker_ping.RunWorkerAsync();

            //wifi
            this.backgroundWorker_wifi.RunWorkerAsync();

            //joystick
            this.backgroundWorker_joystick.RunWorkerAsync();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            Environment.Exit(0);
        }

        #endregion


        #region wifi monitor

        IEnumerable<AccessPoint> accessPoints;
  
        private void backgroundWorker_wifi_DoWork(object sender, DoWorkEventArgs e)
        {
            Wifi wifi = new Wifi();
            while (true)
            {
                try
                {
                    //get list of access points
                    accessPoints = wifi.GetAccessPoints();
                    //scan for ardrone
                    foreach (AccessPoint ap in accessPoints)
                    {
                        if (ap.Name.StartsWith("ardrone_"))
                        {
                            //verify connection
                            if (!ap.IsConnected)
                            {
                                //connect if not connected
                                AuthRequest authRequest = new AuthRequest(ap);
                                ap.Connect(authRequest);
                                break;
                            }
                        }
                    }
                    backgroundWorker_wifi.ReportProgress(1);
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                }
                Thread.Sleep(500);
            }
        }

        private void backgroundWorker_wifi_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            dataGridView1.DataSource = accessPoints;
            dataGridView1.Update();
        }


        #endregion


        #region instrumentation

        Series s_angle_pitch = new Series("angle pitch");
        Series s_angle_roll = new Series("angle roll");
        Series s_angle_yaw = new Series("angle yaw");

        Series s_speed_pitch = new Series("speed pitch");
        Series s_speed_roll = new Series("speed roll");
        Series s_speed_yaw = new Series("speed yaw");

        Series s_vbat = new Series("vbat");

        Series s_front_left = new Series("front_left");
        Series s_front_right = new Series("front_right");
        Series s_rear_left = new Series("rear_left");
        Series s_rear_right = new Series("rear_right");


        double angle_pitch = 0.0;
        double angle_roll = 0.0;
        double angle_yaw = 0.0;

        double speed_pitch = 0.0;
        double speed_roll = 0.0;
        double speed_yaw = 0.0;

        double vbat = 0.0;
        double front_left = 0.0;
        double front_right = 0.0;
        double rear_left = 0.0;
        double rear_right = 0.0;

        private void navboard_init() {

            s_front_left.ChartType = SeriesChartType.FastLine;
            s_front_right.ChartType = SeriesChartType.FastLine;
            s_rear_left.ChartType = SeriesChartType.FastLine;
            s_rear_right.ChartType = SeriesChartType.FastLine;

            this.chart_motors.Series.Add(s_front_left);
            this.chart_motors.Series.Add(s_front_right);
            this.chart_motors.Series.Add(s_rear_left);
            this.chart_motors.Series.Add(s_rear_right);

            s_angle_pitch.ChartType = SeriesChartType.FastLine;
            s_angle_roll.ChartType = SeriesChartType.FastLine;
            s_angle_yaw.ChartType = SeriesChartType.FastLine;

            this.chart_angle.Series.Add(s_angle_pitch);
            this.chart_angle.Series.Add(s_angle_roll);
            this.chart_angle.Series.Add(s_angle_yaw);

            s_speed_pitch.ChartType = SeriesChartType.FastLine;
            s_speed_roll.ChartType = SeriesChartType.FastLine;
            s_speed_yaw.ChartType = SeriesChartType.FastLine;

            this.chart_rate.Series.Add(s_speed_pitch);
            this.chart_rate.Series.Add(s_speed_roll);
            this.chart_rate.Series.Add(s_speed_yaw);

            s_vbat.ChartType = SeriesChartType.FastLine;
            this.chart_vbat.Series.Add(s_vbat);

    
        }

        string udp_send_recv(int port,string cmd,int timeout_ms) {
            string returnData = "";

            try
            {
                //
                UdpClient udpClient = new UdpClient();
                udpClient.Connect("192.168.1.1", port);

                //
                Byte[] sendBytes = Encoding.ASCII.GetBytes(cmd);
                udpClient.Send(sendBytes, sendBytes.Length);

                //
                DateTime start = DateTime.Now;
                while (true)
                {
                    //
                    TimeSpan timeItTook = DateTime.Now - start;

                    //exit by timeout
                    if (timeItTook.Milliseconds > timeout_ms)
                    {
                        break;
                    }

                    //exit by receive data
                    if (udpClient.Available > 0)
                    {
                        IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 0);
                        Byte[] receiveBytes = udpClient.Receive(ref RemoteIpEndPoint);
                        returnData = Encoding.ASCII.GetString(receiveBytes);
                    }
                }
            }
            catch (Exception ex) {
                Console.WriteLine(ex.ToString());
            }
            return returnData;
        }

        private void backgroundWorker_navboard_DoWork(object sender, DoWorkEventArgs e)
        {
            //
            while (true)
            {
                //
                try
                {
                    //
                    Thread.Sleep(33);

                    ///////////////////////////////////////////////////////////////////////////////////////////
                    string json_vbat = udp_send_recv(4000, "get", 33);
                    var o_vbat = (JObject)JsonConvert.DeserializeObject(json_vbat);
                    vbat = (double) o_vbat["vbat"];

                    //////////////////////////////////////////////////////////////////////////////////////////
                    string json_motors = udp_send_recv(6000, "get", 33);
                    //Console.WriteLine(json_motors);
                    var o_motors = (JObject)JsonConvert.DeserializeObject(json_motors);
                    front_left  = (double)o_motors["front_left"];
                    front_right = (double)o_motors["front_right"];
                    rear_left   = (double)o_motors["rear_left"];
                    rear_right  = (double)o_motors["rear_right"];

                    //////////////////////////////////////////////////////////////////////////////////////////
                    /*
                                angle_pitch = double.Parse(aa[0], CultureInfo.InvariantCulture);
                                angle_roll = double.Parse(aa[1], CultureInfo.InvariantCulture);
                                angle_yaw = double.Parse(aa[2], CultureInfo.InvariantCulture);
                                speed_pitch = double.Parse(aa[3], CultureInfo.InvariantCulture);
                                speed_roll = double.Parse(aa[4], CultureInfo.InvariantCulture);
                                speed_yaw = double.Parse(aa[5], CultureInfo.InvariantCulture);
                    */

                                this.backgroundWorker_navboard.ReportProgress(1);
                }
                catch (Exception ex) {
                    Console.WriteLine(ex.ToString());
                }
                Thread.Sleep(33);
            }
        }

        private void backgroundWorker_navboard_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            this.horizonInstrumentControl1.SetAttitudeIndicatorParameters(angle_pitch, angle_roll);
            this.attitudeIndicatorInstrumentControl1.SetAttitudeIndicatorParameters(0, -angle_yaw);

            int max = 30*30;

            while (s_angle_pitch.Points.Count > max) s_angle_pitch.Points.RemoveAt(0);
            while (s_speed_pitch.Points.Count > max) s_speed_pitch.Points.RemoveAt(0);
            s_angle_pitch.Points.AddY(angle_pitch);
            s_speed_pitch.Points.AddY(speed_pitch);

            while (s_angle_roll.Points.Count > max) s_angle_roll.Points.RemoveAt(0);
            while (s_speed_roll.Points.Count > max) s_speed_roll.Points.RemoveAt(0);
            s_angle_roll.Points.AddY(angle_roll);
            s_speed_roll.Points.AddY(speed_roll);

            while (s_angle_yaw.Points.Count > max) s_angle_yaw.Points.RemoveAt(0);
            while (s_speed_yaw.Points.Count > max) s_speed_yaw.Points.RemoveAt(0);
            s_angle_yaw.Points.AddY(angle_yaw);
            s_speed_yaw.Points.AddY(speed_yaw);

            while (s_vbat.Points.Count > max) s_vbat.Points.RemoveAt(0);
            s_vbat.Points.AddY(vbat);



            while (s_front_left.Points.Count > max) s_front_left.Points.RemoveAt(0); s_front_left.Points.AddY(front_left);
            while (s_front_right.Points.Count > max) s_front_right.Points.RemoveAt(0); s_front_right.Points.AddY(front_right);
            while (s_rear_left.Points.Count > max) s_rear_left.Points.RemoveAt(0); s_rear_left.Points.AddY(rear_left);
            while (s_rear_right.Points.Count > max) s_rear_right.Points.RemoveAt(0); s_rear_right.Points.AddY(rear_right);


            chart_angle.ChartAreas[0].RecalculateAxesScale();
            chart_rate.ChartAreas[0].RecalculateAxesScale();
            chart_vbat.ChartAreas[0].RecalculateAxesScale();
            chart_motors.ChartAreas[0].RecalculateAxesScale();


            chart_vbat.ChartAreas[0].AxisY.IsStartedFromZero = false;
            chart_motors.ChartAreas[0].AxisY.IsStartedFromZero = false;
        }

        #endregion


        #region joystick

        class joystick {
            public Int16 pitch;
            public Int16 roll;
            public Int16 throttle;
            public Int16 yaw;
            public Int16 emergency;
            public Int16 takeoff;
        }

        private void backgroundWorker_joystick_DoWork(object sender, DoWorkEventArgs e)
        {
            while (true) {
                Thread.Sleep(10);
                try
                {
                    Gamepad_State_SlimDX joy = new Gamepad_State_SlimDX(SlimDX.XInput.UserIndex.One);
                    joy.Update();

                    joystick js = new joystick();
                    js.pitch    = (Int16)(joy.LeftStick.Position.Y * 511);
                    js.roll     = (Int16)(joy.LeftStick.Position.X * 511);
                    js.throttle = (Int16)(joy.RightStick.Position.Y * 511);
                    js.yaw      = (Int16)(joy.RightStick.Position.X * 511);

                    if (joy.B || joy.RightTrigger > 0.5) js.emergency = 1;
                    else js.emergency = 0;

                    if (joy.A) js.takeoff = 1;
                    else js.takeoff = 0;

                    string json = JsonConvert.SerializeObject(js);

                    UdpClient udpClient = new UdpClient();
                    udpClient.Connect("192.168.1.1", 3000);
                    Byte[] sendBytes = Encoding.ASCII.GetBytes(json);
                    udpClient.Send(sendBytes, sendBytes.Length);
                    udpClient.Close();
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.ToString());
                }

                
            }
        }



        #endregion

        #region ping monitor

        Series ping_rtt_ms = new Series("ping RTT (ms)");

        private void ping_monitor_init() {
            chart_ping.ChartAreas[0].AxisY.Title = "rtt(ms)";
            chart_ping.ChartAreas[0].AxisX.Title = "packet number";
            chart_ping.Series.Clear();
            ping_rtt_ms.ChartType = SeriesChartType.Line;
            chart_ping.Series.Add(ping_rtt_ms);
        }

        int ping_timeout = 50;
        int ping_rtt = 0;

        private void backgroundWorker_ping_DoWork(object sender, DoWorkEventArgs e)
        {
            while (true) {
                try
                {
                    Ping pingSender = new Ping();
                    PingReply reply = pingSender.Send("192.168.1.1", ping_timeout);
                    if (reply.Status == IPStatus.Success)
                    {
                        ping_rtt = (int)reply.RoundtripTime;
                    }
                    else {
                        ping_rtt = ping_timeout;
                    }
                    backgroundWorker_ping.ReportProgress(1);
                    Thread.Sleep(250);
                }
                catch (Exception ex) {
                    Console.WriteLine(ex.ToString());
                }
            }


        }

        private void backgroundWorker_ping_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            while (ping_rtt_ms.Points.Count > 10) ping_rtt_ms.Points.RemoveAt(0);

            chart_ping.ChartAreas[0].AxisY.Maximum = ping_timeout;
            chart_ping.ChartAreas[0].AxisY.Minimum = 0;
            chart_ping.ChartAreas[0].RecalculateAxesScale();
            ping_rtt_ms.Points.AddY(ping_rtt);
        }

        #endregion





 


    }
}
