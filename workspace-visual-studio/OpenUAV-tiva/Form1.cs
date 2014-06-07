using murix_utils;
using SlimDX;
using SlimDX.XInput;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DroneMURIX
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }


        public class TextBoxStreamWriter : TextWriter
        {
            TextBox _output = null;

            public TextBoxStreamWriter(TextBox output)
            {
                _output = output;
            }

            public override void Write(char value)
            {
                //cross thread
                MethodInvoker action = delegate { _output.AppendText(value.ToString()); };
                _output.BeginInvoke(action);
            }

            public override Encoding Encoding
            {
                get { return System.Text.Encoding.UTF8; }
            }
        }

        // That's our custom TextWriter class
        TextWriter _writer = null;

        Gamepad_State_SlimDX joy = new Gamepad_State_SlimDX(UserIndex.One);


        private void Form1_Load(object sender, EventArgs e)
        {
            // Instantiate the writer
            _writer = new TextBoxStreamWriter(this.textBox1);
            // Redirect the out Console stream
            Console.SetOut(_writer);
            Console.WriteLine("Now redirecting output to the text box");

            this.comboBox1.Items.AddRange(SerialPort.GetPortNames());
            this.comboBox1.DropDownStyle = ComboBoxStyle.DropDownList;
            if(this.comboBox1.Items.Count>0) this.comboBox1.SelectedIndex = 0;

            serialPort1.BaudRate = 115200;
            serialPort1.DataReceived += serialPort1_DataReceived;
            serialPort1.ReadTimeout = 50;
            timer1.Start();
        }

        void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            //Console.Write(serialPort1.ReadExisting());
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                if (serialPort1.IsOpen) serialPort1.Close();
                serialPort1.PortName = (string)this.comboBox1.SelectedItem;
                serialPort1.Open();
            }
            catch (Exception ex) {
                MessageBox.Show(ex.Message);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
            if (serialPort1.IsOpen) serialPort1.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            joy.Update();
            if (serialPort1.IsOpen) {

                //
                int yaw = (int)(joy.LeftStick.Position.X * 32768);
                int throttle = (int)(joy.LeftStick.Position.Y * 32768);
                bool ls = joy.LeftStick.Clicked;
                bool lb=joy.LeftShoulder;
                float lt = joy.LeftTrigger;

                //
                bool up = joy.DPad.Up;
                bool down = joy.DPad.Down;
                bool right = joy.DPad.Right;
                bool left = joy.DPad.Left;

                //
                int roll = (int)(joy.RightStick.Position.X * 32768);
                int pitch = (int)(joy.RightStick.Position.Y * 32768);
                bool rs = joy.RightStick.Clicked;
                bool rb = joy.RightShoulder;
                float rt = joy.RightTrigger;

                //
                bool start = joy.Start;
                bool back = joy.Back;

                //                
                bool a=joy.A;
                bool b=joy.B;
                bool x = joy.X;
                bool y = joy.Y;

                //




                string saida = "joy(" + throttle + "," + roll +"," + pitch + "," + yaw + ")\r\n";
                try
                {
                    serialPort1.Write(saida);
                    string lido = serialPort1.ReadTo("ok");
                    Console.WriteLine(lido);
                }
                catch (Exception ex) { 
                    
                }
            }
        }



    }
}
