#include "DbManager.h"
#include <CharacterLib/Character.h>

CDbManager& CDbManager::GetInstance()
{
	static CDbManager Instance;
	return Instance;
}

VOID CDbManager::Initialize()
{
	_wsDns = L"AsktaoServer";
	_wsDbUser = L"GameUser";
	_wsDbPass = L"0x5F58A41C";


	this->RunAsyncThread();
}

VOID CDbManager::AddSearchStarRecord(_In_ CONST StarInfo_Tranfer& StartInfo)
{
	this->AsyncExcuteSQL(libTools::CCharacter::MakeFormatText(L"exec AddStarRecord N'%s',N'%s',N'[%s]'", StartInfo.wsChannelText.c_str(), StartInfo.wsNpcName.c_str(), StartInfo.wsShowContent.c_str()));
}

VOID CDbManager::AddRumorRecord(_In_ CONST std::wstring& wsRumorText)
{
	this->AsyncExcuteSQL(libTools::CCharacter::MakeFormatText(L"exec AddRumorRecord N'%s'", wsRumorText.c_str()));
}
