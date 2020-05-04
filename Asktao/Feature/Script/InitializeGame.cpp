#include "InitializeGame.h"
#include <LogLib/Log.h>
#include <CharacterLib/Character.h>
#include <Feature/Core/Object/Person/PersonAttribute.h>
#include <Feature/Socket/API/SocketAPI.h>
#include "SearchStar.h"
#include "ShiJueFighter.h"
#include "TestForzenScript.h"

#define _SELF L"InitialzieGame.cpp"
BOOL CInitializeGame::Initialize()
{
	std::wstring wsPlayerName;
	while (wsPlayerName.empty() || wsPlayerName == L"无效角色名")
	{
		wsPlayerName = CPersonAttribute::GetPlayerName();
		::Sleep(5 * 1000);
	}


	libTools::CLog::GetInstance().SetClientName(wsPlayerName, libTools::CCharacter::MakeCurrentPath(L"\\Log\\"));
	libTools::CLog::GetInstance().SetSyncSendLog();
	libTools::CLog::GetInstance().SetExceptionPtrToLog();

	LOG_CF_D(L"_wsPlayerName=%s", wsPlayerName.c_str());
	for (UINT i = 0; i < g_pShareMemoryContent->uAccountCount; ++i)
	{
		if (libTools::CCharacter::strcmp_my(g_pShareMemoryContent->Account[i].wszPlayerName, wsPlayerName.c_str()))
		{
			g_pAccountShareMemory = &g_pShareMemoryContent->Account[i];
			g_pAccountShareMemory->bLogin = TRUE;
			g_pAccountShareMemory->dwPid = ::GetCurrentProcessId();
			g_pAccountShareMemory->bLive = TRUE;


			LOG_CF_D(L"emScriptType=%d", g_pAccountShareMemory->emScriptType);
			switch (g_pAccountShareMemory->emScriptType)
			{
			case em_Script_Type::SearchStar:
			case em_Script_Type::SearchChatStar:
				/*if (CPersonAttribute::GetServerName() != L"2019")
				{
					LOG_MSG_CF(L"限制登陆区服!");
					return FALSE;
				}*/

				return CSearchStarService().Run();
			case em_Script_Type::AutoFigter:
				return CShiJueFighter().Run();
			case em_Script_Type::TESTFORZEN:
				return  CTestFrozenScript().Run();
			default:
				break;
			}
			return TRUE;
		}
	}
	LOG_MSG_CF(L"不存在角色[%s]", wsPlayerName.c_str());
	return FALSE;
}
