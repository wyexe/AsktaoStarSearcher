#ifndef __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_NPC_NPC_H__
#define __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_NPC_NPC_H__

#include <Feature/Common/GameStruct.h>

class CNpc
{
public:
	enum class NpcType
	{
		Npc,
		WorldMonster,
		Pet,
		Other,
	};
public:
	CNpc(_In_ DWORD dwBase);
	~CNpc() = default;

	// 
	DWORD GetBase() CONST;

	// 
	CONST std::wstring GetName();

	//
	DWORD GetId() CONST;

	//
	DWORD GetNo() CONST;

	//
	Point GetPoint() CONST;

	//
	NpcType GetNpcType() CONST;

	//
	BOOL IsPlayer() CONST;

	//
	BOOL IsMonster() CONST;

	//
	VOID PrintAttribute() CONST;

	//
	DWORD GetAttributeTableHead() CONST;
private:
	DWORD _dwBase;
	std::wstring _wsName;
};



#endif // !__ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_NPC_NPC_H__
