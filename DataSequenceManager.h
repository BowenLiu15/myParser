#include <string>
#include <iostream>

#define BUFFER_SIZE 20
//Add to buffer, update the write index, detect end of buffer to wrap around.
//	If data is more than 10 bytes(8 bytes USP + 2 bytes CRC) check if first 2 bytes are 0xAA, 0x55 and the checksum(byte 8 & 9) matches with the CRC of the header.
//	If crc match, then get the packet length and keep getting data until the whole packet is complete.
//	Once the full packet is found, push it to the processing packet list(queue), and set an event LIST_HAS_DATA.And restart the search again.
//	Once the input buffer is processed, check if the queue has any packet, then pass them to process.

using namespace std;

//// do the class:
//typedef struct _SequenceLength
//{
//	void* pData;
//	int	  size;
//} SequenceLength;

class SequenceLength
{
public:
	SequenceLength(): s_Size(0), pData(NULL)
	{

	}

	SequenceLength(void* data, int size)
	{
		s_Size = size;
		pData = data;
	}

	

	~SequenceLength()
	{

	}

	void* getData()
	{
		return pData;
	}

protected:
	int	  s_Size;
	void* pData;
};

class DataSequenceManager
{

public:

	DataSequenceManager(): bytesInBuffer(0), s_writeIndex(0), s_readIndex(0)
	{
		memset(dataBuffer, 0x00, sizeof(dataBuffer));
	}

	~DataSequenceManager()
	{

	}

	void readIOData()
	{
		char temp[256];
		cin.getline(temp, 256);
		size_t dataLength = strlen(temp);
		writeSequence(temp, dataLength);
	}

	void handleIOPacket()
	{
		while (bytesInBuffer < 3) //should be 10
		{
			readIOData();
		}
		if (bytesInBuffer >= 3) // should be 10
		{
			//printf("the data is greater than 10!\n");
			//if (checkHeader() == 0)
			//{
				//int dataLength = readLength();
				int dataLength = 10;
				while (bytesInBuffer < dataLength)
				{
					readIOData();
				}
				int length = bytesInBuffer;
				
				//string* completeBuffer = new string(temp, dataLength);
				char* completeBuffer = new char[length + 1];
				if (completeBuffer != NULL)
				{
					// char* pbuff = new char[bytesInBuffer + sizeof(SequenceLength)];
					readSequence(completeBuffer, bytesInBuffer);
					SequenceLength* bufferContainer = new SequenceLength(completeBuffer, length);
					completeBuffers.Enqueue(bufferContainer);
				}

			//}
		}
	}

	int checkHeader()
	{

		char firstByte = seeOneByte();
		char secondByte = seeWithOffset(1);

		if (firstByte == 0xAA && secondByte == 0x55)
		{
			if (checkCrc() == 0)
			{
				return 0;
			}
		}

		return 1;
	}

	int checkCrc()
	{
		char sum = 0;
		for (int i = 0; i < 6; i++)
		{
			sum += seeWithOffset(i);
		}

		if (sum != seeWithOffset(6))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	unsigned short int readLength()
	{
		unsigned short int sh = (seeWithOffset(5) << 8 | seeWithOffset(4));
		return sh;
	}

	char seeOneByte()
	{
		char ch = dataBuffer[s_readIndex];
		return ch;
	}

	char seeWithOffset(size_t offset)
	{
		char ch;
		if (offset > ((BUFFER_SIZE - 1) - s_readIndex))
		{
			ch = dataBuffer[offset + s_readIndex - (BUFFER_SIZE - 1)];
		}
		else
		{
			ch = dataBuffer[s_readIndex + offset];
		}
		return ch;
	}

	void readSequence(char* dest, size_t size)
	{
		int avaliable = BUFFER_SIZE - s_readIndex;

		if (size > avaliable)
		{
			int rest = size - avaliable;
			strncpy(dest, &dataBuffer[s_readIndex], avaliable);
			strncpy(dest + avaliable, dataBuffer, rest);
			
			s_readIndex = rest + 1;

		}
		else
		{
			strncpy(dest, &dataBuffer[s_readIndex], avaliable);
			s_readIndex += size;
		}
		dest[size] = '\0';
		bytesInBuffer -= size;
	}

	void writeOneByte(char ch)
	{
		dataBuffer[s_writeIndex] = ch;
		if (s_writeIndex == (BUFFER_SIZE - 1))
		{
			s_writeIndex = 0;
		}
		else
		{
			s_writeIndex += 1;
		}
	}

	void writeSequence(char* sequence, size_t size)
	{
		int avaliable = BUFFER_SIZE - s_writeIndex;
		if (size > (avaliable))
		{
			strncpy(&dataBuffer[s_writeIndex], sequence, avaliable);
			strncpy(&dataBuffer[0], sequence + avaliable, size - avaliable);
			s_writeIndex = size - avaliable + 1;
			bytesInBuffer += size;
		}
		else
		{
			strcpy(&dataBuffer[s_writeIndex], sequence);
			s_writeIndex += size;
			bytesInBuffer += size;
		}
	}

	void printSequence()
	{
		if (completeBuffers.getCount() > 0)
		{
			SequenceLength* sequence = (SequenceLength*)completeBuffers.Dequeue();
			printf("%s\n", (char*)sequence->getData());
		}

	}

protected:

	int bytesInBuffer;
	int s_writeIndex;
	int s_readIndex;
	char dataBuffer[BUFFER_SIZE];
	SimpleQueue completeBuffers;

};