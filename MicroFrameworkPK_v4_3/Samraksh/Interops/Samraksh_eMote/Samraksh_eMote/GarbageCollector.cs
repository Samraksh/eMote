/* Copyright: The Samaraksh Company (All rights reserved)
 * Author: Mukundan Sridharan
 * Date: Feb 2013
 * Function: Provides access to Samraksh Garbage Collector Extensions functions
 */

using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.eMote
{
    /// <summary>
    /// Garbage Collector class
    /// </summary>
    public class GC
    {
        /// <summary>
        /// Get amount of used heap space in kilo bytes
        /// </summary>
        /// <returns>Returns amount of used heap in kilo bytes</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public float GetUsedKBytes();

        /// <summary>
        /// Get amount of free heap space in kilo bytes
        /// </summary>
        /// <returns>Returns amount of free heap space in kilo bytes</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public float GetFreeKBytes();

        /// <summary>
        /// Check if Samraksh's Garbage Collection Extensions are enabled or not
        /// </summary>
        /// <returns>Returns true if enabled, false otherwise</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool IsGCSamExtensionsEnabled();
        
        /// <summary>
        /// Enable incremental Mark & Sweep  heap cleaning in GC
        /// </summary>
        /// <returns>Returns true if operation is success, false otherwise</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool EnableIncrementalCleaning();

        /// <summary>
        /// Enable incremental Compaction of heap in GC
        /// </summary>
        /// <returns>Returns true if operation is success, false otherwise</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool EnableIncrementalCompaction();

        /// <summary>
        /// Disable incremental Mark & Sweep  heap cleaning in GC
        /// </summary>
        /// <returns>Returns true if operation is success, false otherwise</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool DisableIncrementalCleaning();

        /// <summary>
        /// Disable incremental Compaction of heap in GC
        /// </summary>
        /// <returns>Returns true if operation is success, false otherwise</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool DisableIncrementalCompaction();
        
        /// <summary>
        /// Instruct GC to clean the heap and create the amount of space specified, before putting CPU to sleep
        /// </summary>
        /// <param name="KBytesToFree">Amount of free space to be created</param>
        /// <returns>Returns true if operation is success, false otherwise</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool CleanBeforeSleep(UInt32 KBytesToFree);

        /// <summary>
        /// Instruct GC to clean the heap now (Suspend managed code execution) and create the amount of space specified
        /// </summary>
        /// <param name="KBytesToFree">Amount of free space to be created</param>
        /// <returns>Returns true if operation is success, false otherwise</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool CleanNow(UInt32 KBytesToFree);

        /// <summary>
        /// Instruct GC to clean the heap now and create as much space as possible, without compaction
        /// </summary>
        /// <returns>Returns true if operation is success, false otherwise</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool CleanNow();

        /// <summary>
        /// Average time for the Mark and Sweep operation. Implements weighted (0.5 weight) moving average.
        /// </summary>
        /// <returns>Returns the average time as number of microseconds.</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public UInt32 AvgTimeForCleaning();

        /// <summary>
        /// Average time for the Compaction operation. Implements weighted (0.5 weight) moving average.
        /// </summary>
        /// <returns>Returns the average time as number of microseconds.</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public UInt32 AvgTimeForCompaction();
    }
}
