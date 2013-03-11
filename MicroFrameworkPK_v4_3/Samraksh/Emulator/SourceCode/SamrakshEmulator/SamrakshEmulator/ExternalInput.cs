using System;
using System.Threading;
using System.IO;


using SamrakshEmulator;

namespace SamrakshEmulator
{
    public class ExternalInput
    {
        String fileName;
        int[] delay=null;
        int[] cpuPin = null;
        bool[] value = null;
        Thread signalInputThread;
        bool absoluteTime = false;
        const byte configLines = 1;
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

            delay = new int[readText.Length - configLines];
            cpuPin = new int[readText.Length - configLines];
            value = new bool[readText.Length - configLines];
            int i = 0;

            foreach (string s in readText)
            {
                //Handle config lines using a switch, beware expect config in order
                switch  (i){
                    //Time: Absolute or Relative
                    case 0:     
                        string[] timeField = s.Split(':');
                        if (timeField[1] == "Absolute" || timeField[1] == "absolute")
                        {
                            absoluteTime = true;
                            Console.WriteLine("Time is :" + timeField[1]);
                        }
                        break;
                
                    default:
                        string[] fields = s.Split(',');
                        Console.WriteLine("Delay: " + fields[0] + ", Node: " + fields[1] + ", Port: " + fields[2] + ", Value: " + fields[3]);
                        delay[i-configLines] = Convert.ToInt32(fields[0]);
                        cpuPin[i - configLines] = Convert.ToInt32(fields[2]);
                        value[i - configLines] = Convert.ToBoolean(Convert.ToInt32(fields[3]));
                        break;
                } //End of switch
                i++;
            }
        }
        public void InjectValues(){
            //Console.WriteLine("Starting Injection, entries: " + Convert.ToString(delay.Length));
            for (int i=0;i< delay.Length;i++ ){
                if (absoluteTime)
                {
                    if(i==0)
                        Thread.Sleep(delay[i]);
                    else
                        Thread.Sleep(delay[i] - delay[i-1]);
                }
                else
                {
                    Thread.Sleep(delay[i]);
                }
                Console.WriteLine("Sending signal to CPU Pin: " + Convert.ToString(cpuPin[i]) + ", Value: " + Convert.ToString(value[i]));
                Form1.signalGpio(cpuPin[i], value[i]);
            }

        }

    }
}