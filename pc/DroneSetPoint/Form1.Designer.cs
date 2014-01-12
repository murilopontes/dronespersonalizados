namespace DroneSetPoint
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
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend2 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea3 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend3 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea4 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend4 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series4 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea5 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend5 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series5 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.chart_pitch = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart_roll = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart_yaw = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart_height = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.pictureBox_horizontal = new System.Windows.Forms.PictureBox();
            this.pictureBox_vertical = new System.Windows.Forms.PictureBox();
            this.chart_battery = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.backgroundWorker_battery = new System.ComponentModel.BackgroundWorker();
            this.backgroundWorker_video_horizontal = new System.ComponentModel.BackgroundWorker();
            this.backgroundWorker_video_vertical = new System.ComponentModel.BackgroundWorker();
            this.timer_cameras = new System.Windows.Forms.Timer(this.components);
            this.backgroundWorker_cameras = new System.ComponentModel.BackgroundWorker();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.timer_wifi = new System.Windows.Forms.Timer(this.components);
            this.backgroundWorker_telnet = new System.ComponentModel.BackgroundWorker();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.backgroundWorker_navboard = new System.ComponentModel.BackgroundWorker();
            ((System.ComponentModel.ISupportInitialize)(this.chart_pitch)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_roll)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_yaw)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_height)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_horizontal)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_vertical)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_battery)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // chart_pitch
            // 
            chartArea1.Name = "ChartArea1";
            this.chart_pitch.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.chart_pitch.Legends.Add(legend1);
            this.chart_pitch.Location = new System.Drawing.Point(9, 292);
            this.chart_pitch.Name = "chart_pitch";
            series1.ChartArea = "ChartArea1";
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            this.chart_pitch.Series.Add(series1);
            this.chart_pitch.Size = new System.Drawing.Size(433, 160);
            this.chart_pitch.TabIndex = 0;
            this.chart_pitch.Text = "chart1";
            // 
            // chart_roll
            // 
            chartArea2.Name = "ChartArea1";
            this.chart_roll.ChartAreas.Add(chartArea2);
            legend2.Name = "Legend1";
            this.chart_roll.Legends.Add(legend2);
            this.chart_roll.Location = new System.Drawing.Point(9, 458);
            this.chart_roll.Name = "chart_roll";
            series2.ChartArea = "ChartArea1";
            series2.Legend = "Legend1";
            series2.Name = "Series1";
            this.chart_roll.Series.Add(series2);
            this.chart_roll.Size = new System.Drawing.Size(433, 160);
            this.chart_roll.TabIndex = 1;
            this.chart_roll.Text = "chart2";
            // 
            // chart_yaw
            // 
            chartArea3.Name = "ChartArea1";
            this.chart_yaw.ChartAreas.Add(chartArea3);
            legend3.Name = "Legend1";
            this.chart_yaw.Legends.Add(legend3);
            this.chart_yaw.Location = new System.Drawing.Point(448, 293);
            this.chart_yaw.Name = "chart_yaw";
            series3.ChartArea = "ChartArea1";
            series3.Legend = "Legend1";
            series3.Name = "Series1";
            this.chart_yaw.Series.Add(series3);
            this.chart_yaw.Size = new System.Drawing.Size(402, 160);
            this.chart_yaw.TabIndex = 2;
            this.chart_yaw.Text = "chart3";
            // 
            // chart_height
            // 
            chartArea4.Name = "ChartArea1";
            this.chart_height.ChartAreas.Add(chartArea4);
            legend4.Name = "Legend1";
            this.chart_height.Legends.Add(legend4);
            this.chart_height.Location = new System.Drawing.Point(448, 459);
            this.chart_height.Name = "chart_height";
            series4.ChartArea = "ChartArea1";
            series4.Legend = "Legend1";
            series4.Name = "Series1";
            this.chart_height.Series.Add(series4);
            this.chart_height.Size = new System.Drawing.Size(402, 160);
            this.chart_height.TabIndex = 3;
            this.chart_height.Text = "chart4";
            // 
            // pictureBox_horizontal
            // 
            this.pictureBox_horizontal.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.pictureBox_horizontal.Location = new System.Drawing.Point(594, 12);
            this.pictureBox_horizontal.Name = "pictureBox_horizontal";
            this.pictureBox_horizontal.Size = new System.Drawing.Size(194, 160);
            this.pictureBox_horizontal.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox_horizontal.TabIndex = 4;
            this.pictureBox_horizontal.TabStop = false;
            // 
            // pictureBox_vertical
            // 
            this.pictureBox_vertical.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.pictureBox_vertical.Location = new System.Drawing.Point(794, 12);
            this.pictureBox_vertical.Name = "pictureBox_vertical";
            this.pictureBox_vertical.Size = new System.Drawing.Size(194, 160);
            this.pictureBox_vertical.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox_vertical.TabIndex = 5;
            this.pictureBox_vertical.TabStop = false;
            // 
            // chart_battery
            // 
            chartArea5.Name = "ChartArea1";
            this.chart_battery.ChartAreas.Add(chartArea5);
            legend5.Name = "Legend1";
            this.chart_battery.Legends.Add(legend5);
            this.chart_battery.Location = new System.Drawing.Point(12, 131);
            this.chart_battery.Name = "chart_battery";
            series5.ChartArea = "ChartArea1";
            series5.Legend = "Legend1";
            series5.Name = "Series1";
            this.chart_battery.Series.Add(series5);
            this.chart_battery.Size = new System.Drawing.Size(576, 156);
            this.chart_battery.TabIndex = 6;
            this.chart_battery.Text = "chart_ba";
            // 
            // backgroundWorker_battery
            // 
            this.backgroundWorker_battery.WorkerReportsProgress = true;
            this.backgroundWorker_battery.WorkerSupportsCancellation = true;
            this.backgroundWorker_battery.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_battery_DoWork);
            this.backgroundWorker_battery.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker_battery_ProgressChanged);
            // 
            // backgroundWorker_video_horizontal
            // 
            this.backgroundWorker_video_horizontal.WorkerReportsProgress = true;
            this.backgroundWorker_video_horizontal.WorkerSupportsCancellation = true;
            this.backgroundWorker_video_horizontal.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_video_horizontal_DoWork);
            this.backgroundWorker_video_horizontal.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker_video_horizontal_ProgressChanged);
            // 
            // backgroundWorker_video_vertical
            // 
            this.backgroundWorker_video_vertical.WorkerReportsProgress = true;
            this.backgroundWorker_video_vertical.WorkerSupportsCancellation = true;
            this.backgroundWorker_video_vertical.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_video_vertical_DoWork);
            this.backgroundWorker_video_vertical.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker_video_vertical_ProgressChanged);
            // 
            // timer_cameras
            // 
            this.timer_cameras.Enabled = true;
            this.timer_cameras.Tick += new System.EventHandler(this.timer_cameras_Tick);
            // 
            // backgroundWorker_cameras
            // 
            this.backgroundWorker_cameras.WorkerReportsProgress = true;
            this.backgroundWorker_cameras.WorkerSupportsCancellation = true;
            this.backgroundWorker_cameras.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_cameras_DoWork);
            this.backgroundWorker_cameras.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker_cameras_ProgressChanged);
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(12, 12);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(576, 113);
            this.dataGridView1.TabIndex = 7;
            // 
            // timer_wifi
            // 
            this.timer_wifi.Enabled = true;
            this.timer_wifi.Interval = 2000;
            this.timer_wifi.Tick += new System.EventHandler(this.timer_wifi_Tick);
            // 
            // backgroundWorker_telnet
            // 
            this.backgroundWorker_telnet.WorkerReportsProgress = true;
            this.backgroundWorker_telnet.WorkerSupportsCancellation = true;
            this.backgroundWorker_telnet.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_telnet_DoWork);
            this.backgroundWorker_telnet.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker_telnet_ProgressChanged);
            // 
            // textBox1
            // 
            this.textBox1.AcceptsReturn = true;
            this.textBox1.AcceptsTab = true;
            this.textBox1.Location = new System.Drawing.Point(12, 624);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox1.Size = new System.Drawing.Size(817, 121);
            this.textBox1.TabIndex = 8;
            // 
            // backgroundWorker_navboard
            // 
            this.backgroundWorker_navboard.WorkerReportsProgress = true;
            this.backgroundWorker_navboard.WorkerSupportsCancellation = true;
            this.backgroundWorker_navboard.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_navboard_DoWork);
            this.backgroundWorker_navboard.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker_navboard_ProgressChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(995, 757);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.chart_battery);
            this.Controls.Add(this.pictureBox_vertical);
            this.Controls.Add(this.pictureBox_horizontal);
            this.Controls.Add(this.chart_height);
            this.Controls.Add(this.chart_yaw);
            this.Controls.Add(this.chart_roll);
            this.Controls.Add(this.chart_pitch);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Drone Set Point";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.chart_pitch)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_roll)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_yaw)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_height)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_horizontal)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_vertical)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_battery)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart chart_pitch;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_roll;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_yaw;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_height;
        private System.Windows.Forms.PictureBox pictureBox_horizontal;
        private System.Windows.Forms.PictureBox pictureBox_vertical;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_battery;
        private System.ComponentModel.BackgroundWorker backgroundWorker_battery;
        private System.ComponentModel.BackgroundWorker backgroundWorker_video_horizontal;
        private System.ComponentModel.BackgroundWorker backgroundWorker_video_vertical;
        private System.Windows.Forms.Timer timer_cameras;
        private System.ComponentModel.BackgroundWorker backgroundWorker_cameras;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Timer timer_wifi;
        private System.ComponentModel.BackgroundWorker backgroundWorker_telnet;
        private System.Windows.Forms.TextBox textBox1;
        private System.ComponentModel.BackgroundWorker backgroundWorker_navboard;
    }
}

