#ifndef __ASKTAO_SERVER_FEATURE_SOCKET_ECHO_ECHOPAKCET_H__
#define __ASKTAO_SERVER_FEATURE_SOCKET_ECHO_ECHOPAKCET_H__

#include <functional>
#include <queue>
#include <Asktao/Feature/Common/ServerCommon.h>
#include <SocketCommon/SocketRemoteClient.h>
#include <ProcessLib/Lock/Lock.h>

class CEchoPacket
{
public:
	CEchoPacket() = default;
	~CEchoPacket() = default;

	static BOOL KeepALive(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer);

	static BOOL DownloadFile(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer);

	static BOOL UploadStarInfo(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer);

	static BOOL DownloadStarInfo(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer);

	static BOOL DownloadStarMapPoint(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer);

	static BOOL DownloadKeepALive(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer);

	static BOOL ReleaseSearchStarTask(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer);

	static BOOL AcceptSearchStarTask(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer);

	static BOOL DownloadRumor(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer);
private:
	static VOID ClearTimeoutSearchStarTask();
private:
	template<typename T>
	static T ExtractPacket(_In_ std::function<T()> Ptr)
	{
		return std::move(Ptr());
	}
private:
	static std::queue<StarInfo_Tranfer>			_VecStarInfo;
	static libTools::CThreadLock				_LockVecStarInfo;



	static std::queue<ClientKeepALive_Tranfer>	_QueKeepALiveContent;
	static libTools::CThreadLock				_LockQueKeepALiveContent;


	static std::vector<ReleaseSearchStarTaskContent_Tranfer>	_VecRumorStar;
	static libTools::CThreadLock								_LockRumorStar;

	static std::queue<std::wstring>				_QueRumor;
	static libTools::CThreadLock				_LockQueRumor;

};


#endif // !__ASKTAO_SERVER_FEATURE_SOCKET_ECHO_ECHOPAKCET_H__
