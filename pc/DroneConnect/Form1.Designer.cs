namespace DroneConnect
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend2 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea3 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend3 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea4 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend4 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea5 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend5 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.chart_ping = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.horizonInstrumentControl1 = new AvionicsInstrumentControlDemo.AttitudeIndicatorInstrumentControl();
            this.backgroundWorker_navboard = new System.ComponentModel.BackgroundWorker();
            this.attitudeIndicatorInstrumentControl1 = new AvionicsInstrumentControlDemo.AttitudeIndicatorInstrumentControl();
            this.backgroundWorker_ping = new System.ComponentModel.BackgroundWorker();
            this.backgroundWorker_joystick = new System.ComponentModel.BackgroundWorker();
            this.backgroundWorker_wifi = new System.ComponentModel.BackgroundWorker();
            this.chart_angle = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart_rate = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart_vbat = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart_motors = new System.Windows.Forms.DataVisualization.Charting.Chart();
            ((System.ComponentModel.ISupportInitialize)(this.chart_ping)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_angle)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_rate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_vbat)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_motors)).BeginInit();
            this.SuspendLayout();
            // 
            // chart_ping
            // 
            chartArea1.Name = "ChartArea1";
            this.chart_ping.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.chart_ping.Legends.Add(legend1);
            this.chart_ping.Location = new System.Drawing.Point(12, 170);
            this.chart_ping.Name = "chart_ping";
            this.chart_ping.Size = new System.Drawing.Size(561, 116);
            this.chart_ping.TabIndex = 1;
            this.chart_ping.Text = "chart1";
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(12, 12);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(561, 152);
            this.dataGridView1.TabIndex = 2;
            // 
            // horizonInstrumentControl1
            // 
            this.horizonInstrumentControl1.Location = new System.Drawing.Point(442, 511);
            this.horizonInstrumentControl1.Name = "horizonInstrumentControl1";
            this.horizonInstrumentControl1.Size = new System.Drawing.Size(131, 130);
            this.horizonInstrumentControl1.TabIndex = 4;
            this.horizonInstrumentControl1.Text = "horizonInstrumentControl1";
            // 
            // backgroundWorker_navboard
            // 
            this.backgroundWorker_navboard.WorkerReportsProgress = true;
            this.backgroundWorker_navboard.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_navboard_DoWork);
            this.backgroundWorker_navboard.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker_navboard_ProgressChanged);
            // 
            // attitudeIndicatorInstrumentControl1
            // 
            this.attitudeIndicatorInstrumentControl1.Location = new System.Drawing.Point(442, 357);
            this.attitudeIndicatorInstrumentControl1.Name = "attitudeIndicatorInstrumentControl1";
            this.attitudeIndicatorInstrumentControl1.Size = new System.Drawing.Size(131, 130);
            this.attitudeIndicatorInstrumentControl1.TabIndex = 5;
            this.attitudeIndicatorInstrumentControl1.Text = "attitudeIndicatorInstrumentControl1";
            // 
            // backgroundWorker_ping
            // 
            this.backgroundWorker_ping.WorkerReportsProgress = true;
            this.backgroundWorker_ping.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_ping_DoWork);
            this.backgroundWorker_ping.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker_ping_ProgressChanged);
            // 
            // backgroundWorker_joystick
            // 
            this.backgroundWorker_joystick.WorkerReportsProgress = true;
            this.backgroundWorker_joystick.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_joystick_DoWork);
            // 
            // backgroundWorker_wifi
            // 
            this.backgroundWorker_wifi.WorkerReportsProgress = true;
            this.backgroundWorker_wifi.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_wifi_DoWork);
            this.backgroundWorker_wifi.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker_wifi_ProgressChanged);
            // 
            // chart_angle
            // 
            chartArea2.Name = "ChartArea1";
            this.chart_angle.ChartAreas.Add(chartArea2);
            legend2.Name = "Legend1";
            this.chart_angle.Legends.Add(legend2);
            this.chart_angle.Location = new System.Drawing.Point(743, 12);
            this.chart_angle.Name = "chart_angle";
            this.chart_angle.Size = new System.Drawing.Size(260, 195);
            this.chart_angle.TabIndex = 6;
            this.chart_angle.Text = "chart2";
            // 
            // chart_rate
            // 
            chartArea3.Name = "ChartArea1";
            this.chart_rate.ChartAreas.Add(chartArea3);
            legend3.Name = "Legend1";
            this.chart_rate.Legends.Add(legend3);
            this.chart_rate.Location = new System.Drawing.Point(579, 424);
            this.chart_rate.Name = "chart_rate";
            this.chart_rate.Size = new System.Drawing.Size(424, 195);
            this.chart_rate.TabIndex = 7;
            this.chart_rate.Text = "chart3";
            // 
            // chart_vbat
            // 
            chartArea4.Name = "ChartArea1";
            this.chart_vbat.ChartAreas.Add(chartArea4);
            legend4.Name = "Legend1";
            this.chart_vbat.Legends.Add(legend4);
            this.chart_vbat.Location = new System.Drawing.Point(12, 292);
            this.chart_vbat.Name = "chart_vbat";
            this.chart_vbat.Size = new System.Drawing.Size(424, 195);
            this.chart_vbat.TabIndex = 8;
            this.chart_vbat.Text = "chart3";
            // 
            // chart_motors
            // 
            chartArea5.Name = "ChartArea1";
            this.chart_motors.ChartAreas.Add(chartArea5);
            legend5.Name = "Legend1";
            this.chart_motors.Legends.Add(legend5);
            this.chart_motors.Location = new System.Drawing.Point(12, 493);
            this.chart_motors.Name = "chart_motors";
            this.chart_motors.Size = new System.Drawing.Size(424, 195);
            this.chart_motors.TabIndex = 9;
            this.chart_motors.Text = "chart3";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1013, 700);
            this.Controls.Add(this.chart_motors);
            this.Controls.Add(this.chart_vbat);
            this.Controls.Add(this.chart_rate);
            this.Controls.Add(this.chart_angle);
            this.Controls.Add(this.attitudeIndicatorInstrumentControl1);
            this.Controls.Add(this.horizonInstrumentControl1);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.chart_ping);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Drone Connect";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.chart_ping)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_angle)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_rate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_vbat)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_motors)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart chart_ping;
        private System.Windows.Forms.DataGridView dataGridView1;
        private AvionicsInstrumentControlDemo.AttitudeIndicatorInstrumentControl horizonInstrumentControl1;
        private System.ComponentModel.BackgroundWorker backgroundWorker_navboard;
        private AvionicsInstrumentControlDemo.AttitudeIndicatorInstrumentControl attitudeIndicatorInstrumentControl1;
        private System.ComponentModel.BackgroundWorker backgroundWorker_ping;
        private System.ComponentModel.BackgroundWorker backgroundWorker_joystick;
        private System.ComponentModel.BackgroundWorker backgroundWorker_wifi;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_angle;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_rate;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_vbat;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_motors;
    }
}

