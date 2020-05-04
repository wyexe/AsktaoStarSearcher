#ifndef __ASKTAO_SERVER_FEATURE_FILECACHE_FILECACHE_H__
#define __ASKTAO_SERVER_FEATURE_FILECACHE_FILECACHE_H__

#include <map>
#include <Asktao/Feature/Common/ServerCommon.h>
#include <ProcessLib/Lock/Lock.h>


class CFileCache final : public libTools::CThreadLock
{
public:
	CFileCache() = default;
	~CFileCache() = default;

	static CFileCache& GetInstance();
public:
	//
	BOOL Initialize();
	
	// 
	BOOL DownLoadFile(_In_ CONST std::vector<std::wstring>& VecFileName, _Out_ libTools::CSocketBuffer& SocketBuffer);

	//
	BOOL DownloadPreviewFile(_In_ CONST std::vector<std::wstring>& VecFileName, _Out_ libTools::CSocketBuffer& SocketBuffer);

	//
	BOOL DownloadVecStarMapPoint(_In_ CONST std::wstring& wsMapName, _Out_ libTools::CSocketBuffer& SocketBuffer);
private:
	std::map<std::wstring , SingleDownloadFile> _VecFileCache;
};



#endif // !__ASKTAO_SERVER_FEATURE_FILECACHE_FILECACHE_H__
