using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
//using System.Threading;
using System.Windows.Forms;
using OpenLoopCarModel;

namespace OpenLoopCarModel
{
    public partial class Form1 : Form
    {
        System.Windows.Forms.Timer updateTimer;
        //public static System.Windows.Forms.Timer greenDotUpdateTimer;
        int curX, curY = 0;
        int curAngle = 90;
        //int preX, preY = 0;
        int tarX, tarY = 0;
        int incX, incY, incAngle = 0;
        int noOfIncrements = 0;
        const int centerXoffset = -25;
        const int centerYoffset = -25;
        public int greenX, greenY;
        
        public Form1()
        {
            InitializeComponent();
            greenX=-1;greenY=-1;
            //moveCenter(greenX,greenY, 1000);
            //testTimer = new System.Threading.Timer(new TimerCallback(testTimerCallback), null, 0, 50);
            updateTimer = new System.Windows.Forms.Timer();
            updateTimer.Interval = 50;
            updateTimer.Tick += new EventHandler(updateTimer_Tick);
            updateTimer.Start();
        }

        public void moveGreenDot(int x, int y)
        {         
            greenX = x;
            greenY = y;
        }
        
        void updateTimer_Tick(object sender, EventArgs e)
        {
            if (noOfIncrements == 0)
            {
                return;
            }
            curX = curX + incX;
            curY = curY + incY;
            curAngle += incAngle;
            moveCar(curX, curY, curAngle, 750);
            noOfIncrements--;
            moveCenter(greenX, greenY, 750);
        }

        public void UpdateLocation(double X, double Y, int duration, int carAngle)
        {
            //preX = curX;
            //preY=curY;
            //updateTimer.Stop();
            if (duration < 50) duration = 50;
            tarX = (int)X;
            tarY = (int)Y;
            //Console.WriteLine("Target X:" + tarX.ToString() + ", Y:" + tarY.ToString());
            noOfIncrements = (duration / 50);
            incX = (tarX - curX);
            incY = (tarY - curY);
            incAngle = carAngle - curAngle;

            //Find the increament amount for each iteration
            int max = Math.Max(Math.Abs(incX), Math.Abs(incY));
            if (max < noOfIncrements) noOfIncrements = max;
            if (max == 0) noOfIncrements = 1;

            //Console.WriteLine("Incrementes:" + noOfIncrements.ToString());
            incX = incX / noOfIncrements;
            incY = incY / noOfIncrements;
            incAngle = incAngle / noOfIncrements;

            //Console.WriteLine("Inc X:" + incX.ToString() + ", Y:" + incY.ToString());
            //Console.WriteLine("DeltaAngle in Form: " + carAngle.ToString());
            /*if (carAngle != 0)
            {
                Size s = new Size(40,40);
                Bitmap b = new Bitmap(carImage.Image, s);
                carImage.Image = rotateImage(b, - (float)carAngle);
                //Image rotatedImage = rotateImage(b, -(float)carAngle);

            }*/
           /* if (!updateTimer.Enabled)
            {
                updateTimer.Enabled = true;
                updateTimer.Start();
                Console.WriteLine("Enabled timer");
            }*/
            //moveCar((int)X, (int)Y, duration);
        }

        private Bitmap rotateImage(float angle)
        {
            //create a new empty bitmap to hold rotated image
            //Size s = new Size(40, 40);
            //Console.WriteLine("Rotating car by: " + angle.ToString());
            //Bitmap b = new Bitmap("RedcarS1.bmp");
            Bitmap b = new Bitmap("D:\\Main\\Emote-dev\\Dev\\MicroFrameworkPK_v4_0\\Emulator\\NETMF_EmoteEmulator\\EmoteEmulator\\EmoteEmulator\\RedcarS3.bmp");
            //Console.WriteLine("Read File");
            //make a graphics object from the empty bitmap
            Graphics g = Graphics.FromImage(b);
            //move rotation point to center of image

            //Console.WriteLine("Rotating car by: " + angle.ToString());
            g.TranslateTransform((float)b.Width / 2, (float)b.Height / 2);

            //rotate
            g.RotateTransform(-(angle - 90));
            //move image back
            g.TranslateTransform(-(float)b.Width / 2, -(float)b.Height / 2);
            //draw passed in image onto graphics object
            g.DrawImage(b, new Point(0, 0));
            return b;
        }

        private void moveCar(int X, int Y, int angle, int duration)
        {
            carImage.Left = X + centerXoffset;
            carImage.Top = Y + centerYoffset;
            carImage.Image = rotateImage((float)angle);

        }
        public void moveCenter(int X, int Y, int duration)
        {
            centerPicture.Left = X;
            centerPicture.Top = Y;
            if (X == -1 && Y == -1)
            {
                centerPicture.Visible = false;
            }
            else
            {
                centerPicture.Visible = true;
            }
        }


        public void rotateCar(short degree)
        {
            RotateFlipType type;
            if (degree == 90)
                type = RotateFlipType.Rotate90FlipX;
            else
                type = RotateFlipType.Rotate270FlipX;

            carImage.Image.RotateFlip(type);
        }

        //public void updateTimerCallback(object state)
       /* public void updateTimerCallback()
        {
            if (noOfIncrements == 0)
            {
                return;
            }
            curX = curX + incX;
            curY = curY + incY;
            curAngle += incAngle;
            moveCar(curX, curY, curAngle, 750);
            noOfIncrements--;
            updateTimer.Change(50, Timeout.Infinite);
            Console.WriteLine("X:" + curX.ToString() + ", Y:" + curY.ToString() + ", CurAngle: " + curAngle.ToString());
        }
        */
      

    }
}
