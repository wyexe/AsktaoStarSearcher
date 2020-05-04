#include "ShiJueFighter.h"
#include <LogLib/Log.h>
#include <Feature/Core/Object/VecGameObject.h>
#include <Feature/Core/Object/Person/PersonAttribute.h>
#include <Feature/Core/CALL/GameCALL.h>
#include <Feature/Command/Command.h>

#define _SELF L"ShiJueFigther.cpp"
BOOL CShiJueFighter::Run()
{
	g_emScriptStatus = em_Script_Status_Running;
	while (true)
	{
		if (!GameRun)
		{
			::Sleep(1000);
			continue;
		}


		ClickAutoFightDlg_ContinueButton();
		if (CPersonAttribute::IsFighting())
		{
			::Sleep(1000);
			if (!CPersonAttribute::IsFighting())
			{
				LOG_CF_D(L"½áÊøÕ½¶· -- ²¹³ä×´Ì¬");
				CCommand::GetInstance().ExcutePtrToGame([] { CGameCALL::RestoringStatus(); });
			}
		}
	}
	return TRUE;
}

VOID CShiJueFighter::ClickAutoFightDlg_ContinueButton()
{
	static ULONGLONG ulTick = 0;
	if (::GetTickCount64() - ulTick >= 60 * 1000)
	{
		CGameUi AutoFightDlg(0);
		if (CGameUi::GetRoot().FindChildGameUi(L"AutoFightDlg", &AutoFightDlg))
		{
			CGameUi ContinueBtn(0);
			if (AutoFightDlg.FindChildGameUi(L"ContinueBtn", &ContinueBtn, TRUE))
			{
				LOG_CF_D(L"Exist AutoFightDlg.ContinueBtn, Click it!");
				CCommand::GetInstance().ExcutePtrToGame([ContinueBtn] { CGameCALL::UiClick(ContinueBtn.GetBase()); });
			}
		}



		ulTick = ::GetTickCount64();
	}
}
