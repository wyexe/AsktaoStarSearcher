#include "PersonAttribute.h"
#include <LogLib/Log.h>
#include <Feature/Core/Object/VecGameObject.h>
#include <Feature/Command/Command.h>
#include <CharacterLib/Character.h>

#define _SELF L"PersonAttribute.cpp"
std::wstring CPersonAttribute::GetServerName()
{
	CONST static std::vector<std::wstring> VecServer = { L"经典服",L"2019",L"问鼎天下",L"二〇一八" };

	auto wsFullChannelName = GetChannelName();
	auto itr = std::find_if(VecServer.begin(), VecServer.end(), [wsFullChannelName](CONST std::wstring& itm) { return wsFullChannelName.find(itm) != -1; });
	if (itr != VecServer.end())
	{
		std::vector<std::wstring> Vec;
		libTools::CCharacter::SplitFormatText(wsFullChannelName, *itr + L"*", Vec);
		if (Vec.empty())
		{
			LOG_MSG_CF(L"无效的服务器:[%s]", wsFullChannelName.c_str());
			return L"经典服";
		}

		return *itr;
	}



	LOG_MSG_CF(L"未知的服务器:[%s], 联系老夫!", wsFullChannelName.c_str());
	return L"";
}

CONST std::wstring CPersonAttribute::GetChannelName()
{
	// [zcc01] [经典服二十八线] 问道经典服(1.656.180912) 
	std::vector<std::wstring> Vec;
	libTools::CCharacter::SplitFormatText(CCommand::GetInstance().GetGameWindowText(), L"[*] [*] * ", Vec);
	return Vec.size() > 1 ? Vec[1] : L"无效线";
}

Point CPersonAttribute::GetCurrentPoint()
{
	DWORD X = 0, Y = 0;
	CCommand::GetInstance().ExcutePtrToGame([&X, &Y] 
	{
		CVecGameObject::FindPersonAttributeValue("x", X);
		CVecGameObject::FindPersonAttributeValue("y", Y);
	});


	return Point(static_cast<int>(X), static_cast<int>(Y));
}

CONST std::wstring CPersonAttribute::GetCurrentMapName()
{
	std::wstring wsValue;
	CCommand::GetInstance().ExcutePtrToGame([&wsValue]
	{
		CVecGameObject::FindPersonAttributeText("map_name", wsValue);
	});
	return !wsValue.empty() ? wsValue : L"无效地图";
}

CONST std::wstring CPersonAttribute::GetPlayerName()
{
	std::wstring wsValue;
	return CVecGameObject::FindPersonAttributeText("name", wsValue) ? wsValue : L"无效角色名";
}

BOOL CPersonAttribute::IsFighting()
{
	return strstr(reinterpret_cast<CONST CHAR*>(是否战斗中基址), "战斗") != nullptr;
}

DWORD CPersonAttribute::GetPersonId()
{
	DWORD dwPersonId = 0;
	return CVecGameObject::FindPersonAttributeValue("id", dwPersonId) ? dwPersonId : 0;
}

BOOL CPersonAttribute::IsSelfRound()
{
	return CGameUi::GetRoot().FindChildGameUi(L"FightMeMenuDlg", nullptr, FALSE);
}

BOOL CPersonAttribute::IsMoving()
{
	return RD(RD(走路基址) + 0xC) != RD(RD(走路基址) + 0x8);
}
