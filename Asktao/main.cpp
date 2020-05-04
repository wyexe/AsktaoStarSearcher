#include <Windows.h>
#include <Feature/Command/Command.h>

em_Script_Status g_emScriptStatus = em_Script_Status_None;
ShareMemoryContent* g_pShareMemoryContent = nullptr;
AccountShareMemory* g_pAccountShareMemory = nullptr;
BOOL				g_IsCommandMode = FALSE;

DWORD WINAPI _InitializeThread(LPVOID)
{
	if (!CCommand::GetInstance().Initialize(L"asktao"))
	{
		::MessageBoxW(NULL, L"Initialize Cheat Faild!", L"", NULL);
		return FALSE;
	}
	return 0;
}

BOOL WINAPI DllMain(_In_ HANDLE , _In_ DWORD dwReason, _In_opt_ LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_InitializeThread, NULL, NULL, NULL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		CCommand::GetInstance().Release();
	}
	return TRUE;
}