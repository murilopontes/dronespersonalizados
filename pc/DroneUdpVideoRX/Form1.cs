using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DroneUdpVideoRX
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }




        private void Form1_Load(object sender, EventArgs e1)
        {

            backgroundWorker_6969.RunWorkerAsync();
            backgroundWorker_7070.RunWorkerAsync();


        }





        ConcurrentQueue<Bitmap> frames_6969 = new ConcurrentQueue<Bitmap>();
        private void backgroundWorker_6969_DoWork(object sender, DoWorkEventArgs e)
        {
            int listenPort = 6969;
            UdpClient listener = new UdpClient(listenPort);
            listener.DontFragment = true;
            IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, listenPort);
            int packet_count = 0;
            while (true)
            {

                byte[] receive_byte_array = listener.Receive(ref groupEP);
                packet_count++;
                Console.WriteLine("port=" + listenPort + " count=" + packet_count + " len=" + receive_byte_array.Length);
                
                byte[] rgb = new byte[176*144*3];
                for (int i = 0; i < receive_byte_array.Length; i++) {
                    rgb[i] = receive_byte_array[i];
                }

                Bitmap bmp_work = new Bitmap(176, 144, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
                int z = 0;
                for (int y = 0; y < 144; y++)
                for (int x = 0; x < 176; x++)
                {
                    {
                        bmp_work.SetPixel(x, y, Color.FromArgb(rgb[z],rgb[z+1],rgb[z+2]));
                        z+=3;
                    }
                }
                frames_6969.Enqueue(bmp_work);
                this.backgroundWorker_6969.ReportProgress(1);

            }
        }



        int vertical_dequeue = 0;
        private void backgroundWorker_6969_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            Bitmap output;
            if (frames_6969.TryDequeue(out output))
            {
                vertical_dequeue++;
                this.pictureBox_vertical.Image = (Bitmap)output.Clone();
                this.pictureBox_vertical.Refresh();
                this.toolStripStatusLabel_vertical.Text = "V=" + vertical_dequeue;
            }

        }


        private void backgroundWorker_7070_DoWork(object sender, DoWorkEventArgs e)
        {
            int listenPort = 7070;
            UdpClient listener = new UdpClient(listenPort);
            listener.DontFragment = true;
            IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, listenPort);
            int packet_count = 0;
            while (true)
            {
                byte[] receive_byte_array = listener.Receive(ref groupEP);
                packet_count++;
                Console.WriteLine("port=" + listenPort + " count=" + packet_count + " len=" + receive_byte_array.Length);
            }
        }

        private void backgroundWorker_7070_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {

        }




    }
}
