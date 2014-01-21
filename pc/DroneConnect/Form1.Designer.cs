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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea6 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend6 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea7 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend7 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea8 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend8 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea9 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend9 = new System.Windows.Forms.DataVisualization.Charting.Legend();
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
            this.chart_raw_accel = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart_raw_gyro = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart_raw_echo = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart_raw_accel_z = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            ((System.ComponentModel.ISupportInitialize)(this.chart_ping)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_angle)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_rate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_vbat)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_motors)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_raw_accel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_raw_gyro)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_raw_echo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_raw_accel_z)).BeginInit();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.SuspendLayout();
            // 
            // chart_ping
            // 
            this.chart_ping.BackColor = System.Drawing.Color.Silver;
            chartArea1.Name = "ChartArea1";
            this.chart_ping.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.chart_ping.Legends.Add(legend1);
            this.chart_ping.Location = new System.Drawing.Point(3, 161);
            this.chart_ping.Name = "chart_ping";
            this.chart_ping.Size = new System.Drawing.Size(561, 116);
            this.chart_ping.TabIndex = 1;
            this.chart_ping.Text = "chart1";
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(3, 3);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(561, 152);
            this.dataGridView1.TabIndex = 2;
            // 
            // horizonInstrumentControl1
            // 
            this.horizonInstrumentControl1.Location = new System.Drawing.Point(6, 6);
            this.horizonInstrumentControl1.Name = "horizonInstrumentControl1";
            this.horizonInstrumentControl1.Size = new System.Drawing.Size(175, 176);
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
            this.attitudeIndicatorInstrumentControl1.Location = new System.Drawing.Point(187, 9);
            this.attitudeIndicatorInstrumentControl1.Name = "attitudeIndicatorInstrumentControl1";
            this.attitudeIndicatorInstrumentControl1.Size = new System.Drawing.Size(172, 173);
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
            this.chart_angle.BackColor = System.Drawing.Color.Silver;
            chartArea2.Name = "ChartArea1";
            this.chart_angle.ChartAreas.Add(chartArea2);
            legend2.Name = "Legend1";
            this.chart_angle.Legends.Add(legend2);
            this.chart_angle.Location = new System.Drawing.Point(410, 188);
            this.chart_angle.Name = "chart_angle";
            this.chart_angle.Size = new System.Drawing.Size(324, 171);
            this.chart_angle.TabIndex = 6;
            // 
            // chart_rate
            // 
            this.chart_rate.BackColor = System.Drawing.Color.Silver;
            chartArea3.Name = "ChartArea1";
            this.chart_rate.ChartAreas.Add(chartArea3);
            legend3.Name = "Legend1";
            this.chart_rate.Legends.Add(legend3);
            this.chart_rate.Location = new System.Drawing.Point(410, 9);
            this.chart_rate.Name = "chart_rate";
            this.chart_rate.Size = new System.Drawing.Size(324, 173);
            this.chart_rate.TabIndex = 7;
            // 
            // chart_vbat
            // 
            this.chart_vbat.BackColor = System.Drawing.Color.Silver;
            chartArea4.Name = "ChartArea1";
            this.chart_vbat.ChartAreas.Add(chartArea4);
            legend4.Name = "Legend1";
            this.chart_vbat.Legends.Add(legend4);
            this.chart_vbat.Location = new System.Drawing.Point(6, 222);
            this.chart_vbat.Name = "chart_vbat";
            this.chart_vbat.Size = new System.Drawing.Size(390, 215);
            this.chart_vbat.TabIndex = 8;
            this.chart_vbat.Text = "chart3";
            // 
            // chart_motors
            // 
            this.chart_motors.BackColor = System.Drawing.Color.Silver;
            chartArea5.Name = "ChartArea1";
            this.chart_motors.ChartAreas.Add(chartArea5);
            legend5.Name = "Legend1";
            this.chart_motors.Legends.Add(legend5);
            this.chart_motors.Location = new System.Drawing.Point(6, 443);
            this.chart_motors.Name = "chart_motors";
            this.chart_motors.Size = new System.Drawing.Size(390, 201);
            this.chart_motors.TabIndex = 9;
            this.chart_motors.Text = "chart3";
            // 
            // chart_raw_accel
            // 
            this.chart_raw_accel.BackColor = System.Drawing.Color.LightGray;
            this.chart_raw_accel.BorderlineColor = System.Drawing.Color.Black;
            chartArea6.Name = "ChartArea1";
            this.chart_raw_accel.ChartAreas.Add(chartArea6);
            legend6.Name = "Legend1";
            this.chart_raw_accel.Legends.Add(legend6);
            this.chart_raw_accel.Location = new System.Drawing.Point(16, 19);
            this.chart_raw_accel.Name = "chart_raw_accel";
            this.chart_raw_accel.Size = new System.Drawing.Size(413, 221);
            this.chart_raw_accel.TabIndex = 10;
            // 
            // chart_raw_gyro
            // 
            this.chart_raw_gyro.BackColor = System.Drawing.Color.Silver;
            chartArea7.Name = "ChartArea1";
            this.chart_raw_gyro.ChartAreas.Add(chartArea7);
            legend7.Name = "Legend1";
            this.chart_raw_gyro.Legends.Add(legend7);
            this.chart_raw_gyro.Location = new System.Drawing.Point(435, 19);
            this.chart_raw_gyro.Name = "chart_raw_gyro";
            this.chart_raw_gyro.Size = new System.Drawing.Size(403, 221);
            this.chart_raw_gyro.TabIndex = 11;
            // 
            // chart_raw_echo
            // 
            this.chart_raw_echo.BackColor = System.Drawing.Color.Silver;
            chartArea8.Name = "ChartArea1";
            this.chart_raw_echo.ChartAreas.Add(chartArea8);
            legend8.Name = "Legend1";
            this.chart_raw_echo.Legends.Add(legend8);
            this.chart_raw_echo.Location = new System.Drawing.Point(435, 246);
            this.chart_raw_echo.Name = "chart_raw_echo";
            this.chart_raw_echo.Size = new System.Drawing.Size(403, 265);
            this.chart_raw_echo.TabIndex = 12;
            // 
            // chart_raw_accel_z
            // 
            this.chart_raw_accel_z.BackColor = System.Drawing.Color.Silver;
            chartArea9.Name = "ChartArea1";
            this.chart_raw_accel_z.ChartAreas.Add(chartArea9);
            legend9.Name = "Legend1";
            this.chart_raw_accel_z.Legends.Add(legend9);
            this.chart_raw_accel_z.Location = new System.Drawing.Point(16, 246);
            this.chart_raw_accel_z.Name = "chart_raw_accel_z";
            this.chart_raw_accel_z.Size = new System.Drawing.Size(413, 265);
            this.chart_raw_accel_z.TabIndex = 13;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Location = new System.Drawing.Point(12, 12);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(989, 676);
            this.tabControl1.TabIndex = 14;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.chart_raw_accel);
            this.tabPage1.Controls.Add(this.chart_raw_echo);
            this.tabPage1.Controls.Add(this.chart_raw_accel_z);
            this.tabPage1.Controls.Add(this.chart_raw_gyro);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(981, 650);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Navboard RAW";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.chart_angle);
            this.tabPage2.Controls.Add(this.chart_motors);
            this.tabPage2.Controls.Add(this.chart_rate);
            this.tabPage2.Controls.Add(this.chart_vbat);
            this.tabPage2.Controls.Add(this.attitudeIndicatorInstrumentControl1);
            this.tabPage2.Controls.Add(this.horizonInstrumentControl1);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(981, 650);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Navboard Fusion + Vbat + Motors";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.dataGridView1);
            this.tabPage3.Controls.Add(this.chart_ping);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(981, 650);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "WiFi";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1013, 700);
            this.Controls.Add(this.tabControl1);
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
            ((System.ComponentModel.ISupportInitialize)(this.chart_raw_accel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_raw_gyro)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_raw_echo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_raw_accel_z)).EndInit();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
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
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_raw_accel;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_raw_gyro;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_raw_echo;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_raw_accel_z;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabPage tabPage3;
    }
}

