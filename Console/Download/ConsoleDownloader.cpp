#include "ConsoleDownloader.h"
#include <Asktao/Feature/Socket/Client/IocpClient.h>

CConsoleDownloader& CConsoleDownloader::GetInstance()
{
	static CConsoleDownloader Instance;
	return Instance;
}

BOOL CConsoleDownloader::DownloadPreviewFile(_Out_ PreviewFile_Tranfer& Vec)
{
	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Download_PreviewFile);


	return CIocpClient::GetInstance().SyncSend(SocketBuffer, [&Vec](libTools::CSocketBuffer& Buffer)
	{
		Vec = Buffer.GetStruct<PreviewFile_Tranfer>();
	});
}

BOOL CConsoleDownloader::DownloadHijackFile(_Out_ DownloadFileContent& HijackFile)
{
	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Download_HijackFile);
	return CIocpClient::GetInstance().SyncSend(SocketBuffer, [&HijackFile](libTools::CSocketBuffer& Buffer)
	{
		HijackFile = Buffer.GetStruct<DownloadFileContent>();
	});
}

BOOL CConsoleDownloader::DownloadCheatFile(_Out_ DownloadFileContent& CheatFile)
{
	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Download_CheatFile);
	return CIocpClient::GetInstance().SyncSend(SocketBuffer, [&CheatFile](libTools::CSocketBuffer& Buffer)
	{
		CheatFile = Buffer.GetStruct<DownloadFileContent>();
	});
}
