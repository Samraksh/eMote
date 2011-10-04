/**
 * Leds class for iMote2 platform allows acctuation of LED output.
 * 
 * @file        Leds.cs
 * @author      Kartik Natarajan
 * 
 * @version     2010/12/16    kartikn     Initial version
 * 
 * Copyright (c) 2010 The Samraksh Company  All rights reserved.
 * 
 * 
 */

using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace Samraksh.platform.emote
{
    public enum LedColor
    {
        BLACK = 0x000000,
        RED = 0xFF0000,
        GREEN = 0x00FF00,
        BLUE = 0x0000FF,
        PURPLE = 0xFF00FF,
        CYAN = 0x00FFFF,
        YELLOW = 0xFFFF00,
        WHITE = 0xFFFFFF
    }

    public class Leds
    {
        private OutputPort LightBlue    = new OutputPort(Pins.GPIO_PORT_LED_BLUE, true);
        private OutputPort LightGreen   = new OutputPort(Pins.GPIO_PORT_LED_GREEN, true);
		private OutputPort LightOrange   = new OutputPort(Pins.GPIO_PORT_LED_ORANGE, true);
        private OutputPort LightRed     = new OutputPort(Pins.GPIO_PORT_LED_RED, true);

        /**
         * Sets LED to integer value from 0 to 7.
         * 
         * @version     2007/4/5    mturon     Initial version
         */
		/*public void set() {
            /*if (0 == (i & 0x02)) { 
				Debug.Print("In blue off.....");
                blueOff(); 
            } else {
				Debug.Print("In blue on.....");
                blueOn(); 
            }*/
            		 
        public void set(int i) {
            if (0 == (i & 0x01)) { 
                redOff(); 
            } else {
                redOn(); 
            }
            if (0 == (i & 0x02)) { 	
                blueOff(); 
            } else {				
                blueOn(); 
            }
            if (0 == (i & 0x04)) { 
                greenOff(); 
            } else {
                greenOn(); 
            }
        }

        /**
         * Sets LED to integer RGB value: 0x00RRGGBB.          
         */
        public void setRGB(int rgb)
        {
            if (0 == (rgb & 0x00FF0000))
            {
                redOff();
            }
            else
            {
                redOn();
            }
			
            if (0 == (rgb & 0x000000FF))
            {
                blueOff();
            }
            else
            {
                blueOn();
            }
			
            if (0 == (rgb & 0x0000FF00))
            {
                greenOff();
            }
            else
            {
                greenOn();
            }
			
        }

        public void setRGB(LedColor rgb)
        {
            setRGB((int)rgb);
        }

        public void blueOn()
        {
            LightBlue.Write(false); 
        }

        public void blueOff()
        {		
            LightBlue.Write(true);
        }

        public void redOn()
        {
            LightRed.Write(false);
        }

        public void redOff()
        {
            LightRed.Write(true);
        }

        public void greenOn()
        {
            LightGreen.Write(false);
        }

        public void greenOff()
        {
            LightGreen.Write(true);
        }
		public void orangeOn()
        {
            LightOrange.Write(false); 
        }

        public void orangeOff()
        {		
            LightOrange.Write(true);
        }
    }
}
