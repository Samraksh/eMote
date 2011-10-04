/**
 * Pins class maps GPIO lines for iMote2 platform.
 * 
 * @file        Pins.cs 
 * @author      Kartik Natarajan
 *
 * @version     2010/12/16    kartikn     Initial version
 * 
 * Copyright (c) 2010 The Samraksh Company  All rights reserved.
 * 
 */

using System;
using System.Collections;
using System.Threading;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.platform.emote
{
    public static class Pins
    {
        
        //Leaving as it is for format checking for adding more pins
		public const Cpu.Pin GPIO_PORT_PIN_100 = (Cpu.Pin)100;
		
        public const Cpu.Pin GPIO_PORT_LED_GREEN = (Cpu.Pin)86;
		public const Cpu.Pin GPIO_PORT_LED_ORANGE = (Cpu.Pin)87;
        public const Cpu.Pin GPIO_PORT_LED_RED = (Cpu.Pin)88;
		public const Cpu.Pin GPIO_PORT_LED_BLUE = (Cpu.Pin)89;
        
    }
    
	
	//Leaving as it is for extending it for UART and Radio in future, does not hurt!
    /// <include file='doc\SerialPort.uex' path='docs/doc[@for="Serial"]/*' />
    public static class Serial
    {
// DT_START
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="Serial.COM1"]/*' />
        //public const SerialPort.Serial COM1 = SerialPort.Serial.COM1;
        public const string COM1 = "COM1";
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="Serial.COM2"]/*' />
        //public const SerialPort.Serial COM2 = SerialPort.Serial.COM2;
        public const string COM2 = "COM2";
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="Serial.COM3"]/*' />
        //public const SerialPort.Serial COM3 = SerialPort.Serial.COM3;
        public const string COM3 = "COM3";
// DT_END
    }
    
    /// <include file='doc\SerialPort.uex' path='docs/doc[@for="BaudRate"]/*' />
    public static class BaudRate
    {
// DT_START
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="BaudRate.Baud19200"]/*'  />
        //public const SerialPort.BaudRate Baud19200  = SerialPort.BaudRate.Baud19200;
        public const System.IO.Ports.BaudRate Baud19200 = System.IO.Ports.BaudRate.Baudrate19200;
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="BaudRate.Baud38400"]/*'  />
        //public const SerialPort.BaudRate Baud38400  = SerialPort.BaudRate.Baud38400;
        public const System.IO.Ports.BaudRate Baud38400 = System.IO.Ports.BaudRate.Baudrate38400;
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="BaudRate.Baud57600"]/*'  />
        //public const SerialPort.BaudRate Baud57600  = SerialPort.BaudRate.Baud57600;
        public const System.IO.Ports.BaudRate Baud57600 = System.IO.Ports.BaudRate.Baudrate57600;
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="BaudRate.Baud115200"]/*' />
        //public const SerialPort.BaudRate Baud115200 = SerialPort.BaudRate.Baud115200;
        public const System.IO.Ports.BaudRate Baud115200 = System.IO.Ports.BaudRate.Baudrate115200;
        /// <include file='doc\SerialPort.uex' path='docs/doc[@for="BaudRate.Baud230400"]/*' />
        //public const SerialPort.BaudRate Baud230400 = SerialPort.BaudRate.Baud230400;
        public const System.IO.Ports.BaudRate Baud230400 = System.IO.Ports.BaudRate.Baudrate230400;
// DT_END
    }

    public static class ResistorMode
    {
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="ResistorMode.PullUp"]/*'    />
        public const Port.ResistorMode  PullUp  = Port.ResistorMode.PullUp;
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="ResistorMode.Disabled"]/*'  />
        public const Port.ResistorMode Disabled = Port.ResistorMode.Disabled;
    }

    public static class InterruptMode
    {
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="InterruptMode.InterruptEdgeLow"]/*'       />
        public const Port.InterruptMode InterruptEdgeLow       = Port.InterruptMode.InterruptEdgeLow ;
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="InterruptMode.InterruptEdgeHigh"]/*'      />
        public const Port.InterruptMode InterruptEdgeHigh      = Port.InterruptMode.InterruptEdgeHigh;
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="InterruptMode.InterruptEdgeBoth"]/*'      />
        public const Port.InterruptMode InterruptEdgeBoth      = Port.InterruptMode.InterruptEdgeBoth;
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="InterruptMode.InterruptEdgeLevelHigh"]/*' />
        public const Port.InterruptMode InterruptEdgeLevelHigh = Port.InterruptMode.InterruptEdgeLevelHigh;
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="InterruptMode.InterruptEdgeLevelLow"]/*'  />
        public const Port.InterruptMode InterruptEdgeLevelLow  = Port.InterruptMode.InterruptEdgeLevelLow;
        /// <include file='doc\IOPorts.uex' path='docs/doc[@for="InterruptMode.InterruptNone"]/*'          />
        public const Port.InterruptMode InterruptNone          = Port.InterruptMode.InterruptNone;
    }

}

