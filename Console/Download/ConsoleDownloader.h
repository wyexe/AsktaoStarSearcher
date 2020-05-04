#ifndef __ASKTAO_CONSOLE_DOWNLOAD_CONSOLEDOWNLOADER_H__
#define __ASKTAO_CONSOLE_DOWNLOAD_CONSOLEDOWNLOADER_H__

#include <Asktao/Feature/Common/ServerCommon.h>

class CConsoleDownloader
{
public:
	CConsoleDownloader() = default;
	~CConsoleDownloader() = default;

	static CConsoleDownloader& GetInstance();
public:
	//
	BOOL DownloadPreviewFile(_Out_ PreviewFile_Tranfer& Vec);

	//
	BOOL DownloadHijackFile(_Out_ DownloadFileContent& HijackFile);

	//
	BOOL DownloadCheatFile(_Out_ DownloadFileContent& HijackFile);
private:

};


#endif // !__ASKTAO_CONSOLE_DOWNLOAD_CONSOLEDOWNLOADER_H__
