using System;

namespace ParameterClass
{
    class Parameters
    {
	// required TestRig parameters	
	// parameters used to gather data
	public int testTimeout = 20000;
	public string useLogic = "none";
	public double sampleTimeMs = 15000;
	public double sampleFrequency = 4000000;
	public bool useCOMPort = false;
	public string forceCOM = "";
	public string COMParameters = "115200,N,8,1";
	public bool useTestScript = true;
	public string testScriptName = "ADC_Callback.tst";
	public int testScriptTimeoutMs = 10000;
	public string useAnalysis = "exe";
	public string analysisScriptName = "ADC_Callback.exe";
	public bool useResultsFile = true;
	public string resultsFileName = "testTemp\\results.txt";
	public int testDelay = 0;
	// Do not change text format above this point
    }
}
