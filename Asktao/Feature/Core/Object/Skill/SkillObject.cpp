#include "SkillObject.h"
#include <Feature/Core/Object/VecGameObject.h>

CSkillObject::CSkillObject(_In_ DWORD dwNode) : _dwNode(dwNode)
{

}

DWORD CSkillObject::GetAttributeHead() CONST
{
	return RD(RD(GetSkillObject() + 0x8) + 0x4);
}

VOID CSkillObject::PrintAttribute() CONST
{
	std::wstring wsText;
	CVecGameObject::FindObjectAttributeValue_By_Key(GetAttributeHead(), "*", wsText);
}

std::wstring CSkillObject::GetName() CONST
{
	std::wstring wsText;
	return CVecGameObject::FindObjectAttributeValue_By_Key(GetAttributeHead(), "skill_name", wsText) ? wsText : L"Invalid";
}

DWORD CSkillObject::GetSkillObject() CONST
{
	return RD(_dwNode + 0x10);
}

DWORD CSkillObject::GetBase() CONST
{
	return _dwNode;
}
