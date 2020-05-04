#include "SocketAPI.h"
#include <Feature/Socket/Client/IocpClient.h>
#include <LogLib/Log.h>
#include <FileLib/File.h>

#define _SELF L"SocketAPI.cpp"
CSocketAPI& CSocketAPI::GetInstance()
{
	static CSocketAPI Instance;
	return Instance;
}

BOOL CSocketAPI::DownloadMapStarPoint(_In_ CONST std::wstring& wsMapName, _Out_ StarMap_Tranfer& Content) CONST
{
	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Download_StarMapPoint);
	SocketBuffer << wsMapName;
	LOG_CF_D(L"DownloadMapStarPoint.MapName=[%s]", wsMapName.c_str());

	return CIocpClient::GetInstance().SyncSend(SocketBuffer, [&Content](libTools::CSocketBuffer& Buffer)
	{
		Content = Buffer.GetStruct<StarMap_Tranfer>();
	});
}

BOOL CSocketAPI::UploadStarInfo(_In_ CONST StarInfo_Tranfer& StarInfo) CONST
{
	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Upload_StarInfo);
	SocketBuffer.AddStruct(StarInfo);


	return CIocpClient::GetInstance().SyncSend(SocketBuffer, [](libTools::CSocketBuffer&) {});
}

BOOL CSocketAPI::SendKeepALive(_In_ CONST ClientKeepALive_Tranfer& Content) CONST
{
	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Game_KeepALive);
	SocketBuffer.AddStruct(Content);


	return CIocpClient::GetInstance().SyncSend(SocketBuffer, [](libTools::CSocketBuffer&) {});
}

BOOL CSocketAPI::SendConsoleKeepALive() CONST
{
	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Console_KeepALive);
	return CIocpClient::GetInstance().SyncSend(SocketBuffer, [](libTools::CSocketBuffer&) {});
}

BOOL CSocketAPI::Run() CONST
{
	CIocpClient::GetInstance().SetDisConnectPtr([] { LOG_MSG_CF(L"呃……游戏与我们服务器断开连接了~ 准备X掉游戏了!"); ::TerminateProcess(::GetCurrentProcess(), 0); });
	return CIocpClient::GetInstance().Run(libTools::CFile::DirectoryExist(L"Z:\\TEMP\\") ? DEBUG_SERVER_IP : RELEASE_SERVER_IP, SERVER_PORT, 5 * 1000);
}

BOOL CSocketAPI::SendAcceptTask_SearchStar(_In_ CONST std::wstring& wsMapName, _In_ CONST std::wstring& wsPlayerName,  _Out_ MatchStar_Tranfer& MatchStar) CONST
{
	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_AcceptTask_SearchStar);
	SocketBuffer << wsMapName << wsPlayerName;


	return CIocpClient::GetInstance().SyncSend(SocketBuffer, [&MatchStar](libTools::CSocketBuffer& Buffer)
	{
		MatchStar = Buffer.GetStruct<MatchStar_Tranfer>();
	});
}

BOOL CSocketAPI::SendReleaseSearchStarTask(_In_ CONST ReleaseSearchStarTaskContent_Tranfer& Content) CONST
{
	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_ReleaseTask_SearchStar);
	SocketBuffer.AddStruct(Content);


	return CIocpClient::GetInstance().SyncSend(SocketBuffer, [](libTools::CSocketBuffer&) {});
}
