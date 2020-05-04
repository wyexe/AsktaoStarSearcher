#ifndef __ASKTAO_ASKTAO_FEATURE_COMMON_SERVERCOMMON_H__
#define __ASKTAO_ASKTAO_FEATURE_COMMON_SERVERCOMMON_H__

#include <memory>
#include <vector>
#include "GameStruct.h"
#include <SocketCommon/SocketBuffer.h>

enum em_Sock_Msg
{
	em_Sock_Msg_Error = 0x0,
	em_Sock_Msg_Game_KeepALive = 0x1,
	em_Sock_Msg_Download_StarMapPoint = 0x2,
	em_Sock_Msg_Upload_StarInfo = 0x3,
	em_Sock_Msg_Download_GameFile = 0x4,
	em_Sock_Msg_Console_KeepALive = 0x5,
	em_Sock_Msg_Download_PreviewFile = 0x6,
	em_Sock_Msg_Download_HijackFile = 0x7,
	em_Sock_Msg_Download_CheatFile = 0x8,
	em_Sock_Msg_Download_StarInfo = 0x9,
	em_Sock_Msg_Download_Console = 0xA,
	em_Sock_Msg_Download_ConsoleCRC = 0xB,
	em_Sock_Msg_Download_KeepALive = 0xC,
	em_Sock_Msg_AcceptTask_SearchStar = 0xD,
	em_Sock_Msg_ReleaseTask_SearchStar = 0xE,
	em_Sock_Msg_Download_Rumor = 0xF,
	em_Sock_Msg_Download_KeepALive_Runmor = 0x10,
};


#define RELEASE_SERVER_IP L"47.92.215.172"
//#define DEBUG_SERVER_IP L"120.79.171.219"
#define DEBUG_SERVER_IP L"47.92.215.172"
#define SERVER_PORT 7774

struct ReleaseSearchStarTaskContent_Tranfer : public libTools::BaseSocketBuffer<libTools::CSocketBuffer>
{	
	DWORD		 dwLevel;
	std::wstring wsChannel;
	std::wstring wsNpcName;
	std::wstring wsShowContent;
	std::wstring wsMapName;
	SYSTEMTIME   Tick;


	// 
	ULONGLONG	ulTick;
	virtual CONST ReleaseSearchStarTaskContent_Tranfer& operator >> (libTools::CSocketBuffer& SocketBuffer) CONST override
	{
		SocketBuffer << dwLevel << wsChannel << wsNpcName << wsShowContent << wsMapName << Tick;
		return *this;
	}


	virtual ReleaseSearchStarTaskContent_Tranfer& operator << (libTools::CSocketBuffer& SocketBuffer) override
	{
		SocketBuffer >> dwLevel >> wsChannel >> wsNpcName >> wsShowContent >> wsMapName >> Tick;
		return *this;
	}
};

enum class em_StarMap_Type : DWORD
{
	VecPoint,
	Rect
};

enum class em_StarType : DWORD
{
	Normal	= 0x1,
	Rumor	= 0x2
};

struct StarInfo_Tranfer : public libTools::BaseSocketBuffer<libTools::CSocketBuffer>
{
	em_StarType	 emStarType;
	std::wstring wsChannelText;
	DWORD		 dwLevel;
	std::wstring wsNpcName;
	std::wstring wsShowContent;

	virtual CONST StarInfo_Tranfer& operator >> (libTools::CSocketBuffer& SocketBuffer) CONST override
	{
		SocketBuffer << static_cast<DWORD>(emStarType) << wsChannelText << dwLevel << wsNpcName << wsShowContent;
		return *this;
	}

	virtual StarInfo_Tranfer& operator << (libTools::CSocketBuffer& SocketBuffer) override
	{
		SocketBuffer >> reinterpret_cast<DWORD&>(emStarType) >> wsChannelText >> dwLevel >> wsNpcName >> wsShowContent;
		return *this;
	}
};

struct MatchStar_Tranfer : public libTools::BaseSocketBuffer<libTools::CSocketBuffer>
{
	std::wstring wsChannel;
	std::wstring wsNpcName;
	SYSTEMTIME   Tick;


	virtual CONST MatchStar_Tranfer& operator >> (libTools::CSocketBuffer& SocketBuffer) CONST override
	{
		SocketBuffer << wsChannel << wsNpcName << Tick;
		return *this;
	}

	virtual MatchStar_Tranfer& operator << (libTools::CSocketBuffer& SocketBuffer) override
	{
		SocketBuffer >> wsChannel >> wsNpcName >> Tick;
		return *this;
	}

	VOID Clear()
	{
		wsChannel.clear();
		wsNpcName.clear();
		ZeroMemory(&Tick, sizeof(Tick));
	}
};


struct LordLaoJun_Tranfer
{
	std::wstring wsComputerName;
};


struct PlayerInfo_Tranfer : public libTools::BaseSocketBuffer<libTools::CSocketBuffer>
{
	std::wstring	wsPlayerName;
	std::wstring	wsMapName;
	Point			Pos;
	UINT			uSpecifyItemCount;


	virtual CONST PlayerInfo_Tranfer& operator >>(libTools::CSocketBuffer& SocketBuffer) CONST override
	{
		SocketBuffer << wsPlayerName << wsMapName << Pos.X << Pos.Y << uSpecifyItemCount;
		return *this;
	}

	virtual PlayerInfo_Tranfer& operator << (libTools::CSocketBuffer& SocketBuffer) override
	{
		SocketBuffer >> wsPlayerName >> wsMapName >> Pos.X >> Pos.Y >> uSpecifyItemCount;
		return *this;
	}
};

struct PreviewFile
{
	std::wstring	wsFileName;
	DWORD			dwFileCRC;
};

struct PreviewFile_Tranfer : public libTools::BaseSocketBuffer<libTools::CSocketBuffer>
{
	std::vector<PreviewFile> Vec;

	virtual CONST PreviewFile_Tranfer& operator >> (libTools::CSocketBuffer& SocketBuffer) CONST override
	{
		SocketBuffer << Vec.size();
		for (auto& itm : Vec)
			SocketBuffer << itm.wsFileName << itm.dwFileCRC;

		return *this;
	}

	virtual PreviewFile_Tranfer& operator << (libTools::CSocketBuffer& SocketBuffer) override
	{
		UINT uSize = 0;
		SocketBuffer >> uSize;
		for (UINT i = 0;i < uSize; ++i)
		{
			PreviewFile Pf;
			SocketBuffer >> Pf.wsFileName >> Pf.dwFileCRC;
			Vec.push_back(Pf);
		}
		return *this;
	}
};


struct SingleDownloadFile
{
	std::wstring			wsFileName;
	UINT					uFileSize;
	std::shared_ptr<CHAR>	FileContentPtr;
	DWORD					dwFileCRC;
};


struct DownloadFileContent : public libTools::BaseSocketBuffer<libTools::CSocketBuffer>
{
	std::vector<SingleDownloadFile> Vec;

	virtual CONST DownloadFileContent& operator >> (libTools::CSocketBuffer& SocketBuffer) CONST override
	{
		SocketBuffer << Vec.size();
		for (auto& itm : Vec)
		{
			SocketBuffer << itm.wsFileName << itm.uFileSize << itm.dwFileCRC;
			SocketBuffer.SetDataPtr(itm.FileContentPtr, itm.uFileSize);
		}
		return *this;
	}

	virtual DownloadFileContent& operator << (libTools::CSocketBuffer& SocketBuffer) override
	{
		UINT uSize = 0;
		SocketBuffer >> uSize;
		for (UINT i = 0;i < uSize; ++i)
		{
			SingleDownloadFile itm;
			SocketBuffer >> itm.wsFileName >> itm.uFileSize >> itm.dwFileCRC;
			itm.FileContentPtr = SocketBuffer.GetSizeBuffer(itm.uFileSize);


			Vec.push_back(itm);
		}
		return *this;
	}
};

struct StarMap_Tranfer : public libTools::BaseSocketBuffer<libTools::CSocketBuffer>
{
	em_StarMap_Type		emType;
	std::vector<std::vector<Point>>	VecVec;

	virtual CONST StarMap_Tranfer& operator >> (libTools::CSocketBuffer& SocketBuffer) CONST override
	{
		SocketBuffer << static_cast<DWORD>(emType) << VecVec.size();
		for (auto& Vec : VecVec)
		{
			SocketBuffer << Vec.size();
			for (auto& itm : Vec)
			{
				SocketBuffer << itm.X << itm.Y;
			}
		}

		return *this;
	}

	virtual StarMap_Tranfer& operator << (libTools::CSocketBuffer& SocketBuffer) override
	{
		UINT uVecVecSize = 0;
		SocketBuffer >> reinterpret_cast<DWORD&>(emType) >> uVecVecSize;
		for (UINT i = 0;i < uVecVecSize; ++i)
		{
			std::vector<Point> Vec;
			UINT uVecSize = 0;
			SocketBuffer >> uVecSize;
			for (UINT j = 0;j < uVecSize; ++j)
			{
				Point Pos;
				SocketBuffer >> Pos.X >> Pos.Y;
				Vec.push_back(Pos);
			}
			this->VecVec.push_back(Vec);
		}
		return *this;
	}
};

enum class em_KeepALive_Type : DWORD
{
	// 普通心跳
	Console,
	// 答题
	AnswerQuestion,
	// 游戏心跳
	GameClient,
	// 日志
	GameLog,
};

struct ClientKeepALive_Tranfer : public libTools::BaseSocketBuffer<libTools::CSocketBuffer>
{
	em_KeepALive_Type emType;

	// GameClient
	std::wstring wsPlayerName;
	std::wstring wsLocation;
	std::wstring wsComputerName;
	std::wstring wsLogContent;

	virtual CONST ClientKeepALive_Tranfer& operator >> (libTools::CSocketBuffer& SocketBuffer) CONST override
	{
		SocketBuffer << static_cast<DWORD>(emType);
		switch (emType)
		{
		case em_KeepALive_Type::Console:
			SocketBuffer << wsComputerName;
			break;
		case em_KeepALive_Type::AnswerQuestion:
			SocketBuffer << wsComputerName << wsPlayerName;
			break;
		case em_KeepALive_Type::GameClient:
			SocketBuffer << wsComputerName << wsPlayerName << wsLocation;
			break;
		case em_KeepALive_Type::GameLog:
			SocketBuffer << wsComputerName << wsPlayerName << wsLogContent;
			break;
		default:
			break;
		}
		return *this;
	}

	virtual ClientKeepALive_Tranfer& operator << (libTools::CSocketBuffer& SocketBuffer) override
	{
		SocketBuffer >> reinterpret_cast<DWORD&>(emType);
		switch (emType)
		{
		case em_KeepALive_Type::Console:
			SocketBuffer >> wsComputerName;
			break;
		case em_KeepALive_Type::AnswerQuestion:
			SocketBuffer >> wsComputerName >> wsPlayerName;
			break;
		case em_KeepALive_Type::GameClient:
			SocketBuffer >> wsComputerName >> wsPlayerName >> wsLocation;
			break;
		case em_KeepALive_Type::GameLog:
			SocketBuffer >> wsComputerName >> wsPlayerName >> wsLogContent;
		default:
			break;
		}
		return *this;
	}
};

#endif // !__ASKTAO_ASKTAO_FEATURE_COMMON_SERVERCOMMON_H__
