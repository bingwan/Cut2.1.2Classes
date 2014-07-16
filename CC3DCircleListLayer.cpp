//
//  CCTextLayer.cpp
//  CCTextLayer
//
//  Created by wangbin on 13-1-9.
//
//

#include "CC3DCircleListLayer.h"
#include "CCLabelStroke.h"
#include "CCButton.h"
#include "CCScrollLayer.h"

CC3DCircleListLayer::CC3DCircleListLayer():
m_fDeacceleratePercent(0.98),
m_fToEndActionTime(0.5),
m_pDelegate(NULL),
m_pSelectedButton(NULL),
m_fEllipseA(0),
m_fEllipseB(0),
m_itemSize(CCSizeZero),
m_bDraging(false),
m_fLastMoveDis(0),
m_fLastMoveDegree(0),
m_fCurMoveDegree(0)
{
    m_pButtonArray = CCArray::create();
    CC_SAFE_RETAIN(m_pButtonArray);
}
//=============================================================
//=============================================================
CC3DCircleListLayer::~CC3DCircleListLayer()
{
    m_pButtonArray->removeAllObjects();
    CC_SAFE_RELEASE(m_pButtonArray);
}
//=============================================================
//=============================================================
CC3DCircleListLayer* CC3DCircleListLayer::create(const ccColor4B& color, GLfloat width, GLfloat height)
{
    CC3DCircleListLayer * pLayer = new CC3DCircleListLayer();
    if( pLayer && pLayer->initWithColor(color,width,height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}
//=============================================================
//=============================================================
bool CC3DCircleListLayer::initWithColor(const ccColor4B& color, GLfloat w, GLfloat h)
{
    if(CCCutTouchLayer::initWithColor(color, w, h))
    {
        return true;
    }
    return false;
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority-1, false);
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::initDataCircle(float fEllipseA,float fEllipseB,CCSize itemSize,CCArray* pArray,bool bFlipY)
{
    if(!pArray)
        return;
    m_itemSize = itemSize;
    m_fEllipseA = fEllipseA;
    m_fEllipseB = fEllipseB;
    
    int     nAllItemCount = pArray->count();
    float   fCircleStepDegree = (2*M_PI)/nAllItemCount;
    
    float fMoveX = getContentSize().width*0.5;
    float fMoveY = getContentSize().height*0.5;
    
    m_pButtonArray->removeAllObjects();
    for(int i=0;i<nAllItemCount;i++)
    {
        float fCurDegree = fCircleStepDegree * (i+1);
        float fCircleX = m_fEllipseA * cosf(fCurDegree);
        float fCircleY = m_fEllipseB * sinf(fCurDegree);
        
        fCircleX += fMoveX;
        fCircleY += fMoveY;
        
        CCSprite* pSprite = (CCSprite*)pArray->objectAtIndex(i);
        
        CCScale9Sprite* pScaleSprite = CCScale9Sprite::createWithSpriteFrame(pSprite->displayFrame());
        CCButton* pButton = CCButton::create( pScaleSprite);
        pButton->setTag(i);
        pButton->setZoomOnTouchDown(false);
        
        CCPoint point = ccp(fCircleX,fCircleY);
        pButton->setPosition(point);
        
        CCString* pString = new CCString();
        pString->initWithFormat("%f",fCurDegree);
        pButton->setUserData(pString);
        pButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CC3DCircleListLayer::clickButton), CCControlEventTouchUpInside);
        addChild(pButton);
        m_pButtonArray->addObject(pButton);
        
        if(bFlipY)
        {
            CCSize buttonSize = pButton->getContentSize();
            CCSprite* pFlipYSprite = CCSprite::createWithTexture(pSprite->getTexture());
            CCRect oldRect = pSprite->getTextureRect();
            pFlipYSprite->setFlipY(true);
            pFlipYSprite->setOpacity(255*0.5);
            pFlipYSprite->setAnchorPoint(ccp(0.5,1));
            pFlipYSprite->setPosition(ccp(buttonSize.width*0.5,0));
            pButton->addChild(pFlipYSprite);
        }
        
//        CCSize buttonSize = pButton->getContentSize();
//        char testChr[32];
//        sprintf(testChr, "%d",i);
//        CCLabelTTF* pTestLabel = CCLabelTTF::create(testChr, "Arial", 52);
//        pTestLabel->setColor(ccRED);
//        pTestLabel->setPosition(ccp(buttonSize.width*0.5,buttonSize.height*0.5));
//        pButton->addChild(pTestLabel);
        
    }
    refreshItemWithMoveDegree(0);
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::clickButton(CCButton* pButton)
{
    int index = pButton->getTag();
    CCLog("%d",index);
    moveToCenter(pButton,true);
}
//=============================================================
//=============================================================
bool CC3DCircleListLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(CCCutTouchLayer::ccTouchBegan(pTouch, pEvent))
    {
        m_bDraging = true;
        return true;
    }
    return false;
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if(!isVisible())
    {
        return ;
    }
    
    CCNode* fatherNode = getParent();
    if(!fatherNode)
        return ;
    
    CCPoint touchPointInWorld = pTouch->getLocation();
    CCPoint prePointInWorld = pTouch->getPreviousLocation();
    m_fLastMoveDis = touchPointInWorld.x - prePointInWorld.x;
    
    CCRect  selfRect = boundingBox();
    CCPoint selfOriginInWorld = fatherNode->convertToWorldSpace(selfRect.origin);
    CCRect selfRectInWorld = CCRectMake(selfOriginInWorld.x,selfOriginInWorld.y,selfRect.size.width,selfRect.size.height);
    if(selfRectInWorld.containsPoint(touchPointInWorld) == false)
    {
        touchEnd();
        return;
    }
    refreshItem(m_fLastMoveDis);
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    touchEnd();
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    touchEnd();
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::touchEnd()
{
    if(!m_bDraging)
    {
        return;
    }
    m_bDraging = false;
    
    if(fabsf(m_fLastMoveDis) < 0.5)
    {
        return;
    }
    schedule(schedule_selector(CC3DCircleListLayer::updateItemPos));
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::updateItemPos(float dt)
{
    if(m_bDraging)
	{
		unschedule(schedule_selector(CC3DCircleListLayer::updateItemPos));
        m_fLastMoveDis = 0;
		return;
	}
    m_fLastMoveDis = m_fLastMoveDis * m_fDeacceleratePercent * m_fDeacceleratePercent;
    
    if(fabsf(m_fLastMoveDis) < 0.01)
    {
        unschedule(schedule_selector(CC3DCircleListLayer::updateItemPos));
        m_fLastMoveDis = 0;
        endUpdateToCenter();
        return;
    }
    refreshItem(m_fLastMoveDis);
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::refreshItem(float fMoveDis)
{
    float fMovePercent = fMoveDis/(m_fEllipseA*2);
    float fCircleMoveStepDegree = M_PI * fMovePercent;
    refreshItemWithMoveDegree(fCircleMoveStepDegree);
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::refreshItemWithMoveDegree(float fMoveDegree)
{
    float fMoveX = getContentSize().width*0.5;
    float fMoveY = getContentSize().height*0.5;
    
    float fScaleMax = 1;
    float fScaleMin = 0.5;
    
    int zOrderMax = 1000;
    
    //    float fCameraDegreeMax = 45;
    //    float fCameraDegreeMin = 0;
    
    for(int i=0;i<m_pButtonArray->count();i++)
    {
        CCButton* pButton = (CCButton*)m_pButtonArray->objectAtIndex(i);
        CCString* pFOldDegree = (CCString*)pButton->getUserData();
        float fOldDegree  = pFOldDegree->floatValue();
        
        fOldDegree = fabsf(fOldDegree);
        float fCurDegree = fOldDegree + fMoveDegree;
        if(fCurDegree < 0)
        {
            fCurDegree = 2*M_PI - fabsf(fCurDegree);
        }
        int nCircleCount = (int)fCurDegree/(2*M_PI);
        fCurDegree = fCurDegree - nCircleCount*(2*M_PI);
        //=============================================================
        //=============================================================
        delete pFOldDegree;
        CCString* pString = new CCString();
        pString->initWithFormat("%f",fabs(fCurDegree));
        pButton->setUserData(pString);
        //=============================================================
        //=============================================================
        //
        //
        //
        //=============================================================
        //=============================================================
        float fCircleX = m_fEllipseA * cosf(fCurDegree);
        float fCircleY = m_fEllipseB * sinf(fCurDegree);
        fCircleX += fMoveX;
        fCircleY += fMoveY;
        CCPoint buttonPos = ccp(fCircleX,fCircleY);
        pButton->setPosition(buttonPos);
        //=============================================================
        //=============================================================
        //
        //
        //
        //=============================================================
        //=============================================================
        float fScaleDegree = fCurDegree - M_PI_2;
        float fScalePercent = (1-cosf(fScaleDegree))/2.0;
        float fScale = fScaleMin + (fScaleMax - fScaleMin)*fScalePercent;
        pButton->setScale(fScale);
        int nOrder = zOrderMax * fScalePercent + 1;
        pButton->setZOrder(nOrder);
        //=============================================================
        //=============================================================
        
        m_pSelectedButton =  getSelectedButton();
        if(m_pSelectedButton)
        {
            if(m_pDelegate)
            {
                int nIndex = m_pSelectedButton->getTag();
                m_pDelegate->onCC3DCircleListLayerSelectIndex(nIndex);
            }
        }
        
        //        //=============================================================
        //        //camera
        //        //=============================================================
        //        int flagMove = 1;
        //        if(buttonPos.x > getContentSize().width*0.5)
        //        {
        //            flagMove = 1;
        //        }
        //        else
        //        {
        //            flagMove = -1;
        //        }
        //        float fCameraDegree = fCameraDegreeMin + (fCameraDegreeMax - fCameraDegreeMin)*(1-fScalePercent);
        //        fCameraDegree *= -flagMove;
        //        float radians = (float)CC_DEGREES_TO_RADIANS(fCameraDegree);
        //        float fSin = sinf(radians) * CCCamera::getZEye();
        //        CCCamera* pCamera = pButton->getCamera();
        //        pCamera->setCenterXYZ(fSin, 0, 0);
        //        //=============================================================
        //        //=============================================================
    }
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::moveToCenter(CCButton* pButton,bool bActionFlag)
{
    if(!pButton)
        return;
    stopUpdateItemDegree();
    CCString* pFOldDegree = (CCString*)pButton->getUserData();
    float fOldDegree  = pFOldDegree->floatValue();
    m_fLastMoveDegree = 1.5*M_PI - fOldDegree;
    
    if(m_fLastMoveDegree > M_PI)
    {
        m_fLastMoveDegree = - (2*M_PI - m_fLastMoveDegree);
    }
    if(bActionFlag)
    {
        schedule(schedule_selector(CC3DCircleListLayer::updateItemDegree));
    }
    else
    {
        refreshItemWithMoveDegree(m_fLastMoveDegree);
    }
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::updateItemDegree(float dt)
{
    if(m_bDraging)
    {
        stopUpdateItemDegree();
        return;
    }
   float fTimePercent = dt / m_fToEndActionTime;
   float  m_fStepMoveDegree = m_fLastMoveDegree * fTimePercent;
    float fOldCurDegree = m_fCurMoveDegree;
    m_fCurMoveDegree = m_fCurMoveDegree + m_fStepMoveDegree;
    float fCha = fabs(m_fCurMoveDegree) - fabsf(m_fLastMoveDegree );
    if(fCha > 0)
    {
        CCLog("updateItemDegree:fCha=%f",fCha);
        refreshItemWithMoveDegree(m_fLastMoveDegree-fOldCurDegree);
        stopUpdateItemDegree();
        return;
    }
    refreshItemWithMoveDegree(m_fStepMoveDegree);
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::stopUpdateItemDegree()
{
    unschedule(schedule_selector(CC3DCircleListLayer::updateItemDegree));
    m_fCurMoveDegree = 0;
    m_fLastMoveDegree = 0;
    
}
//=============================================================
//=============================================================
void CC3DCircleListLayer::endUpdateToCenter()
{
    CCButton* pSelButton = getSelectedButton();
    if(!pSelButton)
        return;
    moveToCenter(pSelButton,true);
}
//=============================================================
//=============================================================
CCButton* CC3DCircleListLayer::getSelectedButton()
{
    CCButton* pSelButton = NULL;
    int nSelZOrder = 0;
    for(int i=0;i<m_pButtonArray->count();i++)
    {
        CCButton* pButton = (CCButton*)m_pButtonArray->objectAtIndex(i);
        int nZOrder = pButton->getZOrder();
        if(nZOrder > nSelZOrder)
        {
            pSelButton = pButton;
        }
        nSelZOrder = pSelButton->getZOrder();
    }
    return pSelButton;
}
//=============================================================
//=============================================================


