#define P_ATTRIB_DLL		(1 << 0)
#define P_ATTRIB_CLASS		(1 << 1)
#define P_ATTRIB_CREATE_FN  (1 << 2)
#define P_ATTRIB_DELETE_FN	(1 << 3)
#define P_ATTRIB_TYPE		(1 << 4)
#define P_ATTRIB_NAME		(1 << 5)
#define P_ATTRIB_VALUE		(1 << 6)
#define P_ATTRIB_STRUCT		(1 << 6)

#include "SimpleQueue.h"


typedef enum _P_CLASS_TYPE
{
	USP_PARSER,
	SSH_PARSER,
	HID_PARSER,
	GENERIC_PARSER,
} P_CLASS_TYPE;

typedef enum _P_TYPE
{
	P_TYPE_UINT8,
	P_TYPE_UINT16,
	P_TYPE_UINT32,
	P_TYPE_NONE,
} P_TYPE;

typedef enum _P_STRUCT_TYPE
{
	P_ENUM,
	P_CATEGORY,

} P_STRUCT_TYPE;


class ParserIFace
{
public:

	virtual ~ParserIFace() = 0;

	virtual char* Run(char* buffer) = 0;

	virtual void setPType(P_TYPE p_type) = 0;

	virtual P_TYPE getPType(void) = 0;

	virtual void setValue(int newValue) = 0;

	virtual int getValue(void) = 0;

	virtual void setPStructType(P_STRUCT_TYPE p_type) = 0;

	virtual P_STRUCT_TYPE getPStructType(void) = 0;

	virtual void setName(char* newName) = 0;

	virtual char* getName(void) = 0;

	virtual char* getDllName() = 0;

	virtual void setDllName(char* newName) = 0;

	virtual char* getClassName() = 0;

	virtual void setClassName(char* newName) = 0;

	virtual char* getCreateFnName() = 0;

	virtual void setCreateFnName(char* newName) = 0;

	virtual char* getDeleteFnName() = 0;

	virtual void setDeleteFnName(char* newName) = 0;

	virtual int hasChildren() = 0;

	virtual void setBuffer(char* pDataBuffer) = 0;

	virtual void setLevel(int level) = 0;
	virtual void checkNameTest() = 0;
	virtual void* getChild() = 0;
	virtual void addChild(void* newChild) = 0;
};

inline ParserIFace::~ParserIFace()
{
	printf("ParserIFace's virtual destructor is called.\n");
}


typedef void* (*f_new_Parser)(P_CLASS_TYPE);
typedef int (*f_delete_Parser)(void*);

extern "C" void* __stdcall new_Parser(P_CLASS_TYPE arg);
extern "C" int __stdcall delete_Parser(void* pVoid);

// handle = LoadLibrary("Parder.dll", 0);

// f_new_Parser new_ParserFn = GetProcAdress("new_Parser", ...);
// ParserIFace* pParser = new_ParserFn(USP_CLASS);