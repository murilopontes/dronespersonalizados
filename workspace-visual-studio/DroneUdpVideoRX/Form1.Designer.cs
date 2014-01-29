namespace DroneUdpVideoRX
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
            this.pictureBox_horizontal = new System.Windows.Forms.PictureBox();
            this.pictureBox_vertical = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.backgroundWorker_7070 = new System.ComponentModel.BackgroundWorker();
            this.backgroundWorker_6969 = new System.ComponentModel.BackgroundWorker();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel_vertical = new System.Windows.Forms.ToolStripStatusLabel();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_horizontal)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_vertical)).BeginInit();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBox_horizontal
            // 
            this.pictureBox_horizontal.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox_horizontal.Location = new System.Drawing.Point(12, 28);
            this.pictureBox_horizontal.Name = "pictureBox_horizontal";
            this.pictureBox_horizontal.Size = new System.Drawing.Size(261, 214);
            this.pictureBox_horizontal.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox_horizontal.TabIndex = 0;
            this.pictureBox_horizontal.TabStop = false;
            // 
            // pictureBox_vertical
            // 
            this.pictureBox_vertical.BackColor = System.Drawing.Color.Blue;
            this.pictureBox_vertical.Location = new System.Drawing.Point(291, 28);
            this.pictureBox_vertical.Name = "pictureBox_vertical";
            this.pictureBox_vertical.Size = new System.Drawing.Size(261, 214);
            this.pictureBox_vertical.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox_vertical.TabIndex = 1;
            this.pictureBox_vertical.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(93, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Horizontal Camera";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(291, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(81, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Vertical Camera";
            // 
            // backgroundWorker_7070
            // 
            this.backgroundWorker_7070.WorkerReportsProgress = true;
            this.backgroundWorker_7070.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_7070_DoWork);
            this.backgroundWorker_7070.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker_7070_ProgressChanged);
            // 
            // backgroundWorker_6969
            // 
            this.backgroundWorker_6969.WorkerReportsProgress = true;
            this.backgroundWorker_6969.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_6969_DoWork);
            this.backgroundWorker_6969.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker_6969_ProgressChanged);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.toolStripStatusLabel_vertical});
            this.statusStrip1.Location = new System.Drawing.Point(0, 320);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(805, 22);
            this.statusStrip1.TabIndex = 4;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(118, 17);
            this.toolStripStatusLabel1.Text = "toolStripStatusLabel1";
            // 
            // toolStripStatusLabel_vertical
            // 
            this.toolStripStatusLabel_vertical.Name = "toolStripStatusLabel_vertical";
            this.toolStripStatusLabel_vertical.Size = new System.Drawing.Size(155, 17);
            this.toolStripStatusLabel_vertical.Text = "toolStripStatusLabel_vertical";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(805, 342);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.pictureBox_vertical);
            this.Controls.Add(this.pictureBox_horizontal);
            this.Name = "Form1";
            this.Text = "DroneUdpVideoRX";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_horizontal)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_vertical)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox_horizontal;
        private System.Windows.Forms.PictureBox pictureBox_vertical;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.ComponentModel.BackgroundWorker backgroundWorker_7070;
        private System.ComponentModel.BackgroundWorker backgroundWorker_6969;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel_vertical;
    }
}

