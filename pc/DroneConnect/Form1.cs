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


        double angle_pitch = 0.0;
        double angle_roll = 0.0;
        double angle_yaw = 0.0;

        double speed_pitch = 0.0;
        double speed_roll = 0.0;
        double speed_yaw = 0.0;

        double vbat = 0.0;

        private void navboard_init() {
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


        private void backgroundWorker_navboard_DoWork(object sender, DoWorkEventArgs e)
        {
            //
            while (true)
            {

                //
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
                            if (aa.Length >= 7) {

                                angle_pitch = double.Parse(aa[0], CultureInfo.InvariantCulture);
                                angle_roll = double.Parse(aa[1], CultureInfo.InvariantCulture);
                                angle_yaw = double.Parse(aa[2], CultureInfo.InvariantCulture);

                                speed_pitch = double.Parse(aa[3], CultureInfo.InvariantCulture);
                                speed_roll = double.Parse(aa[4], CultureInfo.InvariantCulture);
                                speed_yaw = double.Parse(aa[5], CultureInfo.InvariantCulture);

                                vbat = double.Parse(aa[6], CultureInfo.InvariantCulture);

                                this.backgroundWorker_navboard.ReportProgress(1);
                            }
                            break;
                        }
                        Thread.Sleep(1);
                    }
                    udpClient.Close();
                }
                catch (Exception ex) { 
                
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

            chart_angle.ChartAreas[0].RecalculateAxesScale();
            chart_rate.ChartAreas[0].RecalculateAxesScale();
            chart_vbat.ChartAreas[0].RecalculateAxesScale();
        }

        #endregion


        #region joystick


        private void backgroundWorker_joystick_DoWork(object sender, DoWorkEventArgs e)
        {
            while (true) {

                try
                {
                    Gamepad_State_SlimDX joy = new Gamepad_State_SlimDX(SlimDX.XInput.UserIndex.One);
                    joy.Update();
                    Int16 pitch_now = (Int16)(joy.LeftStick.Position.Y * 90);
                    Int16 roll_now = (Int16)(joy.LeftStick.Position.X * 180);
                    Int16 yaw_now = (Int16)(joy.RightStick.Position.X * 180);
                    Int16 throttle_now = (Int16)(joy.RightStick.Position.Y * 5);
                    Int16 emergency = 0;
                    if (joy.B || joy.RightTrigger > 0.5) emergency = 1;
                    Int16 takeoff = 0;
                    if (joy.A) takeoff = 1;

                    string cmd = throttle_now + "|" + pitch_now + "|" + roll_now + "|" + yaw_now + "|" + takeoff + "|" + emergency + "|";


                    UdpClient udpClient = new UdpClient();
                    udpClient.Connect("192.168.1.1", 3000);
                    Byte[] sendBytes = Encoding.ASCII.GetBytes(cmd);
                    udpClient.Send(sendBytes, sendBytes.Length);
                    udpClient.Close();
                }
                catch (Exception ex)
                {

                }

                Thread.Sleep(10);
            }
        }

        private void backgroundWorker_joystick_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {

        }

        private void backgroundWorker_joystick_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {

        }


        #endregion

        #region ping monitor

        Series ping_rtt_ms = new Series("ping RTT (ms)");

        private void ping_monitor_init() {
            chart1.ChartAreas[0].AxisY.Title = "rtt(ms)";
            chart1.ChartAreas[0].AxisX.Title = "packet number";
            chart1.Series.Clear();
            ping_rtt_ms.ChartType = SeriesChartType.Line;
            chart1.Series.Add(ping_rtt_ms);
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
                
                }
            }


        }

        private void backgroundWorker_ping_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            while (ping_rtt_ms.Points.Count > 10) ping_rtt_ms.Points.RemoveAt(0);

            chart1.ChartAreas[0].AxisY.Maximum = ping_timeout;
            chart1.ChartAreas[0].AxisY.Minimum = 0;
            chart1.ChartAreas[0].RecalculateAxesScale();
            ping_rtt_ms.Points.AddY(ping_rtt);
        }

        #endregion





 


    }
}
