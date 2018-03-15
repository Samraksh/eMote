using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Samraksh.eMote.DotNow;

namespace TimerExample
{
    public class Program
    {
        Timer testTimer;
        bool timerState;
        int count = 0;
        static OutputPort Output = new OutputPort((Cpu.Pin)30, true);
        static OutputPort Error = new OutputPort((Cpu.Pin)31, true);
        Samraksh.eMote.DotNow.EmoteLCD lcd;

        public Program()
        {
            lcd = new Samraksh.eMote.DotNow.EmoteLCD();
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
            count++;
            timerState = !timerState;
            if (timerState)
                lcd.WriteN(4, Samraksh.eMote.DotNow.LCD.CHAR_0);
            else
                lcd.WriteN(4, Samraksh.eMote.DotNow.LCD.CHAR_1);

            Output.Write(timerState);
            Debug.Print("Inside timer " + count.ToString() + "...");
        }
                
        public static void Main()
        {
            //Debug.Print("Starting Testing..");
            Program p = new Program();
            Thread.Sleep(Timeout.Infinite);
        }


    }
}
