namespace AvionicsInstrumentControlDemo
{
    partial class DemoWinow
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DemoWinow));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label8 = new System.Windows.Forms.Label();
            this.trackBarHeading = new System.Windows.Forms.TrackBar();
            this.label7 = new System.Windows.Forms.Label();
            this.trackBarVerticalSpeed = new System.Windows.Forms.TrackBar();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.trackBarTurnQuality = new System.Windows.Forms.TrackBar();
            this.trackBarTurnRate = new System.Windows.Forms.TrackBar();
            this.trackBarAltitude = new System.Windows.Forms.TrackBar();
            this.trackBarRollAngle = new System.Windows.Forms.TrackBar();
            this.trackPitchAngle = new System.Windows.Forms.TrackBar();
            this.label1 = new System.Windows.Forms.Label();
            this.trackBarAirSpeed = new System.Windows.Forms.TrackBar();
            this.verticalSpeedInstrumentControl1 = new AvionicsInstrumentControlDemo.VerticalSpeedIndicatorInstrumentControl();
            this.headingIndicatorInstrumentControl1 = new AvionicsInstrumentControlDemo.HeadingIndicatorInstrumentControl();
            this.horizonInstrumentControl1 = new AvionicsInstrumentControlDemo.AttitudeIndicatorInstrumentControl();
            this.altimeterInstrumentControl1 = new AvionicsInstrumentControlDemo.AltimeterInstrumentControl();
            this.airSpeedInstrumentControl1 = new AvionicsInstrumentControlDemo.AirSpeedIndicatorInstrumentControl();
            this.turnCoordinatorInstrumentControl1 = new AvionicsInstrumentControlDemo.TurnCoordinatorInstrumentControl();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarHeading)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarVerticalSpeed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTurnQuality)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTurnRate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAltitude)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarRollAngle)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackPitchAngle)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAirSpeed)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.trackBarHeading);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.trackBarVerticalSpeed);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.trackBarTurnQuality);
            this.groupBox1.Controls.Add(this.trackBarTurnRate);
            this.groupBox1.Controls.Add(this.trackBarAltitude);
            this.groupBox1.Controls.Add(this.trackBarRollAngle);
            this.groupBox1.Controls.Add(this.trackPitchAngle);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.trackBarAirSpeed);
            this.groupBox1.Location = new System.Drawing.Point(12, 474);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(662, 230);
            this.groupBox1.TabIndex = 6;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Controls";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(387, 131);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(69, 20);
            this.label8.TabIndex = 15;
            this.label8.Text = "Heading";
            // 
            // trackBarHeading
            // 
            this.trackBarHeading.Location = new System.Drawing.Point(462, 121);
            this.trackBarHeading.Maximum = 360;
            this.trackBarHeading.Name = "trackBarHeading";
            this.trackBarHeading.Size = new System.Drawing.Size(194, 45);
            this.trackBarHeading.TabIndex = 14;
            this.trackBarHeading.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.trackBarHeading.Scroll += new System.EventHandler(this.trackBarHeading_Scroll);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(343, 31);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(113, 20);
            this.label7.TabIndex = 13;
            this.label7.Text = "Vertical Speed";
            // 
            // trackBarVerticalSpeed
            // 
            this.trackBarVerticalSpeed.Location = new System.Drawing.Point(462, 19);
            this.trackBarVerticalSpeed.Maximum = 6000;
            this.trackBarVerticalSpeed.Minimum = -6000;
            this.trackBarVerticalSpeed.Name = "trackBarVerticalSpeed";
            this.trackBarVerticalSpeed.Size = new System.Drawing.Size(194, 45);
            this.trackBarVerticalSpeed.TabIndex = 12;
            this.trackBarVerticalSpeed.TickFrequency = 100;
            this.trackBarVerticalSpeed.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.trackBarVerticalSpeed.Scroll += new System.EventHandler(this.trackBarVerticalSpeed_Scroll);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(363, 184);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(93, 20);
            this.label6.TabIndex = 11;
            this.label6.Text = "Turn Quality";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(6, 184);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(80, 20);
            this.label5.TabIndex = 10;
            this.label5.Text = "Turn Rate";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(6, 131);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(63, 20);
            this.label4.TabIndex = 9;
            this.label4.Text = "Altitude";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(375, 80);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(81, 20);
            this.label3.TabIndex = 8;
            this.label3.Text = "Roll Angle";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(6, 80);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(89, 20);
            this.label2.TabIndex = 7;
            this.label2.Text = "Pitch Angle";
            // 
            // trackBarTurnQuality
            // 
            this.trackBarTurnQuality.Location = new System.Drawing.Point(462, 172);
            this.trackBarTurnQuality.Maximum = 20;
            this.trackBarTurnQuality.Minimum = -20;
            this.trackBarTurnQuality.Name = "trackBarTurnQuality";
            this.trackBarTurnQuality.Size = new System.Drawing.Size(194, 45);
            this.trackBarTurnQuality.TabIndex = 6;
            this.trackBarTurnQuality.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.trackBarTurnQuality.Scroll += new System.EventHandler(this.trackBarTurnQuality_Scroll);
            // 
            // trackBarTurnRate
            // 
            this.trackBarTurnRate.Location = new System.Drawing.Point(120, 172);
            this.trackBarTurnRate.Maximum = 60;
            this.trackBarTurnRate.Minimum = -60;
            this.trackBarTurnRate.Name = "trackBarTurnRate";
            this.trackBarTurnRate.Size = new System.Drawing.Size(201, 45);
            this.trackBarTurnRate.TabIndex = 5;
            this.trackBarTurnRate.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.trackBarTurnRate.Scroll += new System.EventHandler(this.trackBarTurnRate_Scroll);
            // 
            // trackBarAltitude
            // 
            this.trackBarAltitude.Location = new System.Drawing.Point(120, 121);
            this.trackBarAltitude.Maximum = 10000;
            this.trackBarAltitude.Name = "trackBarAltitude";
            this.trackBarAltitude.Size = new System.Drawing.Size(201, 45);
            this.trackBarAltitude.TabIndex = 4;
            this.trackBarAltitude.TickFrequency = 50;
            this.trackBarAltitude.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.trackBarAltitude.Scroll += new System.EventHandler(this.trackBarAltitude_Scroll);
            // 
            // trackBarRollAngle
            // 
            this.trackBarRollAngle.Location = new System.Drawing.Point(462, 70);
            this.trackBarRollAngle.Maximum = 180;
            this.trackBarRollAngle.Minimum = -180;
            this.trackBarRollAngle.Name = "trackBarRollAngle";
            this.trackBarRollAngle.Size = new System.Drawing.Size(194, 45);
            this.trackBarRollAngle.TabIndex = 3;
            this.trackBarRollAngle.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.trackBarRollAngle.Scroll += new System.EventHandler(this.trackBarRollAngle_Scroll);
            // 
            // trackPitchAngle
            // 
            this.trackPitchAngle.Location = new System.Drawing.Point(120, 70);
            this.trackPitchAngle.Maximum = 45;
            this.trackPitchAngle.Minimum = -45;
            this.trackPitchAngle.Name = "trackPitchAngle";
            this.trackPitchAngle.Size = new System.Drawing.Size(201, 45);
            this.trackPitchAngle.TabIndex = 2;
            this.trackPitchAngle.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.trackPitchAngle.Scroll += new System.EventHandler(this.trackPitchAngle_Scroll);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(6, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(79, 20);
            this.label1.TabIndex = 1;
            this.label1.Text = "Air Speed";
            // 
            // trackBarAirSpeed
            // 
            this.trackBarAirSpeed.Location = new System.Drawing.Point(120, 19);
            this.trackBarAirSpeed.Maximum = 800;
            this.trackBarAirSpeed.Name = "trackBarAirSpeed";
            this.trackBarAirSpeed.Size = new System.Drawing.Size(201, 45);
            this.trackBarAirSpeed.TabIndex = 0;
            this.trackBarAirSpeed.TickFrequency = 10;
            this.trackBarAirSpeed.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.trackBarAirSpeed.Scroll += new System.EventHandler(this.trackBarAirSpeed_Scroll);
            // 
            // verticalSpeedInstrumentControl1
            // 
            this.verticalSpeedInstrumentControl1.Location = new System.Drawing.Point(474, 218);
            this.verticalSpeedInstrumentControl1.Name = "verticalSpeedInstrumentControl1";
            this.verticalSpeedInstrumentControl1.Size = new System.Drawing.Size(200, 200);
            this.verticalSpeedInstrumentControl1.TabIndex = 5;
            this.verticalSpeedInstrumentControl1.Text = "verticalSpeedInstrumentControl1";
            // 
            // headingIndicatorInstrumentControl1
            // 
            this.headingIndicatorInstrumentControl1.Location = new System.Drawing.Point(242, 268);
            this.headingIndicatorInstrumentControl1.Name = "headingIndicatorInstrumentControl1";
            this.headingIndicatorInstrumentControl1.Size = new System.Drawing.Size(200, 200);
            this.headingIndicatorInstrumentControl1.TabIndex = 4;
            this.headingIndicatorInstrumentControl1.Text = "headingIndicatorInstrumentControl1";
            // 
            // horizonInstrumentControl1
            // 
            this.horizonInstrumentControl1.Location = new System.Drawing.Point(218, 12);
            this.horizonInstrumentControl1.Name = "horizonInstrumentControl1";
            this.horizonInstrumentControl1.Size = new System.Drawing.Size(250, 250);
            this.horizonInstrumentControl1.TabIndex = 3;
            this.horizonInstrumentControl1.Text = "horizonInstrumentControl1";
            this.horizonInstrumentControl1.Click += new System.EventHandler(this.horizonInstrumentControl1_Click);
            // 
            // altimeterInstrumentControl1
            // 
            this.altimeterInstrumentControl1.Location = new System.Drawing.Point(474, 12);
            this.altimeterInstrumentControl1.Name = "altimeterInstrumentControl1";
            this.altimeterInstrumentControl1.Size = new System.Drawing.Size(200, 200);
            this.altimeterInstrumentControl1.TabIndex = 2;
            this.altimeterInstrumentControl1.Text = "altimeterInstrumentControl1";
            // 
            // airSpeedInstrumentControl1
            // 
            this.airSpeedInstrumentControl1.Location = new System.Drawing.Point(12, 12);
            this.airSpeedInstrumentControl1.Name = "airSpeedInstrumentControl1";
            this.airSpeedInstrumentControl1.Size = new System.Drawing.Size(200, 200);
            this.airSpeedInstrumentControl1.TabIndex = 1;
            this.airSpeedInstrumentControl1.Text = "airSpeedInstrumentControl1";
            // 
            // turnCoordinatorInstrumentControl1
            // 
            this.turnCoordinatorInstrumentControl1.Location = new System.Drawing.Point(12, 218);
            this.turnCoordinatorInstrumentControl1.Name = "turnCoordinatorInstrumentControl1";
            this.turnCoordinatorInstrumentControl1.Size = new System.Drawing.Size(200, 200);
            this.turnCoordinatorInstrumentControl1.TabIndex = 0;
            this.turnCoordinatorInstrumentControl1.Text = "turnCoordinatorInstrumentControl1";
            // 
            // DemoWinow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(686, 712);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.verticalSpeedInstrumentControl1);
            this.Controls.Add(this.headingIndicatorInstrumentControl1);
            this.Controls.Add(this.horizonInstrumentControl1);
            this.Controls.Add(this.altimeterInstrumentControl1);
            this.Controls.Add(this.airSpeedInstrumentControl1);
            this.Controls.Add(this.turnCoordinatorInstrumentControl1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "DemoWinow";
            this.Text = "Avionics Controls Demo";
            this.Load += new System.EventHandler(this.DemoWinow_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarHeading)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarVerticalSpeed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTurnQuality)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarTurnRate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAltitude)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarRollAngle)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackPitchAngle)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarAirSpeed)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private TurnCoordinatorInstrumentControl turnCoordinatorInstrumentControl1;
        private AirSpeedIndicatorInstrumentControl airSpeedInstrumentControl1;
        private AltimeterInstrumentControl altimeterInstrumentControl1;
        private AttitudeIndicatorInstrumentControl horizonInstrumentControl1;
        private HeadingIndicatorInstrumentControl headingIndicatorInstrumentControl1;
        private VerticalSpeedIndicatorInstrumentControl verticalSpeedInstrumentControl1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TrackBar trackBarAirSpeed;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TrackBar trackBarTurnQuality;
        private System.Windows.Forms.TrackBar trackBarTurnRate;
        private System.Windows.Forms.TrackBar trackBarAltitude;
        private System.Windows.Forms.TrackBar trackBarRollAngle;
        private System.Windows.Forms.TrackBar trackPitchAngle;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TrackBar trackBarVerticalSpeed;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TrackBar trackBarHeading;


    }
}

