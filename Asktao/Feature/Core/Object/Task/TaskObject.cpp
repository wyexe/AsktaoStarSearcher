#include "TaskObject.h"
#include <Feature/Core/Object/VecGameObject.h>


CTaskObject::CTaskObject(_In_ DWORD dwBase) : _dwBase(dwBase)
{

}

DWORD CTaskObject::GetBase() CONST
{
	return _dwBase;
}

DWORD CTaskObject::GetAttributeTreeHead() CONST
{
	return RD(RD(GetBase() + 0x4 + 0x4) + 0x4);
}

CONST std::wstring CTaskObject::GetTaskType() CONST
{
	std::wstring wsValue;
	return CVecGameObject::FindObjectAttributeValue_By_Key(GetAttributeTreeHead(), "task_type", wsValue) ? wsValue : L"无效字符";
}

CONST std::wstring CTaskObject::GetRawType() CONST
{
	std::wstring wsValue;
	return CVecGameObject::FindObjectAttributeValue_By_Key(GetAttributeTreeHead(), "raw_type", wsValue) ? wsValue : L"无效字符";
}

CONST std::wstring CTaskObject::GetTaskPrompt() CONST
{
	std::wstring wsValue;
	return CVecGameObject::FindObjectAttributeValue_By_Key(GetAttributeTreeHead(), "task_prompt", wsValue) ? wsValue : L"无效字符";
}

CONST std::wstring CTaskObject::GetTaskTip() CONST
{
	std::wstring wsValue;
	return CVecGameObject::FindObjectAttributeValue_By_Key(GetAttributeTreeHead(), "task_tip", wsValue) ? wsValue : L"无效字符";
}
