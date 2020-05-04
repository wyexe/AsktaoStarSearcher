#include "GameUi.h"
#include <CharacterLib/Character.h>
#include <LogLib/Log.h>
#include <Feature/Core/Object/VecGameObject.h>
#include <Feature/Core/CALL/GameCALL.h>
#include <Feature/Command/Command.h>

#define _SELF L"GameUi.cpp"
CGameUi::CGameUi(_In_ DWORD dwBase) : _dwBase(dwBase)
{
	GetName();
}

DWORD CGameUi::GetBase() CONST
{
	return _dwBase;
}

CONST std::wstring CGameUi::GetName()
{
	libTools::CException::InvokeAction(__FUNCTIONW__, [&]
	{
		if (_wsName.empty() && GetBase() != 0)
		{
			CONST CHAR* pszText = reinterpret_cast<CONST CHAR*>(GetBase() + UI_NameÆ«ÒÆ);
			if (pszText != nullptr && strlen(pszText) < 32)
			{
				_wsName = libTools::CCharacter::ASCIIToUnicode(std::string(pszText));
			}
		}
	});
	
	return _wsName;
}

CGameUi CGameUi::GetRoot()
{
	return CGameUi(RD(UI±éÀú»ùÖ·));
}

UINT CGameUi::GetChildVecGameUi(_Out_ std::vector<CGameUi>& Vec, _In_ BOOL IsShowHide) CONST
{
	CCommand::GetInstance().ExcutePtrToGame([&] 
	{
		CVecGameObject::GetVecGameUi(GetBase(), Vec, IsShowHide);
	});


	return Vec.size();
}

VOID CGameUi::Click() CONST
{
	CCommand::GetInstance().ExcutePtrToGame([this] { CGameCALL::UiClick(GetBase()); });
}

BOOL CGameUi::FindChildGameUi(_In_ CONST std::wstring& wsGameUiName, _Out_ CGameUi* pGameUi, _In_ BOOL IsShowHide) CONST
{
	return libTools::CException::InvokeFunc<BOOL>(__FUNCTIONW__, [&]
	{
		std::vector<CGameUi> Vec;
		CCommand::GetInstance().ExcutePtrToGame([&]{CVecGameObject::GetVecGameUi(GetBase(), Vec, IsShowHide); });
		
		

		auto itr = std::find_if(Vec.begin(), Vec.end(), [wsGameUiName](CGameUi& itm) { return itm.GetName() == wsGameUiName; });
		if (itr != Vec.end() && pGameUi != nullptr)
		{
			*pGameUi = *itr;
		}
		return itr != Vec.end();

	});
}

BOOL CGameUi::GetMenuId() CONST
{
	return RD(GetBase() + UIµÄMenuIdÆ«ÒÆ);
}
