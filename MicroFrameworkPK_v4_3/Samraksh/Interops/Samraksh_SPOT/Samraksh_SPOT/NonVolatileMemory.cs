/* Copyright: The Samaraksh Company (All rights reserved)
 * Author: Mukundan Sridharan, Ananth Muralidharan
 * Date: Feb 2013, Nov 2013
 * Function: Provides access to Samraksh NVM DataStore filesystem
 */

using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;


namespace Samraksh.SPOT.NonVolatileMemory
{
    /// <summary>
    /// Block device type
    /// </summary>
    public enum StorageType
    {
        NOR = 1,
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

    //AnanthAtSamraksh - to be kept in sync with the one in Samraksh_SPOT_Samraksh_SPOT_NonVolatileMemory_DataStore.cpp
    /// <summary>
    /// DataStore status types
    /// </summary>
    public enum DATASTORE_RETURN_STATUS
    {
        Success = 0,
        Failure = -1,
        InvalidArgument = -2,
        InvalidReference = -3
        //AlreadyExists,
        //InvalidPointer
    };

    public class DataStoreException : Exception
    {
        public DataStoreException()
        {
        }

        public DataStoreException(string message)
            : base(message)
        {
        }

        public DataStoreException(string message, Exception inner)
            : base(message, inner)
        {
        }
    }

    /// <summary>
    /// Data class. Provides functions to write, read and delete data. 
    /// Provides a reference/handle to the data stored in the block storage device to the user.
    /// </summary>
    public class DataReference
    {
        // Data Id unique to a pointer
        private UInt32 dataId;
        private UInt32 m_Size = 0;
        private UInt32 dataReference;
        private Type dataType;

        private Byte dataTypeByte = new byte();
        private UInt16 dataTypeUInt16 = new UInt16();
        private UInt32 dataTypeUInt32 = new UInt32();

        DataStore dStore;

        /// <summary>
        /// Data constructor that takes DataStore and the data type as parameters. 
        /// Possible to create max of 256 active data objects.
        /// </summary>
        /// <param name="dStore">DataStore object</param>
        /// <param name="dataType">Type of data to be stored in data store</param>
        public DataReference(DataStore dStore, Type dataType)
        {
            this.dStore = dStore;
            this.dataType = dataType;
        }

        /// <summary>
        /// Default data class constructor. Possible to create max of 256 active data objects.
        /// </summary>
        /// <param name="dStore">DataStore object</param>
        /// <param name="m_Size">Size of the data object to be stored in data store. 
        /// Max size is (2^32 - 1) if type is bytes; (2^31 - 1) if type is uint16; (2^30 - 1) if type is uint32</param>
        /// <param name="dataType">Type of data to be stored in data store</param>
        public DataReference(DataStore dStore, int m_Size, Type dataType)
        {
            this.dStore = dStore;
            this.dataType = dataType;

            //Size allocation should not be zero or negative
            if (m_Size <= 0)
            {
                throw new ArgumentException("m_Size cannot be less than or equal to zero", "m_size");
            }

            if (dataType == dataTypeByte.GetType())
            {
                m_Size = sizeof(byte) * m_Size;
                this.dataReference = (UInt32)CreateData((UInt32)m_Size, 0);
            }
            else if (dataType == dataTypeUInt16.GetType())
            {
                m_Size = sizeof(UInt16) * m_Size;
                this.dataReference = (UInt32)CreateData((UInt32)m_Size, 1);
            }
            else if (dataType == dataTypeUInt32.GetType())
            {
                m_Size = sizeof(UInt32) * m_Size;
                this.dataReference = (UInt32)CreateData((UInt32)m_Size, 2);
            }
            else
            {
                throw new ArgumentException("Invalid dataType", "dataType");
            }

            if (this.dataReference == 0)
                throw new DataStoreException("DataPointer is zero. Data could not be created.");

            this.m_Size = (UInt32)m_Size;
            this.dataId = GetDataID();

            if(this.dataId == 0)
                throw new DataStoreException("DataId is zero. Data could not be created.");
        }

        /// <summary>
        /// Returns a data object corresponding to the input parameter dataId.
        /// Possible to create max of 256 active data objects.
        /// </summary>Rea
        /// <param name="dStore">DataStore object</param>
        /// <param name="dataId">ID of data for which the user wants a reference/handle</param>
        public DataReference(DataStore dStore, int dataId)
        {
            this.dStore = dStore;
            this.dataId = (UInt32)dataId;
            this.dataReference = (UInt32)LookupData((UInt32)dataId);
            if (this.dataReference == 0)
                throw new DataStoreException("Reference to data is NULL. Data was not created.");

            this.m_Size = (UInt32)LookupSize((UInt32)dataId);
            if (this.m_Size == 0)
                throw new DataStoreException("Data size is zero. Data was not created.");

            int retType = LookupDataType((UInt32)dataId);
            if (retType == 0)
                this.dataType = typeof(byte);
            else if (retType == 1)
                this.dataType = typeof(UInt16);
            else if (retType == 2)
                this.dataType = typeof(UInt32);
            //m_Size = ConstructNativeMemoryPointer(dataId, MaxDataSize);
        }

        /// <summary>
        /// Returns the last error status of data store.
        /// </summary>
        /// <returns>Returns success, failure or error status</returns>
        /*public DATASTORE_RETURN_STATUS GetStatus()
        {
            if (dStore.LastErrorStatus() == (int)DATASTORE_ERROR.DATASTORE_ERROR_DATA_ID_ALREADY_EXISTS)
                return DATASTORE_RETURN_STATUS.AlreadyExists;
            else if (dStore.LastErrorStatus() == (int)DATASTORE_ERROR.DATASTORE_ERROR_NONE)
                return DATASTORE_RETURN_STATUS.Success;

            return DATASTORE_RETURN_STATUS.Failure;
        }*/

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
        /// <param name="offset">Offset from which to write to data store. Recommended to be an even value (only for byte data type)</param>
        /// <param name="numData">Count of data to be written to data store</param>
        /// <returns>Returns success or failure</returns>
        public DATASTORE_RETURN_STATUS Write(byte[] data, int offset, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (offset < 0)
                throw new DataStoreException("Offset should not be negative");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.dataType != dataTypeByte.GetType())
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be written is greater than array size");

            //Amount of data to be written should not be greater than allocation size
            if (numData > this.m_Size)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            //Amount of data to be written from offset should not be greater than allocation size
            if (offset + numData > this.m_Size)
                throw new DataStoreException("Amount of data to be written exceeds size of data allocated");

            //Since the NOR flash cannot write to odd addresses.
            if (offset % 2 == 1)
            {
                //offset = offset + sizeof(byte);
                throw new DataStoreException("Offset should not be odd for byte dataType");
            }

            retVal = dStore.Write((uint)this.dataReference, data, (UInt32)offset, (UInt32)numData, dataTypeByte);
            if (retVal == 0)
                return DATASTORE_RETURN_STATUS.Success;
            else if (retVal == -3)
                return DATASTORE_RETURN_STATUS.InvalidReference;
            else
                return DATASTORE_RETURN_STATUS.Failure;

        }

        /// <summary>
        /// Write a UInt16 array into the data store
        /// </summary>
        /// <param name="data">UInt16 array to be written to data store</param>
        /// <param name="offset">Offset within the data array from which to write to data store</param>
        /// <param name="numData">Count of data to be written to data store</param>
        /// <returns>Returns success or failure</returns>
        public DATASTORE_RETURN_STATUS Write(UInt16[] data, int offset, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (offset < 0)
                throw new DataStoreException("Offset should not be negative");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.dataType != dataTypeUInt16.GetType())
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be written is greater than array size");

            //Amount of data to be written should not be greater than allocation size
            if (numData > this.m_Size)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");
            
            //Amount of data to be written from offset should not be greater than allocation size
            if (offset + numData > this.m_Size)
                throw new DataStoreException("Amount of data to be written exceeds size of data allocated");

            UInt32 numBytes = sizeof(UInt16) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            for (uint objIndex = 0; objIndex < numData; ++objIndex)
            {
                byteArray[objIndex * sizeof(UInt16) + 0] = (byte)(data[objIndex] >> 8);
                byteArray[objIndex * sizeof(UInt16) + 1] = (byte)(data[objIndex]);
            }

            retVal = dStore.Write((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeUInt16);

            if (retVal == 0)
                return DATASTORE_RETURN_STATUS.Success;
            else if (retVal == -3)
                return DATASTORE_RETURN_STATUS.InvalidReference;
            else
                return DATASTORE_RETURN_STATUS.Failure;
        }

        /// <summary>
        /// Write a UInt32 array into the data store
        /// </summary>
        /// <param name="data">UInt32 array to be written to data store</param>
        /// <param name="offset">Offset within the data array from which to write to data store</param>
        /// <param name="numData">Count of data to be written to data store</param>
        /// <returns>Returns success or failure</returns>
        public DATASTORE_RETURN_STATUS Write(UInt32[] data, int offset, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (offset < 0)
                throw new DataStoreException("Offset should not be negative");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.dataType != dataTypeUInt32.GetType())
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be written is greater than array size");

            //Amount of data to be written should not be greater than allocation size
            if (numData > this.m_Size)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");
            
            //Amount of data to be written from offset should not be greater than allocation size
            if (offset + numData > this.m_Size)
                throw new DataStoreException("Amount of data to be written exceeds size of data allocated");

            UInt32 numBytes = sizeof(UInt32) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            for (uint objIndex = 0; objIndex < numData; ++objIndex)
            {
                byteArray[objIndex * sizeof(UInt32) + 0] = (byte)(data[objIndex] >> 24);
                byteArray[objIndex * sizeof(UInt32) + 1] = (byte)(data[objIndex] >> 16);
                byteArray[objIndex * sizeof(UInt32) + 2] = (byte)(data[objIndex] >> 8);
                byteArray[objIndex * sizeof(UInt32) + 3] = (byte)(data[objIndex]);
            }

            retVal = dStore.Write((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeUInt32);

            if (retVal == 0)
                return DATASTORE_RETURN_STATUS.Success;
            else if (retVal == -3)
                return DATASTORE_RETURN_STATUS.InvalidReference;
            else
                return DATASTORE_RETURN_STATUS.Failure;
        }

        /// <summary>
        /// Write a byte array into the data store
        /// </summary>
        /// <param name="data">Byte array to be written to data store</param>
        /// <param name="numData">Count of data to be written to data store</param>
        /// <returns>Returns success or failure</returns>
        public DATASTORE_RETURN_STATUS Write(byte[] data, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.dataType != dataTypeByte.GetType())
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be written is greater than array size");

            //Amount of data to be written should not be greater than allocation size
            if (numData > this.m_Size)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            retVal = dStore.Write((uint)this.dataReference, data, 0, (UInt32)numData, dataTypeByte);

            if (retVal == 0)
                return DATASTORE_RETURN_STATUS.Success;
            else if (retVal == -3)
                return DATASTORE_RETURN_STATUS.InvalidReference;
            else
                return DATASTORE_RETURN_STATUS.Failure;
        }

        /// <summary>
        /// Write a UInt16 array into the data store
        /// </summary>
        /// <param name="data">UInt16 array to be written to data store</param>
        /// <param name="numData">Count of data to be written to data store</param>
        /// <returns>Returns success or failure</returns>
        public DATASTORE_RETURN_STATUS Write(UInt16[] data, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.dataType != dataTypeUInt16.GetType())
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be written is greater than array size");

            //Amount of data to be written should not be greater than allocation size
            if (numData > this.m_Size)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            UInt32 numBytes = sizeof(UInt16) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            for (uint objIndex = 0; objIndex < numData; ++objIndex)
            {
                byteArray[objIndex * sizeof(UInt16) + 0] = (byte)(data[objIndex] >> 8);
                byteArray[objIndex * sizeof(UInt16) + 1] = (byte)(data[objIndex]);
            }

            retVal = dStore.Write((uint)this.dataReference, byteArray, 0, numBytes, dataTypeUInt16);

            if (retVal == 0)
                return DATASTORE_RETURN_STATUS.Success;
            else if (retVal == -3)
                return DATASTORE_RETURN_STATUS.InvalidReference;
            else
                return DATASTORE_RETURN_STATUS.Failure;
        }

        /// <summary>
        /// Write a UInt32 array into the data store
        /// </summary>
        /// <param name="data">UInt32 array to be written to data store</param>
        /// <param name="numData">Count of data to be written to data store</param>
        /// <returns>Returns success or failure</returns>
        public DATASTORE_RETURN_STATUS Write(UInt32[] data, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be written should be the same as that created during allocation
            if (this.dataType != dataTypeUInt32.GetType())
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be written should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be written is greater than array size");

            //Amount of data to be written should not be greater than allocation size
            if (numData > this.m_Size)
                throw new DataStoreException("Amount of data to be written is greater than size of data allocated");

            UInt32 numBytes = sizeof(UInt32) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            for (uint objIndex = 0; objIndex < numData; ++objIndex)
            {
                byteArray[objIndex * sizeof(UInt32) + 0] = (byte)(data[objIndex] >> 24);
                byteArray[objIndex * sizeof(UInt32) + 1] = (byte)(data[objIndex] >> 16);
                byteArray[objIndex * sizeof(UInt32) + 2] = (byte)(data[objIndex] >> 8);
                byteArray[objIndex * sizeof(UInt32) + 3] = (byte)(data[objIndex]);
            }

            retVal = dStore.Write((uint)this.dataReference, byteArray, 0, numBytes, dataTypeUInt32);

            if (retVal == 0)
                return DATASTORE_RETURN_STATUS.Success;
            else if (retVal == -3)
                return DATASTORE_RETURN_STATUS.InvalidReference;
            else
                return DATASTORE_RETURN_STATUS.Failure;
        }


        /// <summary>
        /// Read a byte array from the data store into the array passed as parameter
        /// </summary>
        /// <param name="data">Array to be filled up with data for the corresponding data object</param>
        /// <param name="offset">Offset from source from which to read</param>
        /// <param name="numData">Count of data to be read</param>
        /// <returns>Returns if read was a success or a failure</returns>
        public DATASTORE_RETURN_STATUS Read(byte[] data, int offset, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (offset < 0)
                throw new DataStoreException("Offset should not be negative");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be read should be the same as that created during allocation
            if (this.dataType != dataTypeByte.GetType())
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be read should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be read is greater than array size");

            //Amount of data to be read should not be greater than allocation size
            if (numData > this.m_Size)
                throw new DataStoreException("Amount of data to be read is greater than size of data allocated");

            //Amount of data to be read from offset should not be greater than allocation size
            if (offset + numData > this.m_Size)
                throw new DataStoreException("Amount of data to be read exceeds size of data allocated");

            UInt32 numBytes = sizeof(byte) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            retVal = dStore.Read((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeByte);

            if (retVal == 0)
            {
                for (uint objIndex = 0; objIndex < numData; ++objIndex)
                {
                    data[objIndex] = (byte)(byteArray[objIndex * sizeof(byte)]);
                }
                return DATASTORE_RETURN_STATUS.Success;
            }
            else if (retVal == -3)
                return DATASTORE_RETURN_STATUS.InvalidReference;
            else
                return DATASTORE_RETURN_STATUS.Failure;
        }

        /// <summary>
        /// Read a UInt16 array from the data store into the array passed as parameter
        /// </summary>
        /// <param name="data">Array to be filled up with UInt16 data</param>
        /// <param name="offset">Offset from source from which to read</param>
        /// <param name="numData">Count of data to be read</param>
        /// <returns>Returns if read was a success or a failure</returns>
        public DATASTORE_RETURN_STATUS Read(UInt16[] data, int offset, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (offset < 0)
                throw new DataStoreException("Offset should not be negative");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be read should be the same as that created during allocation
            if (this.dataType != dataTypeUInt16.GetType())
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be read should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be read is greater than array size");

            //Amount of data to be read should not be greater than allocation size
            if (numData > this.m_Size)
                throw new DataStoreException("Amount of data to be read is greater than size of data allocated");

            //Amount of data to be read from offset should not be greater than allocation size
            if (offset + numData > this.m_Size)
                throw new DataStoreException("Amount of data to be read exceeds size of data allocated");

            UInt32 numBytes = sizeof(UInt16) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            retVal = dStore.Read((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeUInt16);

            if (retVal == 0)
            {
                for (uint objIndex = 0; objIndex < numData; ++objIndex)
                {
                    data[objIndex] = (UInt16)((byteArray[objIndex * sizeof(UInt16) + 0] << 8) +
                                                        byteArray[objIndex * sizeof(UInt16) + 1]);
                }
                return DATASTORE_RETURN_STATUS.Success;
            }
            else if (retVal == -3)
                return DATASTORE_RETURN_STATUS.InvalidReference;
            else
                return DATASTORE_RETURN_STATUS.Failure;
        }

        /// <summary>
        /// Read a UInt32 array from the data store into the array passed as parameter
        /// </summary>
        /// <param name="data">Array to be filled up with UInt32 data</param>
        /// <param name="offset">Offset from source from which to read</param>
        /// <param name="numData">Count of data to be read</param>
        /// <returns>Returns if read was a success or a failure</returns>
        public DATASTORE_RETURN_STATUS Read(UInt32[] data, int offset, int numData)
        {
            int retVal = 0;

            if (data.Length == 0)
                throw new DataStoreException("data array cannot be of zero length");

            if (offset < 0)
                throw new DataStoreException("Offset should not be negative");

            if (numData < 0)
                throw new DataStoreException("numData should not be negative");

            if (this.dataReference == 0x0)
                throw new DataStoreException("Reference to data is null");

            //DataType to be read should be the same as that created during allocation
            if (this.dataType != dataTypeUInt32.GetType())
                throw new DataStoreException("DataType is not the same as the one during creation");

            //Amount of data to be read should not be greater than size of input array
            if (numData > data.Length)
                throw new DataStoreException("Amount of data to be read is greater than array size");

            //Amount of data to be read should not be greater than allocation size
            if (numData > this.m_Size)
                throw new DataStoreException("Amount of data to be read is greater than size of data allocated");

            //Amount of data to be read from offset should not be greater than allocation size
            if (offset + numData > this.m_Size)
                throw new DataStoreException("Amount of data to be read exceeds size of data allocated");

            UInt32 numBytes = sizeof(UInt32) * (UInt32)numData;
            byte[] byteArray = new byte[numBytes];

            retVal = dStore.Read((uint)this.dataReference, byteArray, (UInt32)offset, numBytes, dataTypeUInt32);

            if (retVal == 0)
            {
                for (uint objIndex = 0; objIndex < numData; ++objIndex)
                {
                    data[objIndex] = (UInt32)((byteArray[objIndex * sizeof(UInt32) + 0] << 24) +
                                                    (byteArray[objIndex * sizeof(UInt32) + 1] << 16) +
                                                    (byteArray[objIndex * sizeof(UInt32) + 2] << 8) +
                                                    byteArray[objIndex * sizeof(UInt32) + 3]);
                }
                return DATASTORE_RETURN_STATUS.Success;
            }
            else if (retVal == -3)
                return DATASTORE_RETURN_STATUS.InvalidReference;
            else
                return DATASTORE_RETURN_STATUS.Failure;
        }

        /// <summary>
        /// Delete a data from the data store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        public DATASTORE_RETURN_STATUS Delete()
        {
            if (DeleteData(dataId) == true)
                return DATASTORE_RETURN_STATUS.Success;
            else
                return DATASTORE_RETURN_STATUS.Failure;
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
        /// <param name="dataId">ID of the data to be looked up</param>
        /// <returns>Returns address/reference to the dataID</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int LookupData(UInt32 dataId);

        /// <summary>
        /// Get the data type of dataID
        /// </summary>
        /// <param name="dataId">ID of the data to be looked up</param>
        /// <returns>Returns data type</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int LookupDataType(UInt32 dataId);

        /// <summary>
        /// Get the allocation size for dataID
        /// </summary>
        /// <param name="dataId">ID of the data to be looked up</param>
        /// <returns>Returns allocation size</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int LookupSize(UInt32 dataId);

        /// <summary>
        /// Delete the data represented by the dataId from data store.
        /// </summary>
        /// <param name="dataId">ID of the data to be deleted</param>
        /// <returns>Returns true or false</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool DeleteData(UInt32 dataId);

        /// <summary>
        /// Creates data for the specified size in the block storage device. Allocates space, assigns a reference which is returned back to the user
        /// </summary>
        /// <param name="Size">Size of the data</param>
        /// <param name="dataTypeByte">Data type - byte</param>
        /// <returns>Reference to the created data</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int CreateData(UInt32 Size, byte dataTypeByte);

        /// <summary>
        /// Creates data for the specified size in the block storage device. Allocates space, assigns a reference which is returned back to the user
        /// </summary>
        /// <param name="Size">Size of the data</param>
        /// <param name="dataTypeUInt16">Data type - UInt16</param>
        /// <returns>Reference to the created data</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int CreateData(UInt32 Size, UInt16 dataTypeUInt16);

        /// <summary>
        /// Creates data for the specified size in the block storage device. Allocates space, assigns a reference which is returned back to the user
        /// </summary>
        /// <param name="Size">Size of the data</param>
        /// <param name="dataTypeUInt32">Data type - UInt32</param>
        /// <returns>Reference to the created data</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int CreateData(UInt32 Size, UInt32 dataTypeUInt32);

        /// <summary>
        /// Returns the current dataID 
        /// </summary>
        /// <returns>Current dataID</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private UInt32 GetDataID();


        /*/// <summary>
        /// 
        /// </summary>
        /// <param name="dataId"></param>
        /// <param name="bytes"></param>
        /// <returns></returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int ConstructNativeMemoryPointer(UInt32 dataId, UInt32 bytes);*/

        /// <summary>
        /// Native call that destroys data created on the flash
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
        
        //private UInt32 m_Size = 0;
        //private UInt16 Blocks=0;
        //private float UsedKiloBytes=0;
        //private float FreeKiloBytes=0;

        // Stores the current data id
        //private static UInt32 currentdataId = 0;

        /// <summary>
        /// The block storage device type
        /// </summary>
        private StorageType storageType;

        /// <summary>
        /// The only instance of DataStore.
        /// </summary>
        private static DataStore DSInstance;
        /// <summary>
        /// Counter for multiton pattern 
        /// </summary>
        //private static int counter = 0;
        /// <summary>
        /// For synchronization between multiple threads
        /// </summary>
        private static object syncObject = new Object();
        /// <summary>
        /// HashTable for storing multiple instances of DataStore initialized to different block storage devices
        /// </summary>
        private static System.Collections.Hashtable dataStoreInstances = new System.Collections.Hashtable();

        /// <summary>
        /// Private constructor so that only one instance of DataStore exists.
        /// </summary>
        private DataStore() { }

        /// <summary>
        /// Multiton pattern. This allows the DataStore to be initialized with different block storage devices such as NOR, NAND etc.
        /// http://en.wikipedia.org/wiki/Multiton_pattern
        /// </summary>
        public static DataStore Instance(StorageType storageTypeKey)
        {
            if (!dataStoreInstances.Contains(storageTypeKey))
            {
                lock (syncObject)
                {
                    DSInstance = new DataStore();
                    dataStoreInstances.Add(storageTypeKey, DSInstance);
                    DSInstance.InitDataStore(storageTypeKey);
                }
            }
            return (DataStore)(dataStoreInstances[storageTypeKey]);
        }

        /*/// <summary>
        /// DataStore constructor that takes a storageType as parameter and initializes the data store with that storageType.
        /// </summary>
        /// <param name="storageType">storageType can be NOR, SD card or any other block storage device</param>
        private DataStore(int storageType)
        {
            this.storageType = storageType;
            bool retVal = CreateDataStore();
            /*if( retVal == false )
                throw new SystemException();
        }*/

        /// <summary>
        /// Takes a storageType as parameter and initializes the data store with that storageType.
        /// </summary>
        /// <param name="storageType">storageType can be NOR, SD card or any other block storage device</param>
        private bool InitDataStore(StorageType storageType)
        {
            this.storageType = storageType;
            return CreateDataStore();
        }

        //////////////////////////public properties and methods/////////////////////

        /// <summary>
        /// Maximum allocation size possible
        /// </summary>
        public float MaxAllocationSize
        {
            get
            {
                return GetMaxAllocationSize();
            }
        }

        /// <summary>
        /// Size of the DataStore in bytes
        /// </summary>
        public float Size
        {
            get
            {
                return GetTotalSpace();
            }
        }

        /// <summary>
        /// Number of bytes of space already occupied by data
        /// </summary>
        public float UsedBytes
        {
            get
            {
                return GetUsedBytes();
            }
        }

        /// <summary>
        /// Number of bytes of space available to store data
        /// </summary>
        public float FreeBytes
        {
            get
            {
                return GetFreeBytes();
            }
        }

        /*public int TempCreateData(UInt32 dataId, UInt32 Size)
        {
            return CreateData(dataId, Size);
        }*/

        /*public void addDataReference(UInt32 dataId)
        {
            dataObj.Add(dataId);
        }*/

        /// <summary>
        /// Fills up the array passed as parameter with all dataIDs
        /// </summary>
        /// <param name="dataIdArray">Array which is filled up with the dataIDs</param>
        /// <param name="dataIdOffset">Get all dataIDs from offset</param>
        /// <returns>Returns success or failure</returns>
        public DATASTORE_RETURN_STATUS ReadAllDataIds(int[] dataIdArray, int dataIdOffset)
        {
            if (dataIdArray.Length == 0)
                throw new DataStoreException("dataIdArray cannot be of zero length");

            if (dataIdOffset < 0)
                throw new DataStoreException("dataIdOffset should not be negative");

            int retVal = GetReadAllDataIds(dataIdArray, dataIdArray.Length, dataIdOffset);

            if (retVal == 0)
                return DATASTORE_RETURN_STATUS.Success;
            else if (retVal == -3)
                return DATASTORE_RETURN_STATUS.InvalidReference;
            else
                return DATASTORE_RETURN_STATUS.Failure;
        }

        /*/// <summary>
        /// Returns the count of dataIDs
        /// </summary>
        /// <returns>Returns count of dataIDs</returns>
        public int CountOfDataIds()
        {
            return GetCountOfDataIds();
        }*/

        /// <summary>
        /// Fills up the array (passed as first parameter) with the dataIDs starting from offset (passed as second parameter)
        /// </summary>
        /// <param name="dataRefArray">Array that holds the references to the data</param>
        /// <param name="dataIdOffset">Offset from which the array references are to be fetched</param>
        /// <returns>Returns success or failure</returns>
        public DATASTORE_RETURN_STATUS ReadAllDataReferences(DataReference[] dataRefArray, int dataIdOffset)
        {
            UInt32 dataId;
            if (dataRefArray.Length == 0)
                throw new DataStoreException("dataRefArray cannot be of zero length");

            if (dataIdOffset < 0)
                throw new DataStoreException("dataIdOffset should not be negative");

            /* User checks if array contents are null to break out of program. 
             * Clear the contents of array so that null values are returned if there is no data allocation in DataStore. */
            Array.Clear(dataRefArray, 0, dataRefArray.Length);
            int[] dataIdArray = new int[dataRefArray.Length];
            
            int retVal = GetReadAllDataIds(dataIdArray, dataIdArray.Length, dataIdOffset);

            if (retVal != 0)
                return DATASTORE_RETURN_STATUS.Failure;
            else
            {
                for (UInt16 arrayIndex = 0; arrayIndex < dataRefArray.Length; ++arrayIndex)
                {
                    dataId = (UInt32)dataIdArray[arrayIndex];
                    if (dataId != 0)
                    {
                        try
                        {
                            dataRefArray[arrayIndex] = new DataReference(this, (int)dataId);
                        }
                        catch (Exception)
                        {
                            return DATASTORE_RETURN_STATUS.Failure;
                        }
                    }
                }
            }
            return DATASTORE_RETURN_STATUS.Success;
        }

        /// <summary>
        /// Deletes all dataIDs from the data store. Does not actually erase the data, but marks them as inactive.
        /// </summary>
        /// <returns>Returns success or failure</returns>
        public static DATASTORE_RETURN_STATUS DeleteAllData()
        {
            int retVal = DeleteAll();

            if (retVal == 0)
                return DATASTORE_RETURN_STATUS.Success;
            else if (retVal == -3)
                return DATASTORE_RETURN_STATUS.InvalidReference;
            else
                return DATASTORE_RETURN_STATUS.Failure;
        }

        /// <summary>
        /// Performs garbage collection on the data store. This is done internally by data store when there is not enough space to add new data.
        /// GC rearranges data internally so that all active data are grouped together. Then the inactive data are erased one block at a time.
        /// </summary>
        /// <returns>Returns success or failure</returns>
        private static DATASTORE_RETURN_STATUS GC()
        {
            if (DataStoreGC() == true)
                return DATASTORE_RETURN_STATUS.Success;
            else
                return DATASTORE_RETURN_STATUS.Failure;
        }

        /// <summary>
        /// Deletes all dataIDs as well as erases all data from data store.
        /// </summary>
        /// <returns>Returns success or failure</returns>
        public static DATASTORE_RETURN_STATUS EraseAllData()
        {
            // Remove contents from AddressTable also before erasing DataStore
            int retVal = DeleteAll();

            if (retVal == 0)
            {
                //DeleteAll was successful
                retVal = EraseAllBlocks();

                if (retVal == 0)
                    return DATASTORE_RETURN_STATUS.Success;
                else if (retVal == -3)
                    return DATASTORE_RETURN_STATUS.InvalidReference;
                else
                    return DATASTORE_RETURN_STATUS.Failure;
            }
            else if (retVal == -3)
                return DATASTORE_RETURN_STATUS.InvalidReference;
            else
                return DATASTORE_RETURN_STATUS.Failure;
        }

        /*/// <summary>
        /// Returns write status 
        /// </summary>
        /// <returns>Returns write status</returns>
        public bool ReadWriteStatus()
        {
            return GetReadWriteStatus();
        }*/

        /*/// <summary>
        /// Returns the last error status of data store
        /// </summary>
        /// <returns>Returns last error status of data store</returns>
        public int LastErrorStatus()
        {
            return GetLastErrorStatus();
        }*/


        /*/// <summary>
        /// Create a data store element, specify the maximum size of data element
        /// </summary>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        private DataStore(UInt32 MaxDataSize)
        {
            dataId++;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(dataId, MaxDataSize);
        }*/

        /*/// <summary>
        /// Create a data store element, specify the maximum size of data element and the ID of the first data to be created
        /// </summary>
        /// <param name="StartdataId">Data ID of the first data recrod to be created</param>
        /// <param name="MaxDataSize">Size of the bigggest data element which can be stored</param>
        private DataStore(UInt32 StartDataId, UInt32 MaxDataSize)
        {
            dataId = StartDataId;
            buffer = new byte[MaxDataSize];
            m_Size = ConstructNativeMemoryPointer(dataId, MaxDataSize);
        }*/

        /// <summary>
        /// Write data array to the data store starting from the address (passed as first parameter)
        /// </summary>
        /// <param name="address">Starting address from which to start writing the data</param>
        /// <param name="data">Data to be written to data store</param>
        /// <param name="offset">Offset from which to start writing</param>
        /// <param name="numBytes">Count of bytes to be written</param>
        /// <param name="dataType">Data type to be written</param>
        /// <returns>Returns true if successful and false if not</returns>
        public int Write(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, byte dataType)
        {
            return Write(address, data, offset, numBytes, dataType, (int)storageType);
        }

        public int Write(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, UInt16 dataType)
        {
            return Write(address, data, offset, numBytes, dataType, (int)storageType);
        }

        public int Write(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, UInt32 dataType)
        {
            return Write(address, data, offset, numBytes, dataType, (int)storageType);
        }

        /// <summary>
        /// Write data array to the data store starting from the address (passed as first parameter). Amount of data to be written is specified by numBytes (third parameter)
        /// </summary>
        /// <param name="address">Starting address from which to start writing the data</param>
        /// <param name="data">Data array to be written to data store</param>
        /// <param name="numBytes">Amount of data to be written</param>
        /// <param name="dataType">Data type to be written</param>
        /// <returns>Returns the number of bytes written, returns -1 if operation failes</returns>
        /*public bool Write(UInt32 address, byte[] data, UInt32 numBytes)
        {
            //return Write(address, data, numBytes, dataType, storageType);
            return Write(address, data, numBytes, storageType);
        }

        public bool Write(UInt32 address, byte[] data, UInt32 numBytes, UInt16 dataType)
        {
            return Write(address, data, numBytes, dataType, storageType);
        }

        public bool Write(UInt32 address, byte[] data, UInt32 numBytes, UInt32 dataType)
        {
            return Write(address, data, numBytes, dataType, storageType);
        }*/

        /// <summary>
        /// Read a byte array from the data store into data array starting from address (first parameter)
        /// </summary>
        /// <param name="address">Starting address from which to start reading</param>
        /// <param name="data">Byte array to be filled up with data from data store</param>
        /// <param name="offset">Offset from which to start reading</param>
        /// <param name="numBytes">Number of bytes to be read</param>
        /// <param name="dataType">Data type of the data to be read</param>
        /// <returns>Returns true if successful and false if not</returns>
        public int Read(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, byte dataType)
        {
            return Read(address, data, offset, numBytes, dataType, (int)storageType);
        }

        public int Read(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, UInt16 dataType)
        {
            return Read(address, data, offset, numBytes, dataType, (int)storageType);
        }

        public int Read(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, UInt32 dataType)
        {
            return Read(address, data, offset, numBytes, dataType, (int)storageType);
        }

        /// <summary>
        /// Destroys the data in the data store
        /// </summary>
        ~DataStore()
        {
            //DisposeNativeMemoryPointer(dataId);
        }

        ///////////////////////////////////Internal methods/////////////////////////

        /// <summary>
        /// Initializes the data store. Performs a scan of the device and builds up the list of active data inside the data store.
        /// </summary>
        /// <returns>True or false</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool CreateDataStore();

        /*/// <summary>
        /// Creates data for the specified size in the block storage device. Allocates space, assigns a reference which is returned back to the user
        /// </summary>
        /// <param name="Size">Size of the data</param>
        /// <param name="dataTypeByte">Data type - byte</param>
        /// <returns>Reference to the created data</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int CreateData(UInt32 Size, byte dataTypeByte);*/

        /*/// <summary>
        /// Creates data for the specified size in the block storage device. Allocates space, assigns a reference which is returned back to the user
        /// </summary>
        /// <param name="Size">Size of the data</param>
        /// <param name="dataTypeUInt16">Data type - UInt16</param>
        /// <returns>Reference to the created data</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int CreateData(UInt32 Size, UInt16 dataTypeUInt16);*/

        /*/// <summary>
        /// Creates data for the specified size in the block storage device. Allocates space, assigns a reference which is returned back to the user
        /// </summary>
        /// <param name="Size">Size of the data</param>
        /// <param name="dataTypeUInt32">Data type - UInt32</param>
        /// <returns>Reference to the created data</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public int CreateData(UInt32 Size, UInt32 dataTypeUInt32);*/

        /*/// <summary>
        /// Returns the current dataID 
        /// </summary>
        /// <returns>Current dataID</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public UInt32 GetDataID();*/

        /// <summary>
        /// Method that returns maximum allocation size possible in data store
        /// </summary>
        /// <returns>Maximum allocation size possible in data store</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private float GetMaxAllocationSize();

        /// <summary>
        /// Get total available space 
        /// </summary>
        /// <returns>Amount of total space</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private float GetTotalSpace();

        /// <summary>
        /// Get amount of used space 
        /// </summary>
        /// <returns>Amount of used space</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private float GetUsedBytes();

        /// <summary>
        /// Get amount of free space
        /// </summary>
        /// <returns>Amount of free space</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private float GetFreeBytes();

        /// <summary>
        /// Returns valid data currently stored in the data store
        /// </summary>
        /// <returns>Returns array with valid data as unsigned integer</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int GetReadAllDataIds(int[] dataIdArray, int arrayLength, int dataIdOffset);

        /// <summary>
        /// Get the count of valid data currently stored in the data store
        /// </summary>
        /// <returns>Returns the count of valid data as unsigned integer</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int GetCountOfDataIds();

        /// <summary>
        /// Delete all references to data in the data store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static int DeleteAll();

        /// <summary>
        /// Erases all data in the data store
        /// </summary>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private static int EraseAllBlocks();

        /// <summary>
        /// Garbage collects inactive data in data store
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

        /*/// <summary>
        /// Delete the data represented by the dataID from data store.
        /// </summary>
        /// <param name="dataId">ID of the data to be deleted</param>
        /// <returns>Returns success or failure of operation</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool Delete(UInt32 dataId);*/

        //Returns the size of the NVM, if 0 operation failed
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern private UInt32 ConstructNativeMemoryPointer(UInt32 dataId, UInt32 bytes);

        /// <summary>
        /// Write data present in buffer starting from address and amount equal to numBytes. 
        /// </summary>
        /// <param name="address">Address from which to start writing data</param>
        /// <param name="data">Buffer holding data to be written</param>
        /// <param name="offset"></param>
        /// <param name="numBytes">Amount of data to be written</param>
        /// <param name="dataType">Data type of the data to be written</param>
        /// <param name="storageType">Block storage type</param>
        /// <returns>Returns success or failure</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Write(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, byte dataType, int storageType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Write(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, UInt16 dataType, int storageType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Write(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, UInt32 dataType, int storageType);

        /*[MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool Write(UInt32 address, byte[] data, UInt32 offset, UInt32 numBytes, int storageType);*/

        /*[MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool Write(UInt32 address, byte[] data, UInt32 numBytes, int storageType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool Write(UInt32 address, byte[] data, UInt32 numBytes, UInt16 dataType, int storageType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool Write(UInt32 address, byte[] data, UInt32 numBytes, UInt32 dataType, int storageType);*/


        /// <summary>
        /// Read from data into the buffer starting from address.
        /// </summary>
        /// <param name="address">Address of data to be read from</param>
        /// <param name="buffer">Buffer into which the data will be read</param>
        /// <param name="offset">Offset from which to start reading</param>
        /// <param name="numBytes">Number of bytes to be read</param>
        /// <param name="dataType">Data type of the data to be read</param>
        /// <param name="storageType">Block storage type</param>
        /// <returns>Returns number of bytes read, returns -1 if operation fails</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Read(UInt32 address, byte[] buffer, UInt32 offset, UInt32 numBytes, byte dataType, int storageType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Read(UInt32 address, byte[] buffer, UInt32 offset, UInt32 numBytes, UInt16 dataType, int storageType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private int Read(UInt32 address, byte[] buffer, UInt32 offset, UInt32 numBytes, UInt32 dataType, int storageType);


        // native call that destroys data created on the flash
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern private bool DisposeNativeMemoryPointer(UInt32 dataId);

    }
}
