#ifndef __ASKTAO_ASKTAO_FEATURE_ANTIDEBUG_ANTIDEBUG_H__
#define __ASKTAO_ASKTAO_FEATURE_ANTIDEBUG_ANTIDEBUG_H__

#include <Windows.h>
#include <winternl.h>

class CAntiDebug
{
private:
	struct NEW_SYSTEM_PROCESS_INFORMATION {
		ULONG NextEntryOffset;
		ULONG NumberOfThreads;
		BYTE Reserved1[48];
		UNICODE_STRING ImageName;
		LONG BasePriority;
		HANDLE UniqueProcessId;
		PVOID Reserved2;
		ULONG HandleCount;
		ULONG SessionId;
		PVOID Reserved3;
		SIZE_T PeakVirtualSize;
		SIZE_T VirtualSize;
		ULONG Reserved4;
		SIZE_T PeakWorkingSetSize;
		SIZE_T WorkingSetSize;
		PVOID Reserved5;
		SIZE_T QuotaPagedPoolUsage;
		PVOID Reserved6;
		SIZE_T QuotaNonPagedPoolUsage;
		SIZE_T PagefileUsage;
		SIZE_T PeakPagefileUsage;
		SIZE_T PrivatePageCount;
		LARGE_INTEGER Reserved7[6];
	};
public:
	CAntiDebug() = default;
	~CAntiDebug();

	static CAntiDebug& GetInstance();

	VOID Release() const;

	VOID Run() const;

public:
	static NTSTATUS WINAPI NewNtQuerySystemInformation(_In_ SYSTEM_INFORMATION_CLASS SystemInformationClass,_Inout_  PVOID SystemInformation,_In_ ULONG SystemInformationLength,_Out_opt_ PULONG ReturnLength);

private:
	using NtQuerySystemInformationPtr = NTSTATUS(WINAPI *)(
		_In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
		_Inout_   PVOID                    SystemInformation,
		_In_      ULONG                    SystemInformationLength,
		_Out_opt_ PULONG                   ReturnLength);


private:

	static NtQuerySystemInformationPtr RealNtQuerySystemInformationPtr;
};



#endif // !__ASKTAO_ASKTAO_FEATURE_ANTIDEBUG_ANTIDEBUG_H__
