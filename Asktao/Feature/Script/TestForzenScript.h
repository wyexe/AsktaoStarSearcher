#ifndef __ASKTAO_ASKTAO_FEATURE_SCRIPT_TESTFROZENSCRIPT_H__
#define __ASKTAO_ASKTAO_FEATURE_SCRIPT_TESTFROZENSCRIPT_H__

#include <Feature/Common/GameStruct.h>

class CTestFrozenScript
{
public:
	CTestFrozenScript() = default;
	~CTestFrozenScript() = default;

	static CTestFrozenScript& GetInstance();

	BOOL Run();

	static VOID MoveToPoint(_In_ CONST Point& Pos);

	static VOID InputText(_In_ CONST std::wstring& wsText);
};

#endif// !__ASKTAO_ASKTAO_FEATURE_SCRIPT_TESTFROZENSCRIPT_H__
