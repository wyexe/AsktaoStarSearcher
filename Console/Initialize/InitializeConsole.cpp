#include "InitializeConsole.h"
#include <iostream>
#include <filesystem>
#include <InjectorLib/DllInjector/DllInjector.h>
#include <FileLib/File.h>
#include <CharacterLib/Character.h>
#include <ProcessLib/Common/ResHandleManager.h>
#include <ProcessLib/Process/Process.h>
#include <AlgroithmLib/Encrypt/CRC32.h>
#include <LogLib/Log.h>
#include <Download/ConsoleDownloader.h>
#include <Asktao/Feature/Socket/Client/IocpClient.h>

#ifdef _DEBUG
#pragma comment(lib,"AlgroithmLib_Debug.lib")
#else
#pragma comment(lib,"AlgroithmLib.lib")
#endif // _DEBUG


#define _SELF L"InitializeConsole.cpp"
BOOL CInitializeConsole::InitializeConsole()
{
	if (::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"AKT_Console") != NULL)
	{
		std::wcout << L"����̨�Ѿ��������ˡ����������ж��!" << std::endl;
		return FALSE;
	}


	::CreateMutexW(NULL, FALSE, L"AKT_Console");
	if (libTools::CProcess::IsExistProcName(L"asktao_jd.mod") || libTools::CProcess::IsExistProcName(L"asktao.mod"))
	{
		std::wcout << L"�����ȹر���Ϸ!" << std::endl;
		return FALSE;
	}



	// Read Game Directory in Config.ini
	return InitializeShareMemory() && InitializeConfig();
}

BOOL CInitializeConsole::InitializeGame()
{
	if (!CIocpClient::GetInstance().Run(libTools::CFile::DirectoryExist(L"Z:\\TEMP\\") ? DEBUG_SERVER_IP : RELEASE_SERVER_IP, SERVER_PORT, 5 * 1000))
	{
		std::wcout << L"���ӷ�����ʧ��!" << std::endl;
		system("pause");
		return 0;
	}

	std::wcout << L"���ڼ�����..." << std::endl;
	if (!DownloadPreviewFile())
	{
		std::wcout << L"����ʧ��!" << std::endl;
		return FALSE;
	}


	if (!CompLocalFile())
	{
		std::wcout << L"���ڸ�����..." << std::endl;
		if (!DownloadHijackFile())
		{
			std::wcout << L"���ؽٳ��ļ�ʧ��!" << std::endl;
			return FALSE;
		}
	}



	std::wcout << L"���ڽٳ���Ϸ..." << std::endl;
	return CopyHijackFileToGame();
}

VOID CInitializeConsole::Free()
{
	libTools::CFile::ForceDeleteFile(GetShareMemoryFilePath());
}

BOOL CInitializeConsole::DownloadPreviewFile()
{
	return CConsoleDownloader::GetInstance().DownloadPreviewFile(_VecPreviewFile);
}

BOOL CInitializeConsole::CompLocalFile()
{
	for (auto& itm : _VecPreviewFile.Vec)
	{
		std::wstring wsFilePath = _wsGamePath + L"\\" + itm.wsFileName;
		if (!libTools::CFile::FileExist(wsFilePath))
			return FALSE;


		LPVOID lpFileContent = nullptr;
		SIZE_T uFileSize = 0;
		if (!libTools::CFile::ReadFileContent(wsFilePath, lpFileContent, uFileSize))
			return FALSE;


		SetResDeleter(lpFileContent, [](LPVOID p) { ::VirtualFree(p, 0, MEM_RELEASE); });
		if (libTools::CCRC32::GetCRC32(reinterpret_cast<LPCSTR>(lpFileContent), uFileSize) != itm.dwFileCRC)
			return FALSE;


	}
	return TRUE;
}

BOOL CInitializeConsole::DownloadHijackFile()
{
	return CConsoleDownloader::GetInstance().DownloadHijackFile(_HijackFile);
}

BOOL CInitializeConsole::CopyHijackFileToGame()
{
	for (auto& itm : _HijackFile.Vec)
	{
		std::wstring wsFilePath = _wsGamePath + L"\\" + itm.wsFileName;
		//std::wcout << L"����д��:" << wsFilePath.c_str() << std::endl;
		if (!libTools::CFile::WriteFile(wsFilePath, reinterpret_cast<CONST BYTE*>(itm.FileContentPtr.get()), itm.uFileSize))
		{
			std::wcout << L"д���ļ�:[" << wsFilePath.c_str() << L"] ʧ��!" << std::endl;
			return FALSE;
		}
		//::MessageBoxW(NULL, wsFilePath.c_str(), L"", NULL);
	}

	return TRUE;
}

BOOL CInitializeConsole::InitializeConfig()
{
	WCHAR wszConfigValue[MAX_PATH] = { 0 };
	std::wstring wsConfigPath = libTools::CCharacter::MakeCurrentPath(L"\\Config.ini");
	
#ifdef _DEBUG
	wsConfigPath = LR"(D:\Visual Studio 2010\Projects\Asktao\Release\Config.ini)";
#endif // _DEBUG
	if (!::GetPrivateProfileStringW(L"Config", L"GamePath", L"", wszConfigValue, MAX_PATH, wsConfigPath.c_str()) || wszConfigValue[0] == L'\0')
	{
		std::wcout << L"��ȡ��Ϸ·��ʧ��!" << std::endl;
		return FALSE;
	}


	_wsGamePath = wszConfigValue;
	if (!libTools::CFile::FileExist(_wsGamePath + L"\\asktao_jd.mod"))
	{
		std::wcout << L"��Ϸ·����Ч!" << std::endl;
		return FALSE;
	}

	if (!::GetPrivateProfileStringW(L"Config", L"ComputerName", L"", g_pShareMemoryContent->wszComputerName, MAX_PATH, wsConfigPath.c_str()) || g_pShareMemoryContent->wszComputerName[0] == L'\0')
	{
		std::wcout << L"��ȡ�������ʧ��!" << std::endl;
		return FALSE;
	}


	ClearGameLog();
	return InitializeAccount();

}

BOOL CInitializeConsole::InitializeAccount()
{
	WCHAR wszConfigValue[MAX_PATH] = { 0 };
	for (int i = 1;i < _countof(g_pShareMemoryContent->Account); ++i)
	{
		if (!::GetPrivateProfileStringW(L"SearchStar", libTools::CCharacter::MakeFormatText(L"Account%d", i).c_str(), L"", wszConfigValue, MAX_PATH, libTools::CCharacter::MakeCurrentPath(L"\\Config.ini").c_str()) || wszConfigValue[0] == L'\0')
			break;


		auto& Account = g_pShareMemoryContent->Account[i - 1];



		std::vector<std::wstring> Vec;
		libTools::CCharacter::SplitFormatText(std::wstring(wszConfigValue), L"[*,*]", Vec);
		if (Vec.size() != 2)
		{
			LOG_MSG_CF(L"��Ч���˺�����:%s", wszConfigValue);
			return FALSE;
		}


		auto wsScriptType = Vec.at(0);
		if (wsScriptType == L"����")
		{
			Vec.clear();
			// ����,��ɫ��,�ǳ�,LineName
			libTools::CCharacter::SplitFormatText(std::wstring(wszConfigValue), L"[*,*,*,*]", Vec);
			if (Vec.size() != 4)
			{
				LOG_MSG_CF(L"��Ч���˺�����:%s", wszConfigValue);
				return FALSE;
			}


			Account.emScriptType = em_Script_Type::SearchStar;
			libTools::CCharacter::strcpy_my(Account.wszPlayerName, Vec.at(1).c_str());
			libTools::CCharacter::strcpy_my(Account.wszAliasName, Vec.at(2).c_str());
			libTools::CCharacter::strcpy_my(Account.Config.StarConfig.wszLineName, Vec.at(3).c_str());
		}
		else if (wsScriptType == L"ҥ��")
		{
			Vec.clear();
			// ҥ��,��ɫ��,�ǳ�
			libTools::CCharacter::SplitFormatText(std::wstring(wszConfigValue), L"[*,*,*,*]", Vec);
			if (Vec.size() != 4)
			{
				LOG_MSG_CF(L"��Ч���˺�����:%s", wszConfigValue);
				return FALSE;
			}


			Account.emScriptType = em_Script_Type::SearchChatStar;
			libTools::CCharacter::strcpy_my(Account.wszPlayerName, Vec.at(1).c_str());
			libTools::CCharacter::strcpy_my(Account.wszAliasName, Vec.at(2).c_str());
			Account.Config.RumorCfg.dwIngoreSec = std::stoul(Vec.at(3));
		}
		else if (wsScriptType == L"����")
		{
			Vec.clear();
			// ����,��ɫ��
			libTools::CCharacter::SplitFormatText(std::wstring(wszConfigValue), L"[*,*]", Vec);
			if (Vec.size() != 2)
			{
				LOG_MSG_CF(L"��Ч���˺�����:%s", wszConfigValue);
				return FALSE;
			}

			Account.emScriptType = em_Script_Type::TESTFORZEN;
			libTools::CCharacter::strcpy_my(Account.wszPlayerName, Vec.at(1).c_str());
		}
		else
		{
			LOG_MSG_CF(L"��Ч�Ľű�����:[%s]", wsScriptType.c_str());
			return FALSE;
		}

		g_pShareMemoryContent->uAccountCount += 1;
	}
	return TRUE;
}

BOOL CInitializeConsole::InitializeShareMemory()
{
	// �����ļ�ӳ��
	HANDLE hFile = ::CreateFile(GetShareMemoryFilePath().c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM, NULL);
	if (hFile == NULL)
	{
		std::wcout << L"������ʱ�ļ�ʧ��!" << std::endl;
		return FALSE;
	}


	HANDLE hFileMap = ::CreateFileMappingW(hFile, NULL, PAGE_READWRITE, NULL, sizeof(ShareMemoryContent), ASKTAO_SHAREMEMORY_NAME);
	if (hFileMap == NULL)
	{
		std::wcout << L"�����ļ�ӳ��ʧ��!" << std::endl;
		return FALSE;
	}


	g_pShareMemoryContent = reinterpret_cast<ShareMemoryContent *>(::MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_SHARE_WRITE, NULL, NULL, sizeof(ShareMemoryContent)));
	if (g_pShareMemoryContent == nullptr)
	{
		std::wcout << L"�ڴ�ӳ��ʧ��!!" << std::endl;
		return FALSE;
	}


	ZeroMemory(g_pShareMemoryContent, sizeof(ShareMemoryContent));
	::GetCurrentDirectoryW(MAX_PATH, g_pShareMemoryContent->wszConsolePath);
	return TRUE;
}

VOID CInitializeConsole::ClearGameLog() CONST
{
	using DirectoryPath = std::experimental::filesystem::v1::path;
	using DirectoryItr = std::experimental::filesystem::v1::directory_iterator;



	DirectoryPath DirPath(_wsGamePath.back() == L'\\' ? (_wsGamePath + L"Log\\") : (_wsGamePath + L"\\Log\\"));
	for (CONST auto& File : DirectoryItr(DirPath))
	{
		libTools::CFile::ForceDeleteFile(File.path().wstring());
	}
}

std::wstring CInitializeConsole::GetShareMemoryFilePath() CONST
{
	WCHAR wszPath[MAX_PATH] = { 0 };
	::GetTempPathW(MAX_PATH, wszPath);


	if (wszPath[wcslen(wszPath) - 1] != L'\\')
		::lstrcatW(wszPath, L"\\");


	return std::wstring(wszPath) + SHAREMEMORY_FILE_NAME;
}
