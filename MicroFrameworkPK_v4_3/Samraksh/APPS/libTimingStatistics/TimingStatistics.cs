using System;
using Microsoft.SPOT;

namespace edu.osu.stemsensors
{
    //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
    //  Timing Statistics
    //  Michael McGrath, mcgrathm@cse.ohio-state.edu 2013-02-06
    //  TimingStatistics records timestamps for periodic events and computes
    //  statistics about the event intervals when Print() is called.
    //  Configure statistics by setting statConfig.
    //  Configure print statements by setting printConfig.
    //  "OnlineStatistics" algorithm outlined by Knuth.
    //  "WindowStatistics" algorithm outlined by Hadstate on Comp.DSP

    /// <summary>
    /// provides moving average, moving variance for a signal.
    /// </summary>
    public class WindowStatistics
    {
        int N;
        long[] sumX;
        long[] sumXsquared;
        int index;
        int prevIndex;

        public WindowStatistics(int windowSize) {
            N = 0;
            try {
                sumX = new long[windowSize];
                sumXsquared = new long[windowSize];
            }
            catch (OutOfMemoryException e) {
                Debug.Print("can't allocate for window stats.  use a smaller array. " + e);
            }
            index = windowSize - 1;
            prevIndex = windowSize - 2;
        }

        public void Put(long X) {
            ++index;
            ++prevIndex;
            if (index == sumX.Length) {
                index = 0;
            }
            else if (index == 1) {
                prevIndex = 0;
            }

            long Xsquared = X * X;
            sumX[index] = sumX[prevIndex] + X - sumX[index];
            sumXsquared[index] = sumXsquared[prevIndex] + Xsquared - sumXsquared[index];

            if (N < sumX.Length) {
                ++N;
            }
        }

        /// <summary>
        /// Average of last N points.
        /// </summary>
        public long Mean {
            get {
                if (N > 0) {
                    return (sumX[index] / N);
                }
                return 0;
            }
        }

        /// <summary>
        /// Variance of last N points.
        /// </summary>
        public long Variance {
            get {
                if (N > 2) {
                    return (N * sumXsquared[index] - (sumX[index] * sumX[index])) / (N * (N - 1));
                }
                return 0;
            }
        }

    }


    /// <summary>
    /// keep track of simple statistics.
    /// </summary>
    public class GenericStatistics
    {
        public long n;
        public long min;
        public long max;
        public long sum;
        public long sumSquared;

        public GenericStatistics() {
            n = 0;
            min = long.MaxValue;
            max = long.MinValue;
            sum = 0;
            sumSquared = 0;
        }

        public void Put(long x) {
            ++n;
            if (x < min) { min = x; }
            if (x > max) { max = x; }
            sum += x;
            sumSquared += x * x;
        }

        public void Put(long[] array, int head, int length) {
            long x;  // placing declaration outside of loop is negligible time savings.
            int last = head + length;
            for (int itr = head; itr < last; ++itr) {
                x = array[itr];
                if (x < min) { min = x; }
                if (x > max) { max = x; }
                sum += x;
                sumSquared += ((uint)x * (uint)x);
            }
            n += (uint)length;
        }

        public void Put(GenericStatistics obj) {
            n += obj.n;
            if (obj.min < min) { min = obj.min; }
            if (obj.max > max) { max = obj.max; }
            sum += obj.sum;
            sumSquared += obj.sumSquared;
        }

        public long Mean {
            get {
                if (n == 0) { return 0; }
                return sum / n;
            }
        }

        public long Variance {
            get {
                if (n == 0) { return 0; }
                return (sumSquared - ((sum * sum) / n)) / n;
            }
        }

        public long UnbiasedVariance {
            get {
                return (sumSquared - ((sum * sum) / n)) / (n - 1);
            }
        }
    }

    /// <summary>
    /// keep track of simple statistics.
    /// </summary>
    public class GenericStatistics_ushort
    {
        public uint n;
        public ushort min;
        public ushort max;
        public ushort sum;
        public uint sumSquared;

        public GenericStatistics_ushort() {
            n = 0;
            min = ushort.MaxValue;
            max = ushort.MinValue;
            sum = 0;
            sumSquared = 0;
        }

        public void Put(ushort[] array) {
            Put(array, 0, array.Length);
        }

        public void Put(ushort[] array, int head, int length) {
            ushort x;  // placing declaration outside of loop is negligible time savings.
            int last = head + length;
            for (int itr = head; itr < last; ++itr) {
                x = array[itr];
                if (x < min) { min = x; }
                if (x > max) { max = x; }
                sum += x;
                sumSquared += ((uint)x * (uint)x);
            }
            n += (uint)length;
        }

        public void Put(ushort x) {
            ++n;
            if (x < min) { min = x; }
            if (x > max) { max = x; }
            sum += x;
            sumSquared += ((uint)x * (uint)x);
        }

        public void Put(GenericStatistics_ushort obj) {
            n += obj.n;
            if (obj.min < min) { min = obj.min; }
            if (obj.max > max) { max = obj.max; }
            sum += obj.sum;
            sumSquared += obj.sumSquared;
        }

        public long Mean {
            get {
                if (n == 0) { return 0; }
                return sum / n;
            }
        }

        public long Variance {
            get {
                if (n == 0) { return 0; }
                return (sumSquared - ((sum * sum) / n)) / n;
            }
        }

        public long UnbiasedVariance {
            get {
                return (sumSquared - ((sum * sum) / n)) / (n - 1);
            }
        }
    }


    /// <summary>
    /// Online Mean and Variance based on Knuth/Welford
    /// numerically stable.  less loss of precision due to cancellation.
    /// not as efficient due to division at every step.
    /// TODO: Two-Pass version that first computes the mean and then runs this with mean as input.
    /// </summary>
    public class OnlineStatistics
    {
        long n;
        long mean;
        long M2;
        long delta;

        public OnlineStatistics() {
            n = 0;
            mean = 0;
            M2 = 0;
            delta = 0;
        }

        /// <summary>
        /// Add data point.
        /// </summary>
        /// <param name="x">New interval ticks.</param>
        public void Put(long x) {
            n = n + 1;
            delta = x - mean;
            mean = mean + delta / n;
            M2 = M2 + delta * (x - mean);
        }

        /// <summary>
        /// Current estimate of mean.
        /// </summary>
        /// <returns>estimate of mean given all inputs so far.</returns>
        public long Mean {
            get {
                return mean;
            }
        }

        /// <summary>
        /// Current estimate of the variance.
        /// </summary>
        /// <returns>estimate of the variance given all inputs so far.</returns>
        public long Variance {
            get {
                return M2 / (n - 1);
            }
        }

    }


    /// <summary>
    /// Collect and print statistics on periodic events.
    /// First, collect timing information by calling TakePeriodicSample().
    /// Then, after enough samples have been grabbed, call Print().
    /// Customize printed message by setting printConfig = PrintFlags | PrintFlags;
    /// Michael McGrath, mcgrathm@cse.ohio-state.edu, 2013-02-05
    /// </summary>
    public class TimingStatistics
    {
        /// <summary>
        /// binary OR these together to set verbosity of output.
        /// </summary>
        public enum PrintFlags : uint
        {
            HEADER = 0x0001,
            CUR_AVG = 0x0002,
            ALL_AVG = 0x0004,
            CUR_MIN = 0x0008,
            ALL_MIN = 0x0010,
            JITTER = 0x0020,
            VARIANCE = 0x0040,
            PRINTTIME = 0x0080,
            ONLINE = 0x0100,
            WINDOW = 0x0200,
            SHORT_HEADER = 0xF000,
            SHORT_JITTER = 0x8000
        }

        /// <summary>
        /// What stats to print out.
        /// Default: nothing, set by constructors.
        /// </summary>
        PrintFlags printConfig = 0x0000;
        /// <summary>
        /// What stats to print out.
        /// Default: nothing, set by constructors.
        /// </summary>
        public PrintFlags PrintConfig {
            get {
                return printConfig;
            }
            set {
                printConfig = value;
                MapPrintConfigToStatConfig();
            }
        }

        /// <summary>
        /// How many time readings to store.
        /// </summary>
        public int capacity;
        /// <summary>
        /// array to store event timestamps.  Buffer between calls to Print.
        /// </summary>
        public System.DateTime[] dt;
        /// <summary>
        /// index where next event timestamp will be noted.
        /// </summary>
        public int dtWriteIndex;
        /// <summary>
        /// Last index read from newStats calculation.
        /// </summary>
        public int dtReadIndex;
        /// <summary>
        /// number of times the statistics have been reset since creation of time statistics object.
        /// </summary>
        public int epochs;
        /// <summary>
        /// number of time intervals sampled.  TODO: make this number of items computed N.
        /// </summary>
        public int samples;
        /// <summary>
        /// timestamp when the time statistics object was created.
        /// </summary>
        public System.DateTime startTime;
        /// <summary>
        /// timestamp of the last reset (start of epoch).
        /// </summary>
        public System.DateTime resetTime;
        /// <summary>
        /// count of all messages sent by Print(obj)
        /// </summary>
        public int PrintMessageCount;
        /// <summary>
        /// name to identify messages sent by this time statistics object.
        /// </summary>
        public string name;

        /// <summary>
        /// Keep track of which statistics to be using.
        /// Choices are currently hard-coded.
        /// </summary>
        StatFlags statConfig = 0x0000;
        public StatFlags StatConfig {
            get {
                return statConfig;
            }
            set {
                InitializeNewStats(value);  //start any new stats.
                statConfig = value;
                StopUnusedStats();
            }
        }

        /// <summary>
        /// state machine to prevent finishing an interval when reset occurs mid-interval.
        /// </summary>
        public bool intervalStarted = false;

        /// <summary>
        /// Sets sampling type, which determines how timestamps are calculated.
        /// Not checked when taking samples.
        /// </summary>
        public bool PeriodicSamples;

        /// <summary>
        /// Flags to determine which statistic collecting code needs to be executed.
        /// </summary>
        public enum StatFlags : uint
        {
            CURRENT = 0x0001,
            OVERALL = 0x0002,    //OVERALL REQUIRES CURRENT.
            ONLINE = 0x0004,
            WINDOW = 0x0008,
        }


        OnlineStatistics onlineStats;        /// Knuth online algorithm.
        WindowStatistics windowStats;        /// Window of last N samples.
        GenericStatistics overallStats;   /// overall MinMaxMeanVar

        /// <summary>
        /// Creates new interval statistics tracker with desired debug message settings.
        /// </summary>
        /// <param name="capacity">maximum number of samples per epoch</param>
        /// <param name="name">identifier for this timer in debug message header.</param>
        /// <param name="printConfig">desired print flags.</param>
        public TimingStatistics(int capacity, string name, PrintFlags printConfig, bool isPeriodic) {
            this.printConfig = printConfig;
            MapPrintConfigToStatConfig();  // will init stats.
            InitializeClass(capacity, name, isPeriodic);
        }


        /// <summary>
        /// Map a print configuration to needed stat flags.
        /// Print flags may be reduced by user after object is created without affecting pre-existing statistics collections.
        /// Call this to ensure Stats will be populated when changing a print flag if not sure.
        /// </summary>
        /// <returns>whether any stats were reinitialized.</returns>
        bool MapPrintConfigToStatConfig() {
            StatFlags newConfig = 0x0000;
            if ((printConfig & (PrintFlags.CUR_AVG | PrintFlags.CUR_MIN)) > 0) {
                newConfig =  StatFlags.CURRENT;
            }
            if ((printConfig & (PrintFlags.ALL_AVG | PrintFlags.ALL_MIN | PrintFlags.SHORT_JITTER | PrintFlags.JITTER | PrintFlags.VARIANCE)) > 0) {
                newConfig = newConfig | StatFlags.OVERALL;
            }
            if ((printConfig & PrintFlags.ONLINE) > 0) {
                newConfig = newConfig | StatFlags.ONLINE;
            }
            if ((printConfig & PrintFlags.WINDOW) > 0) {
                newConfig = newConfig | StatFlags.WINDOW;
            }
            if ((newConfig ^ statConfig) != 0) {
                InitializeNewStats(newConfig); //only init new stats, call before setting statconfig.
                return true;
            }
            return false;
        }


        /// <summary>
        /// Creates new interval statistics tracker with desired statistics logging settings (and all print options possible).
        /// StatFlags is in a different position so compiler can distinguish between ctors via arguments.
        /// </summary>
        /// <param name="capacity">maximum number of samples per epoch</param>
        /// <param name="name">identifier for this timer in debug message header.</param>
        /// <param name="statConfig">desired stat logging.</param>
        public TimingStatistics(int capacity, StatFlags statConfig, string name, bool isPeriodic) {
            this.statConfig = statConfig;
            MapStatConfigToPrintConfig();
            InitializeStats();  //need to call here because MapStatConfigToPrintConfig doesn't init stats.
            InitializeClass(capacity, name, isPeriodic);
        }


        /// <summary>
        /// Map a stat configuration to all available print flags.
        /// Print flags may be reduced by user after object is created.
        /// </summary>
        void MapStatConfigToPrintConfig() {
            if ((statConfig & StatFlags.CURRENT) > 0) {
                printConfig = printConfig | PrintFlags.HEADER | PrintFlags.CUR_AVG | PrintFlags.CUR_MIN;
            }
            if ((statConfig & StatFlags.OVERALL) > 0) { //need mask 0x02 when checking overall so current doesn't catch overall.
                printConfig = printConfig | PrintFlags.HEADER | PrintFlags.ALL_AVG | PrintFlags.ALL_MIN | PrintFlags.JITTER;
            }
            if ((statConfig & (StatFlags.CURRENT | StatFlags.OVERALL)) == 0) {
                printConfig = printConfig | PrintFlags.SHORT_HEADER;
            }
            if ((statConfig & StatFlags.ONLINE) > 0) {
                printConfig = printConfig | PrintFlags.ONLINE;
            }
            if ((statConfig & StatFlags.WINDOW) > 0) {
                printConfig = printConfig | PrintFlags.WINDOW;
            }
        }


        void InitializeClass(int capacity, string name, bool isPeriodic) {
            this.capacity = capacity;
            dt = new System.DateTime[capacity];
            startTime = System.DateTime.Now;
            PrintMessageCount = 0;
            this.name = name;
            this.PeriodicSamples = isPeriodic;
            Reset(); //this will not re-init stats if print config matches stat config.
            epochs = 0; //assign after reset to start epoch at 0.
        }

        /// <summary>
        /// Resets all active time sampling stats and increments the epoch counter.
        /// </summary>
        public void Reset() {
            ++epochs;
            InitializeStats();
            ResetIntervalBuffer();
        }


        /// <summary>
        /// Resetting the Interval Buffer doesn't affect cumulative statistics.
        /// </summary>
        public void ResetIntervalBuffer() {
            dtWriteIndex = 0;
            dtReadIndex = 0;
            resetTime = System.DateTime.Now;
            intervalStarted = false;
        }

        /// <summary>
        /// Resets ALL requested statistics (except CURRENT which is reset each go-round anyway.)
        /// </summary>
        void InitializeStats() {
            if ((statConfig & StatFlags.OVERALL) > 0) { overallStats = new GenericStatistics(); }
            if ((statConfig & StatFlags.ONLINE) > 0) { onlineStats = new OnlineStatistics(); }
            if ((statConfig & StatFlags.WINDOW) > 0) { windowStats = new WindowStatistics(capacity); }
        }

        /// <summary>
        /// ONLY resets new statistics that are not running previously.
        /// Updates statConfig with newConfig with now-running stats.
        /// </summary>
        /// <param name="newConfig">stats to be added to collection.</param>
        public void InitializeNewStats(StatFlags newConfig) {
            if ((((statConfig & StatFlags.OVERALL) ^ (newConfig & StatFlags.OVERALL)) & (newConfig & StatFlags.OVERALL)) > 0) { overallStats = new GenericStatistics(); statConfig = statConfig | StatFlags.OVERALL; }
            if ((((statConfig & StatFlags.ONLINE) ^ (newConfig & StatFlags.ONLINE)) & (newConfig & StatFlags.ONLINE)) > 0) { onlineStats = new OnlineStatistics(); statConfig = statConfig | StatFlags.ONLINE; }
            if ((((statConfig & StatFlags.WINDOW) ^ (newConfig & StatFlags.WINDOW)) & (newConfig & StatFlags.WINDOW)) > 0) { windowStats = new WindowStatistics(capacity); statConfig = statConfig | StatFlags.WINDOW;  }
        }

        /// <summary>
        /// Stops stats collection on stats that are no longer needed.
        /// </summary>
        void StopUnusedStats() {
            if ((statConfig & StatFlags.OVERALL) == 0) { overallStats = null; }
            if ((statConfig & StatFlags.ONLINE) == 0) { onlineStats = null; }
            if ((statConfig & StatFlags.WINDOW) == 0) { windowStats = null; }
        }

        /// <summary>
        /// record a time for later interval analysis when Print is called.
        /// </summary>
        public void TakePeriodicSample() {
            dt[dtWriteIndex] = System.DateTime.Now;
            ++dtWriteIndex;
            if (dtWriteIndex == dt.Length) {
                dtWriteIndex = 0;
            }
        }

        /// <summary>
        /// record start time
        /// </summary>
        public void StartInvervalSample() {
            dt[dtWriteIndex] = System.DateTime.Now;
            intervalStarted = true;
        }

        /// <summary>
        /// commit stop time and advance write pointer.
        /// </summary>
        public void StopIntervalSample() {
            if (intervalStarted == true) {
                dt[++dtWriteIndex] = System.DateTime.Now;
                ++dtWriteIndex;
                if (dtWriteIndex == dt.Length) {
                    dtWriteIndex = 0;
                }
            }
            intervalStarted = false;
        }

        /// <summary>
        /// Prints various stats to Debug.
        /// Default: print verbose statistics.  May take 180ms to complete.
        /// Custom: set printConfig member equal to binary OR of individual flags.
        /// This way you can decide how much to print out, say, at beginning of program or event.
        /// </summary>
        /// <param name="obj">TimeStatistics object you want printed to the Debug interface</param>
        public static void Print(TimingStatistics obj) {
            System.DateTime printStart = System.DateTime.Now;
            System.TimeSpan ts = printStart - obj.startTime;

            GenericStatistics newStats = new GenericStatistics();

            int increment = 2;
            if (obj.PeriodicSamples) {
                increment = 1;
            }
            int lastIndex = obj.dtWriteIndex - 1;
            if (obj.dtReadIndex > obj.dtWriteIndex) { // do continuous wrap-around, but haven't implemented yet.
                lastIndex = obj.capacity;
            }
            int itr = obj.dtReadIndex;
            if ((obj.statConfig & (StatFlags.CURRENT | StatFlags.OVERALL)) > 0) {
                for (itr = obj.dtReadIndex; itr < lastIndex - 1; itr += increment) {
                    long intervalTicks = (obj.dt[itr + 1].Ticks - obj.dt[itr].Ticks);
                    newStats.Put(intervalTicks);
                }
            }
            if ((obj.statConfig & StatFlags.ONLINE) > 0) {
                for (itr = obj.dtReadIndex; itr < lastIndex - 1; itr += increment) {
                    long intervalTicks = (obj.dt[itr + 1].Ticks - obj.dt[itr].Ticks);
                    obj.onlineStats.Put(intervalTicks);
                }
            }
            if ((obj.statConfig & StatFlags.WINDOW) > 0) {
                for (itr = obj.dtReadIndex; itr < lastIndex - 1; itr += increment) {
                    long intervalTicks = (obj.dt[itr + 1].Ticks - obj.dt[itr].Ticks);
                    obj.windowStats.Put(intervalTicks);
                }
            }
            obj.dtReadIndex = itr;

            // insert into stat choices that accept bulk input.
            if ((obj.statConfig & StatFlags.OVERALL ) > 0) { obj.overallStats.Put(newStats); }


            obj.samples += itr;

            TimeSpan epochTime = printStart - obj.resetTime;

            ++obj.PrintMessageCount;
            if ((obj.printConfig & TimingStatistics.PrintFlags.SHORT_HEADER) > 0) {
                Debug.Print(""+obj.name + obj.PrintMessageCount);
            }
            if ((obj.printConfig & TimingStatistics.PrintFlags.HEADER) > 0) {
                Debug.Print("TimingStatistics(" + obj.name + ":" + obj.PrintMessageCount + ")   " + System.TimeSpan.TicksPerMillisecond + "Ticks/sec " + " CPUFreq: " + Microsoft.SPOT.Hardware.Cpu.SystemClock + "Hz" +
                            "\n\t Elapsed: " + ts.Minutes + ":" + ts.Seconds + ":" + ts.Milliseconds +
                            "\n\t Epoch: # " + obj.epochs + " ;  " + epochTime.Minutes + "m:" + epochTime.Seconds +"s:" + epochTime.Milliseconds + "ms  in this epoch." +
                            "\n\t Total Interval Samples: " + obj.samples
                            );
            }
            try {
                if ((obj.printConfig & TimingStatistics.PrintFlags.CUR_AVG) > 0) {
                    TimeSpan curAvgSpan = TimeSpan.FromTicks(newStats.Mean);
                    Debug.Print("\t Average(" + newStats.n + "): " + newStats.Mean + " Ticks; " + curAvgSpan.Minutes + "m:" + curAvgSpan.Seconds + "s:" + curAvgSpan.Milliseconds + "ms");
                }
                if ((obj.printConfig & TimingStatistics.PrintFlags.ALL_AVG) > 0) {
                    TimeSpan allAvgSpan = TimeSpan.FromTicks(obj.overallStats.Mean);
                    Debug.Print("\t Average(" + obj.overallStats.n + "): " + obj.overallStats.Mean + " Ticks; " + allAvgSpan.Minutes + "m:" + allAvgSpan.Seconds + "s:" + allAvgSpan.Milliseconds + "ms");
                }
                if ((obj.printConfig & TimingStatistics.PrintFlags.CUR_MIN) > 0) {
                    Debug.Print("\t Min(" + newStats.n + "): " + newStats.min + " Ticks; \t Max(" + newStats.n + "): " + newStats.max + " Ticks. \t Range: " + (newStats.max - newStats.min) + " Ticks.");
                }
                if ((obj.printConfig & TimingStatistics.PrintFlags.ALL_MIN) > 0) {
                    Debug.Print("\t Min(" + obj.overallStats.n + "): " + obj.overallStats.min + " Ticks; \t Max(" + obj.overallStats.n + "): " + obj.overallStats.max + " Ticks. \t Range: " + (obj.overallStats.max - obj.overallStats.min) + " Ticks.");
                }
                if ((obj.printConfig & TimingStatistics.PrintFlags.SHORT_JITTER) > 0) {
                    Debug.Print("\t" + (newStats.min - obj.overallStats.Mean) + "|" + (newStats.Mean - obj.overallStats.Mean) + "|+" + (newStats.Mean - obj.overallStats.Mean)); //sorry not quite correct.
                }
                if ((obj.printConfig & TimingStatistics.PrintFlags.JITTER) > 0) {
                    Debug.Print("\t Jitter(" + newStats.n + "): " + (newStats.min - obj.overallStats.Mean) + "|" + (newStats.Mean - obj.overallStats.Mean) + "|+" + (newStats.max - obj.overallStats.Mean) + " Ticks. (min(" + newStats.n + ")-avg(" + obj.overallStats.n + ")|avg(" + newStats.n + ")-avg(" + obj.overallStats.n + ")|max(" + newStats.n + ")-avg(" + obj.overallStats.n + "))"); //sorry not quite correct.
                }
                if ((obj.printConfig & TimingStatistics.PrintFlags.VARIANCE) > 0) {
                    Debug.Print("\t Variance(" + newStats.n + "): " + newStats.Variance + "\t Variance(" + obj.overallStats.n + "): " + obj.overallStats.Variance + " Ticks.");
                }
                if ((obj.printConfig & TimingStatistics.PrintFlags.ONLINE) > 0) {
                    Debug.Print("\t Online: Mean:" + obj.onlineStats.Mean + "\t Variance: " + obj.onlineStats.Variance + " Ticks.");
                }
                if ((obj.printConfig & TimingStatistics.PrintFlags.WINDOW) > 0) {
                    Debug.Print("\t Window: Mean:" + obj.windowStats.Mean + "\t Variance: " + obj.windowStats.Variance + " Ticks.");
                }
                if ((obj.printConfig & TimingStatistics.PrintFlags.PRINTTIME) > 0) {
                    ts = System.DateTime.Now - printStart;
                    Debug.Print("Print time: " + ts.Ticks + " Ticks. " + ts.Milliseconds + "ms.");
                }
                if (lastIndex == obj.capacity) {
                    // Different values for each epoch- not moving averages.
                    obj.ResetIntervalBuffer();  // call this to forget samples taken during print function, may introduce jitter.
                }
            }
            catch (NullReferenceException e) {
                Debug.Print("Stats requested that could not be printed! " + e);
            }
        }

    }


}
