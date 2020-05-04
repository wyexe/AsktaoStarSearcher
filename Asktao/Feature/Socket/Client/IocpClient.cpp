#include "IocpClient.h"
#include <LogLib/Log.h>
#include <ProcessLib/Common/ResHandleManager.h>

#ifdef _DEBUG
#pragma comment(lib,"SocketCommonLib_Debug.lib")
#pragma comment(lib,"SocketClientLib_Debug.lib")
#pragma comment(lib,"ProcessLib_Debug.lib")
#else
#pragma comment(lib,"SocketCommonLib.lib")
#pragma comment(lib,"SocketClientLib.lib")
#pragma comment(lib,"ProcessLib.lib")
#endif // _DEBUG


#define _SELF L"IocpClient.cpp"

CIocpClient::CIocpClient()
{
	_dwTimeout = em_DefaulT_TimeOut;
	_DisConnectPtr = nullptr;
	_hEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	if (_hEvent == NULL)
	{
		LOG_MSG_CF(L"_hEvent = NULL");
	}
}

CIocpClient& CIocpClient::GetInstance()
{
	static CIocpClient Instance;
	return Instance;
}

BOOL CIocpClient::AsyncSend(_In_ em_Sock_Msg Msg, _In_ std::function<VOID(libTools::CSocketBuffer&)> FillBufferPtr)
{
	libTools::CSocketBuffer SocketBuffer;
	SocketBuffer.InitializeHead(Msg);
	FillBufferPtr(SocketBuffer);
	return PostSend(SocketBuffer);
}

BOOL CIocpClient::AsyncSend(_In_ libTools::CSocketBuffer& SocketBuffer)
{
	return PostSend(SocketBuffer);
}

BOOL CIocpClient::SyncSend(_In_ libTools::CSocketBuffer& SocketBuffer, _In_ std::function<VOID(libTools::CSocketBuffer&)> EchoPtr)
{
	Lock();
	_dwMsgHead = SocketBuffer.GetMsgHead<DWORD>();
	_EchoPtr = EchoPtr;

	for (int nLoop = 0;nLoop < 6; ++nLoop)
	{
		//LOG_CF_D(L"nLoop=[%d], _dwMsgHead=[%d]", nLoop, _dwMsgHead);
		if (!AsyncSend(SocketBuffer))
		{
			LOG_MSG_CF(L"AsyncSend = FALSE");
			UnLock();
			return FALSE;
		}


		for (UINT i = 0; i < _dwTimeout / 5 && _bConnect; ++i) 
		{
			if (::WaitForSingleObject(_hEvent, 5 * 1000) == WAIT_TIMEOUT)
			{
				continue;
			}



			EchoPtr = nullptr;
			_dwMsgHead = NULL;
			UnLock();
			return TRUE;
		}
	}
	


	LOG_CF_E(L"SyncSend Timeout 30s! Msg=%X, _bConnect = %d", _dwMsgHead, _bConnect);
	UnLock();
	return FALSE;
}

VOID CIocpClient::SetDisConnectPtr(_In_ std::function<VOID(VOID)> DisConnectPtr)
{
	_DisConnectPtr = DisConnectPtr;
}

VOID CIocpClient::SetTimeout(_In_ DWORD dwTimeout)
{
	_dwTimeout = dwTimeout;
}

VOID CIocpClient::EchoPacket(_In_ libTools::CSocketBuffer& SocketBuffer)
{
	em_Sock_Msg emSockMsg;
	if (SocketBuffer.VerDataPtr(emSockMsg) != libTools::CSocketBuffer::em_SocketBuffer_Error_None)
	{
		LOG_MSG_CF(L"Invalid Packet!, emSockMsg=%d", emSockMsg);
		return;
	}


	if (emSockMsg == em_Sock_Msg_Error)
	{
		std::wstring wsErrText;
		SocketBuffer >> wsErrText;
		LOG_MSG_CF(L"错误:[%s]", wsErrText.c_str());
		return;
	}


	if (_hEvent == INVALID_HANDLE_VALUE || SocketBuffer.GetMsgHead<DWORD>() != _dwMsgHead)
	{
		LOG_CF_E(L"无效的包[%d], 当前dwMsgHead=[%d]", SocketBuffer.GetMsgHead<DWORD>(), _dwMsgHead);
		return;
	}


	if (_EchoPtr != nullptr)
	{
		//LOG_CF_D(L"Echo Packet[%d]", _dwMsgHead);
		_EchoPtr(SocketBuffer);
	}

	//LOG_CF_D(L"SetEvent to SyncSend[%d]", _dwMsgHead);
	::SetEvent(_hEvent);
}

VOID CIocpClient::DisConnectServer()
{
	LOG_MSG_CF(L"与咱们的服务器失去连接!");
	if (_DisConnectPtr != nullptr)
	{
		_DisConnectPtr();
	}
}
