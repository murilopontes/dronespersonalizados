using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;

namespace SharpFtpServer
{
    class Program
    {
        static void Main(string[] args)
        {
            using (FtpServer server = new FtpServer(IPAddress.IPv6Any, 21))
            {
                server.Start();

                Console.WriteLine("Press any key to stop...");
                Console.ReadKey(true);
            }
        }
    }
}
