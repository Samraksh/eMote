namespace OpenLoopCarModel
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
            this.centerPicture = new System.Windows.Forms.PictureBox();
            this.carImage = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.centerPicture)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.carImage)).BeginInit();
            this.SuspendLayout();
            // 
            // centerPicture
            // 
            this.centerPicture.Image = ((System.Drawing.Image)(resources.GetObject("centerPicture.Image")));
            this.centerPicture.Location = new System.Drawing.Point(299, 356);
            this.centerPicture.Name = "centerPicture";
            this.centerPicture.Size = new System.Drawing.Size(15, 15);
            this.centerPicture.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.centerPicture.TabIndex = 2;
            this.centerPicture.TabStop = false;
            // 
            // carImage
            // 
            this.carImage.BackColor = System.Drawing.SystemColors.Window;
            this.carImage.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.carImage.Image = ((System.Drawing.Image)(resources.GetObject("carImage.Image")));
            this.carImage.Location = new System.Drawing.Point(452, 321);
            this.carImage.Name = "carImage";
            this.carImage.Size = new System.Drawing.Size(50, 50);
            this.carImage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.carImage.TabIndex = 3;
            this.carImage.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(765, 726);
            this.Controls.Add(this.carImage);
            this.Controls.Add(this.centerPicture);
            this.Name = "Form1";
            this.Text = "Form1";
           
            ((System.ComponentModel.ISupportInitialize)(this.centerPicture)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.carImage)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox centerPicture;
        private System.Windows.Forms.PictureBox carImage;
    }
}

