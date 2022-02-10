namespace WinFade
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
            this.c_buttonRunStop = new System.Windows.Forms.Button();
            this.c_listBoxSerialOutput = new System.Windows.Forms.ListBox();
            this.c_textBoxProgramText = new System.Windows.Forms.TextBox();
            this.c_labelOutputLineCount = new System.Windows.Forms.Label();
            this.c_buttonDrawTest = new System.Windows.Forms.Button();
            this.c_labelErrorText = new System.Windows.Forms.Label();
            this.c_buttonSetup = new System.Windows.Forms.Button();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.c_timerProgramTextHover = new System.Windows.Forms.Timer(this.components);
            this.c_buttonReformat = new System.Windows.Forms.Button();
            this.c_buttonUpload = new System.Windows.Forms.Button();
            this.c_textBoxIPAddress = new System.Windows.Forms.TextBox();
            this.c_labelWorking = new System.Windows.Forms.Label();
            this.c_buttonRemote9 = new System.Windows.Forms.Button();
            this.c_buttonRemote8 = new System.Windows.Forms.Button();
            this.c_buttonRemote7 = new System.Windows.Forms.Button();
            this.c_buttonRemote6 = new System.Windows.Forms.Button();
            this.c_buttonRemote5 = new System.Windows.Forms.Button();
            this.c_buttonRemote4 = new System.Windows.Forms.Button();
            this.c_buttonRemote3 = new System.Windows.Forms.Button();
            this.c_buttonRemote2 = new System.Windows.Forms.Button();
            this.c_buttonRemote1 = new System.Windows.Forms.Button();
            this.c_buttonRemote0 = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.c_labelRemoteStatus = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.c_buttonNewFromRemote = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.c_checkBoxFilterToRemoteIP = new System.Windows.Forms.CheckBox();
            this.label4 = new System.Windows.Forms.Label();
            this.c_buttonUdpLog = new System.Windows.Forms.Button();
            this.c_textBoxUdpPort = new System.Windows.Forms.TextBox();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.menuStrip1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // c_buttonRunStop
            // 
            this.c_buttonRunStop.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonRunStop.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonRunStop.Location = new System.Drawing.Point(16, 29);
            this.c_buttonRunStop.Name = "c_buttonRunStop";
            this.c_buttonRunStop.Size = new System.Drawing.Size(78, 39);
            this.c_buttonRunStop.TabIndex = 0;
            this.c_buttonRunStop.Text = "Run";
            this.toolTip1.SetToolTip(this.c_buttonRunStop, "Run the local simulator");
            this.c_buttonRunStop.UseVisualStyleBackColor = true;
            this.c_buttonRunStop.Click += new System.EventHandler(this.RunProgram);
            // 
            // c_listBoxSerialOutput
            // 
            this.c_listBoxSerialOutput.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.c_listBoxSerialOutput.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_listBoxSerialOutput.FormattingEnabled = true;
            this.c_listBoxSerialOutput.HorizontalScrollbar = true;
            this.c_listBoxSerialOutput.ItemHeight = 25;
            this.c_listBoxSerialOutput.Location = new System.Drawing.Point(989, 126);
            this.c_listBoxSerialOutput.Name = "c_listBoxSerialOutput";
            this.c_listBoxSerialOutput.Size = new System.Drawing.Size(850, 354);
            this.c_listBoxSerialOutput.TabIndex = 2;
            this.toolTip1.SetToolTip(this.c_listBoxSerialOutput, "Output windows");
            // 
            // c_textBoxProgramText
            // 
            this.c_textBoxProgramText.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.c_textBoxProgramText.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_textBoxProgramText.Location = new System.Drawing.Point(47, 47);
            this.c_textBoxProgramText.Multiline = true;
            this.c_textBoxProgramText.Name = "c_textBoxProgramText";
            this.c_textBoxProgramText.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.c_textBoxProgramText.Size = new System.Drawing.Size(838, 599);
            this.c_textBoxProgramText.TabIndex = 4;
            this.c_textBoxProgramText.Text = "Error:";
            this.toolTip1.SetToolTip(this.c_textBoxProgramText, "Program text");
            this.c_textBoxProgramText.TextChanged += new System.EventHandler(this.c_textBoxProgramText_TextChanged);
            this.c_textBoxProgramText.MouseLeave += new System.EventHandler(this.c_textBoxProgramText_MouseLeave);
            this.c_textBoxProgramText.MouseMove += new System.Windows.Forms.MouseEventHandler(this.c_textBoxProgramText_MouseMove);
            // 
            // c_labelOutputLineCount
            // 
            this.c_labelOutputLineCount.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.c_labelOutputLineCount.AutoSize = true;
            this.c_labelOutputLineCount.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_labelOutputLineCount.Location = new System.Drawing.Point(1750, 495);
            this.c_labelOutputLineCount.Name = "c_labelOutputLineCount";
            this.c_labelOutputLineCount.Size = new System.Drawing.Size(23, 25);
            this.c_labelOutputLineCount.TabIndex = 7;
            this.c_labelOutputLineCount.Text = "0";
            this.toolTip1.SetToolTip(this.c_labelOutputLineCount, "The number of lines in the output box");
            // 
            // c_buttonDrawTest
            // 
            this.c_buttonDrawTest.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonDrawTest.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonDrawTest.Location = new System.Drawing.Point(914, 665);
            this.c_buttonDrawTest.Name = "c_buttonDrawTest";
            this.c_buttonDrawTest.Size = new System.Drawing.Size(26, 36);
            this.c_buttonDrawTest.TabIndex = 8;
            this.c_buttonDrawTest.Text = "Draw Test";
            this.c_buttonDrawTest.UseVisualStyleBackColor = true;
            this.c_buttonDrawTest.Visible = false;
            this.c_buttonDrawTest.Click += new System.EventHandler(this.c_buttonDrawTest_Click);
            // 
            // c_labelErrorText
            // 
            this.c_labelErrorText.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.c_labelErrorText.AutoSize = true;
            this.c_labelErrorText.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_labelErrorText.Location = new System.Drawing.Point(51, 677);
            this.c_labelErrorText.Name = "c_labelErrorText";
            this.c_labelErrorText.Size = new System.Drawing.Size(60, 25);
            this.c_labelErrorText.TabIndex = 10;
            this.c_labelErrorText.Text = "Error:";
            // 
            // c_buttonSetup
            // 
            this.c_buttonSetup.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonSetup.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonSetup.Location = new System.Drawing.Point(105, 29);
            this.c_buttonSetup.Name = "c_buttonSetup";
            this.c_buttonSetup.Size = new System.Drawing.Size(89, 39);
            this.c_buttonSetup.TabIndex = 11;
            this.c_buttonSetup.Text = "Setup";
            this.toolTip1.SetToolTip(this.c_buttonSetup, "Set up the simulator environment");
            this.c_buttonSetup.UseVisualStyleBackColor = true;
            this.c_buttonSetup.Click += new System.EventHandler(this.setupButton_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1851, 28);
            this.menuStrip1.TabIndex = 12;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(44, 24);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(135, 26);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(135, 26);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(135, 26);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(135, 26);
            this.saveAsToolStripMenuItem.Text = "Save As";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // c_timerProgramTextHover
            // 
            this.c_timerProgramTextHover.Tick += new System.EventHandler(this.c_timerProgramTextHover_Tick);
            // 
            // c_buttonReformat
            // 
            this.c_buttonReformat.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonReformat.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonReformat.Location = new System.Drawing.Point(204, 29);
            this.c_buttonReformat.Name = "c_buttonReformat";
            this.c_buttonReformat.Size = new System.Drawing.Size(103, 39);
            this.c_buttonReformat.TabIndex = 16;
            this.c_buttonReformat.Text = "Reformat";
            this.toolTip1.SetToolTip(this.c_buttonReformat, "Reformat the program text");
            this.c_buttonReformat.UseVisualStyleBackColor = true;
            this.c_buttonReformat.Click += new System.EventHandler(this.c_buttonReformat_Click);
            // 
            // c_buttonUpload
            // 
            this.c_buttonUpload.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonUpload.Location = new System.Drawing.Point(8, 116);
            this.c_buttonUpload.Name = "c_buttonUpload";
            this.c_buttonUpload.Size = new System.Drawing.Size(131, 70);
            this.c_buttonUpload.TabIndex = 13;
            this.c_buttonUpload.Text = "Save To Remote";
            this.toolTip1.SetToolTip(this.c_buttonUpload, "Copy program to the ESP");
            this.c_buttonUpload.UseVisualStyleBackColor = true;
            this.c_buttonUpload.Click += new System.EventHandler(this.c_buttonUpload_Click);
            // 
            // c_textBoxIPAddress
            // 
            this.c_textBoxIPAddress.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_textBoxIPAddress.Location = new System.Drawing.Point(140, 36);
            this.c_textBoxIPAddress.Name = "c_textBoxIPAddress";
            this.c_textBoxIPAddress.Size = new System.Drawing.Size(189, 30);
            this.c_textBoxIPAddress.TabIndex = 14;
            this.c_textBoxIPAddress.TextChanged += new System.EventHandler(this.c_textBoxIPAddress_TextChanged);
            // 
            // c_labelWorking
            // 
            this.c_labelWorking.AutoSize = true;
            this.c_labelWorking.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_labelWorking.Location = new System.Drawing.Point(382, 66);
            this.c_labelWorking.Name = "c_labelWorking";
            this.c_labelWorking.Size = new System.Drawing.Size(0, 25);
            this.c_labelWorking.TabIndex = 15;
            // 
            // c_buttonRemote9
            // 
            this.c_buttonRemote9.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonRemote9.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonRemote9.Location = new System.Drawing.Point(733, 30);
            this.c_buttonRemote9.Name = "c_buttonRemote9";
            this.c_buttonRemote9.Size = new System.Drawing.Size(37, 39);
            this.c_buttonRemote9.TabIndex = 17;
            this.c_buttonRemote9.Text = "9";
            this.toolTip1.SetToolTip(this.c_buttonRemote9, "Press Button 9 in the simulator");
            this.c_buttonRemote9.UseVisualStyleBackColor = true;
            this.c_buttonRemote9.Click += new System.EventHandler(this.c_buttonRemote0_Click);
            // 
            // c_buttonRemote8
            // 
            this.c_buttonRemote8.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonRemote8.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonRemote8.Location = new System.Drawing.Point(690, 30);
            this.c_buttonRemote8.Name = "c_buttonRemote8";
            this.c_buttonRemote8.Size = new System.Drawing.Size(37, 39);
            this.c_buttonRemote8.TabIndex = 17;
            this.c_buttonRemote8.Text = "8";
            this.toolTip1.SetToolTip(this.c_buttonRemote8, "Press Button 8 in the simulator");
            this.c_buttonRemote8.UseVisualStyleBackColor = true;
            this.c_buttonRemote8.Click += new System.EventHandler(this.c_buttonRemote0_Click);
            // 
            // c_buttonRemote7
            // 
            this.c_buttonRemote7.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonRemote7.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonRemote7.Location = new System.Drawing.Point(647, 30);
            this.c_buttonRemote7.Name = "c_buttonRemote7";
            this.c_buttonRemote7.Size = new System.Drawing.Size(37, 39);
            this.c_buttonRemote7.TabIndex = 17;
            this.c_buttonRemote7.Text = "7";
            this.toolTip1.SetToolTip(this.c_buttonRemote7, "Press Button 7 in the simulator");
            this.c_buttonRemote7.UseVisualStyleBackColor = true;
            this.c_buttonRemote7.Click += new System.EventHandler(this.c_buttonRemote0_Click);
            // 
            // c_buttonRemote6
            // 
            this.c_buttonRemote6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonRemote6.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonRemote6.Location = new System.Drawing.Point(604, 30);
            this.c_buttonRemote6.Name = "c_buttonRemote6";
            this.c_buttonRemote6.Size = new System.Drawing.Size(37, 39);
            this.c_buttonRemote6.TabIndex = 17;
            this.c_buttonRemote6.Text = "6";
            this.toolTip1.SetToolTip(this.c_buttonRemote6, "Press Button 6 in the simulator");
            this.c_buttonRemote6.UseVisualStyleBackColor = true;
            this.c_buttonRemote6.Click += new System.EventHandler(this.c_buttonRemote0_Click);
            // 
            // c_buttonRemote5
            // 
            this.c_buttonRemote5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonRemote5.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonRemote5.Location = new System.Drawing.Point(561, 30);
            this.c_buttonRemote5.Name = "c_buttonRemote5";
            this.c_buttonRemote5.Size = new System.Drawing.Size(37, 39);
            this.c_buttonRemote5.TabIndex = 17;
            this.c_buttonRemote5.Text = "5";
            this.toolTip1.SetToolTip(this.c_buttonRemote5, "Press Button 5 in the simulator");
            this.c_buttonRemote5.UseVisualStyleBackColor = true;
            this.c_buttonRemote5.Click += new System.EventHandler(this.c_buttonRemote0_Click);
            // 
            // c_buttonRemote4
            // 
            this.c_buttonRemote4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonRemote4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonRemote4.Location = new System.Drawing.Point(518, 30);
            this.c_buttonRemote4.Name = "c_buttonRemote4";
            this.c_buttonRemote4.Size = new System.Drawing.Size(37, 39);
            this.c_buttonRemote4.TabIndex = 17;
            this.c_buttonRemote4.Text = "4";
            this.toolTip1.SetToolTip(this.c_buttonRemote4, "Press Button 4 in the simulator");
            this.c_buttonRemote4.UseVisualStyleBackColor = true;
            this.c_buttonRemote4.Click += new System.EventHandler(this.c_buttonRemote0_Click);
            // 
            // c_buttonRemote3
            // 
            this.c_buttonRemote3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonRemote3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonRemote3.Location = new System.Drawing.Point(475, 30);
            this.c_buttonRemote3.Name = "c_buttonRemote3";
            this.c_buttonRemote3.Size = new System.Drawing.Size(37, 39);
            this.c_buttonRemote3.TabIndex = 17;
            this.c_buttonRemote3.Text = "3";
            this.toolTip1.SetToolTip(this.c_buttonRemote3, "Press Button 3 in the simulator");
            this.c_buttonRemote3.UseVisualStyleBackColor = true;
            this.c_buttonRemote3.Click += new System.EventHandler(this.c_buttonRemote0_Click);
            // 
            // c_buttonRemote2
            // 
            this.c_buttonRemote2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonRemote2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonRemote2.Location = new System.Drawing.Point(432, 30);
            this.c_buttonRemote2.Name = "c_buttonRemote2";
            this.c_buttonRemote2.Size = new System.Drawing.Size(37, 39);
            this.c_buttonRemote2.TabIndex = 17;
            this.c_buttonRemote2.Text = "2";
            this.toolTip1.SetToolTip(this.c_buttonRemote2, "Press Button 2 in the simulator");
            this.c_buttonRemote2.UseVisualStyleBackColor = true;
            this.c_buttonRemote2.Click += new System.EventHandler(this.c_buttonRemote0_Click);
            // 
            // c_buttonRemote1
            // 
            this.c_buttonRemote1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonRemote1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonRemote1.Location = new System.Drawing.Point(389, 30);
            this.c_buttonRemote1.Name = "c_buttonRemote1";
            this.c_buttonRemote1.Size = new System.Drawing.Size(37, 39);
            this.c_buttonRemote1.TabIndex = 17;
            this.c_buttonRemote1.Text = "1";
            this.toolTip1.SetToolTip(this.c_buttonRemote1, "Press Button 1 in the simulator");
            this.c_buttonRemote1.UseVisualStyleBackColor = true;
            this.c_buttonRemote1.Click += new System.EventHandler(this.c_buttonRemote0_Click);
            // 
            // c_buttonRemote0
            // 
            this.c_buttonRemote0.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.c_buttonRemote0.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonRemote0.Location = new System.Drawing.Point(346, 30);
            this.c_buttonRemote0.Name = "c_buttonRemote0";
            this.c_buttonRemote0.Size = new System.Drawing.Size(37, 39);
            this.c_buttonRemote0.TabIndex = 17;
            this.c_buttonRemote0.Text = "0";
            this.toolTip1.SetToolTip(this.c_buttonRemote0, "Press Button 0 in the simulator");
            this.c_buttonRemote0.UseVisualStyleBackColor = true;
            this.c_buttonRemote0.Click += new System.EventHandler(this.c_buttonRemote0_Click);
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(11, 35);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(114, 25);
            this.label3.TabIndex = 19;
            this.label3.Text = "IP Address:";
            this.toolTip1.SetToolTip(this.label3, "IP address of ESP running Fade");
            // 
            // c_labelRemoteStatus
            // 
            this.c_labelRemoteStatus.AutoSize = true;
            this.c_labelRemoteStatus.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_labelRemoteStatus.Location = new System.Drawing.Point(11, 77);
            this.c_labelRemoteStatus.Name = "c_labelRemoteStatus";
            this.c_labelRemoteStatus.Size = new System.Drawing.Size(138, 25);
            this.c_labelRemoteStatus.TabIndex = 20;
            this.c_labelRemoteStatus.Text = "Not connected";
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.c_buttonNewFromRemote);
            this.groupBox1.Controls.Add(this.groupBox2);
            this.groupBox1.Controls.Add(this.c_labelRemoteStatus);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.c_labelWorking);
            this.groupBox1.Controls.Add(this.c_textBoxIPAddress);
            this.groupBox1.Controls.Add(this.c_buttonUpload);
            this.groupBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(989, 532);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(850, 195);
            this.groupBox1.TabIndex = 19;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Remote";
            // 
            // c_buttonNewFromRemote
            // 
            this.c_buttonNewFromRemote.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonNewFromRemote.Location = new System.Drawing.Point(160, 116);
            this.c_buttonNewFromRemote.Name = "c_buttonNewFromRemote";
            this.c_buttonNewFromRemote.Size = new System.Drawing.Size(131, 70);
            this.c_buttonNewFromRemote.TabIndex = 21;
            this.c_buttonNewFromRemote.Text = "Copy From Remote";
            this.toolTip1.SetToolTip(this.c_buttonNewFromRemote, "Copy current program from ESP");
            this.c_buttonNewFromRemote.UseVisualStyleBackColor = true;
            this.c_buttonNewFromRemote.Click += new System.EventHandler(this.c_buttonNewFromRemote_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.c_checkBoxFilterToRemoteIP);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.c_buttonUdpLog);
            this.groupBox2.Controls.Add(this.c_textBoxUdpPort);
            this.groupBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F);
            this.groupBox2.Location = new System.Drawing.Point(518, 25);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(235, 162);
            this.groupBox2.TabIndex = 20;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Udp Logging";
            // 
            // c_checkBoxFilterToRemoteIP
            // 
            this.c_checkBoxFilterToRemoteIP.AutoSize = true;
            this.c_checkBoxFilterToRemoteIP.Location = new System.Drawing.Point(26, 125);
            this.c_checkBoxFilterToRemoteIP.Name = "c_checkBoxFilterToRemoteIP";
            this.c_checkBoxFilterToRemoteIP.Size = new System.Drawing.Size(192, 29);
            this.c_checkBoxFilterToRemoteIP.TabIndex = 25;
            this.c_checkBoxFilterToRemoteIP.Text = "Filter to Remote IP";
            this.toolTip1.SetToolTip(this.c_checkBoxFilterToRemoteIP, "Display only udp messages from remote IP address");
            this.c_checkBoxFilterToRemoteIP.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(21, 81);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(58, 25);
            this.label4.TabIndex = 24;
            this.label4.Text = "Port: ";
            // 
            // c_buttonUdpLog
            // 
            this.c_buttonUdpLog.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonUdpLog.Location = new System.Drawing.Point(26, 35);
            this.c_buttonUdpLog.Name = "c_buttonUdpLog";
            this.c_buttonUdpLog.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.c_buttonUdpLog.Size = new System.Drawing.Size(147, 36);
            this.c_buttonUdpLog.TabIndex = 22;
            this.c_buttonUdpLog.Text = "Start";
            this.toolTip1.SetToolTip(this.c_buttonUdpLog, "Start or stop listening for UDP logging messages");
            this.c_buttonUdpLog.UseVisualStyleBackColor = true;
            this.c_buttonUdpLog.Click += new System.EventHandler(this.c_buttonUdpLog_Click);
            // 
            // c_textBoxUdpPort
            // 
            this.c_textBoxUdpPort.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_textBoxUdpPort.Location = new System.Drawing.Point(97, 80);
            this.c_textBoxUdpPort.Name = "c_textBoxUdpPort";
            this.c_textBoxUdpPort.Size = new System.Drawing.Size(106, 30);
            this.c_textBoxUdpPort.TabIndex = 23;
            this.toolTip1.SetToolTip(this.c_textBoxUdpPort, "UDP Logging port specified for DebugPrintOverUdp");
            this.c_textBoxUdpPort.TextChanged += new System.EventHandler(this.c_textBoxUdpPort_TextChanged);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.c_buttonRemote5);
            this.groupBox3.Controls.Add(this.c_buttonRemote9);
            this.groupBox3.Controls.Add(this.c_buttonRemote8);
            this.groupBox3.Controls.Add(this.c_buttonReformat);
            this.groupBox3.Controls.Add(this.c_buttonRemote7);
            this.groupBox3.Controls.Add(this.c_buttonSetup);
            this.groupBox3.Controls.Add(this.c_buttonRemote0);
            this.groupBox3.Controls.Add(this.c_buttonRemote6);
            this.groupBox3.Controls.Add(this.c_buttonRemote4);
            this.groupBox3.Controls.Add(this.c_buttonRemote1);
            this.groupBox3.Controls.Add(this.c_buttonRemote3);
            this.groupBox3.Controls.Add(this.c_buttonRemote2);
            this.groupBox3.Controls.Add(this.c_buttonRunStop);
            this.groupBox3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F);
            this.groupBox3.Location = new System.Drawing.Point(989, 37);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(850, 74);
            this.groupBox3.TabIndex = 21;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Simulator";
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(1548, 495);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(177, 25);
            this.label1.TabIndex = 22;
            this.label1.Text = "Output Line Count:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1851, 739);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.c_labelErrorText);
            this.Controls.Add(this.c_labelOutputLineCount);
            this.Controls.Add(this.c_buttonDrawTest);
            this.Controls.Add(this.c_textBoxProgramText);
            this.Controls.Add(this.c_listBoxSerialOutput);
            this.Controls.Add(this.menuStrip1);
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "WinFade";
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Button c_buttonRunStop;
    private System.Windows.Forms.ListBox c_listBoxSerialOutput;
    private System.Windows.Forms.TextBox c_textBoxProgramText;
    private System.Windows.Forms.Label c_labelOutputLineCount;
    private System.Windows.Forms.Button c_buttonDrawTest;
    private System.Windows.Forms.Label c_labelErrorText;
        private System.Windows.Forms.Button c_buttonSetup;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.Timer c_timerProgramTextHover;
        private System.Windows.Forms.Button c_buttonReformat;
        private System.Windows.Forms.Button c_buttonUpload;
        private System.Windows.Forms.TextBox c_textBoxIPAddress;
        private System.Windows.Forms.Label c_labelWorking;
        private System.Windows.Forms.Button c_buttonRemote9;
        private System.Windows.Forms.Button c_buttonRemote8;
        private System.Windows.Forms.Button c_buttonRemote7;
        private System.Windows.Forms.Button c_buttonRemote6;
        private System.Windows.Forms.Button c_buttonRemote5;
        private System.Windows.Forms.Button c_buttonRemote4;
        private System.Windows.Forms.Button c_buttonRemote3;
        private System.Windows.Forms.Button c_buttonRemote2;
        private System.Windows.Forms.Button c_buttonRemote1;
        private System.Windows.Forms.Button c_buttonRemote0;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label c_labelRemoteStatus;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button c_buttonNewFromRemote;
        private System.Windows.Forms.Button c_buttonUdpLog;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox c_textBoxUdpPort;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.CheckBox c_checkBoxFilterToRemoteIP;
        private System.Windows.Forms.Label label1;
    }
}

