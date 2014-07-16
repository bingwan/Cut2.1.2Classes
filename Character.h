//
//  Character.h
//  HappyFish
//
//  Created by  on 12-8-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef HappyFish_Character_h
#define HappyFish_Character_h

#include "CommonStruct.h"
#include "cocos2d.h" 
#include "BuffObject.h"
#include "BuffProcess.h"
#include "VRope.h"
USING_NS_CC;

#define BALL_TAG 10

class Character : public CCObject
{
public:
    Character();
    virtual ~Character();
    bool                        init(CharacterInfo info);
    void                        remove();
    CharacterInfo*              getData(){return &m_info;}
    CCLayerColor*               getBgNode(){return m_pBgLayer;}
    CharacterType               getCharacterType(){return m_eType;}
    void                        onNotifyContactBegin(Character* pCharacterB);
    bool                        getNeedToDelete(){return m_bNeedToDelete;}
    void                        setNeedToDelete(bool bToDelete){m_bNeedToDelete = bToDelete;}
    b2Body*                     getBody(int nBodyIndex);
    void                        setScaleGravity(float fScale);
    void                        addBuff(int nBuffId,Character* pFromCharacter,BuffStateType eType);
    virtual void                update(float dt);
    void                        deleteBuffWithIndex( int nIndex);
    void                        deleteBuff(BuffObject* pBuff);
    int                         getBuffIndex(BuffObject* pBuff);
    int                         getSameProcessIndex(BuffProcess* pNewProcess);
    void                        createRopeWithBody(b2Body *bodyA,b2Body *bodyB,b2Vec2 anchorA,b2Vec2 anchorB,float sag);
    int                         getApplyToMeContectBeginBuffId();
    int                         getInitApplyToMeBuffId();
    int                         getSelfContectBeginBuffId();
    int                         getClickAppySelfBuffId();
    int                         getApplyToMeContentEndBuffId();
    void                        setLinkRopeToMe(bool bLink){m_bLinkToMe = bLink;}
    bool                        getLinkRopeTome(){return m_bLinkToMe;}
    void                        showBall();
    void                        removeBall();
    void                        applyForceToCenter(b2Vec2& force);
    float                       getRotation();
    void                        stopMove();
    void                        setToHatCharacter(Character* pChar){m_pToCharacter = pChar;}
    Character*                  getToHatCharacter(){return m_pToCharacter;}
    void                        setWorldPos(CCPoint posInWorld);
    CCPoint                     getWorldPos();
    
    void                        setFromOtherHat(bool bFrom){m_bFromOtherhat = bFrom;}
    bool                        getFromOtherHat(){return  m_bFromOtherhat;}
    
    void                        setContactStateType(ContactStateType type){m_eContctStateType = type;}
    ContactStateType            getContactSateType(){return m_eContctStateType;}
    
    int                         getClickAppyToMeBuffId();
    
    CCPoint getPositionInWorld();
    
    void showScaleAction(float fScaleX,float fScaleY,float fScaleTime,float fResetTime);
protected:
 

private:
    bool                            m_bNeedToDelete;
    std::vector<BuffObject*>        m_vBuff;
    bool                            m_bLinkToMe;
    Character*                      m_pToCharacter;
    std::vector<b2Body*>            m_bodyArray;
    std::vector<b2Joint*>           m_jointArray;
    CCLayerColor*                   m_pBgLayer;
    CharacterInfo                   m_info;
    CharacterType                   m_eType;
    bool                            m_bFromOtherhat;
    
    ContactStateType                m_eContctStateType;
};

#endif
