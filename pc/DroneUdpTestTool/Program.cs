using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DroneUdpTestTool
{
    class Program
    {

        static void Main(string[] args)
        {

            // This constructor arbitrarily assigns the local port number.
            UdpClient udpClient = new UdpClient();
            try
            {
                udpClient.Connect("192.168.1.1", 4000);

                int tx_count = 0;
                int rx_count = 0;

                while (true)
                {
                    // Sends a message to the host to which you have connected.
                    Byte[] sendBytes = Encoding.ASCII.GetBytes("Is anybody there?");
                    udpClient.Send(sendBytes, sendBytes.Length);
                    tx_count++;
                    Console.WriteLine("TX[" + tx_count + "]");
                    if (udpClient.Available > 0) {
                        //IPEndPoint object will allow us to read datagrams sent from any source.
                        IPEndPoint RemoteIpEndPoint = new IPEndPoint(IPAddress.Any, 0);
                        // Blocks until a message returns on this socket from a remote host.
                        Byte[] receiveBytes = udpClient.Receive(ref RemoteIpEndPoint);
                        rx_count++;
                        string returnData = Encoding.ASCII.GetString(receiveBytes);
                        // Uses the IPEndPoint object to determine which of these two hosts responded.
                        Console.WriteLine("RX["+rx_count+"]=" +  returnData.ToString());
                        Console.WriteLine("RX info=" + RemoteIpEndPoint.Address.ToString() + ":" + RemoteIpEndPoint.Port.ToString());
                        
                    }
                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
            Console.ReadKey();
        }
    }
}
