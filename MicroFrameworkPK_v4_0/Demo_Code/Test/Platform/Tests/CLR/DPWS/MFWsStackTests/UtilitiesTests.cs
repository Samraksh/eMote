/*---------------------------------------------------------------------
* FaultsTests.cs - file description
* Version: 1.0
* Author: REDMOND\a-grchat
* Created: 11/14/2007 10:27:20 AM 
* ---------------------------------------------------------------------*/

using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Platform.Test;
using Ws.Services;
using Ws.Services.Utilities;
using Dpws.Device;

namespace Microsoft.SPOT.Platform.Tests
{
    public class UtilitiesTests : IMFTestInterface
    {
        [SetUp]
        public InitializeResult Initialize()
        {
            Log.Comment("Adding set up for the tests");

            return InitializeResult.ReadyToGo;
        }

        [TearDown]
        public void CleanUp()
        {
            Log.Comment("Cleaning up after the tests.");
        }


        [TestMethod]
        public MFTestResults UtilitiesTest1_WsDuration()
        {
            /// <summary>
            /// 1. Gets and verifies each of the properties of a WsDuration object
            /// 2. Sets and re-verifies all properties
            /// </summary>
            ///

            bool testResult = true;
            try
            {
                Log.Comment("Calling Duragtion with Integer");
                WsDuration testWD = new WsDuration(10);

                if (testWD.DurationInSeconds != 10)
                    throw new Exception("Duration In Seconds is incorrect");

                Log.Comment("Calling Duragtion with string");
                testWD = new WsDuration("P1Y2M3DT4H4M6S");
                if (testWD.DurationString != "P1Y2M3DT4H4M6S")
                    throw new Exception("Duration In Seconds is incorrect");

                Log.Comment("Calling Duragtion with timespan");
                testWD = new WsDuration(new TimeSpan(1000));
                if (testWD.Ticks != new TimeSpan(1000).Ticks)
                    throw new Exception("Duration In Seconds is incorrect");

                Log.Comment("DurationString");
                if (testWD.DurationString != null)
                    if (testWD.DurationString.GetType() !=
                        Type.GetType("System.String"))
                        throw new Exception("DurationString wrong type");
            }
            catch (Exception e)
            {
                testResult = false;
                Log.Comment("Incorrect exception caught: " + e.Message);
            }
            return (testResult ? MFTestResults.Pass : MFTestResults.Fail);
        }

        [TestMethod]
        public MFTestResults UtilitiesTest_WSWellKnownUri()
        {
            /// <summary>
            /// Verifies each of the methods in WsUtilities
            /// </summary>
            ///

            if (WsWellKnownUri.SoapNamespaceUri == "http://www.w3.org/2003/05/soap-envelope" &&
                 WsWellKnownUri.WsaNamespaceUri_2004_08== "http://schemas.xmlsoap.org/ws/2004/08/addressing" &&
                 WsWellKnownUri.WsaNamespaceUri_2005_08== "http://www.w3.org/2005/08/addressing" &&
                 WsWellKnownUri.XopNamespaceUri == "http://www.w3.org/2004/08/xop/include" &&
                 WsWellKnownUri.WsdpNamespaceUri == "http://schemas.xmlsoap.org/ws/2006/02/devprof" &&
                 WsWellKnownUri.WseNamespaceUri == "http://schemas.xmlsoap.org/ws/2004/08/eventing" &&
                 WsWellKnownUri.WsxNamespaceUri == "http://schemas.xmlsoap.org/ws/2004/09/mex" &&
                 WsWellKnownUri.WstNamespaceUri == "http://schemas.xmlsoap.org/ws/2004/09/transfer" &&
                 WsWellKnownUri.SchemaNamespaceUri == "http://www.w3.org/2001/XMLSchema-instance")
            {
               if (Device.DiscoVersion is Dpws.Device.Discovery.DiscoveryVersion11 )
                {
                    if (WsWellKnownUri.WsdNamespaceUri == "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01")
                    {
                        return MFTestResults.Pass;
                    }
                }
                else if(WsWellKnownUri.WsdNamespaceUri == "http://schemas.xmlsoap.org/ws/2005/04/discovery")
                {
                    return MFTestResults.Pass;
                }
            }

            return MFTestResults.Fail;
        }
    }
}
