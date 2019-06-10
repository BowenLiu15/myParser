

#include <iostream>
#include "tinyxml2.h"
#include <string.h>
#include "ParserIFace.h"
#include <stdlib.h>


using namespace tinyxml2;

/* According to the type read from the XML element, 
   set the type in the parser node */
void setPType(ParserIFace* parserNode, XMLElement* xmlNode)
{
	char* typeName = (char*)xmlNode->Attribute("type");
	if (typeName != NULL)
	{
		if (strcmp(typeName, "UINT8") == 0)
		{
			parserNode->setPType(P_TYPE_UINT8);
		}

		if (strcmp(typeName, "UINT16") == 0)
		{
			parserNode->setPType(P_TYPE_UINT8);
		}

		if (strcmp(typeName, "UINT32") == 0)
		{
			parserNode->setPType(P_TYPE_UINT8);
		}

	}
}

/* Read all the avaliable attributes in the XML element,
   set the attribute names and flag in the parser node */
void setParserAttributes(ParserIFace* parserNode, XMLElement* xmlNode)
{
	char* name = (char*)xmlNode->Attribute("name");
	if (name != NULL)
	{
		parserNode->setName(name);
	}

	int isEnum = xmlNode->IntAttribute("isENUM");
	if (isEnum == 1)
	{
		parserNode->setIsEnum();
	}

	char* dllName = (char*)xmlNode->Attribute("Dll");
	if (dllName != NULL)
	{
		parserNode->setDllName(dllName);
	}

	char* class_name = (char*)xmlNode->Attribute("class");
	if (class_name != NULL)
	{
		parserNode->setClassName(class_name);
	}

	char* createFnName = (char*)xmlNode->Attribute("createFn");
	if (createFnName != NULL)
	{
		parserNode->setCreateFnName(createFnName);
	}
	char* createDeleteName = (char*)xmlNode->Attribute("deleteFn");
	if (createDeleteName != NULL)
	{
		parserNode->setDeleteFnName(createDeleteName);
	}

	char* value = (char*)xmlNode->Attribute("value");
	if (value != NULL)
	{
		int v = (int)strtol(value, NULL, 16);

		parserNode->setValue(v);
	}
	

	setPType(parserNode, xmlNode);
}

/* A recursive help function to build the parser tree */
ParserIFace* buildXmlTreeHelp(XMLElement* xmlNode, int level)
{

	//deal with the current element and build a node for it.
	ParserIFace* newNode = NULL;
	char* name = (char*)xmlNode->Attribute("name");
	P_CLASS_TYPE pType = GENERIC_PARSER;
	if (name != NULL)
	{
		if (strcmp(name, "Usp") == 0)
		{
			pType = USP_PARSER;
		}
		else if (strcmp(name, "Ssh") == 0)
		{
			pType = SSH_PARSER;
		}
		else if (strcmp(name, "HID") == 0)
		{
			pType = HID_PARSER;
		}
	}

	newNode = (ParserIFace*)new_Parser(pType);
	// check for NULL
	if (newNode != NULL)
	{
		newNode->setLevel(level);
		setParserAttributes(newNode, xmlNode);
		//deal with the children
		XMLElement* child = xmlNode->FirstChildElement();
		if (child != NULL)
		{

			while (child != NULL)
			{
				newNode->addChild(buildXmlTreeHelp(child, level + 1)); // recursive call
				child = child->NextSiblingElement();
			}
		}
	}



	return newNode;
}

/* The function to build the parser tree */
ParserIFace* buildXmlTree(const char* xmlFileName)
{
	XMLDocument doc;
	doc.LoadFile(xmlFileName);
	int errorID = doc.ErrorID();
	if (errorID != 0)
	{
		printf("Cannot open file %s\n", xmlFileName);
		printf("errorId: %d\n", errorID);
		return NULL;
	}
	XMLElement* root = doc.FirstChildElement("root");
	if (root == NULL)
	{
		std::cout << "no element root!!\n";
		return NULL;
	}
	else
	{
		return buildXmlTreeHelp(root,0);
	}


}


/* The main function to open XML and build the Parser Tree*/
int main(void)
{
	printf("Hallo World\n");

	ParserIFace* parserTreeRoot = buildXmlTree("xmlParser.xml");
	if (parserTreeRoot == NULL)
	{
		printf("faill!!!!\n");
	}
	if (parserTreeRoot->hasChildren() != 0)
	{
		printf("it gets children nodes\n");
	}

    /*parserTreeRoot->checkNameTest();*/
	printf("\n\n");
	char buffer[128] = { 0xAA, 0x55, 0x80, 0x02, 0x03, 0x04, 0x05, 0x06,  0xAA, 0x55, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xAA, 0x55, 0x01, 0x02, 0x03, 
		0x04, 0x05, 0x06, 0xAA, 0x55, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xAA, 0x55, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xAA, 0x55, 0x01, 0x02, 0x03, 
		0x04, 0x05, 0x06 };
	parserTreeRoot->Run(buffer);


	return 0;

}
