#include "Command.h"
#include <LogLib/Log.h>
#include <LogLib/CmdLog.h>
#include <CharacterLib/Character.h>
#include <HookLib/InlineHook/InlineHook.h>
#include <Feature/Core/Object/VecGameObject.h>
#include <Feature/Core/Object/Person/PersonAttribute.h>
#include <Feature/Script/SearchStar.h>
#include <Feature/Core/CALL/GameCALL.h>
#include <Feature/ScanBase/ScanBase.h>
#include <Feature/Script/InitializeGame.h>
#include <Feature/AntiDebug/AntiDebug.h>
#include <FileLib/File.h>
#include <ProcessLib/KeyboardMsg/KeyboardMsg.h>
#include <MathLib/DistanceCalc.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"LogLib.lib")
#pragma comment(lib,"CharacterLib.lib")
#pragma comment(lib,"HookLib.lib")

#define _SELF L"Command.cpp"
CCommandExpr::~CCommandExpr()
{
}



std::vector<libTools::ExpressionFunPtr>& CCommandExpr::GetVec()
{
	static std::vector<libTools::ExpressionFunPtr> Vec = 
	{
		{ std::bind(&CCommandExpr::Help, this, std::placeholders::_1), L"Help" },
		{ std::bind(&CCommandExpr::Run, this, std::placeholders::_1), L"Run" },
		{ std::bind(&CCommandExpr::Stop, this, std::placeholders::_1), L"Stop" },
		{ std::bind(&CCommandExpr::PrintNpc, this, std::placeholders::_1), L"PrintNpc" },
		{ std::bind(&CCommandExpr::PrintTask, this, std::placeholders::_1), L"PrintTask" },
		{ std::bind(&CCommandExpr::PrintItem, this, std::placeholders::_1), L"PrintItem" },
		{ std::bind(&CCommandExpr::PrintPerson, this, std::placeholders::_1), L"PrintPerson" },
		{ std::bind(&CCommandExpr::PrintGameUi, this, std::placeholders::_1), L"PrintGameUi" },
		{ std::bind(&CCommandExpr::PrintMonster, this, std::placeholders::_1), L"PrintMonster" },
		{ std::bind(&CCommandExpr::PrintPet, this, std::placeholders::_1), L"PrintPet" },
		{ std::bind(&CCommandExpr::PrintPetSkill, this, std::placeholders::_1), L"PrintPetSkill" },
		{ std::bind(&CCommandExpr::ScanBase, this, std::placeholders::_1), L"ScanBase" },
		{ std::bind(&CCommandExpr::Test, this, std::placeholders::_1), L"Test" },
		{ std::bind(&CCommandExpr::PrintChat, this, std::placeholders::_1), L"PrintChat" },
		
	};

	return Vec;
}

VOID CCommandExpr::Release()
{

}

VOID CCommandExpr::Help(CONST std::vector<std::wstring>&)
{
	for (auto& itm : GetVec())
		LOG_C_D(L"MethodName = [%s]", itm.wsFunName.c_str());
}

VOID CCommandExpr::Run(CONST std::vector<std::wstring>& Vec)
{
	if (Vec.empty())
	{
		LOG_C_D(L"Param = ");
		return;
	}

	if (g_emScriptStatus == em_Script_Status_Running)
	{
		LOG_C_D(L"Running...");
		return;
	}

	// Run Python Script
	_AsyncScriptPtr = std::async(std::launch::async, [] 
	{
		g_emScriptStatus = em_Script_Status_Running;
		libTools::CException::InvokeAction(__FUNCTIONW__, [] 
		{
			CSearchStarService Service;
			Service.Run();
		});
		LOG_C_D(L"Script Excute Done!");
		g_emScriptStatus = em_Script_Status_Stop;
	});
}

VOID CCommandExpr::Stop(CONST std::vector<std::wstring>&)
{
	g_emScriptStatus = em_Script_Status_Stop;
	if (_AsyncScriptPtr.valid())
	{
		LOG_C_D(L"Waiting ...");
		_AsyncScriptPtr.wait();
	}
	LOG_C_D(L"Stoped");
}

VOID CCommandExpr::PrintNpc(CONST std::vector<std::wstring>&)
{
	std::vector<CNpc> Vec;
	CVecGameObject::GetVecNpc(Vec);
	LOG_C_D(L"Vec.size=%d", Vec.size());
	for (auto& itm : Vec)
	{
		LOG_C_D(L"Npc.Base=[%X]", itm.GetBase());
		LOG_C_D(L"Npc.Name=[%s]", itm.GetName().c_str());
		LOG_C_D(L"Npc.ID=[%X]", itm.GetId());
		LOG_C_D(L"Npc.Pos=[%d,%d]", itm.GetPoint().X, itm.GetPoint().Y);
		LOG_C_D(L"Npc.dis=[%.2f]", libTools::CDistanceCalc::GetDisBy2D(itm.GetPoint(), CPersonAttribute::GetCurrentPoint()));
		LOG_C_D(L"IsPet=[%d]", itm.GetNpcType());

		itm.PrintAttribute();
	}
}

VOID CCommandExpr::PrintTask(CONST std::vector<std::wstring>&)
{
	std::vector<CTaskObject> Vec;
	CVecGameObject::GetVecTask(Vec);
	LOG_C_D(L"Vec.size1=%d", Vec.size());
	for (auto& itm : Vec)
	{
		LOG_C_D(L"Task.TaskType=[%s]", itm.GetTaskType().c_str());
		LOG_C_D(L"Task.GetRawType=[%s]", itm.GetRawType().c_str());
		LOG_C_D(L"Task.GetTaskPrompt=[%s]", itm.GetTaskPrompt().c_str());
		LOG_C_D(L"Task.GetTaskTip=[%s]", itm.GetTaskTip().c_str());
	}
}

VOID CCommandExpr::PrintItem(CONST std::vector<std::wstring>&)
{
	std::vector<CItemObject> VecItem;
	CVecGameObject::GetVecBagItem(VecItem);
	for (auto& itm : VecItem)
	{
		LOG_C_D(L"-----------------------------");
		LOG_C_D(L"Item.Base=[%X], Name=[%s], Pos=[%X]", itm.GetBase(),itm.GetName().c_str(), itm.GetPos());
		itm.PrintAttribute();
	}
}

VOID CCommandExpr::PrintPerson(CONST std::vector<std::wstring>&)
{
	std::wstring wsEmpty;
	CVecGameObject::FindPersonAttributeText("*", wsEmpty);
	CVecGameObject::FindPersonSystemAttributeText("*", wsEmpty);
}


VOID CCommandExpr::PrintGameUi(CONST std::vector<std::wstring>& Param)
{
	auto RootGameUi = CGameUi::GetRoot();
	for (auto& itm : Param)
		LOG_C_D(L"itm=[%s]",itm.c_str());

	BOOL bShow = Param.empty() ? FALSE : _wtoi(Param.at(0).c_str());

	std::vector<CGameUi> VecFirst;
	RootGameUi.GetChildVecGameUi(VecFirst, bShow);
	LOG_C_D(L"VecFirst.size=%d, bShow=%d",VecFirst.size(), bShow);
	for (auto& itm : VecFirst)
	{
		auto wsGameUiName = itm.GetName();
		LOG_C_D(L"-----------[%s].Base=[%X]--MenuId[%X]----------------------", wsGameUiName.c_str(),itm.GetBase(), itm.GetMenuId());
		std::vector<CGameUi> VecSecond;
		itm.GetChildVecGameUi(VecSecond, TRUE);
		for (auto& Sec : VecSecond)
		{
			LOG_C_D(L"[%s.%s].NodeBase=%X", wsGameUiName.c_str(), Sec.GetName().c_str(), Sec.GetBase());
		}
	}
}

VOID CCommandExpr::PrintMonster(CONST std::vector<std::wstring>&)
{
	std::vector<CNpc> Vec;
	CVecGameObject::GetVecMonster(Vec);
	for (auto& itm : Vec)
	{
		LOG_C_D(L"Monster.Base=[%X], Name=[%s]", itm.GetBase(), itm.GetName().c_str());
	}
}

VOID CCommandExpr::PrintPet(CONST std::vector<std::wstring>&)
{
	std::vector<CNpc> Vec;
	CVecGameObject::GetVecPet(Vec);
	for (auto& itm : Vec)
	{
		LOG_C_D(L"----------------Pet.Base=[%X], Name=[%s], ID=[%X]----------------------", itm.GetBase(), itm.GetName().c_str(), itm.GetId());
		//itm.PrintAttribute();


		std::vector<CSkillObject> VecSkill;
		CVecGameObject::GetVecPetSkill(itm.GetId(), VecSkill);
		for (auto& Sk : VecSkill)
		{
			LOG_C_D(L"Skill.Name=[%s]", Sk.GetName().c_str());
		}
	}
}

VOID CCommandExpr::PrintPetSkill(CONST std::vector<std::wstring>&)
{
	DWORD dwAddr = RD(RD(RD(0x0E332AC) + 0x24) + 0x0);
	std::queue<DWORD> Vec;
	Vec.push(dwAddr);

	int Count = 0;
	while (!Vec.empty() && ++Count < 200)
	{
		DWORD dwNode = Vec.front();
		Vec.pop();
		if (dwNode == 0)
		{
			LOG_C_E(L"dwNode == 0");
			break;
		}

		
		DWORD dwAttributTableAddr = RD(RD(RD(dwNode + 0x10) + 0x8) + 0x4);
		if (dwAttributTableAddr != 0)
		{
			LOG_C_D(L"-------------------------------------");
			LOG_C_D(L"dwNode=%X, + 0x10=%X + 0xC=[%X]", dwNode, RD(dwNode + 0x10), RD(dwNode + 0xC));


			std::wstring wsText;
			CVecGameObject::FindObjectAttributeValue_By_Key(dwAttributTableAddr, "*", wsText);
		}


		DWORD v2 = dwNode;
		if (RB(v2 + 0x15) != 0)
		{
			LOG_C_E(L"RD(v2 + 0x15) != 0");
			break;
		}


		DWORD v4 = RD(v2 + 0x8);
		LOG_C_D(L"v4=%X", v4);
		if (RB(v4 + 0x15) != 0)
		{
			LOG_C_D(L"RD(v4 + 0x15) != 0");
			DWORD Result = 0;
			for (Result = RD(v2 + 0x4); RB(Result + 0x15) == 0; Result = RD(Result + 0x4))
			{
				if (dwNode != RD(Result + 0x8))
				{
					LOG_C_D(L"dwNode[%X] != RD(Result + 0x8)[%X]", dwNode, RD(Result + 0x8));
					break;
				}
				dwNode = Result;
			}
			dwNode = Result;
		}
		else
		{
			LOG_C_D(L"RD(v4 + 0x15) == 0");
			for (DWORD Result = RD(v4); RB(Result + 0x15) == 0; Result = RD(Result))
			{
				v4 = Result;
			}
			dwNode = v4;
		}

		LOG_C_D(L"Push.dwNode=%X", dwNode);
		Vec.push(dwNode);
	}
}

VOID CCommandExpr::ScanBase(CONST std::vector<std::wstring>&)
{
	CScanBase().Run();
}

void CCommandExpr::PrintChat(const std::vector<std::wstring>&)
{
	std::stack<DWORD> VecChatAddr;
	CVecGameObject::GetVecChatAddr(VecChatAddr, ChannelType::谣言);


	static CHAR szText[1024] = { 0 };
	while (!VecChatAddr.empty())
	{
		DWORD dwAddr = VecChatAddr.top();
		VecChatAddr.pop();


		// Get Msg Content Text
		DWORD dwTextSize = RD(dwAddr + 0x0);
		CONST CHAR* pszText = reinterpret_cast<CHAR*>(dwAddr + 0x12);


		ZeroMemory(szText, sizeof(szText));
		libTools::CCharacter::strcpy_my(szText, pszText, dwTextSize);
		std::wstring wsChatText = libTools::CCharacter::ASCIIToUnicode(szText);


		// Get Msg Tick Text
		DWORD dwTimeTextAddr = dwAddr + dwTextSize - 0x11;
		ZeroMemory(szText, sizeof(szText));
		libTools::CCharacter::strcpy_my(szText, reinterpret_cast<CHAR *>(dwTimeTextAddr), 0xC);



		std::wstring wsTimeTick = libTools::CCharacter::ASCIIToUnicode(szText);

		LOG_C_D(L"dwAddr=[%X], wsChatText=[%s], wsTimeTick=[%s]", dwAddr, wsChatText.c_str(), wsTimeTick.c_str());
	}
}

BOOL bRunTest = TRUE;
DWORD WINAPI _TestWorker(LPVOID)
{
	Point Pos;
	while (bRunTest)
	{
		::Sleep(1000);
		std::vector<CNpc> Vec;
		CVecGameObject::GetVecNpc(Vec);
		auto itr = std::find_if(Vec.begin(), Vec.end(), [](CNpc& Npc) { return Npc.GetName() == L"丨猫癖"; });
		if (itr == Vec.end())
		{
			LOG_C_E(L"猫癖 消失了!");
			continue;
		}


		if (itr->GetPoint() == Pos)
			continue;


		LOG_C_E(L"猫癖 切换坐标 [%d,%d] => [%d,%d]", Pos.X, Pos.Y, itr->GetPoint().X, itr->GetPoint().Y);
		Pos = itr->GetPoint();
	}
	return 0;
}

VOID CCommandExpr::Test(CONST std::vector<std::wstring>&)
{
	static HANDLE hThread = NULL;
	if (hThread == NULL)
	{
		bRunTest = TRUE;
		hThread = ::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_TestWorker, NULL, NULL, NULL);
	}
	else
	{
		bRunTest = FALSE;
		hThread = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////



CCommand::PeekMessageADef CCommand::_OldPeekMessageA		= nullptr;
std::atomic<bool>			CCommand::_IsExistExcuteMethod	= false;
std::function<VOID(VOID)>	CCommand::_ExcuteMethodPtr		= nullptr;
HANDLE						CCommand::_hEvent				= NULL;


CCommand::~CCommand()
{

}

CCommand& CCommand::GetInstance()
{
	static CCommand Instance;
	return Instance;
}

BOOL CALLBACK EnumSetWinName(HWND hLauncher, LPARAM lpParam)
{
	if (IsWindow(hLauncher) && IsWindowVisible(hLauncher) && (GetWindowLongW(hLauncher, GWL_EXSTYLE)&WS_EX_TOOLWINDOW) != WS_EX_TOOLWINDOW && (GetWindowLong(hLauncher, GWL_HWNDPARENT) == 0))
	{
		WCHAR wszWinText[64] = { 0 };
		WCHAR wszWinClass[64] = { 0 };

		if (GetClassNameW(hLauncher, wszWinClass, _countof(wszWinClass) / sizeof(WCHAR)) > 0 && GetWindowTextW(hLauncher, wszWinText, _countof(wszWinText)) > 0)
		{
			if (libTools::CCharacter::strcmp_my(wszWinClass, L"AskTao"))
			{
				DWORD PID;
				::GetWindowThreadProcessId(hLauncher, &PID);
				if (PID == ::GetCurrentProcessId())
				{
					*reinterpret_cast<HWND*>(lpParam) = hLauncher;
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

VOID ExcuteCustomeMethod()
{
	if (CCommand::_IsExistExcuteMethod)
	{
		CCommand::_IsExistExcuteMethod = false;
		//DWORD dwOldValue = RD(发包全局互斥基址);
		//*(DWORD*)发包全局互斥基址 = 0x0;
		libTools::CException::InvokeAction(__FUNCTIONW__, [] 
		{
			CCommand::_ExcuteMethodPtr();
		});
		//*(DWORD*)发包全局互斥基址 = dwOldValue;
		::SetEvent(CCommand::_hEvent);
	}
}

DWORD g_dwOldVirtualHookPtr = 0x0;
#pragma   warning(push) 
#pragma   warning(disable:4740)
__declspec(naked) void WINAPI HookGamePtr()
{
	__asm PUSHAD;
	ExcuteCustomeMethod();

	__asm
	{
		POPAD;
		MOV EAX, g_dwOldVirtualHookPtr;
		JMP EAX;
	}
}
#pragma   warning(pop)



BOOL CCommand::Initialize(_In_ CONST std::wstring& wsPlayerName)
{
	_IsRun = TRUE;
	_hEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	_dwPeekMessageAProc = reinterpret_cast<DWORD>(::GetProcAddress(::GetModuleHandleW(L"user32.dll"), "PeekMessageA"));
	//CAntiDebug::GetInstance().Run(); 
	/*if (!libTools::CInlineHook::Hook(reinterpret_cast<LPVOID>(_dwPeekMessageAProc), reinterpret_cast<LPVOID>(NewPeekMessageA), reinterpret_cast<LPVOID *>(&_OldPeekMessageA)))
	{
		LOG_MSG_CF(L"Hook PeekMessageA Faild!");
		return FALSE;
	}*/

	while (EnumWindows((WNDENUMPROC)EnumSetWinName, reinterpret_cast<LPARAM>(&_hGameWnd)))
		::Sleep(1000);


	/*g_dwOldVirtualHookPtr = RD(RD(RD(人物属性基址) + 0x0) + 0x30);
	libTools::CMemory::WriteDWORD(RD(RD(人物属性基址) + 0x0) + 0x30, reinterpret_cast<DWORD>(HookGamePtr));
	if (g_dwOldVirtualHookPtr == RD(RD(RD(人物属性基址) + 0x0) + 0x30))
	{
		LOG_MSG_CF(L"Hook Faild!");
	}*/
	/*while (g_dwOldVirtualHookPtr == RD(RD(RD(人物属性基址) + 0x0) + 0x30))
	{
		DWORD dwOldProtect = 0, dwHookAddr = RD(RD(人物属性基址) + 0x0) + 0x30;

		MEMORY_BASIC_INFORMATION memInfo = { 0 };
		::VirtualQuery(reinterpret_cast<LPCVOID>(dwHookAddr), &memInfo, sizeof(memInfo));
		LOG_MSG_CF(L"1  dwHookAddr=[%X], Mem.AllocationProtect=[%X], mem.Protect=%X", dwHookAddr, memInfo.AllocationProtect, memInfo.Protect);

		NewVirtualProtect(reinterpret_cast<LPVOID>(dwHookAddr), 4, PAGE_READWRITE, &dwOldProtect);

		DWORD dwNewHookValue = reinterpret_cast<DWORD>(HookGamePtr);
		::WriteProcessMemory(::GetCurrentProcess(), reinterpret_cast<LPVOID>(dwHookAddr), &dwNewHookValue, 4, nullptr);
		::VirtualQuery(reinterpret_cast<LPCVOID>(dwHookAddr), &memInfo, sizeof(memInfo));
		LOG_MSG_CF(L"2  dwHookAddr=[%X], Mem.AllocationProtect=[%X], mem.Protect=%X", dwHookAddr, memInfo.AllocationProtect, memInfo.Protect);

		::VirtualProtect(reinterpret_cast<LPVOID>(dwHookAddr), 4, PAGE_READWRITE, &dwOldProtect);
		LOG_MSG_CF(L"3  dwHookAddr=[%X], Mem.AllocationProtect=[%X], mem.Protect=%X", dwHookAddr, memInfo.AllocationProtect, memInfo.Protect);
		if (g_dwOldVirtualHookPtr == RD(RD(RD(人物属性基址) + 0x0) + 0x30))
		{
			LOG_MSG_CF(L"Hook Faild!");
		}
	}*/

	

	if (!InitializeShareMemory())
	{
		g_IsCommandMode = TRUE;
		libTools::CLog::GetInstance().SetClientName(wsPlayerName, libTools::CCharacter::MakeCurrentPath(L"\\Log\\"));
		libTools::CLog::GetInstance().SetSyncSendLog();
		libTools::CLog::GetInstance().SetExceptionPtrToLog();

		LOG_MSG_CF(L"Command Mode~");
		static CCommandExpr Instance;
		if (!libTools::CCmdLog::GetInstance().Run(wsPlayerName, Instance.GetVec()))
		{
			LOG_MSG_CF(L"CCmdLog::GetInstance().Run = FALSE!");
			return FALSE;
		}
		return TRUE;
	}



	libTools::CException::InvokeAction(__FUNCTIONW__, []
	{
		CInitializeGame().Initialize();
	});
	return TRUE;
}


VOID CCommand::Release()
{
	_IsRun = FALSE;
	g_emScriptStatus = em_Script_Status_Stop;

	if (_OldPeekMessageA != nullptr)
	{
		CAntiDebug::GetInstance().Release();
		libTools::CInlineHook::UnHook(reinterpret_cast<LPVOID>(_dwPeekMessageAProc), _OldPeekMessageA);
		::CloseHandle(_hEvent);
		_OldPeekMessageA = nullptr;
	}
	if (g_pShareMemoryContent != nullptr)
	{
		::UnmapViewOfFile(g_pShareMemoryContent);
		g_pShareMemoryContent = nullptr;
	}
	if (_hFileMap != NULL)
	{
		::CloseHandle(_hFileMap);
		_hFileMap = NULL;
	}
}

std::wstring CCommand::GetGameWindowText() CONST
{
	WCHAR wszText[MAX_PATH] = { 0 };
	::GetWindowTextW(_hGameWnd, wszText, MAX_PATH);


	return std::wstring(wszText);
}

HWND CCommand::GetGameHwnd() const
{
	return  _hGameWnd;
}

BOOL CCommand::InitializeShareMemory()
{
	_hFileMap = ::OpenFileMappingW(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, ASKTAO_SHAREMEMORY_NAME);
	if (_hFileMap == NULL)
		return FALSE;


	g_pShareMemoryContent = reinterpret_cast<ShareMemoryContent *>(::MapViewOfFile(_hFileMap, FILE_MAP_WRITE | FILE_MAP_READ, NULL, NULL, sizeof(ShareMemoryContent)));
	if (g_pShareMemoryContent == nullptr)
		return FALSE;


	return TRUE;
}


BOOL WINAPI CCommand::NewPeekMessageA(_Out_ LPMSG lpMsg, _In_opt_ HWND hWnd, _In_ UINT wMsgFilterMin, _In_ UINT wMsgFilterMax, _In_ UINT wRemoveMsg)
{
	if (lpMsg->message == WM_KEYDOWN)
	{
		if (lpMsg->wParam == VK_F12)
		{
			LOG_CF_D(L"Run");
			g_emScriptStatus = em_Script_Status_Running;
		}
		else if (lpMsg->wParam == VK_F11)
		{
			LOG_CF_D(L"Stop");
			g_emScriptStatus = em_Script_Status_Stop;
		}
	}
	return _OldPeekMessageA(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
}

VOID CCommand::ExcutePtrToGame(_In_ std::function<VOID(VOID)> Ptr)
{
	std::lock_guard<CCommand> _LockPtr(*this);
	/*LOG_MSG_CF(L"ExcutePtrToGame");
	DWORD dwOldValue = RD(发包全局互斥基址);
	Ptr();
	*(DWORD*)发包全局互斥基址 = dwOldValue;*/
	if (!g_IsCommandMode)
	{
		while (_IsExistExcuteMethod)
			::Sleep(100);


		_ExcuteMethodPtr = Ptr;
		_IsExistExcuteMethod = true;
		::WaitForSingleObject(_hEvent, INFINITE);
	}
	else
	{
		Ptr();
	}
}
