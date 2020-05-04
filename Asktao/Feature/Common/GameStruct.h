#ifndef __ASKTAO_ASKTAO_FEATURE_COMMON_GAMESTRUCT_H__
#define __ASKTAO_ASKTAO_FEATURE_COMMON_GAMESTRUCT_H__

#include <Windows.h>
#include <ProcessLib/Memory/Memory.h>

#ifdef _PRIVATESERVER
#define �������Ի�ַ			0xD09C0C
#define ��������ƫ��1			0x104
#define ��������ƫ��2			0xCC
#define ��������ƫ��3			0x280
#define ����CALL				0x804FF0
#define ����������ַ			0xE42BA4
#define ���������ַ         0xE459CC
#define �Ƿ�ս���л�ַ		0xE429B8
#define ��·��ַ				0xE4569C
#define ��·CALL				0x760850
#define Npc������ַ			0xD090F4
#define ����ϵͳ���Ի�ַ		0xE3FFE4
#define ���߻�ַ				0xE41BE4
#define ����CALL				0x66AF50
#define UI������ַ			0xE4293C
#define UI����ƫ��			0x1E4
#define UI���CALL			0x53A890
#define ����ȫ�ֻ����ַ		0xE49668
#define Ѱ·��ַ				0xE4169C
#define Ѱ·CALL1			0x61AD90
#define Ѱ·CALL2			0x61C7A0
#define ���촰�ڻ�ַ         0xE41AE0
#define ���촰��ƫ��			0x48
#define �������ƫ��			0x3C0
#define UI��MenuIdƫ��		0x38C
#define ���ﲹ��״̬CALL		0xA05FD0
#define ���ﲹ��״̬CALL		0xA06E80
#define �������ƫ��			0x314
#define ���������ַ         0xE456A8
#define ���＼�ܻ�ַ         0xE46334
#define Npc����ƫ��			0x28C

#define ����ƫ��				0x68
#define Npc����CALL			0xE8
#define NpcIDCALL			0xE4
#define Npc����X				0x1E8
#define UI_Nameƫ��			0x94
#define UI���ƫ��			0x13C
#define �������ƫ��			0x98
#define Npc����ƫ��			0x28
#define Player����ƫ��		0x150
#define �Ƿ����ƫ��			0x38
#define �Ƿ����ƫ��			0x58
#define ���＼��ƫ��			0x24
#else
#define �������Ի�ַ			0xE55480
#define ��������ƫ��1			0x120
#define ��������ƫ��2			0xE8
#define ��������ƫ��3			0x35C
#define ����CALL				0x80E660
#define ����������ַ			0xE4CBC0 // !
#define ���������ַ         0xE54AAC
#define �Ƿ�ս���л�ַ		0xE51A80
#define ��·��ַ				0xE5477C
#define ��·CALL				0x768320
#define Npc������ַ			0xE549FC
#define ����ϵͳ���Ի�ַ		0xE4F08C
#define ���߻�ַ				0xE50CAC
#define ����CALL				0x671F60
#define UI������ַ			0xE51A04
#define UI����ƫ��			0x1E4
#define UI���CALL			0x5410C0
#define ����ȫ�ֻ����ַ		0xE58760
#define Ѱ·��ַ				0xE50764
#define Ѱ·CALL1           0x621B70
#define Ѱ·CALL2           0x623580
#define ���촰�ڻ�ַ         0xE50BA8
#define ���촰��ƫ��			0x48
#define �������ƫ��			0x3C0
#define UI��MenuIdƫ��		0x38C
#define ���ﲹ��״̬CALL		0xA0FCA0
#define ���ﲹ��״̬CALL		0xA10A70
#define �������ƫ��			0x314
#define ���������ַ			0xE54788
#define ���＼�ܻ�ַ			0xE55430
#define Npc����ƫ��			0x3A8
#define ��Χ���ж����ַ		0xE54630
#define ��Χ���ж���ƫ��		0x1F8


#define ����ƫ��				0x68
#define Npc����CALL			0xEC
#define NpcIDCALL			0xE8
#define Npc����X				0x1FC
#define UI_Nameƫ��			0x94
#define UI���ƫ��			0x13C
#define �������ƫ��			0x98
#define Npc����ƫ��			0x28
#define Player����ƫ��		0x170
#define �Ƿ����ƫ��			0x38
#define �Ƿ����ƫ��			0x58
#define ���＼��ƫ��			0x24
#endif // _PRIVATESERVER





#define SHAREMEMORY_MAPPING_NAME	L"AskTao_ShareMemory_Name"
#define MAX_PLAYERCOUNT_SHAREMEMORY 10
#define CONSOLE_NAME				L"calc.exe"
#define RC4_KEY						"9c7ffc796d0404c6de069f44b0f7df"
#define SHAREMEMORY_FILE_NAME		L"gdsdkq387420489a536870912zbldz.tmp"

#define SKILLID_���Ƽ���		0x134

enum em_Script_Status
{
	em_Script_Status_None,
	em_Script_Status_Running,
	em_Script_Status_Stop
};


enum class ChannelType : DWORD
{
	ҥ�� = 0xA,
	���� = 0xB,
	ϵͳ = 0xFFFFFFFF,
	���� = 0x5
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
