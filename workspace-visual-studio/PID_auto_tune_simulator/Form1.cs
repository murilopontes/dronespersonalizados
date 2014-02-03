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

namespace MURIX_PID_Auto_tune_simulator
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        Random r = new Random();
        Series pid_input = new Series("sensor input");
        Series pid_input_maxs = new Series("locals max");
        Series pid_input_mins = new Series("locals min");
        Series pid_ku = new Series("PID Pu (seconds)");
        Series pid_A = new Series("PID A (input amplitude)");



        public double[] function_g(double[] u, double[] x) {
            //
            double[] y;

            return null;
        }
        public double[] function_f(double[] u, double[] x)
        {
            //memory
            double[] w;

            return null;
        }

        public void control_system() {
            int p = 4; //number of outputs (actuators)
            int n = 6; //number of internal states (memory)
            int u = 6; //number of inputs (sensors)

            //memory
            double[] vu = new double[u];//
            
            //memory less
            double[] vx = new double[n];//
            double[] vy = new double[p];//

            // X = predict
            // W = state-update (system update)
        
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        


            pid_input.ChartType = SeriesChartType.Spline;
            pid_input_maxs.ChartType = SeriesChartType.Point;
            pid_input_mins.ChartType = SeriesChartType.Point;
            pid_ku.ChartType = SeriesChartType.Spline;
            pid_A.ChartType = SeriesChartType.Spline;

            this.chart1.Series.Add(pid_input);
            this.chart1.Series.Add(pid_input_maxs);
            this.chart1.Series.Add(pid_input_mins);

            this.chart2.Series.Add(pid_ku);
            this.chart3.Series.Add(pid_A);
        }

        private void detected(Series data, Series local_max_peaks, Series local_min_peaks)
        {
            if (data.Points.Count <= 2) return;

            //local_max_peaks.Points.Clear();
            //local_min_peaks.Points.Clear();
            double delta = 1e-100;
  
            bool is_detecting_local_max = true;

            double local_max = data.Points[0].YValues[0];
            double local_min = data.Points[0].YValues[0]; ;
            int local_max_index = 0;
            int local_min_index = 0;

            for (int index = 1; index < data.Points.Count; index++) {
                //find mininum and maximum values and index
                if (data.Points[index].YValues[0] > local_max) {
                    local_max_index = index;
                    local_max = data.Points[index].YValues[0];
                }
                if (data.Points[index].YValues[0] < local_min)
                {
                    local_min_index = index;
                    local_min = data.Points[index].YValues[0];
                }
                //
                if (is_detecting_local_max)
                {
                    if(data.Points[index].YValues[0] < local_max - delta){
                        //max found
                        local_max_peaks.Points.AddXY(data.Points[local_max_index].XValue, local_max);
                        //try found min
                        is_detecting_local_max = false;
                        //back to previous min
                        index = local_max_index - 1;
                        //reset min
                        local_min = data.Points[local_max_index].YValues[0];
                        local_min_index = local_max_index;
                    }
                }
                else 
                {
                    if (data.Points[index].YValues[0] > local_min + delta)
                    {
                        //min found
                        local_min_peaks.Points.AddXY(data.Points[local_min_index].XValue, local_min);
                        //try found max
                        is_detecting_local_max = true;
                        //back to previous max
                        index = local_min_index - 1;
                        //reset max
                        local_max = data.Points[local_min_index].YValues[0];
                        local_max_index = local_min_index;
                    }
                }


            }




        }


        public sealed class GaussianRandom
        {
            private bool _hasDeviate;
            private double _storedDeviate;
            private readonly Random _random;

            public GaussianRandom(Random random = null)
            {
                _random = random ?? new Random();
            }

            /// <summary>
            /// Obtains normally (Gaussian) distributed random numbers, using the Box-Muller
            /// transformation.  This transformation takes two uniformly distributed deviates
            /// within the unit circle, and transforms them into two independently
            /// distributed normal deviates.
            /// </summary>
            /// <param name="mu">The mean of the distribution.  Default is zero.</param>
            /// <param name="sigma">The standard deviation of the distribution.  Default is one.</param>
            /// <returns></returns>
            public double NextGaussian(double mu = 0, double sigma = 1)
            {
                if (sigma <= 0)
                    throw new ArgumentOutOfRangeException("sigma", "Must be greater than zero.");

                if (_hasDeviate)
                {
                    _hasDeviate = false;
                    return _storedDeviate * sigma + mu;
                }

                double v1, v2, rSquared;
                do
                {
                    // two random values between -1.0 and 1.0
                    v1 = 2 * _random.NextDouble() - 1;
                    v2 = 2 * _random.NextDouble() - 1;
                    rSquared = v1 * v1 + v2 * v2;
                    // ensure within the unit circle
                } while (rSquared >= 1 || rSquared == 0);

                // calculate polar tranformation for each deviate
                var polar = Math.Sqrt(-2 * Math.Log(rSquared) / rSquared);
                // store first deviate
                _storedDeviate = v2 * polar;
                _hasDeviate = true;
                // return second deviate
                return v1 * polar * sigma + mu;
            }
        }

        int t2 = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {
            DateTime t = DateTime.Now;
            t2++;

            int limite = 50;
            while (pid_input.Points.Count > limite) pid_input.Points.RemoveAt(0);
            while (pid_input_maxs.Points.Count > limite) pid_input_maxs.Points.RemoveAt(0);
            while (pid_input_mins.Points.Count > limite) pid_input_mins.Points.RemoveAt(0);
            while (pid_ku.Points.Count > limite) pid_ku.Points.RemoveAt(0);
            while (pid_A.Points.Count > limite) pid_A.Points.RemoveAt(0);

            double caos = 1/d;

            double input =  (1-caos)*Math.Sin(t2)+ (caos)*new GaussianRandom(r).NextGaussian(0, 1);

            pid_input.Points.AddXY(t,input);


            detected(pid_input, pid_input_maxs, pid_input_mins);
            if (pid_input_maxs.Points.Count >= 2 && pid_input_mins.Points.Count >= 1) {

                double Pu = 1e5* Math.Abs(pid_input_maxs.Points[pid_input_maxs.Points.Count - 2].XValue - pid_input_maxs.Points[pid_input_maxs.Points.Count - 1].XValue);

                double A = Math.Abs(pid_input_maxs.Points[pid_input_maxs.Points.Count - 1].YValues[0] - pid_input_mins.Points[pid_input_mins.Points.Count - 1].YValues[0]);

                pid_ku.Points.AddY(Pu);
                pid_A.Points.AddY(A);

                label1.Text="";
                label1.Text += "D="+d+"\nA="+A+"\nPu="+Pu+"\n\n";

                double ku = 4*d/(A*Math.PI);

                label1.Text += "\nZiegler-Nicholas method\n\n";

                label1.Text+="Ku=4*D/(A*Math.Pi)="+ku+"\n\n";

                double pi_kp = 0.4 * ku;
                double pi_ki = 0.48 * ku / Pu;
                label1.Text += "******** PI Controller ********\n";
                label1.Text += "Kp = 0.4 * Ku="+pi_kp+"\n";
                label1.Text += "Ki = 0.48 * Ku / Pu =" + pi_ki + "\n";

                double pid_kp = 0.6 * ku;
                double pid_ki = 1.2 * ku / Pu;
                double pid_kd = 0.0075 * ku * Pu;
                label1.Text += "******** PID Controller ********\n";
                label1.Text += "Kp = 0.6 * Ku =" + pid_kp + "\n";
                label1.Text += "Ki = 1.2 * Ku / Pu ="+pid_ki+"\n";
                label1.Text += "Kd = 0.075 * Ku * Pu ="+pid_kd+"\n";



            }
            

            this.chart1.ChartAreas[0].RecalculateAxesScale();
            this.chart2.ChartAreas[0].RecalculateAxesScale();
            this.chart3.ChartAreas[0].RecalculateAxesScale();
            this.chart1.ChartAreas[0].AxisY.IsStartedFromZero = false;
            this.chart2.ChartAreas[0].AxisY.IsStartedFromZero = false;
            this.chart3.ChartAreas[0].AxisY.IsStartedFromZero = false;
        }

        double d = 1;

        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            d = trackBar1.Value;
        }

       

      
    }
}
