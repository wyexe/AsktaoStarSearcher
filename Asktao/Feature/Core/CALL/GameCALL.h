#ifndef __ASKTAO_ASKTAO_FEATURE_CORE_CALL_GAMECALL_H__
#define __ASKTAO_ASKTAO_FEATURE_CORE_CALL_GAMECALL_H__

#include <Feature/Common/GameStruct.h>

class CGameCALL
{
private:
	using SendCALLPtr = BOOL(WINAPIV*)(...);
	static SendCALLPtr	_SendPtr;
public:
	CGameCALL() = default;
	~CGameCALL() = default;

	// 
	static VOID UseItem(_In_ DWORD dwItemId);

	//
	static VOID ClickNpcMenuItem(_In_ DWORD dwGameUiObject);

	//
	static VOID MoveToPoint(_In_ CONST Point& Pos);

	//
	static CHAR* GetNpcName(_In_ DWORD dwObjectPtr);

	//
	static DWORD GetNpcId(_In_ DWORD dwObjectPtr);

	//
	static VOID SwitchChannel(_In_ LPCSTR pszChannelName);

	//
	static VOID UiClick(_In_ DWORD dwGameUiObject);

	//
	static VOID UseSkill_NoFight(_In_ DWORD dwVictimId, _In_ DWORD dwSkillNo);

	//
	static VOID AutoFindPath(_In_ LPCSTR pszText);

	//
	static VOID RunAway();

	//
	static VOID CatchPet(_In_ DWORD dwTargetId);

	//
	static VOID RestoringStatus();

	//
	static VOID Alchemy_By_Pet(_In_ DWORD dwPetNo);

	//
	static VOID Alchemy_By_Nedan(_In_ DWORD dwPos1, _In_ DWORD dwPos2, _In_ DWORD dwPos3);

	//
	static VOID OpenNpc(_In_ DWORD dwNpcId);

	// ½«³èÎï´æ²Ö StorePetDlg + 0x340 or CALL VirtualFuncPtr 0x198
	static VOID StorePet(_In_ DWORD dwStorePetDlgId/*+340*/, _In_ DWORD dwLocation/*MinIndex=1*/);
private:
	static CHAR szEmptyText[32];
};



#endif // !__ASKTAO_ASKTAO_FEATURE_CORE_CALL_GAMECALL_H__
