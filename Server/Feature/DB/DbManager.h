#ifndef __ASKTAO_SERVER_FEATURE_DB_DBMANAGER_H__
#define __ASKTAO_SERVER_FEATURE_DB_DBMANAGER_H__

#include <DbManagerLib/BaseDbManager.h>
#include <Asktao/Feature/Common/ServerCommon.h>



class CDbManager final : public libTools::CBaseDbManager
{
public:
	CDbManager() = default;
	~CDbManager() = default;

	static CDbManager& GetInstance();

	virtual VOID Initialize() override;

	// 
	VOID AddSearchStarRecord(_In_ CONST StarInfo_Tranfer& StartInfo);

	//
	VOID AddRumorRecord(_In_ CONST std::wstring& wsRumorText);
private:

};



#endif // !__ASKTAO_SERVER_FEATURE_DB_DBMANAGER_H__
