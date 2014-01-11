using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
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
        Series serie_battery = new Series("battery (V)");

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

            this.backgroundWorker_battery.RunWorkerAsync();
        }

        private void backgroundWorker_battery_DoWork(object sender, DoWorkEventArgs e)
        {
            this.backgroundWorker_battery.ProgressChanged += backgroundWorker_battery_ProgressChanged;
            while (true) {
                this.backgroundWorker_battery.ReportProgress(1);
                Thread.Sleep(1);
                Thread.Yield();
            }
        }

        void backgroundWorker_battery_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {

        }
    }
}
