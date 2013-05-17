using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using System.Threading;
using Samraksh.SPOT.Net.Mac;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware.EmoteDotNow;
using Samraksh.SPOT.RealTime;

namespace MakerFairController
{
    public delegate void ControlCallback (bool Direction);

    public class ControlLoop
    {
        System.Threading.Timer ControlTimer;
        static bool Timer_State;
        static OutputPort Output = new OutputPort((Cpu.Pin)25, true);        
        
        const int MaxDegree = 180;
        uint[] PulseHash = new uint[MaxDegree + 1]; //Width of pulse for each degree 0-180        
        uint CurrentPosition = 0;
        bool ZeroTo180 = true;
        int UserControl=0;
        bool Pin1State = false;
        PWM ServoPWM;
        uint PreemptiveGC=0;
        ushort StateCount = 0;
        uint testData;

        public ControlLoop()
        {
            for (int i = 0; i < MaxDegree+1; i++)
            {
                PulseHash[i] = 550 + (uint)(1900 * ((double)(180-i) / 180));
                //PulseHash[i] = 1000 + (uint)(1000 * i);
            }            
        }

		public void Start(){
            ControlTimer = new System.Threading.Timer(ControlTimerCallback, null, 20, 20);
            ServoPWM= new PWM();
            testData = 10000;
		}
        void ControlTimerCallback(Object o)
        {
            try{
            Timer_State = !Timer_State;
            Output.Write(Timer_State);
            Timer_State = !Timer_State;
            Output.Write(Timer_State);
        	
        	/*if(PreemptiveGC >=50){
        		Debug.GC(true);
        		PreemptiveGC=0;
        	}
        	PreemptiveGC++;*/

            /*if (testData < 400000)
                testData = testData + 10000;
            else
                testData = 10000;*/
            //Debug.Print("testdata: " + testData.ToString());
            //ServoPWM.GeneratePulse(testData);
        	Gravity();                	
            //Oscillate();
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
        }
        
        /// User pushes motor against gravity towards 180 degrees. If he stops pushing it comes back to zero
        ///in 3.6 secs
        void Gravity(){
            //Debug.Print("uc: " + UserControl.ToString() + " cp: " + CurrentPosition.ToString());
            if (UserControl > MaxDegree)
            {
                UserControl = MaxDegree;
            } else if (UserControl > 0)
            {
                UserControl = UserControl - 5;
                ServoPWM.GeneratePulse(PulseHash[CurrentPosition]);
        	} else 
            {
        		UserControl=0;
        		if(CurrentPosition > 0)
                { 
                    CurrentPosition--;
                    ServoPWM.GeneratePulse(PulseHash[CurrentPosition]);
                }                
        	}        	
        }
        
        ///Oscillates from 0-90-180-90-0 stoping at each postion for 2 secs
        /*void DiscreteOscillate(){
        	if(StateCount< 100){     		
        		StateCount++;
        	}else{
        		StateCount=0;
        		if (ZeroTo180){
        			CurrentPosition +=90;
        			 if (CurrentPosition == 180) ZeroTo180 = false;
        		}else {
        			CurrentPosition -=90;
        			 if (CurrentPosition == 0) ZeroTo180 = true;
        		}
        	}
        	ServoPWM.GeneratePulse(PulseHash[CurrentPosition]);
        }*/
        
        ///Oscillates from 0-180-0 continously 1degree each 20ms. 
        ///A complete oscillation from 0-180 and 180-0 takes 7.2 secs 
        void Oscillate()
        {
        	//if((Delay % 3)==0){
	            if (ZeroTo180)
	            {
	                CurrentPosition++;
	                if (CurrentPosition == MaxDegree) ZeroTo180 = false;
	            }
	            else
	            {
	                CurrentPosition--;
	                if (CurrentPosition == 0) ZeroTo180 = true;
	            }
            //}
            //Delay++;
            ServoPWM.GeneratePulse(PulseHash[CurrentPosition]);
        }
		
        /*void Move(bool direction)
        {
            if (direction) MoveRight();
            else MoveLeft();
        }
		
        public void MoveLeft()
        {
        	UserControl++;
        	CurrentPosition--;
			if(CurrentPosition < 0) CurrentPosition=0; 
        }
		
        public void MoveRight()
        {
        	//ControlPort.Write(true);ControlPort.Write(false);
        	UserControl++;;
			CurrentPosition++;
			if(CurrentPosition >=MaxDegree) CurrentPosition =(uint) MaxDegree;
        }*/
        public void Move(byte Position)
        {            
            UserControl=Position;
            if(Position > 0){
            	CurrentPosition += Position;
            }else {
            	//CurrentPosition -= (uint)-Position;
            }
            if (CurrentPosition >= MaxDegree){
            	CurrentPosition = 180;
            }else if (CurrentPosition < 0) {
            	CurrentPosition = 0;
            }
        }
    }
}
