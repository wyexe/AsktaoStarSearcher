#include "ItemObject.h"
#include <Feature/Core/Object/VecGameObject.h>
#include <Feature/Core/CALL/GameCALL.h>
#include <Feature/Command/Command.h>

CItemObject::CItemObject(_In_ DWORD dwBase) : _dwBase(dwBase)
{

}

DWORD CItemObject::GetBase() CONST
{
	return _dwBase;
}

CONST std::wstring CItemObject::GetName() CONST
{
	std::wstring wsItemName;
	return CVecGameObject::FindObjectAttributeValue_By_Key(GetAttributeTreeHead(), "name", wsItemName) ? wsItemName : L"无效物品";
}

DWORD CItemObject::GetPos() CONST
{
	std::wstring wsPos;
	return CVecGameObject::FindObjectAttributeValue_By_Key(GetAttributeTreeHead(), "pos", wsPos) ? std::stoul(wsPos) : -1;
}

VOID CItemObject::UseItem() CONST
{
	DWORD dwPos = GetPos();
	if (dwPos != 0xFFFFFFFF)
	{
		CCommand::GetInstance().ExcutePtrToGame([dwPos] { CGameCALL::UseItem(dwPos); });
	}
}


VOID CItemObject::PrintAttribute() CONST
{
	std::wstring wsItemName;
	CVecGameObject::FindObjectAttributeValue_By_Key(GetAttributeTreeHead(), "*", wsItemName);
}

DWORD CItemObject::GetAttributeTreeHead() CONST
{
	return RD(RD(RD(GetBase() + 0x10) + 0x4 + 0x4 + 0x4) + 0x4);
}
