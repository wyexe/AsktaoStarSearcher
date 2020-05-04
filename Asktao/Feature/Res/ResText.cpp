#include "ResText.h"
#include <vector>
#include <map>
#include <LogLib/Log.h>
#include <ExceptionLib/Exception.h>
#include <Feature/Core/Object/VecGameObject.h>
#include <Feature/Command/Command.h>

#define _SELF L"ResText.cpp"


CResText& CResText::GetInstance()
{
	static CResText Instance;
	return Instance;
}

BOOL CResText::IsStar(_In_ CONST std::wstring& wsNpcName, _Out_opt_ DWORD* pdwLevel)
{
	struct NpcNameContent
	{
		std::wstring wsNpcName;
		DWORD        dwLevel;
	};

	CONST static std::map<std::wstring, DWORD> Vec =
	{
		//{ L"地魁星", 60 }, { L"地煞星", 63 }, { L"地勇星", 66 }, { L"地杰星", 69 },// { L"地雄星", 72 },
		//{ L"地威星", 75 }, { L"地英星", 78 }, { L"地奇星", 81 }, { L"地猛星", 84 }, { L"地文星", 87 },
		//{ L"地正星", 90 }, { L"地辟星", 93 }, { L"地阖星", 96 }, { L"地强星", 99 }, { L"地暗星", 102 },
		//{ L"地辅星", 105 }, { L"地会星", 108 }, 
		/*{ L"地佐星", 111 }, { L"地佑星", 114 }, { L"地灵星", 117 },
		{ L"地兽星", 120 }, { L"地微星", 122 }, { L"地慧星", 124 }, { L"地暴星", 126 }, { L"地默星", 128 },
		{ L"地猖星", 130 }, { L"地狂星", 132 }, { L"地飞星", 134 }, { L"地走星", 139 },	{ L"地巧星", 139 },
		{ L"地明星", 140 },	{ L"地进星", 142 },	{ L"地退星", 144 },	{ L"地满星", 146 },	{ L"地遂星", 148 },
		{ L"地周星", 150 },	{ L"地隐星", 152 },	{ L"地异星", 154 },	{ L"地理星", 156 },	{ L"地俊星", 158 },
		{ L"地乐星", 160 },	{ L"地捷星", 162 },	{ L"地速星", 164 },*/
		{ L"孔雀妖姬", 140 },{ L"白骨精", 130 },	{ L"罗刹王", 120 }, { L"夜叉王", 110 }, { L"牛魔王", 100 },
		{ L"百花羞", 90}, { L"百年象精", 80}, { L"百年猪妖", 70}, { L"百年刺猬精", 60}, { L"百年狂狮怪", 50}, 
		{ L"百年黑熊精", 40},
		// 测试数据
		//{ L"小花仙", 139}, { L"翻天鼠", 139},{ L"恶灵鬼王", 139},
	};

	
	CONST auto itr = Vec.find(wsNpcName);
	if (itr != Vec.end() && pdwLevel != nullptr)
	{
		*pdwLevel = itr->second;
	}

	return itr != Vec.end();
}

BOOL CResText::IsExistStar_In_Text(_In_ CONST std::wstring& wsText, _Out_ StarFixedMap* pFixedMap)
{
	CONST static std::vector<StarFixedMap> Vec = 
	{
		//{ L"小花仙", 139, { L"百花谷七"} },
		//{ L"翻天鼠", 139, { L"绝仙阵"} },
		//{ L"恶灵鬼王", 139, { L"绝人阵"} },
		/*{ L"地佐星", 111, { L"百花谷六", L"百花谷七"} },
		{ L"地佑星", 114, { L"百花谷七", L"绝人阵" } },
		{ L"地灵星", 117, { L"绝人阵", L"绝仙阵" } },
		{ L"地兽星", 120, { L"绝仙阵", L"海底迷宫",L"昆仑云海" } },
		{ L"地微星", 122, { L"海底迷宫", L"昆仑云海",L"雪域冰原" } },
		{ L"地慧星", 124, { L"昆仑云海", L"雪域冰原",L"迷境花树" } },
		{ L"地暴星", 126, { L"雪域冰原", L"迷境花树",L"水云间" } },
		{ L"地默星", 128, { L"雪域冰原", L"迷境花树",L"水云间",L"热砂荒漠" } },*/
		/*{ L"地猖星", 130, { L"雪域冰原", L"迷境花树",L"水云间",L"热砂荒漠",L"方丈岛" } },
		{ L"地狂星", 132, { L"雪域冰原", L"迷境花树",L"水云间",L"热砂荒漠",L"方丈岛" } },
		{ L"地飞星", 134, { L"雪域冰原", L"迷境花树",L"水云间",L"热砂荒漠",L"方丈岛" } },
		{ L"地走星", 136, { L"迷境花树", L"水云间",L"热砂荒漠",L"方丈岛",L"断魂窟" } },
		{ L"地巧星", 138, { L"迷境花树", L"水云间",L"热砂荒漠",L"方丈岛",L"断魂窟" } },
		{ L"地明星", 140, { L"迷境花树", L"水云间",L"热砂荒漠",L"方丈岛",L"断魂窟" } },
		{ L"地进星", 142, { L"迷境花树", L"水云间",L"热砂荒漠",L"方丈岛",L"断魂窟" } },
		{ L"地退星", 144, { L"迷境花树", L"水云间",L"热砂荒漠",L"方丈岛",L"断魂窟" } },*/
		/*{ L"地满星", 146, { L"迷境花树", L"水云间",L"热砂荒漠",L"方丈岛",L"断魂窟" } },
		{ L"地遂星", 148, { L"迷境花树", L"水云间",L"热砂荒漠",L"方丈岛",L"断魂窟" } },*/
	};


	CONST auto itr = std::find_if(Vec.begin(), Vec.end(), [wsText](CONST StarFixedMap& itm) { return wsText.find(itm.wsNpcName) != std::wstring::npos; });
	if (itr != Vec.end() && pFixedMap != nullptr)
	{
		*pFixedMap = *itr;
	}
	

	return itr != Vec.end();
}

std::wstring CResText::GetChannelName(_In_ DWORD dwChannel)
{
	CONST static std::vector<std::wstring> Vec = 
	{
		 L"十", L"一", L"二", L"三", L"四", L"五", L"六", L"七", L"八", L"九"
	};


	if (dwChannel == 0 || dwChannel > 99)
	{
		LOG_MSG_CF(L"无效的频道:[%d]", dwChannel);
		return L"一线";
	}


	return ConvertChannelNumberToText(dwChannel) + L"线";
}

BOOL CResText::IsExistTalkMenuDlg(_Out_ CGameUi* pTalkMenuDlg)
{
	CGameUi TalkMenuDlg(0);
	if (CGameUi::GetRoot().FindChildGameUi(L"TalkMenuDlg]", &TalkMenuDlg, FALSE) && TalkMenuDlg.GetMenuId() != 0)
	{
		if (pTalkMenuDlg != nullptr)
		{
			*pTalkMenuDlg = TalkMenuDlg;
		}


		return TRUE;
	}
	return FALSE;
}

BOOL CResText::GetResCatchPet(_In_ CONST std::wstring& wsMapName, _Out_ ResCatchPet* pResCatchPet)
{
	CONST static std::vector<ResCatchPet> Vec;
	CONST auto itr = std::find_if(Vec.begin(), Vec.end(), [wsMapName](CONST ResCatchPet& itm) { return itm.wsMapName == wsMapName; });
	if (itr != Vec.end() && pResCatchPet != nullptr)
	{
		*pResCatchPet = *itr;
	}


	return itr != Vec.end();
}


BOOL CResText::GetResStarPoint(_In_ CONST std::wstring& wsTargetMapName, _Out_ std::vector<Point>& VecPoint)
{
	struct ResMapPoint
	{
		std::wstring wsMapName;
		std::vector<Point> VecPos;
	};


	CONST static std::map<std::wstring,std::vector<Point>> Vec = 
	{
		{ L"迷境花树", { Point(37,29),Point(37,160),Point(109,144),Point(109,50),Point(181,50),Point(181,165),Point(253,190),Point(253,31), } },
		{ L"雪域冰原", { Point(37,31),Point(37,185),Point(109,185),Point(109,31),Point(181,31),Point(181,185),Point(253,185),Point(253,31), } },
		{ L"昆仑云海", { Point(37,31),Point(37,166),Point(109,180),Point(109,31),Point(181,31),Point(181,180),Point(253,185),Point(253,31), } },
		{ L"海底迷宫", { Point(37,40),Point(37,260),Point(109,260),Point(109,40),Point(181,40),Point(181,259),Point(237,250),Point(237,40), } },
		{ L"天绝阵", { Point(38,31),Point(37,200),Point(109,200),Point(109,31),Point(153,31),Point(153,193), } },
		{ L"地绝阵", { Point(37,40),Point(37,155),Point(109,155),Point(109,40),Point(181,40),Point(181,155),Point(205,138),Point(205,40), } },
		{ L"绝仙阵", { Point(37,40),Point(37,159),Point(109,159),Point(109,40),Point(181,40),Point(181,159), } },
		{ L"绝人阵", { Point(37,31),Point(37,194),Point(109,190),Point(109,31),Point(181,31),Point(181,194), } },
		{ L"百花谷七", { Point(39,29),Point(37,220),Point(109,220),Point(109,31),Point(150,31),Point(150,220), } },
		{ L"百花谷六", { Point(37,31),Point(37,206),Point(109,206),Point(109,31),Point(181,31),Point(181,206), } },
		{ L"百花谷五", { Point(37,35),Point(37,185),Point(109,185),Point(109,31),Point(181,31),Point(181,185),Point(253,176),Point(253,31), } },
		{ L"百花谷四", { Point(37,31),Point(37,270),Point(109,270),Point(109,31),Point(181,31),Point(181,271), } },
		{ L"百花谷三", { Point(181,265),Point(181,31),Point(109,31),Point(109,265),Point(37,265),Point(37,31), } },
		{ L"百花谷二", { Point(37,30),Point(37,185),Point(109,185),Point(109,31),Point(181,31),Point(181,196), } },
		{ L"百花谷一", { Point(37,185),Point(37,31),Point(109,31),Point(109,185),Point(181,185),Point(181,31),Point(253,31),Point(253,185), } },
		{ L"蓬莱岛", { Point(37,40),Point(37,305),Point(109,305),Point(109,31),Point(181,31),Point(181,305),Point(253,305),Point(253,31),Point(325,31),Point(325,305),Point(389,305),Point(389,31), } },
		{ L"五龙窟五层", { Point(37,87),Point(145,25),Point(95,95),Point(175,50), } },
		{ L"五龙窟四层", { Point(37,94),Point(133,40),Point(162,72),Point(82,121), } },
		{ L"五龙窟三层", { Point(205,67),Point(107,133),Point(60,133),Point(154,40), } },
		{ L"五龙窟二层", { Point(37,110),Point(155,50),Point(195,75),Point(73,128), } },
		{ L"五龙窟一层", { Point(48,115),Point(180,36),Point(231,65),Point(120,137), } },
		{ L"轩辕坟三层", { Point(44,86),Point(128,31),Point(93,115),Point(172,60), } },
		{ L"轩辕坟二层", { Point(47,92),Point(130,36),Point(164,75),Point(85,112), } },
		{ L"轩辕坟一层", { Point(39,68),Point(100,33),Point(128,66),Point(74,83), } },
		{ L"轩辕庙", { Point(37,31),Point(37,230),Point(109,230),Point(109,31),Point(181,31),Point(181,230),Point(253,230),Point(253,31),Point(320,31),Point(320,230), } },
		{ L"桃柳林", { Point(37,31),Point(37,110),Point(109,110),Point(109,31),Point(181,31),Point(181,97), } },
		{ L"官道北", { Point(37,31),Point(37,170),Point(109,170),Point(109,31),Point(181,31),Point(181,170), } },
		{ L"官道南", { Point(37,37),Point(37,215),Point(109,215),Point(109,40),Point(181,40),Point(181,217), } },
		{ L"揽仙镇外", { Point(37,150),Point(37,31),Point(109,31),Point(109,155), } },
		{ L"卧龙坡", { Point(37,176),Point(37,40),Point(109,40),Point(109,160),Point(168,158),Point(168,40), } },
		{ L"东昆仑", { Point(37,116),Point(37,260),Point(109,290),Point(109,31),Point(181,31),Point(181,290),Point(230,283),Point(230,224), } },
		{ L"十里坡", { Point(52,31),Point(37,300),Point(109,350),Point(109,95),Point(181,90),Point(181,350),Point(286,278),Point(286,31), } },
		{ L"乾元山", { Point(37,136),Point(37,230),Point(109,230),Point(109,40),Point(181,40),Point(181,230),Point(253,230),Point(253,40),Point(325,40),Point(315,221), } },
		{ L"五龙山", { Point(37,91),Point(37,300),Point(109,300),Point(109,40),Point(181,40),Point(181,300), } },
		{ L"终南山", { Point(39,40),Point(39,400),Point(111,400),Point(111,40), } },
		{ L"凤凰山", { Point(37,300),Point(109,300),Point(109,90),Point(181,90),Point(181,300),Point(253,300),Point(253,90),Point(304,48), } },
		{ L"骷髅山", { Point(37,124),Point(37,275),Point(109,275),Point(109,60),Point(181,60),Point(181,263), } },
		{ L"幽冥涧", { Point(440,120),Point(117,28),Point(45,65),Point(376,173), } },
		{ L"北海沙滩", { Point(67,40),Point(37,86),Point(37,135),Point(109,135),Point(109,40),Point(150,40),Point(150,145) } },
	};


	auto itr = Vec.find(wsTargetMapName);
	if (itr == Vec.end())
		return FALSE;


	VecPoint = itr->second;
	return TRUE;
}

std::wstring CResText::ConvertChannelNumberToText(_In_ CONST DWORD dwChannel)
{
	CONST static std::vector<std::wstring> Vec =
	{
		 L"一", L"二", L"三", L"四", L"五", L"六", L"七", L"八", L"九",L"十"
	};


	if (dwChannel <= 10)
		return Vec.at(dwChannel - 1);
	else if (dwChannel < 20)
		return L"十" + Vec.at(dwChannel % 10 - 1);
	else if (dwChannel % 10 == 0)
		return Vec.at(dwChannel / 10 - 1) + L"十";

	return Vec.at(dwChannel / 10 - 1) + L"十" + Vec.at(dwChannel % 10 - 1);
}
