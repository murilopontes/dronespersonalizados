using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace DroneSetPoint
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }


        //
        Series serie_battery = new Series("battery");
        //Series serie_battery2 = new Series("battery2");

        //
        Series serie_height_setpoint = new Series("height_set");
        Series serie_height_ultrasonic = new Series("height_ultrasonic");

        //
        Series serie_pitch_setpoint = new Series("pitch_set");
        Series serie_pitch_accel = new Series("pitch_accel");
        Series serie_pitch_giro = new Series("pitch_giro");
        Series serie_pitch_fusion = new Series("pitch_fusion");

        //
        Series serie_roll_setpoint = new Series("roll_set");
        Series serie_roll_accel = new Series("roll_accel");
        Series serie_roll_giro = new Series("roll_giro");
        Series serie_roll_fusion = new Series("roll_fusion");

        //
        Series serie_yaw_setpoint = new Series("yaw_set");
        Series serie_yaw_accel = new Series("yaw_accel");
        Series serie_yaw_giro = new Series("yaw_giro");
        Series serie_yaw_fusion = new Series("yaw_fusion");



        private void Form1_Load(object sender, EventArgs e)
        {
            chart_roll.Series.Clear();
            chart_pitch.Series.Clear();
            chart_yaw.Series.Clear();
            chart_height.Series.Clear();
            chart_battery.Series.Clear();

            //
            chart_pitch.Series.Add(serie_pitch_setpoint);
            chart_pitch.Series.Add(serie_pitch_accel);
            chart_pitch.Series.Add(serie_pitch_giro);
            chart_pitch.Series.Add(serie_pitch_fusion);

            //
            chart_roll.Series.Add(serie_roll_setpoint);
            chart_roll.Series.Add(serie_roll_accel);
            chart_roll.Series.Add(serie_roll_giro);
            chart_roll.Series.Add(serie_roll_fusion);

            //
            chart_yaw.Series.Add(serie_yaw_setpoint);
            chart_yaw.Series.Add(serie_yaw_accel);
            chart_yaw.Series.Add(serie_yaw_giro);
            chart_yaw.Series.Add(serie_yaw_fusion);

            //
            chart_height.Series.Add(serie_height_setpoint);
            chart_height.Series.Add(serie_height_ultrasonic);

            //
            serie_battery.ChartType = SeriesChartType.Line;
            chart_battery.Series.Add(serie_battery);

            //
            //serie_battery2.ChartType = SeriesChartType.Line;
            //chart_battery.Series.Add(serie_battery2);

            this.backgroundWorker_battery.RunWorkerAsync();
        }


        string vbat_data = "";
        private void backgroundWorker_battery_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                int listenPort = 50001;
                UdpClient listener = new UdpClient(listenPort);
                listener.DontFragment = true;
                IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, listenPort);

                this.backgroundWorker_battery.ProgressChanged += backgroundWorker_battery_ProgressChanged;
                while (true)
                {
                    this.backgroundWorker_battery.ReportProgress(1);
                    byte[] receive_byte_array = listener.Receive(ref groupEP);
                    vbat_data = Encoding.ASCII.GetString(receive_byte_array);
                }
            }
            catch (Exception ex)
            {

            }
        }

        void backgroundWorker_battery_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            try
            {
                //Console.WriteLine("vbat->" + vbat_data);
                string[] aa = vbat_data.Split('|');
                if (aa.Length >= 8)
                {
                    int vbat_packet = int.Parse(aa[1]);
                    double vbat_voltage = double.Parse(aa[7], CultureInfo.InvariantCulture);
                    //Console.WriteLine(aa[1] + "==" + vbat_packet);
                    //Console.WriteLine(aa[7] + "==" + vbat_voltage);

                    while (serie_battery.Points.Count > 10000)
                    {
                        serie_battery.Points.RemoveAt(0);
                    }
                    serie_battery.Points.AddXY(vbat_packet, vbat_voltage);
                    chart_battery.ChartAreas[0].AxisY.IsStartedFromZero = false;
                    //chart_battery.ChartAreas[0].AxisY.Minimum = 8;
                    //chart_battery.ChartAreas[0].AxisY.Maximum = 13;
                    chart_battery.ChartAreas[0].RecalculateAxesScale();

                    //chart_battery.DataManipulator.CopySeriesValues("battery", "battery2");
                    //chart_battery.DataManipulator.FinancialFormula(FinancialFormula.WeightedMovingAverage, "battery2");
                    
                }
            }
            catch (Exception ex)
            {

            }

        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            Environment.Exit(0);
        }
    }
}
