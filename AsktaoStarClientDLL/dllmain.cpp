#include <Windows.h>
#include <queue>
#include <filesystem>
#include <Asktao/Feature/Socket/Client/IocpClient.h>
#include <Asktao/Feature/Common/ServerCommon.h>
#include <SocketCommon/SocketBuffer.h>
#include <LogLib/Log.h>
#include <FileLib/File.h>
#include <CharacterLib/Character.h>

#define _SELF L"dllmain.cpp"
extern "C" __declspec(dllexport) int GetStarInfo(_Out_ LPWSTR pwszText)
{
	int Level = 0;

	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Download_StarInfo);
	BOOL bRetCode = CIocpClient::GetInstance().SyncSend(SocketBuffer, [&](libTools::CSocketBuffer& Buffer)
	{
		BOOL bExist = FALSE;
		Buffer >> bExist;
		if (!bExist)
		{
			wsprintfW(pwszText, L"0");
		}
		else
		{
			StarInfo_Tranfer StartInfo = Buffer.GetStruct<StarInfo_Tranfer>();
			Level = static_cast<int>(StartInfo.dwLevel) / 10 * 10;
			libTools::CCharacter::strcpy_my(pwszText, StartInfo.wsShowContent.c_str());
		}
	});


	if (!bRetCode)
		return -1;


	return Level;
}

extern "C" __declspec(dllexport) int GetRumor(_Out_ LPWSTR RumorText)
{
	BOOL IsExistRumor = FALSE;

	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Download_Rumor);
	BOOL bRetCode = CIocpClient::GetInstance().SyncSend(SocketBuffer, [&RumorText, &IsExistRumor](libTools::CSocketBuffer& Buffer)
	{
		Buffer >> IsExistRumor;
		if (IsExistRumor)
		{
			std::wstring wsText;
			Buffer >> wsText;
			libTools::CCharacter::strcpy_my(RumorText, wsText.c_str());
			LOG_C_D(L"GetRumor:[%s]", wsText.c_str());
		}
	});


	if (!bRetCode)
		return -1;


	return IsExistRumor;
}

std::queue<ClientKeepALive_Tranfer> VecKeepALive;
extern "C" __declspec(dllexport) int GetAccountKeepALive(_In_ int IsRumor)
{
	libTools::CSocketBuffer SocketBuffer(IsRumor ? em_Sock_Msg_Download_KeepALive_Runmor : em_Sock_Msg_Download_KeepALive);
	return CIocpClient::GetInstance().SyncSend(SocketBuffer, [](libTools::CSocketBuffer& Buffer)
	{
		UINT uSize = 0;
		Buffer >> uSize;
		for (UINT i = 0; i < uSize; ++i)
		{
			VecKeepALive.push(Buffer.GetStruct<ClientKeepALive_Tranfer>());
		}
	});
}


extern "C" __declspec(dllexport) int GetAccountKeepALiveContent(_Out_ LPWSTR pwszText)
{
	if (VecKeepALive.empty())
		return FALSE;


	auto itm = VecKeepALive.front();
	VecKeepALive.pop();

	switch (itm.emType)
	{
	case em_KeepALive_Type::AnswerQuestion:
		wsprintfW(pwszText, L"老君,%s,Empty,Empty", itm.wsPlayerName.c_str());
		break;
	case em_KeepALive_Type::GameClient:
		wsprintfW(pwszText, L"正常,%s,%s,%s", itm.wsPlayerName.c_str(), itm.wsComputerName.c_str(), itm.wsLocation.c_str());
		break;
	case em_KeepALive_Type::GameLog:
		wsprintfW(pwszText, L"日志,%s,%s,%s", itm.wsPlayerName.c_str(), itm.wsComputerName.c_str(), itm.wsLogContent.c_str());
		break;
	default:
		break;
	}
	
	return TRUE;
}

DWORD WINAPI _InititlizeThread(LPVOID)
{
	for (;;)
	{
		libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Console_KeepALive);
		CIocpClient::GetInstance().SyncSend(SocketBuffer, [](libTools::CSocketBuffer&) {});
		::Sleep(3 * 1000);
	}
	return 0;
}

VOID InitializeLog()
{
	if (!libTools::CFile::DirectoryExist(libTools::CCharacter::MakeCurrentPath(L"\\Log\\")))
	{
		libTools::CFile::CreateMyDirectory(libTools::CCharacter::MakeCurrentPath(L"\\Log\\").c_str(), TRUE);
	}


	WCHAR wszFilePath[MAX_PATH] = { 0 };
	::GetModuleFileNameW(NULL, wszFilePath, MAX_PATH);
	libTools::CFile::ForceDeleteFile(wszFilePath);


	auto wsFileName = std::experimental::filesystem::v1::path(wszFilePath).filename().wstring();
	wsFileName = wsFileName.substr(0, wsFileName.find_last_of(L"."));


	if (libTools::CFile::DirectoryExist(L"Z:\\TEMP\\"))
	{
		libTools::CLog::GetInstance().SetSyncSendLog();
		libTools::CLog::GetInstance().SetClientName(wsFileName.c_str(), libTools::CCharacter::MakeCurrentPath(L"\\Log\\"));
	}
}

extern "C" __declspec(dllexport) int Initialize()
{
	InitializeLog();
	if (!CIocpClient::GetInstance().Run(libTools::CFile::DirectoryExist(L"Z:\\TEMP\\") ? DEBUG_SERVER_IP : RELEASE_SERVER_IP, SERVER_PORT, 5000))
	{
		::MessageBoxW(NULL, L"连接服务器失败!", L"", NULL);
		return FALSE;
	}

	
	CIocpClient::GetInstance().SetDisConnectPtr([] 
	{
		::MessageBoxW(NULL, L"与我们的服务器断开连接了……联系老夫!", L"", NULL);
	});


	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_InititlizeThread, NULL, NULL, NULL);
	return TRUE;
}

BOOL WINAPI DllMain(_In_ HMODULE, _In_ DWORD dwReason, _In_ LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{

	}
	return TRUE;
}