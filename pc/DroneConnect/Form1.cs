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


        public class murix_series : Series
        {

            double circular_count = 0;
            public murix_series(string name)
                : base(name)
            {
                this.ChartType = SeriesChartType.Line;
            }
            public void circular_append_y(double value, int max_items)
            {
                while (this.Points.Count > max_items) this.Points.RemoveAt(0);
                this.Points.AddY( value);
                circular_count++;
            }
        }


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


    

         murix_series s_raw_ac_x = new murix_series("raw_ac_x");
         murix_series s_raw_ac_y = new murix_series("raw_ac_y");
         murix_series s_raw_ac_z = new murix_series("raw_ac_z");

         murix_series s_raw_gy_x = new murix_series("raw_gy_x");
         murix_series s_raw_gy_y = new murix_series("raw_gy_y");
         murix_series s_raw_gy_z = new murix_series("raw_gy_z");

         murix_series s_us_echo = new murix_series("raw_us_echo");


        murix_series s_angle_pitch = new murix_series("angle pitch");
        murix_series s_angle_roll = new murix_series("angle roll");
        murix_series s_angle_yaw = new murix_series("angle yaw");

        murix_series s_speed_pitch = new murix_series("speed pitch");
        murix_series s_speed_roll = new murix_series("speed roll");
        murix_series s_speed_yaw = new murix_series("speed yaw");

        murix_series s_vbat = new murix_series("vbat");

        murix_series s_vdd0 = new murix_series("vdd0");
        murix_series s_vdd1 = new murix_series("vdd1");
        murix_series s_vdd2 = new murix_series("vdd2");
        murix_series s_vdd3 = new murix_series("vdd3");
        murix_series s_vdd4 = new murix_series("vdd4");

        murix_series s_vdd0_setpoint = new murix_series("vdd0_setpoint");
        murix_series s_vdd1_setpoint = new murix_series("vdd1_setpoint");
        murix_series s_vdd2_setpoint = new murix_series("vdd2_setpoint");
        murix_series s_vdd3_setpoint = new murix_series("vdd3_setpoint");
        murix_series s_vdd4_setpoint = new murix_series("vdd4_setpoint");

        murix_series s_front_left = new murix_series("front_left");
        murix_series s_front_right = new murix_series("front_right");
        murix_series s_rear_left = new murix_series("rear_left");
        murix_series s_rear_right = new murix_series("rear_right");

            

        private void navboard_init() {


            this.chart_raw_accel.Series.Add(s_raw_ac_x);
            this.chart_raw_accel.Series.Add(s_raw_ac_y);
            this.chart_raw_accel_z.Series.Add(s_raw_ac_z);

            this.chart_raw_gyro.Series.Add(s_raw_gy_x);
            this.chart_raw_gyro.Series.Add(s_raw_gy_y);
            this.chart_raw_gyro.Series.Add(s_raw_gy_z);

            this.chart_raw_echo.Series.Add(s_us_echo);

            
            this.chart_motors.Series.Add(s_front_left);
            this.chart_motors.Series.Add(s_front_right);
            this.chart_motors.Series.Add(s_rear_left);
            this.chart_motors.Series.Add(s_rear_right);

            this.chart_angle.Series.Add(s_angle_pitch);
            this.chart_angle.Series.Add(s_angle_roll);
            this.chart_angle.Series.Add(s_angle_yaw);

            this.chart_rate.Series.Add(s_speed_pitch);
            this.chart_rate.Series.Add(s_speed_roll);
            this.chart_rate.Series.Add(s_speed_yaw);

            this.chart_vbat.Series.Add(s_vbat);

            this.chart_vbat.Series.Add(s_vdd0);
            this.chart_vbat.Series.Add(s_vdd1);
            this.chart_vbat.Series.Add(s_vdd2);
            this.chart_vbat.Series.Add(s_vdd3);
            this.chart_vbat.Series.Add(s_vdd4);

            this.chart_vbat.Series.Add(s_vdd0_setpoint);
            this.chart_vbat.Series.Add(s_vdd1_setpoint);
            this.chart_vbat.Series.Add(s_vdd2_setpoint);
            this.chart_vbat.Series.Add(s_vdd3_setpoint);
            this.chart_vbat.Series.Add(s_vdd4_setpoint);

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

        string json_vbat = "";
        string json_motors = "";
        string json_navboard = "";

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
                    json_vbat = udp_send_recv(4000, "get", 100);
                    json_navboard = udp_send_recv(5000, "get", 100);
                    json_motors = udp_send_recv(6000, "get", 100);
                    //////////////////////////////////////////////////////////////////////////////////////////
                    //////////////////////////////////////////////////////////////////////////////////////////
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
            try
            {
                //this.horizonInstrumentControl1.SetAttitudeIndicatorParameters(angle_pitch, angle_roll);
                //this.attitudeIndicatorInstrumentControl1.SetAttitudeIndicatorParameters(0, -angle_yaw);

                int max = 100;

                if (json_navboard.Length > 0)
                {
                    var o_navboard = (JObject)JsonConvert.DeserializeObject(json_navboard);

                    s_raw_ac_x.circular_append_y((double)o_navboard["raw_ac_x"], max);
                    s_raw_ac_y.circular_append_y((double)o_navboard["raw_ac_y"], max);
                    s_raw_ac_z.circular_append_y((double)o_navboard["raw_ac_z"], max);

                    s_raw_gy_x.circular_append_y((double)o_navboard["raw_gy_x"], max);
                    s_raw_gy_y.circular_append_y((double)o_navboard["raw_gy_y"], max);
                    s_raw_gy_z.circular_append_y((double)o_navboard["raw_gy_z"], max);

                    s_us_echo.circular_append_y((double)o_navboard["raw_us_echo"], max);
                }

                if (json_vbat.Length > 0)
                {
                    var o_vbat = (JObject)JsonConvert.DeserializeObject(json_vbat);
                    s_vbat.circular_append_y((double)o_vbat["vbat"], max);
                    s_vdd0.circular_append_y((double)o_vbat["vdd0"], max);
                    s_vdd1.circular_append_y((double)o_vbat["vdd1"], max);
                    s_vdd2.circular_append_y((double)o_vbat["vdd2"], max);
                    s_vdd3.circular_append_y((double)o_vbat["vdd3"], max);
                    s_vdd4.circular_append_y((double)o_vbat["vdd4"], max);
                    s_vdd0_setpoint.circular_append_y((double)o_vbat["vdd0_setpoint"], max);
                    s_vdd1_setpoint.circular_append_y((double)o_vbat["vdd1_setpoint"], max);
                    s_vdd2_setpoint.circular_append_y((double)o_vbat["vdd2_setpoint"], max);
                    s_vdd3_setpoint.circular_append_y((double)o_vbat["vdd3_setpoint"], max);
                    s_vdd4_setpoint.circular_append_y((double)o_vbat["vdd4_setpoint"], max);
                }

                if (json_motors.Length > 0)
                {
                    var o_motors = (JObject)JsonConvert.DeserializeObject(json_motors);
                    s_front_left.circular_append_y((double)o_motors["front_left"], max);
                    s_front_right.circular_append_y((double)o_motors["front_right"], max);
                    s_rear_left.circular_append_y((double)o_motors["rear_left"], max);
                    s_rear_right.circular_append_y((double)o_motors["rear_right"], max);
                }

                //
                chart_angle.ChartAreas[0].RecalculateAxesScale();
                chart_rate.ChartAreas[0].RecalculateAxesScale();
                chart_vbat.ChartAreas[0].RecalculateAxesScale();
                chart_motors.ChartAreas[0].RecalculateAxesScale();

                chart_raw_echo.ChartAreas[0].RecalculateAxesScale();
                chart_raw_accel.ChartAreas[0].RecalculateAxesScale();
                chart_raw_accel_z.ChartAreas[0].RecalculateAxesScale();
                chart_raw_gyro.ChartAreas[0].RecalculateAxesScale();

                //
                chart_raw_accel.ChartAreas[0].AxisY.IsStartedFromZero = false;
                chart_raw_accel_z.ChartAreas[0].AxisY.IsStartedFromZero = false;
                chart_raw_gyro.ChartAreas[0].AxisY.IsStartedFromZero = false;
                chart_raw_echo.ChartAreas[0].AxisY.IsStartedFromZero = false;


                chart_vbat.ChartAreas[0].AxisY.IsStartedFromZero = false;
                chart_motors.ChartAreas[0].AxisY.IsStartedFromZero = false;
            }
            catch (Exception ex) {
                Console.WriteLine(ex);
            }
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


        murix_series ping_rtt_ms = new murix_series("ping RTT (ms)");

        private void ping_monitor_init() {
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
            ping_rtt_ms.circular_append_y(ping_rtt, 50);

            chart_ping.ChartAreas[0].AxisY.Maximum = ping_timeout;
            chart_ping.ChartAreas[0].AxisY.Minimum = 0;
            chart_ping.ChartAreas[0].RecalculateAxesScale();
        }

        #endregion

   





 


    }
}
