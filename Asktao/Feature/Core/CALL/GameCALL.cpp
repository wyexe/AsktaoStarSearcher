#include "GameCALL.h"
#include <ExceptionLib/Exception.h>
#include <CharacterLib/Character.h>

CGameCALL::SendCALLPtr CGameCALL::_SendPtr = reinterpret_cast<CGameCALL::SendCALLPtr>(发包CALL);
CHAR CGameCALL::szEmptyText[32] = { 0 };
VOID CGameCALL::UseItem(_In_ DWORD dwItemId)
{
	__try
	{
		CONST static CHAR szText[] = { "pos = %d" };
		_SendPtr(0x220C, szText, dwItemId);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//::MessageBoxW(NULL, L"使用物品出现异常!   联系老夫!", L"", NULL);
	}
}

VOID CGameCALL::ClickNpcMenuItem(_In_ DWORD dwGameUiObject)
{
	__try
	{
		__asm
		{
			MOV ECX, dwGameUiObject;
			MOV EBX, DWORD PTR DS : [ECX];
			MOV EAX, DWORD PTR DS : [EBX + UI点击偏移];
			PUSH 0;
			CALL EAX;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//::MessageBoxW(NULL, L"点击选项UI出现异常! 联系老夫!", L"", NULL);
	}
}

VOID CGameCALL::MoveToPoint(_In_ CONST Point& Pos)
{
	__try
	{
		DWORD X = Pos.X, Y = Pos.Y;
		__asm
		{
			MOV ECX, 走路基址;
			MOV ECX, DWORD PTR DS : [ECX];

			PUSH 0;
			PUSH 1;
			PUSH 0;
			LEA EAX, szEmptyText;
			PUSH EAX;
			PUSH 0;
			PUSH 0;
			PUSH Y;
			PUSH X;
			MOV EAX, 走路CALL;
			CALL EAX;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//::MessageBoxW(NULL, L"走路出现异常! 联系老夫!", L"", NULL);
	}
}

CHAR* CGameCALL::GetNpcName(_In_ DWORD dwObjectPtr)
{
	__try
	{
/*
		static DWORD dwCALL = 0;
		if (dwCALL == 0)
		{
			dwCALL = RD(RD(dwObjectPtr + 0x0) + Npc名字CALL);
		}
		CHAR* pszNpcName = nullptr;
		__asm
		{
			MOV ECX, dwObjectPtr;
			MOV EDX, dwCALL;
			CALL EDX;
			MOV pszNpcName, EAX;
		}
		return pszNpcName;*/
		CHAR* pszNpcName = nullptr;
		__asm
		{
			MOV ECX, dwObjectPtr;
			MOV EAX, DWORD PTR DS : [ECX];
			MOV EDX, DWORD PTR DS : [EAX + Npc名字CALL];
			CALL EDX;
			MOV pszNpcName, EAX;
		}
		return pszNpcName;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//::MessageBoxW(NULL, L"获取Npc名字出现异常! 联系老夫!", L"", NULL);
	}
	return nullptr;
}

DWORD CGameCALL::GetNpcId(_In_ DWORD dwObjectPtr)
{
	__try
	{
		DWORD dwValue = 0;
		__asm
		{
			MOV ECX, dwObjectPtr;
			MOV EAX, DWORD PTR DS : [ECX];
			MOV EDX, DWORD PTR DS : [EAX + NpcIDCALL];
			CALL EDX;
			MOV dwValue, EAX;
		}
		return dwValue;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//::MessageBoxW(NULL, L"获取NpcID出现异常! 联系老夫!", L"", NULL);
	}
	return 0;
}



VOID CGameCALL::SwitchChannel(_In_ LPCSTR pszChannelName)
{
	__try
	{
		static CHAR szText[32] = { 0 };
		strcpy_s(szText, 32 - 1, pszChannelName);
		_asm
		{
			MOV ECX, 换线基址;
			MOV ECX, DWORD PTR DS:[ECX];

			LEA EAX, DWORD PTR DS : [ECX + 换线偏移];
			MOV DWORD PTR DS : [EAX], 0;

			LEA EAX, szText;
			PUSH 0;
			PUSH EAX;
			MOV EAX, 换线CALL;
			CALL EAX;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//::MessageBoxW(NULL, L"切换线 出现异常! 联系老夫!", L"", NULL);
	}
	
}

VOID CGameCALL::UiClick(_In_ DWORD dwGameUiObject)
{
	__try
	{
		CONST static CHAR szText[32] = { "CLICKED_BTN" };
		_asm
		{
			PUSH 0;
			LEA EAX, szText;
			PUSH EAX;
			MOV ECX, dwGameUiObject;
			MOV EAX, UI点击CALL;
			CALL EAX;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//::MessageBoxW(NULL, L"UI点击 出现异常! 联系老夫!", L"", NULL);
	}
}

VOID CGameCALL::UseSkill_NoFight(_In_ DWORD dwVictimId, _In_ DWORD dwSkillNo)
{
	__try
	{
		CONST static CHAR szText[] = { "victim_id = %d, skill_no = %d" };
		_SendPtr(0x1156, szText, dwVictimId, dwSkillNo);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//::MessageBoxW(NULL, L"使用技能 出现异常! 联系老夫!", L"", NULL);
	}
}

VOID CGameCALL::AutoFindPath(_In_ LPCSTR pszText)
{
	libTools::CException::InvokeAction(__FUNCTIONW__, [pszText]
	{
		/*GameText Text(pszText);
		DWORD dwArray[64] = { 0 };
		memcpy(reinterpret_cast<BYTE*>(dwArray) + 0x4, &Text, sizeof(Text));
		__asm
		{
			PUSH 0;
			LEA EBX, dwArray;
			PUSH EBX;

			MOV ECX, 寻路基址;
			MOV ECX, DWORD PTR DS : [ECX];
			MOV EAX, 寻路CALL1;
			CALL EAX;

			MOV ECX, 寻路基址;
			MOV ECX, DWORD PTR DS : [ECX];
			MOV EAX, 寻路CALL2;
			CALL EAX;
		}*/
	});
}

VOID CGameCALL::RunAway()
{
	__try
	{
		_SendPtr(0x1108, szEmptyText);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//::MessageBoxW(NULL, L"使用物品出现异常!   联系老夫!", L"", NULL);
	}
}

VOID CGameCALL::CatchPet(_In_ DWORD dwTargetId)
{
	__try
	{
		CONST static CHAR szText[] = { "id = %d" };
		_SendPtr(0x226A, szText, dwTargetId);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//::MessageBoxW(NULL, L"使用物品出现异常!   联系老夫!", L"", NULL);
	}
}

VOID CGameCALL::RestoringStatus()
{
	__try
	{
		CONST static CHAR szHpText[] = { "life" };
		CONST static CHAR szMpText[] = { "mana" };
		

		__asm
		{
			// Person HP
			PUSH 0;
			LEA EAX, szHpText;
			PUSH EAX;
			PUSH 1;
			MOV EAX, 人物补充状态CALL;
			CALL EAX;

			// Person MP
			PUSH 0;
			LEA EAX, szMpText;
			PUSH EAX;
			PUSH 1;
			MOV EAX, 人物补充状态CALL;
			CALL EAX;

			// Pet HP
			PUSH 0;
			LEA EAX, szHpText;
			PUSH EAX;
			PUSH 1;
			MOV EAX, 宠物补充状态CALL;
			CALL EAX;

			// Pet MP
			PUSH 0;
			LEA EAX, szMpText;
			PUSH EAX;
			PUSH 1;
			MOV EAX, 宠物补充状态CALL;
			CALL EAX;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//::MessageBoxW(NULL, L"使用物品出现异常!   联系老夫!", L"", NULL);
	}
}

VOID CGameCALL::Alchemy_By_Pet(_In_ DWORD dwPetNo)
{
	__try
	{
		CONST static CHAR szText[] = { "pos1 = %d, pos2 = %d, num2=%d, pos3 = %d, num3=%d, pos4 = %d, num4=%d, pos5 = %d, num5=%d, pos6 = %d" };
		_SendPtr(0x5506, szText, dwPetNo, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//::MessageBoxW(NULL, L"使用物品出现异常!   联系老夫!", L"", NULL);
	}
}

VOID CGameCALL::Alchemy_By_Nedan(_In_ DWORD dwPos1, _In_ DWORD dwPos2, _In_ DWORD dwPos3)
{
	__try
	{
		CONST static CHAR szText[] = { "pos1 = %d, pos2 = %d, num2=%d, pos3 = %d, num3=%d, pos4 = %d, num4=%d, pos5 = %d, num5=%d, pos6 = %d" };
		_SendPtr(0x5506, szText, 0, dwPos1, 1, dwPos2, 1, dwPos3, 1, 0, 0, 0, 0, 0, 0, 9);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//::MessageBoxW(NULL, L"使用物品出现异常!   联系老夫!", L"", NULL);
	}
}

VOID CGameCALL::OpenNpc(_In_ DWORD dwNpcId)
{
	__try
	{
		CONST static CHAR szText[] = { "id = %d" };
		_SendPtr(0x2064, szText, dwNpcId);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		
	}
}

VOID CGameCALL::StorePet(_In_ DWORD dwStorePetDlgId/*+340*/, _In_ DWORD dwLocation/*MinIndex=1*/)
{
	__try
	{
		CONST static CHAR szText[] = { "dir = %d, pos = %d, id = %d" };
		_SendPtr(0x5000, szText, 0x1, dwLocation, dwStorePetDlgId);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{

	}
}
