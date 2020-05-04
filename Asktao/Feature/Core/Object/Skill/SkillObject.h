#ifndef __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_SKILL_SKILLOBJECT_H__
#define __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_SKILL_SKILLOBJECT_H__

#include <Feature/Common/GameStruct.h>

class CSkillObject
{
public:
	CSkillObject(_In_ DWORD dwNode);
	~CSkillObject() = default;

	//
	DWORD GetAttributeHead() CONST;

	//
	VOID PrintAttribute() CONST;

	//
	std::wstring GetName() CONST;

	//
	DWORD GetSkillObject() CONST;

	//
	DWORD GetBase() CONST;
private:
	DWORD _dwNode;
};



#endif // !__ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_SKILL_SKILLOBJECT_H__
