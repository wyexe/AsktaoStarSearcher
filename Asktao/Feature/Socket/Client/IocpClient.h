#ifndef __ASKTAO_ASKTAO_FEATURE_CORE_SOCKET_CLIENT_IOCPCLIENT_H__
#define __ASKTAO_ASKTAO_FEATURE_CORE_SOCKET_CLIENT_IOCPCLIENT_H__

#include <SocketClientLib/SocketBaseClientService.h>
#include <SocketCommon/SocketBuffer.h>
#include <ProcessLib/Lock/Lock.h>
#include <Asktao/Feature/Common/ServerCommon.h>

class CIocpClient : public libTools::CSocketBaseClientService, public libTools::CThreadLock
{
public:
	enum
	{
		em_DefaulT_TimeOut = 10
	};
public:
	CIocpClient();
	virtual ~CIocpClient() = default;

	static CIocpClient& GetInstance();

	BOOL SyncSend(_In_ libTools::CSocketBuffer& SocketBuffer, _In_ std::function<VOID(libTools::CSocketBuffer&)> EchoPtr);

	VOID SetDisConnectPtr(_In_ std::function<VOID(VOID)> DisConnectPtr);

	// Second
	VOID SetTimeout(_In_ DWORD dwTimeout);
protected:
	virtual VOID EchoPacket(_In_ libTools::CSocketBuffer& SocketBuffer) override;

	virtual VOID DisConnectServer() override;

	BOOL AsyncSend(_In_ libTools::CSocketBuffer& SocketBuffer);

	BOOL AsyncSend(_In_ em_Sock_Msg Msg, _In_ std::function<VOID(libTools::CSocketBuffer&)> FillBufferPtr);
private:
	DWORD _dwMsgHead;
	HANDLE _hEvent;
	std::function<VOID(libTools::CSocketBuffer& SocketBuffer)> _EchoPtr;
	std::function<VOID(VOID)> _DisConnectPtr;
	DWORD _dwTimeout;
};




#endif // !__ASKTAO_ASKTAO_FEATURE_CORE_SOCKET_CLIENT_IOCPCLIENT_H__
