#include <Windows.h>
#include <iostream>
#include <CharacterLib/Character.h>
#include <LogLib/Log.h>
#include <Server/Feature/Socket/Server/IOCPServer.h>
#include <Asktao/Feature/Common/ServerCommon.h>
#include <Server/Feature/FileCache/FileCache.h>


#pragma comment(lib,"DbManagerLib.lib")
#pragma comment(lib,"LogLib.lib")
#pragma comment(lib,"SocketCommonLib.lib")
#pragma comment(lib,"AlgroithmLib.lib")

int main()
{
	libTools::CCharacter::SetConsoleLanguage();
	libTools::CCharacter::SetSpecialCharacterMode();
	libTools::CLog::GetInstance().SetClientName(L"AskServer", libTools::CCharacter::MakeCurrentPath(L"\\Log\\"));


	CIOCPServer IocpServer;
	if (!IocpServer.Run(SERVER_PORT, 100))
	{
		std::wcout << L"IOCP.Run = FALSE" << std::endl;
		system("pause");
	}


	while (true)
	{
		std::wcout << L"Cmd:(RefreshFileCache)";

		std::wstring wsLine;
		std::getline(std::wcin, wsLine);
		if (wsLine == L"RefreshFileCache")
		{
			CFileCache::GetInstance().Initialize();
		}
		else
		{
			std::wcout << L"ÎÞÐ§µÄÃüÁî..." << std::endl;
		}
	}
	::Sleep(INFINITE);
	return 0;
}