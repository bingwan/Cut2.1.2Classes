

#include <iostream>
#include "CharacterManager.h"
#include "tinyxml.h"
#include "CCAPI.h"

static CharacterManager* s_pCharacterManager = NULL;
CharacterManager* CharacterManager::shareCharacterManager()
{
    if( NULL == s_pCharacterManager)
    {
        s_pCharacterManager = new CharacterManager();
    }
    return s_pCharacterManager;
}

CharacterManager::CharacterManager()
{
}


CharacterManager::~CharacterManager()
{
}

void CharacterManager::init()
{  
}

void CharacterManager::deleteCharacterDelay(Character* pCharacter)
{
    if(!pCharacter)
        return ;
    m_vNeedToDeleteCharacterArray.push_back(pCharacter);
}

void CharacterManager::update(float dt)
{
    for(int i=0;i<m_vNeedToDeleteRope.size();++i)
    {
        CCBodyRope* pRope = m_vNeedToDeleteRope[i];
        if(!pRope)
            continue;
        
        deleteBodyRope(pRope);
    }
    m_vNeedToDeleteRope.clear();
    
    
    for(int i=0;i<m_vNeedToCreateRope.size();++i)
    {
        CCBodyRope* pRope = m_vNeedToCreateRope[i];
        if(!pRope)
            continue;
        pRope->createBodyRope();
        m_vBodyRope.push_back(pRope);
    }
    m_vNeedToCreateRope.clear();
    
    for(int i=0;i<m_vNeedToDeleteCharacterArray.size();++i)
    {
        Character* pChar = m_vNeedToDeleteCharacterArray[i];
        if(!pChar)
            continue;
        deleteLevelCharacter(pChar->getData()->index);
    }
    m_vNeedToDeleteCharacterArray.clear();
    
    for(int i=0;i<m_vNeedToCharacter.size();i++)
    {
        NeedToCharacter* pNeedToChar = &m_vNeedToCharacter[i];
        if(!pNeedToChar)
            continue;
        if(!pNeedToChar->pChar)
            continue;
        if(eNeedToCharacterType_Setpos == pNeedToChar->eType)
        {
            pNeedToChar->pChar->setWorldPos(pNeedToChar->pos);
            
        }
    }
    m_vNeedToCharacter.clear();
    
    for(int i=0;i<m_vNeedToChangeForStaticCharacter.size();i++)
    {
        Character* pChar = m_vNeedToDeleteCharacterArray[i];
        if(!pChar)
            continue;
        pChar->stopMove();
    }
    m_vNeedToChangeForStaticCharacter.clear();
}

Character* CharacterManager::createCharacter(CharacterInfo info)
{
    Character* pCharacter = new Character();
    pCharacter->init(info);
    m_levelCharacerMap.insert(std::make_pair(info.index, pCharacter));
    
    return pCharacter;
}

void CharacterManager::clearLevelCharacterMap()
{
    std::map<int, Character*>:: iterator it = m_levelCharacerMap.begin();
    while (it != m_levelCharacerMap.end())
    {
        Character* pCharacter = it->second;
        pCharacter->remove();
        delete pCharacter;
        pCharacter = NULL;
        it++;
    }
    m_levelCharacerMap.clear();
}

Character* CharacterManager::getLevelCharacter(int index)
{
    std::map<int, Character*>:: iterator it = m_levelCharacerMap.find(index);
    if(it != m_levelCharacerMap.end()) 
    {
        return it->second;
    }
    return NULL;
}

void CharacterManager::deleteLevelCharacter(int index)
{
    std::map<int, Character*>:: iterator it = m_levelCharacerMap.find(index);
    if(it != m_levelCharacerMap.end())
    {
        Character* pCharacter = it->second;
        if(!pCharacter)
            return;
        pCharacter->remove();
        delete pCharacter;
        pCharacter = NULL;
        m_levelCharacerMap.erase(it);
    }
}

bool CharacterManager::isHveBodyRopeToDelete(CCBodyRope* pRope)
{
    for(int i=0;i<m_vNeedToDeleteRope.size();++i)
    {
        CCBodyRope* pBodyRope = m_vNeedToDeleteRope[i];
        if(pRope == pBodyRope)
        {
            return true;
        }
    }
    return false;
}

void CharacterManager::deleteBodyRopeDelay(CCBodyRope* pRope)
{
    if(!pRope)
    {
        return;
    }
    if(isHveBodyRopeToDelete(pRope))
    {
        return;
    }
    m_vNeedToDeleteRope.push_back(pRope);
}

bool CharacterManager::isHveBodyRopeToCreate(CCBodyRope* pRope)
{
    for(int i=0;i<m_vNeedToCreateRope.size();++i)
    {
        CCBodyRope* pBodyRope = m_vNeedToCreateRope[i];
        if(pRope == pBodyRope)
        {
            return true;
        }
    }
    return false;
}
void CharacterManager::createBodyRopeDelay(CCBodyRope* pRope)
{
    if(!pRope)
    {
        return;
    }
    if(isHveBodyRopeToCreate(pRope))
    {
        return;
    }
    m_vNeedToCreateRope.push_back(pRope);
}

void CharacterManager::cleanCurLevel()
{
//    clearLevelCharacterMap();
//    deleteAllBodyRope();
}

void CharacterManager::deleteAllBodyRopeDelay()
{
    m_vNeedToDeleteRope.clear();
    m_vNeedToCreateRope.clear();
    
    for(int i=0;i<m_vBodyRope.size();i++)
    {
        CCBodyRope* pBodyRope = m_vBodyRope[i];
        if(!pBodyRope)
        {
            continue;
        }
        deleteBodyRopeDelay(pBodyRope);
    }
}

void CharacterManager::deleteAllCharacterDelay()
{
    m_vNeedToDeleteCharacterArray.clear();
    
    std::map<int, Character*>:: iterator it = m_levelCharacerMap.begin();
    while (it != m_levelCharacerMap.end())
    {
        m_vNeedToDeleteCharacterArray.push_back(it->second);
        it++;
    }
}

int CharacterManager::getBodyRopeIndex(CCBodyRope* pRope)
{
    for(int i=0;i<m_vBodyRope.size();i++)
    {
        CCBodyRope* pBodyRope = m_vBodyRope[i];
        if(!pBodyRope)
        {
            continue;
        }
        if(pBodyRope == pRope)
        {
            return i;
        }
    }
    return -1;
}
void CharacterManager::deleteBodyRope(CCBodyRope* pRope)
{
    int nRopeIndex = getBodyRopeIndex(pRope);
    if(nRopeIndex < 0 ||  nRopeIndex >= m_vBodyRope.size())
    {
        return;
    }
    pRope->removeBodyRope();
    m_vBodyRope.erase(m_vBodyRope.begin()+nRopeIndex);
    
    delete pRope;
    pRope = NULL;
}

void CharacterManager::stopMoveDelay(Character* pChar)
{
    if(!pChar)
        return;
    m_vNeedToChangeForStaticCharacter.push_back(pChar);
}

void CharacterManager::setTransformDelay(Character* pChar,CCPoint pos ,float fRadian)
{
    if(!pChar)
        return;
    NeedToCharacter needToChar;
    needToChar.eType = eNeedToCharacterType_Setpos;
    needToChar.pChar = pChar;
    needToChar.pos = pos;
    
    m_vNeedToCharacter.push_back(needToChar);
}




