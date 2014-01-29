using De.Mud.Telnet;
using Net.Graphite.Telnet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace DroneTelnetCmd
{
    class Program
    {

        static void Main(string[] args)
        {
            try
            {

                string cmd = string.Join(" ", args);
                Console.WriteLine("telnet send command>" + cmd);


                TcpClient client = new TcpClient("192.168.1.1", 23);
                Byte[] data = System.Text.Encoding.ASCII.GetBytes(cmd + "\r\n");
                NetworkStream stream = client.GetStream();
                stream.Write(data, 0, data.Length);
                Console.WriteLine("Sent: {0}", cmd);

                data = new Byte[4096];

                int count = 0;
                client.ReceiveTimeout = 200;
                client.SendTimeout = 200;
                DateTime start = DateTime.Now;
                while (client.Connected)
                {
                    String responseData = String.Empty;
                    Int32 bytes = stream.Read(data, 0, data.Length);
                    if (bytes == 0) count++;
                    if (count > 10) break;
                    responseData = System.Text.Encoding.ASCII.GetString(data, 0, bytes);
                    Console.WriteLine("msglen=" + bytes + "{0}", responseData);
                    TimeSpan duration = DateTime.Now - start;
                    if (duration.TotalMilliseconds > 1000) break;
                }
                stream.Close();
                client.Close();
            }
            catch (Exception ex) {
                Console.WriteLine(ex.Message);
            }
        }
    }
}
