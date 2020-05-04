#include "FileCache.h"
#include <filesystem> // C++ 17
#include <iostream>
#include <CharacterLib/Character.h>
#include <FileLib/File.h>
#include <AlgroithmLib/Encrypt/CRC32.h>
#include <AlgroithmLib/Encrypt/RC4.h>
#include <ProcessLib/Common/ResHandleManager.h>

CFileCache& CFileCache::GetInstance()
{
	static CFileCache Instance;
	return Instance;
}

BOOL CFileCache::Initialize()
{
	using DirectoryPath = std::experimental::filesystem::v1::path;
	using DirectoryItr = std::experimental::filesystem::v1::directory_iterator;

	std::lock_guard<CFileCache> tmpLock(*this);


	_VecFileCache.clear();
	DirectoryPath DirPath(libTools::CCharacter::MakeCurrentPath(L"\\File\\"));
	for (CONST auto& File : DirectoryItr(DirPath))
	{
		SingleDownloadFile FileContent;
		FileContent.wsFileName = File.path().filename().wstring();


		LPVOID lpFileContent	= nullptr;
		SIZE_T uFileSize		= 0;
		if (!libTools::CFile::ReadFileContent(File.path().wstring(), lpFileContent, uFileSize))
		{
			std::wcout << L"读取文件[" << FileContent.wsFileName.c_str() << L"]失败!";
			return FALSE;
		}


		SetResDeleter(lpFileContent, [](LPVOID& p) { ::VirtualFree(p, 0, MEM_RELEASE); });


		FileContent.uFileSize = uFileSize;
		if (libTools::CCharacter::MakeTextToLower(FileContent.wsFileName) == L"asktao.dll")
		{
			libTools::CRC4 Rc4(RC4_KEY, strlen(RC4_KEY));
			FileContent.FileContentPtr = Rc4.GetEncryptText(static_cast<CONST CHAR*>(lpFileContent), uFileSize);
		}
		else
		{
			FileContent.FileContentPtr = std::shared_ptr<CHAR>(new CHAR[uFileSize], [](CHAR* p) {delete[] p; });
			memcpy(FileContent.FileContentPtr.get(), lpFileContent, uFileSize);
		}
		

		FileContent.dwFileCRC = libTools::CCRC32::GetCRC32(reinterpret_cast<CHAR*>(lpFileContent), uFileSize);
		_VecFileCache.insert(std::make_pair(FileContent.wsFileName, FileContent));
	}
	
	return !_VecFileCache.empty();
}

BOOL CFileCache::DownLoadFile(_In_ CONST std::vector<std::wstring>& VecFileName, _Out_ libTools::CSocketBuffer& SocketBuffer)
{
	std::lock_guard<CFileCache> tmpLock(*this);

	DownloadFileContent VecFileContent;
	for (auto& itm : VecFileName)
	{
		auto itr = _VecFileCache.find(itm);
		if (itr == _VecFileCache.end())
		{
			SocketBuffer.InitializeHead(em_Sock_Msg_Error);
			SocketBuffer << libTools::CCharacter::MakeFormatText(L"不存在文件[%s]", itm.c_str());
			return TRUE;
		}


		VecFileContent.Vec.push_back(itr->second);
	}

	SocketBuffer.AddStruct(VecFileContent);
	return TRUE;
}

BOOL CFileCache::DownloadVecStarMapPoint(_In_ CONST std::wstring& wsMapName, _Out_ libTools::CSocketBuffer& SocketBuffer)
{
	struct StarMapPoint
	{
		std::wstring		wsMapName;
		std::vector<Point>	VecPoint;
	};


	CONST static std::vector<StarMapPoint> Vec = 
	{
		{ L"十里坡", 
			{
				Point(318, 9), Point(231, 205), Point(295, 215), Point(48, 244), Point(114, 364),
				Point(17, 268),Point(12, 201),  Point(221, 157), Point(94, 92),  Point(137, 174),
				Point(66, 13),Point(211, 114)
			}
		},
		{ L"风月谷",
			{
				Point(149,199), Point(204,84), Point(168,21), Point(106,16), Point(150,103), Point(93,106),
				Point(120,69),  Point(29,49),  Point(87,170), Point(34,145), Point(61,187),  Point(116,185)
			}
		},
		{ L"五龙窟一层",
			{
				Point(275,63), Point(229,90),  Point(176,110), Point(131,144), Point(66,133),
				Point(21,107), Point(100,104), Point(157,123), Point(108,73),  Point(180,56),
				Point(223,61), Point(183,36)
			}
		},
		{ L"五龙窟二层",
			{
				Point(145,27),Point(112,71),Point(195,70),Point(162,89),Point(104,88),
				Point(109,131),Point(150,110),Point(63,125),Point(65,73),Point(39,71),
				Point(117,51)
			}
		},
		{ L"五龙窟三层",
			{
				Point(16,92),Point(124,142),Point(143,90),Point(186,109),Point(117,129),
				Point(106,73),Point(49,96),Point(139,32),Point(125,54),Point(210,71),
				Point(170,39)
			}
		},
		{ L"五龙窟四层",
			{
				Point(187,75),Point(73,117),Point(144,86),Point(161,56),Point(121,52),
				Point(117,37),Point(39,74),Point(88,81),Point(48,101)
			}
		},
		{ L"五龙窟五层",
			{
				Point(8,62),Point(106,39),Point(92,65),Point(111,90),Point(136,25),Point(168,69)
			}
		},
		{ L"五龙山",
			{
				Point(98,37), Point(232,118), Point(178,209), Point(67,253), Point(44,309),
				Point(183,284), Point(171,188), Point(207,110), Point(180,80)
			}
		},
		{ L"五龙山",
			{
				Point(61,280), Point(194,311), Point(216,109), Point(183,77), Point(121,112),
				Point(36,108), Point(53,157), Point(65,254), 
			}
		},
		{ L"五龙山",
			{
				Point(181,188), Point(210,110), Point(159,57), Point(106,46), Point(52,132),
				Point(24,202), Point(35,309), Point(168,273), Point(155,114)
			}
		},
		{ L"幽冥涧",
			{
				Point(445,123), Point(335,95), Point(351,63), Point(209,34), Point(200,55),
				Point(300,80), Point(350,137), Point(100,40), Point(14,65), Point(60,99),
				Point(136,104), Point(192,153), Point(211,121), Point(159,91), Point(307,166),
				Point(312,148), Point(324,188), Point(357,199)
			}
		},
		{ L"终南山",
			{
				Point(128,56), Point(113,117), Point(75,167), Point(131,201), Point(112,288),
				Point(77,336), Point(134,360), Point(131,298), Point(105,413), Point(34,407),
				Point(32,295), Point(80,291), Point(36,275), Point(82,239), Point(21,212),
				Point(55,169), Point(6,131), Point(63,117)
			}
		},
		{ L"凤凰山",
			{
				Point(163,140), Point(245,147), Point(247,170), Point(185,168), Point(171,252),
				Point(209,216), Point(233,219), Point(219,282), Point(302,319), Point(198,279),
				Point(112,314), Point(136,284), Point(11,320),  Point(76,231),  Point(141,259),
				Point(145,222), Point(106,216), Point(74,145),  Point(131,133), Point(62,87),
				Point(122,82),  Point(180,95)
			}
		},
		{ L"乾元山",
			{
				Point(319,27), Point(132,37), Point(176,71), Point(226,46), Point(141,101),
				Point(50,136), Point(197,154),Point(289,147),Point(211,176),Point(82,173),
				Point(88,155), Point(22,163), Point(20,237), Point(75,188), Point(147,236),
				Point(264,237),Point(325,199),Point(150,206),Point(303,175),Point(343,111),
				Point(157,127)
			}
		},
		{ L"骷髅山",
			{
				Point(181,59), Point(134,125), Point(182,172), Point(132,116), Point(139,192),
				Point(156,200),Point(142,232), Point(199,234), Point(183,266), Point(83,235),
				Point(120,276),Point(64,285),  Point(6,282),   Point(10,233),  Point(114,213),
				Point(35,191), Point(69,177),  Point(26,134),  Point(110,127)
			}
		},
		{ L"昆仑云海",
			{
				Point(243,199), Point(199,184), Point(278,124), Point(192,117), Point(266,56),
				Point(185,18),  Point(145,45),  Point(88,13),   Point(16,61),   Point(18,19),
				Point(109,54),  Point(18,81),   Point(97,91),   Point(229,63),  Point(226,140),
				Point(154,142), Point(79,145),  Point(158,96),  Point(70,107),  Point(1,145),
				Point(111,196), Point(182,157)
			}
		},
		{ L"迷境花树",
			{
				Point(15,20),   Point(173,98),  Point(172,81),  Point(95,114),  Point(68,90),
				Point(127,119), Point(137,46),  Point(163,50),  Point(266,60),  Point(208,108),
				Point(257,140), Point(267,199), Point(70,131),  Point(31,166),  Point(21,88),
				Point(35,86)
			}
		},
		{ L"百花谷六",
			{
				Point(16,19),   Point(16,219),  Point(207,219),  Point(207,194),  Point(49,194),
				Point(207,19), Point(207,169),  Point(174,169),  Point(174,44),  Point(82,44),
				Point(82,144), Point(153,144), Point(153,69),  Point(115,69),  Point(115,119)
			}
		},
		{ L"百花谷七",
			{
				Point(115,19),   Point(115,230),  Point(16, 230),  Point(16, 219),  Point(82, 219),
				Point(82, 19), Point(49, 19),  Point(49, 194),  Point(16, 194),  Point(16, 19),
			}
		},
		{ L"绝人阵",
			{
				Point(16,32), Point(16,167), Point(195,167), Point(195,32), Point(181,32),
				Point(181,157), Point(148,157), Point(148,32), Point(115,32), Point(115,157),
				Point(82,157), Point(82,32), Point(49,32), Point(49,157),
			}
		},
		{ L"绝仙阵",
			{
				Point(205,172), Point(205,31), Point(16,31), Point(16,172), Point(49,172),
				Point(49,56), Point(82,56), Point(82,172), Point(115,172), Point(115,56),
				Point(148,56), Point(148,172), Point(181,172), Point(181,56)
			}
		},
		{ L"海底迷宫",
			{
				Point(18,20), Point(18,261), Point(236,261), Point(236,20), Point(216,20),
				Point(216,245), Point(183,245), Point(183,20), Point(150,20), Point(150,245),
				Point(117,245), Point(117,20), Point(84,20), Point(84,245), Point(51,245),
				Point(51,20)
			}
		},
		{ L"雪域冰原",
			{
				Point(16,20), Point(266,20), Point(266,195), Point(16,195), Point(16,170),
				Point(247,170), Point(247,145), Point(16,145), Point(16,120), Point(247,120),
				Point(247,95), Point(16,95), Point(16,70), Point(247,70), Point(247,45),
				Point(16,45)
			}
		},
		{ L"热砂荒漠",
			{
				Point(13,17), Point(12,195), Point(40,199), Point(42,16), Point(71,15), Point(69,198),
				Point(97,197),  Point(102,13),  Point(132,17), Point(128,198), Point(157,194),  Point(161,15),
				Point(189,18), Point(188,195), Point(218,197), Point(220,17), Point(250,18), Point(248,196),
				Point(269,198), Point(270,15)
			}
		},
		{ L"热砂荒漠",
			{
				Point(15,18), Point(277,29),  Point(268,51), Point(7,40), Point(10,62),
				Point(273,71), Point(272,91), Point(8,82), Point(9,100),  Point(267,113),
				Point(272,132), Point(4,123), Point(13,145), Point(274,151), Point(273,172),
				Point(10,166), Point(15,190), Point(266,193)
			}
		},
		{ L"水云间",
			{
				Point(12,23),   Point(26,63),   Point(174,39),  Point(239,73),  Point(228,93),
				Point(161,78),  Point(58,106),  Point(14,76),   Point(28,147),  Point(154,114),
				Point(228,131), Point(276,191), Point(141,192), Point(2,195),   Point(43,155),
				Point(115,173), Point(165,136), Point(260,116), Point(265,22)
			}
		},
		{ L"断魂窟",
			{
				Point(63,121),  Point(134,118), Point(134,30),  Point(101,30),  Point(101,108),
				Point(48,85),   Point(90,35),   Point(35,24),   Point(42,47),   Point(15,45),
				Point(7,118)
			}
		}
	};


	/*struct StarRectMap
	{
		std::wstring	wsMapName;
		Point			LeftTopPos;
		Point			LeftBottomPos;
		Point			RightTopPos;
		Point			RightBottomPos;
	};

	CONST static std::vector<StarRectMap> VecRect =
	{
		{ L"蓬莱岛", Point(16,20),Point(16,315),Point(399,20), Point(399,315) },
		{ L"百花谷一", Point(16,21),Point(16,195),Point(271,30), Point(271,195) },
		{ L"百花谷二", Point(16,20),Point(16,195),Point(185,20), Point(185,196) },
		{ L"百花谷三", Point(16,20),Point(16,277),Point(182,20), Point(182,277) },
		{ L"百花谷四", Point(16,20),Point(10,277),Point(181,20), Point(181,277) },
		{ L"百花谷五", Point(16,20),Point(16,194),Point(266,20), Point(266,194) },
		{ L"百花谷六", Point(16,20),Point(16,219),Point(207,20), Point(207,219) },
		{ L"百花谷七", Point(16,20),Point(16,230),Point(150,20), Point(150,230) },
		{ L"绝人阵", Point(16,35),Point(16,171),Point(196,35), Point(196,166) },
		{ L"绝仙阵", Point(16,30),Point(16,172),Point(204,30), Point(204,172) },
		{ L"海底迷宫", Point(18,20),Point(18,261),Point(236,20), Point(236,261) },
		{ L"雪域冰原", Point(16,20),Point(16,195),Point(266,20), Point(266,195) },
		{ L"热砂荒漠", Point(16,20),Point(20,195),Point(267,20), Point(267,195) },
		{ L"方丈岛", Point(14,19),Point(26,322),Point(393,19), Point(365,322) },
	};*/


	StarMap_Tranfer Content;
	Content.emType = em_StarMap_Type::VecPoint;


	BOOL bExist = FALSE;
	for (auto& itm : Vec)
	{
		if (itm.wsMapName == wsMapName)
		{
			bExist = TRUE;
			Content.VecVec.push_back(itm.VecPoint);
		}
	}


	if (bExist)
	{
		SocketBuffer.AddStruct(Content);
		return TRUE;
	}


	SocketBuffer.InitializeHead(em_Sock_Msg_Error);
	SocketBuffer << libTools::CCharacter::MakeFormatText(L"无效的地图名:[%s]", wsMapName.c_str());
	return TRUE;
}

BOOL CFileCache::DownloadPreviewFile(_In_ CONST std::vector<std::wstring>& VecFileName, _Out_ libTools::CSocketBuffer& SocketBuffer)
{
	std::lock_guard<CFileCache> tmpLock(*this);

	PreviewFile_Tranfer VecFileContent;
	for (auto& itm : VecFileName)
	{
		auto itr = _VecFileCache.find(itm);
		if (itr == _VecFileCache.end())
		{
			SocketBuffer.InitializeHead(em_Sock_Msg_Error);
			SocketBuffer << libTools::CCharacter::MakeFormatText(L"不存在文件[%s]", itm.c_str());
			return TRUE;
		}


		PreviewFile PrFile;
		PrFile.wsFileName = itr->second.wsFileName;
		PrFile.dwFileCRC = itr->second.dwFileCRC;
		VecFileContent.Vec.push_back(PrFile);
	}

	SocketBuffer.AddStruct(VecFileContent);
	return TRUE;
}
