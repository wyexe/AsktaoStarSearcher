#include "TestForzenScript.h"
#include <LogLib/Log.h>
#include "Feature/Socket/API/SocketAPI.h"
#include "Feature/Core/Object/Person/PersonAttribute.h"
#include <TimeLib/TimeRand.h>
#include <MathLib/DistanceCalc.h>
#include "Feature/Core/CALL/GameCALL.h"
#include "Feature/Command/Command.h"
#include <ProcessLib/KeyboardMsg/KeyboardMsg.h>
#include "Feature/Core/Object/VecGameObject.h"
#include <CharacterLib/Character.h>
#include "Feature/Res/ResText.h"

#define _SELF L"TestForzenScript"
CTestFrozenScript& CTestFrozenScript::GetInstance()
{
	static CTestFrozenScript Instance;
	return  Instance;
}

BOOL CTestFrozenScript::Run()
{
	g_emScriptStatus = em_Script_Status_Running;
	while (true)
	{
		CONST static std::vector<Point> Vec = 
		{
			Point(18,25),Point(19,202),
			Point(90,216),Point(90,22),
			Point(161,26),Point(161,226),
		};


		for (auto& Pos : Vec)
		{
			while (true)
			{
				Point PersonPos = CPersonAttribute::GetCurrentPoint();
				if (libTools::CDistanceCalc::GetDisBy2D(Pos, PersonPos) < 12.0f)
				{
					//::Sleep(libTools::CTimeRand::GetRand(2, 4) * 1000);
					break;
				}

				//CResText::GetInstance().IsExistInvalidTalkMenuDlg();
				::Sleep(2000);

				if (CPersonAttribute::IsFighting())
				{
					LOG_C_D(L"战斗中……");
					continue;
				}
				else if (CPersonAttribute::GetCurrentPoint() == PersonPos)
				{
					LOG_C_D(L"2s内坐标没变化……重新走路!");
					MoveToPoint(Pos);
				}
			}
		}


		std::wstring wsChannelName = CPersonAttribute::GetChannelName();
		while (wsChannelName == CPersonAttribute::GetChannelName())
		{
			::Sleep(1000);
		}


		for (auto itr = Vec.rbegin(); itr != Vec.rend(); ++itr)
		{
			while (true)
			{
				Point PersonPos = CPersonAttribute::GetCurrentPoint();
				if (libTools::CDistanceCalc::GetDisBy2D(*itr, PersonPos) < 12.0f)
				{
					//::Sleep(libTools::CTimeRand::GetRand(2, 4) * 1000);
					break;
				}

				//CResText::GetInstance().IsExistInvalidTalkMenuDlg();
				::Sleep(2000);

				if (CPersonAttribute::IsFighting())
				{
					LOG_C_D(L"战斗中……");
					continue;
				}
				else if (CPersonAttribute::GetCurrentPoint() == PersonPos)
				{
					LOG_C_D(L"2s内坐标没变化……重新走路!");
					MoveToPoint(*itr);
				}
			}
		}
	}
	

	return  TRUE;
}

void CTestFrozenScript::MoveToPoint(const Point& Pos)
{
	CGameUi PanInfoDlg(0);
	if (CGameUi::GetRoot().FindChildGameUi(L"PanInfoDlg", &PanInfoDlg, FALSE))
	{
		CGameUi NPCBtn(0);
		if (PanInfoDlg.FindChildGameUi(L"NPCBtn", &NPCBtn, TRUE))
		{
			LOG_C_D(L"Click NPCBtn");
			NPCBtn.Click();
			::Sleep(1000);

			InputText(libTools::CCharacter::MakeFormatText(L"%d,%d", Pos.X, Pos.Y));
		}
	}
}

void CTestFrozenScript::InputText(const std::wstring& wsText)
{
	const HWND hWnd = CCommand::GetInstance().GetGameHwnd();
	for (CONST auto &itm : wsText)
	{
		libTools::CKeyboardMsg::SimulationKey(hWnd, static_cast<CHAR>(itm));
		::Sleep(500);
	}
	::Sleep(500);
	libTools::CKeyboardMsg::SimulationKey(hWnd, VK_RETURN);
}
