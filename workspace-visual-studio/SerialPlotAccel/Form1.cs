using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace SerialPlotAccel
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }


        Series sax = new Series("ax");
        Series say = new Series("ay");
        Series saz = new Series("az");

        Series ssx = new Series("sx");
        Series ssy = new Series("sy");
        Series ssz = new Series("sz");

        Series spx = new Series("px");
        Series spy = new Series("py");
        Series spz = new Series("pz");

        private void Form1_Load(object sender, EventArgs e)
        {
            sax.ChartType = SeriesChartType.Line;
            say.ChartType = SeriesChartType.Line;
            saz.ChartType = SeriesChartType.Line;

            ssx.ChartType = SeriesChartType.Line;
            ssy.ChartType = SeriesChartType.Line;
            ssz.ChartType = SeriesChartType.Line;

            spx.ChartType = SeriesChartType.Line;
            spy.ChartType = SeriesChartType.Line;
            spz.ChartType = SeriesChartType.Line;

            //chart_acc.ChartAreas[0].AxisY.Maximum = 3;
            //chart_acc.ChartAreas[0].AxisY.Minimum = -3;
            chart_acc.Series.Add(sax);
            chart_acc.Series.Add(say);
            chart_acc.Series.Add(saz);

            chart_speed.Series.Add(ssx);
            chart_speed.Series.Add(ssy);
            chart_speed.Series.Add(ssz);

            chart_position.Series.Add(spx);
            chart_position.Series.Add(spy);
            chart_position.Series.Add(spz);



            this.serialPort1.PortName = "COM3";
            this.serialPort1.Open();
            //while (true) {
            //    Console.WriteLine(this.serialPort1.ReadExisting());
            //}
        }

        double[] gravity=new double[3];
        double[] linear_acceleration=new double[3];
        double[] linear_speed = new double[3];
        double[] linear_speed_filter = new double[3];
        double[] linear_position = new double[3];
        double[] linear_acc_deadzone_count = new double[3];

        int samples = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {
            try
            {
                samples++;
                
                this.serialPort1.WriteLine("g");

                this.serialPort1.ReadTimeout = 10;
                string[] aa = this.serialPort1.ReadLine().Split('|');
                if (aa.Length >= 3)
                {
                    double ax = double.Parse(aa[0], CultureInfo.InvariantCulture);
                    double ay = double.Parse(aa[1], CultureInfo.InvariantCulture);
                    double az = double.Parse(aa[2], CultureInfo.InvariantCulture);
                    double at = Math.Sqrt(ax * ax + ay * ay + az * az);

                    // alpha is calculated as t / (t + dT)
                    // with t, the low-pass filter's time-constant
                    // and dT, the event delivery rate

                    double dt = 5;
                    double alpha = 1/(1+dt);
                    alpha = 0.8;

                    // Isolate the force of gravity with the low-pass filter.
                    gravity[0] = alpha * gravity[0] + (1 - alpha) * ax;
                    gravity[1] = alpha * gravity[1] + (1 - alpha) * ay;
                    gravity[2] = alpha * gravity[2] + (1 - alpha) * az;

                    // Remove the gravity contribution with the high-pass filter.
                    linear_acceleration[0] = ax - gravity[0];
                    linear_acceleration[1] = ay - gravity[1];
                    linear_acceleration[2] = az - gravity[2];

                    //linear accelaretion deadzone
                    double th=0.025;
                    if (linear_acceleration[0] > -th && linear_acceleration[0] < th) linear_acceleration[0] = 0;
                    if (linear_acceleration[1] > -th && linear_acceleration[1] < th) linear_acceleration[1] = 0;
                    if (linear_acceleration[2] > -th && linear_acceleration[2] < th) linear_acceleration[2] = 0;

                    //deadzone counter
                    if (linear_acceleration[0] == 0) { linear_acc_deadzone_count[0]++; } else { linear_acc_deadzone_count[0] = 0; }
                    if (linear_acceleration[1] == 0) { linear_acc_deadzone_count[1]++; } else { linear_acc_deadzone_count[1] = 0; }
                    if (linear_acceleration[2] == 0) { linear_acc_deadzone_count[2]++; } else { linear_acc_deadzone_count[2] = 0; }

                    if (samples > 30)
                    {
                        //
                        linear_speed[0] += linear_acceleration[0];
                        linear_speed[1] += linear_acceleration[1];
                        linear_speed[2] += linear_acceleration[2];
                        //
                        int reset_speed = 10;
                        if (linear_acc_deadzone_count[0] > reset_speed) linear_speed[0] = 0;
                        if (linear_acc_deadzone_count[1] > reset_speed) linear_speed[1] = 0;
                        if (linear_acc_deadzone_count[2] > reset_speed) linear_speed[2] = 0;

                        //
                        linear_position[0] += linear_speed[0];
                        linear_position[1] += linear_speed[1];
                        linear_position[2] += linear_speed[2];
                    }

                    while (sax.Points.Count > 100) sax.Points.RemoveAt(0); sax.Points.AddY(linear_acceleration[0]);
                    while (say.Points.Count > 100) say.Points.RemoveAt(0); say.Points.AddY(linear_acceleration[1]);
                    while (saz.Points.Count > 100) saz.Points.RemoveAt(0); saz.Points.AddY(linear_acceleration[2]);

                    while (ssx.Points.Count > 100) ssx.Points.RemoveAt(0); ssx.Points.AddY(linear_speed[0]);
                    while (ssy.Points.Count > 100) ssy.Points.RemoveAt(0); ssy.Points.AddY(linear_speed[1]);
                    while (ssz.Points.Count > 100) ssz.Points.RemoveAt(0); ssz.Points.AddY(linear_speed[2]);

                    while (spx.Points.Count > 100) spx.Points.RemoveAt(0); spx.Points.AddY(linear_position[0]);
                    while (spy.Points.Count > 100) spy.Points.RemoveAt(0); spy.Points.AddY(linear_position[1]);
                    while (spz.Points.Count > 100) spz.Points.RemoveAt(0); spz.Points.AddY(linear_position[2]);



                    chart_acc.ChartAreas[0].RecalculateAxesScale();
                    chart_speed.ChartAreas[0].RecalculateAxesScale();
                    chart_position.ChartAreas[0].RecalculateAxesScale();

                    //while (sat.Points.Count > 100) sat.Points.RemoveAt(0);
                    //sat.Points.AddY(at);



                }
            }
            catch (Exception ex) { 
            
            }

        }
    }
}
