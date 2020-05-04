#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <CharacterLib/Character.h>
#include <Asktao/Feature/Socket/Client/IOCPClient.h>
#include <InjectorLib/DllInjector/DllInjector.h>
#include <FileLib/File.h>
#include <AlgroithmLib/Encrypt/CRC32.h>
#include <ProcessLib/Common/ResHandleManager.h>
#include <ProcessLib/Process/Process.h>

#pragma comment(lib,"AlgroithmLib.lib")

DWORD DownloadConsoleCRC()
{
	DWORD dwFileCRC = 0;

	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Download_ConsoleCRC);
	CIocpClient::GetInstance().SyncSend(SocketBuffer, [&dwFileCRC](libTools::CSocketBuffer& Buffer)
	{
		PreviewFile_Tranfer Vec = Buffer.GetStruct<PreviewFile_Tranfer>();
		auto itr = std::find_if(Vec.Vec.begin(), Vec.Vec.end(), [](CONST PreviewFile& itm) { return itm.wsFileName == CONSOLE_NAME; });
		if (itr != Vec.Vec.end())
		{
			dwFileCRC = itr->dwFileCRC;
		}
	});
	return dwFileCRC;
}


BOOL CompLocalFile(_In_ DWORD dwCRC)
{
	std::wstring wsPath = libTools::CCharacter::MakeCurrentPath(L"\\calc.exe");
	if (!libTools::CFile::FileExist(wsPath))
	{
		std::wcout << L"UnExist calc.exe" << std::endl;
		return FALSE;
	}


	LPVOID lpFileContentPtr = nullptr;
	SIZE_T uFileSize = 0;
	if (!libTools::CFile::ReadFileContent(wsPath, lpFileContentPtr, uFileSize))
	{
		std::wcout << L"Read Console.exe Faild!" << std::endl;
		return FALSE;
	}


	SetResDeleter(lpFileContentPtr, [](LPVOID& p) { ::VirtualFree(p, 0, MEM_RELEASE); });
	std::wcout << L"dwCRC32 = [" << std::hex << dwCRC << L"], == [" << libTools::CCRC32::GetCRC32(reinterpret_cast<LPCSTR>(lpFileContentPtr), uFileSize) << std::hex << L"]" << std::endl;
	return dwCRC == libTools::CCRC32::GetCRC32(reinterpret_cast<LPCSTR>(lpFileContentPtr), uFileSize);
}

BOOL DownloadConsole()
{
	DownloadFileContent Content;


	libTools::CSocketBuffer SocketBuffer(em_Sock_Msg_Download_Console);
	BOOL bRetCode = CIocpClient::GetInstance().SyncSend(SocketBuffer, [&Content](libTools::CSocketBuffer& Buffer)
	{
		Content = Buffer.GetStruct<DownloadFileContent>();
	});


	if (!bRetCode)
		return FALSE;


	libTools::CFile::ForceDeleteFile(libTools::CCharacter::MakeCurrentPath(L"\\calc.exe"));
	return libTools::CFile::WriteFile(libTools::CCharacter::MakeCurrentPath(L"\\calc.exe"), reinterpret_cast<CONST BYTE*>(Content.Vec.at(0).FileContentPtr.get()), Content.Vec.at(0).uFileSize);
}

VOID RunConsole()
{
	libTools::CProcess::CreateProc(libTools::CCharacter::MakeCurrentPath(L"\\calc.exe"),FALSE, L"", FALSE);
}

int main()
{
	libTools::CCharacter::SetConsoleLanguage();
	libTools::CCharacter::SetSpecialCharacterMode();
	if (!libTools::CDllInjector::IsRunAsAdministrator())
	{
		std::wcout << L"需要管理员权限!" << std::endl;
		system("pause");
		return 0;
	}

	if (libTools::CProcess::IsExistProcName(CONSOLE_NAME))
	{
		std::wcout << L"控制台:'calc.exe' 已经在运行了!" << std::endl;
		system("pause");
		return 0;
	}


	std::wcout << L"连接服务器中..." << std::endl;
	CIocpClient::GetInstance().SetTimeout(2 * 60);
	if (!CIocpClient::GetInstance().Run(libTools::CFile::DirectoryExist(L"Z:\\TEMP\\") ? DEBUG_SERVER_IP : RELEASE_SERVER_IP, SERVER_PORT, 5 * 1000))
	{
		std::wcout << L"连接服务器失败!" << std::endl;
		system("pause");
		return 0;
	}


	std::wcout << L"正在检查更新..." << std::endl;


	// Download 'Console.exe'.CRC32
	DWORD dwConsoleCRC = DownloadConsoleCRC();

	// Comp LocalFile
	if (!CompLocalFile(dwConsoleCRC))
	{
		// Download 'Console.exe'
		std::wcout << L"正在下载新版本..." << std::endl;
		if (!DownloadConsole())
		{
			std::wcout << L"下载文件失败!" << std::endl;
			system("pause");
			return 0;
		}
	}


	std::wcout << L"正在启动控制台!" << std::endl;
	RunConsole();
	::Sleep(3 * 1000);
	return 0;
}