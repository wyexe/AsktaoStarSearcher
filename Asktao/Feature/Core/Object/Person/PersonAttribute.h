#ifndef __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_PERSON_PERSONATTRIBUTE_H__
#define __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_PERSON_PERSONATTRIBUTE_H__

#include <string>
#include <Feature/Common/GameStruct.h>

class CPersonAttribute
{
public:
	CPersonAttribute() = default;
	~CPersonAttribute() = default;

	// 获取当前服务器
	static std::wstring GetServerName();

	// 当前角色的线路
	static CONST std::wstring GetChannelName();


	// 当前角色的坐标
	static Point GetCurrentPoint();


	// 当前角色的地图名
	static CONST std::wstring GetCurrentMapName();


	// 当前角色名
	static CONST std::wstring GetPlayerName();

	// 是否战斗中
	static BOOL IsFighting();

	// ID
	static DWORD GetPersonId();

	// 是否轮到自己的回合
	static BOOL IsSelfRound();

	// 是否Moving
	static BOOL IsMoving();
private:

};



#endif // !__ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_PERSON_PERSONATTRIBUTE_H__
