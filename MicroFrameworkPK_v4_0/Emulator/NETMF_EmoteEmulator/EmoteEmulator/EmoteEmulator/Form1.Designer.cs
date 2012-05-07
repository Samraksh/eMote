namespace EmoteEmulator
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
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.statusBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.led1 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.led2 = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.led3 = new System.Windows.Forms.TextBox();
            this.button3 = new System.Windows.Forms.Button();
            this.inputEmulation = new System.Windows.Forms.Button();
            this.button_carGUI = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(14, 238);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(125, 239);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 1;
            this.button2.Text = "button2";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // statusBox
            // 
            this.statusBox.Location = new System.Drawing.Point(16, 82);
            this.statusBox.Multiline = true;
            this.statusBox.Name = "statusBox";
            this.statusBox.Size = new System.Drawing.Size(302, 72);
            this.statusBox.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(110, 63);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(101, 16);
            this.label1.TabIndex = 3;
            this.label1.Text = "Emulator Status";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(44, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(256, 24);
            this.label2.TabIndex = 4;
            this.label2.Text = "Samraksh Emote Emulator";
            // 
            // led1
            // 
            this.led1.BackColor = System.Drawing.Color.MistyRose;
            this.led1.Location = new System.Drawing.Point(39, 208);
            this.led1.Name = "led1";
            this.led1.Size = new System.Drawing.Size(23, 20);
            this.led1.TabIndex = 7;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(32, 189);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(44, 16);
            this.label3.TabIndex = 8;
            this.label3.Text = "LED 1";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(144, 189);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(44, 16);
            this.label4.TabIndex = 10;
            this.label4.Text = "LED 2";
            // 
            // led2
            // 
            this.led2.BackColor = System.Drawing.Color.LightSteelBlue;
            this.led2.Location = new System.Drawing.Point(152, 208);
            this.led2.Name = "led2";
            this.led2.Size = new System.Drawing.Size(23, 20);
            this.led2.TabIndex = 9;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(256, 189);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(44, 16);
            this.label5.TabIndex = 12;
            this.label5.Text = "LED 3";
            // 
            // led3
            // 
            this.led3.BackColor = System.Drawing.Color.Honeydew;
            this.led3.Location = new System.Drawing.Point(265, 208);
            this.led3.Name = "led3";
            this.led3.Size = new System.Drawing.Size(23, 20);
            this.led3.TabIndex = 11;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(240, 240);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 13;
            this.button3.Text = "button3";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // inputEmulation
            // 
            this.inputEmulation.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.inputEmulation.Location = new System.Drawing.Point(16, 274);
            this.inputEmulation.Name = "inputEmulation";
            this.inputEmulation.Size = new System.Drawing.Size(145, 23);
            this.inputEmulation.TabIndex = 14;
            this.inputEmulation.Text = "Start Input Emulation";
            this.inputEmulation.UseVisualStyleBackColor = true;
            this.inputEmulation.Click += new System.EventHandler(this.inputEmulation_Click);
            // 
            // button_carGUI
            // 
            this.button_carGUI.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button_carGUI.Location = new System.Drawing.Point(173, 274);
            this.button_carGUI.Name = "button_carGUI";
            this.button_carGUI.Size = new System.Drawing.Size(145, 23);
            this.button_carGUI.TabIndex = 15;
            this.button_carGUI.Text = "Display CarGUI";
            this.button_carGUI.UseVisualStyleBackColor = true;
            this.button_carGUI.Click += new System.EventHandler(this.button_carGUI_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(343, 309);
            this.Controls.Add(this.button_carGUI);
            this.Controls.Add(this.inputEmulation);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.led3);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.led2);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.led1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.statusBox);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox statusBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox led1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox led2;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox led3;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button inputEmulation;
        private System.Windows.Forms.Button button_carGUI;
    }
}

