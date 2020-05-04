#ifndef __ASKTAO_ASKTAO_FEATURE_CORE_SOCKET_API_SOCKETAPI_H__
#define __ASKTAO_ASKTAO_FEATURE_CORE_SOCKET_API_SOCKETAPI_H__


#include <Feature/Common/ServerCommon.h>

class CSocketAPI
{
public:
	CSocketAPI() = default;
	~CSocketAPI() = default;

	//
	static CSocketAPI& GetInstance();
public:
	// 
	BOOL DownloadMapStarPoint(_In_ CONST std::wstring& wsMapName, _Out_ StarMap_Tranfer& Content) CONST;

	//
	BOOL UploadStarInfo(_In_ CONST StarInfo_Tranfer& StarInfo) CONST;

	//
	BOOL SendKeepALive(_In_ CONST ClientKeepALive_Tranfer& Content) CONST;

	//
	BOOL SendConsoleKeepALive() CONST;

	//
	BOOL Run() CONST;

	//
	BOOL SendAcceptTask_SearchStar(_In_ CONST std::wstring& wsMapName, _In_ CONST std::wstring& wsPlayerName,  _Out_ MatchStar_Tranfer& MatchStar) CONST;

	//
	BOOL SendReleaseSearchStarTask(_In_ CONST ReleaseSearchStarTaskContent_Tranfer& Content) CONST;
};



#endif // !__ASKTAO_ASKTAO_FEATURE_CORE_SOCKET_API_SOCKETAPI_H__
