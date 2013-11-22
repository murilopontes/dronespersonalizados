using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using XInputDotNetPure;

namespace OpenFlight
{
    public partial class Form1 : Form
    {
        private Drone drone = new Drone();
        private UdpLogger log;

        public Form1()
        {
            InitializeComponent();

            log = new UdpLogger(7778, string.Format("{0:yyyyMMddHHmmss}_navlog.csv", DateTime.Now));
            timer1.Interval = 1000/10;
            timer1.Enabled = true;

            backgroundWorker1.RunWorkerAsync();


            ping_rtt.SmartLabelStyle.Enabled = true;
            
            
            this.chart1.Series.Clear();
            this.chart2.Series.Clear();
            this.chart3.Series.Clear();
            this.chart4.Series.Clear();
            this.chart5.Series.Clear();
            this.chart6.Series.Clear();
            this.chart7.Series.Clear();
            this.chart8.Series.Clear();

            this.chart8.Series.Add(ping_rtt);

            this.chart1.Series.Add(log_ax);
            this.chart1.Series.Add(log_ay);
            this.chart1.Series.Add(log_az);

            this.chart2.Series.Add(log_gx);
            this.chart2.Series.Add(log_gy);
            this.chart2.Series.Add(log_gz);

            this.chart5.Series.Add(log_act_h);
            this.chart5.Series.Add(log_set_h);
            this.chart5.Series.Add(log_act_hv);

            this.chart3.Series.Add(log_act_pitch);
            this.chart3.Series.Add(log_act_roll);
            this.chart3.Series.Add(log_act_yaw);

            this.chart3.Series.Add(log_set_pitch);
            this.chart3.Series.Add(log_set_roll);
            this.chart3.Series.Add(log_set_yaw);

            this.chart4.Series.Add(log_throttle);
            this.chart4.Series.Add(log_adj_pitch);
            this.chart4.Series.Add(log_adj_roll);
            this.chart4.Series.Add(log_adj_yaw);

            this.chart7.Series.Add(udp_tx_h);
            this.chart6.Series.Add(udp_tx_pitch);
            this.chart6.Series.Add(udp_tx_roll);
            this.chart6.Series.Add(udp_tx_yaw);

            

            foreach (Series x in chart1.Series) {
                x.ChartType = SeriesChartType.FastLine;
            }
            foreach (Series x in chart2.Series)
            {
                x.ChartType = SeriesChartType.FastLine;
            }
            foreach (Series x in chart3.Series)
            {
                x.ChartType = SeriesChartType.FastLine;
            }
            foreach (Series x in chart4.Series)
            {
                x.ChartType = SeriesChartType.FastLine;
            }
            foreach (Series x in chart5.Series)
            {
                x.ChartType = SeriesChartType.FastLine;
            }
            foreach (Series x in chart6.Series)
            {
                x.ChartType = SeriesChartType.FastLine;
            }
            foreach (Series x in chart7.Series)
            {
                x.ChartType = SeriesChartType.FastLine;
            }
            foreach (Series x in chart8.Series)
            {
                x.ChartType = SeriesChartType.FastLine;
            }
        }


        Series log_ax = new Series("ax m/s^2 (front/rear)");
        Series log_ay = new Series("ay m/s^2 (left/right)");
        Series log_az = new Series("az m/s^2 (top/bottom)");

        Series log_gx = new Series("gx (left/right)");
        Series log_gy = new Series("gy (front/rear)");
        Series log_gz = new Series("gz");

        Series log_act_h = new Series("act_h (cm)");
        Series log_act_pitch = new Series("act_pitch (graus)");
        Series log_act_roll = new Series("act_roll (graus)");
        Series log_act_yaw = new Series("act_yaw (graus)");
        Series log_act_hv = new Series("act_hv");


        Series log_set_h = new Series("set_h (cm)");
        Series log_set_pitch = new Series("set_pitch (graus)");
        Series log_set_roll = new Series("set_roll (graus)");
        Series log_set_yaw = new Series("set_yaw (graus)");

        Series log_throttle = new Series("throttle");
        Series log_adj_pitch = new Series("adj_pitch (throttle)");
        Series log_adj_roll = new Series("adj_roll (throttle)");
        Series log_adj_yaw = new Series("adj_yaw (throttle)");

        Series udp_tx_pitch = new Series("tx_pitch (graus [-5,5])");
        Series udp_tx_roll = new Series("tx_roll (graus [-5,5])");
        Series udp_tx_yaw = new Series("tx_yaw (graus [-5,5])");
        Series udp_tx_h = new Series("tx_h (cm [0-600])");

        private void timer1_Tick(object sender, EventArgs e)
        {
            this.toolStripStatusLabel_recv_count.Text = "UDP RX " + log.msgcount + " packets";

            if (log.msgcount > 0)
            {
                //acelerometro
                log_ax.Points.AddY(log.ax / 100000);
                log_ay.Points.AddY(log.ay / 100000);
                log_az.Points.AddY(log.az / 100000);

                //giroscopio
                log_gx.Points.AddY(log.gx / 100000);
                log_gy.Points.AddY(log.gy / 100000);
                log_gz.Points.AddY(log.gz / 100000);

                //
                log_act_pitch.Points.AddY(log.act_pitch / 100000);
                log_act_roll.Points.AddY(log.act_roll / 100000);
                log_act_yaw.Points.AddY(log.act_yaw / 100000);

                //
                log_act_h.Points.AddY(log.act_h / 100000);
                log_set_h.Points.AddY(log.set_h / 100000);
                log_act_hv.Points.AddY(log.act_hv / 100000);

                log_set_pitch.Points.AddY(log.set_pitch / 100000);
                log_set_roll.Points.AddY(log.set_roll / 100000);
                log_set_yaw.Points.AddY(log.set_yaw / 100000);

                //motores
                log_throttle.Points.AddY(log.throttle / 100000);
                log_adj_pitch.Points.AddY(log.throttle_pitch_adj / 100000);
                log_adj_roll.Points.AddY(log.throttle_roll_adj / 100000);
                log_adj_yaw.Points.AddY(log.throttle_yaw_adj / 100000);
            }


            //xbox360 gamepad with circular deadzone analog noise reduction
            GamePadState state = GamePad.GetState(PlayerIndex.One, GamePadDeadZone.Circular);

            /////////////////////////////////////////////////////////
            //HOVER (planar: por padrao fica parado no mesmo lugar)
            drone.Pitch = 0;
            drone.Roll = 0;
            drone.Yaw = 0;
            //////////////////////////////////////////////////////////////////////////////////////
            //PITCH (passo para frente e para tras)
            drone.Pitch += DEG2RAD(state.ThumbSticks.Left.Y * 5);
            ///////////////////////////////////////////////////////////////////////////////////////
            //ROLL (andar de lado para esquerda e direta)
            drone.Roll += DEG2RAD(state.ThumbSticks.Left.X * 5);
            ///////////////////////////////////////////////////////////////////////////////////////
            //Throttle (subir e descer)
            drone.H += state.ThumbSticks.Right.Y*5;
            //Yaw (Rotate clockwise/anticlockwise)
            drone.Yaw += DEG2RAD(state.ThumbSticks.Right.X*5);

            if (state.Buttons.A == XInputDotNetPure.ButtonState.Pressed)
            {
                //takeoff
                drone.Pitch = 0;
                drone.Roll = 0;
                drone.Yaw = 0;
                drone.H = 25;
            }
            if (state.Buttons.B == XInputDotNetPure.ButtonState.Pressed)
            {
                //land / emergency
                drone.Pitch = 0;
                drone.Roll = 0;
                drone.Yaw = 0;
                drone.H = 0;
            }

            udp_tx_h.Points.AddY(drone.H);
            udp_tx_pitch.Points.AddY(RAD2DEG(drone.Pitch));
            udp_tx_roll.Points.AddY(RAD2DEG(drone.Roll));
            udp_tx_yaw.Points.AddY(RAD2DEG(drone.Yaw));

            if (
                previous_state.H != drone.H || 
                previous_state.Yaw != drone.Yaw || 
                previous_state.Roll != drone.Roll || 
                previous_state.Pitch != drone.Pitch 
                ) {

                previous_state.H = drone.H ;
                previous_state.Yaw = drone.Yaw ; 
                previous_state.Roll = drone.Roll ;
                previous_state.Pitch = drone.Pitch; 

                    drone.SendCmd();
                }
            //////////////////////////////////////////////////////////////////////////////////////
            
            toolStripStatusLabel_send_count.Text = "UDP TX " + drone.cmd_count + " packets";



        }

        Drone previous_state = new Drone();

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {

        }

        //degrees to radians
        private double DEG2RAD(double x)
        {
            return x * 3.1415926 / 180;
        }
        //radians to degrees
        private double RAD2DEG(double x)
        {
            return x / 3.1415926 * 180;
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void chart4_Click(object sender, EventArgs e)
        {

        }

        private void timer2_Tick(object sender, EventArgs e)
        {
          
        }

        Series ping_rtt = new Series("192.168.1.1 RTT (ms)");


        double last_ping_rtt = -1;

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            
            while (true) {
                try
                {
                    Ping pingSender = new Ping();
                    PingOptions options = new PingOptions();

                    // Use the default Ttl value which is 128, 
                    // but change the fragmentation behavior.
                    options.DontFragment = true;

                    // Create a buffer of 32 bytes of data to be transmitted. 
                    string data = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
                    byte[] buffer = Encoding.ASCII.GetBytes(data);
                    int timeout = 1;
                    PingReply reply = pingSender.Send("192.168.1.1", timeout, buffer, options);
                    if (reply.Status == IPStatus.Success)
                    {
                        toolStripStatusLabel1.Text = "192.168.1.1 RoundTripTime(RTT): " + reply.RoundtripTime + " ms";
                        last_ping_rtt = reply.RoundtripTime;


                        Console.WriteLine("Address: {0}", reply.Address.ToString());
                        Console.WriteLine("RoundTrip time: {0} ms", reply.RoundtripTime);
                        Console.WriteLine("Time to live: {0}", reply.Options.Ttl);
                        Console.WriteLine("Don't fragment: {0}", reply.Options.DontFragment);
                        Console.WriteLine("Buffer size: {0}", reply.Buffer.Length);
                    }
                    else
                    {
                        last_ping_rtt = -1;
                        Console.WriteLine("Ping status=" + reply.Status);
                        toolStripStatusLabel1.Text = "192.168.1.1 RoundTripTime(RTT): " + reply.Status;

                    }
                    backgroundWorker1.ReportProgress(0);
                    System.Threading.Thread.Sleep(100);
                }
                catch (Exception e1) { 
                
                }
            }
        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            try
            {
                ping_rtt.Points.AddY(last_ping_rtt);
            }
            catch (Exception e2) { 
            
            }
        }


    }
}
