#include "VecGameObject.h"
#include <LogLib/Log.h>
#include <CharacterLib/Character.h>
#include <Feature/Command/Command.h>

#define _SELF L"VecGameObject.cpp"
BOOL CVecGameObject::FindObjectAttributeValue_By_Key(_In_ DWORD dwTreeHead, _In_ LPCSTR pszKey, _Out_ std::wstring& wsValue)
{
	return libTools::CException::InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		std::queue<DWORD> Vec;
		Vec.push(dwTreeHead);

		while (!Vec.empty())
		{
			DWORD dwAddr = Vec.front();
			Vec.pop();


			if (RB(dwAddr + 0x45) == 0)
			{
				Vec.push(RD(dwAddr + 0x0));
				Vec.push(RD(dwAddr + 0x8));


				if (RB(dwAddr + 0x10) != 0 && RB(dwAddr + 0x10 + 0x10) != 0 && RB(dwAddr + 0x10 + 0x10) < 32)
				{
					CONST CHAR* pszAttributeName = reinterpret_cast<CONST CHAR*>(RD(dwAddr + 0x10 + 0x14) > 0xF ? RD(dwAddr + 0x10) : (dwAddr + 0x10));
					if (pszKey[0] == L'*')
					{
						CONST CHAR* pszAttributeValue = reinterpret_cast<CONST CHAR*>(RD(dwAddr + 0x2C + 0x14) > 0xF ? RD(dwAddr + 0x2C) : (dwAddr + 0x2C));
						LOG_C_D(L"KeyName=[%s], KeyValue=[%s]", libTools::CCharacter::ASCIIToUnicode(pszAttributeName).c_str(), libTools::CCharacter::ASCIIToUnicode(pszAttributeValue).c_str());
					}
					else if (libTools::CCharacter::strcmp_my(pszAttributeName, pszKey))
					{
						wsValue = libTools::CMemory::ReadASCIIText(dwAddr + 0x2C);
						return TRUE;
					}
				}
			}
		}

		return FALSE;
	});
}

BOOL CVecGameObject::FindPersonAttributeValue(_In_ LPCSTR pszKey, _Out_ DWORD& dwValue)
{
	std::wstring wsValue1, wsValue2, wsValue3;
	if (FindObjectAttributeValue_By_Key(RD(RD(RD(人物属性基址) + 人物属性偏移1 + 0x4 + 0x4) + 0x4), pszKey, wsValue1))
	{
		dwValue = std::stoul(wsValue1);
	}
	else if (FindObjectAttributeValue_By_Key(RD(RD(RD(人物属性基址) + 人物属性偏移2 + 0x4 + 0x4) + 0x4), pszKey, wsValue2))
	{
		dwValue = std::stoul(wsValue2);
	}
	else if (FindObjectAttributeValue_By_Key(RD(RD(RD(人物属性基址) + 人物属性偏移3 + 0x4 + 0x4) + 0x4), pszKey, wsValue3))
	{
		dwValue = std::stoul(wsValue3);
	}

	return !wsValue1.empty() || !wsValue2.empty() || !wsValue3.empty();
}

BOOL CVecGameObject::FindPersonAttributeText(_In_ LPCSTR pszKey, _Out_ std::wstring& wsValue)
{
	if (FindObjectAttributeValue_By_Key(RD(RD(RD(人物属性基址) + 人物属性偏移1 + 0x4 + 0x4) + 0x4), pszKey, wsValue))
	{
		return TRUE;
	}


	if (FindObjectAttributeValue_By_Key(RD(RD(RD(人物属性基址) + 人物属性偏移2 + 0x4 + 0x4) + 0x4), pszKey, wsValue))
	{
		return TRUE;
	}


	return FindObjectAttributeValue_By_Key(RD(RD(RD(人物属性基址) + 人物属性偏移3 + 0x4 + 0x4) + 0x4), pszKey, wsValue);
}

BOOL CVecGameObject::FindPersonSystemAttributeText(_In_ LPCSTR pszKey, _Out_ std::wstring& wsValue)
{
	return FindObjectAttributeValue_By_Key(RD(RD(人物系统属性基址 + 0x8) + 0x4), pszKey, wsValue);
}

UINT CVecGameObject::GetVecBagItem(_Out_ std::vector<CItemObject>& VecItem)
{
	return GetVecObject(RD(RD(RD(背包遍历基址) + 0x14 + 0x4) + 0x4), VecItem, [](_In_ DWORD dwAddr) {return dwAddr; });
}

UINT CVecGameObject::GetVecTask(_Out_ std::vector<CTaskObject>& VecTask)
{
	DWORD dwAddr = RD(任务遍历基址) + 任务遍历偏移;
	DWORD dwArrayHead = RD(dwAddr);
	DWORD dwArrayTail = RD(dwAddr + 0x4);

	
	DWORD dwCount = (dwArrayTail - dwArrayHead) / 4;
	for (DWORD i = 0;i < dwCount && i < 100; ++i)
	{
		VecTask.emplace_back(RD(dwArrayHead + i * 4));
	}
	return VecTask.size();
}

UINT CVecGameObject::GetVecNpc(_Out_ std::vector<CNpc>& Vec)
{
	CCommand::GetInstance().ExcutePtrToGame([&]
	{
		libTools::CException::InvokeAction(__FUNCTIONW__, [&Vec]
		{
			DWORD dwLinkTableHead = RD(RD(Npc遍历基址) + Npc遍历偏移);
			for (DWORD dwNode = RD(dwLinkTableHead), i = 0; dwNode != NULL && dwNode != dwLinkTableHead && i < 1000; dwNode = RD(dwNode), ++i)
			{
				Vec.emplace_back(RD(dwNode + 0x8));
			}

			/*DWORD dwRoot = RD(RD(周围所有对象基址) + 周围所有对象偏移);
			std::queue<DWORD> VecQueue;
			VecQueue.push(dwRoot);


			UINT uCount = 0;
			while (!VecQueue.empty() && ++uCount < 1000)
			{
				DWORD dwAddr = VecQueue.front();
				VecQueue.pop();


				dwAddr = GetNextNpcObject(dwAddr);
				if (dwAddr == RD(dwRoot))
					break;


				VecQueue.push(dwAddr);

				DWORD dwObject = RD(dwAddr + 0x1C);
				if (RB(dwObject + 0x28) != 0x1 || RB(dwObject + 0x1C) != 0x1)
					continue;


				Vec.emplace_back(dwObject);
			}*/
		});
	});


	return Vec.size();
}

UINT CVecGameObject::GetVecGameUi(_In_ DWORD dwAddr, _Out_ std::vector<CGameUi>& Vec, _In_ BOOL IsShowHide)
{
	libTools::CException::InvokeAction(__FUNCTIONW__, [&]
	{
		DWORD dwLinkTableHead = RD(dwAddr + UI遍历偏移);
		DWORD dwObjectAddr = RD(dwLinkTableHead);

		while (dwObjectAddr != dwLinkTableHead && RD(dwObjectAddr + 0x8) != 0 && Vec.size() < 1000)
		{
			if (IsShowHide)
				Vec.emplace_back(RD(dwObjectAddr + 0x8));
			else
			{
				// 过滤隐藏
				if (/*RB(RD(dwObjectAddr + 0x8) + 0x40) != 0 && */RD(RD(dwObjectAddr + 0x8) + UI遍历偏移 + 4) != 0)
				{
					Vec.emplace_back(RD(dwObjectAddr + 0x8));
				}
			}


			dwObjectAddr = RD(dwObjectAddr);
		}
	});
	return Vec.size();
}

UINT CVecGameObject::GetVecChat(_In_ CONST StarChatMsgContent& LastStar, _Out_ std::vector<StarChatMsgContent>& VecContent)
{
	libTools::CException::InvokeFunc<UINT>(__FUNCTIONW__, [&]
	{
		std::stack<DWORD> VecChatAddr;
		GetVecChatAddr(VecChatAddr, ChannelType::谣言);
		static CHAR szText[1024] = { 0 };
		while (!VecChatAddr.empty())
		{
			DWORD dwAddr = VecChatAddr.top();
			VecChatAddr.pop();


			// Get Msg Content Text
			DWORD dwTextSize = RD(dwAddr + 0x0);
			CONST CHAR* pszText = reinterpret_cast<CHAR*>(dwAddr + 0x12);
			if (::IsBadCodePtr(reinterpret_cast<FARPROC>(pszText)) || dwTextSize >= _countof(szText) - 1)
			{
				LOG_CF_E(L"dwTextSize=%d", dwTextSize);
				continue;
			}


			ZeroMemory(szText, sizeof(szText));
			libTools::CCharacter::strcpy_my(szText, pszText, dwTextSize);
			std::wstring wsChatText = libTools::CCharacter::ASCIIToUnicode(szText);


			// Get Msg Tick Text
			DWORD dwTimeTextAddr = dwAddr + dwTextSize - 0x11;
			ZeroMemory(szText, sizeof(szText));
			libTools::CCharacter::strcpy_my(szText, reinterpret_cast<CHAR *>(dwTimeTextAddr), 0xC);



			std::wstring wsTimeTick = libTools::CCharacter::ASCIIToUnicode(szText);
			if (!LastStar.wsTick.empty() && wsTimeTick == LastStar.wsTick && wsChatText == LastStar.wsFullContent)
			{
				break;
			}


			VecContent.emplace_back(wsTimeTick, wsChatText);
		}

		return VecContent.size();
	});

	return VecContent.size();
}


UINT CVecGameObject::GetVecMonster(_Out_ std::vector<CNpc>& VecMonster)
{
	CGameUi FightMgr(0);
	if (CGameUi::GetRoot().FindChildGameUi(L"FightMgr", &FightMgr, TRUE))
	{
		DWORD dwAddr = RD(FightMgr.GetBase() + 怪物遍历偏移);
		for (DWORD i = 0; i < 20; ++i)
		{
			CNpc Npc(RD(dwAddr + i * 4));
			if (!Npc.GetName().empty() && Npc.IsMonster())
			{
				VecMonster.push_back(Npc);
			}
		}
	}
	return VecMonster.size();
}


UINT CVecGameObject::GetVecPet(_Out_ std::vector<CNpc>& Vec)
{
	return GetVecObject(RD(RD(RD(宠物遍历基址) + 0x14 + 0x4) + 0x4), Vec, [](DWORD dwAddr) { return RD(dwAddr + 0x10); });
}


UINT CVecGameObject::GetVecPetSkill(_In_ DWORD dwPetId, _Out_ std::vector<CSkillObject>& Vec)
{
	std::queue<DWORD> Que;
	Que.push(RD(RD(RD(宠物技能基址) + 宠物技能偏移) + 0x0));

	int Count = 0;
	while (!Que.empty() && ++Count < 200)
	{
		DWORD dwNode = Que.front();
		Que.pop();


		if (dwNode == 0)
			break;


		if (RD(dwNode + 0xC) == dwPetId)
		{
			Vec.emplace_back(dwNode);
		}


		DWORD v2 = dwNode;
		if (RB(v2 + 0x15) != 0)
			break;


		DWORD v4 = RD(v2 + 0x8);
		if (RB(v4 + 0x15) != 0)
		{
			DWORD v5 = 0;
			for (v5 = RD(v2 + 0x4); RB(v5 + 0x15) == 0; v5 = RD(v5 + 0x4))
			{
				if (dwNode != RD(v5 + 0x8))
					break;


				dwNode = v5;
			}
			dwNode = v5;
		}
		else
		{
			for (DWORD v5 = RD(v4); RB(v5 + 0x15) == 0; v5 = RD(v5))
			{
				v4 = v5;
			}

			dwNode = v4;
		}

		Que.push(dwNode);
	}
	return Vec.size();
}

UINT CVecGameObject::GetVecChatAddr(_Out_ std::stack<DWORD>& Vec, _In_ ChannelType ChatType)
{
	return libTools::CException::InvokeFunc<UINT>(__FUNCTIONW__, [&]
	{
		DWORD dwArrayAddr = RD(聊天窗口基址) + 聊天窗口偏移;
		DWORD dwArrayCount = (RD(dwArrayAddr + 0x4) - RD(dwArrayAddr)) / 4;
		for (DWORD i = 0; i < dwArrayCount; ++i)
		{
			DWORD dwAddr = RD(RD(RD(dwArrayAddr) + i * 4) + 0xC);
			while (RD(dwAddr) != 0)
			{
				if (static_cast<ChannelType>(RD(dwAddr + 0x4)) == ChatType)
					Vec.push(dwAddr);


				dwAddr += RD(dwAddr);
			}
		}


		return Vec.size();
	});
}

DWORD CVecGameObject::GetNextNpcObject(_In_ DWORD v2)
{
	DWORD dwValue = 0, dwResult = 0;
	if (RB(v2 + 0x21) != 0)
	{
		//LOG_C_D(L"v2[%X] + 0x21 != 0", v2);
		dwResult = RD(v2 + 0x8);
		return dwResult;
	}


	DWORD v4 = RD(v2 + 0x0);
	if (RB(v4 + 0x21) != 0)
	{
		//LOG_C_D(L"RB(v4[%X] + 0x21) != 0", v4);
		for (dwResult = RD(v2 + 0x4); RB(dwResult + 0x21) == 0; dwResult = RD(dwResult + 0x4))
		{
			//LOG_C_D(L"Loop1 => v2[%X] RD(dwResult)[%X]", v2, RD(dwResult));
			if (v2 != RD(dwResult))
				break;

			v2 = dwResult;
		}
		dwValue = dwResult;
		//LOG_C_D(L"dwValue=dwResult => [%X]", dwValue);
	}
	else
	{
		for (dwResult = RD(v4 + 0x8); RB(dwResult + 0x21) == 0; dwResult = RD(dwResult + 0x8))
		{
			//LOG_C_D(L"Loop2 => dwResult[%X]", dwResult);
			v4 = dwResult;
		}

		dwValue = v4;
	}

	return dwValue;
}

