////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Reflection;
using Microsoft.SPOT.Platform.Test;

namespace Microsoft.SPOT.Platform.Tests
{
    public class ConvertTests : IMFTestInterface
    {
        [SetUp]
        public InitializeResult Initialize()
        {
            Log.Comment("Adding set up for the tests.");
            // Add your functionality here.                

            return InitializeResult.ReadyToGo;
        }

        [TearDown]
        public void CleanUp()
        {
        }

        //Test Case Calls
        [TestMethod]
        public MFTestResults Convert_Positive()
        {
            string number = "12";
            int actualNumber = 12;

            SByte value_sb = Convert.ToSByte(number);

            if (value_sb != (byte)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            Byte value_b = Convert.ToByte(number);

            if (value_b != (byte)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            Int16 value_s16 = Convert.ToInt16(number);

            if (value_s16 != (short)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            UInt16 value_u16 = Convert.ToUInt16(number);

            if (value_u16 != (ushort)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            Int32 value_s32 = Convert.ToInt32(number);

            if (value_s32 != (int)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            UInt32 value_u32 = Convert.ToUInt32(number);

            if (value_u32 != (uint)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            Int64 value_s64 = Convert.ToInt32(number);

            if (value_s64 != (long)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            UInt64 value_u64 = Convert.ToUInt64(number);

            if (value_u64 != (ulong)12)
            {
                return MFTestResults.Fail;
            }

            return MFTestResults.Pass;
        }

        //Test Case Calls
        [TestMethod]
        public MFTestResults Convert_PositivePlus()
        {
            string number = "+12";
            int actualNumber = 12;

            SByte value_sb = Convert.ToSByte(number);

            if (value_sb != (byte)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            Byte value_b = Convert.ToByte(number);

            if (value_b != (byte)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            Int16 value_s16 = Convert.ToInt16(number);

            if (value_s16 != (short)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            UInt16 value_u16 = Convert.ToUInt16(number);

            if (value_u16 != (ushort)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            Int32 value_s32 = Convert.ToInt32(number);

            if (value_s32 != (int)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            UInt32 value_u32 = Convert.ToUInt32(number);

            if (value_u32 != (uint)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            Int64 value_s64 = Convert.ToInt32(number);

            if (value_s64 != (long)12)
            {
                return MFTestResults.Fail;
            }

            //--//

            UInt64 value_u64 = Convert.ToUInt64(number);

            if (value_u64 != (ulong)12)
            {
                return MFTestResults.Fail;
            }

            return MFTestResults.Pass;
        }


        [TestMethod]
        public MFTestResults Convert_Negative()
        {
            string number = "-12";
            int actualNumber = -12;

            SByte value_sb = Convert.ToSByte(number);

            if (value_sb != (sbyte)actualNumber)
            {
                return MFTestResults.Fail;
            }

            //--//

            try
            {
                Byte value_b = Convert.ToByte(number);
                return MFTestResults.Fail;
            }
            catch
            {
            }

            //--//

            Int16 value_s16 = Convert.ToInt16(number);

            if (value_s16 != (short)actualNumber)
            {
                return MFTestResults.Fail;
            }

            //--//

            try
            {
                UInt16 value_u16 = Convert.ToUInt16(number);
                return MFTestResults.Fail;
            }
            catch
            {
            }

            //--//

            Int32 value_s32 = Convert.ToInt32(number);

            if (value_s32 != (int)actualNumber)
            {
                return MFTestResults.Fail;
            }

            //--//

            try
            {
                UInt32 value_u32 = Convert.ToUInt32(number);
                return MFTestResults.Fail;
            }
            catch
            {
            }

            //--//

            Int64 value_s64 = Convert.ToInt32(number);

            if (value_s64 != (long)actualNumber)
            {
                return MFTestResults.Fail;
            }

            //--//

            try
            {
                UInt64 value_u64 = Convert.ToUInt64(number);
                return MFTestResults.Fail;
            }
            catch
            {
            }

            return MFTestResults.Pass;
        }


        [TestMethod]
        public MFTestResults Convert_Double()
        {
            string number = "36.123456";
            double actualNumber = 36.123456;

            double value_dd = Convert.ToDouble(number);

            if (value_dd != actualNumber)
            {
                return MFTestResults.Fail;
            }

            return MFTestResults.Pass;
        }


        [TestMethod]
        public MFTestResults Convert_Plus()
        {
            string number = "+36.123456";
            double actualNumber = 36.123456;

            double value_dd = Convert.ToDouble(number);

            if (value_dd != actualNumber)
            {
                return MFTestResults.Fail;
            }

            return MFTestResults.Pass;
        }


        [TestMethod]
        public MFTestResults Convert_Neg()
        {
            string number = "-36.123456";
            double actualNumber = -36.123456;

            double value_dd = Convert.ToDouble(number);

            if (value_dd != actualNumber)
            {
                return MFTestResults.Fail;
            }

            return MFTestResults.Pass;
        }
    }
}
