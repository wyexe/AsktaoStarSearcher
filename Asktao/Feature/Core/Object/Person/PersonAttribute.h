#ifndef __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_PERSON_PERSONATTRIBUTE_H__
#define __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_PERSON_PERSONATTRIBUTE_H__

#include <string>
#include <Feature/Common/GameStruct.h>

class CPersonAttribute
{
public:
	CPersonAttribute() = default;
	~CPersonAttribute() = default;

	// ��ȡ��ǰ������
	static std::wstring GetServerName();

	// ��ǰ��ɫ����·
	static CONST std::wstring GetChannelName();


	// ��ǰ��ɫ������
	static Point GetCurrentPoint();


	// ��ǰ��ɫ�ĵ�ͼ��
	static CONST std::wstring GetCurrentMapName();


	// ��ǰ��ɫ��
	static CONST std::wstring GetPlayerName();

	// �Ƿ�ս����
	static BOOL IsFighting();

	// ID
	static DWORD GetPersonId();

	// �Ƿ��ֵ��Լ��Ļغ�
	static BOOL IsSelfRound();

	// �Ƿ�Moving
	static BOOL IsMoving();
private:

};



#endif // !__ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_PERSON_PERSONATTRIBUTE_H__
