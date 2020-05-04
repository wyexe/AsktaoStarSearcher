#ifndef __ASKTAO_ASKTAO_FEATURE_SCRIPT_SHIJUEFIGHTER_H__
#define __ASKTAO_ASKTAO_FEATURE_SCRIPT_SHIJUEFIGHTER_H__

#include <functional>
#include <Feature/Common/GameStruct.h>


class CGameUi;
class CShiJueFighter
{
public:
	CShiJueFighter() = default;
	~CShiJueFighter() = default;

	BOOL Run();
private:
	//
	VOID ClickAutoFightDlg_ContinueButton();
};



#endif // !__ASKTAO_ASKTAO_FEATURE_SCRIPT_SHIJUEFIGHTER_H__
