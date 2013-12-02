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
    public enum StorageType
    {
        NOR,
        SD
    };

    /* Error values */
    public enum DATASTORE_ERROR
    {
        DATASTORE_ERROR_NONE,
        DATASTORE_ERROR_INVALID_PARAM,
        DATASTORE_ERROR_OUT_OF_MEMORY,
        DATASTORE_ERROR_INVALID_RECORD_ID,
        DATASTORE_ERROR_INVALID_GIVEN_ADDR,
        DATASTORE_ERROR_OUT_OF_BOUND_ACCESS,
        DATASTORE_ERROR_OUT_OF_FLASH_MEMORY,
        DATASTORE_ERROR_RECORD_ID_ALREADY_EXISTS,
        DATASTORE_ERROR_UNEXPECTED_ERROR
    };

    public class Data
    {
        // Record Id unique to a pointer
        private UInt32 recordId;
        private UInt32 m_Size = 0;
        private byte[] buffer;
        
        DataStore dStore;

        Data(DataStore dStore)
        {
            this.dStore = dStore;
        }

        /// <summary>
        /// Create a data element, specify the maximum size of data element
        /// </summary>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        private Data(UInt32 MaxDataSize)
        {
            recordId++;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(recordId, MaxDataSize);
        }

        /// <summary>
        /// Create a data element, specify the maximum size of data element and the ID of the first data record to be created
        /// </summary>
        /// <param name="StartRecordID">Data ID of the first data record to be created</param>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        private Data(UInt32 StartRecordID, UInt32 MaxDataSize)
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
        private int Write(int data, UInt32 dataID)
        {
            byte[] buffer = new byte[sizeof(int)];
            return dStore.Write(recordId, buffer, dataID);
        }

        /// <summary>
        /// Write a byte array into the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        private int Write(byte[] data, UInt32 dataID)
        {
            return dStore.Write(recordId, data, dataID);
        }

        ~Data()
        {
            DisposeNativeMemoryPointer(recordId);
        }

        ///////////////////////////////////Internal methods/////////////////////////

        //Returns the size of the NVM, if 0 operation failed
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private UInt32 ConstructNativeMemoryPointer(UInt32 recordId, UInt32 bytes);

        // native call that destroys record created on the flash
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool DisposeNativeMemoryPointer(UInt32 recordId);
        
    }


    public class DataStore
    {
        private UInt32 m_Size = 0;
        //private UInt16 Blocks=0;
        //private float UsedKiloBytes=0;
        //private float FreeKiloBytes=0;

        // Stores the current record id
        //private static UInt32 currentRecordId = 0;

        ////private byte[] buffer;
        /// <summary>
        /// The block storage device type
        /// </summary>
        private int storageType;

        public DataStore(int storageType)
        {
            this.storageType = storageType;
        }

        //////////////////////////private properties and methods/////////////////////

        /// <summary>
        /// Size of the DataStore in Kilo Bytes
        /// </summary>
        private UInt32 Size
        {
            get
            {
                return m_Size;
            }
        }

        /// <summary>
        /// Number of Kilo Bytes of space already occupied by data
        /// </summary>
        private float UsedKBytes
        {
            get
            {
                return GetUsedKBytes();
            }
        }

        /// <summary>
        /// Number of Kilo Bytes of space available to store data
        /// </summary>
        private float FreeKBytes
        {
            get
            {
                return GetFreeKBytes();
            }
        }

        private bool ReadAllRecordIDs(UInt32 recordId, UInt32 Size)
        {
            return GetReadAllRecordIDs();
        }

        private UInt32 NumberOfDataRecords()
        {
            return GetNumberOfDataRecords();
        }

        private static bool DeleteAllRecords()
        {
            return DeleteAll();
        }

        private static bool GC()
        {
            return DataStoreGC();
        }

        private static bool EraseAll()
        {
            DataStore.DeleteAllRecords();
            DataStore.GC();
            return true;
        }

        private bool ReadWriteStatus()
        {
            return GetReadWriteStatus();
        }

        private DATASTORE_ERROR LastErrorStatus()
        {
            return GetLastErrorStatus();
        }

        /// <summary>
        /// Create a data store element, specify the maximum size of data element
        /// </summary>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        /*private DataStore(UInt32 MaxDataSize)
        {
            recordId++;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(recordId, MaxDataSize);
        }*/

        /// <summary>
        /// Create a data store element, specify the maximum size of data element and the ID of the first data record to be created
        /// </summary>
        /// <param name="StartRecordID">Data ID of the first data recrod to be created</param>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        /*private DataStore(UInt32 StartRecordID, UInt32 MaxDataSize)
        {
            recordId = StartRecordID;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(recordId, MaxDataSize);
        }*/     

        /// <summary>
        /// Write an integer to the data store
        /// </summary>
        /// <param name="data">Integer to be stored</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns 4 (number of bytes written) if successful, returns -1 if operation fails.</returns>
        public int Write(UInt32 recordId, int data, UInt32 dataID)
        {
            byte[] buffer = new byte[sizeof(int)];
            return Write(recordId, buffer, dataID, storageType);
        }

        /// <summary>
        /// Write a byte array into the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        public int Write(UInt32 recordId, byte[] data, UInt32 dataID)
        {
            return Write(recordId, data, dataID, storageType);
        }

        // destroys the record on the flash
        ~DataStore()
        {
            //DisposeNativeMemoryPointer(recordId);
        }

        ///////////////////////////////////Internal methods/////////////////////////

        // get amount of used space
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private float GetUsedKBytes();

        // get amount of free space
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private float GetFreeKBytes();

        /// <summary>
        /// Returns valid data records currently stored in the NVM
        /// </summary>
        /// <returns>Returns array with valid data records as unsigned integer</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool GetReadAllRecordIDs();

        /// <summary>
        /// Get the number of valid data records currently stored in the NVM
        /// </summary>
        /// <returns>Returns a the number of valid data records as unsigned integer</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private UInt32 GetNumberOfDataRecords();

        /// <summary>
        /// Delete all records in the Data Store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static bool DeleteAll();

        /// <summary>
        /// Garbage collects inactive records in Data Store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static bool DataStoreGC();

        /// <summary>
        /// Gets read/write status of the Data Store
        /// </summary>
        /// <returns>Returns status of read/write on data store</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static bool GetReadWriteStatus();

        /// <summary>
        /// Gets error status of data Store
        /// </summary>
        /// <returns>Returns error status of data store</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static DATASTORE_ERROR GetLastErrorStatus();

        /// <summary>
        /// Read data the data record represented by the recordId, into the buffer
        /// </summary>
        /// <param name="recordId">ID of the record to be read</param>
        /// <param name="buffer">Byte buffer into which the data will be read</param>
        /// <returns>Returns number of bytes read, returns -1 if operation fails</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Read(UInt32 recordId, byte[] buffer);

        /// <summary>
        /// Read the data record IDs currently stored in the NVM into the unsigned integer array provided
        /// </summary>
        /// <param name="buffer">Buffer into which the data record IDs will be read into</param>
        /// <returns>Returns the number of data iDs read</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private UInt16 ReadAllDataIDs(UInt32[] buffer);
        
        /// <summary>
        /// Delete the data represented by the dataID from data store.
        /// </summary>
        /// <param name="dataID">ID of the data to be deleted</param>
        /// <returns>Returns success or failure of operation</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool Delete(UInt32 dataID);

        //Returns the size of the NVM, if 0 operation failed
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern private UInt32 ConstructNativeMemoryPointer(UInt32 recordId, UInt32 bytes);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Write(UInt32 recordId, byte[] data, UInt32 dataID, int storageType);       
               
        // native call that destroys record created on the flash
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern private bool DisposeNativeMemoryPointer(UInt32 recordId);
        
    }
}
