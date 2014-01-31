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
        Series sat = new Series("at");

        private void Form1_Load(object sender, EventArgs e)
        {
            sax.ChartType = SeriesChartType.Line;
            say.ChartType = SeriesChartType.Line;
            saz.ChartType = SeriesChartType.Line;
            sat.ChartType = SeriesChartType.Line;

            chart1.ChartAreas[0].AxisY.Maximum = 3;
            chart1.ChartAreas[0].AxisY.Minimum = -3;
            chart1.Series.Add(sax);
            chart1.Series.Add(say);
            chart1.Series.Add(saz);
            chart1.Series.Add(sat);
            this.serialPort1.PortName = "COM3";
            this.serialPort1.Open();
            //while (true) {
            //    Console.WriteLine(this.serialPort1.ReadExisting());
            //}
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            try
            {
                
                this.serialPort1.WriteLine("g");

                this.serialPort1.ReadTimeout = 10;
                string[] aa = this.serialPort1.ReadLine().Split('|');
                if (aa.Length >= 3)
                {
                    double ax = double.Parse(aa[0], CultureInfo.InvariantCulture);
                    double ay = double.Parse(aa[1], CultureInfo.InvariantCulture);
                    double az = double.Parse(aa[2], CultureInfo.InvariantCulture);
                    double at = Math.Sqrt(ax * ax + ay * ay + az * az);


                    while (sax.Points.Count > 100) sax.Points.RemoveAt(0);
                    sax.Points.AddY(ax);

                    while (say.Points.Count > 100) say.Points.RemoveAt(0);
                    say.Points.AddY(ay);

                    while (saz.Points.Count > 100) saz.Points.RemoveAt(0);
                    saz.Points.AddY(az);

                    while (sat.Points.Count > 100) sat.Points.RemoveAt(0);
                    sat.Points.AddY(at);

                }
            }
            catch (Exception ex) { 
            
            }

        }
    }
}
