#ifndef __ASKTAO_ASKTAO_FEATURE_SCRIPT_CATCHPETSCRIPT_H__
#define __ASKTAO_ASKTAO_FEATURE_SCRIPT_CATCHPETSCRIPT_H__

#include <Feature/Common/GameStruct.h>
#include <vector>

class CCatchPetScript
{
public:
	CCatchPetScript() = default;
	~CCatchPetScript() = default;

	BOOL Run();
private:
	//
	VOID MoveToNextPoint(_In_ CONST Point& Pos);

	//
	VOID CatchPet() CONST;

	//
	BOOL IsSpecifyMonsterName(_In_ CONST std::wstring& wsMonsterName) CONST;

	//
	VOID CheckMonsterSkill();

	//
	VOID CheckOtherDlg();
private:
	std::vector<std::wstring> _VecSpecifyMonsterName;
};



#endif // !__ASKTAO_ASKTAO_FEATURE_SCRIPT_CATCHPETSCRIPT_H__
