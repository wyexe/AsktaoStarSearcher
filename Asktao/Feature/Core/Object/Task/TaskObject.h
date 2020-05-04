#ifndef __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_TASK_TASKOBJECT_H__
#define __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_TASK_TASKOBJECT_H__

#include <Feature/Common/GameStruct.h>

class CTaskObject
{
public:
	CTaskObject(_In_ DWORD dwBase);
	~CTaskObject() = default;

	//
	DWORD GetBase() CONST;

	// 
	DWORD GetAttributeTreeHead() CONST;

	//
	CONST std::wstring GetTaskType() CONST;

	//
	CONST std::wstring GetRawType() CONST;

	//
	CONST std::wstring GetTaskPrompt() CONST;

	//
	CONST std::wstring GetTaskTip() CONST;
private:
	DWORD _dwBase;
};



#endif // !__ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_TASK_TASKOBJECT_H__
