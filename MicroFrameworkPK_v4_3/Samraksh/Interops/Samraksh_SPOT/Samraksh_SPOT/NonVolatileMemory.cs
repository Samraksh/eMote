/* Copyright: The Samaraksh Company (All rights reserved)
 * Author: Mukundan Sridharan
 * Date: Feb 2013
 * Function: Provides access to Samraksh NVM DataStore filesystem
 */

using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.NonVolatileMemory
{
    public class DataStore
    {
        private UInt32 m_Size = 0;
        //private UInt16 Blocks=0;
        //private float UsedKiloBytes=0;
        //private float FreeKiloBytes=0;

        // Record Id unique to a pointer
        private UInt32 recordId;
        
        // Stores the current record id
        //private static UInt32 currentRecordId = 0;

        private byte[] buffer;

        //////////////////////////public properties and methods/////////////////////

        /// <summary>
        /// Size of the DataStore in Kilo Bytes
        /// </summary>
        public UInt32 Size
        {
            get
            {
                return m_Size;
            }
        }

        /// <summary>
        /// Number of Kilo Bytes of space already occupied by data
        /// </summary>
        public float UsedKBytes
        {
            get
            {
                return GetUsedKBytes();
            }
        }

        /// <summary>
        /// Number of Kilo Bytes of space available to store data
        /// </summary>
        public float FreeKBytes
        {
            get
            {
                return GetFreeKBytes();
            }
        }

        /// <summary>
        /// Create a data store element, specify the maximum size of data element
        /// </summary>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        public DataStore(UInt32 MaxDataSize)
        {
            recordId++;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(recordId, MaxDataSize);
        }

        /// <summary>
        /// Create a data store element, specify the maximum size of data element and the ID of the first data record to be created
        /// </summary>
        /// <param name="StartRecordID">Data ID of the first data recrod to be created</param>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        public DataStore(UInt32 StartRecordID, UInt32 MaxDataSize)
        {
            recordId = StartRecordID;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(recordId, MaxDataSize);
        }     


        /// <summary>
        /// Write an integer to the data store
        /// </summary>
        /// <param name="data">Integer to be stored</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns 4 (number of bytes written) if successful, returns -1 if operation fails.</returns>
        public int Write(int data, ref UInt32 dataID)
        {
            byte[] buffer = new byte[sizeof(int)];
            return Write(recordId, buffer, ref dataID);
        }

        /// <summary>
        /// Write a byte array into the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        public int Write(byte[] data, ref UInt32 dataID)
        {
            return Write(recordId, data, ref dataID);
        }

        // destroys the record on the flash
        ~DataStore()
        {
            DisposeNativeMemoryPointer(recordId);
        }


        ///////////////////////////////////Internal methods/////////////////////////


        /// <summary>
        /// Get the number of valid data records currently stored in the NVM
        /// </summary>
        /// <returns>Returns a the number of valid data records as unsigned integer</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public UInt32 GetNumberOfDataRecords();

        /// <summary>
        /// Read data the data record represented by the recordId, into the buffer
        /// </summary>
        /// <param name="recordId">ID of the record to be read</param>
        /// <param name="buffer">Byte buffer into which the data will be read</param>
        /// <returns>Returns number of bytes read, returns -1 if operation fails</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int Read(UInt32 recordId, byte[] buffer);

        /// <summary>
        /// Read the data record IDs currently stored in the NVM into the unsigned integer array provided
        /// </summary>
        /// <param name="buffer">Buffer into which the data record IDs will be read into</param>
        /// <returns>Returns the number of data iDs read</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public UInt16 ReadAllDataIDs(UInt32[] buffer);
        
        /// <summary>
        /// Delete the data represented by the dataID from data store.
        /// </summary>
        /// <param name="dataID">ID of the data to be deleted</param>
        /// <returns>Returns success or failure of operation</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool Delete(UInt32 dataID);

        /// <summary>
        /// Delete all records in the Data Store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool DeleteAll();

        //Returns the size of the NVM, if 0 operation failed
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private UInt32 ConstructNativeMemoryPointer(UInt32 recordId, UInt32 bytes);

        // get amount of used space
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private float GetUsedKBytes();

        // get amount of free space
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private float GetFreeKBytes();
        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Write(UInt32 recordId, byte[] data, ref UInt32 dataID);       
               
        // native call that destroys record created on the flash
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool DisposeNativeMemoryPointer(UInt32 recordId);


    }
}
