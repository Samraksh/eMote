using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace MakerFairSensor
{
    class RotaryEncoder
    {
        public InterruptPort EncoderBlack = new InterruptPort((Cpu.Pin)4, true, Port.ResistorMode.PullDown, Port.InterruptMode.InterruptEdgeHigh);
        //public InterruptPort EncoderWhite = new InterruptPort(EmotePins.GPIO_PIN_7, true, Port.ResistorMode.PullDown, Port.InterruptMode.InterruptEdgeBoth);
        //static OutputPort DebugPort1 = new OutputPort(EmotePins.GPIO_PIN_7, true);
        //static OutputPort DebugPortB = new OutputPort((Cpu.Pin)25, true);
        
        //static bool Inphase, Quadrature;
        static int RightTurn=0;
        //static int LeftTurn=0;
        const int ScalingFactor = 6;
        static EncoderCallback SendAction;

        public RotaryEncoder(EncoderCallback Action)
        {
//            SendAction = Action;
//            EncoderBlack.OnInterrupt += new NativeEventHandler(InPhaseHandler);
            //EncoderWhite.OnInterrupt += new NativeEventHandler(QuadratureHandler);
            //DebugPortB.Write(true);DebugPortB.Write(false);
            //DebugPortW.Write(true);DebugPortW.Write(false);
        }

        void InPhaseHandler(uint port, uint state, DateTime time)
        {
            Debug.Print("inphase handler");
            //DebugPortB.Write(true);DebugPortB.Write(false);
            /*Inphase=EncoderBlack.Read();    
            if(Inphase){
                if (Quadrature)
                    RightTurn++;
                else
                    LeftTurn++;
            }else {
                if (Quadrature)
                    LeftTurn++;
                else
                    RightTurn++;
            }*/
            //DebugPort2.Write(true);DebugPort2.Write(false);
            RightTurn++;
            //SendAction(true);
            CheckAction();
        }
        
       /* void QuadratureHandler(uint port, uint state, DateTime time)
        {
        	//DebugPortW.Write(true);DebugPortW.Write(false);
            Quadrature = EncoderWhite.Read(); 
            if (Quadrature)
            {
                if (Inphase)
                    LeftTurn++;
                else
                    RightTurn++;
            }
            else
            {
                if (Quadrature)
                    RightTurn++;
                else
                    LeftTurn++;
            }
            CheckAction();
            LeftTurn++;
        }*/
        
        static void CheckAction()
        {
            if (RightTurn == ScalingFactor)
            {
            	//DebugPort1.Write(true);DebugPort1.Write(false);
                RightTurn = 0;
                SendAction(true);
            }
            /*else {
            	//DebugPortW.Write(true);DebugPortW.Write(false);
            }
            if (LeftTurn == ScalingFactor)
            {
                LeftTurn = 0;
                ControlAction(false);
                DebugPortW.Write(true);DebugPortW.Write(false);
            }*/
        }
    }
    
}