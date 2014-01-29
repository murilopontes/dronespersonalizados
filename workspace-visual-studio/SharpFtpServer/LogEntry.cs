using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SharpFtpServer
{
    // Fields: date time c-ip c-port cs-username cs-method cs-uri-stem sc-status sc-bytes cs-bytes s-name s-port

    internal class LogEntry
    {
        public DateTime Date { get; set; }
        public string CIP { get; set; }
        public string CPort { get; set; }
        public string CSUsername { get; set; }
        public string CSMethod { get; set; }
        public string CSUriStem { get; set; }
        public string SCStatus { get; set; }
        public string SCBytes { get; set; }
        public string CSBytes { get; set; }
        public string SName { get; set; }
        public string SPort { get; set; }

        public override string ToString()
        {
            return string.Format("{0:yyyy-MM-dd HH:mm:ss} {1} {2} {3} {4} {5} {6} {7} {8} {9} {10}",
                Date,
                CIP,
                CPort ?? "-",
                CSUsername,
                CSMethod,
                CSUriStem ?? "-",
                SCStatus,
                SCBytes ?? "-",
                CSBytes ?? "-",
                SName ?? "-",
                SPort ?? "-"
                );
        }
    }
}
