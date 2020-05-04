#ifndef __ASKTAO_ASKTAO_FEATURE_COMMON_GAMESTRUCT_H__
#define __ASKTAO_ASKTAO_FEATURE_COMMON_GAMESTRUCT_H__

#include <Windows.h>
#include <ProcessLib/Memory/Memory.h>

#ifdef _PRIVATESERVER
#define 人物属性基址			0xD09C0C
#define 人物属性偏移1			0x104
#define 人物属性偏移2			0xCC
#define 人物属性偏移3			0x280
#define 发包CALL				0x804FF0
#define 背包遍历基址			0xE42BA4
#define 任务遍历基址         0xE459CC
#define 是否战斗中基址		0xE429B8
#define 走路基址				0xE4569C
#define 走路CALL				0x760850
#define Npc遍历基址			0xD090F4
#define 人物系统属性基址		0xE3FFE4
#define 换线基址				0xE41BE4
#define 换线CALL				0x66AF50
#define UI遍历基址			0xE4293C
#define UI遍历偏移			0x1E4
#define UI点击CALL			0x53A890
#define 发包全局互斥基址		0xE49668
#define 寻路基址				0xE4169C
#define 寻路CALL1			0x61AD90
#define 寻路CALL2			0x61C7A0
#define 聊天窗口基址         0xE41AE0
#define 聊天窗口偏移			0x48
#define 怪物遍历偏移			0x3C0
#define UI的MenuId偏移		0x38C
#define 人物补充状态CALL		0xA05FD0
#define 宠物补充状态CALL		0xA06E80
#define 宠物遍历偏移			0x314
#define 宠物遍历基址         0xE456A8
#define 宠物技能基址         0xE46334
#define Npc类型偏移			0x28C

#define 换线偏移				0x68
#define Npc名字CALL			0xE8
#define NpcIDCALL			0xE4
#define Npc坐标X				0x1E8
#define UI_Name偏移			0x94
#define UI点击偏移			0x13C
#define 任务遍历偏移			0x98
#define Npc遍历偏移			0x28
#define Player名字偏移		0x150
#define 是否玩家偏移			0x38
#define 是否怪物偏移			0x58
#define 宠物技能偏移			0x24
#else
#define 人物属性基址			0xE55480
#define 人物属性偏移1			0x120
#define 人物属性偏移2			0xE8
#define 人物属性偏移3			0x35C
#define 发包CALL				0x80E660
#define 背包遍历基址			0xE4CBC0 // !
#define 任务遍历基址         0xE54AAC
#define 是否战斗中基址		0xE51A80
#define 走路基址				0xE5477C
#define 走路CALL				0x768320
#define Npc遍历基址			0xE549FC
#define 人物系统属性基址		0xE4F08C
#define 换线基址				0xE50CAC
#define 换线CALL				0x671F60
#define UI遍历基址			0xE51A04
#define UI遍历偏移			0x1E4
#define UI点击CALL			0x5410C0
#define 发包全局互斥基址		0xE58760
#define 寻路基址				0xE50764
#define 寻路CALL1           0x621B70
#define 寻路CALL2           0x623580
#define 聊天窗口基址         0xE50BA8
#define 聊天窗口偏移			0x48
#define 怪物遍历偏移			0x3C0
#define UI的MenuId偏移		0x38C
#define 人物补充状态CALL		0xA0FCA0
#define 宠物补充状态CALL		0xA10A70
#define 宠物遍历偏移			0x314
#define 宠物遍历基址			0xE54788
#define 宠物技能基址			0xE55430
#define Npc类型偏移			0x3A8
#define 周围所有对象基址		0xE54630
#define 周围所有对象偏移		0x1F8


#define 换线偏移				0x68
#define Npc名字CALL			0xEC
#define NpcIDCALL			0xE8
#define Npc坐标X				0x1FC
#define UI_Name偏移			0x94
#define UI点击偏移			0x13C
#define 任务遍历偏移			0x98
#define Npc遍历偏移			0x28
#define Player名字偏移		0x170
#define 是否玩家偏移			0x38
#define 是否怪物偏移			0x58
#define 宠物技能偏移			0x24
#endif // _PRIVATESERVER





#define SHAREMEMORY_MAPPING_NAME	L"AskTao_ShareMemory_Name"
#define MAX_PLAYERCOUNT_SHAREMEMORY 10
#define CONSOLE_NAME				L"calc.exe"
#define RC4_KEY						"9c7ffc796d0404c6de069f44b0f7df"
#define SHAREMEMORY_FILE_NAME		L"gdsdkq387420489a536870912zbldz.tmp"

#define SKILLID_腾云驾雾		0x134

enum em_Script_Status
{
	em_Script_Status_None,
	em_Script_Status_Running,
	em_Script_Status_Stop
};


enum class ChannelType : DWORD
{
	谣言 = 0xA,
	交易 = 0xB,
	系统 = 0xFFFFFFFF,
	世界 = 0x5
};


#define RD(Addr) libTools::CMemory::ReadDWORD(Addr)
#define RB(Addr) (RD(Addr) & 0xFF)


struct Point
{
	int X;
	int Y;

	Point()
	{
		X = Y = 0;
	}


	Point(_In_ int dwX, _In_ int dwY)
	{
		X = dwX;
		Y = dwY;
	}
	Point(_In_ float dwX, _In_ float dwY)
	{
		X = static_cast<int>(dwX);
		Y = static_cast<int>(dwY);
	}

	bool operator == (CONST Point& Pos) CONST
	{
		return this->X == Pos.X && this->Y == Pos.Y;
	}
};

struct GameText
{
	union
	{
		CHAR	szText[16];
		CHAR*	pszTextPtr;
	};
	DWORD dwSize;
	DWORD dwMaxSize;

	GameText(_In_ LPCSTR pszText)
	{
		dwSize = strlen(pszText);
		if (dwSize > 0xF)
		{
			dwMaxSize = dwSize * 2;
			this->pszTextPtr = new CHAR[dwMaxSize];
			ZeroMemory(this->pszTextPtr, dwMaxSize);
			memcpy(this->pszTextPtr, pszText, dwSize);
		}
		else
		{
			dwMaxSize = 0xF;
			ZeroMemory(szText, sizeof(szText));
			memcpy(this->szText, pszText, dwSize);
		}
	}

	~GameText()
	{
		if (dwMaxSize > 0xF)
		{
			delete[] pszTextPtr;
			pszTextPtr = nullptr;
		}
	}
};

struct StarChatMsgContent
{
	std::wstring wsStarName;
	std::wstring wsTick;
	std::wstring wsFullContent;
	std::wstring wsChannel;
	DWORD		 dwLevel = 0;

	StarChatMsgContent() = default;
	StarChatMsgContent(_In_ CONST std::wstring wsTick_, _In_ CONST std::wstring wsContent_) : wsTick(wsTick_), wsFullContent(wsContent_) {}
};

#define ASKTAO_SHAREMEMORY_NAME L"ASK_SHAREMEMORY"

enum class em_Script_Type : DWORD
{
	SearchStar			= 0x1,
	SearchChatStar		= 0x2,
	AutoFigter			= 0x3,
	TESTFORZEN			= 0x4
};

struct SearchStarConfig
{
	WCHAR				wszLineName[32];
};

struct SearChatStarConfig
{
	DWORD dwChannel;
};

struct CatchPetConfig
{
	WCHAR wszMapName[32];
};

struct RumorConfig
{
	DWORD dwIngoreSec;
};

struct AccountShareMemory
{
	WCHAR				wszPlayerName[32];
	WCHAR				wszAliasName[32];
	BOOL				bLogin;
	BOOL				bLive;
	DWORD				dwPid;
	em_Script_Type		emScriptType;

	union
	{
		CatchPetConfig		CatchConfig;
		SearchStarConfig	StarConfig;
		RumorConfig			RumorCfg;
	} Config;
};


struct ShareMemoryContent
{
	// 4M
	CHAR					CheatFileContentPtr[1024 * 1024 * 4];
	UINT					uFileSize;


	WCHAR					wszConsolePath[MAX_PATH];
	

	UINT					uAccountCount;
	AccountShareMemory		Account[1000];


	WCHAR					wszComputerName[32];
};


extern ShareMemoryContent* g_pShareMemoryContent;
extern em_Script_Status g_emScriptStatus;
extern AccountShareMemory* g_pAccountShareMemory;
extern BOOL					g_IsCommandMode;
#define GameRun (g_emScriptStatus == em_Script_Status_Running)
#define StopGame() g_emScriptStatus = em_Script_Status_Stop;
#endif // !__ASKTAO_ASKTAO_FEATURE_COMMON_GAMESTRUCT_H__
