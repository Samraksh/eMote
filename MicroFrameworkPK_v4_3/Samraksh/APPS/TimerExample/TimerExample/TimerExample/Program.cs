using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware;

namespace TimerExample
{
    public class Program
    {
        Timer testTimer;
        bool timerState;
        static OutputPort Output = new OutputPort((Cpu.Pin)30, true);
        static OutputPort Error = new OutputPort((Cpu.Pin)31, true);
        Samraksh.SPOT.Hardware.EmoteDotNow.EmoteLCD lcd;

        public Program()
        {
            lcd = new Samraksh.SPOT.Hardware.EmoteDotNow.EmoteLCD();
            lcd.Initialize();

            try
            {
                testTimer = new Timer(timerCallback, null, 0, 1000);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
                while (true)
                {
                    Error.Write(true);
                    Error.Write(false);
                    Thread.Sleep(10);
                }
            }


        }

        private void timerCallback(object state)
        {
            
            timerState = !timerState;
            if (timerState)
                lcd.WriteN(4, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_0);
            else
                lcd.WriteN(4, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_1);

            Output.Write(timerState);
            Debug.Print("Inside timer..");
        }
                
        public static void Main()
        {
            Debug.Print("Starting Testing..");
            Program p = new Program();
            Thread.Sleep(Timeout.Infinite);
        }


    }
}
