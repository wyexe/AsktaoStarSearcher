#ifndef __ASKTAO_ASKTAO_CORE_COMMAND_COMMAND_H__
#define __ASKTAO_ASKTAO_CORE_COMMAND_COMMAND_H__

#include <Feature/Common/GameStruct.h>
#include <atomic>
#include <functional>
#include <future>
#include <LogLib/LogExpression.h>
#include <ProcessLib/Lock/Lock.h>

class CCommandExpr : public libTools::CExprFunBase
{
public:
	CCommandExpr() = default;
	virtual ~CCommandExpr();

	virtual std::vector<libTools::ExpressionFunPtr>& GetVec() override;

	virtual VOID Release() override;

	virtual VOID Help(CONST std::vector<std::wstring>&) override;
private:
	VOID Run(CONST std::vector<std::wstring>& Vec);

	VOID Stop(CONST std::vector<std::wstring>&);

	VOID PrintNpc(CONST std::vector<std::wstring>&);

	VOID PrintTask(CONST std::vector<std::wstring>&);

	VOID PrintItem(CONST std::vector<std::wstring>&);

	VOID PrintPerson(CONST std::vector<std::wstring>&);

	VOID PrintGameUi(CONST std::vector<std::wstring>&);

	VOID PrintMonster(CONST std::vector<std::wstring>&);

	VOID PrintPet(CONST std::vector<std::wstring>&);

	VOID PrintPetSkill(CONST std::vector<std::wstring>&);

	VOID ScanBase(CONST std::vector<std::wstring>&);

	VOID PrintChat(CONST std::vector<std::wstring>&);

	VOID Test(CONST std::vector<std::wstring>&);
private:
	std::future<VOID> _AsyncScriptPtr;
};



class CCommand : public libTools::CThreadLock
{
private:
	using PeekMessageADef = BOOL(WINAPI *)(_Out_ LPMSG lpMsg, _In_opt_ HWND hWnd, _In_ UINT wMsgFilterMin, _In_ UINT wMsgFilterMax, _In_ UINT wRemoveMsg);
public:
	CCommand() = default;
	~CCommand();

	static CCommand& GetInstance();

	// Initialize Log System, CmdLog System, Hook System
	// 初始化日志、 命令、 Hook
	BOOL Initialize(_In_ CONST std::wstring& wsPlayerName);


	// Put Method to Game Ui Thread
	// 把代码丢到游戏线程去执行, 做线程同步
	VOID ExcutePtrToGame(_In_ std::function<VOID(VOID)> Ptr);

	//
	VOID Release();

	//
	std::wstring GetGameWindowText() CONST;

	//
	HWND GetGameHwnd() CONST;
private:
	//
	BOOL InitializeShareMemory();
private:
	static BOOL WINAPI NewPeekMessageA(_Out_ LPMSG lpMsg, _In_opt_ HWND hWnd, _In_ UINT wMsgFilterMin, _In_ UINT wMsgFilterMax, _In_ UINT wRemoveMsg);
public:
	static PeekMessageADef				_OldPeekMessageA;
	static std::atomic<bool>			_IsExistExcuteMethod;
	static std::function<VOID(VOID)>	_ExcuteMethodPtr;
	static HANDLE						_hEvent;
	DWORD								_dwPeekMessageAProc;
	HWND								_hGameWnd;
	HANDLE								_hFileMap;
	std::future<VOID>					_SyncScriptServicePtr;
	BOOL								_IsRun;
};


#endif // !__ASKTAO_ASKTAO_CORE_COMMAND_COMMAND_H__
