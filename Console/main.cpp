#include <Windows.h>
#include <iostream>
#include <CharacterLib/Character.h>
#include <Initialize/InitializeConsole.h>
#include <InjectorLib/DllInjector/DllInjector.h>
#include <Asktao/Feature/Socket/Client/IocpClient.h>
#include <Service/RunGameService.h>
#include <LogLib/Log.h>
#include <FileLib/File.h>
#include <Asktao/Feature/Common/GameStruct.h>

VOID InitializeLog()
{
	if (!libTools::CFile::DirectoryExist(libTools::CCharacter::MakeCurrentPath(L"\\Log\\")))
	{
		libTools::CFile::CreateMyDirectory(libTools::CCharacter::MakeCurrentPath(L"\\Log\\").c_str(), TRUE);
	}


	libTools::CLog::GetInstance().SetSyncSendLog();
	libTools::CFile::ForceDeleteFile(libTools::CCharacter::MakeCurrentPath(L"\\Log\\Console.log"));
	libTools::CLog::GetInstance().SetClientName(L"Console", libTools::CCharacter::MakeCurrentPath(L"\\Log\\"));
}


ShareMemoryContent* g_pShareMemoryContent = nullptr;

BOOL IsLoad()
{
	std::wstring wsLine;
	while (true)
	{
		std::wcout << L"���� 1 ��װ��  ���� 2 ж��!" << std::endl;
		std::getline(std::wcin, wsLine);
		if (wsLine == L"1")
			return TRUE;
		else if (wsLine == L"2")
			return FALSE;


		std::wcout << L"��Ч������..." << std::endl;
	}
	return FALSE;
}

VOID PrintExitMsg()
{
	std::wcout << L"���Թرոó�����!" << std::endl;
	::Sleep(INFINITE);
}

int main()
{
	libTools::CCharacter::SetConsoleLanguage();
	libTools::CCharacter::SetSpecialCharacterMode();

	if (!libTools::CDllInjector::IsRunAsAdministrator())
	{
		std::wcout << L"���������ڹ���ԱȨ����..." << std::endl;
		system("pause");
		return 0;
	}

	std::wcout << L"��ʼ����..." << std::endl;
	BOOL bChoice = IsLoad();


	CInitializeConsole Init;
	if (bChoice && Init.InitializeConsole() && Init.InitializeGame())
	{
		CRunGameService().Run();
	}
	else if (!bChoice)
	{
		Init.Free();
	}

	PrintExitMsg();
	/*InitializeLog();
	if (!CIocpClient::GetInstance().Run(libTools::CFile::DirectoryExist(L"Z:\\TEMP\\") ? DEBUG_SERVER_IP : RELEASE_SERVER_IP, SERVER_PORT, 5 * 1000))
	{
		std::wcout << L"���ӷ�����ʧ��!" << std::endl;
		system("pause");
		return 0;
	}


	if (!Init.InitializeGame())
	{
		system("pause");
		return 0;
	}


	CRunGameService Service;
	if (!Service.Run())
	{
		std::wcout << L"Run Faild!" << std::endl;
		system("pause");
		return 0;
	}*/
	return 0;
}