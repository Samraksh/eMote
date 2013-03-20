using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Messaging;

namespace EmulatorTestApp
{
    public class EmulatorTest
    {
        InterruptPort _button1_InterruptPort, _button2_InterruptPort, _button3_InterruptPort;
        static OutputPort _led1_port, _led2_port, _led3_port;
        //Boolean variables to keep tract of the state of the buttons
        static bool button1_state, button2_state, button3_state;
        EmulatorTest(){

            //Instantiate the Output ports
            _led1_port = new OutputPort((Cpu.Pin)0, false);
            _led2_port = new OutputPort((Cpu.Pin)1, false);
            _led3_port = new OutputPort((Cpu.Pin)2, false);

            //Instantiate the interrupt ports
            _button1_InterruptPort = new InterruptPort((Cpu.Pin)3, false, Port.ResistorMode.PullDown, Port.InterruptMode.InterruptEdgeBoth);
            _button2_InterruptPort = new InterruptPort((Cpu.Pin)4, false, Port.ResistorMode.PullDown, Port.InterruptMode.InterruptEdgeBoth);
            _button3_InterruptPort = new InterruptPort((Cpu.Pin)5, false, Port.ResistorMode.PullDown, Port.InterruptMode.InterruptEdgeBoth);

            //Connect the interrupts to their handler methods
            _button1_InterruptPort.OnInterrupt += new NativeEventHandler(button1_OnInterrupt);
            _button2_InterruptPort.OnInterrupt += new NativeEventHandler(button2_OnInterrupt);
            _button3_InterruptPort.OnInterrupt += new NativeEventHandler(button3_OnInterrupt);

        }

        static void button1_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            //Toggle button1_state
            button1_state = !button1_state;
            _led1_port.Write(button1_state); //write the state to LED1
            Debug.Print("Button 1" + ((button1_state) ? "Down" : "Up"));
        }

        static void button2_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            //Toggle button2_state
            button2_state = !button2_state; 
             _led2_port.Write(button2_state); //write the state to LED2
             Debug.Print("Button 2" + ((button2_state) ? "Down" : "Up"));
        }

        static void button3_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            //Toggle button3_state
            button3_state = !button3_state;
            _led3_port.Write(button3_state); //write the state to LED3
            Debug.Print("Button 3" + ((button3_state) ? "Down" : "Up"));
        }

        //Application starts here
        public static void Main()
        {
            EmulatorTest e = new EmulatorTest();
            Thread.Sleep(Timeout.Infinite);            
        }

    }
}
