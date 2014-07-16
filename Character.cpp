//
//  Character.cpp
//  HappyFish
//
//  Created by  on 12-8-26.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Character.h"
#include "CharacterManager.h"
#include "Box2dManager.h"
#include "cocos2d.h"
#include "CCAPI.h"
#include "ResManager.h"
#include "BuffManager.h"
#include "BuffProcess.h"
#include "VRope.h"
USING_NS_CC;


Character::Character():
m_bNeedToDelete(false),
m_bLinkToMe(false),
m_pToCharacter(NULL),
m_bFromOtherhat(false),
m_pBgLayer(NULL),
m_eType(eCharacterType_Unknown),
m_eContctStateType(eContactStateType_Unknown)
{
}
Character::~Character()
{
    if(m_pBgLayer)
    {
        delete m_pBgLayer;
    }
    
    m_bodyArray.clear();
    m_jointArray.clear();
}
bool Character::init(CharacterInfo info)
{    
    m_info = info;
    
    CharacterTemplateInfo* pCTI = ResManager::shareResManager()->getCharacterTemplateInfo(m_info.Id);
    if(!pCTI)
        return false;
    
    m_eType = pCTI->eType;
    m_pBgLayer = new  CCLayerColor();
    m_pBgLayer->initWithColor(BG_COLOR_LEVEL, pCTI->contentSize.width, pCTI->contentSize.height);
    m_pBgLayer->setContentSize(pCTI->contentSize);
    m_pBgLayer->setPosition(m_info.position);

    for(int i=0;i<pCTI->vBodyArray.size();++i)
    {
        BodyInfo aBodyInfo = pCTI->vBodyArray[i];
        
        b2Body* pBody = Box2dManager::shareBox2dManager()->createBody(aBodyInfo);
        if(!pBody)
        {
            continue;
        }
        
        CCPoint bodyPosInWorld = m_pBgLayer->convertToWorldSpace(ccp(pCTI->contentSize.width*aBodyInfo.bodyPosition.x, pCTI->contentSize.height*aBodyInfo.bodyPosition.y));
        pBody->SetTransform(b2Vec2(bodyPosInWorld.x/PTM_RATIO,bodyPosInWorld.y/PTM_RATIO), aBodyInfo.rotate*b2_pi/180);
        BodyTemplateInfo* pBodyTemplate = ResManager::shareResManager()->getBodyTemplateInfo(aBodyInfo.Id);
        if(!pBodyTemplate)
        {
            continue;
        }
        
        CCSprite* pSprite = CCSprite::create(pBodyTemplate->png.c_str());
        pSprite->setPosition(ccp(pCTI->contentSize.width*aBodyInfo.bodyPosition.x, pCTI->contentSize.height*aBodyInfo.bodyPosition.y));
        m_pBgLayer->addChild(pSprite);
        pBody->SetUserData(pSprite);
        
        pSprite->setTag(eSpriteTagType_Character);  
        pSprite->setUserData(this);
        
        m_bodyArray.push_back(pBody);
    }
    
    for(int jIndex=0;jIndex<pCTI->vJointArray.size();++jIndex)
    {
        JointInfo jointInfo = pCTI->vJointArray[jIndex];
        JointTemplateInfo* pJTI = ResManager::shareResManager()->getJointTemplate(jointInfo.Id);
        if(!pJTI)
        {
            continue;
        }
            
        //            e_unknownJoint,
        //            e_revoluteJoint,
        //            e_prismaticJoint,
        //            e_distanceJoint,
        //            e_pulleyJoint, 滑轮
        //            e_mouseJoint,
        //            e_gearJoint, 齿轮
        //            e_wheelJoint, 车轮
        //            e_weldJoint,焊接
        //            e_frictionJoint,
        //            e_ropeJoint
        b2Joint* pJoint = NULL;
        switch (pJTI->jointType)
        {
            case e_revoluteJoint:// 旋转
            {}
                break;
            case e_prismaticJoint:
            {
            }
                break;
            case e_distanceJoint:
            {
                b2DistanceJointDef distanceJoint;
                
                CCAPI::share()->setJointInfo(&distanceJoint, pJTI->parmArray);
                distanceJoint.bodyA = m_bodyArray[jointInfo.bodyAIndex];
                distanceJoint.bodyB = m_bodyArray[jointInfo.bodyBIndex];
                distanceJoint.localAnchorA = b2Vec2(jointInfo.bodyAPos.x/PTM_RATIO,jointInfo.bodyAPos.y/PTM_RATIO);
                distanceJoint.localAnchorB = b2Vec2(jointInfo.bodyBPos.x/PTM_RATIO,jointInfo.bodyBPos.y/PTM_RATIO);
                
                //ropeJoint.collideConnected = true;
                pJoint = Box2dManager::shareBox2dManager()->getWord()->CreateJoint(&distanceJoint);
                              
            }
                break;
            case e_pulleyJoint:
            {}
                break;
            case e_gearJoint:
            {}
                break;
            case e_wheelJoint:
            {}
                break;
            case e_weldJoint :
            {}
                break;
            case e_frictionJoint:
            {}
                break;
            case e_ropeJoint:
            {
                b2RopeJointDef ropeJoint;
                
                
                CCAPI::share()->setJointInfo(&ropeJoint, pJTI->parmArray);
                ropeJoint.bodyA = m_bodyArray[jointInfo.bodyAIndex];
                ropeJoint.bodyB = m_bodyArray[jointInfo.bodyBIndex];
                ropeJoint.localAnchorA = b2Vec2(jointInfo.bodyAPos.x/PTM_RATIO,jointInfo.bodyAPos.y/PTM_RATIO);
                ropeJoint.localAnchorB = b2Vec2(jointInfo.bodyBPos.x/PTM_RATIO,jointInfo.bodyBPos.y/PTM_RATIO);
                
                pJoint = Box2dManager::shareBox2dManager()->getWord()->CreateJoint(&ropeJoint);
          
                
            }
                break;
                
            default:
                break;
        }
        if(pJoint)
        {
            m_jointArray.push_back(pJoint);
        }
    }

    return  true;
}

void Character::createRopeWithBody(b2Body *bodyA,b2Body *bodyB,b2Vec2 anchorA,b2Vec2 anchorB,float sag)
{
    b2RopeJointDef jd;
    jd.bodyA = bodyA;
    jd.bodyB = bodyB;
    jd.localAnchorA.Set(anchorA.x,anchorA.y);
    jd.localAnchorB.Set(anchorB.x,anchorB.y);
    
    // Max length of joint = current distance between bodies * sag
    float32 ropeLength = (bodyA->GetWorldPoint(anchorA) - bodyB->GetWorldPoint(anchorB)).Length() * sag;
    jd.maxLength = ropeLength;
    
    // Create joint
    //b2RopeJoint *ropeJoint = (b2RopeJoint *)Box2dManager::shareBox2dManager()->getWord()->CreateJoint(&jd);
    
    //ropes.push_back(newRope);
    
}
// delete joint
// delete sprite
// delete body
void Character::remove()
{
    for(int jointIndex=0;jointIndex<m_jointArray.size();++jointIndex)
    {
        b2Joint* pJoint = m_jointArray[jointIndex];
        if(!pJoint)
            continue;
        Box2dManager::shareBox2dManager()->destroyJoint(pJoint);
    }
    
    for(int bodyIndex=0;bodyIndex<m_bodyArray.size();++bodyIndex)
    {
        b2Body* pBody = m_bodyArray[bodyIndex];
        if(!pBody)
            continue;
        CCSprite* pBodySprite = (CCSprite*)pBody->GetUserData();
        if(pBodySprite)
        {
            pBodySprite->setUserData(NULL);
            pBodySprite->removeFromParentAndCleanup(true);
            pBodySprite = NULL;
        }
        pBody->SetUserData(NULL);
        Box2dManager::shareBox2dManager()->destroyBody(pBody);
    }
    if(m_pBgLayer)
    {
        m_pBgLayer->removeFromParentAndCleanup(true);
        m_pBgLayer = NULL;
    }
}

void Character::onNotifyContactBegin(Character* pCharacterB)
{
   // remove();
    CCLog("11111");
}

b2Body* Character::getBody(int nBodyIndex)
{
    if(nBodyIndex >= m_bodyArray.size())
    {
        return NULL;
    }
    return m_bodyArray[nBodyIndex];
}

void Character::setScaleGravity(float fScale)
{
    for(int i=0;i<m_bodyArray.size();i++)
    {
        b2Body* pBody = m_bodyArray[i];
        if(!pBody)
            continue;
        pBody->SetGravityScale(fScale);
    }
}

int Character::getSameProcessIndex(BuffProcess* pNewProcess)
{
    for(int buffIndex = 0; buffIndex != m_vBuff.size();++buffIndex)
	{
		if(m_vBuff[buffIndex] != NULL)
		{
			BuffProcess* pIndexBuffProcess = BuffManager::share()->getBuffProcess(m_vBuff[buffIndex]);
			if (pIndexBuffProcess == pNewProcess)
			{
                return buffIndex;
			}
		}
	}
    return -1;
}
//void Character::addBuff(int nBuffId)
//{
//    BuffObject* pBuff = BuffManager::share()->createBuffObject(nBuffId);
//    if(!pBuff)
//    {
//        return;
//    }
//    BuffProcess* pNewProcess = BuffManager::share()->getBuffProcess(pBuff);
//    if(!pNewProcess)
//    {
//        return;
//    }
//    
//    int nOldProcessIndex = getSameProcessIndex(pNewProcess);
//    if(nOldProcessIndex != -1)
//    {
//        BuffObject* pBuff = m_vBuff[nOldProcessIndex];
//        if(!pBuff)
//        {
//            return;
//        }
//        deleteBuffWithIndex(nOldProcessIndex);
//    }
//    
//    pNewProcess->onStartProcessBuff(this,pBuff);
//	if(pNewProcess->isOnced())
//	{
//        pNewProcess->onEndProcessBuff(this, pBuff);
//        BuffManager::share()->deleteBuff(pBuff);
//	}
//    else
//    {
//        m_vBuff.push_back(pBuff);
//    }
//    
//}
void Character::addBuff(int nBuffId,Character* pFromCharacter,BuffStateType eType)
{
    BuffObject* pBuff = BuffManager::share()->createBuffObject(nBuffId);
    if(!pBuff)
    {
        return;
    }
    BuffProcess* pNewProcess = BuffManager::share()->getBuffProcess(pBuff);
    if(!pNewProcess)
    {
        return;
    }
    pBuff->setBuffStateType(eType);
    if(pFromCharacter)
    {
        pBuff->setFromCharacter(pFromCharacter);
    }
    int nOldProcessIndex = getSameProcessIndex(pNewProcess);
    if(nOldProcessIndex != -1)
    {
        BuffObject* pBuff = m_vBuff[nOldProcessIndex];
        if(!pBuff)
        {
            return;
        }
        deleteBuffWithIndex(nOldProcessIndex);
    }
    
    pNewProcess->onStartProcessBuff(this,pBuff);
	if(pNewProcess->isOnced())
	{
        pNewProcess->onEndProcessBuff(this, pBuff);
        BuffManager::share()->deleteBuff(pBuff);
	}
    else
    {
        m_vBuff.push_back(pBuff);
    }
}

void Character::update(float dt)
{
    std::vector<int> vToDeleteBuff;
    for(int i=0;i<m_vBuff.size();i++)
    {
        BuffObject* pBuff = m_vBuff[i];
        if(!pBuff)
            continue;
        BuffProcess* pProcess = BuffManager::share()->getBuffProcess(pBuff);
        if(!pProcess)
            continue;
        if(pBuff->getFinished())
        {
            vToDeleteBuff.push_back(i);
        }
        else
        {
             pProcess->onTickProcessBuff(this, pBuff, dt);
        }        
    }
    for(int i=0;i<vToDeleteBuff.size();i++)
    {
        deleteBuffWithIndex(vToDeleteBuff[i]);
    }
    vToDeleteBuff.clear();
}
int Character::getBuffIndex(BuffObject* pBuff)
{
    for(int buffIndex = 0; buffIndex != m_vBuff.size();++buffIndex)
	{
		if(m_vBuff[buffIndex] != NULL)
		{
			if (pBuff == m_vBuff[buffIndex] )
			{
                return buffIndex;
			}
		}
	}
    return -1;
}
	
void Character::deleteBuffWithIndex( int nIndex)
{
    if(nIndex >= m_vBuff.size() || nIndex<0)
    {
        return;
    }
    BuffObject* pBuff = m_vBuff[nIndex];
    if(!pBuff)
    {
        return;
    }
    m_vBuff.erase(m_vBuff.begin()+nIndex);
    deleteBuff(pBuff);
    
}

void Character::deleteBuff(BuffObject* pBuff)
{
    if(!pBuff)
        return;
    BuffProcess* pNewProcess = BuffManager::share()->getBuffProcess(pBuff);
    if(!pNewProcess)
        return;
    pNewProcess->onEndProcessBuff(this,pBuff);
    BuffManager::share()->deleteBuff(pBuff);
  
}

int Character::getApplyToMeContectBeginBuffId()
{
    CharacterTemplateInfo* pCh = ResManager::shareResManager()->getCharacterTemplateInfo(m_info.Id);
    if(!pCh)
        return -1;
    return pCh->nToBeginBuffId;
}

int Character::getInitApplyToMeBuffId()
{
    CharacterTemplateInfo* pCh = ResManager::shareResManager()->getCharacterTemplateInfo(m_info.Id);
    if(!pCh)
        return -1;
    return pCh->nInitBuffId;
}
int Character::getSelfContectBeginBuffId()
{
    CharacterTemplateInfo* pCh = ResManager::shareResManager()->getCharacterTemplateInfo(m_info.Id);
    if(!pCh)
        return -1;
    return pCh->nSelfBeginBuffId;
}

int Character::getClickAppySelfBuffId()
{
    CharacterTemplateInfo* pCh = ResManager::shareResManager()->getCharacterTemplateInfo(m_info.Id);
    if(!pCh)
        return -1;
    return pCh->nClickApplySelfBuffId;
}
int Character::getClickAppyToMeBuffId()
{
    CharacterTemplateInfo* pCh = ResManager::shareResManager()->getCharacterTemplateInfo(m_info.Id);
    if(!pCh)
        return -1;
    return pCh->nClickApplyToMeBuffId;
}

int Character::getApplyToMeContentEndBuffId()
{
    CharacterTemplateInfo* pCh = ResManager::shareResManager()->getCharacterTemplateInfo(m_info.Id);
    if(!pCh)
        return -1;
    return pCh->nToEndBuffId;
}


void Character::showBall()
{
    if(m_bodyArray.size() == 0)
        return;
    
    b2Body* pBody = m_bodyArray[0];
    if(!pBody)
    {
        return;
    }
    CCSprite* pSprite =  (CCSprite*)pBody->GetUserData();
    if(!pSprite)
    {
        return;
    }
    CCNode* pNode = pSprite->getChildByTag(BALL_TAG);  //bingwan todo
    if(pNode)
    {
        return;
    }
    
    CCSize size = pSprite->getContentSize();
    CCSprite* pBallSprite = CCSprite::create("bubble.png");
    //pBallSprite->setOpacity(255*0.7);
    pBallSprite->setPosition(ccp(size.width*0.5,size.height*0.5));
    pBallSprite->setTag(BALL_TAG);
    pSprite->addChild(pBallSprite);
}

void Character::removeBall()
{
    if(m_bodyArray.size() == 0)
        return;
    
    b2Body* pBody = m_bodyArray[0];
    if(!pBody)
    {
        return;
    }
    CCSprite* pSprite =  (CCSprite*)pBody->GetUserData();
    if(!pSprite)
    {
        return;
    }
    pSprite->removeChildByTag(BALL_TAG);
}

void Character::applyForceToCenter(b2Vec2& force)
{
    for(int i=0;i<m_bodyArray.size();i++)
    {
        b2Body* pBody = m_bodyArray[i];
        if(!pBody)
            continue;
        pBody->ApplyForceToCenter(force);
    }
}

float Character::getRotation()
{
    if(m_bodyArray.size() == 0)
        return 0;
    b2Body* pBody = m_bodyArray[0];
    if(!pBody)
        return 0;
    CCSprite* pSprite =  (CCSprite*)pBody->GetUserData();
    if(!pSprite)
        return 0;
    
    float frotation = pSprite->getRotation();
    return frotation;
    
    
//    CharacterTemplateInfo* pCTI = ResManager::shareResManager()->getCharacterTemplateInfo(m_info.Id);
//    if(!pCTI)
//        return 0;
//    BodyInfo* pBodyInfo = &pCTI->vBodyArray[0];
//    if(!pBodyInfo)
//        return 0;
//    float fRotation = pBodyInfo->rotate;
//    return fRotation;
}

void Character::stopMove()
{
    for(int i=0;i<m_bodyArray.size();i++)
    {
        b2Body* pBody = m_bodyArray[i];
        if(!pBody)
            continue;
        pBody->SetType(b2_staticBody);
    }
}

void Character::setWorldPos(CCPoint posInWorld)
{
    if(m_bodyArray.size() == 0)
        return ;
    b2Body* pBody = m_bodyArray[0];
    if(!pBody)
        return;
    pBody->SetTransform(b2Vec2(posInWorld.x/PTM_RATIO,posInWorld.y/PTM_RATIO), 0);
    CCSprite* pSprite =  (CCSprite*)pBody->GetUserData();
    if(!pSprite)
        return ;
    CCNode* pParent =pSprite->getParent();
    if(!pParent)
        return;
    CCPoint point = pParent->convertToNodeSpace(CCPointMake( posInWorld.x/PTM_RATIO,posInWorld.y/PTM_RATIO));
    pSprite->setPosition(point);
}

CCPoint Character::getWorldPos()
{    
    if(m_bodyArray.size() == 0)
        return CCPointZero;
    
    b2Body* pBody = m_bodyArray[0];
    if(!pBody)
        return CCPointZero;
    CCSprite* pSprite =  (CCSprite*)pBody->GetUserData();
    if(!pSprite)
        return CCPointZero;
    CCPoint pos =  pSprite->getParent()->convertToWorldSpace(pSprite->getPosition());
    return pos;
}

CCPoint Character::getPositionInWorld()
{
    CCPoint pos;
    if(m_bodyArray.size() == 0)
        return pos;
    b2Body* pBody = m_bodyArray[0];
    if(!pBody)
        return pos;
    CCSprite* pSprite =  (CCSprite*)pBody->GetUserData();
    if(!pSprite)
        return pos;
    CCNode* pParent = pSprite->getParent();
    if(!pParent)
        return pos;
    pos = pSprite->convertToWorldSpace(pSprite->getPosition());
    return pos ;
}

void Character::showScaleAction(float fScaleX,float fScaleY,float fScaleTime,float fResetTime)
{
    if(m_bodyArray.size() == 0)
        return ;
    b2Body* pBody = m_bodyArray[0];
    if(!pBody)
        return ;
    CCSprite* pSprite = (CCSprite*)pBody->GetUserData();
    if(!pSprite)
        return ;
    CCNode* pParent = pSprite->getParent();
    if(!pParent)
        return ;
    
    CCScaleTo* pScaleTo1 = CCScaleTo::create(fScaleTime, fScaleX,fScaleY);
    CCScaleTo* pScaleTo2 = CCScaleTo::create(fResetTime, 1,1);
    CCSequence* pSeq = CCSequence::create(pScaleTo1,pScaleTo2,NULL);
    pParent->runAction(pSeq);
    
}
