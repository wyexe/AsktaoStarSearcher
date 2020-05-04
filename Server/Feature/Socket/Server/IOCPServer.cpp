#include "IOCPServer.h"
#include <Asktao/Feature/Common/ServerCommon.h>
#include <LogLib/Log.h>
#include <SocketCommon/SocketRemoteClient.h>
#include <Server/Feature/FileCache/FileCache.h>
#include <Server/Feature/DB/DbManager.h>
#include <Server/Feature/Socket/Echo/EchoPacket.h>

#pragma comment(lib,"SocketServerLib.lib")

#define _SELF L"IOCPServer.cpp"
CIOCPServer::CIOCPServer()
{

}
libTools::CSocketRemoteClient* CIOCPServer::CreateNewSocketClient(_In_ UINT_PTR ClientSock)
{
	return new libTools::CSocketRemoteClient(ClientSock);
}

VOID CIOCPServer::ReleaseSocketClient(_In_ libTools::CSocketRemoteClient* pSocketClient)
{
	pSocketClient->DisConnect();
	pSocketClient->Remove();
	delete dynamic_cast<libTools::CSocketRemoteClient *>(pSocketClient);
	pSocketClient = nullptr;
}

BOOL CIOCPServer::EchoRecive(_In_ libTools::CSocketRemoteClient* pSocketClient, _In_ libTools::CSocketBuffer* pSocketBuffer)
{
	em_Sock_Msg emSockMsg;
	if (pSocketBuffer->VerDataPtr(emSockMsg) != libTools::CSocketBuffer::em_SocketBuffer_Error_None)
	{
		LOG_C_E(L"VerDataPtr = FALSE!");
		return FALSE;
	}



	switch (emSockMsg)
	{
	case em_Sock_Msg_Game_KeepALive:
	case em_Sock_Msg_Console_KeepALive:
		CEchoPacket::KeepALive(pSocketClient, pSocketBuffer);
		break;
	case em_Sock_Msg_Download_StarMapPoint:
		CEchoPacket::DownloadStarMapPoint(pSocketClient, pSocketBuffer);
		break;
	case em_Sock_Msg_Upload_StarInfo:
		CEchoPacket::UploadStarInfo(pSocketClient, pSocketBuffer);
	break; 
	case em_Sock_Msg_Download_PreviewFile:
	case em_Sock_Msg_Download_GameFile:
	case em_Sock_Msg_Download_HijackFile:
	case em_Sock_Msg_Download_CheatFile:
	case em_Sock_Msg_Download_Console:
	case em_Sock_Msg_Download_ConsoleCRC:
		CEchoPacket::DownloadFile(pSocketClient, pSocketBuffer);
		break;
	case em_Sock_Msg_Download_StarInfo:
		CEchoPacket::DownloadStarInfo(pSocketClient, pSocketBuffer);
		break;
	case em_Sock_Msg_Download_KeepALive:
		CEchoPacket::DownloadKeepALive(pSocketClient, pSocketBuffer);
		break;
	case em_Sock_Msg_ReleaseTask_SearchStar:
		CEchoPacket::ReleaseSearchStarTask(pSocketClient, pSocketBuffer);
		break;
	case em_Sock_Msg_Download_Rumor:
		CEchoPacket::DownloadRumor(pSocketClient, pSocketBuffer);
		break;
	case em_Sock_Msg_AcceptTask_SearchStar:
		CEchoPacket::AcceptSearchStarTask(pSocketClient, pSocketBuffer);
		break;
	default:
		break;
	}


	return PostSend(pSocketClient, pSocketBuffer);
}

BOOL CIOCPServer::Run(_In_ SHORT shPort, _In_ UINT uMaxAccept)
{
	CDbManager::GetInstance().Initialize();
	return CFileCache::GetInstance().Initialize() && RunServer(shPort, uMaxAccept);
}

VOID CIOCPServer::Stop()
{
	StopServer();
}