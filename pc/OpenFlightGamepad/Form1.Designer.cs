namespace OpenFlight
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea59 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend59 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series59 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea60 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend60 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series60 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea61 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend61 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series61 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea62 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend62 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series62 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea63 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend63 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series63 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea64 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend64 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series64 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea65 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend65 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series65 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea66 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend66 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series66 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel_send_count = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel_recv_count = new System.Windows.Forms.ToolStripStatusLabel();
            this.chart2 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart3 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart4 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart5 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart6 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart7 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.timer2 = new System.Windows.Forms.Timer(this.components);
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.chart8 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            this.statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart5)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart6)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart7)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart8)).BeginInit();
            this.SuspendLayout();
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // chart1
            // 
            chartArea59.Name = "ChartArea1";
            this.chart1.ChartAreas.Add(chartArea59);
            legend59.Name = "Legend1";
            this.chart1.Legends.Add(legend59);
            this.chart1.Location = new System.Drawing.Point(8, 19);
            this.chart1.Name = "chart1";
            series59.ChartArea = "ChartArea1";
            series59.Legend = "Legend1";
            series59.Name = "Series1";
            this.chart1.Series.Add(series59);
            this.chart1.Size = new System.Drawing.Size(370, 97);
            this.chart1.TabIndex = 0;
            this.chart1.Text = "chart1";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel_send_count,
            this.toolStripStatusLabel_recv_count,
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 622);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(821, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel_send_count
            // 
            this.toolStripStatusLabel_send_count.Name = "toolStripStatusLabel_send_count";
            this.toolStripStatusLabel_send_count.Size = new System.Drawing.Size(72, 17);
            this.toolStripStatusLabel_send_count.Text = "UDP send=0";
            // 
            // toolStripStatusLabel_recv_count
            // 
            this.toolStripStatusLabel_recv_count.Name = "toolStripStatusLabel_recv_count";
            this.toolStripStatusLabel_recv_count.Size = new System.Drawing.Size(69, 17);
            this.toolStripStatusLabel_recv_count.Text = "UDP recv=0";
            // 
            // chart2
            // 
            chartArea60.Name = "ChartArea1";
            this.chart2.ChartAreas.Add(chartArea60);
            legend60.Name = "Legend1";
            this.chart2.Legends.Add(legend60);
            this.chart2.Location = new System.Drawing.Point(8, 122);
            this.chart2.Name = "chart2";
            series60.ChartArea = "ChartArea1";
            series60.Legend = "Legend1";
            series60.Name = "Series1";
            this.chart2.Series.Add(series60);
            this.chart2.Size = new System.Drawing.Size(370, 121);
            this.chart2.TabIndex = 2;
            this.chart2.Text = "chart2";
            // 
            // chart3
            // 
            chartArea61.Name = "ChartArea1";
            this.chart3.ChartAreas.Add(chartArea61);
            legend61.Name = "Legend1";
            this.chart3.Legends.Add(legend61);
            this.chart3.Location = new System.Drawing.Point(8, 368);
            this.chart3.Name = "chart3";
            series61.ChartArea = "ChartArea1";
            series61.Legend = "Legend1";
            series61.Name = "Series1";
            this.chart3.Series.Add(series61);
            this.chart3.Size = new System.Drawing.Size(370, 105);
            this.chart3.TabIndex = 3;
            this.chart3.Text = "chart3";
            // 
            // chart4
            // 
            chartArea62.Name = "ChartArea1";
            this.chart4.ChartAreas.Add(chartArea62);
            legend62.Name = "Legend1";
            this.chart4.Legends.Add(legend62);
            this.chart4.Location = new System.Drawing.Point(6, 479);
            this.chart4.Name = "chart4";
            series62.ChartArea = "ChartArea1";
            series62.Legend = "Legend1";
            series62.Name = "Series1";
            this.chart4.Series.Add(series62);
            this.chart4.Size = new System.Drawing.Size(370, 119);
            this.chart4.TabIndex = 4;
            this.chart4.Text = "chart4";
            this.chart4.Click += new System.EventHandler(this.chart4_Click);
            // 
            // chart5
            // 
            chartArea63.Name = "ChartArea1";
            this.chart5.ChartAreas.Add(chartArea63);
            legend63.Name = "Legend1";
            this.chart5.Legends.Add(legend63);
            this.chart5.Location = new System.Drawing.Point(8, 249);
            this.chart5.Name = "chart5";
            series63.ChartArea = "ChartArea1";
            series63.Legend = "Legend1";
            series63.Name = "Series1";
            this.chart5.Series.Add(series63);
            this.chart5.Size = new System.Drawing.Size(370, 113);
            this.chart5.TabIndex = 5;
            this.chart5.Text = "chart5";
            // 
            // chart6
            // 
            chartArea64.Name = "ChartArea1";
            this.chart6.ChartAreas.Add(chartArea64);
            legend64.Name = "Legend1";
            this.chart6.Legends.Add(legend64);
            this.chart6.Location = new System.Drawing.Point(6, 139);
            this.chart6.Name = "chart6";
            series64.ChartArea = "ChartArea1";
            series64.Legend = "Legend1";
            series64.Name = "Series1";
            this.chart6.Series.Add(series64);
            this.chart6.Size = new System.Drawing.Size(370, 114);
            this.chart6.TabIndex = 6;
            this.chart6.Text = "chart6";
            // 
            // chart7
            // 
            chartArea65.Name = "ChartArea1";
            this.chart7.ChartAreas.Add(chartArea65);
            legend65.Name = "Legend1";
            this.chart7.Legends.Add(legend65);
            this.chart7.Location = new System.Drawing.Point(6, 19);
            this.chart7.Name = "chart7";
            series65.ChartArea = "ChartArea1";
            series65.Legend = "Legend1";
            series65.Name = "Series1";
            this.chart7.Series.Add(series65);
            this.chart7.Size = new System.Drawing.Size(370, 114);
            this.chart7.TabIndex = 7;
            this.chart7.Text = "chart7";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.chart7);
            this.groupBox1.Controls.Add(this.chart6);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(390, 362);
            this.groupBox1.TabIndex = 8;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "UDP TX - send to drone";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.chart1);
            this.groupBox2.Controls.Add(this.chart2);
            this.groupBox2.Controls.Add(this.chart4);
            this.groupBox2.Controls.Add(this.chart5);
            this.groupBox2.Controls.Add(this.chart3);
            this.groupBox2.Location = new System.Drawing.Point(408, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(398, 604);
            this.groupBox2.TabIndex = 9;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "UDP RX - receive from drone";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 256);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(179, 91);
            this.label1.TabIndex = 8;
            this.label1.Text = "XBOX 360 Gamepad configuration\r\nanalog Left X - pitch\r\nanalog Left Y - roll\r\nanal" +
    "og Right X - yaw\r\nanalog Right Y - throttle\r\nButton A - takeoff\r\nButton B - emer" +
    "gency (cutoff motors)";
            // 
            // timer2
            // 
            this.timer2.Enabled = true;
            this.timer2.Tick += new System.EventHandler(this.timer2_Tick);
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(175, 17);
            this.toolStripStatusLabel1.Text = "toolStripStatusLabel_ping_result";
            // 
            // backgroundWorker1
            // 
            this.backgroundWorker1.WorkerReportsProgress = true;
            this.backgroundWorker1.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker1_DoWork);
            this.backgroundWorker1.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker1_ProgressChanged);
            // 
            // chart8
            // 
            chartArea66.Name = "ChartArea1";
            this.chart8.ChartAreas.Add(chartArea66);
            legend66.Name = "Legend1";
            this.chart8.Legends.Add(legend66);
            this.chart8.Location = new System.Drawing.Point(12, 380);
            this.chart8.Name = "chart8";
            series66.ChartArea = "ChartArea1";
            series66.Legend = "Legend1";
            series66.Name = "Series1";
            this.chart8.Series.Add(series66);
            this.chart8.Size = new System.Drawing.Size(370, 114);
            this.chart8.TabIndex = 9;
            this.chart8.Text = "chart8";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(821, 644);
            this.Controls.Add(this.chart8);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.statusStrip1);
            this.Name = "Form1";
            this.Text = "OpenFlight Gamepad";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart5)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart6)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart7)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.chart8)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel_send_count;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel_recv_count;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart2;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart3;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart4;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart5;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart6;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart7;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Timer timer2;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart8;
    }
}

