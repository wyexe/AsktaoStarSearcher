#ifndef __ASKTAO_CONSOLE_INITIALIZE_INITIALIZECONSOLE_H__
#define __ASKTAO_CONSOLE_INITIALIZE_INITIALIZECONSOLE_H__

#include <Windows.h>
#include <vector>
#include <string>
#include <Asktao/Feature/Common/ServerCommon.h>

class CInitializeConsole
{
public:
	CInitializeConsole() = default;
	~CInitializeConsole() = default;

	// Initialize Console
	BOOL InitializeConsole();

	// Initialize Downloader

	// Initialize Game(Modify Import Table, Copy HijackDLL to GameDirectory)
	BOOL InitializeGame();

	//
	VOID Free();
private:
	// 
	BOOL DownloadPreviewFile();

	//
	BOOL CompLocalFile();

	//
	BOOL DownloadHijackFile();

	//
	BOOL CopyHijackFileToGame();

	//
	BOOL InitializeConfig();

	//
	BOOL InitializeAccount();

	//
	BOOL InitializeShareMemory();

	//
	VOID ClearGameLog() CONST;

	//
	std::wstring GetShareMemoryFilePath() CONST;
private:
	DownloadFileContent		_HijackFile;
	std::wstring			_wsGamePath;
	PreviewFile_Tranfer		_VecPreviewFile;
};




#endif // !__ASKTAO_CONSOLE_INITIALIZE_INITIALIZECONSOLE_H__
