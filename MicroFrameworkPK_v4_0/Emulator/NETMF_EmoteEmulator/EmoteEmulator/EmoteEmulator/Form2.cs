using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;


namespace EmoteEmulator
{
    public partial class Form2 : Form
    {
        public OpenLoopCar olc; 
        System.Threading.Timer updateTimer;
        int curX, curY = 0;
        //int preX, preY = 0;
        int tarX, tarY = 0;
        int incX, incY = 0;
        int noOfIncrements = 0;

        public Form2()
        {
            InitializeComponent();
            olc = new OpenLoopCar(this);
            //curX = this.Width / 2;
            //curY = this.Height - 50;
            //moveCar(200, 200, 750);
            centerPicture.Left = -1;
            centerPicture.Top = -1;
            //testTimer = new System.Threading.Timer(new TimerCallback(testTimerCallback), null, 0, 50);
            updateTimer = new System.Threading.Timer(new TimerCallback(updateTimerCallback), null, Timeout.Infinite, Timeout.Infinite);            
        }

        public void UpdateLocation(double X, double Y, int duration)
        {
            //preX = curX;
            //preY=curY;
            if (duration < 50) duration = 50;
            tarX = (int)X;
            tarY = (int)Y;
            //Console.WriteLine("Target X:" + tarX.ToString() + ", Y:" + tarY.ToString());
            noOfIncrements = (duration / 50);
            incX= (tarX - curX) ;
            incY = (tarY - curY);
            
            //Find the increament amount for each iteration
            int max = Math.Max(Math.Abs(incX), Math.Abs(incY)); 
            if(max < noOfIncrements) noOfIncrements=max;
            if (max ==0 ) noOfIncrements = 1;
            
            //Console.WriteLine("Incrementes:" + noOfIncrements.ToString());
            incX = incX / noOfIncrements;
            incY = incY / noOfIncrements ;
            //Console.WriteLine("Inc X:" + incX.ToString() + ", Y:" + incY.ToString());
            
            
            updateTimer.Change(50, Timeout.Infinite);
            //moveCar((int)X, (int)Y, duration);
        }

        private void moveCar(int X, int Y, int duration)
        {
            carImage.Left = X;
            carImage.Top = Y;
        }
        public void moveCenter(int X, int Y, int duration)
        {
            centerPicture.Left = X;
            centerPicture.Top = Y;
        }


        public void rotateCar (short degree){
            RotateFlipType type;
            if(degree ==90)
                type = RotateFlipType.Rotate90FlipX;
            else
                type = RotateFlipType.Rotate270FlipX;

            carImage.Image.RotateFlip(type);
        }

        public void updateTimerCallback(object state)
        {
            if (noOfIncrements == 0)
            {
                return;
            }
            curX = curX + incX;
            curY = curY + incY;
            moveCar(curX, curY, 750);
            noOfIncrements--;
            updateTimer.Change(50, Timeout.Infinite);
            //Console.WriteLine("X:" + curX.ToString() + ", Y:" + curY.ToString() + ", Inc:" + noOfIncrements.ToString());
        }
    }
}
