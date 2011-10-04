/*---------------------------------------------------------------------
* TransportTests.cs - file description
* Version: 1.0
* Author: REDMOND\a-grchat
* Created: 11/2/2007 3:58:29 PM 
* ---------------------------------------------------------------------*/

using System;
using System.IO;
using System.Xml;
using Dpws.Client.Transport;
using Microsoft.SPOT.Platform.Test;
using Ws.Services.WsaAddressing;

namespace Microsoft.SPOT.Platform.Tests
{
    public class TransportTests : IMFTestInterface
    {
        [SetUp]
        public InitializeResult Initialize()
        {
            Log.Comment("Adding set up for the tests");

            try
            {
                // Check networking - we need to make sure we can reach our proxy server
                System.Net.Dns.GetHostEntry("itgproxy.dns.microsoft.com");
            }
            catch (Exception ex)
            {
                Log.Exception("Unable to get address for itgproxy.dns.microsoft.com", ex);
                return InitializeResult.Skip;
            }

            return InitializeResult.ReadyToGo;
        }

        [TearDown]
        public void CleanUp()
        {
            Log.Comment("Cleaning up after the tests.");
        }

        [TestMethod]
        public MFTestResults TransportTest1_DpwsSoapResponse()
        {
            /// <summary>
            /// 1. Verifies each of the proerties of a DpwsSoapResponse object
            /// 2. Sets and re-verifies.
            /// </summary>
            ///
            bool testResult = true;
            try
            {
                WsWsaHeader wwh = new Ws.Services.WsaAddressing.WsWsaHeader();
                XmlReader xr = System.Xml.XmlReader.Create(new MemoryStream(new byte [] {}));

                DpwsSoapResponse testDSR = new DpwsSoapResponse(wwh, xr );

                Log.Comment("Reader");
                if (testDSR.Reader != null)
                    if (testDSR.Reader.GetType() != Type.GetType("System.Xml.XmlReader"))
                        throw new Exception("Reader wrong type");

                if (testDSR.Reader != xr)
                    throw new Exception("Reader is not set correctly");


                Log.Comment("Header");
                if (testDSR.Header != null)
                    if (testDSR.Header.GetType() != Type.GetType("Ws.Services.WsaAddressing.WsWsaHeader"))
                        throw new Exception("Header wrong type");

                if (testDSR.Header != wwh)
                    throw new Exception("Header is not set correctly");
            }
            catch (Exception e)
            {
                testResult = false;
                Log.Comment("Incorrect exception caught: " + e.Message);
            }
            return (testResult ? MFTestResults.Pass : MFTestResults.Fail);
        }

        [TestMethod]
        public MFTestResults TransportTest2_DpwsHttpClient()
        {
            /// <summary>
            /// 1. Verifies each of the proerties of a DpwsHttpClient object
            /// 2. Sets and re-verifies.
            /// </summary>
            ///

            bool testResult = true;
            try
            {
                DpwsHttpClient testDHC = new DpwsHttpClient();
                Random random = new Random();
                int timeout = random.Next(10000);

                Log.Comment("ReceiveTimeout");
                    
                if (testDHC.ReceiveTimeout.GetType() !=
                        Type.GetType("System.Int32"))
                        throw new Exception("ReceiveTimeout wrong type");

                testDHC.ReceiveTimeout = timeout;
                if (testDHC.ReceiveTimeout.GetType() !=
                    Type.GetType("System.Int32"))
                    throw new Exception("ReceiveTimeout wrong type after set to new");

                if (testDHC.ReceiveTimeout != timeout)
                    throw new Exception("ReceiveTimeout wrong data after set to new");

                Log.Comment("SendTimeOut");
                timeout = random.Next(10000);

                if (testDHC.SendTimeOut.GetType() !=
                        Type.GetType("System.Int32"))
                        throw new Exception("SendTimeOut wrong type");

                testDHC.SendTimeOut = timeout;
                if (testDHC.SendTimeOut.GetType() !=
                    Type.GetType("System.Int32"))
                    throw new Exception("SendTimeOut wrong type after set to new");

                if (testDHC.SendTimeOut != timeout)
                    throw new Exception("SendTimeOut wrong data after set to new");

            }
            catch (Exception e)
            {
                testResult = false;
                Log.Comment("Incorrect exception caught: " + e.Message);
            }
            return (testResult ? MFTestResults.Pass : MFTestResults.Fail);
        }
    }
}
