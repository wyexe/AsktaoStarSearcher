#include "AntiDebug.h"
#include <CharacterLib/Character.h>
#include <HookLib/InlineHook/InlineHook.h>

CAntiDebug::NtQuerySystemInformationPtr CAntiDebug::RealNtQuerySystemInformationPtr = nullptr;

CAntiDebug::~CAntiDebug()
{
	Release();
}

CAntiDebug& CAntiDebug::GetInstance()
{
	static CAntiDebug Instance;
	return Instance;
}

VOID CAntiDebug::Release() const
{
	if (RealNtQuerySystemInformationPtr != nullptr)
	{
		const NtQuerySystemInformationPtr Ptr = reinterpret_cast<NtQuerySystemInformationPtr>(::GetProcAddress(::GetModuleHandleW(L"ntdll.dll"), "NtQuerySystemInformation"));
		libTools::CInlineHook::UnHook(Ptr, RealNtQuerySystemInformationPtr);
		RealNtQuerySystemInformationPtr = nullptr;
	}
}

VOID CAntiDebug::Run() const
{
	const NtQuerySystemInformationPtr Ptr = reinterpret_cast<NtQuerySystemInformationPtr>(::GetProcAddress(::GetModuleHandleW(L"ntdll.dll"),
	                                                                                                 "NtQuerySystemInformation"));
	if (!libTools::CInlineHook::Hook(Ptr, reinterpret_cast<void **>(&NewNtQuerySystemInformation), reinterpret_cast<void **>(&RealNtQuerySystemInformationPtr)))
	{
		::MessageBoxW(nullptr, L"Hook Failed ...", L"", NULL);
	}
}

NTSTATUS WINAPI CAntiDebug::NewNtQuerySystemInformation(_In_ SYSTEM_INFORMATION_CLASS SystemInformationClass, _Inout_ PVOID SystemInformation, _In_ ULONG SystemInformationLength, _Out_opt_ PULONG ReturnLength)
{
	if (SystemInformationClass != SystemProcessInformation)
	{
		return RealNtQuerySystemInformationPtr(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	}


	const NTSTATUS NtRetCode = RealNtQuerySystemInformationPtr(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	if (NT_SUCCESS(NtRetCode))
	{
		NEW_SYSTEM_PROCESS_INFORMATION * pPrevent = nullptr;
		for (NEW_SYSTEM_PROCESS_INFORMATION * pCurrent = static_cast<NEW_SYSTEM_PROCESS_INFORMATION *>(SystemInformation); ; pCurrent = reinterpret_cast<NEW_SYSTEM_PROCESS_INFORMATION *>(reinterpret_cast<PUCHAR>(pCurrent) + pCurrent->NextEntryOffset))
		{
			// break Link Table
			if (pCurrent->ImageName.Buffer != nullptr && libTools::CCharacter::MakeTextToLower(std::wstring(pCurrent->ImageName.Buffer)) == L"calc.exe")
			{
				if (pCurrent->NextEntryOffset == NULL)
				{
					pPrevent->NextEntryOffset = NULL;
					break;
				}
				else
				{
					pPrevent->NextEntryOffset += pCurrent->NextEntryOffset;
					continue;
				}
			}
			else if (pCurrent->NextEntryOffset == NULL)
			{
				break;
			}


			pPrevent = pCurrent;
		}
	}
	return NtRetCode;
}
