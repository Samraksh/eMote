using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.DSP
{
    /// <summary>
    /// Simple statistics on data
    /// </summary>
    public class Statistics
    {
        /**********************************************SUM******************************/
        /// <summary>
        /// Returns the Sum of an Array
        /// </summary>
        /// <param name="array">The Array of bytes</param>
        /// <returns>The result of the method: UInt64</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt64 Sum(byte[] array);

        /// <summary>
        /// Returns the Sum of an Array
        /// </summary>
        /// <param name="array">The Array of UInt16</param>
        /// <returns>The result of the method: UInt64</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt64 Sum(UInt16[] array);

        /// <summary>
        /// Returns the Sum of an Array
        /// </summary>
        /// <param name="array">The Array of UInt32</param>
        /// <returns>The result of the method: UInt64</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt64 Sum(UInt32[] array);

        /// <summary>
        /// Returns the Sum of an Array
        /// </summary>
        /// <param name="array">The Array of UInt64</param>
        /// <returns>The result of the method: UInt64</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt64 Sum(UInt64[] array);

        /**********************************************Median******************************/
        /// <summary>
        /// Returns the Median of an Array
        /// </summary>
        /// <param name="array">The Array of bytes</param>
        /// <returns>The result of the method: byte</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte Median(byte[] array);

        /// <summary>
        /// Returns the Median of an Array
        /// </summary>
        /// <param name="array">The Array of UInt16</param>
        /// <returns>The result of the method: UInt16</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16 Median(UInt16[] array);

        /// <summary>
        /// Returns the Median of an Array
        /// </summary>
        /// <param name="array">The Array of UInt32</param>
        /// <returns>The result of the method: UInt32</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt32 Median(UInt32[] array);

        /// <summary>
        /// Returns the Median of an Array
        /// </summary>
        /// <param name="array">The Array of UInt64</param>
        /// <returns>The result of the method: UInt64</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt64 Median(UInt64[] array);



        /**********************************************Mode******************************/
        /// <summary>
        /// Returns the Mode of an Array
        /// </summary>
        /// <param name="array">The Array of bytes</param>
        /// <returns>The result of the method: byte</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte Mode(byte[] array);

        /// <summary>
        /// Returns the Mode of an Array
        /// </summary>
        /// <param name="array">The Array of UInt16</param>
        /// <returns>The result of the method: UInt16</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16 Mode(UInt16[] array);

        /// <summary>
        /// Returns the Mode of an Array
        /// </summary>
        /// <param name="array">The Array of UInt32</param>
        /// <returns>The result of the method: UInt32</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt32 Mode(UInt32[] array);

        /// <summary>
        /// Returns the Mode of an Array
        /// </summary>
        /// <param name="array">The Array of UInt64</param>
        /// <returns>The result of the method: UInt64</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt64 Mode(UInt64[] array);

        /**********************************************Variance******************************/
        /// <summary>
        /// Returns the Variance of an Array
        /// </summary>
        /// <param name="array">The Array of bytes</param>
        /// <returns>The result of the method: UInt64</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern double Variance(byte[] array);

        /// <summary>
        /// Returns the Variance of an Array
        /// </summary>
        /// <param name="array">The Array of UInt16</param>
        /// <returns>The result of the method: UInt64</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern double Variance(UInt16[] array);

        /// <summary>
        /// Returns the Variance of an Array
        /// </summary>
        /// <param name="array">The Array of UInt32</param>
        /// <returns>The result of the method: UInt64</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern double Variance(UInt32[] array);

        /// <summary>
        /// Returns the Variance of an Array
        /// </summary>
        /// <param name="array">The Array of UInt64</param>
        /// <returns>The result of the method: UInt64</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern double Variance(UInt64[] array);


    }
}
