#include <InjectorLib/ModuleInjector/NoTraceModuleInjector.h>
#include <Windows.h>
#include <Asktao/Feature/Common/GameStruct.h>
#include <FileLib/File.h>
#include <AlgroithmLib/Encrypt/RC4.h>
#include <ProcessLib/Memory/SearchBinary.h>
#include <SystemEnvLib/SystemInfo.h>
#include <LogLib/Log.h>

#pragma comment(lib,"InjectorLib.lib")
#pragma comment(lib,"AlgroithmLib.lib")
#pragma comment(lib,"SystemEnvLib.lib")
#pragma comment(lib,"LogLib.lib")
#pragma comment(lib,"user32.lib")

#define _SELF L"dllmain.cpp"

DWORD dwKernelTranferCALL = 0;
DWORD dwHokkVirtualProtectAddr = 0;
__declspec(naked) void WINAPI NewNtProtectVirtualMemoryWin10()
{
	__asm
	{
		MOV EAX, 0x50;
		MOV EDX, dwKernelTranferCALL;
		CALL EDX;
		RETN 0x14;
	}
}


__declspec(naked) void WINAPI NewNtProtectVirtualMemoryWin7()
{
	__asm
	{
		MOV EAX, 0x4D;
		MOV ECX, dwKernelTranferCALL;
		JMP ECX;
	}
}

VOID InitializeHookVirtualProtect()
{
	auto emCurrentSystem = CSystemInfo::GetSystemVersion();
	if (emCurrentSystem == CSystemInfo::em_System_Version::Windows10)
	{
		dwKernelTranferCALL = reinterpret_cast<DWORD>(::GetProcAddress(::GetModuleHandleW(L"ntdll.dll"), "ZwProtectVirtualMemory"));
		dwKernelTranferCALL += 5;
		dwKernelTranferCALL = RD(dwKernelTranferCALL + 1);


		dwHokkVirtualProtectAddr = reinterpret_cast<DWORD>(::GetProcAddress(::GetModuleHandleW(L"KernelBase.dll"), "VirtualProtect"));
		if (dwHokkVirtualProtectAddr == 0)
		{
			::MessageBoxW(NULL, L"dwHokkVirtualProtectAddr = 0", L"", NULL);
			return;
		}


		dwHokkVirtualProtectAddr += (0x100D4BA4 - 0x100D4B80)/*CALL FF15*/;

		//LOG_MSG_CF(L"dwHookAddr=%X, dwFunc=%X, NewValue=%X", dwHookAddr, dwFuckAddr, reinterpret_cast<DWORD>(NewNtProtectVirtualMemory) - dwHookAddr - 5);
		DWORD dwOldProtect = 0;
		::VirtualProtect(reinterpret_cast<LPVOID>(dwHokkVirtualProtectAddr), 0x10, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	}
	else if (emCurrentSystem == CSystemInfo::em_System_Version::Windows7)
	{
		DWORD dwOldProtect = 0;
		DWORD dwHookAddr = reinterpret_cast<DWORD>(::GetProcAddress(::GetModuleHandleW(L"ntdll.dll"), "ZwProtectVirtualMemory"));
		::VirtualProtect(reinterpret_cast<LPVOID>(dwHookAddr), 0x10, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	}
	else
	{
		LOG_MSG_CF(L"不支持该系统!");
		ExitProcess(0);
	}
}

VOID HookVirtualProtect()
{
	auto emCurrentSystem = CSystemInfo::GetSystemVersion();
	if (emCurrentSystem == CSystemInfo::em_System_Version::Windows10)
	{
		DWORD dwFuckAddr = reinterpret_cast<DWORD>(NewNtProtectVirtualMemoryWin10) - dwHokkVirtualProtectAddr - 5;
		*reinterpret_cast<BYTE*>(dwHokkVirtualProtectAddr + 0) = 0xE8;
		*reinterpret_cast<DWORD*>(dwHokkVirtualProtectAddr + 1) = dwFuckAddr;
		*reinterpret_cast<BYTE*>(dwHokkVirtualProtectAddr + 5) = 0x90;
	}
	else if (emCurrentSystem == CSystemInfo::em_System_Version::Windows7)
	{
		DWORD dwHookAddr = reinterpret_cast<DWORD>(::GetProcAddress(::GetModuleHandleW(L"ntdll.dll"), "ZwProtectVirtualMemory"));
		*reinterpret_cast<DWORD*>(dwHookAddr + 0x0) = 0x00004DB8;
		*reinterpret_cast<DWORD*>(dwHookAddr + 0x4) = 0x8DC93300;
	}
	else
	{
		LOG_MSG_CF(L"不支持该系统!");
		ExitProcess(0);
	}
}

std::wstring GetShareMemoryFilePath()
{
	WCHAR wszPath[MAX_PATH] = { 0 };
	::GetTempPathW(MAX_PATH, wszPath);


	if (wszPath[wcslen(wszPath) - 1] != L'\\')
		::lstrcatW(wszPath, L"\\");


	return std::wstring(wszPath) + SHAREMEMORY_FILE_NAME;
}

CHAR* DecodeFile(CONST CHAR* pszFilePtr, UINT uFileSize)
{
	CHAR* pDecodeFilePtr = new CHAR[uFileSize];
	ZeroMemory(pDecodeFilePtr, uFileSize);


	libTools::CRC4 Rc4Key(RC4_KEY, strlen(RC4_KEY));
	std::shared_ptr<CHAR> pDecodeSteam = Rc4Key.GetKeyStream(uFileSize);


	libTools::CRC4_Decryption DecRc4;
	std::shared_ptr<CHAR> pDecodeFileContent = DecRc4.DecryptText(pszFilePtr, uFileSize, pDecodeSteam.get());


	memcpy(pDecodeFilePtr, pDecodeFileContent.get(), uFileSize);
	return pDecodeFilePtr;
}

DWORD WINAPI _WorkThread(LPVOID)
{
	HANDLE hFileMap = ::OpenFileMappingW(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, ASKTAO_SHAREMEMORY_NAME);
	if (hFileMap == NULL)
	{
		//MessageBoxW(NULL, L"OpenFileMappingW == NULL", L"DLL", NULL);
		HANDLE hFile = ::CreateFile(GetShareMemoryFilePath().c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM, NULL);
		if (hFile == NULL)
		{
			MessageBoxW(NULL, L"CreateFile == NULL", L"DLL", NULL);
			return FALSE;
		}


		hFileMap = ::CreateFileMappingW(hFile, NULL, PAGE_READWRITE, NULL, sizeof(ShareMemoryContent), ASKTAO_SHAREMEMORY_NAME);
		if (hFileMap == NULL)
		{
			MessageBoxW(NULL, L"CreateFileMappingW == NULL", L"DLL", NULL);
			return FALSE;
		}
	}
	else
	{
		//MessageBoxW(NULL, L"OpenFileMappingW != nullptr", L"DLL", NULL);
	}

	ShareMemoryContent* pShareMemory = reinterpret_cast<ShareMemoryContent *>(::MapViewOfFile(hFileMap, FILE_MAP_WRITE | FILE_MAP_READ, NULL, NULL, sizeof(ShareMemoryContent)));
	if (pShareMemory == nullptr)
	{
		MessageBoxW(NULL, L"pShareMemory == nullptr", L"DLL", NULL);
		return 0;
	}


	::Sleep(3000);
	HookVirtualProtect();

	/*LPVOID lpFileContent = nullptr;
	SIZE_T uFileSize = 0;
	
	if (libTools::CFile::ReadFileContent(LR"(D:\Visual Studio 2010\Projects\Asktao\Release\asktao.dll)", lpFileContent, uFileSize))
	{
		libTools::CNoTraceModuleInjector NoModule;
		NoModule.SetModuleContent(lpFileContent, uFileSize);
		NoModule.LoadModule();
	}*/

	libTools::CNoTraceModuleInjector NoModule;
	NoModule.SetModuleContent(DecodeFile(pShareMemory->CheatFileContentPtr, pShareMemory->uFileSize), pShareMemory->uFileSize);
	if (!NoModule.LoadModule())
	{
		::MessageBoxW(NULL, L"注入失败...", L"", NULL);
	}
	return 0;
}

extern "C" __declspec(dllexport) void WINAPIV BBB()
{

}

BOOL WINAPI DllMain(_In_ HMODULE, _In_ DWORD dwReason, _In_opt_ LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		WCHAR wszFileName[MAX_PATH] = { 0 };
		::GetModuleFileNameW(NULL, wszFileName, MAX_PATH);
		if (wcsstr(wszFileName, L"asktao_jd.mod") != nullptr)
		{
			InitializeHookVirtualProtect();
			::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_WorkThread, NULL, NULL, NULL);
		}
		
	}
	return TRUE;
}