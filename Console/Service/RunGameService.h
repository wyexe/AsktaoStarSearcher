#ifndef __ASKTAO_CONSOLE_SERVICE_RUNGAMESERVICE_H__
#define __ASKTAO_CONSOLE_SERVICE_RUNGAMESERVICE_H__

#include <Asktao/Feature/Common/ServerCommon.h>

class CRunGameService
{
public:
	CRunGameService() = default;
	~CRunGameService() = default;


	BOOL Run();
private:
	//
	BOOL DownloadGameCheatFile();

	//
	VOID KeepAliveThread();

	//
	BOOL SendConsoleKeepALive() CONST;

	//
	BOOL SendLoseConnectKeepALive(_In_ CONST ClientKeepALive_Tranfer& Content) CONST;
private:
	HANDLE _hFileMap = NULL;
};



#endif // !__ASKTAO_CONSOLE_SERVICE_RUNGAMESERVICE_H__
