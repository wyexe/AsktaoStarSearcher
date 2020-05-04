#include "CatchPetScript.h"
#include <LogLib/Log.h>
#include <MathLib/DistanceCalc.h>
#include <CharacterLib/Character.h>
#include <Feature/Core/Object/VecGameObject.h>
#include <Feature/Core/Object/Person/PersonAttribute.h>
#include <Feature/Res/ResText.h>
#include <Feature/Core/CALL/GameCALL.h>
#include <Feature/Command/Command.h>


#define _SELF L"CatchPetScript.cpp"
BOOL CCatchPetScript::Run()
{
	CResText::ResCatchPet ResPoint;
	if (!CResText::GetInstance().GetResCatchPet(g_pAccountShareMemory->Config.CatchConfig.wszMapName, &ResPoint))
	{
		LOG_MSG_CF(L"抓宝宝的地图[%s] 并没有写!", g_pAccountShareMemory->Config.CatchConfig.wszMapName);
		return FALSE;
	}

	g_emScriptStatus = em_Script_Status_Running;
	while (true)
	{
		if (!GameRun)
		{
			::Sleep(1000);
			continue;
		}


		for (UINT i = 0;i < ResPoint.Vec.size() && GameRun; ++i)
		{
			MoveToNextPoint(ResPoint.Vec.at(i));
			while (GameRun)
			{
				Point PersonPos = CPersonAttribute::GetCurrentPoint();
				::Sleep(2000);
				if (CPersonAttribute::IsFighting())
				{
					LOG_C_D(L"进入战斗……");
					CatchPet();
					continue;
				}
				else if (CPersonAttribute::GetCurrentPoint() == PersonPos)
				{
					CheckOtherDlg();
					if (GameRun)
					{
						CheckMonsterSkill();
						MoveToNextPoint(ResPoint.Vec.at(i));
					}
				}
			}
			
		}
	}
	return TRUE;
}

VOID CCatchPetScript::MoveToNextPoint(_In_ CONST Point& Pos)
{
	if (CPersonAttribute::GetCurrentMapName() == g_pAccountShareMemory->Config.CatchConfig.wszMapName)
	{
		CCommand::GetInstance().ExcutePtrToGame([Pos] { CGameCALL::MoveToPoint(Pos); });
	}
	else
	{
		LOG_C_D(L"跨图寻路..");
		std::string Text = libTools::CCharacter::UnicodeToASCII(libTools::CCharacter::MakeFormatText(L"%s(%d,%d)", g_pAccountShareMemory->Config.CatchConfig.wszMapName, Pos.X, Pos.Y));
		CCommand::GetInstance().ExcutePtrToGame([Text] {CGameCALL::AutoFindPath(Text.c_str()); });
	}
}

VOID CCatchPetScript::CatchPet() CONST
{
	// 战斗逻辑
	while (CPersonAttribute::IsFighting() && GameRun)
	{
		if (!CPersonAttribute::IsSelfRound())
		{
			::Sleep(2000);
			continue;
		}


		// GetVecMonster
		std::vector<CNpc> Vec;
		CVecGameObject::GetVecMonster(Vec);


		BOOL bExistSpecifyMonster = FALSE;
		for (auto& itm : Vec)
		{
			if (IsSpecifyMonsterName(itm.GetName()))
			{
				CCommand::GetInstance().ExcutePtrToGame([itm] { CGameCALL::CatchPet(itm.GetId()); });
				bExistSpecifyMonster = TRUE;
				break;
			}
		}

		
		if (!bExistSpecifyMonster)
		{
			CCommand::GetInstance().ExcutePtrToGame([] { CGameCALL::RunAway(); });
		}
	}



}

BOOL CCatchPetScript::IsSpecifyMonsterName(_In_ CONST std::wstring& wsMonsterName) CONST
{
	return std::find_if(_VecSpecifyMonsterName.begin(), _VecSpecifyMonsterName.end(), [wsMonsterName](CONST std::wstring& itm) { return itm == wsMonsterName; }) != _VecSpecifyMonsterName.end();
}

VOID CCatchPetScript::CheckMonsterSkill()
{
	while (GameRun)
	{
		std::vector<CNpc> VecPet;
		CVecGameObject::GetVecPet(VecPet);


		BOOL bExist = FALSE;
		for (auto& itm : VecPet)
		{
			std::vector<CSkillObject> VecPetSkill;
			if (CVecGameObject::GetVecPetSkill(itm.GetId(), VecPetSkill) <= 3)
			{
				DWORD dwPetNo = itm.GetNo();
				if (dwPetNo == -1)
				{
					LOG_CF_E(L"Pet[%s]. No = -1", itm.GetName().c_str());
					continue;
				}
				CCommand::GetInstance().ExcutePtrToGame([dwPetNo] { CGameCALL::Alchemy_By_Pet(dwPetNo); });
				bExist = TRUE;
				break;
			}
		}
		if (!bExist)
		{
			break;
		}
	}
}

VOID CCatchPetScript::CheckOtherDlg()
{
	CGameUi FightTalkMenuDlg(0), TalkNoMenuDlg(0);


	std::vector<CGameUi> Vec;
	if ((CGameUi::GetRoot().FindChildGameUi(L"FightTalkMenuDlg", &FightTalkMenuDlg, FALSE) && FightTalkMenuDlg.FindChildGameUi(L"Instruction", nullptr, TRUE)) ||
		(CGameUi::GetRoot().FindChildGameUi(L"TalkNoMenuDlg", &TalkNoMenuDlg, FALSE) && TalkNoMenuDlg.FindChildGameUi(L"Instruction", nullptr, TRUE)))
	{
		LOG_C_D(L"出现老君!!!");
		g_emScriptStatus = em_Script_Status_Stop;
	}
}
