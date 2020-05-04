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
		//{ L"�ؿ���", 60 }, { L"��ɷ��", 63 }, { L"������", 66 }, { L"�ؽ���", 69 },// { L"������", 72 },
		//{ L"������", 75 }, { L"��Ӣ��", 78 }, { L"������", 81 }, { L"������", 84 }, { L"������", 87 },
		//{ L"������", 90 }, { L"�ر���", 93 }, { L"������", 96 }, { L"��ǿ��", 99 }, { L"�ذ���", 102 },
		//{ L"�ظ���", 105 }, { L"�ػ���", 108 }, 
		/*{ L"������", 111 }, { L"������", 114 }, { L"������", 117 },
		{ L"������", 120 }, { L"��΢��", 122 }, { L"�ػ���", 124 }, { L"�ر���", 126 }, { L"��Ĭ��", 128 },
		{ L"�ز���", 130 }, { L"�ؿ���", 132 }, { L"�ط���", 134 }, { L"������", 139 },	{ L"������", 139 },
		{ L"������", 140 },	{ L"�ؽ���", 142 },	{ L"������", 144 },	{ L"������", 146 },	{ L"������", 148 },
		{ L"������", 150 },	{ L"������", 152 },	{ L"������", 154 },	{ L"������", 156 },	{ L"�ؿ���", 158 },
		{ L"������", 160 },	{ L"�ؽ���", 162 },	{ L"������", 164 },*/
		{ L"��ȸ����", 140 },{ L"�׹Ǿ�", 130 },	{ L"��ɲ��", 120 }, { L"ҹ����", 110 }, { L"ţħ��", 100 },
		{ L"�ٻ���", 90}, { L"������", 80}, { L"��������", 70}, { L"�����⬾�", 60}, { L"�����ʨ��", 50}, 
		{ L"������ܾ�", 40},
		// ��������
		//{ L"С����", 139}, { L"������", 139},{ L"�������", 139},
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
		//{ L"С����", 139, { L"�ٻ�����"} },
		//{ L"������", 139, { L"������"} },
		//{ L"�������", 139, { L"������"} },
		/*{ L"������", 111, { L"�ٻ�����", L"�ٻ�����"} },
		{ L"������", 114, { L"�ٻ�����", L"������" } },
		{ L"������", 117, { L"������", L"������" } },
		{ L"������", 120, { L"������", L"�����Թ�",L"�����ƺ�" } },
		{ L"��΢��", 122, { L"�����Թ�", L"�����ƺ�",L"ѩ���ԭ" } },
		{ L"�ػ���", 124, { L"�����ƺ�", L"ѩ���ԭ",L"�Ծ�����" } },
		{ L"�ر���", 126, { L"ѩ���ԭ", L"�Ծ�����",L"ˮ�Ƽ�" } },
		{ L"��Ĭ��", 128, { L"ѩ���ԭ", L"�Ծ�����",L"ˮ�Ƽ�",L"��ɰ��Į" } },*/
		/*{ L"�ز���", 130, { L"ѩ���ԭ", L"�Ծ�����",L"ˮ�Ƽ�",L"��ɰ��Į",L"���ɵ�" } },
		{ L"�ؿ���", 132, { L"ѩ���ԭ", L"�Ծ�����",L"ˮ�Ƽ�",L"��ɰ��Į",L"���ɵ�" } },
		{ L"�ط���", 134, { L"ѩ���ԭ", L"�Ծ�����",L"ˮ�Ƽ�",L"��ɰ��Į",L"���ɵ�" } },
		{ L"������", 136, { L"�Ծ�����", L"ˮ�Ƽ�",L"��ɰ��Į",L"���ɵ�",L"�ϻ��" } },
		{ L"������", 138, { L"�Ծ�����", L"ˮ�Ƽ�",L"��ɰ��Į",L"���ɵ�",L"�ϻ��" } },
		{ L"������", 140, { L"�Ծ�����", L"ˮ�Ƽ�",L"��ɰ��Į",L"���ɵ�",L"�ϻ��" } },
		{ L"�ؽ���", 142, { L"�Ծ�����", L"ˮ�Ƽ�",L"��ɰ��Į",L"���ɵ�",L"�ϻ��" } },
		{ L"������", 144, { L"�Ծ�����", L"ˮ�Ƽ�",L"��ɰ��Į",L"���ɵ�",L"�ϻ��" } },*/
		/*{ L"������", 146, { L"�Ծ�����", L"ˮ�Ƽ�",L"��ɰ��Į",L"���ɵ�",L"�ϻ��" } },
		{ L"������", 148, { L"�Ծ�����", L"ˮ�Ƽ�",L"��ɰ��Į",L"���ɵ�",L"�ϻ��" } },*/
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
		 L"ʮ", L"һ", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��"
	};


	if (dwChannel == 0 || dwChannel > 99)
	{
		LOG_MSG_CF(L"��Ч��Ƶ��:[%d]", dwChannel);
		return L"һ��";
	}


	return ConvertChannelNumberToText(dwChannel) + L"��";
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
		{ L"�Ծ�����", { Point(37,29),Point(37,160),Point(109,144),Point(109,50),Point(181,50),Point(181,165),Point(253,190),Point(253,31), } },
		{ L"ѩ���ԭ", { Point(37,31),Point(37,185),Point(109,185),Point(109,31),Point(181,31),Point(181,185),Point(253,185),Point(253,31), } },
		{ L"�����ƺ�", { Point(37,31),Point(37,166),Point(109,180),Point(109,31),Point(181,31),Point(181,180),Point(253,185),Point(253,31), } },
		{ L"�����Թ�", { Point(37,40),Point(37,260),Point(109,260),Point(109,40),Point(181,40),Point(181,259),Point(237,250),Point(237,40), } },
		{ L"�����", { Point(38,31),Point(37,200),Point(109,200),Point(109,31),Point(153,31),Point(153,193), } },
		{ L"�ؾ���", { Point(37,40),Point(37,155),Point(109,155),Point(109,40),Point(181,40),Point(181,155),Point(205,138),Point(205,40), } },
		{ L"������", { Point(37,40),Point(37,159),Point(109,159),Point(109,40),Point(181,40),Point(181,159), } },
		{ L"������", { Point(37,31),Point(37,194),Point(109,190),Point(109,31),Point(181,31),Point(181,194), } },
		{ L"�ٻ�����", { Point(39,29),Point(37,220),Point(109,220),Point(109,31),Point(150,31),Point(150,220), } },
		{ L"�ٻ�����", { Point(37,31),Point(37,206),Point(109,206),Point(109,31),Point(181,31),Point(181,206), } },
		{ L"�ٻ�����", { Point(37,35),Point(37,185),Point(109,185),Point(109,31),Point(181,31),Point(181,185),Point(253,176),Point(253,31), } },
		{ L"�ٻ�����", { Point(37,31),Point(37,270),Point(109,270),Point(109,31),Point(181,31),Point(181,271), } },
		{ L"�ٻ�����", { Point(181,265),Point(181,31),Point(109,31),Point(109,265),Point(37,265),Point(37,31), } },
		{ L"�ٻ��ȶ�", { Point(37,30),Point(37,185),Point(109,185),Point(109,31),Point(181,31),Point(181,196), } },
		{ L"�ٻ���һ", { Point(37,185),Point(37,31),Point(109,31),Point(109,185),Point(181,185),Point(181,31),Point(253,31),Point(253,185), } },
		{ L"������", { Point(37,40),Point(37,305),Point(109,305),Point(109,31),Point(181,31),Point(181,305),Point(253,305),Point(253,31),Point(325,31),Point(325,305),Point(389,305),Point(389,31), } },
		{ L"���������", { Point(37,87),Point(145,25),Point(95,95),Point(175,50), } },
		{ L"�������Ĳ�", { Point(37,94),Point(133,40),Point(162,72),Point(82,121), } },
		{ L"����������", { Point(205,67),Point(107,133),Point(60,133),Point(154,40), } },
		{ L"�����߶���", { Point(37,110),Point(155,50),Point(195,75),Point(73,128), } },
		{ L"������һ��", { Point(48,115),Point(180,36),Point(231,65),Point(120,137), } },
		{ L"��ԯ������", { Point(44,86),Point(128,31),Point(93,115),Point(172,60), } },
		{ L"��ԯ�ض���", { Point(47,92),Point(130,36),Point(164,75),Point(85,112), } },
		{ L"��ԯ��һ��", { Point(39,68),Point(100,33),Point(128,66),Point(74,83), } },
		{ L"��ԯ��", { Point(37,31),Point(37,230),Point(109,230),Point(109,31),Point(181,31),Point(181,230),Point(253,230),Point(253,31),Point(320,31),Point(320,230), } },
		{ L"������", { Point(37,31),Point(37,110),Point(109,110),Point(109,31),Point(181,31),Point(181,97), } },
		{ L"�ٵ���", { Point(37,31),Point(37,170),Point(109,170),Point(109,31),Point(181,31),Point(181,170), } },
		{ L"�ٵ���", { Point(37,37),Point(37,215),Point(109,215),Point(109,40),Point(181,40),Point(181,217), } },
		{ L"��������", { Point(37,150),Point(37,31),Point(109,31),Point(109,155), } },
		{ L"������", { Point(37,176),Point(37,40),Point(109,40),Point(109,160),Point(168,158),Point(168,40), } },
		{ L"������", { Point(37,116),Point(37,260),Point(109,290),Point(109,31),Point(181,31),Point(181,290),Point(230,283),Point(230,224), } },
		{ L"ʮ����", { Point(52,31),Point(37,300),Point(109,350),Point(109,95),Point(181,90),Point(181,350),Point(286,278),Point(286,31), } },
		{ L"ǬԪɽ", { Point(37,136),Point(37,230),Point(109,230),Point(109,40),Point(181,40),Point(181,230),Point(253,230),Point(253,40),Point(325,40),Point(315,221), } },
		{ L"����ɽ", { Point(37,91),Point(37,300),Point(109,300),Point(109,40),Point(181,40),Point(181,300), } },
		{ L"����ɽ", { Point(39,40),Point(39,400),Point(111,400),Point(111,40), } },
		{ L"���ɽ", { Point(37,300),Point(109,300),Point(109,90),Point(181,90),Point(181,300),Point(253,300),Point(253,90),Point(304,48), } },
		{ L"����ɽ", { Point(37,124),Point(37,275),Point(109,275),Point(109,60),Point(181,60),Point(181,263), } },
		{ L"��ڤ��", { Point(440,120),Point(117,28),Point(45,65),Point(376,173), } },
		{ L"����ɳ̲", { Point(67,40),Point(37,86),Point(37,135),Point(109,135),Point(109,40),Point(150,40),Point(150,145) } },
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
		 L"һ", L"��", L"��", L"��", L"��", L"��", L"��", L"��", L"��",L"ʮ"
	};


	if (dwChannel <= 10)
		return Vec.at(dwChannel - 1);
	else if (dwChannel < 20)
		return L"ʮ" + Vec.at(dwChannel % 10 - 1);
	else if (dwChannel % 10 == 0)
		return Vec.at(dwChannel / 10 - 1) + L"ʮ";

	return Vec.at(dwChannel / 10 - 1) + L"ʮ" + Vec.at(dwChannel % 10 - 1);
}
