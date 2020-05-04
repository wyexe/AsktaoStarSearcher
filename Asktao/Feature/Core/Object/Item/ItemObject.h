#ifndef __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_ITEM_ITEMOBJECT_H__
#define __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_ITEM_ITEMOBJECT_H__

#include <Feature/Common/GameStruct.h>

class CItemObject
{
public:
	CItemObject(_In_ DWORD dwBase);
	~CItemObject() = default;

	//
	DWORD GetBase() CONST;

	//
	CONST std::wstring GetName() CONST;

	//
	DWORD GetPos() CONST;

	//
	VOID UseItem() CONST;

	//
	VOID PrintAttribute() CONST;
private:
	//
	DWORD GetAttributeTreeHead() CONST;
private:
	DWORD _dwBase;
};



#endif // !__ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_ITEM_ITEMOBJECT_H__
