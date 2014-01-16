using SimpleWifi;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Speech.Synthesis;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace murix_utils
{
    public partial class Form1 : Form
    {

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            UDP_TX_Gamepad.Start();
            Voice_Alarm.Start();
            Wifi_Monitor.Start();
            Ping_Monitor.Start();
            UDP_RX_Navboard.Start();
            timer_rssi.Enabled = true;
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            timer_rssi.Enabled = false;
            UDP_TX_Gamepad.Stop();
            UDP_RX_Navboard.Stop();
            Ping_Monitor.Stop();
            Wifi_Monitor.Stop();
            Voice_Alarm.Stop();
        }

        private void timer_rssi_Tick(object sender, EventArgs e)
        {
            progressBar_ping.Value = Ping_Monitor.rtt;
            progressBar_RSSI.Value = Wifi_Monitor.rssi;
        }




       

    }
}
