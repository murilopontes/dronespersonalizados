using SimpleWifi;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.NetworkInformation;
using System.Speech.Synthesis;
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
            timer_gamepad.Interval = 1000 / 10;
            timer_gamepad.Enabled = true;

            backgroundWorker_pinger.RunWorkerAsync();


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
            this.chart8.Series.Add(wifi_rssi);


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



            foreach (Series x in chart1.Series)
            {
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
            drone.H += state.ThumbSticks.Right.Y * 5;
            //Yaw (Rotate clockwise/anticlockwise)
            drone.Yaw += DEG2RAD(state.ThumbSticks.Right.X * 5);

            if (state.Buttons.A == XInputDotNetPure.ButtonState.Pressed)
            {
                //takeoff
                voice_queue.TryAdd("decolar");
                drone.Pitch = 0;
                drone.Roll = 0;
                drone.Yaw = 0;
                drone.H = 25;
            }
            if (state.Buttons.B == XInputDotNetPure.ButtonState.Pressed)
            {
                //land / emergency
                voice_queue.TryAdd("emergência");
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
                )
            {

                previous_state.H = drone.H;
                previous_state.Yaw = drone.Yaw;
                previous_state.Roll = drone.Roll;
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

        // Initialize a new instance of the SpeechSynthesizer.
        SpeechSynthesizer synth = new SpeechSynthesizer();

        static BlockingCollection<string> voice_queue = new BlockingCollection<string>(1);


        static void wifi_ConnectionStatusChanged(object sender, WifiStatusEventArgs e)
        {
            //Console.WriteLine("\nNew status: {0}", e.NewStatus.ToString());
            voice_queue.TryAdd("wifi mudou de estado para " + e.NewStatus.ToString());
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            synth.SetOutputToDefaultAudioDevice();


            //voice_queue.Add("olá, vamos voar?");

            backgroundWorker_voice.RunWorkerAsync();

        }


        private void chart4_Click(object sender, EventArgs e)
        {

        }

        private void timer2_Tick(object sender, EventArgs e)
        {

        }

        Series ping_rtt = new Series("192.168.1.1 RTT (ms)");


        double last_ping_rtt = -1;

        Series wifi_rssi = new Series("wifi RSSI(%)");
        AccessPoint drone_ap = null;

        bool ardrone_wifi_previous_state = false;

        private bool ardrone_wifi_is_ok()
        {

            try
            {
                Wifi wifi = new Wifi();
                IEnumerable<AccessPoint> accessPoints = wifi.GetAccessPoints();
                foreach (AccessPoint ap in accessPoints)
                {
                    if (ap.Name.StartsWith("ardrone_"))
                    {
                        drone_ap = ap;
                        if (!ap.IsConnected)
                        {
                            ardrone_wifi_previous_state = false;
                            voice_queue.TryAdd("drone não conectado");
                            AuthRequest authRequest = new AuthRequest(ap);
                            ap.Connect(authRequest);
                        }
                        else
                        {
                            if (ardrone_wifi_previous_state == false)
                            {
                                ardrone_wifi_previous_state = true;
                                voice_queue.TryAdd("drone conectado");
                            }
                            return true;
                        }
                    }
                }
            }
            catch (Exception e2)
            {

            }

            voice_queue.TryAdd("drone não encontrado");
            return false;
        }


        int ping_counter = 0;
        private void ardrone_ping()
        {
            try
            {
                Ping pingSender = new Ping();
                PingOptions options = new PingOptions();
                options.DontFragment = true;

                string data = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
                byte[] buffer = Encoding.ASCII.GetBytes(data);
                int timeout = 100;
                ping_counter++;

                PingReply reply = pingSender.Send("192.168.1.1", timeout, buffer, options);
                if (reply.Status == IPStatus.Success)
                {
                    toolStripStatusLabel1.Text = "192.168.1.1 seq=" + ping_counter + " RoundTripTime(RTT): " + reply.RoundtripTime + " ms";
                    last_ping_rtt = reply.RoundtripTime;
                }
                else
                {
                    last_ping_rtt = -10;
                    voice_queue.TryAdd("ping falhou");
                    toolStripStatusLabel1.Text = "192.168.1.1 seq=" + ping_counter + " RoundTripTime(RTT): " + reply.Status;

                }
                backgroundWorker_pinger.ReportProgress(0);
                System.Threading.Thread.Sleep(100);
            }
            catch (Exception e1)
            {

            }


        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            while (true)
            {
                //
                if (!ardrone_wifi_is_ok())
                {
                    System.Threading.Thread.Sleep(1000);
                    continue;
                }
                //
                ardrone_ping();
                //
                System.Threading.Thread.Sleep(250);
            }
        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            try
            {
                wifi_rssi.Points.AddY(drone_ap.SignalStrength);
                ping_rtt.Points.AddY(last_ping_rtt);
            }
            catch (Exception e2)
            {

            }
        }

        private void backgroundWorker_voice_DoWork(object sender, DoWorkEventArgs e)
        {
            while (true)
            {
                string text = voice_queue.Take();
                //Console.WriteLine(text);
                synth.Speak(text);
            }
        }


    }
}
