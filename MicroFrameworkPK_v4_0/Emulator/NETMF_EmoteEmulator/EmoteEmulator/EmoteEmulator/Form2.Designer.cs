namespace EmoteEmulator
{
    partial class Form2
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form2));
            this.carImage = new System.Windows.Forms.PictureBox();
            this.centerPicture = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.carImage)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.centerPicture)).BeginInit();
            this.SuspendLayout();
            // 
            // carImage
            // 
            this.carImage.Image = ((System.Drawing.Image)(resources.GetObject("carImage.Image")));
            this.carImage.Location = new System.Drawing.Point(495, 252);
            this.carImage.Name = "carImage";
            this.carImage.Size = new System.Drawing.Size(37, 36);
            this.carImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.carImage.TabIndex = 0;
            this.carImage.TabStop = false;
            // 
            // centerPicture
            // 
            this.centerPicture.Image = ((System.Drawing.Image)(resources.GetObject("centerPicture.Image")));
            this.centerPicture.Location = new System.Drawing.Point(277, 356);
            this.centerPicture.Name = "centerPicture";
            this.centerPicture.Size = new System.Drawing.Size(15, 15);
            this.centerPicture.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.centerPicture.TabIndex = 1;
            this.centerPicture.TabStop = false;
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(831, 630);
            this.Controls.Add(this.centerPicture);
            this.Controls.Add(this.carImage);
            this.Name = "Form2";
            this.Text = "Form2";
            ((System.ComponentModel.ISupportInitialize)(this.carImage)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.centerPicture)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox carImage;
        private System.Windows.Forms.PictureBox centerPicture;
    }
}