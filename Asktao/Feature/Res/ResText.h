#ifndef __ASKTAO_ASKTAO_FEATURE_RES_RESTEXT_H__
#define __ASKTAO_ASKTAO_FEATURE_RES_RESTEXT_H__

#include <Feature/Common/GameStruct.h>
#include <vector>

class CGameUi;
class CResText
{
public:
	struct StarFixedMap
	{
		std::wstring				wsNpcName;
		DWORD						dwLevel;
		std::vector<std::wstring>	VecMapName;
	};


	struct ResCatchPet
	{
		std::wstring		wsMapName;
		std::vector<Point>	Vec;
	};

	struct ResStarPoint
	{
		std::wstring	wsMapName;
		std::wstring	wsLineName;
		Point			Pos1;
		Point			Pos2;
	};
public:
	CResText() = default;
	~CResText() = default;

	static CResText& GetInstance();

public:
	static BOOL IsStar(_In_ CONST std::wstring& wsNpcName, _Out_opt_ DWORD* pdwLevel = nullptr);

	static BOOL IsExistStar_In_Text(_In_ CONST std::wstring& wsText, _Out_ StarFixedMap* pFixedMap);

	static std::wstring GetChannelName(_In_ DWORD dwChannel);

	static BOOL IsExistTalkMenuDlg(_Out_ CGameUi* pTalkMenuDlg);

	static BOOL GetResCatchPet(_In_ CONST std::wstring& wsMapName, _Out_ ResCatchPet* pResCatchPet);

	static BOOL GetResStarPoint(_In_ CONST std::wstring& wsMapName, _Out_ std::vector<Point>& VecPoint);
private:
	static std::wstring ConvertChannelNumberToText(_In_ CONST DWORD dwChannel);
};



#endif // !__ASKTAO_ASKTAO_FEATURE_RES_RESTEXT_H__
