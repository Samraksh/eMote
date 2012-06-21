using System;
using System.Collections.Generic;
//using System.Linq;
using System.Text;
using System.Threading;
using Microsoft.SPOT.Emulator;

namespace EmoteEmulator
{
    public class OpenLoopCar
    {
        //Program myProgram;
        //CarGUI _cg;
        Form2 _f2;
        Timer locationUpdateTimer, inputTimer, testTimer;
        Thread testThread;

        //Input variables
        CarPMInput carInput;
        short wheelAngle;
        double[] centerOfTurn = new double[2];
        //Constants
        const int omegaMax = 20; //degrees/sec
        const int accelMax = 10; //m/sec^2
        const int velocityMax = 75; //m/sec

        const int lengthOfCar = 10; //feet
        const int widthOfcar = 5; //feet
        const short TurnLeft = 90;
        const short TurnRight = -90;

        //Intermediate variables
        bool isTurning;


        //Output Variables;
        double radiusOfTurn;
        int carVelocity;
        int carAngle;
       
        double preX,curX;
        double preY,curY;
        
        public OpenLoopCar(Form2 f2)
        {
           _f2= f2;
            locationUpdateTimer = new Timer(new TimerCallback(updateCarLocation), null, 0, 1000);
            

            //Initialize variables
            curX = _f2.Width / 2;
            curY = _f2.Height - 50;
            carAngle = 90;
            wheelAngle = 0;
            centerOfTurn[0] = centerOfTurn[1] = 0;
            radiusOfTurn = Timeout.Infinite;
            carVelocity = velocityMax/2;

            //testTimer = new Timer(new TimerCallback(Drive_A_Eight), null, 0, 1000);
            //testThread = new Thread(new ThreadStart(Drive_A_Eight));
            //testThread.Start();
            
        }

        void Drive_A_Eight()
        {
            Thread.Sleep(5000);
            Turn(70);
            Thread.Sleep(3000);
            
            Turn(-70);
            Thread.Sleep(5000);
            
            Turn(TurnRight);
            Thread.Sleep(13000);
            
            Turn(TurnLeft);
            Thread.Sleep(10000);
            
            Turn(-30);
            Thread.Sleep(5000);
            
            Turn(30);
            Thread.Sleep(5000);
            Thread.Sleep(5000);
            
            Stop();

        }

        public void Stop()
        {
            //testTimer.Dispose();
            locationUpdateTimer.Dispose();
            Console.WriteLine("Stop");
        }

        public void Turn(short degrees)
        {
            Console.WriteLine ("Turning by: " +degrees.ToString() );
            wheelAngle += degrees;
            wheelAngle = (short)(wheelAngle % 360);
            centerOfTurn = findCenterOfTurn();
            if (wheelAngle!=0)
            {
                isTurning = true;
            }
            else
            {
                isTurning = false;
                centerOfTurn[0] = -1;
                centerOfTurn[1] = -1;
            }
            _f2.moveCenter((int)centerOfTurn[0], (int)centerOfTurn[1], 750);
        }

        public void Accelerate (int acce)
        {

        }

        void  updateCarLocation(object state)
        {
            double[] a = new double[2]; double[] b = new double[2];
            a[0] = curX; a[1] = curY;
            if (isTurning)
            {
                b = findNextLocationCircle(centerOfTurn, a, Math.Abs(radiusOfTurn), wheelAngle);
                //Console.WriteLine("Radius of turn: " + radiusOfTurn.ToString());
            }
            else
            {

                b = findNextLocationSL();
                //Console.WriteLine("We are on a straight line");
            }
            preX = curX; preY = curY;
            curX = b[0]; curY = b[1];
            //Console.WriteLine("newX:" + curX.ToString() + " , newY:" + curY.ToString());
            _f2.UpdateLocation(curX, curY, 1000);
        }

        double[] findCenterOfTurn()
        {
            double[] C = new double[2];
            //Console.WriteLine("Wheel Angle: " + wheelAngle.ToString());
            radiusOfTurn = findRadiusOfTurn(wheelAngle);
            //Console.WriteLine("RT: " + radiusOfTurn.ToString());
            //C[0] = (curX + Math.Cos(Math.PI * (carAngle + wheelAngle + 90) / 180) * radiusOfTurn);
            //C[1] = (curY + Math.Cos(Math.PI * (carAngle + wheelAngle + 180) / 180) * radiusOfTurn);
            C[0] = (curX + Math.Cos(Math.PI * (carAngle + 90) / 180) * radiusOfTurn);
            C[1] = (curY + Math.Cos(Math.PI * (carAngle + 180) / 180) * radiusOfTurn);
            return C;
        }

        //Find next location on a circle with center c, from current point a, next point b, and radius r
        double[] findNextLocationCircle(double[] c, double[] a, double r, short wheelAngle)
        {
            //create point b
            double[] b = new double[2];


            //Find the angle from y-axis for line ar
            double alpha = Math.Atan2((a[1] - c[1]), (a[0] - c[0]));
            //Console.WriteLine(a[0].ToString() + "," + a[1].ToString() + "," + c[0].ToString() + "," + c[1].ToString() + "," + r.ToString());

            //Determine angular displacement in radians
            double theta = carVelocity / r;

            //Console.WriteLine("Intial angle: " + alpha.ToString() + ", displacement angle: " + theta.ToString());
            //Point b is distance r, bearing alpha+theta from point c (bearing measured cw from 
            //positive y-axis, as in navigation). Finally, in rectangular
            if (wheelAngle > 0)
            {
                b[0] = (c[0] + r * Math.Cos(alpha - theta));
                b[1] = (c[1] + r * Math.Sin(alpha - theta));
                carAngle += (int) (theta*180/Math.PI);
            }
            else
            {
                b[0] = (c[0] + r * Math.Cos(alpha + theta));
                b[1] = (c[1] + r * Math.Sin(alpha + theta));
                carAngle -= (int)(theta * 180 / Math.PI);
            }
            Console.WriteLine("Car Angle: " + carAngle.ToString());
            //b[0] = (c[0] + r * Math.Sin(alpha + Math.PI - (theta * Math.PI)));
            //b[1] = (c[1] + r * Math.Cos(alpha + Math.PI - (theta * Math.PI)));


            /*double nextX, nextY;
            nextX = curX + Math.Cos(Math.PI * carAngle / 180) * carVelocity;
            nextY = curY + Math.Cos(Math.PI * (carAngle + 90) / 180) * carVelocity;
            preX = curX; preY = curY;
            curX = nextX; curY = nextY;*/
            return b;
        }


        double[]  findNextLocationSL()
        {
            double[] next = new double[2] ;
            next[0] = curX + Math.Cos(Math.PI * carAngle/180) * carVelocity;
            next[1] = curY + Math.Cos(Math.PI * (carAngle+90) / 180) * carVelocity;
            return next;
        }


        void setCarVelocity(int __pedal)
        {
            //Dummy function to calculate pedal
            carVelocity += __pedal;
            if (carVelocity > velocityMax) carVelocity = velocityMax;
            if (carVelocity < -velocityMax) carVelocity = -velocityMax;
        }

        void setCarAngle(int __steering)
        {
            radiusOfTurn += __steering;
            if (carVelocity > velocityMax) carVelocity = velocityMax;
            if (carVelocity < -velocityMax) carVelocity = -velocityMax;

        }

        double findRadiusOfTurn(short angle)
        {
            //Console.WriteLine("WA: " + angle.ToString());
            //double radius = Math.Abs((lengthOfCar * 2 * Math.PI) / ((double)angle/180));
            double radius = (lengthOfCar * 2 * Math.PI) / ((double)angle / 180);
            //Console.WriteLine("RT: " + radius.ToString());
            return radius;
        }



    }

}
