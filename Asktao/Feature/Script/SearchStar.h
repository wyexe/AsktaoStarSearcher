#ifndef __ASKTAO_ASKTAO_FEATURE_CORE_SCRIPT_SEARCHSTAR_H__
#define __ASKTAO_ASKTAO_FEATURE_CORE_SCRIPT_SEARCHSTAR_H__

#include <vector>
#include <future>
#include <Feature/Socket/API/SocketAPI.h>
#include <Feature/Common/GameStruct.h>
#include <TimeLib/TimeCharacter.h>

class CItemObject;
class CNpc;
class CSearchStarService
{
private:
	struct OldStarInfo
	{
		std::wstring wsNpcName;
		Point		 Pos;
		ULONGLONG	 ulTick;


		OldStarInfo() = default;
		OldStarInfo( _In_ CONST std::wstring& wsNpcName_, _In_ CONST Point& Pos_ ) : wsNpcName(wsNpcName_), Pos(Pos_)
		{
			ulTick = ::GetTickCount64();
		}
	};

	struct StarMapResPoint
	{
		std::wstring		wsLineName;
		Point				Pos1;
		Point				Pos2;
	};

	struct StarMapResMap
	{
		std::wstring					wsMapName;
		std::vector<StarMapResPoint>	Vec;
	};

	struct StarMsg
	{
		std::wstring wsNpcName;
		std::wstring wsMapName;
		Point		 Pos;
		std::wstring wsChannelName;

		void Clear()
		{
			wsNpcName.clear();
			wsMapName.clear();
			wsChannelName.clear();
		}
	};
public:
	CSearchStarService();
	~CSearchStarService();

	// 
	BOOL Run();
private:
	//
	BOOL SearchMapStar();

	// 
	BOOL SearchStar_In_ChatSystem();

	//
	BOOL DownloadMapPoint();

	//
	BOOL PlayerMove(_In_ CONST Point& Pos);

	//
	BOOL SearchStar();

	//
	VOID CheckExorcism() CONST;

	// 获取驱魔香任务的剩余时间
	DWORD GetRemainExorcismTaskMinute() CONST;

	//
	BOOL ExistExorcism(_Out_ CItemObject* pItemObject = nullptr) CONST;

	//
	BOOL Initialize();

	//
	BOOL SwitchChannel();

	//
	VOID MoveToPoint(_In_ CONST Point& Pos) CONST;

	//
	VOID RunSearchStarKeepALive();

	//
	BOOL IsExistLaoJun() CONST;

	//
	VOID UploadPlayerAttribute() CONST;

	//
	VOID AddColumnsPath(_In_ CONST std::vector<Point>& Vec);

	//
	VOID AddRowsPath(_In_ CONST std::vector<Point>& Vec);

	//
	VOID AddCircularPath(_In_ CONST std::vector<Point>& Vec);

	//
	VOID MoveToNpc(_In_ CONST Point& NpcPos) CONST;

	//
	VOID StopMove() CONST;

	//
	BOOL IsExistPlayer(_In_ CONST Point& Pos) CONST;

	//
	VOID WaitToReStart();

	// 
	BOOL GetVecStar_In_ChatSystem(_Out_ std::vector<StarChatMsgContent>& VecChatStar);

	//
	libTools::CTimeCharacter::TimeContent ConvertChatTick(_In_ CONST std::wstring& wsTickText) CONST;
private:
	std::vector<std::vector<Point>> _VecVecPoint;
	std::vector<OldStarInfo>	_VecOldStar;
	std::wstring				_wsPlayerName;
	AccountShareMemory*			_pAccountShareMemory;
	std::future<VOID>			_RunSearchStarKeepALivePtr;
	StarChatMsgContent			_LastStarChatMsgContent;
	BOOL						_bLive;
	std::wstring				_wsMapName;
};



#endif // !__ASKTAO_ASKTAO_FEATURE_CORE_SCRIPT_SEARCHSTAR_H__
