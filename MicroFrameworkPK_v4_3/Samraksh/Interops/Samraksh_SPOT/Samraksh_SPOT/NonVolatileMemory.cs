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

    /// <summary>
    /// DataStore error values 
    /// </summary>
    public enum DATASTORE_ERROR
    {
        DATASTORE_ERROR_NONE,
        DATASTORE_ERROR_INVALID_PARAM,
        DATASTORE_ERROR_OUT_OF_MEMORY,
        DATASTORE_ERROR_INVALID_DATA_ID,
        DATASTORE_ERROR_INVALID_GIVEN_ADDR,
        DATASTORE_ERROR_OUT_OF_BOUND_ACCESS,
        DATASTORE_ERROR_OUT_OF_FLASH_MEMORY,
        DATASTORE_ERROR_DATA_ID_ALREADY_EXISTS,
        DATASTORE_ERROR_UNEXPECTED_ERROR
    };

    /// <summary>
    /// DataStore status types
    /// </summary>
    public enum DataStatus
    {
        Success,
        Failure,
        AlreadyExists,
        InvalidParameter,
        InvalidPointer
    };

    /// <summary>
    /// Data class. Provides functions to write, read and delete data. 
    /// Provides a reference/handle to the data stored in the block storage device to the user.
    /// </summary>
    public class Data
    {
        // Record Id unique to a pointer
        private UInt32 dataId;
        private UInt32 m_Size = 0;
        private Int32 dataPointer;
        private Type dataType;
        
        DataStore dStore;

        /// <summary>
        /// Data constructor that takes DataStore and the data type as parameters
        /// </summary>
        /// <param name="dStore"></param>
        /// <param name="dataType"></param>
        public Data(DataStore dStore, Type dataType)
        {
            this.dStore = dStore;
            this.dataType = dataType;
        }

        /// <summary>
        /// Default data class constructor
        /// </summary>
        /// <param name="dStore">DataStore object</param>
        /// <param name="m_Size">Size of the data object to be stored in data store</param>
        /// <param name="dataType">Type of data to be stored in data store</param>
        public Data(DataStore dStore, UInt32 m_Size, Type dataType)
        {
            this.dStore = dStore;
            this.m_Size = m_Size;
            this.dataType = dataType;
            this.dataPointer = dStore.CreateData(m_Size);
            this.dataId = dStore.GetDataID();
        }

        /// <summary>
        /// Returns a data object corresponding to the input parameter dataId.
        /// </summary>
        /// <param name="dStore">DataStore object</param>
        /// <param name="dataId">ID of data for which the user wants a reference/handle</param>
        public Data(DataStore dStore, UInt32 dataId)
        {
            this.dStore = dStore;
            this.dataPointer = LookupData(dataId);
            //m_Size = ConstructNativeMemoryPointer(dataId, MaxDataSize);
        }

        /// <summary>
        /// Returns the last error status of data store.
        /// </summary>
        /// <returns>Returns success, failure or error status</returns>
        public DataStatus GetStatus()
        {
            if (dStore.LastErrorStatus() == (int)DATASTORE_ERROR.DATASTORE_ERROR_DATA_ID_ALREADY_EXISTS)
                return DataStatus.AlreadyExists;
            else if (dStore.LastErrorStatus() == (int)DATASTORE_ERROR.DATASTORE_ERROR_NONE)
                return DataStatus.Success;

            return DataStatus.Failure;
        }

        /// <summary>
        /// Create a data element, specify the maximum size of data element
        /// </summary>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        /*public Data(UInt32 MaxDataSize)
        {
            dataId++;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(dataId, MaxDataSize);
        }*/

        /// <summary>
        /// Write a byte array into the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="offset">Offset within the data array from which to write to data store</param>
        /// <param name="numBytes">Number of bytes to be written to data store</param>
        /// <returns>Returns success or failure</returns>
        public DataStatus Write(byte[] data, UInt32 offset, UInt32 numBytes)
        {
            if (dataPointer == 0x0)
                return DataStatus.InvalidPointer;

            if (numBytes > data.Length)
                return DataStatus.InvalidParameter;
            
            bool retVal = dStore.Write((uint)dataPointer, data, numBytes);
            if (retVal == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        /// <summary>
        /// Write a byte array into the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="numBytes">Number of bytes to be written to data store</param>
        /// <returns>Returns success or failure</returns>
        public DataStatus Write(byte[] data, UInt32 numBytes)
        {
            if (dataPointer == 0x0)
                return DataStatus.InvalidPointer;

            if (numBytes > data.Length)
                return DataStatus.InvalidParameter;
            
            bool retVal = dStore.Write((uint) dataPointer, data, numBytes);
            if (retVal == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        /// <summary>
        /// Read a byte array from the data store into the array passed as parameter
        /// </summary>
        /// <param name="data">Byte array to be filled up with data for the corresponding data object</param>
        /// <returns>Returns if read was a success or a failure</returns>
        public DataStatus Read(byte[] data)
        {
            if (dataPointer == 0x0)
                return DataStatus.InvalidPointer;

            if (dStore.Read((uint)dataPointer, data) == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        /// <summary>
        /// Delete a data/record from the data store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        public DataStatus Delete()
        {
            if(DeleteData(dataId) == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        /*~Data()
        {
            DisposeNativeMemoryPointer(dataId);
            Debug.Print("Destructor");
        }*/


        ///////////////////////////////////Internal methods/////////////////////////

        /// <summary>
        /// Get address of dataID from the data store
        /// </summary>
        /// <param name="dataId">ID of the data/record to be looked up</param>
        /// <returns>Returns address/reference to the dataID</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private Int32 LookupData(UInt32 dataId);

        /// <summary>
        /// Delete the data represented by the dataId from data store.
        /// </summary>
        /// <param name="dataId">ID of the data/record to be deleted</param>
        /// <returns>Returns true or false</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool DeleteData(UInt32 dataId);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="dataId"></param>
        /// <param name="bytes"></param>
        /// <returns></returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private UInt32 ConstructNativeMemoryPointer(UInt32 dataId, UInt32 bytes);

        /// <summary>
        /// Native call that destroys record created on the flash
        /// </summary>
        /// <param name="dataId"></param>
        /// <returns>Returns true or false</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool DisposeNativeMemoryPointer(UInt32 dataId);
        
    }

    /// <summary>
    /// DataStore class that holds data. 
    /// DataStore can be any block storage device (eg. NOR, SD card)
    /// </summary>
    public class DataStore
    {

        private enum DATASTORE_ERROR
        {
            DATASTORE_ERROR_NONE,
            DATASTORE_ERROR_INVALID_PARAM,
            DATASTORE_ERROR_OUT_OF_MEMORY,
            DATASTORE_ERROR_INVALID_DATA_ID,
            DATASTORE_ERROR_INVALID_GIVEN_ADDR,
            DATASTORE_ERROR_OUT_OF_BOUND_ACCESS,
            DATASTORE_ERROR_OUT_OF_FLASH_MEMORY,
            DATASTORE_ERROR_WRITE_TO_FLASH_MEMORY_FAILED,
            DATASTORE_ERROR_DATA_ID_ALREADY_EXISTS,
            DATASTORE_ERROR_UNEXPECTED_ERROR
        }

        private UInt32 m_Size = 0;
        //private UInt16 Blocks=0;
        //private float UsedKiloBytes=0;
        //private float FreeKiloBytes=0;

        // Stores the current record id
        //private static UInt32 currentdataId = 0;

        /// <summary>
        /// The block storage device type
        /// </summary>
        private int storageType;

        /// <summary>
        /// DataStore constructor that takes a storageType as parameter and initializes the data store with that storageType.
        /// </summary>
        /// <param name="storageType">storageType can be NOR, SD card or any other block storage device</param>
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

        /*public int TempCreateRecord(UInt32 dataId, UInt32 Size)
        {
            return CreateRecord(dataId, Size);
        }*/

        /*public void addDataReference(UInt32 dataId)
        {
            dataObj.Add(dataId);
        }*/

        /// <summary>
        /// Fills up the array passed as parameter with all dataIDs
        /// </summary>
        /// <param name="dataIdArray">Array which is filled up with the dataIDs</param>
        /// <returns>Returns success or failure</returns>
        public DataStatus ReadAllDataIds(int[] dataIdArray)
        {
            if (GetReadAllDataIds(dataIdArray) == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        /// <summary>
        /// Returns the count of dataIDs
        /// </summary>
        /// <returns>Returns count of dataIDs</returns>
        public UInt32 CountOfDataIds()
        {
            return GetCountOfDataIds();
        }

        /// <summary>
        /// Fills up the array (passed as first parameter) with the dataIDs starting from offset (passed as second parameter)
        /// </summary>
        /// <param name="dataRefArray"></param>
        /// <param name="dataIdOffset"></param>
        /// <returns>Returns success or failure</returns>
        public DataStatus ReadAllDataReferences(Data[] dataRefArray, UInt16 dataIdOffset)
        {
            UInt16 dataId;
            int[] dataIdArray = new int[GetCountOfDataIds()];
            if (GetReadAllDataIds(dataIdArray) == false)
                return DataStatus.Failure;
            else
            {
                for (UInt16 arrayIndex = 0; arrayIndex < dataRefArray.Length; ++arrayIndex)
                {
                    dataId = (UInt16)dataIdArray[dataIdOffset];
                    dataRefArray[arrayIndex] = new Data(this, dataId);
                    ++dataIdOffset;
                }
            }
            return DataStatus.Success;
        }

        /*public UInt32 CountOfDataReferences()
        {
            return (UInt32)dataObj.Count;
        }*/

        /// <summary>
        /// Deletes all dataIDs from the data store. Does not actually erase the data, but marks them as inactive.
        /// </summary>
        /// <returns>Returns success or failure</returns>
        public static DataStatus DeleteAllData()
        {
            if (DeleteAll() == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        /// <summary>
        /// Performs garbage collection on the data store. This is done internally by data store when there is not enough space to add new data.
        /// GC rearranges data internally so that all active records are grouped together. Then the inactive records are erased one block at a time.
        /// </summary>
        /// <returns>Returns success or failure</returns>
        private static DataStatus GC()
        {
            if(DataStoreGC() == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        /// <summary>
        /// Erases all data from data store.
        /// </summary>
        /// <returns>Returns success or failure</returns>
        public static DataStatus EraseAll()
        {
            /*if(DataStore.DeleteAllData() != DataStatus.Success)
                return DataStatus.Failure;
            
            if(DataStore.GC() != DataStatus.Success)
                return DataStatus.Failure;

            return DataStatus.Success;*/

            if (EraseAllBlocks() == true)
                return DataStatus.Success;
            else
                return DataStatus.Failure;
        }

        /// <summary>
        /// Returns write status 
        /// </summary>
        /// <returns>Returns write status</returns>
        public bool ReadWriteStatus()
        {
            return GetReadWriteStatus();
        }

        /// <summary>
        /// Returns the last error status of data store
        /// </summary>
        /// <returns>Returns last error status of data store</returns>
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
            dataId++;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(dataId, MaxDataSize);
        }*/

        /// <summary>
        /// Create a data store element, specify the maximum size of data element and the ID of the first data record to be created
        /// </summary>
        /// <param name="StartdataId">Data ID of the first data recrod to be created</param>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        /*private DataStore(UInt32 StartDataId, UInt32 MaxDataSize)
        {
            dataId = StartDataId;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(dataId, MaxDataSize);
        }*/     

        /// <summary>
        /// Write data array to the data store starting from the address (passed as first parameter)
        /// </summary>
        /// <param name="address">Starting address from which to start writing the data</param>
        /// <param name="data">Data array to be written to data store</param>
        /// <returns>Returns 4 (number of bytes written) if successful, returns -1 if operation fails.</returns>
        public bool Write(UInt32 address, byte[] data)
        {
            //byte[] buffer = new byte[sizeof(int)];
            return Write(address, data, (UInt32)data.Length, storageType);
        }

        /// <summary>
        /// Write data array to the data store starting from the address (passed as first parameter). Amount of data to be written is specified by numBytes (third parameter)
        /// </summary>
        /// <param name="address">Starting address from which to start writing the data</param>
        /// <param name="data">Data array to be written to data store</param>
        /// <param name="numBytes">Amount of data to be written</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        public bool Write(UInt32 address, byte[] data, UInt32 numBytes)
        {
            return Write(address, data, numBytes, storageType);
        }

        /// <summary>
        /// Read a byte array from the data store into data array starting from address (first parameter)
        /// </summary>
        /// <param name="address">Starting address from which to start reading</param>
        /// <param name="data">Byte array to be filled up with data from data store</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        public bool Read(UInt32 address, byte[] data)
        {
            return Read(address, data, storageType);
        }

        /// <summary>
        /// Destroys the record in the data store
        /// </summary>
        ~DataStore()
        {
            //DisposeNativeMemoryPointer(dataId);
        }

        ///////////////////////////////////Internal methods/////////////////////////

        /// <summary>
        /// Returns the current dataID 
        /// </summary>
        /// <returns>Current dataID</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public UInt32 GetDataID();

        /// <summary>
        /// Creates data for the specified size in the block storage device. Allocates space, assigns a reference which is returned back to the user
        /// </summary>
        /// <param name="Size">Size of the data</param>
        /// <returns>Reference to the created data</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int CreateData(UInt32 Size);

        /// <summary>
        /// Initializes the data store. Performs a scan of the device and builds up the list of active data inside the data store.
        /// </summary>
        /// <returns>True or false</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool CreateDataStore();

        /// <summary>
        /// Get amount of used space 
        /// </summary>
        /// <returns>Amount of used space</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private float GetUsedKBytes();

        /// <summary>
        /// Get amount of free space
        /// </summary>
        /// <returns>Amount of free space</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private float GetFreeKBytes();

        /// <summary>
        /// Returns valid data currently stored in the data store
        /// </summary>
        /// <returns>Returns array with valid data as unsigned integer</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool GetReadAllDataIds(int[] dataIdArray);

        /*[MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool GetReadAllDataReferences(int[] dataArray, UInt32 CountOfData);*/

        /// <summary>
        /// Get the count of valid data currently stored in the data store
        /// </summary>
        /// <returns>Returns the count of valid data as unsigned integer</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private UInt32 GetCountOfDataIds();

        /*[MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private UInt32 GetCountOfDataReferences();*/

        /// <summary>
        /// Delete all references to data in the data store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static bool DeleteAll();

        /// <summary>
        /// Erases all data in the data store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static bool EraseAllBlocks();

        /// <summary>
        /// Garbage collects inactive records in data store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static bool DataStoreGC();

        /// <summary>
        /// Gets read/write status of the data store
        /// </summary>
        /// <returns>Returns status of read/write on data store</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static bool GetReadWriteStatus();

        /// <summary>
        /// Gets error status of data Store
        /// </summary>
        /// <returns>Returns error status of data store</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static int GetLastErrorStatus();

        /// <summary>
        /// Read from data into the buffer starting from address.
        /// </summary>
        /// <param name="address">Address of data to be read from</param>
        /// <param name="buffer">Buffer into which the data will be read</param>
        /// <param name="storageType">Block storage type</param>
        /// <returns>Returns number of bytes read, returns -1 if operation fails</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool Read(UInt32 address, byte[] buffer, int storageType);

        /// <summary>
        /// Read the data record IDs currently stored in the NVM into the unsigned integer array provided
        /// </summary>
        /// <param name="buffer">Buffer into which the data record IDs will be read into</param>
        /// <returns>Returns the number of data iDs read</returns>
        /*[MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public UInt16 ReadAllDataIDs(UInt32[] buffer);*/
        
        /// <summary>
        /// Delete the data represented by the dataID from data store.
        /// </summary>
        /// <param name="dataID">ID of the data to be deleted</param>
        /// <returns>Returns success or failure of operation</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool Delete(UInt32 dataId);

        //Returns the size of the NVM, if 0 operation failed
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern private UInt32 ConstructNativeMemoryPointer(UInt32 dataId, UInt32 bytes);

        /// <summary>
        /// Write data present in buffer starting from address and amount equal to numBytes. 
        /// </summary>
        /// <param name="address">Address from which to start writing data</param>
        /// <param name="data">Buffer holding data to be written</param>
        /// <param name="numBytes">Amount of data to be written</param>
        /// <param name="storageType">Block storage type</param>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool Write(UInt32 address, byte[] data, UInt32 numBytes, int storageType);

        // native call that destroys record created on the flash
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern private bool DisposeNativeMemoryPointer(UInt32 dataId);
        
    }
}
