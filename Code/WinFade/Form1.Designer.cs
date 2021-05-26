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
            this.c_button = new System.Windows.Forms.Button();
            this.c_listBoxSerialOutput = new System.Windows.Forms.ListBox();
            this.c_buttonAbort = new System.Windows.Forms.Button();
            this.c_textBoxProgramText = new System.Windows.Forms.TextBox();
            this.c_labelErrorTag = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.c_labelOutputLineCount = new System.Windows.Forms.Label();
            this.c_buttonDrawTest = new System.Windows.Forms.Button();
            this.c_labelErrorText = new System.Windows.Forms.Label();
            this.c_comboBoxLedArrangements = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // c_button
            // 
            this.c_button.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_button.Location = new System.Drawing.Point(1066, 79);
            this.c_button.Name = "c_button";
            this.c_button.Size = new System.Drawing.Size(99, 39);
            this.c_button.TabIndex = 0;
            this.c_button.Text = "Run";
            this.c_button.UseVisualStyleBackColor = true;
            this.c_button.Click += new System.EventHandler(this.RunProgram);
            // 
            // c_listBoxSerialOutput
            // 
            this.c_listBoxSerialOutput.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_listBoxSerialOutput.FormattingEnabled = true;
            this.c_listBoxSerialOutput.ItemHeight = 25;
            this.c_listBoxSerialOutput.Location = new System.Drawing.Point(987, 142);
            this.c_listBoxSerialOutput.Name = "c_listBoxSerialOutput";
            this.c_listBoxSerialOutput.Size = new System.Drawing.Size(721, 429);
            this.c_listBoxSerialOutput.TabIndex = 2;
            // 
            // c_buttonAbort
            // 
            this.c_buttonAbort.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonAbort.Location = new System.Drawing.Point(1250, 79);
            this.c_buttonAbort.Name = "c_buttonAbort";
            this.c_buttonAbort.Size = new System.Drawing.Size(99, 39);
            this.c_buttonAbort.TabIndex = 3;
            this.c_buttonAbort.Text = "Abort";
            this.c_buttonAbort.UseVisualStyleBackColor = true;
            this.c_buttonAbort.Click += new System.EventHandler(this.c_buttonAbort_Click);
            // 
            // c_textBoxProgramText
            // 
            this.c_textBoxProgramText.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_textBoxProgramText.Location = new System.Drawing.Point(41, 79);
            this.c_textBoxProgramText.Multiline = true;
            this.c_textBoxProgramText.Name = "c_textBoxProgramText";
            this.c_textBoxProgramText.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.c_textBoxProgramText.Size = new System.Drawing.Size(838, 549);
            this.c_textBoxProgramText.TabIndex = 4;
            // 
            // c_labelErrorTag
            // 
            this.c_labelErrorTag.AutoSize = true;
            this.c_labelErrorTag.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_labelErrorTag.Location = new System.Drawing.Point(36, 656);
            this.c_labelErrorTag.Name = "c_labelErrorTag";
            this.c_labelErrorTag.Size = new System.Drawing.Size(60, 25);
            this.c_labelErrorTag.TabIndex = 5;
            this.c_labelErrorTag.Text = "Error:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(1498, 65);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(171, 25);
            this.label2.TabIndex = 6;
            this.label2.Text = "Output Line Count";
            // 
            // c_labelOutputLineCount
            // 
            this.c_labelOutputLineCount.AutoSize = true;
            this.c_labelOutputLineCount.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_labelOutputLineCount.Location = new System.Drawing.Point(1538, 101);
            this.c_labelOutputLineCount.Name = "c_labelOutputLineCount";
            this.c_labelOutputLineCount.Size = new System.Drawing.Size(0, 25);
            this.c_labelOutputLineCount.TabIndex = 7;
            // 
            // c_buttonDrawTest
            // 
            this.c_buttonDrawTest.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_buttonDrawTest.Location = new System.Drawing.Point(1066, 600);
            this.c_buttonDrawTest.Name = "c_buttonDrawTest";
            this.c_buttonDrawTest.Size = new System.Drawing.Size(131, 39);
            this.c_buttonDrawTest.TabIndex = 8;
            this.c_buttonDrawTest.Text = "Draw Test";
            this.c_buttonDrawTest.UseVisualStyleBackColor = true;
            this.c_buttonDrawTest.Click += new System.EventHandler(this.c_buttonDrawTest_Click);
            // 
            // c_labelErrorText
            // 
            this.c_labelErrorText.AutoSize = true;
            this.c_labelErrorText.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_labelErrorText.Location = new System.Drawing.Point(102, 656);
            this.c_labelErrorText.Name = "c_labelErrorText";
            this.c_labelErrorText.Size = new System.Drawing.Size(0, 25);
            this.c_labelErrorText.TabIndex = 10;
            // 
            // c_comboBoxLedArrangements
            // 
            this.c_comboBoxLedArrangements.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.c_comboBoxLedArrangements.FormattingEnabled = true;
            this.c_comboBoxLedArrangements.Location = new System.Drawing.Point(232, 25);
            this.c_comboBoxLedArrangements.Name = "c_comboBoxLedArrangements";
            this.c_comboBoxLedArrangements.Size = new System.Drawing.Size(285, 33);
            this.c_comboBoxLedArrangements.TabIndex = 11;
            this.c_comboBoxLedArrangements.SelectedIndexChanged += new System.EventHandler(this.c_comboBoxLedArrangements_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(36, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(174, 25);
            this.label1.TabIndex = 12;
            this.label1.Text = "LED Arrangement:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1851, 690);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.c_comboBoxLedArrangements);
            this.Controls.Add(this.c_labelErrorText);
            this.Controls.Add(this.c_buttonDrawTest);
            this.Controls.Add(this.c_labelOutputLineCount);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.c_labelErrorTag);
            this.Controls.Add(this.c_textBoxProgramText);
            this.Controls.Add(this.c_buttonAbort);
            this.Controls.Add(this.c_listBoxSerialOutput);
            this.Controls.Add(this.c_button);
            this.Name = "Form1";
            this.Text = "WinFade";
            this.ResumeLayout(false);
            this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.Button c_button;
    private System.Windows.Forms.ListBox c_listBoxSerialOutput;
    private System.Windows.Forms.Button c_buttonAbort;
    private System.Windows.Forms.TextBox c_textBoxProgramText;
    private System.Windows.Forms.Label c_labelErrorTag;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.Label c_labelOutputLineCount;
    private System.Windows.Forms.Button c_buttonDrawTest;
    private System.Windows.Forms.Label c_labelErrorText;
        private System.Windows.Forms.ComboBox c_comboBoxLedArrangements;
        private System.Windows.Forms.Label label1;
    }
}

