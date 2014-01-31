namespace SerialPlotAccel
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea7 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend7 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea8 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend8 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea9 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend9 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.chart_acc = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart_speed = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart_position = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.chart_acc)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_speed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_position)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // serialPort1
            // 
            this.serialPort1.BaudRate = 115200;
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 1;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // chart_acc
            // 
            chartArea7.Name = "ChartArea1";
            this.chart_acc.ChartAreas.Add(chartArea7);
            legend7.Name = "Legend1";
            this.chart_acc.Legends.Add(legend7);
            this.chart_acc.Location = new System.Drawing.Point(33, 30);
            this.chart_acc.Name = "chart_acc";
            this.chart_acc.Size = new System.Drawing.Size(494, 136);
            this.chart_acc.TabIndex = 0;
            this.chart_acc.Text = "chart_acc";
            // 
            // chart_speed
            // 
            chartArea8.Name = "ChartArea1";
            this.chart_speed.ChartAreas.Add(chartArea8);
            legend8.Name = "Legend1";
            this.chart_speed.Legends.Add(legend8);
            this.chart_speed.Location = new System.Drawing.Point(33, 172);
            this.chart_speed.Name = "chart_speed";
            this.chart_speed.Size = new System.Drawing.Size(494, 136);
            this.chart_speed.TabIndex = 1;
            this.chart_speed.Text = "chart_acc";
            // 
            // chart_position
            // 
            chartArea9.Name = "ChartArea1";
            this.chart_position.ChartAreas.Add(chartArea9);
            legend9.Name = "Legend1";
            this.chart_position.Legends.Add(legend9);
            this.chart_position.Location = new System.Drawing.Point(33, 314);
            this.chart_position.Name = "chart_position";
            this.chart_position.Size = new System.Drawing.Size(494, 136);
            this.chart_position.TabIndex = 2;
            this.chart_position.Text = "chart_acc";
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.Silver;
            this.pictureBox1.Location = new System.Drawing.Point(533, 30);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(408, 420);
            this.pictureBox1.TabIndex = 3;
            this.pictureBox1.TabStop = false;
            // 
            // timer2
            // 
            this.timer2.Enabled = true;
            this.timer2.Interval = 30;
            this.timer2.Tick += new System.EventHandler(this.timer2_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(953, 496);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.chart_position);
            this.Controls.Add(this.chart_speed);
            this.Controls.Add(this.chart_acc);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.chart_acc)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_speed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_position)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_acc;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_speed;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_position;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Timer timer2;
    }
}

