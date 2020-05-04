#ifndef __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_GAMEUI_GAMEUI_H__
#define __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_GAMEUI_GAMEUI_H__

#include <vector>
#include <Feature/Common/GameStruct.h>

class CGameUi
{
public:
	CGameUi(_In_ DWORD dwBase);
	~CGameUi() = default;

	DWORD GetBase() CONST;

	CONST std::wstring GetName();

	static CGameUi GetRoot();

	UINT GetChildVecGameUi(_Out_ std::vector<CGameUi>& Vec, _In_ BOOL IsShowHide) CONST;

	VOID Click() CONST;

	// 二级窗口遍历的时候, IsShowHide 必须 = TRUE
	BOOL FindChildGameUi(_In_ CONST std::wstring& wsGameUiName, _Out_ CGameUi* pGameUi, _In_ BOOL IsShowHide = FALSE) CONST;

	BOOL GetMenuId() CONST;
private:
	DWORD _dwBase;
	std::wstring _wsName;
};



#endif // !__ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_GAMEUI_GAMEUI_H__
