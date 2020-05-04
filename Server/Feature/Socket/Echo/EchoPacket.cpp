#include "EchoPacket.h"
#include <Server/Feature/FileCache/FileCache.h>
#include <Server/Feature/DB/DbManager.h>
#include <SocketCommon/SocketRemoteClient.h>
#include <LogLib/Log.h>

#define _SELF L"EchoPacket.cpp"

std::queue<StarInfo_Tranfer>	CEchoPacket::_VecStarInfo;
libTools::CThreadLock			CEchoPacket::_LockVecStarInfo;

std::queue<ClientKeepALive_Tranfer>	CEchoPacket::_QueKeepALiveContent;
libTools::CThreadLock				CEchoPacket::_LockQueKeepALiveContent;

std::vector<ReleaseSearchStarTaskContent_Tranfer>	CEchoPacket::_VecRumorStar;
libTools::CThreadLock								CEchoPacket::_LockRumorStar;

std::queue<std::wstring>	CEchoPacket::_QueRumor;
libTools::CThreadLock	CEchoPacket::_LockQueRumor;

BOOL CEchoPacket::KeepALive(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer)
{
	pRemoteClient->SetKeepALive();
	if(pSocketBuffer->GetMsgHead<em_Sock_Msg>() != em_Sock_Msg_Console_KeepALive || TRUE)
	{
		auto Content = pSocketBuffer->GetStruct<ClientKeepALive_Tranfer>();
		//LOG_C_D(L"�˺�[%s]����", Content.wsPlayerName.c_str());
		std::lock_guard<libTools::CThreadLock> Lock(_LockQueKeepALiveContent);
		while (_QueKeepALiveContent.size() > 1000)
		{
			_QueKeepALiveContent.pop();
		}


		//LOG_C_D(L"�ϴ��˺ſ���̨��Message=[%d,%s,%s]", Content.emType, Content.wsPlayerName.c_str(), Content.wsComputerName.c_str());
		_QueKeepALiveContent.push(Content);
	}


	pSocketBuffer->InitializeHead(pSocketBuffer->GetMsgHead<em_Sock_Msg>());
	return TRUE;
}

BOOL CEchoPacket::DownloadFile(_In_ libTools::CSocketRemoteClient* , _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer)
{
	CONST static std::vector<std::wstring> VecHijackFile = { L"msvcp80.dll", L"msvcp91.dll" };
	CONST static std::vector<std::wstring> VecCheatFile = { L"Asktao.dll" };
	CONST static std::vector<std::wstring> VecConsoleFile = { CONSOLE_NAME };


	em_Sock_Msg Msg = pSocketBuffer->GetMsgHead<em_Sock_Msg>();
	pSocketBuffer->InitializeHead(Msg);


	switch (Msg)
	{
	case em_Sock_Msg_Download_PreviewFile:
		return CFileCache::GetInstance().DownloadPreviewFile(VecHijackFile, *pSocketBuffer);
	case em_Sock_Msg_Download_HijackFile:
		return CFileCache::GetInstance().DownLoadFile(VecHijackFile, *pSocketBuffer);
	case em_Sock_Msg_Download_GameFile:
	case em_Sock_Msg_Download_CheatFile:
		return CFileCache::GetInstance().DownLoadFile(VecCheatFile, *pSocketBuffer);
	case em_Sock_Msg_Download_Console:
		return CFileCache::GetInstance().DownLoadFile(VecConsoleFile, *pSocketBuffer);
	case em_Sock_Msg_Download_ConsoleCRC:
		return CFileCache::GetInstance().DownloadPreviewFile(VecConsoleFile, *pSocketBuffer);
	default:
		break;
	}


	pSocketBuffer->InitializeHead(em_Sock_Msg_Error);
	*pSocketBuffer << L"��Ч����Ϣ����!";
	return FALSE;
}

BOOL CEchoPacket::UploadStarInfo(_In_ libTools::CSocketRemoteClient* , _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer)
{
	StarInfo_Tranfer StartInfo = pSocketBuffer->GetStruct<StarInfo_Tranfer>();


	_LockVecStarInfo.lock();
	while (_VecStarInfo.size() > 1000)
	{
		_VecStarInfo.pop();
	}
	_VecStarInfo.push(StartInfo);
	_LockVecStarInfo.unlock();


	//LOG_C_D(L"[%s]", StartInfo.wsShowContent.c_str());
	CDbManager::GetInstance().AddSearchStarRecord(StartInfo);

	pSocketBuffer->InitializeHead(em_Sock_Msg_Upload_StarInfo);
	return TRUE;
}

BOOL CEchoPacket::DownloadStarInfo(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer)
{
	pRemoteClient->SetKeepALive();

	pSocketBuffer->InitializeHead(em_Sock_Msg_Download_StarInfo);
	_LockVecStarInfo.lock();
	if (_VecStarInfo.empty())
	{
		*pSocketBuffer << FALSE;
	}
	else
	{
		StarInfo_Tranfer StarInfo = _VecStarInfo.front();
		_VecStarInfo.pop();


		*pSocketBuffer << TRUE;
		pSocketBuffer->AddStruct(StarInfo);
	}

	_LockVecStarInfo.unlock();
	return TRUE;
}

BOOL CEchoPacket::DownloadStarMapPoint(_In_ libTools::CSocketRemoteClient* , _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer)
{
	std::wstring wsMapName;
	*pSocketBuffer >> wsMapName;

	pSocketBuffer->InitializeHead(em_Sock_Msg_Download_StarMapPoint);
	return CFileCache::GetInstance().DownloadVecStarMapPoint(wsMapName, *pSocketBuffer);
}

BOOL CEchoPacket::DownloadKeepALive(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer)
{
	pRemoteClient->SetKeepALive();
	pSocketBuffer->InitializeHead(pSocketBuffer->GetMsgHead<em_Sock_Msg>());
	{
		std::lock_guard<libTools::CThreadLock> Lock(_LockQueKeepALiveContent);
		*pSocketBuffer << _QueKeepALiveContent.size();
		while (!_QueKeepALiveContent.empty())
		{
			auto Content = _QueKeepALiveContent.front();
			//LOG_C_D(L"�·��˺ſ���̨��Message=[%d,%s,%s]", Content.emType, Content.wsPlayerName.c_str(), Content.wsComputerName.c_str());
			pSocketBuffer->AddStruct(Content);
			_QueKeepALiveContent.pop();
		}
	}

	return TRUE;
}

BOOL CEchoPacket::ReleaseSearchStarTask(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer)
{
	pRemoteClient->SetKeepALive();


	BOOL bExist = TRUE;
	ReleaseSearchStarTaskContent_Tranfer Content = pSocketBuffer->GetStruct<ReleaseSearchStarTaskContent_Tranfer>();
	{
		std::lock_guard<libTools::CThreadLock> tmpLock(_LockRumorStar);
		ClearTimeoutSearchStarTask();


		bExist = FALSE;
		Content.ulTick = ::GetTickCount64();
		_VecRumorStar.push_back(Content);
		//LOG_C_D(L"ҥ�Է���[%s] ��[%s-%s]", Content.wsNpcName.c_str(), Content.wsChannel.c_str(), Content.wsMapName.c_str());
		/*CONST auto itr = std::find_if(_VecRumorStar.begin(), _VecRumorStar.end(), [Content](CONST ReleaseSearchStarTaskContent_Tranfer& itm) { return itm.wsMapName == Content.wsMapName; });
		if (itr == _VecRumorStar.end())
		{
			bExist = FALSE;
			Content.ulTick = ::GetTickCount64();
			_VecRumorStar.push_back(Content);
			LOG_C_D(L"����[%s] ��[%s-%s]", Content.wsNpcName.c_str(), Content.wsChannel.c_str(), Content.wsMapName.c_str());
		}*/
	}


	if(!bExist)
	{
		std::lock_guard<libTools::CThreadLock> tmpLock(_LockQueRumor);

		_QueRumor.push(Content.wsShowContent);
	}



	CDbManager::GetInstance().AddRumorRecord(Content.wsShowContent);
	pSocketBuffer->InitializeHead(em_Sock_Msg_ReleaseTask_SearchStar);
	return TRUE;
}

BOOL CEchoPacket::AcceptSearchStarTask(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer)
{
	pRemoteClient->SetKeepALive();


	std::wstring wsMapName, wsPlayerName;
	*pSocketBuffer >> wsMapName >> wsPlayerName;
	pSocketBuffer->InitializeHead(em_Sock_Msg_AcceptTask_SearchStar);
	{
		std::lock_guard<libTools::CThreadLock> tmpLock(_LockRumorStar);
		ClearTimeoutSearchStarTask();


		for (CONST auto& itm : _VecRumorStar)
		{
			if (itm.wsMapName == wsMapName/* && (::GetTickCount64() - itm.ulTick) >= (2 * 60) * 1000*/)// 2min + half min
			{
				MatchStar_Tranfer Star;
				Star.Tick = itm.Tick;
				Star.wsNpcName = itm.wsNpcName;
				Star.wsChannel = itm.wsChannel;
				//LOG_C_D(L"����[%s-%s] ����ͼ[%s-%s]", itm.wsNpcName.c_str(), itm.wsChannel.c_str(), itm.wsMapName.c_str(), wsPlayerName.c_str());
				pSocketBuffer->AddStruct(Star);
				return TRUE;
			}
		}
	}

	*pSocketBuffer << L"";
	return TRUE;
}

BOOL CEchoPacket::DownloadRumor(_In_ libTools::CSocketRemoteClient* pRemoteClient, _In_ _Out_ libTools::CSocketBuffer* pSocketBuffer)
{
	pRemoteClient->SetKeepALive();
	{
		pSocketBuffer->InitializeHead(em_Sock_Msg_Download_Rumor);
		std::lock_guard<libTools::CThreadLock> tmpLock(_LockQueRumor);
		if (_QueRumor.empty())
		{
			*pSocketBuffer << FALSE;
		}
		else
		{
			//LOG_C_D(L"����ҥ��[%s]������̨!", _QueRumor.front().c_str());
			*pSocketBuffer << TRUE << _QueRumor.front();
			_QueRumor.pop();
		}
	}
	return TRUE;
}

VOID CEchoPacket::ClearTimeoutSearchStarTask()
{
	for (auto itr = _VecRumorStar.begin(); itr != _VecRumorStar.end();)
	{
		if (::GetTickCount64() - itr->ulTick >= (2 * 60 + 50)  * 1000)
		{
			//LOG_C_D(L"2��30s ����,���[%s]",itr->wsNpcName.c_str());
			itr = _VecRumorStar.erase(itr);
		}
		else
		{
			itr = itr + 1;
		}
	}
}

