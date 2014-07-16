
#ifndef CCWater_CharacterManager_h
#define CCWater_CharacterManager_h

#include "cocos2d.h"
#include "CommonEnum.h"
#include "CommonStruct.h"
#include "Character.h"
#include "CCBodyRope.h"

USING_NS_CC;



class  CharacterManager : public CCObject
{
public:
    
	CharacterManager();
	virtual			~CharacterManager();
    static CharacterManager*                    shareCharacterManager();
    void                                        init();
    Character*                                  createCharacter(CharacterInfo info);
    void                                        clearLevelCharacterMap();
    Character*                                  getLevelCharacter(int index);
    void                                        deleteLevelCharacter(int index);
    std::map<int, Character*>*                  getCurLevelCharacterMap(){return &m_levelCharacerMap;}
    void                                        deleteCharacterDelay(Character* pCharacter);
    virtual void                                update(float dt);
    void                                        deleteBodyRopeDelay(CCBodyRope* pRope);
    void                                        createBodyRopeDelay(CCBodyRope* pRope);
    void                                        cleanCurLevel();
    void                                        deleteAllBodyRopeDelay();
    void                                        deleteAllCharacterDelay();
    void                                        stopMoveDelay(Character* pchar);
    void                                        setTransformDelay(Character* pChar,CCPoint pos ,float fRadian);
protected:
    bool                                        isHveBodyRopeToDelete(CCBodyRope* pRope);
    bool                                        isHveBodyRopeToCreate(CCBodyRope* pRope);
    int                                         getBodyRopeIndex(CCBodyRope* pRope);
    void                                        deleteBodyRope(CCBodyRope* pRope);
   
private:
    std::map<int, Character*>                    m_levelCharacerMap;
    std::vector<Character*>                      m_vNeedToDeleteCharacterArray;
    
    std::vector<CCBodyRope*>                     m_vNeedToCreateRope;
    std::vector<CCBodyRope*>                     m_vNeedToDeleteRope;
    std::vector<CCBodyRope*>                     m_vBodyRope;
    std::vector<Character*>                      m_vNeedToChangeForStaticCharacter;
    
    std::vector<NeedToCharacter>                m_vNeedToCharacter;


};


#define gCharacterManager   CharacterManager::GetSingleton()
#define gCharacterManagerPtr   CharacterManager::GetSingletonPtr()

#endif
