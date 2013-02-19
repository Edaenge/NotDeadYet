namespace GUI
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.map_combobox = new System.Windows.Forms.ComboBox();
            this.groupBox_gamemode = new System.Windows.Forms.GroupBox();
            this.FFARadioButton = new System.Windows.Forms.RadioButton();
            this.TestGameModeRadioButton = new System.Windows.Forms.RadioButton();
            this.label_maps = new System.Windows.Forms.Label();
            this.button_copy = new System.Windows.Forms.Button();
            this.label_IP_address = new System.Windows.Forms.Label();
            this.textBox_public_ip = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.button_Shutdown = new System.Windows.Forms.Button();
            this.label_Status = new System.Windows.Forms.Label();
            this.button_Start = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.textBox_Port = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.textBox_Players = new System.Windows.Forms.TextBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.panel2 = new System.Windows.Forms.Panel();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel3 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel4 = new System.Windows.Forms.ToolStripStatusLabel();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.fileSystemWatcher1 = new System.IO.FileSystemWatcher();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.groupBox_gamemode.SuspendLayout();
            this.panel1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.panel2.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fileSystemWatcher1)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(12, 12);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(322, 425);
            this.tabControl1.TabIndex = 5;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.Color.Silver;
            this.tabPage1.Controls.Add(this.map_combobox);
            this.tabPage1.Controls.Add(this.groupBox_gamemode);
            this.tabPage1.Controls.Add(this.label_maps);
            this.tabPage1.Controls.Add(this.button_copy);
            this.tabPage1.Controls.Add(this.label_IP_address);
            this.tabPage1.Controls.Add(this.textBox_public_ip);
            this.tabPage1.Controls.Add(this.panel1);
            this.tabPage1.ForeColor = System.Drawing.SystemColors.ControlText;
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPage1.Size = new System.Drawing.Size(314, 399);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Setup";
            // 
            // map_combobox
            // 
            this.map_combobox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.map_combobox.FormattingEnabled = true;
            this.map_combobox.Location = new System.Drawing.Point(213, 298);
            this.map_combobox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.map_combobox.Name = "map_combobox";
            this.map_combobox.Size = new System.Drawing.Size(92, 21);
            this.map_combobox.TabIndex = 23;
            // 
            // groupBox_gamemode
            // 
            this.groupBox_gamemode.Controls.Add(this.FFARadioButton);
            this.groupBox_gamemode.Controls.Add(this.TestGameModeRadioButton);
            this.groupBox_gamemode.Location = new System.Drawing.Point(50, 282);
            this.groupBox_gamemode.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox_gamemode.Name = "groupBox_gamemode";
            this.groupBox_gamemode.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox_gamemode.Size = new System.Drawing.Size(110, 64);
            this.groupBox_gamemode.TabIndex = 22;
            this.groupBox_gamemode.TabStop = false;
            this.groupBox_gamemode.Text = "Gamemode";
            // 
            // FFARadioButton
            // 
            this.FFARadioButton.AutoSize = true;
            this.FFARadioButton.Checked = true;
            this.FFARadioButton.Dock = System.Windows.Forms.DockStyle.Top;
            this.FFARadioButton.Location = new System.Drawing.Point(2, 32);
            this.FFARadioButton.Name = "FFARadioButton";
            this.FFARadioButton.Size = new System.Drawing.Size(106, 17);
            this.FFARadioButton.TabIndex = 18;
            this.FFARadioButton.TabStop = true;
            this.FFARadioButton.Text = "FFA";
            this.FFARadioButton.UseVisualStyleBackColor = true;
            // 
            // TestGameModeRadioButton
            // 
            this.TestGameModeRadioButton.AutoSize = true;
            this.TestGameModeRadioButton.Dock = System.Windows.Forms.DockStyle.Top;
            this.TestGameModeRadioButton.Location = new System.Drawing.Point(2, 15);
            this.TestGameModeRadioButton.Name = "TestGameModeRadioButton";
            this.TestGameModeRadioButton.Size = new System.Drawing.Size(106, 17);
            this.TestGameModeRadioButton.TabIndex = 19;
            this.TestGameModeRadioButton.Text = "TestMode";
            this.TestGameModeRadioButton.UseVisualStyleBackColor = true;
            // 
            // label_maps
            // 
            this.label_maps.AutoSize = true;
            this.label_maps.Location = new System.Drawing.Point(185, 301);
            this.label_maps.Name = "label_maps";
            this.label_maps.Size = new System.Drawing.Size(31, 13);
            this.label_maps.TabIndex = 21;
            this.label_maps.Text = "Map:";
            // 
            // button_copy
            // 
            this.button_copy.Location = new System.Drawing.Point(238, 250);
            this.button_copy.Name = "button_copy";
            this.button_copy.Size = new System.Drawing.Size(42, 23);
            this.button_copy.TabIndex = 16;
            this.button_copy.Text = "Copy";
            this.button_copy.UseVisualStyleBackColor = true;
            this.button_copy.MouseClick += new System.Windows.Forms.MouseEventHandler(this.button_copy_MouseClick);
            // 
            // label_IP_address
            // 
            this.label_IP_address.AutoSize = true;
            this.label_IP_address.Location = new System.Drawing.Point(47, 253);
            this.label_IP_address.Name = "label_IP_address";
            this.label_IP_address.Size = new System.Drawing.Size(54, 13);
            this.label_IP_address.TabIndex = 15;
            this.label_IP_address.Text = "Server IP:";
            // 
            // textBox_public_ip
            // 
            this.textBox_public_ip.Location = new System.Drawing.Point(106, 250);
            this.textBox_public_ip.Name = "textBox_public_ip";
            this.textBox_public_ip.ReadOnly = true;
            this.textBox_public_ip.Size = new System.Drawing.Size(126, 20);
            this.textBox_public_ip.TabIndex = 14;
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1.Controls.Add(this.button_Shutdown);
            this.panel1.Controls.Add(this.label_Status);
            this.panel1.Controls.Add(this.button_Start);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.textBox_Port);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.textBox_Players);
            this.panel1.Location = new System.Drawing.Point(50, 20);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(208, 202);
            this.panel1.TabIndex = 8;
            // 
            // button_Shutdown
            // 
            this.button_Shutdown.Enabled = false;
            this.button_Shutdown.Location = new System.Drawing.Point(79, 144);
            this.button_Shutdown.Name = "button_Shutdown";
            this.button_Shutdown.Size = new System.Drawing.Size(75, 23);
            this.button_Shutdown.TabIndex = 13;
            this.button_Shutdown.Text = "Shutdown";
            this.button_Shutdown.UseVisualStyleBackColor = true;
            this.button_Shutdown.EnabledChanged += new System.EventHandler(this.button_Shutdown_EnabledChanged);
            this.button_Shutdown.Click += new System.EventHandler(this.button_Shutdown_Click);
            // 
            // label_Status
            // 
            this.label_Status.AutoSize = true;
            this.label_Status.Location = new System.Drawing.Point(3, 173);
            this.label_Status.Name = "label_Status";
            this.label_Status.Size = new System.Drawing.Size(0, 13);
            this.label_Status.TabIndex = 9;
            // 
            // button_Start
            // 
            this.button_Start.Location = new System.Drawing.Point(79, 115);
            this.button_Start.Name = "button_Start";
            this.button_Start.Size = new System.Drawing.Size(75, 23);
            this.button_Start.TabIndex = 12;
            this.button_Start.Text = "Start";
            this.button_Start.UseVisualStyleBackColor = true;
            this.button_Start.EnabledChanged += new System.EventHandler(this.button_Start_EnabledChanged);
            this.button_Start.MouseClick += new System.Windows.Forms.MouseEventHandler(this.button_Start_MouseClick);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(21, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 13);
            this.label2.TabIndex = 11;
            this.label2.Text = "TCP Port:";
            // 
            // textBox_Port
            // 
            this.textBox_Port.Location = new System.Drawing.Point(80, 53);
            this.textBox_Port.MaxLength = 6;
            this.textBox_Port.Name = "textBox_Port";
            this.textBox_Port.Size = new System.Drawing.Size(100, 20);
            this.textBox_Port.TabIndex = 10;
            this.textBox_Port.Text = "11521";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(20, 30);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(44, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "Players:";
            // 
            // textBox_Players
            // 
            this.textBox_Players.Location = new System.Drawing.Point(80, 27);
            this.textBox_Players.MaxLength = 2;
            this.textBox_Players.Name = "textBox_Players";
            this.textBox_Players.Size = new System.Drawing.Size(100, 20);
            this.textBox_Players.TabIndex = 8;
            this.textBox_Players.Text = "10";
            // 
            // tabPage2
            // 
            this.tabPage2.BackColor = System.Drawing.Color.Silver;
            this.tabPage2.Controls.Add(this.panel2);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPage2.Size = new System.Drawing.Size(314, 399);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Console";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.richTextBox1);
            this.panel2.Location = new System.Drawing.Point(6, 6);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(302, 387);
            this.panel2.TabIndex = 0;
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(3, 3);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.ReadOnly = true;
            this.richTextBox1.Size = new System.Drawing.Size(296, 356);
            this.richTextBox1.TabIndex = 0;
            this.richTextBox1.Text = "";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.toolStripStatusLabel2,
            this.toolStripStatusLabel3,
            this.toolStripStatusLabel4});
            this.statusStrip1.Location = new System.Drawing.Point(0, 427);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(346, 22);
            this.statusStrip1.TabIndex = 6;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(47, 17);
            this.toolStripStatusLabel1.Text = "Players:";
            // 
            // toolStripStatusLabel2
            // 
            this.toolStripStatusLabel2.Name = "toolStripStatusLabel2";
            this.toolStripStatusLabel2.Size = new System.Drawing.Size(24, 17);
            this.toolStripStatusLabel2.Text = "0/0";
            // 
            // toolStripStatusLabel3
            // 
            this.toolStripStatusLabel3.Name = "toolStripStatusLabel3";
            this.toolStripStatusLabel3.Size = new System.Drawing.Size(42, 17);
            this.toolStripStatusLabel3.Text = "Status:";
            // 
            // toolStripStatusLabel4
            // 
            this.toolStripStatusLabel4.Name = "toolStripStatusLabel4";
            this.toolStripStatusLabel4.Size = new System.Drawing.Size(75, 17);
            this.toolStripStatusLabel4.Text = "Not Running";
            // 
            // backgroundWorker1
            // 
            this.backgroundWorker1.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker1_DoWork);
            // 
            // fileSystemWatcher1
            // 
            this.fileSystemWatcher1.EnableRaisingEvents = true;
            this.fileSystemWatcher1.SynchronizingObject = this;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(346, 449);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.tabControl1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "NotDeadYet - Dedicated Server";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.groupBox_gamemode.ResumeLayout(false);
            this.groupBox_gamemode.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fileSystemWatcher1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBox_Port;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBox_Players;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button button_Start;
        private System.Windows.Forms.Label label_Status;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.Button button_Shutdown;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel2;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel3;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel4;
        private System.Windows.Forms.Label label_IP_address;
        private System.Windows.Forms.TextBox textBox_public_ip;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.Button button_copy;
        private System.Windows.Forms.RadioButton TestGameModeRadioButton;
        private System.Windows.Forms.RadioButton FFARadioButton;
        private System.Windows.Forms.Label label_maps;
        private System.Windows.Forms.ComboBox map_combobox;
        private System.Windows.Forms.GroupBox groupBox_gamemode;
        private System.IO.FileSystemWatcher fileSystemWatcher1;

    }
}

