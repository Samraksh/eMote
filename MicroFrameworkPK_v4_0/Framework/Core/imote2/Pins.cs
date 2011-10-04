/**
 * Pins class maps GPIO lines for iMote2 platform.
 * 
 * @file        Pins.cs
 * @author      Microsoft
 * @author      Martin Turon
 * 
 * @version     2007/4/5    mturon     Initial revision
 * 
 * Copyright (c) 2007 Crossbow Technology, Inc.   All rights reserved.
 * 
 * $Id: Pins.cs,v 1.3 2007/06/07 01:20:59 nxu Exp $
 */

using System;
using System.Collections;
using System.Threading;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Crossbow.platform.imote2
{

    /// <summary>
    /// Specifies identifiers for hardware I/O pins.
    /// </summary>
    public static class Pins
    {
        /// <summary>
        /// GPIO port pin 0.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_CC2420_FIFOP = (Cpu.Pin)0;
        /// <summary>
        /// GPIO port pin 11.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_11 = (Cpu.Pin)11;
        /// <summary>
        /// GPIO port pin 16.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_CC2420_SFD = (Cpu.Pin)16;
        /// <summary>
        /// GPIO port pin 22.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_CC2420_RESETN = (Cpu.Pin)22;
        /// <summary>
        /// GPIO port pin 23.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_23 = (Cpu.Pin)23;
        /// <summary>
        /// GPIO port pin 25.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_25 = (Cpu.Pin)25;
        /// <summary>
        /// GPIO port pin 26.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_26 = (Cpu.Pin)26;
        /// <summary>
        /// GPIO port pin 34.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_34 = (Cpu.Pin)34;
        /// <summary>
        /// GPIO port pin 35.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_35 = (Cpu.Pin)35;
        /// <summary>
        /// GPIO port pin 36.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_36 = (Cpu.Pin)36;
        /// <summary>
        /// GPIO port pin 38.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_38 = (Cpu.Pin)38;
        /// <summary>
        /// GPIO port pin 39.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_39 = (Cpu.Pin)39;
        /// <summary>
        /// GPIO port pin 41.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_41 = (Cpu.Pin)41;
        /// <summary>
        /// GPIO port pin 42.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_42 = (Cpu.Pin)42;
        /// <summary>
        /// GPIO port pin 43.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_43 = (Cpu.Pin)43;
        /// <summary>
        /// GPIO port pin 46.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_46 = (Cpu.Pin)46;
        /// <summary>
        /// GPIO port pin 47.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_47 = (Cpu.Pin)47;
        /// <summary>
        /// GPIO port pin 96.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_96 = (Cpu.Pin)96;
        /// <summary>
        /// GPIO port pin 98.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_98 = (Cpu.Pin)98;
        /// <summary>
        /// GPIO port pin 99.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_99 = (Cpu.Pin)99;
        /// <summary>
        /// GPIO port pin 100.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_100 = (Cpu.Pin)100;
        /// <summary>
        /// GPIO port pin 103.
        /// </summary>
		public const Cpu.Pin GPIO_PORT_LED_GREEN = (Cpu.Pin)86;//Changed KN....
		public const Cpu.Pin GPIO_PORT_LED_ORANGE = (Cpu.Pin)87;//Changed KN....
        public const Cpu.Pin GPIO_PORT_LED_RED = (Cpu.Pin)88;//Changed KN....
		public const Cpu.Pin GPIO_PORT_LED_BLUE = (Cpu.Pin)89; //Changed KN....
        /// <summary>       
        public const Cpu.Pin GPIO_PORT_CC2420_FIFO = (Cpu.Pin)114;
        /// <summary>
        /// GPIO port pin 115.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_CC2420_VREG_EN = (Cpu.Pin)115;
        /// <summary>
        /// GPIO port pin 116.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_CC2420_CCA = (Cpu.Pin)116;
        /// <summary>
        /// GPIO port pin 117.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_117 = (Cpu.Pin)117;
        /// <summary>
        /// GPIO port pin 118.
        /// </summary>
        public const Cpu.Pin GPIO_PORT_PIN_118 = (Cpu.Pin)118;
        public const Cpu.Pin GPIO_NONE = Cpu.Pin.GPIO_NONE;

        /// <summary>
        /// GPIO port pin 24.  SSP1_SFRM (cs)
        /// </summary>
        public const Cpu.Pin GPIO_PORT_SSP1_SFRM = (Cpu.Pin)24;
    }
    
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

