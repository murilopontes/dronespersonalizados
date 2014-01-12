using SharpFtpServer;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO;
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
            FtpServer server = new FtpServer();
            server.Start();

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
            this.backgroundWorker_video_vertical.RunWorkerAsync();
            this.backgroundWorker_video_horizontal.RunWorkerAsync();
            this.backgroundWorker_cameras.RunWorkerAsync();
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

                    while (serie_battery.Points.Count > 1000)
                    {
                        serie_battery.Points.RemoveAt(0);
                    }
                    //serie_battery.Points.AddXY(vbat_packet, vbat_voltage);
                    serie_battery.Points.AddY( vbat_voltage);
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


        static double[,] YUV2RGB_CONVERT_MATRIX = new double[3, 3] { { 1, 0, 1.4022 }, { 1, -0.3456, -0.7145 }, { 1, 1.771, 0 } };
        static byte clamp(float input)
        {
            if (input < 0) input = 0;
            if (input > 255) input = 255;
            return (byte)Math.Abs(input);
        }

        static Bitmap ConvertYUV2RGB(byte[] yuvFrame, int width, int height)
        {
            int uIndex = width * height;
            int vIndex = uIndex + ((width * height) >> 2);
            int gIndex = width * height;
            int bIndex = gIndex * 2;

            int temp = 0;

            byte[] rgbFrame = new byte[width * height * 3];
            Bitmap bm = new Bitmap(width, height);

            int r = 0;
            int g = 0;
            int b = 0;
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    // R
                    temp = (int)(yuvFrame[y * width + x] + (yuvFrame[vIndex + (y / 2) * (width / 2) + x / 2] - 128) * YUV2RGB_CONVERT_MATRIX[0, 2]);
                    rgbFrame[y * width + x] = (byte)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
                    // G
                    temp = (int)(yuvFrame[y * width + x] + (yuvFrame[uIndex + (y / 2) * (width / 2) + x / 2] - 128) * YUV2RGB_CONVERT_MATRIX[1, 1] + (yuvFrame[vIndex + (y / 2) * (width / 2) + x / 2] - 128) * YUV2RGB_CONVERT_MATRIX[1, 2]);
                    rgbFrame[gIndex + y * width + x] = (byte)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
                    // B
                    temp = (int)(yuvFrame[y * width + x] + (yuvFrame[uIndex + (y / 2) * (width / 2) + x / 2] - 128) * YUV2RGB_CONVERT_MATRIX[2, 1]);
                    rgbFrame[bIndex + y * width + x] = (byte)(temp < 0 ? 0 : (temp > 255 ? 255 : temp));
                    Color c = Color.FromArgb(rgbFrame[y * width + x], rgbFrame[gIndex + y * width + x], rgbFrame[bIndex + y * width + x]);
                    bm.SetPixel(x, y, c);
                }
            }
            return bm;

        }


        ConcurrentQueue<Bitmap> frames_vertical = new ConcurrentQueue<Bitmap>();
        private void backgroundWorker_video_vertical_DoWork(object sender, DoWorkEventArgs e)
        {

            int listenPort = 6969;
            int width = 176;
            int height = 144;

            UdpClient listener = new UdpClient(listenPort);
            listener.DontFragment = true;
            IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, listenPort);
            byte[] image = new byte[1024*1024*3];

            while (true)
            {
                byte[] receive_byte_array = listener.Receive(ref groupEP);
                int tipo = BitConverter.ToInt32(receive_byte_array, 0);
                int offset = BitConverter.ToInt32(receive_byte_array, 4);
                Console.WriteLine("tipo="+tipo+" offset="+offset);
                if (tipo == 0) {
                    image = new byte[1024 * 1024 * 3];
                }
                if (tipo == 1) {
                    for (int idx = 0; idx < 1024; idx++) {
                        image[offset+idx] = receive_byte_array[idx + 8];
                    }
                }
                if (tipo == 2) {
                    Bitmap bmp_work = ConvertYUV2RGB(image, width, height);
                    frames_vertical.Enqueue(bmp_work);
                    this.backgroundWorker_video_vertical.ReportProgress(1);
                }
            }
        }

        private void backgroundWorker_video_vertical_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            Bitmap output;
            if (frames_vertical.TryDequeue(out output))
            {
                this.pictureBox_vertical.Image = (Bitmap)output.Clone();
                this.pictureBox_vertical.Refresh();
            }
        }


        ConcurrentQueue<Bitmap> frames_horizontal = new ConcurrentQueue<Bitmap>();
        private void backgroundWorker_video_horizontal_DoWork(object sender, DoWorkEventArgs e)
        {
            int listenPort = 7070;
            int width = 640;
            int height = 480;

            UdpClient listener = new UdpClient(listenPort);
            listener.DontFragment = true;
            IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, listenPort);
            byte[] image = new byte[1024 * 1024 * 3];

            while (true)
            {
                byte[] receive_byte_array = listener.Receive(ref groupEP);
                int tipo = BitConverter.ToInt32(receive_byte_array, 0);
                int offset = BitConverter.ToInt32(receive_byte_array, 4);
                Console.WriteLine("tipo=" + tipo + " offset=" + offset);
                if (tipo == 0)
                {
                    image = new byte[1024 * 1024 * 3];
                }
                if (tipo == 1)
                {
                    for (int idx = 0; idx < 1024; idx++)
                    {
                        image[offset + idx] = receive_byte_array[idx + 8];
                    }
                }
                if (tipo == 2)
                {
                    Bitmap bmp_work = ConvertYUV2RGB(image, width, height);
                    frames_horizontal.Enqueue(bmp_work);
                    this.backgroundWorker_video_horizontal.ReportProgress(1);
                }
            }
        }

        private void backgroundWorker_video_horizontal_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            Bitmap output;
            if (frames_horizontal.TryDequeue(out output))
            {
                this.pictureBox_horizontal.Image = (Bitmap)output.Clone();
                this.pictureBox_horizontal.Refresh();
            }
        }

        private void timer_cameras_Tick(object sender, EventArgs e)
        {
            try
            {

              //pictureBox_vertical.Image = ConvertYUV2RGB(File.ReadAllBytes("c:\\ftp\\vertical.yuv"), 176, 144);
              //pictureBox_horizontal.Image = ConvertYUV2RGB(File.ReadAllBytes("c:\\ftp\\horizontal.yuv"), 640, 480);
            }
            catch (Exception ex)
            {

            }
        }

        Bitmap bmp_vertical = null;
        Bitmap bmp_horizontal = null;
        private void backgroundWorker_cameras_DoWork(object sender, DoWorkEventArgs e)
        {
            while (true) {
                try
                {
                    Bitmap bmp = ConvertYUV2RGB(File.ReadAllBytes("c:\\ftp\\vertical.yuv"), 176, 144);
                    bmp_vertical = (Bitmap) bmp.Clone();
                    backgroundWorker_cameras.ReportProgress(1);
                }
                catch (Exception ex)
                {

                }
                try
                {
                    Bitmap bmp = ConvertYUV2RGB(File.ReadAllBytes("c:\\ftp\\horizontal.yuv"), 640, 480);
                    bmp_horizontal = (Bitmap)bmp.Clone();
                    backgroundWorker_cameras.ReportProgress(1);
                }
                catch (Exception ex)
                {

                }

                Thread.Sleep(10);
            }
        }

        private void backgroundWorker_cameras_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            if (bmp_horizontal != null) {
                pictureBox_horizontal.Image = bmp_horizontal;
            }
            if (bmp_vertical != null) {
                pictureBox_vertical.Image = bmp_vertical;
            }
        }
    }
}
