using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace EmoteEmulatorButtonTest
{
    public class ButtonTest
    {
        InterruptPort _button1_InterruptPort, _button2_InterruptPort, _button3_InterruptPort;
        static OutputPort _led1_port, _led2_port, _led3_port;
        static bool button1_state, button2_state, button3_state;

        public ButtonTest()
        {
            _button1_InterruptPort = new InterruptPort((Cpu.Pin)9, false, Port.ResistorMode.PullDown, Port.InterruptMode.InterruptEdgeBoth);
            _button2_InterruptPort = new InterruptPort((Cpu.Pin)10, false, Port.ResistorMode.PullDown, Port.InterruptMode.InterruptEdgeBoth);
            _button3_InterruptPort = new InterruptPort((Cpu.Pin)22, false, Port.ResistorMode.PullDown, Port.InterruptMode.InterruptEdgeBoth);

            _button1_InterruptPort.OnInterrupt += new NativeEventHandler(button1_OnInterrupt);
            _button2_InterruptPort.OnInterrupt += new NativeEventHandler(button2_OnInterrupt);
            _button3_InterruptPort.OnInterrupt += new NativeEventHandler(button3_OnInterrupt);

            //_led1_port = new OutputPort((Cpu.Pin)21, false);
            //_led2_port = new OutputPort((Cpu.Pin)4, false);
            //_led3_port = new OutputPort((Cpu.Pin)38, false);
          
            _led1_port = new OutputPort((Cpu.Pin)1, false);
            _led2_port = new OutputPort((Cpu.Pin)2, false);
            _led3_port = new OutputPort((Cpu.Pin)3, false);

            
        }

        public static void Main()
        {
            //Debug.Print(Resources.GetString(Resources.StringResources.String1));
            new ButtonTest();
            Thread.Sleep(Timeout.Infinite);
        }

        static void button1_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            if (button1_state) button1_state = false; else button1_state = true;

            _led1_port.Write(button1_state);
            Debug.Print("Button 1" + ((button1_state) ? "Down" : "Up"));
        }

        static void button2_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            if (button2_state) button2_state = false; else button2_state = true;
            _led2_port.Write(button2_state);
            Debug.Print("Button 2" + ((button2_state) ? "Down" : "Up"));
        }

        static void button3_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            if (button3_state) button3_state = false; else button3_state = true;
            _led3_port.Write(button3_state);
            Debug.Print("Button 3" + ((button3_state) ? "Down" : "Up"));
        }

       
    }
}
