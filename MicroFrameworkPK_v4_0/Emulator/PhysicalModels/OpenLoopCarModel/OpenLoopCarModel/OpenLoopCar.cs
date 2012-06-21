using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Threading;

using Samraksh.PhysicalModel;
using OpenLoopCarModel;

namespace OpenLoopCarModel
{
    enum CarCommands
    {
        START,STOP
    };
    class CarModelInput
    {
        public short SteeringAngle=0; //-128to127
        public sbyte Accelaration=0;//int8 -63 to 63
        public byte State=0; //Special instructions 
    }

    class CarModelOutput
    {
        public ushort X=0; //0-65535
        public ushort Y=0; //0-65535
        public byte speed=0;//0-255
        public short direction=0; //degrees 
    }

    class OpenLoopCar
    {
        PhysicalModelEmulatorComm emulatorCom;
        //Declare a delegate to handle serial port input
        public delegate void ProcessSerialInput(byte[] sInput);
        //static ProcessSerialInput mySerialInput;
        //CarModelInput carInput; 


        Form1 f1;

        System.Windows.Forms.Timer locationUpdateTimer;
        //System.Threading.Timer locationUpdateTimer, inputTimer, testTimer;
        //Thread testThread;

        //Input variables
        //CarPMInput carInput;
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
        int deltaCarAngle = 0;

        double preX, curX;
        double preY, curY;
       

        public void Start()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            f1 = new Form1();
            curX = f1.Width / 2;
            curY = f1.Height - 50;
            //locationUpdateTimer = new System.Threading.Timer(new TimerCallback(updateCarLocation), null, 0, 1000);
            locationUpdateTimer = new System.Windows.Forms.Timer(); //declared       
            //under the class

            locationUpdateTimer.Interval = 1000;
            locationUpdateTimer.Tick += new EventHandler(locationUpdateTimer_Tick);
            locationUpdateTimer.Start();

            
            Application.Run(f1);
        }

        void SendMessageToEmulator(byte[] message)
        {
            emulatorCom.SendToSerial(message);
        }
      

        public OpenLoopCar()
        {       
            emulatorCom = new PhysicalModelEmulatorComm();
            emulatorCom.InitializeSerial(HandleSerialInput);
            emulatorCom.ConnectToEmulator();

            //Initialize variables
            //f1 = new Form1();
            //curX = f1.Width / 2;
            //curY = f1.Height - 50;
            curX = 300;
            curY = 300;
            carAngle = 90;
            wheelAngle = 0;
            centerOfTurn[0] = centerOfTurn[1] = 0;
            radiusOfTurn = Timeout.Infinite;
            carVelocity = velocityMax / 2;


            //locationUpdateTimer = new System.Threading.Timer(new TimerCallback(updateCarLocation), null, 0, 1000);
            //testTimer = new Timer(new TimerCallback(Drive_A_Eight), null, 0, 1000);
            //testThread = new Thread(new ThreadStart(Drive_A_Eight));
            //testThread.Start();

        }

        void Drive_A_Eight()
        {
            Thread.Sleep(8000);
            Turn(70);
            Thread.Sleep(5000);

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

        void Stop()
        {
            //testTimer.Dispose();
            locationUpdateTimer.Dispose();
            Console.WriteLine("Stop");
        }

        void Turn(short degrees)
        {
            Console.WriteLine("Turning by: " + degrees.ToString());
            wheelAngle += degrees;
            wheelAngle = (short)(wheelAngle % 360);
            centerOfTurn = findCenterOfTurn();
            if (wheelAngle != 0)
            {
                isTurning = true;
            }
            else
            {
                isTurning = false;
                centerOfTurn[0] = -1;
                centerOfTurn[1] = -1;
            }

            f1.moveGreenDot((int)centerOfTurn[0], (int)centerOfTurn[1]);
        }

        void Accelerate(int acce)
        {

        }

        //void updateCarLocation(object state)
        void locationUpdateTimer_Tick(object sender, EventArgs e)
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
                deltaCarAngle = 0;
                //Console.WriteLine("We are on a straight line");
            }
            preX = curX; preY = curY;
            curX = b[0]; curY = b[1];
            //Console.WriteLine("newX:" + curX.ToString() + " , newY:" + curY.ToString());
            f1.UpdateLocation(curX, curY, 1000, carAngle);
            //Console.WriteLine("DeltaAngle: " + deltaCarAngle.ToString());
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
                deltaCarAngle = (int)(theta * 180 / Math.PI);
                carAngle += deltaCarAngle;
            }
            else
            {
                b[0] = (c[0] + r * Math.Cos(alpha + theta));
                b[1] = (c[1] + r * Math.Sin(alpha + theta));
                deltaCarAngle = -(int)(theta * 180 / Math.PI);
                carAngle += deltaCarAngle;
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


        double[] findNextLocationSL()
        {
            double[] next = new double[2];
            next[0] = curX + Math.Cos(Math.PI * carAngle / 180) * carVelocity;
            next[1] = curY + Math.Cos(Math.PI * (carAngle + 90) / 180) * carVelocity;
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
            double radius = (lengthOfCar * (Math.PI / 2)) / ((double)angle / 180);
            //Console.WriteLine("RT: " + radius.ToString());
            return radius;
        }

        public void HandleSerialInput(byte[] input)
        {
            if (input.Length == 4)
            {
                short SteeringAgle = BitConverter.ToInt16(input, 0);
                sbyte Accelaration = (sbyte)input[2];
                byte State = input[3];

                Console.WriteLine("Read from MF App: " + input.Length.ToString() + ", Wheel Angle:" + SteeringAgle.ToString() + " , Accl:" + Accelaration.ToString());
                if (State == (byte)CarCommands.STOP)
                {
                    Stop();
                }
                else
                {
                    Turn(SteeringAgle);
                }
            }
            else
            {
                Console.WriteLine("Packet of unknown length: " + input.Length.ToString());
            }
        }
        

        
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            OpenLoopCar car = new OpenLoopCar();
            car.Start();           
        }
    }
}
