

#include <iostream>
#include "cocos2d.h"
#include "LevelLayer.h"
#include "CCScrollLayer.h"
#include "CCButton.h"
#include "CCAPI.h"
#include "CharacterManager.h"
#include "tinyxml.h"
#include "CCMenuItemButton.h"
#include "CCSceneManager.h"
#include "Box2dManager.h"
#include "ResManager.h"
#include "vrope.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define  WAIT_TIME 0.5
class QueryCallback : public b2QueryCallback
{
public:
    QueryCallback(const b2Vec2& point)
    {
        m_point = point;
        m_fixture = NULL;
    }
    
    virtual bool ReportFixture(b2Fixture* fixture)
    {
        b2Body* body = fixture->GetBody();
        if (body->GetType() == b2_dynamicBody ||
            body->GetType() == b2_staticBody
            )
        {
            bool inside = fixture->TestPoint(m_point);
            if (inside)
            {
                m_fixture = fixture;
                return false;
            }
        }
        
        return true;
    }
    b2Vec2      m_point;
    b2Fixture*  m_fixture;
};

LevelLayer::LevelLayer():
m_nCurStar(0)
{
}

LevelLayer::~LevelLayer()
{
}

LevelLayer * LevelLayer::create(const ccColor4B& color, GLfloat width, GLfloat height)
{
    LevelLayer * pLayer = new LevelLayer();
    if( pLayer && pLayer->initWithColor(color,width,height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

bool LevelLayer::initWithColor(const ccColor4B& color, GLfloat width, GLfloat height)
{
	if ( !CCLayerColor::initWithColor(color,width,height) )
	{
		return false;
	}
    setTouchEnabled(true);
    return true;
}

void LevelLayer::onExit()
{
    CCLayerColor::onExit();    
    Box2dManager::shareBox2dManager()->setAbleUpdate(false);
}

void LevelLayer::onEnter()
{
    CCLayerColor::onEnter();
    
    CCSize contentSize = getContentSize();

//    CCLayerColor* pBgLayer = CCLayerColor::create(ccc4(0,0,200,200), contentSize.width, contentSize.height);
//    pBgLayer->setPosition(ccp(0,0));
//    addChild(pBgLayer);
    
    CCMenu* pNormalMenu = CCMenu::create();
    pNormalMenu->setPosition(ccp(0,0));
    addChild(pNormalMenu);
  
    CCMenuItemButton* pBackButton = CCMenuItemButton::create("CloseNormal.png", NULL, NULL, this, menu_selector(LevelLayer::clickBack));
    pBackButton->setPosition(ccp(contentSize.width*0.9,contentSize.height*0.9));
    pNormalMenu->addChild(pBackButton);
    
    gameStart();
}

void LevelLayer::initBuff()
{
    std::map<int, Character*>*  pCurCharMap = CharacterManager::shareCharacterManager()->getCurLevelCharacterMap();
    if(!pCurCharMap)
    {
        return;
    }
    std::map<int, Character*>::iterator it = pCurCharMap->begin();
    while (it != pCurCharMap->end())
    {
        Character* pChar = it->second;
        int nInitBuffId =  pChar->getInitApplyToMeBuffId();
        m_pCharacterMe->addBuff(nInitBuffId,pChar,eBuffStateType_Unknown);
        it++;
    }
}
void  LevelLayer::ropeTwoCharacter(Character* pCharA ,Character* pCharB ,float fLength)
{
    if(!pCharA || ! pCharB)
        return;
    b2Body* bodyA = pCharA->getBody(0);
    b2Body* bodyB = pCharB->getBody(0);
    CCBodyRope* pBodyRope = new CCBodyRope();
    pBodyRope->init(bodyA, bodyB,this,fLength);
    CharacterManager::shareCharacterManager()->createBodyRopeDelay(pBodyRope);
}


void LevelLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
    CCPoint pTouchPoint = pTouch->getLocation();
    clickCharacter(pTouchPoint);
    
}

void LevelLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch* pTouch = (CCTouch*)pTouches->anyObject();
    CCPoint pTouchPoint = pTouch->getLocation();
    CCPoint pPreTouchPoint = pTouch->getPreviousLocation();
    float fDis = ccpDistance(pTouchPoint, pPreTouchPoint);
    CCPoint fDirDis = ccp(pTouchPoint.x - pPreTouchPoint.x,pTouchPoint.y - pPreTouchPoint.y);
    
    for(int i=0;i<=fDis;i++)
    {
        CCPoint pos = ccp(pPreTouchPoint.x+fDirDis.x*i /fDis,pPreTouchPoint.y+fDirDis.y*i/fDis);
        cutRope(pos);
    }
}
void LevelLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
}
void LevelLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{}

void LevelLayer::clickCharacter(CCPoint pTouchPoint)
{
    b2Vec2 touchPos = b2Vec2(pTouchPoint.x/PTM_RATIO,pTouchPoint.y/PTM_RATIO);
    
    b2AABB aabb;
    b2Vec2 d;
    d.Set(2, 2);
    aabb.lowerBound = touchPos - d;
    aabb.upperBound = touchPos + d;
    QueryCallback  callback(touchPos);
    
    Box2dManager::shareBox2dManager()->getWord()->QueryAABB(&callback, aabb);
    
    if (!callback.m_fixture)
    {
        CCLog("!callback.m_fixture");
        return;
    }
    
    b2Body* pBody = callback.m_fixture->GetBody();
    if(!pBody)
    {
        return;
    }
    CCSprite* pSprite =  (CCSprite*)pBody->GetUserData();
    if(!pSprite)
    {
        return;
    }
    if(pSprite->getTag() != eSpriteTagType_Character)
    {
        return;
    }
    Character* pChar = (Character*)pSprite->getUserData();
    if(!pChar)
    {
        return;
    }
    int nClickBuffId = pChar->getClickAppySelfBuffId();
    pChar->addBuff(nClickBuffId, NULL,eBuffStateType_Begin);
    
    Character* pMe = getCharacterMe();
    if(!pMe)
    {
        CCLog("!pMe");
    }
    int nToMeBuffId = pChar->getClickAppyToMeBuffId();
    pMe->addBuff(nToMeBuffId, pChar,eBuffStateType_Begin);

}
void LevelLayer::cutRope(CCPoint pTouchPoint)
{
    b2Vec2 touchPos = b2Vec2(pTouchPoint.x/PTM_RATIO,pTouchPoint.y/PTM_RATIO);
    
    b2AABB aabb;
    b2Vec2 d;
    d.Set(2, 2);
    aabb.lowerBound = touchPos - d;
    aabb.upperBound = touchPos + d;
    
    QueryCallback  callback(touchPos);
    
    Box2dManager::shareBox2dManager()->getWord()->QueryAABB(&callback, aabb);
    
    if (callback.m_fixture)
    {
        b2Body* pBody = callback.m_fixture->GetBody();
        if(!pBody)
        {
            CCLog("!pBody");
            return;
        }
        CCSprite* pRopeSprite =  (CCSprite*)pBody->GetUserData();
        if(!pRopeSprite)
        {
            CCLog("!pRopeSprite");
            return;
        }
        if(pRopeSprite->getTag() != eSpriteTagType_BodyRope)
        {
            CCLog("pRopeSprite->getTag() != eSpriteTagType_BodyRope");
            return;
        }
        CCBodyRope* pRope = (CCBodyRope*)pRopeSprite->getUserData();
        if(!pRope)
        {
            CCLog("!pRope");
            return;
        }
        CharacterManager::shareCharacterManager()->deleteBodyRopeDelay(pRope);
    }
}


//bool LevelLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
//{
//    if(CCLayerColor::ccTouchBegan(pTouch, pEvent))
//    {
//        CCPoint touchPointInWorld = pTouch->getLocationInView();
//        touchPointInWorld = CCDirector::sharedDirector()->convertToGL(touchPointInWorld);
//        
//        return true;
//    }
//    return false;
//}
//void LevelLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
//{
////    CCPoint touchPointInWorld = pTouch->getLocationInView();
////    CCPoint prePointInWorld = pTouch->getPreviousLocationInView();
////    touchPointInWorld = CCDirector::sharedDirector()->convertToGL(touchPointInWorld);
////    prePointInWorld = CCDirector::sharedDirector()->convertToGL(prePointInWorld);
////    
////    int moveX = touchPointInWorld.x - prePointInWorld.x;
////    int moveY = touchPointInWorld.y - prePointInWorld.y;
//    
//}
//
//void LevelLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
//{
//}

void LevelLayer::clickBack()
{
    deleteCurLevelAll();
    
     scheduleOnce(schedule_selector(LevelLayer::goHomeScene), WAIT_TIME);
}

void LevelLayer::goHomeScene()
{
    CCSceneManager::share()->replaceScene(SIE_HOME);
}

void LevelLayer::draw()
{
    CCLayerColor::draw();
    Box2dManager::shareBox2dManager()->draw();
}
int LevelLayer::getCurStar()
{
    return m_nCurStar;
}
void LevelLayer::setCurStar(int nStar)
{
    m_nCurStar = nStar;
    CCLog("m_nCurStar=%d",m_nCurStar);
}

void LevelLayer::reStarGame()
{
    deleteCurLevelAll();
    scheduleOnce(schedule_selector(LevelLayer::gameStart), WAIT_TIME);
}

void LevelLayer::gameSuccess()
{    
    deleteCurLevelAll();
    scheduleOnce(schedule_selector(LevelLayer::showGameResult), WAIT_TIME);
}
void LevelLayer::showGameResult()
{
    Box2dManager::shareBox2dManager()->setAbleUpdate(false);
    CCSceneManager::share()->replaceScene(SIE_HOME);
}
void LevelLayer::deleteCurLevelAll()
{
    CharacterManager::shareCharacterManager()->deleteAllBodyRopeDelay();
    CharacterManager::shareCharacterManager()->deleteAllCharacterDelay();
}

void LevelLayer::gameStart()
{
    int nLevel = CCSceneManager::share()->getCurLevelId();
    std::map<int, CharacterInfo> levelMap;
    char chrLevelName[32];
    sprintf(chrLevelName, "%d.xml",nLevel);
    ResManager::shareResManager()->loadLevelMap(chrLevelName,&levelMap);
    
    std::map<int, CharacterInfo>:: iterator it = levelMap.begin();
    while (it != levelMap.end())
    {
        Character* pCharacter = CharacterManager::shareCharacterManager()->createCharacter(it->second);
        if(!pCharacter)
        {
            it++;
            continue;
        }
        if(!pCharacter->getBgNode())
        {
            it++;
            continue;
        }
        addChild(pCharacter->getBgNode());
        it++;
        
        if(eCharacterType_Me == pCharacter->getCharacterType())
        {
            m_pCharacterMe = pCharacter;
            m_pCharacterMe->getBgNode()->setZOrder(CHARACTER_ME_Z);
        }
    }
    
    initHat();
    initBuff();
    Box2dManager::shareBox2dManager()->setAbleUpdate(true);
}


void LevelLayer::initHat()
{
    std::map<int, Character*>*  pCurCharMap = CharacterManager::shareCharacterManager()->getCurLevelCharacterMap();
    if(!pCurCharMap)
    {
        return;
    }
    std::map<int, Character*>::iterator it = pCurCharMap->begin();
    while (it != pCurCharMap->end())
    {
        Character* pChar = it->second;
        if(!pChar)
        {
            it++;
            continue;
        }
        int nBuffId = pChar->getApplyToMeContectBeginBuffId();
        BuffTemplateInfo* pBTI = ResManager::shareResManager()->getBuffTemplate(nBuffId);
        if(!pBTI)
        {
            it++;
            continue;
        }
        BufferType eBuffType = pBTI->eBufferType;
        if(eBufferType_Hat == eBuffType)
        {
            Character* pSameIcChar = getSameToBuffTypeCharacter(pChar,nBuffId);
            if(pSameIcChar)
            {
                pChar->setToHatCharacter(pSameIcChar);
            }
            else
            {
                CCAssert(0, "pSameIcChar==NULL");
            }
        }
        it++;
    }
}

Character*  LevelLayer::getSameToBuffTypeCharacter(Character* pCharacter,int BuffId)
{
    std::map<int, Character*>*  pCurCharMap = CharacterManager::shareCharacterManager()->getCurLevelCharacterMap();
    if(!pCurCharMap)
    {
        return NULL;
    }
    std::map<int, Character*>::iterator it = pCurCharMap->begin();
    while (it != pCurCharMap->end())
    {
        Character* pChar = it->second;
        if(!pChar)
            continue;
        
        int nBuffId = pChar->getApplyToMeContectBeginBuffId();
        BuffTemplateInfo* pBTI = ResManager::shareResManager()->getBuffTemplate(nBuffId);
        if(!pBTI)
        {
            it++;
            continue;
        }
        BufferType eBuffType = pBTI->eBufferType;
        if(eBufferType_Hat == eBuffType)
        {
            if(pCharacter != pChar )
            {
                //pChar->setToHatCharacter(pCharacter);
                return pChar;
            }
        }
        
        it++;
    }
    return NULL;
    
}







