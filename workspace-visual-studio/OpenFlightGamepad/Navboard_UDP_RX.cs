using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace murix_utils
{
    class UDP_RX_Navboard
    {
        public static int fields = 26;
        public static int calib_samples_total = 10000;

        static bool run = true;
        static UdpClient listener = new UdpClient(8080);

        private static void navboard_work()
        {
            IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, 0);
            Stopwatch time = new Stopwatch();
            time.Start();
            long now = time.ElapsedTicks;
            long pre = now;

            List<double[]> table = new List<double[]>();
            for (int i = 0; i < fields; i++) {
                table.Add(new double[calib_samples_total]);
            }

            int calib_samples_count = 0;
            bool calib_done = false;

            double[] packet_double = new double[fields]; 
            while (run)
            {
                
                //////////////////////////////////////////////////////////////////////
                byte[] receive_byte_array = listener.Receive(ref groupEP);
                //incomplete packet
                if (receive_byte_array.Length != (fields*2))
                {
                    Console.WriteLine("packet error len=" + receive_byte_array.Length);
                    continue;
                }
                //converte to double packet
                int j = 0;
                for (int i = 0; i < (fields*2); i += 2)
                {
                    Int16 tmp = (Int16)(receive_byte_array[i] + (receive_byte_array[i + 1] << 8));
                    UInt16 utmp = (UInt16)(receive_byte_array[i] + (receive_byte_array[i + 1] << 8));
                    packet_double[j] = utmp;

                    if (calib_samples_count < calib_samples_total && !calib_done) {
                        table[j][calib_samples_count] = utmp;               
                    }
                    //
                    j++;
                }


                if (calib_samples_count == calib_samples_total - 1) {
                    for (int i = 0; i < fields; i++)
                    {
                        Console.WriteLine("------------ serie i="+i+" ---------------");
                        Console.WriteLine("avg=" + sensors_fusion.avg(table[i]) + " std=" + sensors_fusion.stddev(table[i]));
                        //HistSample.CalculateOptimalBinWidth(table[i]);
                        sensors_fusion.histrogram(table[i]);
                    }


                    calib_done = true;
                }
                calib_samples_count++;
               // Console.WriteLine(string.Join("\t", packet_double));

                //////////////////////////////////////////////////////////////////////
                pre = now;
                now = time.ElapsedTicks;
                long dt_ticks = now-pre;
                double dt_s = dt_ticks/(double)Stopwatch.Frequency;
                sensors_fusion.Update(

                    dt_s, 
                    
                    packet_double[2], 
                    packet_double[3], 
                    packet_double[4], 
                    
                    packet_double[5], 
                    packet_double[6], 
                    packet_double[7], 
                    
                    packet_double[17]);
            }

        }



        static Thread fd;

        public static void Start()
        {
            fd = new Thread(navboard_work);
            fd.Start();
        }

        public static void Stop() {
            listener.Close();
            run = false;
            fd.Interrupt();
            fd.Abort();
        }

    }
}
