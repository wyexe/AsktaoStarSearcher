#ifndef __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_VECGAMEOBJECT_H__
#define __ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_VECGAMEOBJECT_H__

#include <string>
#include <stack>
#include <vector>
#include <queue>
#include <functional>
#include <Feature/Common/GameStruct.h>
#include <Feature/Core/Object/Item/ItemObject.h>
#include <Feature/Core/Object/Task/TaskObject.h>
#include <Feature/Core/Object/Npc/Npc.h>
#include <Feature/Core/Object/Skill/SkillObject.h>
#include <Feature/Core/Object/GameUi/GameUi.h>
#include <TimeLib/TimeCharacter.h>

class CVecGameObject
{
public:
	CVecGameObject() = default;
	~CVecGameObject() = default;


public:
	// Find Attribute Value by Key
	static BOOL FindObjectAttributeValue_By_Key(_In_ DWORD dwTreeHead, _In_ LPCSTR pszKey, _Out_ std::wstring& wsValue);

	//
	static BOOL FindPersonAttributeValue(_In_ LPCSTR pszKey, _Out_ DWORD& dwValue);

	//
	static BOOL FindPersonAttributeText(_In_ LPCSTR pszKey, _Out_ std::wstring& wsValue);

	//
	static BOOL FindPersonSystemAttributeText(_In_ LPCSTR pszKey, _Out_ std::wstring& wsValue);

	//
	static UINT GetVecBagItem(_Out_ std::vector<CItemObject>& VecItem);

	//
	static UINT GetVecTask(_Out_ std::vector<CTaskObject>& VecTask);

	//
	static UINT GetVecNpc(_Out_ std::vector<CNpc>& Vec);

	//
	static UINT GetVecGameUi(_In_ DWORD dwAddr, _Out_ std::vector<CGameUi>& Vec, _In_ BOOL IsShowHide);

	//
	static UINT GetVecChat(_In_ CONST StarChatMsgContent& LastStar, _Out_ std::vector<StarChatMsgContent>& VecContent);

	//
	static UINT GetVecMonster(_Out_ std::vector<CNpc>& VecMonster);

	//
	static UINT GetVecPet(_Out_ std::vector<CNpc>& Vec);

	//
	static UINT GetVecPetSkill(_In_ DWORD dwPetId, _Out_ std::vector<CSkillObject>& Vec);

	// 
	static UINT GetVecChatAddr(_Out_ std::stack<DWORD>& Vec, _In_ ChannelType ChatType);
private:
	//
	static DWORD GetNextNpcObject(_In_ DWORD v2);

	//
	template<typename T>
	static UINT GetVecObject(_In_ DWORD dwBase, _Out_ std::vector<T>& VecObject, _In_ std::function<DWORD(DWORD)> GetObjectPtr)
	{
		std::queue<DWORD> Vec;
		Vec.push(dwBase);

		while (!Vec.empty() && Vec.size() < 100)
		{
			DWORD dwAddr = Vec.front();
			Vec.pop();

			if (RB(dwAddr + 0x15) == 0)
			{
				Vec.push(RD(dwAddr + 0x0));
				Vec.push(RD(dwAddr + 0x8));

				if (RD(dwAddr + 0xC) != 0)
				{
					VecObject.emplace_back(GetObjectPtr(dwAddr));
				}
			}
		}

		return VecObject.size();
	}
};




#endif // !__ASKTAO_ASKTAO_FEATURE_CORE_OBJECT_VECGAMEOBJECT_H__
