/*
 * Copyright The Samraksh Company
 *
 * Name : 		 message.h
 *
 * Author : 	 Mukundan.Sridharan@samraksh.com, nived.sivadas@samraksh.com
 *
 * Description : defines the interface for message, the theory is that all protocols would be required to
 *				 implement this interface thereby ensuring a common template across multiple higher layers
 *
 */

// template for message header
template<class Header_T,class Footer_T,class PayLoad_T,class Metadata_T>
class Message
{
	Header_T header;
	Footer_T footer;
	Metadata_T metadata;
	PayLoad_T payload;

public:
	// Returns a pointer to the message
	Message* GetMessage()
	{
		return this;
	}

	// returns the size of the message packet
	UINT16 	 GetMessageSize()
	{
		return header.size() + footer.size() + metadata.size() + sizeof(PayLoadSize_T);
	}


	// returns a pointer to the header
	Header_T* GetHeader()
	{
		return &header;
	}

	// Returns the size of the header
	// ensure that the mac header class has a size function
	UINT16 GetHeaderSize()
	{
		return header.size();
	}

	// returns a pointer to the payload
	PayLoad_T* GetPayload()
	{
		return payload;
	}

	// returns the payload size
	UINT16 GetPayloadSize()
	{
		return payload.size();
	}

	// returns a pointer to the footer
	Footer_T* GetFooter()
	{
		return &footer;
	}

	// returns the size of the footer
	UINT16 GetFooterSize()
	{
		return footer.size();
	}

	// returns a pointer to the metadata
	Metadata_T* GetMetaData()
	{
		return &metadata;
	}

	// returns the metadata size
	UINT16 GetMetaDataSize()
	{
		return metadata.size();
	}

};
