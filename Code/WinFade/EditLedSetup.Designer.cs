using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace WinFade
{
    partial class EditLedSetup
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private IContainer components = null;

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
      this.c_listBoxLedGroups = new System.Windows.Forms.ListBox();
      this.label1 = new System.Windows.Forms.Label();
      this.label3 = new System.Windows.Forms.Label();
      this.c_textBoxGroupNumber = new System.Windows.Forms.TextBox();
      this.c_textBoxLedCount = new System.Windows.Forms.TextBox();
      this.c_buttonOK = new System.Windows.Forms.Button();
      this.c_textBoxX = new System.Windows.Forms.TextBox();
      this.label4 = new System.Windows.Forms.Label();
      this.c_textBoxY = new System.Windows.Forms.TextBox();
      this.label5 = new System.Windows.Forms.Label();
      this.groupBox1 = new System.Windows.Forms.GroupBox();
      this.groupBox2 = new System.Windows.Forms.GroupBox();
      this.c_textBoxServoMaxAngle = new System.Windows.Forms.TextBox();
      this.c_textBoxServoMinAngle = new System.Windows.Forms.TextBox();
      this.label14 = new System.Windows.Forms.Label();
      this.label15 = new System.Windows.Forms.Label();
      this.label16 = new System.Windows.Forms.Label();
      this.c_radioButtonServo = new System.Windows.Forms.RadioButton();
      this.c_textBoxStripSpacing = new System.Windows.Forms.TextBox();
      this.label13 = new System.Windows.Forms.Label();
      this.c_textBoxPwmColorCoding = new System.Windows.Forms.TextBox();
      this.label12 = new System.Windows.Forms.Label();
      this.c_radioButtonPwm = new System.Windows.Forms.RadioButton();
      this.c_textBoxMatrixYCount = new System.Windows.Forms.TextBox();
      this.c_textBoxMatrixYSpacing = new System.Windows.Forms.TextBox();
      this.c_textBoxMatrixXSpacing = new System.Windows.Forms.TextBox();
      this.c_textBoxMatrixXCount = new System.Windows.Forms.TextBox();
      this.label11 = new System.Windows.Forms.Label();
      this.label10 = new System.Windows.Forms.Label();
      this.label9 = new System.Windows.Forms.Label();
      this.label8 = new System.Windows.Forms.Label();
      this.c_radioButtonMatrix = new System.Windows.Forms.RadioButton();
      this.c_checkBoxReversed = new System.Windows.Forms.CheckBox();
      this.c_textBoxRingRadius = new System.Windows.Forms.TextBox();
      this.label7 = new System.Windows.Forms.Label();
      this.c_radioButtonRing = new System.Windows.Forms.RadioButton();
      this.c_buttonCustomPaste = new System.Windows.Forms.Button();
      this.c_radioButtonCustom = new System.Windows.Forms.RadioButton();
      this.c_radioButtonStrip = new System.Windows.Forms.RadioButton();
      this.c_textBoxLedSize = new System.Windows.Forms.TextBox();
      this.label6 = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.c_buttonAdd = new System.Windows.Forms.Button();
      this.c_buttonDelete = new System.Windows.Forms.Button();
      this.c_buttonUp = new System.Windows.Forms.Button();
      this.c_buttonDown = new System.Windows.Forms.Button();
      this.groupBox1.SuspendLayout();
      this.groupBox2.SuspendLayout();
      this.SuspendLayout();
      // 
      // c_listBoxLedGroups
      // 
      this.c_listBoxLedGroups.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_listBoxLedGroups.FormattingEnabled = true;
      this.c_listBoxLedGroups.ItemHeight = 25;
      this.c_listBoxLedGroups.Location = new System.Drawing.Point(29, 78);
      this.c_listBoxLedGroups.Name = "c_listBoxLedGroups";
      this.c_listBoxLedGroups.Size = new System.Drawing.Size(318, 329);
      this.c_listBoxLedGroups.TabIndex = 0;
      this.c_listBoxLedGroups.SelectedIndexChanged += new System.EventHandler(this.c_listBoxLedGroups_SelectedIndexChanged);
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label1.Location = new System.Drawing.Point(521, 38);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(146, 25);
      this.label1.TabIndex = 1;
      this.label1.Text = "Group Number:";
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label3.Location = new System.Drawing.Point(522, 78);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(109, 25);
      this.label3.TabIndex = 3;
      this.label3.Text = "Led Count:";
      // 
      // c_textBoxGroupNumber
      // 
      this.c_textBoxGroupNumber.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxGroupNumber.Location = new System.Drawing.Point(678, 36);
      this.c_textBoxGroupNumber.Name = "c_textBoxGroupNumber";
      this.c_textBoxGroupNumber.Size = new System.Drawing.Size(94, 30);
      this.c_textBoxGroupNumber.TabIndex = 4;
      this.c_textBoxGroupNumber.Leave += new System.EventHandler(this.MainValuesChanged);
      // 
      // c_textBoxLedCount
      // 
      this.c_textBoxLedCount.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxLedCount.Location = new System.Drawing.Point(677, 78);
      this.c_textBoxLedCount.Name = "c_textBoxLedCount";
      this.c_textBoxLedCount.Size = new System.Drawing.Size(94, 30);
      this.c_textBoxLedCount.TabIndex = 5;
      this.c_textBoxLedCount.Leave += new System.EventHandler(this.MainValuesChanged);
      // 
      // c_buttonOK
      // 
      this.c_buttonOK.DialogResult = System.Windows.Forms.DialogResult.OK;
      this.c_buttonOK.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_buttonOK.Location = new System.Drawing.Point(1045, 30);
      this.c_buttonOK.Name = "c_buttonOK";
      this.c_buttonOK.Size = new System.Drawing.Size(98, 41);
      this.c_buttonOK.TabIndex = 7;
      this.c_buttonOK.Text = "OK";
      this.c_buttonOK.UseVisualStyleBackColor = true;
      // 
      // c_textBoxX
      // 
      this.c_textBoxX.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxX.Location = new System.Drawing.Point(96, 36);
      this.c_textBoxX.Name = "c_textBoxX";
      this.c_textBoxX.Size = new System.Drawing.Size(94, 30);
      this.c_textBoxX.TabIndex = 10;
      this.c_textBoxX.Leave += new System.EventHandler(this.MainValuesChanged);
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label4.Location = new System.Drawing.Point(30, 39);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size(32, 25);
      this.label4.TabIndex = 9;
      this.label4.Text = "X:";
      // 
      // c_textBoxY
      // 
      this.c_textBoxY.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxY.Location = new System.Drawing.Point(96, 77);
      this.c_textBoxY.Name = "c_textBoxY";
      this.c_textBoxY.Size = new System.Drawing.Size(94, 30);
      this.c_textBoxY.TabIndex = 12;
      this.c_textBoxY.Leave += new System.EventHandler(this.MainValuesChanged);
      // 
      // label5
      // 
      this.label5.AutoSize = true;
      this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label5.Location = new System.Drawing.Point(31, 77);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size(31, 25);
      this.label5.TabIndex = 11;
      this.label5.Text = "Y:";
      // 
      // groupBox1
      // 
      this.groupBox1.Controls.Add(this.label4);
      this.groupBox1.Controls.Add(this.label5);
      this.groupBox1.Controls.Add(this.c_textBoxY);
      this.groupBox1.Controls.Add(this.c_textBoxX);
      this.groupBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.groupBox1.Location = new System.Drawing.Point(527, 157);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new System.Drawing.Size(210, 124);
      this.groupBox1.TabIndex = 13;
      this.groupBox1.TabStop = false;
      this.groupBox1.Text = "Starting Position";
      // 
      // groupBox2
      // 
      this.groupBox2.Controls.Add(this.c_textBoxServoMaxAngle);
      this.groupBox2.Controls.Add(this.c_textBoxServoMinAngle);
      this.groupBox2.Controls.Add(this.label14);
      this.groupBox2.Controls.Add(this.label15);
      this.groupBox2.Controls.Add(this.label16);
      this.groupBox2.Controls.Add(this.c_radioButtonServo);
      this.groupBox2.Controls.Add(this.c_textBoxStripSpacing);
      this.groupBox2.Controls.Add(this.label13);
      this.groupBox2.Controls.Add(this.c_textBoxPwmColorCoding);
      this.groupBox2.Controls.Add(this.label12);
      this.groupBox2.Controls.Add(this.c_radioButtonPwm);
      this.groupBox2.Controls.Add(this.c_textBoxMatrixYCount);
      this.groupBox2.Controls.Add(this.c_textBoxMatrixYSpacing);
      this.groupBox2.Controls.Add(this.c_textBoxMatrixXSpacing);
      this.groupBox2.Controls.Add(this.c_textBoxMatrixXCount);
      this.groupBox2.Controls.Add(this.label11);
      this.groupBox2.Controls.Add(this.label10);
      this.groupBox2.Controls.Add(this.label9);
      this.groupBox2.Controls.Add(this.label8);
      this.groupBox2.Controls.Add(this.c_radioButtonMatrix);
      this.groupBox2.Controls.Add(this.c_checkBoxReversed);
      this.groupBox2.Controls.Add(this.c_textBoxRingRadius);
      this.groupBox2.Controls.Add(this.label7);
      this.groupBox2.Controls.Add(this.c_radioButtonRing);
      this.groupBox2.Controls.Add(this.c_buttonCustomPaste);
      this.groupBox2.Controls.Add(this.c_radioButtonCustom);
      this.groupBox2.Controls.Add(this.c_radioButtonStrip);
      this.groupBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.groupBox2.Location = new System.Drawing.Point(519, 311);
      this.groupBox2.Name = "groupBox2";
      this.groupBox2.Size = new System.Drawing.Size(541, 426);
      this.groupBox2.TabIndex = 14;
      this.groupBox2.TabStop = false;
      this.groupBox2.Text = "Group Type";
      // 
      // c_textBoxServoMaxAngle
      // 
      this.c_textBoxServoMaxAngle.Enabled = false;
      this.c_textBoxServoMaxAngle.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxServoMaxAngle.Location = new System.Drawing.Point(243, 346);
      this.c_textBoxServoMaxAngle.Name = "c_textBoxServoMaxAngle";
      this.c_textBoxServoMaxAngle.Size = new System.Drawing.Size(65, 30);
      this.c_textBoxServoMaxAngle.TabIndex = 39;
      this.c_textBoxServoMaxAngle.Leave += new System.EventHandler(this.ServoAnglesChanged);
      // 
      // c_textBoxServoMinAngle
      // 
      this.c_textBoxServoMinAngle.Enabled = false;
      this.c_textBoxServoMinAngle.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxServoMinAngle.Location = new System.Drawing.Point(153, 346);
      this.c_textBoxServoMinAngle.Name = "c_textBoxServoMinAngle";
      this.c_textBoxServoMinAngle.Size = new System.Drawing.Size(65, 30);
      this.c_textBoxServoMinAngle.TabIndex = 38;
      this.c_textBoxServoMinAngle.Leave += new System.EventHandler(this.ServoAnglesChanged);
      // 
      // label14
      // 
      this.label14.AutoSize = true;
      this.label14.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label14.Location = new System.Drawing.Point(69, 346);
      this.label14.Name = "label14";
      this.label14.Size = new System.Drawing.Size(69, 25);
      this.label14.TabIndex = 37;
      this.label14.Text = "Angle:";
      this.label14.Click += new System.EventHandler(this.label14_Click);
      // 
      // label15
      // 
      this.label15.AutoSize = true;
      this.label15.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label15.Location = new System.Drawing.Point(246, 314);
      this.label15.Name = "label15";
      this.label15.Size = new System.Drawing.Size(56, 25);
      this.label15.TabIndex = 36;
      this.label15.Text = "Max:";
      this.label15.Click += new System.EventHandler(this.label15_Click);
      // 
      // label16
      // 
      this.label16.AutoSize = true;
      this.label16.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label16.Location = new System.Drawing.Point(161, 314);
      this.label16.Name = "label16";
      this.label16.Size = new System.Drawing.Size(50, 25);
      this.label16.TabIndex = 35;
      this.label16.Text = "Min:";
      this.label16.Click += new System.EventHandler(this.label16_Click);
      // 
      // c_radioButtonServo
      // 
      this.c_radioButtonServo.AutoSize = true;
      this.c_radioButtonServo.Location = new System.Drawing.Point(24, 312);
      this.c_radioButtonServo.Name = "c_radioButtonServo";
      this.c_radioButtonServo.Size = new System.Drawing.Size(85, 29);
      this.c_radioButtonServo.TabIndex = 34;
      this.c_radioButtonServo.TabStop = true;
      this.c_radioButtonServo.Text = "Servo";
      this.c_radioButtonServo.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      this.c_radioButtonServo.UseVisualStyleBackColor = true;
      this.c_radioButtonServo.Click += new System.EventHandler(this.c_radioButtonServo_Click);
      // 
      // c_textBoxStripSpacing
      // 
      this.c_textBoxStripSpacing.Enabled = false;
      this.c_textBoxStripSpacing.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxStripSpacing.Location = new System.Drawing.Point(221, 34);
      this.c_textBoxStripSpacing.Name = "c_textBoxStripSpacing";
      this.c_textBoxStripSpacing.Size = new System.Drawing.Size(94, 30);
      this.c_textBoxStripSpacing.TabIndex = 33;
      this.c_textBoxStripSpacing.TextChanged += new System.EventHandler(this.c_textBoxStripSpacing_TextChanged);
      // 
      // label13
      // 
      this.label13.AutoSize = true;
      this.label13.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label13.Location = new System.Drawing.Point(112, 36);
      this.label13.Name = "label13";
      this.label13.Size = new System.Drawing.Size(90, 25);
      this.label13.TabIndex = 32;
      this.label13.Text = "Spacing:";
      // 
      // c_textBoxPwmColorCoding
      // 
      this.c_textBoxPwmColorCoding.Enabled = false;
      this.c_textBoxPwmColorCoding.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxPwmColorCoding.Location = new System.Drawing.Point(251, 224);
      this.c_textBoxPwmColorCoding.Name = "c_textBoxPwmColorCoding";
      this.c_textBoxPwmColorCoding.Size = new System.Drawing.Size(94, 30);
      this.c_textBoxPwmColorCoding.TabIndex = 31;
      this.c_textBoxPwmColorCoding.Leave += new System.EventHandler(this.c_textBoxPwmColorCoding_Leave);
      // 
      // label12
      // 
      this.label12.AutoSize = true;
      this.label12.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label12.Location = new System.Drawing.Point(112, 224);
      this.label12.Name = "label12";
      this.label12.Size = new System.Drawing.Size(133, 25);
      this.label12.TabIndex = 30;
      this.label12.Text = "Color coding: ";
      // 
      // c_radioButtonPwm
      // 
      this.c_radioButtonPwm.AutoSize = true;
      this.c_radioButtonPwm.Location = new System.Drawing.Point(24, 223);
      this.c_radioButtonPwm.Name = "c_radioButtonPwm";
      this.c_radioButtonPwm.Size = new System.Drawing.Size(83, 29);
      this.c_radioButtonPwm.TabIndex = 29;
      this.c_radioButtonPwm.TabStop = true;
      this.c_radioButtonPwm.Text = "PWM";
      this.c_radioButtonPwm.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      this.c_radioButtonPwm.UseVisualStyleBackColor = true;
      this.c_radioButtonPwm.Click += new System.EventHandler(this.c_radioButtonPwm_Click);
      // 
      // c_textBoxMatrixYCount
      // 
      this.c_textBoxMatrixYCount.Enabled = false;
      this.c_textBoxMatrixYCount.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxMatrixYCount.Location = new System.Drawing.Point(145, 183);
      this.c_textBoxMatrixYCount.Name = "c_textBoxMatrixYCount";
      this.c_textBoxMatrixYCount.Size = new System.Drawing.Size(65, 30);
      this.c_textBoxMatrixYCount.TabIndex = 28;
      this.c_textBoxMatrixYCount.Leave += new System.EventHandler(this.c_textBoxMatrixControl_Changed);
      // 
      // c_textBoxMatrixYSpacing
      // 
      this.c_textBoxMatrixYSpacing.Enabled = false;
      this.c_textBoxMatrixYSpacing.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxMatrixYSpacing.Location = new System.Drawing.Point(238, 182);
      this.c_textBoxMatrixYSpacing.Name = "c_textBoxMatrixYSpacing";
      this.c_textBoxMatrixYSpacing.Size = new System.Drawing.Size(65, 30);
      this.c_textBoxMatrixYSpacing.TabIndex = 27;
      this.c_textBoxMatrixYSpacing.Leave += new System.EventHandler(this.c_textBoxMatrixControl_Changed);
      // 
      // c_textBoxMatrixXSpacing
      // 
      this.c_textBoxMatrixXSpacing.Enabled = false;
      this.c_textBoxMatrixXSpacing.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxMatrixXSpacing.Location = new System.Drawing.Point(238, 145);
      this.c_textBoxMatrixXSpacing.Name = "c_textBoxMatrixXSpacing";
      this.c_textBoxMatrixXSpacing.Size = new System.Drawing.Size(65, 30);
      this.c_textBoxMatrixXSpacing.TabIndex = 26;
      this.c_textBoxMatrixXSpacing.Leave += new System.EventHandler(this.c_textBoxMatrixControl_Changed);
      // 
      // c_textBoxMatrixXCount
      // 
      this.c_textBoxMatrixXCount.Enabled = false;
      this.c_textBoxMatrixXCount.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxMatrixXCount.Location = new System.Drawing.Point(145, 145);
      this.c_textBoxMatrixXCount.Name = "c_textBoxMatrixXCount";
      this.c_textBoxMatrixXCount.Size = new System.Drawing.Size(65, 30);
      this.c_textBoxMatrixXCount.TabIndex = 25;
      this.c_textBoxMatrixXCount.Leave += new System.EventHandler(this.c_textBoxMatrixControl_Changed);
      // 
      // label11
      // 
      this.label11.AutoSize = true;
      this.label11.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label11.Location = new System.Drawing.Point(82, 183);
      this.label11.Name = "label11";
      this.label11.Size = new System.Drawing.Size(31, 25);
      this.label11.TabIndex = 24;
      this.label11.Text = "Y:";
      // 
      // label10
      // 
      this.label10.AutoSize = true;
      this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label10.Location = new System.Drawing.Point(82, 145);
      this.label10.Name = "label10";
      this.label10.Size = new System.Drawing.Size(32, 25);
      this.label10.TabIndex = 23;
      this.label10.Text = "X:";
      // 
      // label9
      // 
      this.label9.AutoSize = true;
      this.label9.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label9.Location = new System.Drawing.Point(231, 113);
      this.label9.Name = "label9";
      this.label9.Size = new System.Drawing.Size(90, 25);
      this.label9.TabIndex = 22;
      this.label9.Text = "Spacing:";
      // 
      // label8
      // 
      this.label8.AutoSize = true;
      this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label8.Location = new System.Drawing.Point(140, 115);
      this.label8.Name = "label8";
      this.label8.Size = new System.Drawing.Size(71, 25);
      this.label8.TabIndex = 21;
      this.label8.Text = "Count:";
      // 
      // c_radioButtonMatrix
      // 
      this.c_radioButtonMatrix.AutoSize = true;
      this.c_radioButtonMatrix.Location = new System.Drawing.Point(24, 113);
      this.c_radioButtonMatrix.Name = "c_radioButtonMatrix";
      this.c_radioButtonMatrix.Size = new System.Drawing.Size(86, 29);
      this.c_radioButtonMatrix.TabIndex = 20;
      this.c_radioButtonMatrix.TabStop = true;
      this.c_radioButtonMatrix.Text = "Matrix";
      this.c_radioButtonMatrix.UseVisualStyleBackColor = true;
      this.c_radioButtonMatrix.Click += new System.EventHandler(this.c_radioButtonMatrix_Click);
      // 
      // c_checkBoxReversed
      // 
      this.c_checkBoxReversed.AutoSize = true;
      this.c_checkBoxReversed.Enabled = false;
      this.c_checkBoxReversed.Location = new System.Drawing.Point(350, 75);
      this.c_checkBoxReversed.Name = "c_checkBoxReversed";
      this.c_checkBoxReversed.Size = new System.Drawing.Size(117, 29);
      this.c_checkBoxReversed.TabIndex = 19;
      this.c_checkBoxReversed.Text = "Reversed";
      this.c_checkBoxReversed.UseVisualStyleBackColor = true;
      this.c_checkBoxReversed.Click += new System.EventHandler(this.RingValuesChanged);
      // 
      // c_textBoxRingRadius
      // 
      this.c_textBoxRingRadius.Enabled = false;
      this.c_textBoxRingRadius.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxRingRadius.Location = new System.Drawing.Point(221, 72);
      this.c_textBoxRingRadius.Name = "c_textBoxRingRadius";
      this.c_textBoxRingRadius.Size = new System.Drawing.Size(94, 30);
      this.c_textBoxRingRadius.TabIndex = 18;
      this.c_textBoxRingRadius.TextChanged += new System.EventHandler(this.RingValuesChanged);
      // 
      // label7
      // 
      this.label7.AutoSize = true;
      this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label7.Location = new System.Drawing.Point(112, 75);
      this.label7.Name = "label7";
      this.label7.Size = new System.Drawing.Size(78, 25);
      this.label7.TabIndex = 17;
      this.label7.Text = "Radius:";
      // 
      // c_radioButtonRing
      // 
      this.c_radioButtonRing.AutoSize = true;
      this.c_radioButtonRing.Location = new System.Drawing.Point(24, 71);
      this.c_radioButtonRing.Name = "c_radioButtonRing";
      this.c_radioButtonRing.Size = new System.Drawing.Size(72, 29);
      this.c_radioButtonRing.TabIndex = 3;
      this.c_radioButtonRing.TabStop = true;
      this.c_radioButtonRing.Text = "Ring";
      this.c_radioButtonRing.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
      this.c_radioButtonRing.UseVisualStyleBackColor = true;
      this.c_radioButtonRing.Click += new System.EventHandler(this.c_radioButtonRing_Click);
      // 
      // c_buttonCustomPaste
      // 
      this.c_buttonCustomPaste.Enabled = false;
      this.c_buttonCustomPaste.Location = new System.Drawing.Point(130, 259);
      this.c_buttonCustomPaste.Name = "c_buttonCustomPaste";
      this.c_buttonCustomPaste.Size = new System.Drawing.Size(105, 39);
      this.c_buttonCustomPaste.TabIndex = 2;
      this.c_buttonCustomPaste.Text = "Paste";
      this.c_buttonCustomPaste.UseVisualStyleBackColor = true;
      this.c_buttonCustomPaste.Click += new System.EventHandler(this.c_buttonCustomPaste_Click);
      // 
      // c_radioButtonCustom
      // 
      this.c_radioButtonCustom.AutoSize = true;
      this.c_radioButtonCustom.Location = new System.Drawing.Point(23, 264);
      this.c_radioButtonCustom.Name = "c_radioButtonCustom";
      this.c_radioButtonCustom.Size = new System.Drawing.Size(101, 29);
      this.c_radioButtonCustom.TabIndex = 1;
      this.c_radioButtonCustom.TabStop = true;
      this.c_radioButtonCustom.Text = "Custom";
      this.c_radioButtonCustom.UseVisualStyleBackColor = true;
      this.c_radioButtonCustom.Click += new System.EventHandler(this.c_radioButtonCustom_Click);
      // 
      // c_radioButtonStrip
      // 
      this.c_radioButtonStrip.AutoSize = true;
      this.c_radioButtonStrip.Location = new System.Drawing.Point(25, 34);
      this.c_radioButtonStrip.Name = "c_radioButtonStrip";
      this.c_radioButtonStrip.Size = new System.Drawing.Size(73, 29);
      this.c_radioButtonStrip.TabIndex = 0;
      this.c_radioButtonStrip.TabStop = true;
      this.c_radioButtonStrip.Text = "Strip";
      this.c_radioButtonStrip.UseVisualStyleBackColor = true;
      this.c_radioButtonStrip.Click += new System.EventHandler(this.c_radioButtonStrip_Click);
      // 
      // c_textBoxLedSize
      // 
      this.c_textBoxLedSize.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_textBoxLedSize.Location = new System.Drawing.Point(678, 121);
      this.c_textBoxLedSize.Name = "c_textBoxLedSize";
      this.c_textBoxLedSize.Size = new System.Drawing.Size(94, 30);
      this.c_textBoxLedSize.TabIndex = 16;
      this.c_textBoxLedSize.Leave += new System.EventHandler(this.MainValuesChanged);
      // 
      // label6
      // 
      this.label6.AutoSize = true;
      this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label6.Location = new System.Drawing.Point(523, 121);
      this.label6.Name = "label6";
      this.label6.Size = new System.Drawing.Size(95, 25);
      this.label6.TabIndex = 15;
      this.label6.Text = "Led Size:";
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.label2.Location = new System.Drawing.Point(34, 30);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(213, 25);
      this.label2.TabIndex = 17;
      this.label2.Text = "Groups defined in code";
      // 
      // c_buttonAdd
      // 
      this.c_buttonAdd.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_buttonAdd.Location = new System.Drawing.Point(362, 89);
      this.c_buttonAdd.Name = "c_buttonAdd";
      this.c_buttonAdd.Size = new System.Drawing.Size(89, 38);
      this.c_buttonAdd.TabIndex = 18;
      this.c_buttonAdd.Text = "Add";
      this.c_buttonAdd.UseVisualStyleBackColor = true;
      this.c_buttonAdd.Click += new System.EventHandler(this.c_buttonAdd_Click);
      // 
      // c_buttonDelete
      // 
      this.c_buttonDelete.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_buttonDelete.Location = new System.Drawing.Point(362, 142);
      this.c_buttonDelete.Name = "c_buttonDelete";
      this.c_buttonDelete.Size = new System.Drawing.Size(89, 38);
      this.c_buttonDelete.TabIndex = 19;
      this.c_buttonDelete.Text = "Delete";
      this.c_buttonDelete.UseVisualStyleBackColor = true;
      this.c_buttonDelete.Click += new System.EventHandler(this.c_buttonDeleteClick);
      // 
      // c_buttonUp
      // 
      this.c_buttonUp.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_buttonUp.Location = new System.Drawing.Point(362, 194);
      this.c_buttonUp.Name = "c_buttonUp";
      this.c_buttonUp.Size = new System.Drawing.Size(89, 38);
      this.c_buttonUp.TabIndex = 20;
      this.c_buttonUp.Text = "Up";
      this.c_buttonUp.UseVisualStyleBackColor = true;
      this.c_buttonUp.Click += new System.EventHandler(this.c_buttonUp_Click);
      // 
      // c_buttonDown
      // 
      this.c_buttonDown.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.c_buttonDown.Location = new System.Drawing.Point(362, 249);
      this.c_buttonDown.Name = "c_buttonDown";
      this.c_buttonDown.Size = new System.Drawing.Size(89, 38);
      this.c_buttonDown.TabIndex = 21;
      this.c_buttonDown.Text = "Down";
      this.c_buttonDown.UseVisualStyleBackColor = true;
      this.c_buttonDown.Click += new System.EventHandler(this.c_buttonDown_Click);
      // 
      // EditLedSetup
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(1188, 782);
      this.Controls.Add(this.c_buttonDown);
      this.Controls.Add(this.c_buttonUp);
      this.Controls.Add(this.c_buttonDelete);
      this.Controls.Add(this.c_buttonAdd);
      this.Controls.Add(this.label2);
      this.Controls.Add(this.c_textBoxLedSize);
      this.Controls.Add(this.label6);
      this.Controls.Add(this.groupBox2);
      this.Controls.Add(this.groupBox1);
      this.Controls.Add(this.c_buttonOK);
      this.Controls.Add(this.c_textBoxLedCount);
      this.Controls.Add(this.c_textBoxGroupNumber);
      this.Controls.Add(this.label3);
      this.Controls.Add(this.label1);
      this.Controls.Add(this.c_listBoxLedGroups);
      this.Name = "EditLedSetup";
      this.Text = "EditLedSetup";
      this.groupBox1.ResumeLayout(false);
      this.groupBox1.PerformLayout();
      this.groupBox2.ResumeLayout(false);
      this.groupBox2.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

        }

        #endregion

        private ListBox c_listBoxLedGroups;
        private Label label1;
        private Label label3;
        private TextBox c_textBoxGroupNumber;
        private TextBox c_textBoxLedCount;
        private Button c_buttonOK;
        private TextBox c_textBoxX;
        private Label label4;
        private TextBox c_textBoxY;
        private Label label5;
        private GroupBox groupBox1;
        private GroupBox groupBox2;
        internal RadioButton c_radioButtonCustom;
        internal RadioButton c_radioButtonStrip;
        private TextBox c_textBoxLedSize;
        private Label label6;
        internal Button c_buttonCustomPaste;
        private Label label2;
        internal RadioButton c_radioButtonRing;
        internal TextBox c_textBoxRingRadius;
        private Label label7;
        private Button c_buttonAdd;
        private Button c_buttonDelete;
        private Button c_buttonUp;
        private Button c_buttonDown;
        internal CheckBox c_checkBoxReversed;
        internal RadioButton c_radioButtonMatrix;
        internal TextBox c_textBoxMatrixYCount;
        internal TextBox c_textBoxMatrixYSpacing;
        internal TextBox c_textBoxMatrixXSpacing;
        internal TextBox c_textBoxMatrixXCount;
        private Label label11;
        private Label label10;
        private Label label9;
        private Label label8;
        internal TextBox c_textBoxPwmColorCoding;
        private Label label12;
        internal RadioButton c_radioButtonPwm;
        internal TextBox c_textBoxStripSpacing;
        private Label label13;
    internal RadioButton c_radioButtonServo;
    internal TextBox c_textBoxServoMaxAngle;
    internal TextBox c_textBoxServoMinAngle;
    private Label label14;
    private Label label15;
    private Label label16;
  }
}