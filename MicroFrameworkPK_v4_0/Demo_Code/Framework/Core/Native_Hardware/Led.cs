using System;
using System.Collections;
using System.Threading;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Microsoft.SPOT.Hardware
{
	public class Led
	{
        private OutputPort LightBlue = new OutputPort(Pins.GPIO_PORT_LED_BLUE, true);

        public void set(int i)
        {
            if (1 == i)
            {
                blueOn();
            }
            else
            {
                blueOff();
            }

        }

        public void blueOn()
        {
            LightBlue.Write(false);
        }

        public void blueOff()
        {
            LightBlue.Write(true);
        }
	}
}