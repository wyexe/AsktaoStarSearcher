#include "SearchStar.h"
#include <regex>
#include <LogLib/Log.h>
#include <MathLib/DistanceCalc.h>
#include <CharacterLib/Character.h>
#include <Feature/Core/Object/VecGameObject.h>
#include <Feature/Core/Object/Person/PersonAttribute.h>
#include <Feature/Core/CALL/GameCALL.h>
#include <Feature/Command/Command.h>
#include <Feature/Res/ResText.h>
#include <TimeLib/TimeRand.h>
#include <ProcessLib/KeyboardMsg/KeyboardMsg.h>
#include <map>

#define _SELF L"SearchStar.cpp"

CSearchStarService::CSearchStarService()
{
	_bLive = TRUE;
}

CSearchStarService::~CSearchStarService()
{
	
}

BOOL CSearchStarService::Run()
{
	return libTools::CException::InvokeFunc<BOOL>(__FUNCTIONW__, [=]
	{
		if (!CSocketAPI::GetInstance().Run())
		{
			LOG_MSG_CF(L"连接服务器失败!");
			return FALSE;
		}


		Initialize();


		g_emScriptStatus = em_Script_Status_Running;
		_RunSearchStarKeepALivePtr = std::async(std::launch::async, &CSearchStarService::RunSearchStarKeepALive, this);
		switch (_pAccountShareMemory->emScriptType)
		{
		case em_Script_Type::SearchStar:
			return SearchMapStar();
		case em_Script_Type::SearchChatStar:
			return SearchStar_In_ChatSystem();
		default:
			LOG_MSG_CF(L"无效的emScriptType=[%d]", _pAccountShareMemory->emScriptType);
			break;
		}
		return TRUE;
	});
}

BOOL CSearchStarService::SearchMapStar()
{
	// Send CurrentMap to Server  => Download vector<Map Pos>
	std::wstring _wsLastChannelName;
	while (true)
	{
		::Sleep(1000);
		if (!GameRun)
			continue;
		else if(CPersonAttribute::IsFighting())
			continue;



		_wsMapName = CPersonAttribute::GetCurrentMapName();
		if (_wsMapName.empty() || _wsMapName == L"无效地图")
			continue;

		if (SearchStar())
			continue;


		std::vector<Point> VecPoint;
		if (!CResText::GetInstance().GetResStarPoint(_wsMapName, VecPoint) || VecPoint.empty())
		{
			LOG_CF_E(L"Invalid MapName=[%s]", _wsMapName.c_str());
			continue;
		}
			

		std::wstring wsChannelName = CPersonAttribute::GetChannelName();
		if (_wsLastChannelName == wsChannelName)
			continue;


		auto MoveFuncPtr = [this](CONST Point& Pos)
		{
			if (_wsMapName != CPersonAttribute::GetCurrentMapName())
			{
				LOG_CF_D(L"新地图[%s], != [%s]", CPersonAttribute::GetCurrentMapName().c_str(), _wsMapName.c_str());
				return FALSE;
			}


			Point NewPos = Pos;
			if ((libTools::CTimeRand::GetRand(0, 10) % 2) == 0)
				NewPos.X += libTools::CTimeRand::GetRand(0, 4);
			else
				NewPos.X -= libTools::CTimeRand::GetRand(0, 4);

			if ((libTools::CTimeRand::GetRand(0, 10) % 3) == 0)
				NewPos.Y += libTools::CTimeRand::GetRand(0, 4);
			else
				NewPos.Y -= libTools::CTimeRand::GetRand(0, 4);


			LOG_CF_D(L"Searching [%s] [%d,%d]", _wsMapName.c_str(), Pos.X, Pos.Y);
			if (PlayerMove(NewPos))
			{
				LOG_CF_D(L"Find Target ……break!");
				return FALSE;
			}
			return TRUE;
		};


		_wsLastChannelName = wsChannelName;
		LOG_CF_D(L"Start Search [%s]", _wsMapName.c_str());
		auto& FirstPos = VecPoint.at(0);
		::Sleep(5 * 1000);
		if (libTools::CDistanceCalc::GetDisBy2D(CPersonAttribute::GetCurrentPoint(), FirstPos) < 24)
		{
			for (auto itr = VecPoint.begin() + 1; itr != VecPoint.end(); ++itr)
			{
				if (!MoveFuncPtr(*itr))
				{
					break;
				}
			}
		}
		else
		{
			for (auto itr = VecPoint.rbegin() + 1;  itr != VecPoint.rend(); ++itr)
			{
				if (!MoveFuncPtr(*itr))
				{
					break;
				}
			}
		}
		/*CGameUi NpcGuideDlg(0);
		_wsLastChannelName = wsChannelName;


		Point Pos1, Pos2;
		if (!CResText::GetInstance().GetResStarPoint(_wsMapName, _pAccountShareMemory->Config.StarConfig.wszLineName, Pos1, Pos2))
			continue;


		// 检查服务器上该地图是否出现了星
		MatchStar_Tranfer MatchStar;
		if(!CSocketAPI::GetInstance().SendAcceptTask_SearchStar(_wsMapName, _wsPlayerName, MatchStar) || MatchStar.wsChannel.empty())
			continue;
		

		// 等足3分钟
		libTools::CTimeCharacter::TimeContent FindedTick(MatchStar.Tick);
		ULONGLONG ulDiffSecond = 0;
		do 
		{
			::Sleep(1000);


			auto CurrentTick = libTools::CTimeCharacter::GetCurrentTick();
			ulDiffSecond = libTools::CTimeCharacter::DateDiff(FindedTick, CurrentTick, libTools::CTimeTick::em_TimeTick::em_TimeTick_Second);
			LOG_CF_D(L"星发现的时间是[%d:%d:%d], 当前是[%d:%d:%d], 相差[%d]s", 
				FindedTick.SysTime.wHour, FindedTick.SysTime.wMinute, FindedTick.SysTime.wSecond, 
				CurrentTick.SysTime.wHour, CurrentTick.SysTime.wMinute, CurrentTick.SysTime.wSecond, static_cast<int>(ulDiffSecond));
		} while (ulDiffSecond < 180 && GameRun);

		Point Pos = CPersonAttribute::GetCurrentPoint();
		if (libTools::CDistanceCalc::GetDisBy2D(Pos, Pos1) < libTools::CDistanceCalc::GetDisBy2D(Pos, Pos2))
			Pos = Pos2;
		else
			Pos = Pos1;


		MoveToPoint(Pos);*/
	}
	return TRUE;
}

BOOL CSearchStarService::SearchStar_In_ChatSystem()
{
	std::map<std::wstring, ULONGLONG> VecStarTick;

	g_emScriptStatus = em_Script_Status_Running;
	LOG_CF_D(L"开始搜索聊天系统...");
	std::vector<StarChatMsgContent> Vec;
	while (true)
	{
		if (!GameRun)
		{
			::Sleep(1000);
			continue;
		}


		if (!GetVecStar_In_ChatSystem(Vec))
		{
			::Sleep(1000);
			continue;
		}


		// Send Messagee to Server
		ReleaseSearchStarTaskContent_Tranfer StarInfo;
		for (auto& itm : Vec)
		{
			//std::vector<std::wstring> VecText;
			/*if(!libTools::CCharacter::SplitFormatText(itm.wsFullContent, L"*#R*#n，#Y*#n#R（*级）#n将在*下凡云游到#Z*#Z，各位道友快去该地图等候吧！", VecText) || VecText.size() != 6)
			{
				LOG_MSG_CF(L"无效的谣言格式:[%s]", itm.wsFullContent.c_str());
				break;
			}
			
			
			StarInfo.dwLevel = std::stoul(VecText.at(3));
			StarInfo.wsNpcName = itm.wsStarName;
			StarInfo.wsChannel = VecText.at(1);
			StarInfo.wsMapName = VecText.at(5);

			*/
			// BOSS
			/*if (!libTools::CCharacter::SplitFormatText(itm.wsFullContent, L"*据闻#Y*#n正在#R*#n作乱*", VecText) || VecText.size() != 2)
			{
				LOG_MSG_CF(L"无效的谣言格式:[%s]", itm.wsFullContent.c_str());
				break;
			}*/


			StarInfo.dwLevel = itm.dwLevel;
			StarInfo.wsNpcName = itm.wsStarName;
			StarInfo.wsChannel = itm.wsChannel;
			StarInfo.wsMapName = L"未知";


			std::vector<std::wstring> VecText;
			if(!libTools::CCharacter::SplitFormatText(itm.wsTick, L"#W*#n", VecText) || VecText.empty())
			{
				LOG_MSG_CF(L"无效的谣言日期格式:[%s]", itm.wsFullContent.c_str());
				break;
			}


			static ULONGLONG ulTick = 0;
			if (::GetTickCount64() - ulTick < _pAccountShareMemory->Config.RumorCfg.dwIngoreSec * 1000)
			{
				LOG_CF_D(L"当前星距离上一个地图[%s]星刷的时间不超过%ds……忽略!", StarInfo.wsMapName.c_str(), _pAccountShareMemory->Config.RumorCfg.dwIngoreSec);
				continue;
			}

			
			ulTick = ::GetTickCount64();
			//StarInfo.wsShowContent = libTools::CCharacter::MakeFormatText(L"[%s] [%s(%d级)] [%s,%s]", itm.wsTick.c_str(), itm.wsStarName.c_str(), StarInfo.dwLevel, StarInfo.wsChannel.c_str(), StarInfo.wsMapName.c_str());
			StarInfo.wsShowContent = libTools::CCharacter::MakeFormatText(L"[%s] [%s(%d级)] [%s]", itm.wsTick.c_str(), itm.wsStarName.c_str(), StarInfo.dwLevel, StarInfo.wsChannel.c_str());
			LOG_CF_D(L"%s", StarInfo.wsShowContent.c_str());
			::GetLocalTime(&StarInfo.Tick);
			if (!CSocketAPI::GetInstance().SendReleaseSearchStarTask(StarInfo))
			{
				LOG_MSG_CF(L"UploadStarInfo = FALSE!");
				_bLive = FALSE;
				break;
			}
		}

		Vec.clear();
	}
	return TRUE;
}

BOOL CSearchStarService::DownloadMapPoint()
{
	/*StarMap_Tranfer Content;
	if (!CSocketAPI::GetInstance().DownloadMapStarPoint(_wsSearchMapName.c_str(), Content))
		return FALSE;


	if (Content.emType == em_StarMap_Type::VecPoint)
	{
		_VecVecPoint = Content.VecVec;
		return TRUE;
	}*/


	/*if (Content.Vec.size() != 4)
	{
		LOG_MSG_CF(L"地图坐标.size=[%d]", Content.Vec.size());
		return FALSE;
	}*/


	//AddColumnsPath(Content.Vec);
	//AddRowsPath(Content.Vec);
	//AddCircularPath(Content.Vec);
	return TRUE;
}

BOOL CSearchStarService::PlayerMove(_In_ CONST Point& Pos)
{
	MoveToPoint(Pos);
	while (GameRun)
	{
		Point PersonPos = CPersonAttribute::GetCurrentPoint();
		if (libTools::CDistanceCalc::GetDisBy2D(Pos, PersonPos) < 12.0f)
		{
			//::Sleep(libTools::CTimeRand::GetRand(2, 4) * 1000);
			break;
		}



		//CheckExorcism();
		if (SearchStar())
			return TRUE;


		// 
		//UploadPlayerAttribute();
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

	return FALSE;
}

BOOL CSearchStarService::SearchStar()
{
	return libTools::CException::InvokeFunc<BOOL>(__FUNCTIONW__, [&] 
	{
		DWORD dwLevel = 0;
		std::vector<CNpc> Vec;
		CVecGameObject::GetVecNpc(Vec);
		

		for (auto& Npc : Vec)
		{
			LOG_C_D(L"Npc.Name[%s], Base=%X", Npc.GetName().c_str(), Npc.GetBase());
			if (Npc.GetNpcType() != CNpc::NpcType::WorldMonster)
				continue;


			auto wsNpcName = Npc.GetName();
			if (!CResText::GetInstance().IsStar(wsNpcName, &dwLevel))
				continue;


			LOG_C_D(L"Npc Is Star!");
			BOOL bUpload = TRUE;
			libTools::CException::InvokeAction(L"Exception in _VecOldStar", [&]
			{
				for (auto itr = _VecOldStar.begin(); itr != _VecOldStar.end();)
				{
					if ((::GetTickCount64() - itr->ulTick) < 35 * 60 * 1000 && itr->wsNpcName == wsNpcName && itr->Pos == Npc.GetPoint())
					{
						bUpload = FALSE;
					}
					itr = (::GetTickCount64() - itr->ulTick) >= 35 * 60 * 1000 ? _VecOldStar.erase(itr) : itr + 1;
				}
			});

			//LOG_C_D(L"wsNpcName=[%s], bUpload=[%d]", wsNpcName.c_str(), bUpload);
			if (bUpload)
			{
				Point NpcPos = Npc.GetPoint();
				_VecOldStar.emplace_back(wsNpcName, NpcPos);
				libTools::CException::InvokeAction(L"Exception in Upload _VecOldStar", [&]
				{
					libTools::CTimeCharacter::TimeContent LocalTime = libTools::CTimeCharacter::GetCurrentTick();
					// Upload to Server
					StarInfo_Tranfer Info;
					Info.wsNpcName = wsNpcName;
					Info.dwLevel = dwLevel;
					Info.emStarType = em_StarType::Normal;
					Info.wsChannelText = CPersonAttribute::GetChannelName();
					Info.wsShowContent = libTools::CCharacter::MakeFormatText(L"[%02d:%02d:%02d] [%s发现 %s (%d级)] 在 [%s, %s, %d, %d]",
						static_cast<DWORD>(LocalTime.SysTime.wHour),
						static_cast<DWORD>(LocalTime.SysTime.wMinute),
						static_cast<DWORD>(LocalTime.SysTime.wSecond),
						_pAccountShareMemory->wszAliasName,
						wsNpcName.c_str(),
						dwLevel,
						Info.wsChannelText.c_str(),
						_wsMapName.c_str(),
						NpcPos.X,
						NpcPos.Y);


					LOG_CF_D(L"%s", Info.wsShowContent.c_str());
					CSocketAPI::GetInstance().UploadStarInfo(Info);
					LOG_CF_D(L"上传完毕!");
				});


				// 靠近  然后暂停
				StopMove();
				::Sleep(1000);
				MoveToNpc(NpcPos);
				g_emScriptStatus = em_Script_Status_Stop;
				return TRUE;
			}
		}
		return FALSE;
	});
}

VOID CSearchStarService::CheckExorcism() CONST
{
	static ULONGLONG ulTick = ::GetTickCount64();
	static int dwRemainMinute = 0;
	while (dwRemainMinute == 0)
	{
		dwRemainMinute = static_cast<int>(GetRemainExorcismTaskMinute());
		ulTick = ::GetTickCount64();
		::Sleep(500);
	}
	

	if (static_cast<int>(::GetTickCount64() - ulTick) / 1000 / 60 >= dwRemainMinute - 3 )
	{
		LOG_CF_D(L"准备使用驱魔香! dwRemainMinute = [%d]Minute, Tick=[%d]Minute", dwRemainMinute, static_cast<int>(::GetTickCount64() - ulTick) / 1000 / 60);


		CItemObject Exorcism(0);
		if (ExistExorcism(&Exorcism))
		{
			LOG_CF_D(L"使用驱魔香");
			Exorcism.UseItem();
			::Sleep(1000);


			// Refresh
			dwRemainMinute = static_cast<int>(GetRemainExorcismTaskMinute());
			ulTick = ::GetTickCount64();
		}
	}
}

DWORD CSearchStarService::GetRemainExorcismTaskMinute() CONST
{
	std::vector<CTaskObject> Vec;
	CVecGameObject::GetVecTask(Vec);


	auto itr = std::find_if(Vec.begin(), Vec.end(), [](CONST CTaskObject& itm)
	{
		auto wsTaskType = itm.GetTaskType();
		auto wsTaskRawType = itm.GetRawType();
		return wsTaskType == L"强力驱魔香时间" || wsTaskType == L"驱魔香时间" || wsTaskRawType == L"强力驱魔香时间" || wsTaskRawType == L"驱魔香时间";
	});


	if (itr != Vec.end())
	{
		// 检查驱魔香剩余时间
		std::vector<std::wstring> VecText;
		std::wstring wsTaskPrompt;


		wsTaskPrompt = itr->GetTaskPrompt();
		libTools::CCharacter::SplitFormatText(wsTaskPrompt, L"*有效时间还有#R*#n分钟*", VecText);
		if (VecText.size() == 3)
		{
			LOG_CF_D(L"驱魔香任务:[%s]", wsTaskPrompt.c_str());

			DWORD dwMinute = std::stoi(VecText.at(1));
			LOG_CF_D(L"剩余时间:[%d]", dwMinute);
			return dwMinute;
		}


		return 0;
	}


	return 0;
}

BOOL CSearchStarService::ExistExorcism(_Out_ CItemObject* pItemObject /*= nullptr*/) CONST
{
	std::vector<CItemObject> Vec;
	CVecGameObject::GetVecBagItem(Vec);
	auto itr = std::find_if(Vec.begin(), Vec.end(), [](CONST CItemObject& itm) 
	{ 
		auto wsItemName = itm.GetName();
		return wsItemName == L"强力驱魔香" || wsItemName == L"驱魔香";
	});


	if (itr != Vec.end() && pItemObject != nullptr)
	{
		*pItemObject = *itr;
	}
	if (itr == Vec.end())
	{
		for (auto& itm : Vec)
		{
			LOG_CF_E(L"Item.Name=[%s]", itm.GetName().c_str());
		}
		LOG_MSG_CF(L"找不到驱魔香!");
	}
	return itr != Vec.end();
}

BOOL CSearchStarService::Initialize()
{
	_wsPlayerName = CPersonAttribute::GetPlayerName();;
	_pAccountShareMemory = g_pAccountShareMemory;
	return TRUE;
}

BOOL CSearchStarService::SwitchChannel()
{
	DWORD dwCurrentChannel = libTools::CTimeRand::GetRand(5, 20);
	while (dwCurrentChannel == 0 || dwCurrentChannel > 20)
	{
		LOG_CF_D(L"dwCurrentChannel=%d", dwCurrentChannel);
		dwCurrentChannel = libTools::CTimeRand::GetRand(5, 20);
	}

	LOG_CF_D(L"dwCurrentChannel=%d", dwCurrentChannel);
	auto wsFullChannelName = CPersonAttribute::GetServerName() + CResText::GetInstance().GetChannelName(dwCurrentChannel);
	if (!wsFullChannelName.empty())
	{
		LOG_CF_D(L"准备切换频道:[%s]", wsFullChannelName.c_str());
		CCommand::GetInstance().ExcutePtrToGame([wsFullChannelName] { CGameCALL::SwitchChannel(libTools::CCharacter::UnicodeToASCII(wsFullChannelName).c_str()); });
	}
	return TRUE;
}

VOID CSearchStarService::MoveToPoint(_In_ CONST Point& Pos) CONST
{
/*
	if (CPersonAttribute::GetCurrentMapName() == std::wstring(_wsSearchMapName.c_str()))
	{
		CCommand::GetInstance().ExcutePtrToGame([Pos] { CGameCALL::MoveToPoint(Pos); });
	}
	else
	{
		// 寻路
		LOG_C_D(L"跨图寻路..");
		std::string Text = libTools::CCharacter::UnicodeToASCII(libTools::CCharacter::MakeFormatText(L"%s(%d,%d)", _wsSearchMapName.c_str(), Pos.X, Pos.Y));
		CCommand::GetInstance().ExcutePtrToGame([Text] {CGameCALL::AutoFindPath(Text.c_str()); });
	}*/
	CGameUi PanInfoDlg(0);
	if (CGameUi::GetRoot().FindChildGameUi(L"PanInfoDlg", &PanInfoDlg, FALSE))
	{
		CGameUi NPCBtn(0);
		if (PanInfoDlg.FindChildGameUi(L"NPCBtn", &NPCBtn, TRUE))
		{
			LOG_C_D(L"Click NPCBtn");
			NPCBtn.Click();
			::Sleep(1000);

			const HWND hWnd = CCommand::GetInstance().GetGameHwnd();
			for (CONST auto &itm : libTools::CCharacter::MakeFormatText(L"%d.%d", Pos.X, Pos.Y))
			{
				libTools::CKeyboardMsg::SimulationKey(hWnd, static_cast<CHAR>(itm));
				::Sleep(500);
			}
			::Sleep(500);
			libTools::CKeyboardMsg::SimulationKey(hWnd, VK_RETURN);
		}
	}
}

VOID CSearchStarService::RunSearchStarKeepALive()
{
	while (_bLive)
	{
		libTools::CException::InvokeAction(__FUNCTIONW__, [&] 
		{
			//LOG_CF_D(L"SendConsoleKeepALive");
			ClientKeepALive_Tranfer Content;
			Content.emType = em_KeepALive_Type::GameClient;
			Content.wsComputerName = L"随便吧";
			Content.wsPlayerName = _wsPlayerName;
			Content.wsLocation = _wsMapName.empty() ? L"无效地图" : _wsMapName;
			if (!CSocketAPI::GetInstance().SendKeepALive(Content))
			{
				::MessageBoxW(NULL, L"呃……游戏与我们的服务器失去连接...", L"", NULL);
				::TerminateProcess(::GetCurrentProcess(), 0);
				return;
			}
			//LOG_CF_D(L"SendConsoleKeepALive Done");

			::Sleep(5 * 1000);
		});
	}
}

BOOL CSearchStarService::IsExistLaoJun() CONST
{
	CGameUi FightTalkMenuDlg(0), TalkNoMenuDlg(0);


	std::vector<CGameUi> Vec;
	if ((CGameUi::GetRoot().FindChildGameUi(L"FightTalkMenuDlg", &FightTalkMenuDlg, FALSE) && FightTalkMenuDlg.FindChildGameUi(L"Instruction", nullptr, TRUE)) ||
		(CGameUi::GetRoot().FindChildGameUi(L"TalkNoMenuDlg", &TalkNoMenuDlg, FALSE) && TalkNoMenuDlg.FindChildGameUi(L"Instruction", nullptr, TRUE)))
	{
		LOG_C_D(L"出现老君!!!");
		/*ClientKeepALive_Tranfer Content;
		Content.emType = em_KeepALive_Type::AnswerQuestion;
		Content.wsComputerName = g_pShareMemoryContent->wszComputerName;
		CSocketAPI::GetInstance().SendKeepALive(Content);*/
		return TRUE;
	}
	return FALSE;
}

VOID CSearchStarService::UploadPlayerAttribute() CONST
{
	while (GameRun && IsExistLaoJun())
	{
		::Sleep(2000);
	}

	static ULONGLONG ulTick = ::GetTickCount64();
	if (::GetTickCount64() - ulTick >= 5 * 1000)
	{
		ClientKeepALive_Tranfer Content;
		Content.emType = em_KeepALive_Type::GameClient;
		//Content.wsPlayerName = _wsPlayerName;
		Content.wsPlayerName = _pAccountShareMemory->wszAliasName;
		Content.wsComputerName = g_pShareMemoryContent->wszComputerName;
		LOG_C_D(L"wszAliasName=%s", _pAccountShareMemory->wszAliasName);
		//Point Pos = CPersonAttribute::GetCurrentPoint();
		//Content.wsLocation = libTools::CCharacter::MakeFormatText(L"[%s %s %d-%d]", CPersonAttribute::GetChannelName().c_str(), CPersonAttribute::GetCurrentMapName().c_str(), Pos.X, Pos.Y);
		CSocketAPI::GetInstance().SendKeepALive(Content);
		ulTick = ::GetTickCount64();
	}
}

VOID CSearchStarService::AddColumnsPath(_In_ CONST std::vector<Point>& Vec)
{
	Point LeftTopPos = Vec.at(0), LeftBottomPos = Vec.at(1), RightTop = Vec.at(2), RightBottomPos = Vec.at(3);

	// 归一化处理成矩形
	LeftTopPos.X = min(LeftTopPos.X, LeftBottomPos.X);
	LeftTopPos.Y = max(LeftTopPos.Y, RightTop.Y);
	LeftBottomPos.X = min(LeftTopPos.X, LeftBottomPos.X);
	LeftBottomPos.Y = min(LeftBottomPos.Y, RightBottomPos.Y);
	RightTop.X = max(RightTop.X, RightBottomPos.X);
	RightTop.Y = max(LeftTopPos.Y, RightTop.Y);
	RightBottomPos.X = max(RightTop.X, RightBottomPos.X);
	RightBottomPos.Y = min(LeftBottomPos.Y, RightBottomPos.Y);


	std::vector<Point> VecTop;
	int MaxXCount = (RightTop.X - LeftTopPos.X) % 30 == 0 ? (RightTop.X - LeftTopPos.X) / 30 : (RightTop.X - LeftTopPos.X) / 30 + 1;
	for (int i = 0; i < MaxXCount; ++i)
	{
		VecTop.push_back(Point(LeftTopPos.X + i * 30, RightTop.Y));
	}
	VecTop.push_back(Point(RightTop.X, RightTop.Y));


	std::vector<Point> VecBottom;
	MaxXCount = (RightBottomPos.X - LeftBottomPos.X) % 30 == 0 ? (RightBottomPos.X - LeftBottomPos.X) / 30 : (RightBottomPos.X - LeftBottomPos.X) / 30 + 1;
	for (int i = 0; i < MaxXCount; ++i)
	{
		VecBottom.push_back(Point(LeftBottomPos.X + i * 30, RightBottomPos.Y));
	}
	VecBottom.push_back(Point(RightBottomPos.X, RightBottomPos.Y));



	BOOL bReverse = FALSE;

	std::vector<Point> VecPoint;
	for (UINT i = 0; i < VecTop.size() && i < VecBottom.size(); ++i)
	{
		if (bReverse)
		{
			VecPoint.push_back(VecBottom.at(i));
			VecPoint.push_back(VecTop.at(i));
		}
		else
		{
			VecPoint.push_back(VecTop.at(i));
			VecPoint.push_back(VecBottom.at(i));
		}
		bReverse = !bReverse;
	}
	_VecVecPoint.push_back(VecPoint);
}

VOID CSearchStarService::AddRowsPath(_In_ CONST std::vector<Point>& Vec)
{
	Point LeftTopPos = Vec.at(0), LeftBottomPos = Vec.at(1), RightTop = Vec.at(2), RightBottomPos = Vec.at(3);

	// 归一化处理成矩形
	LeftTopPos.X = min(LeftTopPos.X, LeftBottomPos.X);
	LeftTopPos.Y = max(LeftTopPos.Y, RightTop.Y);
	LeftBottomPos.X = min(LeftTopPos.X, LeftBottomPos.X);
	LeftBottomPos.Y = min(LeftBottomPos.Y, RightBottomPos.Y);
	RightTop.X = max(RightTop.X, RightBottomPos.X);
	RightTop.Y = max(LeftTopPos.Y, RightTop.Y);
	RightBottomPos.X = max(RightTop.X, RightBottomPos.X);
	RightBottomPos.Y = min(LeftBottomPos.Y, RightBottomPos.Y);


	std::vector<Point> VecLeft;
	int MaxYCount = (LeftBottomPos.Y - LeftTopPos.Y) % 20 == 0 ? (LeftBottomPos.Y - LeftTopPos.Y) / 20 : (LeftBottomPos.Y - LeftTopPos.Y) / 20 + 1;
	for (int i = 0; i < MaxYCount; ++i)
	{
		VecLeft.push_back(Point(LeftTopPos.X, LeftTopPos.Y + i * 20));
	}
	VecLeft.push_back(Point(LeftBottomPos.X, LeftBottomPos.Y));


	std::vector<Point> VecRight;
	MaxYCount = (RightBottomPos.Y - RightTop.Y) % 20 == 0 ? (RightBottomPos.Y - RightTop.Y) / 20 : (RightBottomPos.Y - RightTop.Y) / 20 + 1;
	for (int i = 0; i < MaxYCount; ++i)
	{
		VecRight.push_back(Point(RightTop.X, RightTop.Y + i * 20));
	}
	VecRight.push_back(Point(RightBottomPos.X, RightBottomPos.Y));



	BOOL bReverse = FALSE;

	std::vector<Point> VecPoint;
	for (UINT i = 0; i < VecLeft.size() && i < VecRight.size(); ++i)
	{
		if (bReverse)
		{
			VecPoint.push_back(VecRight.at(i));
			VecPoint.push_back(VecLeft.at(i));
		}
		else
		{
			VecPoint.push_back(VecLeft.at(i));
			VecPoint.push_back(VecRight.at(i));
		}
		bReverse = !bReverse;
	}
	_VecVecPoint.push_back(VecPoint);
}

VOID CSearchStarService::AddCircularPath(_In_ CONST std::vector<Point>& Vec)
{
	Point LeftTopPos = Vec.at(0), LeftBottomPos = Vec.at(1), RightTop = Vec.at(2), RightBottomPos = Vec.at(3);

	// 归一化处理成矩形
	LeftTopPos.X = min(LeftTopPos.X, LeftBottomPos.X);
	LeftTopPos.Y = max(LeftTopPos.Y, RightTop.Y);
	LeftBottomPos.X = min(LeftTopPos.X, LeftBottomPos.X);
	LeftBottomPos.Y = min(LeftBottomPos.Y, RightBottomPos.Y);
	RightTop.X = max(RightTop.X, RightBottomPos.X);
	RightTop.Y = max(LeftTopPos.Y, RightTop.Y);
	RightBottomPos.X = max(RightTop.X, RightBottomPos.X);
	RightBottomPos.Y = min(LeftBottomPos.Y, RightBottomPos.Y);


	std::vector<Point> VecPoint;
	Point CenterPos(RightBottomPos.X / 2, RightBottomPos.Y / 2);
	VecPoint.push_back(CenterPos); // CenterPos
	

	for (int i/*StoreLevel*/ = 1; i < 100; ++i)
	{
		//Point Rect_LeftPoint(CenterPos.X - i * 30, CenterPos.Y);
		Point Rect_LeftTopPos(CenterPos.X - i * 30, CenterPos.Y - i * 30);
		Point Rect_LeftBottomPos(CenterPos.X - i * 30, CenterPos.Y + i * 30);
		Point Rect_RightTopPos(CenterPos.X + i * 30, CenterPos.Y - i * 30);
		Point Rect_RightBottomPos(CenterPos.X + i * 30, CenterPos.Y + i * 30);


		if (Rect_LeftTopPos.X < LeftTopPos.X)
			Rect_LeftTopPos.X = LeftTopPos.X;
		if (Rect_LeftTopPos.Y < LeftTopPos.Y)
			Rect_LeftTopPos.Y = LeftTopPos.Y;


		if (Rect_LeftBottomPos.X < LeftBottomPos.X)
			Rect_LeftBottomPos.X = LeftBottomPos.X;
		if (Rect_LeftBottomPos.Y > LeftBottomPos.Y)
			Rect_LeftBottomPos.Y = LeftBottomPos.Y;


		if (Rect_RightTopPos.X > RightTop.X)
			Rect_RightTopPos.X = RightTop.X;
		if (Rect_RightTopPos.Y < RightTop.Y)
			Rect_RightTopPos.Y = RightTop.Y;


		if (Rect_RightBottomPos.X > RightBottomPos.X)
			Rect_RightBottomPos.X = RightBottomPos.X;
		if (Rect_RightBottomPos.Y > RightBottomPos.Y)
			Rect_RightBottomPos.Y = RightBottomPos.Y;



		if (Rect_LeftTopPos == LeftTopPos && Rect_LeftBottomPos == LeftBottomPos && Rect_RightTopPos == RightTop && Rect_RightBottomPos == RightBottomPos)
		{
			VecPoint.push_back(Rect_LeftBottomPos);
			VecPoint.push_back(Rect_LeftTopPos);
			VecPoint.push_back(Rect_RightTopPos);
			VecPoint.push_back(Rect_RightBottomPos);
			VecPoint.push_back(Rect_LeftBottomPos);
			break;
		}


		VecPoint.push_back(Rect_LeftBottomPos);
		VecPoint.push_back(Rect_LeftTopPos);
		VecPoint.push_back(Rect_RightTopPos);
		VecPoint.push_back(Rect_RightBottomPos);
		VecPoint.push_back(Rect_LeftBottomPos);
	}

	_VecVecPoint.push_back(VecPoint);
}

VOID CSearchStarService::MoveToNpc(_In_ CONST Point& NpcPos) CONST
{
	Point Pos = NpcPos;
	Pos.X += 2;
	Pos.Y += 3;
	MoveToPoint(Pos);
}

VOID CSearchStarService::StopMove() CONST
{
	libTools::CMemory::WriteDWORD(RD(走路基址) + 0xC, RD(RD(走路基址) + 0x8));
}

BOOL CSearchStarService::IsExistPlayer(_In_ CONST Point& Pos) CONST
{
	std::vector<CNpc> Vec;
	CVecGameObject::GetVecNpc(Vec);
	for (auto& Npc : Vec)
	{
		if (!Npc.IsPlayer())
			continue;


		LOG_CF_D(L"地星附近有Player[%s], 距离是[%.2f]", Npc.GetName().c_str(), libTools::CDistanceCalc::GetDisBy2D(Npc.GetPoint(), Pos));
		if (Npc.GetName() != _wsPlayerName && libTools::CDistanceCalc::GetDisBy2D(Npc.GetPoint(), Pos) <= 15.0f)
		{
			return TRUE;
		}
	}
	return FALSE;
}

VOID CSearchStarService::WaitToReStart()
{
	for (int i = 0;i < 60 / 5 && GameRun; ++i)
	{
		ClientKeepALive_Tranfer Content;
		Content.emType = em_KeepALive_Type::GameLog;
		Content.wsPlayerName = _pAccountShareMemory->wszAliasName;
		Content.wsComputerName = g_pShareMemoryContent->wszComputerName;
		Content.wsLogContent = libTools::CCharacter::MakeFormatText(L"等星 [%d]秒", i * 5);
		CSocketAPI::GetInstance().SendKeepALive(Content);


		::Sleep(5 * 1000);
	}
}

BOOL CSearchStarService::GetVecStar_In_ChatSystem(_Out_ std::vector<StarChatMsgContent>& VecChatStar)
{
	std::vector<StarChatMsgContent> Vec;
	CVecGameObject::GetVecChat(_LastStarChatMsgContent, Vec);
	for (auto& itm : Vec)
	{
		// #[#image<0>#]#W听闻在#R经典服二十线#n，#Y地阖星#n#R（96级）#n将在#R三分钟后#n下凡云游到#Z百花谷二#Z，各位道友快去该地图等候吧！
		//if (itm.wsFullContent.find(L"下凡云游") == std::wstring::npos)
		//	continue;

		// #[#image<0>#]#W据闻#Y牛魔王#n正在#R经典服七线#n作乱，建议等级在#R90-129#n级的有志之士前去斩妖除魔，保卫中洲。
		if (itm.wsFullContent.find(L"据闻") == std::wstring::npos || itm.wsFullContent.find(L"作乱") == std::wstring::npos)
			continue;


		LOG_CF_D(L"Tick[%s] Checking ...", itm.wsTick.c_str());
		// Check Time.  1 minute Max !!!
		//auto ChatTick = ConvertChatTick(itm.wsTick);
		//if (libTools::CTimeCharacter::DateDiff(libTools::CTimeCharacter::GetCurrentTick(), ChatTick, libTools::CTimeTick::em_TimeTick::em_TimeTick_Second) >= 60)
		//	continue;



		std::vector<std::wstring> VecStarInfo;
		libTools::CCharacter::SplitFormatText(itm.wsFullContent, L"*#Y*#n正在#R*#n*", VecStarInfo);
		if (VecStarInfo.size() != 4)
		{
			LOG_MSG_CF(L"无效的谣言文本:[%s]", itm.wsFullContent.c_str());
			continue;
		}

		LOG_CF_D(L"StarName=[%s]", VecStarInfo.at(1).c_str());
		itm.wsStarName = VecStarInfo.at(1);
		itm.wsChannel = VecStarInfo.at(2);



		LOG_CF_D(L"IsStar Checking ...");
		if (!CResText::IsStar(itm.wsStarName, &itm.dwLevel))
			continue;


		LOG_CF_D(L"Is BOSS!");
		VecChatStar.push_back(itm);
	}


	// cause at(0) = Newest Star. Keep it
	
	if (!VecChatStar.empty())
	{
		_LastStarChatMsgContent = VecChatStar.at(0);
	}
	else if (!Vec.empty())
	{
		_LastStarChatMsgContent = Vec.at(0);
	}
	return !VecChatStar.empty();
}

libTools::CTimeCharacter::TimeContent CSearchStarService::ConvertChatTick(_In_ CONST std::wstring& wsTickText) CONST
{
	SYSTEMTIME LocalTime;
	::GetLocalTime(&LocalTime);


	std::vector<std::wstring> VecTickText;
	libTools::CCharacter::SplitFormatText(wsTickText, L"#W*:*:*#n", VecTickText);
	if (VecTickText.size() != 3)
	{
		LOG_MSG_CF(L"无效的时间文本:[%s]", wsTickText.c_str());
		return libTools::CTimeCharacter::TimeContent(LocalTime);
	}


	// 17:06:46
	if (LocalTime.wHour > std::stoi(VecTickText.at(0))) // 凌晨
		LocalTime.wDay += 1;


	LocalTime.wHour = static_cast<WORD>(std::stoi(VecTickText.at(0)));
	LocalTime.wMinute = static_cast<WORD>(std::stoi(VecTickText.at(1)));
	LocalTime.wSecond = static_cast<WORD>(std::stoi(VecTickText.at(2)));
	LocalTime.wMilliseconds = 0;
	return libTools::CTimeCharacter::TimeContent(LocalTime);
}
