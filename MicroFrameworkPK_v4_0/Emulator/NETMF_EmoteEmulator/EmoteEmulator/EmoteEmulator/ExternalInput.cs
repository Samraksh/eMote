using System;
using System.Threading;
using System.IO;


using EmoteEmulator;

namespace EmoteEmulator
{
    public class ExternalInput
    {
        String fileName;
        int[] delay=null;
        int[] gpioPort = null;
        bool[] value = null;
        Thread signalInputThread;
        //Form1 f1 = new Form1();

        public ExternalInput()
        {
        }

        public ExternalInput(String fName)
        {
            fileName = fName;
            signalInputThread = new Thread(new ThreadStart(InjectValues));
        }

        public void Run()
        {
            ReadInputFile(fileName);
            signalInputThread.Start();
            //InjectValues();
            //Thread.Sleep(10);
        }

        public void ReadInputFile(String fileName)
        {
            string[] readText = File.ReadAllLines(fileName);
            delay = new int[readText.Length];
            gpioPort = new int[readText.Length];
            value = new bool[readText.Length];
            int i = 0;
            foreach (string s in readText)
            {       
                string[] fields = s.Split(',');
                Console.WriteLine("Delay: " + fields[0] + ", Port: " + fields[1] + ", Value: " + fields[2]);
                delay[i] = Convert.ToInt32(fields[0]);
                gpioPort[i] = Convert.ToInt32(fields[1]);
                value[i] = Convert.ToBoolean(Convert.ToInt32(fields[2]));
                i++;
            }
        }
        public void InjectValues(){
            //Console.WriteLine("Starting Injection, entries: " + Convert.ToString(delay.Length));
            for (int i=0;i< delay.Length;i++ ){
                Thread.Sleep(delay[i]);
                Console.WriteLine("Sending signal to port: " + Convert.ToString(gpioPort[i]) + ", Value: " + Convert.ToString(value[i]));
                Form1.signalGpio(gpioPort[i], value[i]);
            }

        }

    }
}