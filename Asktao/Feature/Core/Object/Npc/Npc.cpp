#include "Npc.h"
#include <CharacterLib/Character.h>
#include <Feature/Core/CALL/GameCALL.h>
#include <Feature/Core/Object/VecGameObject.h>
#include <LogLib/Log.h>

#define _SELF L"Npc.cpp"
CNpc::CNpc(_In_ DWORD dwBase) : _dwBase(dwBase)
{
	GetName();
}

DWORD CNpc::GetBase() CONST
{
	return _dwBase;
}

CONST std::wstring CNpc::GetName()
{
	libTools::CException::InvokeAction(__FUNCTIONW__, [&]
	{
		if (_wsName.empty() && GetBase() != 0)
		{
			libTools::CException::InvokeAction(__FUNCTIONW__, [&]
			{
				if (RD(GetBase() + Player名字偏移 + 0x10) != 0 && RD(GetBase() + Player名字偏移 + 0x10) < 32)
				{
					_wsName = libTools::CMemory::ReadASCIIText(GetBase() + Player名字偏移);
				}
				else
				{
					/*CONST CHAR* pszNpcName = CGameCALL::GetNpcName(GetBase());
					if (pszNpcName != nullptr && strlen(pszNpcName) < 12)
					{
						_wsName = libTools::CCharacter::ASCIIToUnicode(pszNpcName);
						//LOG_C_D(L"_wsName=[%s]", _wsName.c_str());
					}*/
				}
			});
		}
	});
	return _wsName;
	
}

DWORD CNpc::GetId() CONST
{
	return CGameCALL::GetNpcId(GetBase());
}

DWORD CNpc::GetNo() CONST
{
	std::wstring wsText;
	return CVecGameObject::FindObjectAttributeValue_By_Key(GetAttributeTableHead(), "no", wsText) ? std::stoul(wsText) : 0xFFFFFFFF;
}

Point CNpc::GetPoint() CONST
{
	return Point(libTools::CMemory::ReadFloat(GetBase() + Npc坐标X) / 24, libTools::CMemory::ReadFloat(GetBase() + Npc坐标X + 0x4) / 24);
}


CNpc::NpcType CNpc::GetNpcType() CONST
{
	CONST CHAR* szNpcType = nullptr;
	libTools::CException::InvokeAction(__FUNCTIONW__, [&]
	{
		using DefGetNpcType = CONST CHAR*(WINAPIV*)(VOID);
		DefGetNpcType GetNpcTypeFuncPtr = reinterpret_cast<DefGetNpcType>(RD(RD(GetBase() + 0) + 0));


		szNpcType = GetNpcTypeFuncPtr();
	});


	if (libTools::CCharacter::strcmp_my(szNpcType, "GNpc"))
		return NpcType::Npc;
	else if (libTools::CCharacter::strcmp_my(szNpcType, "GPet"))
		return NpcType::Pet;
	else if (libTools::CCharacter::strcmp_my(szNpcType, "GMonster"))
		return NpcType::WorldMonster;

	return NpcType::Other;
}

BOOL CNpc::IsPlayer() CONST
{
	return RD(GetBase() + 是否玩家偏移) == 1;
}

BOOL CNpc::IsMonster() CONST
{
	return RD(GetBase() + 是否怪物偏移) == 1;
}

VOID CNpc::PrintAttribute() CONST
{
	std::wstring wsValue;
	CVecGameObject::FindObjectAttributeValue_By_Key(GetAttributeTableHead(), "*", wsValue);
}

DWORD CNpc::GetAttributeTableHead() CONST
{
	return RD(RD(GetBase() + 人物属性偏移1 + 0x4 + 0x4) + 0x4);
}
