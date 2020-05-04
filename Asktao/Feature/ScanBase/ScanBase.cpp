#include "ScanBase.h"
#include <ProcessLib/Memory/SearchBinary.h>
#include <LogLib/Log.h>

#ifdef _PRIVATESERVER
#define ASKTAO_MODULENAME L"asktao.mod"
#else
#define ASKTAO_MODULENAME L"asktao_jd.mod"
#endif // _PRIVATESERVER


#define _SELF L"ScanBase.cpp"
VOID CScanBase::Run() CONST
{
	DWORD dwAddr = 0, dwCALL = 0, dwBase = 0;
	libTools::CSearchBinary SearchBinary;


	dwBase = SearchBinary.FindBase("8B??????FF??33??85", 0x886DEE - 0x886DE1, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 人物属性基址           0x%X", dwBase);

	dwBase = SearchBinary.FindBase("6A00??8D??????????E8????????6A00??8D", 0x7D4789 - 0x7D478C, 0x2, 0x2, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 人物属性偏移1           0x%X", dwBase);

	dwBase = SearchBinary.FindBase("6A00??8D??????????E8????????6A00??8D", 0x7D4789 - 0x7D479A, 0x2, 0x2, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 人物属性偏移2           0x%X", dwBase);

	dwBase = SearchBinary.FindBase("6A00??8D??????????E8????????6A00??8D", 0x7D4789 - 0x7D47AA, 0x2, 0x2, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 人物属性偏移3           0x%X", dwBase);

	dwAddr = SearchBinary.FindAddr("81??2444000074??E8????????F6", 0x7F0175 - 0x7F0140, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 发包CALL               0x%X", dwAddr);

	dwBase = SearchBinary.FindBase("85??89??24??????85??????8b??8B", 0x52867D - 0x528653, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 背包遍历基址           0x%X", dwBase);

	dwBase = SearchBinary.FindBase("68????????8B??E8????????8B????????????E8????????8D??24??C6", 0x7AD30A - 0x7AD316, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 任务遍历基址           0x%X", dwBase);

	dwBase = SearchBinary.FindBase("680001000068????????E8????????A1", 0x6CCD09 - 0x6CCD0E, 0x1, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 是否战斗中基址           0x%X", dwBase);

	dwBase = SearchBinary.FindBase("6A016A0068????????6A006A00????E8", 0x9D18E9 - 0x9D18E1, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 走路基址              0x%X", dwBase);

	dwCALL = SearchBinary.FindCALL("6A016A0068????????6A006A00????E8", 0x9C3316 - 0x9C3325, 0x1, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 走路CALL              0x%X", dwCALL);

	dwBase = SearchBinary.FindBase("0F84????????8B??????????8B????????????E8????????8B??85", 0x716F8B - 0x716F97, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define Npc遍历基址              0x%X", dwBase);

	dwBase = SearchBinary.FindBase("68????????B9????????E8????????68????????E8????????83C4048B", 0x5CD094 - 0x5CD099, 0x1, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 人物系统属性基址   		0x%X", dwBase);

	dwBase = SearchBinary.FindBase("8B??????????6A00??E8????????8D", 0x9BB0E9 - 0x9BB0E9, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 换线基址              0x%X", dwBase);

	dwCALL = SearchBinary.FindCALL("8B??????????6A00??E8????????8D", 0x9BB0E9 - 0x9BB0F2, 0x1, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 换线CALL              0x%X", dwCALL);

	dwBase = SearchBinary.FindBase("B90800000033C0F3??7509A1", 0x64B59E - 0x64B5A9, 0x1, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define UI遍历基址              0x%X", dwBase);

	dwBase = SearchBinary.FindBase("E9????????8B??????00008B??????00008B??24", 0x46EB49 - 0x46EB4E, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define UI遍历偏移              0x%X", dwBase);

	dwCALL = SearchBinary.FindCALL("6A0068????????8BCEE8????????5E59C2", 0x44D199 - 0x44D1A2, 0x1, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define UI点击CALL              0x%X", dwCALL);

	dwBase = SearchBinary.FindBase("C3A1????????85C07409", 0x80E27F - 0x080E280, 0x1, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 发包全局互斥基址         0x%X", dwBase);

	dwBase = SearchBinary.FindBase("6A00??7D??E8????????83C408", 0x53591C - 0x53592B, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 寻路基址		         0x%X", dwBase);

	dwCALL = SearchBinary.FindCALL("6A00??7D??E8????????83C408", 0x53591C - 0x535931, 0x1, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 寻路CALL1              0x%X", dwCALL);

	dwCALL = SearchBinary.FindCALL("6A00??7D??E8????????83C408", 0x53591C - 0x53593C, 0x1, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 寻路CALL2              0x%X", dwCALL);

	dwBase = SearchBinary.FindBase("E8????????6A006A0068????????68??????????E8", 0x896089 - 0x8960C2, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 聊天窗口基址         0x%X", dwBase);

	dwBase = SearchBinary.FindBase("FF15????????8B????8B????8B??0C", 0x635668 - 0x63566E, 0x2, 0x0, ASKTAO_MODULENAME, 0xFF);
	LOG_C_D(L"#define 聊天窗口偏移		         0x%X", dwBase);

	dwBase = SearchBinary.FindBase("3B??7E??EB078D????????????8B", 0x6AF963 - 0x6AF970, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 怪物遍历偏移		         0x%X", dwBase);

	dwCALL = SearchBinary.FindCALL("CC6A0068????????6A01E8", 0x9FA21F - 0x9FA229, 0x1, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 人物补充状态CALL              0x%X", dwCALL);

	dwCALL = SearchBinary.FindCALL("CC6A0068????????6A01E8", 0x9FA21F - 0x9FA229, 0x1, 0x2, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 宠物补充状态CALL              0x%X", dwCALL);

	dwBase = SearchBinary.FindBase("E8????????33??3B??89??????0F??????????8B??????0000", 0xA1C68F - 0xA1C6A2, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 宠物遍历偏移		         0x%X", dwBase);

	dwBase = SearchBinary.FindBase("E8????????33??3B??89??????0F??????????8B??????0000", 0xA1C68F - 0xA1C6CD, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 宠物遍历基址		         0x%X", dwBase);

	dwBase = SearchBinary.FindBase("8B??????????E8????????68????????8D????????????8B", 0xA2A43E - 0xA2A43E, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 宠物技能基址		         0x%X", dwBase);

	dwBase = SearchBinary.FindBase("83??????????0189??????0F86", 0x7E89B5 - 0x7E89B5, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define Npc类型偏移		         0x%X", dwBase);

	dwBase = SearchBinary.FindBase("8B??????????83C40885??7405E8????????8B", 0x81AF73 - 0x81AF73, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 周围所有对象基址		         0x%X", dwBase);

	dwBase = SearchBinary.FindBase("85C00F84????????8B????????????8B??81", 0x72B580 - 0x72B588, 0x2, 0x0, ASKTAO_MODULENAME);
	LOG_C_D(L"#define 周围所有对象偏移		         0x%X", dwBase);
}
