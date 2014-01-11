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
            ((System.ComponentModel.ISupportInitialize)(this.chart_pitch)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_roll)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_yaw)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_height)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_horizontal)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_vertical)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_battery)).BeginInit();
            this.SuspendLayout();
            // 
            // chart_pitch
            // 
            chartArea1.Name = "ChartArea1";
            this.chart_pitch.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.chart_pitch.Legends.Add(legend1);
            this.chart_pitch.Location = new System.Drawing.Point(12, 12);
            this.chart_pitch.Name = "chart_pitch";
            series1.ChartArea = "ChartArea1";
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            this.chart_pitch.Series.Add(series1);
            this.chart_pitch.Size = new System.Drawing.Size(307, 160);
            this.chart_pitch.TabIndex = 0;
            this.chart_pitch.Text = "chart1";
            // 
            // chart_roll
            // 
            chartArea2.Name = "ChartArea1";
            this.chart_roll.ChartAreas.Add(chartArea2);
            legend2.Name = "Legend1";
            this.chart_roll.Legends.Add(legend2);
            this.chart_roll.Location = new System.Drawing.Point(12, 178);
            this.chart_roll.Name = "chart_roll";
            series2.ChartArea = "ChartArea1";
            series2.Legend = "Legend1";
            series2.Name = "Series1";
            this.chart_roll.Series.Add(series2);
            this.chart_roll.Size = new System.Drawing.Size(307, 160);
            this.chart_roll.TabIndex = 1;
            this.chart_roll.Text = "chart2";
            // 
            // chart_yaw
            // 
            chartArea3.Name = "ChartArea1";
            this.chart_yaw.ChartAreas.Add(chartArea3);
            legend3.Name = "Legend1";
            this.chart_yaw.Legends.Add(legend3);
            this.chart_yaw.Location = new System.Drawing.Point(734, 12);
            this.chart_yaw.Name = "chart_yaw";
            series3.ChartArea = "ChartArea1";
            series3.Legend = "Legend1";
            series3.Name = "Series1";
            this.chart_yaw.Series.Add(series3);
            this.chart_yaw.Size = new System.Drawing.Size(307, 160);
            this.chart_yaw.TabIndex = 2;
            this.chart_yaw.Text = "chart3";
            // 
            // chart_height
            // 
            chartArea4.Name = "ChartArea1";
            this.chart_height.ChartAreas.Add(chartArea4);
            legend4.Name = "Legend1";
            this.chart_height.Legends.Add(legend4);
            this.chart_height.Location = new System.Drawing.Point(734, 178);
            this.chart_height.Name = "chart_height";
            series4.ChartArea = "ChartArea1";
            series4.Legend = "Legend1";
            series4.Name = "Series1";
            this.chart_height.Series.Add(series4);
            this.chart_height.Size = new System.Drawing.Size(307, 160);
            this.chart_height.TabIndex = 3;
            this.chart_height.Text = "chart4";
            // 
            // pictureBox_horizontal
            // 
            this.pictureBox_horizontal.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.pictureBox_horizontal.Location = new System.Drawing.Point(325, 12);
            this.pictureBox_horizontal.Name = "pictureBox_horizontal";
            this.pictureBox_horizontal.Size = new System.Drawing.Size(403, 310);
            this.pictureBox_horizontal.TabIndex = 4;
            this.pictureBox_horizontal.TabStop = false;
            // 
            // pictureBox_vertical
            // 
            this.pictureBox_vertical.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(128)))), ((int)(((byte)(255)))));
            this.pictureBox_vertical.Location = new System.Drawing.Point(434, 328);
            this.pictureBox_vertical.Name = "pictureBox_vertical";
            this.pictureBox_vertical.Size = new System.Drawing.Size(203, 190);
            this.pictureBox_vertical.TabIndex = 5;
            this.pictureBox_vertical.TabStop = false;
            // 
            // chart_battery
            // 
            chartArea5.Name = "ChartArea1";
            this.chart_battery.ChartAreas.Add(chartArea5);
            legend5.Name = "Legend1";
            this.chart_battery.Legends.Add(legend5);
            this.chart_battery.Location = new System.Drawing.Point(362, 524);
            this.chart_battery.Name = "chart_battery";
            series5.ChartArea = "ChartArea1";
            series5.Legend = "Legend1";
            series5.Name = "Series1";
            this.chart_battery.Series.Add(series5);
            this.chart_battery.Size = new System.Drawing.Size(314, 122);
            this.chart_battery.TabIndex = 6;
            this.chart_battery.Text = "chart_ba";
            // 
            // backgroundWorker_battery
            // 
            this.backgroundWorker_battery.WorkerReportsProgress = true;
            this.backgroundWorker_battery.WorkerSupportsCancellation = true;
            this.backgroundWorker_battery.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_battery_DoWork);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1053, 652);
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
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.chart_pitch)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_roll)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_yaw)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_height)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_horizontal)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_vertical)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart_battery)).EndInit();
            this.ResumeLayout(false);

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
    }
}

