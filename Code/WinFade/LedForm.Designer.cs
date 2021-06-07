namespace WinFade
{
  partial class LedForm
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
            this.c_pictureBoxLeds = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.c_pictureBoxLeds)).BeginInit();
            this.SuspendLayout();
            // 
            // c_pictureBoxLeds
            // 
            this.c_pictureBoxLeds.BackColor = System.Drawing.Color.Black;
            this.c_pictureBoxLeds.Dock = System.Windows.Forms.DockStyle.Fill;
            this.c_pictureBoxLeds.Location = new System.Drawing.Point(0, 0);
            this.c_pictureBoxLeds.Name = "c_pictureBoxLeds";
            this.c_pictureBoxLeds.Size = new System.Drawing.Size(1408, 685);
            this.c_pictureBoxLeds.TabIndex = 0;
            this.c_pictureBoxLeds.TabStop = false;
            this.c_pictureBoxLeds.MouseDown += new System.Windows.Forms.MouseEventHandler(this.c_pictureBoxLeds_MouseDown);
            this.c_pictureBoxLeds.MouseMove += new System.Windows.Forms.MouseEventHandler(this.c_pictureBoxLeds_MouseMove);
            this.c_pictureBoxLeds.MouseUp += new System.Windows.Forms.MouseEventHandler(this.c_pictureBoxLeds_MouseUp);
            // 
            // LedForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1408, 685);
            this.Controls.Add(this.c_pictureBoxLeds);
            this.Name = "LedForm";
            this.ShowInTaskbar = false;
            this.Text = "Fade Output";
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.LedForm_KeyPress);
            ((System.ComponentModel.ISupportInitialize)(this.c_pictureBoxLeds)).EndInit();
            this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.PictureBox c_pictureBoxLeds;
    }
}