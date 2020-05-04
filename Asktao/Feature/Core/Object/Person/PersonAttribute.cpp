#include "PersonAttribute.h"
#include <LogLib/Log.h>
#include <Feature/Core/Object/VecGameObject.h>
#include <Feature/Command/Command.h>
#include <CharacterLib/Character.h>

#define _SELF L"PersonAttribute.cpp"
std::wstring CPersonAttribute::GetServerName()
{
	CONST static std::vector<std::wstring> VecServer = { L"�����",L"2019",L"�ʶ�����",L"����һ��" };

	auto wsFullChannelName = GetChannelName();
	auto itr = std::find_if(VecServer.begin(), VecServer.end(), [wsFullChannelName](CONST std::wstring& itm) { return wsFullChannelName.find(itm) != -1; });
	if (itr != VecServer.end())
	{
		std::vector<std::wstring> Vec;
		libTools::CCharacter::SplitFormatText(wsFullChannelName, *itr + L"*", Vec);
		if (Vec.empty())
		{
			LOG_MSG_CF(L"��Ч�ķ�����:[%s]", wsFullChannelName.c_str());
			return L"�����";
		}

		return *itr;
	}



	LOG_MSG_CF(L"δ֪�ķ�����:[%s], ��ϵ�Ϸ�!", wsFullChannelName.c_str());
	return L"";
}

CONST std::wstring CPersonAttribute::GetChannelName()
{
	// [zcc01] [�������ʮ����] �ʵ������(1.656.180912) 
	std::vector<std::wstring> Vec;
	libTools::CCharacter::SplitFormatText(CCommand::GetInstance().GetGameWindowText(), L"[*] [*] * ", Vec);
	return Vec.size() > 1 ? Vec[1] : L"��Ч��";
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
	return !wsValue.empty() ? wsValue : L"��Ч��ͼ";
}

CONST std::wstring CPersonAttribute::GetPlayerName()
{
	std::wstring wsValue;
	return CVecGameObject::FindPersonAttributeText("name", wsValue) ? wsValue : L"��Ч��ɫ��";
}

BOOL CPersonAttribute::IsFighting()
{
	return strstr(reinterpret_cast<CONST CHAR*>(�Ƿ�ս���л�ַ), "ս��") != nullptr;
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
	return RD(RD(��·��ַ) + 0xC) != RD(RD(��·��ַ) + 0x8);
}
