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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend2 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea3 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend3 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.chart_acc = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart_acc_speed = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart_acc_position = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            ((System.ComponentModel.ISupportInitialize)(this.chart_acc)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_acc_speed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_acc_position)).BeginInit();
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
            this.timer1.Interval = 30;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // chart_acc
            // 
            chartArea1.Name = "ChartArea1";
            this.chart_acc.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.chart_acc.Legends.Add(legend1);
            this.chart_acc.Location = new System.Drawing.Point(33, 30);
            this.chart_acc.Name = "chart_acc";
            this.chart_acc.Size = new System.Drawing.Size(494, 136);
            this.chart_acc.TabIndex = 0;
            // 
            // chart_acc_speed
            // 
            chartArea2.Name = "ChartArea1";
            this.chart_acc_speed.ChartAreas.Add(chartArea2);
            legend2.Name = "Legend1";
            this.chart_acc_speed.Legends.Add(legend2);
            this.chart_acc_speed.Location = new System.Drawing.Point(33, 172);
            this.chart_acc_speed.Name = "chart_acc_speed";
            this.chart_acc_speed.Size = new System.Drawing.Size(494, 136);
            this.chart_acc_speed.TabIndex = 1;
            // 
            // chart_acc_position
            // 
            chartArea3.Name = "ChartArea1";
            this.chart_acc_position.ChartAreas.Add(chartArea3);
            legend3.Name = "Legend1";
            this.chart_acc_position.Legends.Add(legend3);
            this.chart_acc_position.Location = new System.Drawing.Point(33, 314);
            this.chart_acc_position.Name = "chart_acc_position";
            this.chart_acc_position.Size = new System.Drawing.Size(494, 136);
            this.chart_acc_position.TabIndex = 2;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.Silver;
            this.pictureBox1.Location = new System.Drawing.Point(533, 30);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(408, 388);
            this.pictureBox1.TabIndex = 3;
            this.pictureBox1.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(953, 496);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.chart_acc_position);
            this.Controls.Add(this.chart_acc_speed);
            this.Controls.Add(this.chart_acc);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.chart_acc)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_acc_speed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_acc_position)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_acc;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_acc_speed;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_acc_position;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Timer timer2;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
    }
}

