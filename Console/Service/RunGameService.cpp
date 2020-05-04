#include "RunGameService.h"
#include <iostream>
#include <future>
#include <Download/ConsoleDownloader.h>
#include <Asktao/Feature/Socket/Client/IocpClient.h>
#include <ProcessLib/Process/Process.h>
#include <FileLib/File.h>
#include <CharacterLib/Character.h>
#include <AlgroithmLib/Encrypt/RC4.h>

BOOL CRunGameService::Run()
{
	std::wcout << L"准备下载主文件..." << std::endl;
	if (!DownloadGameCheatFile())
	{
		std::wcout << L"下载失败..." << std::endl;
		return FALSE;
	}


	//CIocpClient::GetInstance().Stop();
	//auto KeepAliveThreadPtr = std::async(std::launch::async, &CRunGameService::KeepAliveThread, this);
	for(int i = 0;i < 1000; ++i)
		std::wcout << L'\b';


	std::wcout << L"下载完毕!";
	return TRUE;
}


BOOL CRunGameService::DownloadGameCheatFile()
{
	if (libTools::CFile::DirectoryExist(L"Z:\\TEMP\\"))
	{
		std::wcout << L"Local File!" << std::endl;
		LPVOID lpFileContent = nullptr;
		SIZE_T uFileSize = 0;
		if (libTools::CFile::ReadFileContent(libTools::CCharacter::MakeCurrentPath(L"\\asktao.dll"), lpFileContent, uFileSize))
		{
			memcpy(g_pShareMemoryContent->CheatFileContentPtr, libTools::CRC4(RC4_KEY, strlen(RC4_KEY)).GetEncryptText(static_cast<CONST CHAR*>(lpFileContent), uFileSize).get(), uFileSize);
			g_pShareMemoryContent->uFileSize = uFileSize;
		}
	}
	else
	{
		DownloadFileContent VecCheatFile;
		if (!CConsoleDownloader::GetInstance().DownloadCheatFile(VecCheatFile) || VecCheatFile.Vec.size() == 0)
			return FALSE;


		g_pShareMemoryContent->uFileSize = VecCheatFile.Vec.at(0).uFileSize;
		memcpy(g_pShareMemoryContent->CheatFileContentPtr, VecCheatFile.Vec.at(0).FileContentPtr.get(), g_pShareMemoryContent->uFileSize);
	}


	return TRUE;
}

VOID CRunGameService::KeepAliveThread()
{
	CIocpClient::GetInstance().SetTimeout(10);
	for (;;)
	{
		for (UINT i = 0;i < g_pShareMemoryContent->uAccountCount; ++i)
		{
			if(!g_pShareMemoryContent->Account[i].bLogin)
				continue;


			g_pShareMemoryContent->Account[i].bLive = FALSE;
		}

		for (int i = 0;i < 2; ++i)
		{
			::Sleep(3000);
			if (!SendConsoleKeepALive())
			{
				std::wcout << L"与服务器失去连接!" << std::endl;
				break;
			}
		}

	}
}

BOOL CRunGameService::SendConsoleKeepALive() CONST
{
	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Console_KeepALive);
	return CIocpClient::GetInstance().SyncSend(SocketBuffer, [](libTools::CSocketBuffer&) {});
}

BOOL CRunGameService::SendLoseConnectKeepALive(_In_ CONST ClientKeepALive_Tranfer& Content) CONST
{
	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Game_KeepALive);
	SocketBuffer.AddStruct(Content);
	return CIocpClient::GetInstance().SyncSend(SocketBuffer, [](libTools::CSocketBuffer&) {});
}
