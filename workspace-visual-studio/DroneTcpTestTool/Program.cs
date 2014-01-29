using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace DroneTcpTestTool
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                
                TcpClient client = new TcpClient("192.168.1.1", 4000);
                client.SendTimeout = 50;
                client.ReceiveTimeout = 50;

                // Translate the passed message into ASCII and store it as a Byte array.
                string message = "Is anybody there?";
                Byte[] data = System.Text.Encoding.ASCII.GetBytes(message);

                // Get a client stream for reading and writing. 
                //  Stream stream = client.GetStream();

                NetworkStream stream = client.GetStream();

                while (true)
                {
                    stream.Write(data, 0, data.Length);
                    Console.WriteLine("Sent: {0}", message);
                    // Buffer to store the response bytes.
                    data = new Byte[256];
                    // String to store the response ASCII representation.
                    String responseData = String.Empty;
                    // Read the first batch of the TcpServer response bytes.
                    Int32 bytes = stream.Read(data, 0, data.Length);
                    responseData = System.Text.Encoding.ASCII.GetString(data, 0, bytes);
                    Console.WriteLine("Received: {0}", responseData);
                }

                // Close everything.
                stream.Close();
                client.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine("ex=", ex.Message);
            }
            Console.ReadKey();

         
        }
    }
}
