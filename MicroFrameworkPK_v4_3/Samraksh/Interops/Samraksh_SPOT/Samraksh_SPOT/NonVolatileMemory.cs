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

    public enum DataStatus
    {
        Success,
        Failure,
        AlreadyExists,
        InvalidParameter,
        InvalidPointer
    };

    public class Data
    {
        // Record Id unique to a pointer
        private UInt32 recordId;
        private UInt32 m_Size = 0;
        private byte[] buffer;
        private Int32 recordPointer;
        
        DataStore dStore;

        public Data(DataStore dStore, UInt32 recordId, UInt32 m_Size)
        {
            this.dStore = dStore;
            this.m_Size = m_Size;
            this.recordId = recordId;
            this.recordPointer = 0x0;
        }

        /// <summary>
        /// Create a data element, specify the maximum size of data element and the ID of the first data record to be created
        /// </summary>
        /// <param name="StartRecordID">Data ID of the first data record to be created</param>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        public Data(UInt32 StartRecordID, UInt32 MaxDataSize)
        {
            recordId = StartRecordID;
            //buffer = new byte[MaxDataSize];
            //m_Size = ConstructNativeMemoryPointer(recordId, MaxDataSize);
        }

        public DataStatus Create()
        {
            recordPointer = dStore.CreateRecord(recordId, m_Size);
            if (recordPointer != 0x0)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        public DataStatus GetStatus()
        {
            if (dStore.LastErrorStatus() == (int)DATASTORE_ERROR.DATASTORE_ERROR_RECORD_ID_ALREADY_EXISTS)
                return DataStatus.AlreadyExists;
            else if (dStore.LastErrorStatus() == (int)DATASTORE_ERROR.DATASTORE_ERROR_NONE)
                return DataStatus.Success;

            return DataStatus.Failure;

        }

        /*public Data(DataStore dStore, UInt32 m_Size)
        {
            this.dStore = dStore;
            this.m_Size = m_Size;
            recordId++;
            dStore.GetCreateRecord(recordId, m_Size);
        }*/

        /// <summary>
        /// Create a data element, specify the maximum size of data element
        /// </summary>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        /*public Data(UInt32 MaxDataSize)
        {
            recordId++;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(recordId, MaxDataSize);
        }*/

        /// <summary>
        /// Write an integer to the data store
        /// </summary>
        /// <param name="data">Integer to be stored</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns 4 (number of bytes written) if successful, returns -1 if operation fails.</returns>
        /*public int Write(int data, UInt32 dataID)
        {
            byte[] buffer = new byte[sizeof(int)];
            return dStore.Write(recordId, buffer, dataID);
        }*/

        /// <summary>
        /// Write a byte array into the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        public DataStatus Write(byte[] data, UInt32 offset, UInt32 numBytes)
        {
            if (recordPointer == 0x0)
                return DataStatus.InvalidPointer;

            if (numBytes > data.Length)
                return DataStatus.InvalidParameter;
            //return dStore.Write(recordId, data, offset, numBytes);
            return (DataStatus)dStore.Write((uint) recordPointer, data, numBytes);
        }

        public DataStatus Write(byte[] data, UInt32 numBytes)
        {
            if (recordPointer == 0x0)
                return DataStatus.InvalidPointer;

            if (numBytes > data.Length)
                return DataStatus.InvalidParameter;
            //return dStore.Write(recordId, data, 0, numBytes);
            return (DataStatus)dStore.Write((uint) recordPointer, data, numBytes);
        }

        /// <summary>
        /// Read a byte array into the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        public DataStatus Read(byte[] data)
        {

            if (recordPointer == 0x0)
                return DataStatus.InvalidPointer;

            if (dStore.Read((uint)recordPointer, data) == 0x0)
                return DataStatus.Failure;

            return DataStatus.Success;
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

        private enum DATASTORE_ERROR
        {
            DATASTORE_ERROR_NONE,
            DATASTORE_ERROR_INVALID_PARAM,
            DATASTORE_ERROR_OUT_OF_MEMORY,
            DATASTORE_ERROR_INVALID_RECORD_ID,
            DATASTORE_ERROR_INVALID_GIVEN_ADDR,
            DATASTORE_ERROR_OUT_OF_BOUND_ACCESS,
            DATASTORE_ERROR_OUT_OF_FLASH_MEMORY,
            DATASTORE_ERROR_WRITE_TO_FLASH_MEMORY_FAILED,
            DATASTORE_ERROR_RECORD_ID_ALREADY_EXISTS,
            DATASTORE_ERROR_UNEXPECTED_ERROR
        }

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
            bool retVal = CreateDataStore();
            /*if( retVal == false )
                throw new SystemException();*/
        }

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

        /*public int TempCreateRecord(UInt32 recordId, UInt32 Size)
        {
            return CreateRecord(recordId, Size);
        }*/

        //public bool ReadAllRecordIDs(UInt32 recordId, UInt32 Size)
        public bool ReadAllRecordIDs()
        {
            int[] recIdArray = new int[256];
            return GetReadAllRecordIDs(recIdArray);
        }

        public UInt32 NumberOfDataRecords()
        {
            return GetNumberOfDataRecords();
        }

        public static int DeleteAllRecords()
        {
            return DeleteAll();
        }

        public static int GC()
        {
            return DataStoreGC();
        }

        public static int EraseAll()
        {
            DataStore.DeleteAllRecords();
            DataStore.GC();
            return 1;
        }

        public bool ReadWriteStatus()
        {
            return GetReadWriteStatus();
        }

        public int LastErrorStatus()
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
        public int Write(UInt32 address, byte[] data)
        {
            //byte[] buffer = new byte[sizeof(int)];
            return Write(address, data, (UInt32)data.Length, storageType);
        }

        /// <summary>
        /// Write a byte array into the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="dataID">The dataID of stored data is copied to dataID</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        public int Write(UInt32 address, byte[] data, UInt32 numBytes)
        {
            return Write(address, data, numBytes, storageType);
        }

        /// <summary>
        /// Read a byte array from the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        public int Read(UInt32 address, byte[] data)
        {
            return Read(address, data, storageType);
        }

        // destroys the record on the flash
        ~DataStore()
        {
            //DisposeNativeMemoryPointer(recordId);
        }

        ///////////////////////////////////Internal methods/////////////////////////

        // get amount of used space
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int CreateRecord(UInt32 recordId, UInt32 Size);

        // get amount of used space
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool CreateDataStore();

        // get amount of used space
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public float GetUsedKBytes();

        // get amount of free space
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public float GetFreeKBytes();

        /// <summary>
        /// Returns valid data records currently stored in the NVM
        /// </summary>
        /// <returns>Returns array with valid data records as unsigned integer</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public bool GetReadAllRecordIDs(int[] recordIdArray);

        /// <summary>
        /// Get the number of valid data records currently stored in the NVM
        /// </summary>
        /// <returns>Returns a the number of valid data records as unsigned integer</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public UInt32 GetNumberOfDataRecords();

        /// <summary>
        /// Delete all records in the Data Store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static int DeleteAll();

        /// <summary>
        /// Garbage collects inactive records in Data Store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static int DataStoreGC();

        /// <summary>
        /// Gets read/write status of the Data Store
        /// </summary>
        /// <returns>Returns status of read/write on data store</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool GetReadWriteStatus();

        /// <summary>
        /// Gets error status of data Store
        /// </summary>
        /// <returns>Returns error status of data store</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static int GetLastErrorStatus();

        /// <summary>
        /// Read data the data record represented by the recordId, into the buffer
        /// </summary>
        /// <param name="recordId">ID of the record to be read</param>
        /// <param name="buffer">Byte buffer into which the data will be read</param>
        /// <returns>Returns number of bytes read, returns -1 if operation fails</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int Read(UInt32 address, byte[] buffer, int storageType);

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

        //Returns the size of the NVM, if 0 operation failed
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern private UInt32 ConstructNativeMemoryPointer(UInt32 recordId, UInt32 bytes);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int Write(UInt32 address, byte[] data, UInt32 numBytes, int storageType);       
        
        // native call that destroys record created on the flash
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern private bool DisposeNativeMemoryPointer(UInt32 recordId);
        
    }
}
