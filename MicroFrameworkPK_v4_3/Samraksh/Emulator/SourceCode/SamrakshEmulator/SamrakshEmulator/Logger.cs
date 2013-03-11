using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace SamrakshEmulator
{
    class Logger : TraceListener
    {
        public static System.IO.StreamWriter logFile;
        static Form1 _f1;
        static bool Initialized;
        //TextWriter _writer;
        public Logger()
        {
            if (!Initialized) Initialize();
        }
        public Logger(Form1 f1)
        {
            _f1 = f1;
            if (!Initialized) Initialize();
        }
        public void Initialize()
        {
            DateTime now = DateTime.Now;
            String nowString = now.ToString("yyyy_MM_dd_H_mm");
            logFile = new System.IO.StreamWriter("EmulatorLog_" + nowString + ".txt");
            Initialized = true;
            //_writer = new TextWriter 
        }

        public void Debug(Module m, String s)
        {
            DateTime now = DateTime.Now;
            String nowString = now.ToString("H:mm:ss.fff");
            logFile.WriteLine(nowString + " :: " + m + " :: " + s);
            logFile.Flush();
            if (_f1 != null)
                _f1.DisplayStatus(nowString + " :: " + m + " :: " + s + "\r\n");
        }
        override public void Close()
        {
            //logFile.Flush();
            logFile.Close();
        }
        override public void Write(String s)
        {
            Debug(Module.MFAPP, s);
        }
        override public void WriteLine(String s)
        {
            Debug(Module.MFAPP, s);
        }

    }

}
