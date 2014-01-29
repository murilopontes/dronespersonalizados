namespace murix_utils
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
            this.progressBar_RSSI = new System.Windows.Forms.ProgressBar();
            this.progressBar_ping = new System.Windows.Forms.ProgressBar();
            this.timer_rssi = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // progressBar_RSSI
            // 
            this.progressBar_RSSI.Location = new System.Drawing.Point(25, 28);
            this.progressBar_RSSI.Name = "progressBar_RSSI";
            this.progressBar_RSSI.Size = new System.Drawing.Size(100, 23);
            this.progressBar_RSSI.TabIndex = 10;
            // 
            // progressBar_ping
            // 
            this.progressBar_ping.Location = new System.Drawing.Point(25, 57);
            this.progressBar_ping.Name = "progressBar_ping";
            this.progressBar_ping.Size = new System.Drawing.Size(100, 23);
            this.progressBar_ping.TabIndex = 11;
            // 
            // timer_rssi
            // 
            this.timer_rssi.Tick += new System.EventHandler(this.timer_rssi_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(845, 578);
            this.Controls.Add(this.progressBar_ping);
            this.Controls.Add(this.progressBar_RSSI);
            this.Name = "Form1";
            this.Text = "OpenFlight Gamepad";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ProgressBar progressBar_RSSI;
        private System.Windows.Forms.ProgressBar progressBar_ping;
        private System.Windows.Forms.Timer timer_rssi;
    }
}

