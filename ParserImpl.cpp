#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#define _CRT_SECURE_NO_WARNINGS 1

#include "ParserIFace.h" 
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* Implementation of the Parser */


class ParserImpl : public ParserIFace
{
public:

	ParserImpl() : attributes(0), pType(P_TYPE_NONE), pBuffer(NULL)
	{
		memset(dllName, 0x00, sizeof(dllName));
		memset(className, 0x00, sizeof(className));
		memset(createFnName, 0x00, sizeof(createFnName));
		memset(deleteFnName, 0x00, sizeof(deleteFnName));
		memset(name, 0x00, sizeof(name));
	}

	~ParserImpl()
	{
		printf("ParserImpl's virtual destructor is called.\n");
	}

	virtual char* Run(char* buffer)
	{
		return NULL;
	}

	void setPType(P_TYPE p_type)
	{
		attributes |= P_ATTRIB_TYPE;
		pType = p_type;
	}

	P_TYPE getPType(void)
	{
		return pType;
	}

	void setIsEnum(void)
	{
		attributes |= P_ATTRIB_ENUM;
	}

	void setValue(int newValue)
	{
		pValue = newValue;
		attributes |= P_ATTRIB_VALUE;
	}

	int getValue()
	{
		return pValue;
	}

	void setLevel(int newLevel)
	{
		level = newLevel;
	}

	char* getName()
	{
		return name;
	}

	void setName(char* newName)
	{
		attributes |= P_ATTRIB_NAME;
		strcpy(name, newName);
	}

	char* getDllName()
	{
		return dllName;
	}

	void setDllName(char* newName)
	{
		attributes |= P_ATTRIB_DLL;
		strcpy(dllName, newName);
	}

	char* getClassName()
	{
		return className;
	}

	void setClassName(char* newName)
	{
		attributes |= P_ATTRIB_CLASS;
		strcpy(className, newName);
	}

	char* getCreateFnName()
	{
		return createFnName;
	}

	void setCreateFnName(char* newName)
	{
		attributes |= P_ATTRIB_CREATE_FN;
		strcpy(createFnName, newName);
	}

	char* getDeleteFnName()
	{
		return deleteFnName;
	}

	void setDeleteFnName(char* newName)
	{
		attributes |= P_ATTRIB_DELETE_FN;
		strcpy(deleteFnName, newName);
	}

	void setDataLength(int length)
	{
		attributes |= P_ATTRIB_LENGTH;
		s_dataLength = length;
	}

	int hasChildren()
	{
		return childrenQueue.getCount();
	}

	void setBuffer(char* pDataBuffer)
	{
		pBuffer = pDataBuffer;
	}

	virtual void setPayload(ParserIFace* payload)
	{
		pPayload = payload;
		attributes |= P_ATTRIB_PAYLOAD;
	}

	/* Check whether the parser has payload */
	int hasPayload()
	{
		return(attributes & P_ATTRIB_PAYLOAD);
	}

	ParserIFace* getPayload()
	{
		return pPayload;
	}

	void checkNameTest()
	{
		// print the name of the current node
		char* name = getName();
		for (int i = 0; i < level; i++)
		{
			printf("  ");
		}
		if (name[0] != '\0')
		{
			printf("<%s>\n", name);
		}
		else
		{
			// if the node has no name
			printf("<no name node: enum or catogery>\n");
		}

		// handle all the children nodes
		while (hasChildren() != 0)
		{
			ParserIFace* child = (ParserIFace*)childrenQueue.Dequeue();
			if (child != NULL)
			{
				child->checkNameTest();    // recursive call here
			}
			else
			{
				printf("something wrong with the queue\n");
			}
		}

		// print the end of the current node
		for (int i = 0; i < level; i++)
		{
			printf("  ");
		}
		if (name[0] != '\0')
		{
			printf("<%s\\>\n", name);
		}
		else
		{
			printf("<no name node: enum or catogery\\>\n");
		}

	}

	void* getFirstChild()
	{
		return childrenQueue.GetFirst();
	}

	void* getNextChild()
	{
		return childrenQueue.GetNext();
	}

	void addChild(void* newChild)
	{
		childrenQueue.Enqueue(newChild);
	}

	/* 
	 * Return the name of the parser member, inside a 
	 * enumeration parser node, whose value match the name ID
	 */

	virtual char* enumHandle(int nameID)
	{
		// if the enumeration doesn't have member just return.

		if (hasChildren() == 0)
		{
			return NULL;
		}

		// print the name of the node.
		ParserImpl* enumList = (ParserImpl*)childrenQueue.GetFirst();
		char* name = enumList->getName();
		if (name[0] != '\0')
		{
			printf("%s: ", name);
		}

		// go through the members and find the match
		ParserImpl* enumNode = (ParserImpl*)enumList->childrenQueue.GetFirst();
		while (enumNode != NULL)
		{
			if (nameID == enumNode->getValue())
			{

				printf("%s, ", enumNode->getName());
				return enumNode->getName();
			}
			enumNode = (ParserImpl*)enumList->childrenQueue.GetNext();
		}

		printf("cannot find match, ");
		return NULL;
	}

	virtual ParserIFace* searchByValue(int value)
	{

		if (hasChildren() == 0)
		{
			return NULL;
		}
		ParserIFace* child = (ParserIFace*)getFirstChild();
		while (child != NULL)
		{
			if (child->getValue() == value)
			{
				return child;
			}
			ParserIFace* found = child->searchByValue(value);
			if (found != NULL)
			{
				return found;
			}
			child = (ParserIFace*)getNextChild();
		}
		return NULL;
	}

	virtual ParserIFace* searchByName(char* name)
	{

		if (hasChildren() == 0)
		{
			return NULL;
		}

		ParserIFace* child = (ParserIFace*)getFirstChild();
		while (child != NULL)
		{
			if (!strcmp(child->getName(),name))
			{
				return child;
			}
			ParserIFace* found = child->searchByName(name);
			if (found != NULL)
			{
				return found;
			}
			child = (ParserIFace*)getNextChild();
		}
		return NULL;
	}




	virtual void registerPrintFunction(fPutS func)
	{
		putS = func;
	}

protected:

	bool isEnum()
	{
		return((attributes & P_ATTRIB_ENUM) != 0);
	}

	/* Hanlde the enumeration types.
	 * print the correct member whose value matched the given data byte. 
	 */
	
	void printf(const char* format, ...)
	{
		// create str
		char buffer[128];
		// print to string

		va_list args;
		va_start(args, format);
		vsprintf(buffer, format, args);
		va_end(args);
		if (putS != NULL)
		{
			putS(buffer);
		}
		else
		{
			puts(buffer);
		}
	}

protected:

	SimpleQueue    childrenQueue;			  // A queue contains all the child parser nodes
	int            level;					  // Level in the tree
	int            pValue;					  // Expected value matching this node
	unsigned int   attributes;                // Attribute flags 
	char           dllName[4096];
	char           className[4096];
	char           createFnName[4096];
	char           deleteFnName[4096];
	P_TYPE         pType;				      // Data type of this node	
	char           name[4096];			      // Name buffer
	char*          pBuffer;				      // Local data buffer
	int            s_dataLength;              // How many pType data need to read
	ParserIFace*    pPayload;
	fPutS          putS;

};



class ParserGenericImpl : public ParserImpl
{
public:

	ParserGenericImpl()
	{
		attributes = 0;
	}

	~ParserGenericImpl()
	{

	}
	
	virtual char* Run(char* buffer)
	{
		if (buffer == NULL)
		{
			return NULL;
		}


		char* newBuffer = action(buffer);
		if (isEnum())
		{
			return newBuffer;
		}
		newBuffer = dispatch(newBuffer);
		return newBuffer;
	}

protected:

	/**
     *  A function to handle the current parser node.
     *	Consume some data bytes from the data buffer,
     *  print out the information and data,
     *  and return the updated data buffer.
     */
	virtual char* action(char* pBuffer)
	{

		// print out all the avaliable attributes 
		if ((attributes & P_ATTRIB_NAME) != 0)
		{
			printf("Name: %s, ", name);
		}
		if ((attributes & P_ATTRIB_DLL) != 0)
		{
			printf("DLL NAME: %s, ", dllName);
		}

		if ((attributes & P_ATTRIB_VALUE) != 0)
		{
			printf("Expected Data Value: 0x%02X, ", getValue());
		}

		if ((attributes & P_ATTRIB_CLASS) != 0)
		{
			printf("CLASS NAME: %s, ", className);
		}

		if ((attributes & P_ATTRIB_CREATE_FN) != 0)
		{
			printf("CREATE FN NAME: %s, ", createFnName);
		}

		if ((attributes & P_ATTRIB_DELETE_FN) != 0)
		{
			printf("DELETE FN NAME: %s.", deleteFnName);
		}

		// according to the data type, consume correct number of bytes.
		switch (pType)
		{
		case P_TYPE_UINT8:
		{
			// read one byte
			if (s_dataLength & P_ATTRIB_LENGTH != 0)
			{
				char localBuffer[1024];
				memset(localBuffer, 0x00, sizeof(localBuffer));
				strncpy(localBuffer, pBuffer, s_dataLength);
				printf("DATA: %s\n", localBuffer);
				return pBuffer + s_dataLength;
			}

			unsigned char ch = pBuffer[0];


			if (isEnum())
			{
				enumHandle(ch); // handle the enumeration type 
			}
			printf("DATA: %02X\n", ch);
			if (strlen(pBuffer) > 1)
			{

				return (pBuffer + 1); // return updated the buffer
			}
			else
			{
				return NULL;
			}
		}
		case P_TYPE_UINT16:
		{
			// read two bytes
			unsigned short int sh = (pBuffer[1] << 8) | (pBuffer[0]);

			if (isEnum())
			{
				enumHandle(sh); // handle the enumeration type 
			}
			printf("DATA: %04X\n", sh);
			if (strlen(pBuffer) > 2)
			{

				return (pBuffer + 2); // return updated the buffer
			}
			else
			{
				return NULL;
			}
		}
		case P_TYPE_UINT32:
		{
			// read four bytes
			unsigned int wd = (pBuffer[3] << 24) | (pBuffer[2] << 16) | (pBuffer[1] << 8) | (pBuffer[0]);

			if (isEnum())
			{
				enumHandle(wd); // handle the enumeration type 
			}
			printf("DATA: %08X\n", wd);
			if (strlen(pBuffer) > 4)
			{

				return (pBuffer + 4); // return updated the buffer
			}
			else
			{

				return NULL;
			}
		}
		case P_TYPE_NONE:
		{
			printf("\n");
			return pBuffer;
		}

		default:
		{
			printf("\n");
			return pBuffer;
		}

		}

	}

	/**
	 *  A recursive function to use action() to handle the children nodes.
	 */
	virtual char* dispatch(char* pBuffer)
	{

		unsigned int i = hasChildren();
		// Do the recursive calls for all its child nodes
		if (i > 0)
		{
			if (pBuffer != NULL)
			{
				char* pBufferTemp = pBuffer;
				ParserImpl* pParser = (ParserImpl*)childrenQueue.GetFirst();
				while (pParser != NULL)
				{
					pBufferTemp = pParser->
						Run(pBufferTemp); // recursive call here
					pParser = (ParserImpl*)childrenQueue.GetNext();
				}
				return pBufferTemp;
			}
			else
			{
				printf("No more data to feed, stop parsering!!\n");
				return NULL;
			}
		}
		else
		{
			return pBuffer;
		}
	}
};


class ParserUspImpl : public ParserGenericImpl
{
public:
	ParserUspImpl()
	{

	}
	~ParserUspImpl()
	{

	}
	// virtual char* Run(char* buffer)
	// {
	//	return NULL;
	//
	//
	// }

protected:

};


class ParserSshImpl : public ParserImpl
{
public:

	ParserSshImpl()
	{
		memset(categoryName, 0x00, sizeof(categoryName));
		memset(commandName, 0x00, sizeof(commandName));
	}

	~ParserSshImpl()
	{

	}

	/* Handle the SSH parser to parse the data correctly */
	virtual char* Run(char* buffer)
	{
		// print out all the avaliable attributes 
		if ((attributes & P_ATTRIB_NAME) != 0)
		{
			printf("Name: %s, ", name);
		}
		else
		{
			printf("No name ELEMENT: enum or Categoty.");
		}

		if ((attributes & P_ATTRIB_DLL) != 0)
		{
			printf("DLL NAME: %s, ", dllName);
		}

		if ((attributes & P_ATTRIB_VALUE) != 0)
		{
			printf("Expected Data Value: 0x%02X, ", getValue());
		}

		if ((attributes & P_ATTRIB_CLASS) != 0)
		{
			printf("CLASS NAME: %s, ", className);
		}

		if ((attributes & P_ATTRIB_CREATE_FN) != 0)
		{
			printf("CREATE FN NAME: %s, ", createFnName);
		}

		if ((attributes & P_ATTRIB_DELETE_FN) != 0)
		{
			printf("DELETE FN NAME: %s. \n", deleteFnName);
		}

		pBuffer = buffer;
		
		/** Read the TransportProtocol*/
		ParserImpl* child = (ParserImpl*)getFirstChild();
		pBuffer = child->Run(pBuffer);
		
		/* Read the Category and store the category name*/
		child = (ParserImpl*)getNextChild();
		categoryNodeAction(child);

		/* Read the targetId sourceId instanceId and requestId*/
		for (int i = 0; i < 4; i++)
		{
			child = (ParserImpl*)getNextChild();
			pBuffer = child->Run(pBuffer);
		}

		/* Read the Command Name */
		child = (ParserImpl*)getNextChild();
		commandNodeAction(child);
		return pBuffer;



	}
protected:

private:
	void categoryNodeAction(ParserImpl* categoryChild)
	{
		// according to the data type, consume correct number of bytes.
		printf("%s :", categoryChild->getName());

		// read four bytes
		unsigned int wd = (pBuffer[3] << 24) | (pBuffer[2] << 16) | (pBuffer[1] << 8) | (pBuffer[0]);
		char* cat_name = categoryChild->enumHandle(wd);
		if (cat_name != NULL)
		{
			strcpy(categoryName, cat_name); // handle the enumeration type 
		}

		printf("DATA: %08X\n", wd);

		pBuffer = pBuffer + 4; // return updated the buffer

	}

	void commandNodeAction(ParserImpl* commandIdChild)
	{
		// according to the data type, consume correct number of bytes.
		printf("%s: ", commandIdChild->getName());

		// read one byte
		if (categoryName[0] != '\0')
		{
			searchCommandId(commandIdChild, categoryName);
		}

		if (strlen(pBuffer) > 1)
		{

			pBuffer = pBuffer + 1; // return updated the buffer
		}
		else
		{
			pBuffer = NULL;
		}


	}

	void searchCommandId(ParserImpl* commandIdChild, char* enumName)
	{
		// if the enumeration doesn't have member just return.
		unsigned char nameData = pBuffer[0];
		if (hasChildren() == 0)
		{
			return;
		}

		ParserImpl* caregoryNode = (ParserImpl*)commandIdChild->searchByName(enumName);

		if (caregoryNode != NULL)
		{
			ParserImpl* enumNode = (ParserImpl*)caregoryNode->searchByValue(nameData);
			if (enumNode != NULL)
			{
				printf("%s, ", enumNode->getName());
				strcpy(commandName, enumNode->getName());
			}


			if (caregoryNode->hasPayload() > 0)
			{
				searchPayload(caregoryNode->getPayload());
			}

		}
		else
		{
			printf("Cannot find match\n");
		}
	}

	void searchPayload(ParserIFace* payloadFile)
	{
		ParserIFace* payloadNode = payloadFile->searchByName(commandName);
		if (payloadNode != NULL)
		{
			pBuffer = payloadNode->Run(pBuffer);
		}
	}


private:
	char categoryName[128];
	char commandName[128];
};

class ParserHIDImpl : public ParserGenericImpl
{
public:
	ParserHIDImpl()
	{

	}
	~ParserHIDImpl()
	{

	}

protected:

};

/* Return the specified type of parser. If the argument is not valid, return NULL */
void* __stdcall new_Parser(P_CLASS_TYPE arg)
{
	switch (arg)
	{
	case USP_PARSER:
		return new ParserUspImpl;
	case SSH_PARSER:
		return new ParserSshImpl;
	case HID_PARSER:
		return new ParserHIDImpl;
	case GENERIC_PARSER:
		return new ParserGenericImpl;
	default:
		return NULL;
	}

}

/* Free the ParserIFace pointer. If it deletes successfully, return 0; otherwise return 1*/
int __stdcall delete_Parser(void* pVoid)
{
	ParserIFace* pParser = (ParserIFace*)pVoid;
	ParserUspImpl* pParserUsp = dynamic_cast<ParserUspImpl*>(pParser);
	if (pParserUsp != NULL)
	{
		delete pParserUsp;
		return 0;
	}

	ParserHIDImpl* pParserHID = dynamic_cast<ParserHIDImpl*>(pParser);
	if (pParserUsp != NULL)
	{
		delete pParserUsp;
		return 0;
	}

	ParserSshImpl* pParserSsh = dynamic_cast<ParserSshImpl*>(pParser);
	if (pParserUsp != NULL)
	{
		delete pParserUsp;
		return 0;
	}

	ParserGenericImpl* pParserGeneric = dynamic_cast<ParserGenericImpl*>(pParser);
	if (pParserUsp != NULL)
	{
		delete pParserUsp;
		return 0;
	}

	return 1;
}