//
//  CCTextLayer.cpp
//  CCTextLayer
//
//  Created by wangbin on 13-1-9.
//
//

#include "CC3DListLayer.h"
#include "CCLabelStroke.h"
#include "CCButton.h"
#include "CCScrollLayer.h"

CC3DListLayer::CC3DListLayer():
m_fSpaceX(0),
m_pDelegate(NULL),
m_pScrollLayer(NULL),
m_fOriginSpaceX(0),
m_itemSize(CCSizeZero),
m_eListType(eListType_Unknown),
m_pSelectedButton(NULL),
m_fEllipseA(0),
m_fEllipseB(0),
m_fItemScaleMax(1),
m_fItemScaleMin(0.5)
{
    m_pButtonArray = CCArray::create();
    CC_SAFE_RETAIN(m_pButtonArray);
}
//=============================================================
//=============================================================
CC3DListLayer::~CC3DListLayer()
{
    m_pButtonArray->removeAllObjects();
    CC_SAFE_RELEASE(m_pButtonArray);
}
//=============================================================
//=============================================================
CC3DListLayer* CC3DListLayer::create(const ccColor4B& color, GLfloat width, GLfloat height)
{
    CC3DListLayer * pLayer = new CC3DListLayer();
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
bool CC3DListLayer::initWithColor(const ccColor4B& color, GLfloat w, GLfloat h)
{
    if(CCCutTouchLayer::initWithColor(color, w, h))
    {
        m_pScrollLayer = CCScrollLayer::scrollLayerWidthColor(color, CCSizeMake(w, h));
        m_pScrollLayer->setAnchorPoint(ccp(0,0));
        m_pScrollLayer->setPosition(ccp(0,0));
        addChild(m_pScrollLayer);
        
        m_pScrollLayer->setDelegate(this, callfuncN_selector(CC3DListLayer::refreshItem), CCScrollLayer::SSTE_TOUCH_MOVE);
        m_pScrollLayer->setDelegate(this, callfuncN_selector(CC3DListLayer::refreshItem), CCScrollLayer::SSTE_TOUCH_END);
        m_pScrollLayer->setDelegate(this, callfuncN_selector(CC3DListLayer::refreshItem), CCScrollLayer::SSTE_TOUCH_CANCEL);
        
        m_pScrollLayer->setDelegate(this, callfuncN_selector(CC3DListLayer::refreshItem), CCScrollLayer::SSTE_DECELERATE_MOVE);
        m_pScrollLayer->setDelegate(this, callfuncN_selector(CC3DListLayer::decelerateEnd), CCScrollLayer::SSTE_DECELERATE_END);
        
        m_pScrollLayer->setDelegate(this, callfuncN_selector(CC3DListLayer::refreshItem), CCScrollLayer::SSTE_ACTION_MOVE);
        m_pScrollLayer->setDelegate(this, callfuncN_selector(CC3DListLayer::refreshItem), CCScrollLayer::SSTE_ACTION_END);
        return true;
    }
    return false;
}
//=============================================================
//=============================================================
void CC3DListLayer::initDataCoverFlow(float fSpaceX,CCSize itemSize,CCArray* pArray,bool bFlipY)
{
    if(!pArray)
        return;
    
    m_eListType = eListType_CoverFlow;
    
    m_fSpaceX = fSpaceX;
    m_itemSize = itemSize;
    m_fOriginSpaceX = getContentSize().width*0.5 - m_itemSize.width*0.5;
    
    m_pButtonArray->removeAllObjects();
    for(int i=0;i<pArray->count();i++)
    {
        CCSprite* pSprite = (CCSprite*)pArray->objectAtIndex(i);
        CCScale9Sprite* pScaleSprite = CCScale9Sprite::createWithSpriteFrame(pSprite->displayFrame());
        pSprite->setOpacityModifyRGB(true);
        CCButton* pButton = CCButton::create( pScaleSprite);
        pButton->setTag(i);
        pButton->setZoomOnTouchDown(false);
        float X = m_fOriginSpaceX + m_itemSize.width*0.5 + (m_fSpaceX + m_itemSize.width)*i;
        float Y = getContentSize().height*0.5; 
        CCPoint point = ccp(X,Y);
        pButton->setPosition(point);
        pButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CC3DListLayer::clickButton), CCControlEventTouchUpInside);
        m_pScrollLayer->addSubChild(pButton);
        m_pButtonArray->addObject(pButton);
        pButton->setAnchorPoint(ccp(0.5,0));
        //=============================================================
        //flipY
        //=============================================================
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
        //=============================================================
        //=============================================================
        //
        //
        //
        //=============================================================
        //=============================================================
        CCSize buttonSize = pButton->getContentSize();
        char testChr[32];
        sprintf(testChr, "%d",i);
        CCLabelTTF* pTestLabel = CCLabelTTF::create(testChr, "Arial", 52);
        pTestLabel->setColor(ccRED);
        pTestLabel->setPosition(ccp(buttonSize.width*0.5,buttonSize.height*0.5));
        pButton->addChild(pTestLabel);
        //=============================================================
        //=============================================================
    }
    float scrollWidth = m_fOriginSpaceX*2 + (m_fSpaceX + m_itemSize.width)*pArray->count() - m_fSpaceX;
    CCSize scrollSize = CCSizeMake(scrollWidth, getContentSize().height);
    m_pScrollLayer->setScrollContentSize(scrollSize);
}
//=============================================================
//=============================================================
void CC3DListLayer::refreshItemCoverFlow()
{
    //m_fSpaceX = -m_itemSize.width*0.6;
    float itemScaleMax = m_fItemScaleMax;
    float itemScaleMin = m_fItemScaleMin;
    
    int   zOrderMax = 1000;
    
    float fPosXMoveMax = m_itemSize.width*0.5;
    float fPosXMoveMin = m_itemSize.width*0;
    
    float buttonPosY = getContentSize().height*0.5;
    
    float fCameraDegreeMax = 90;
    float fCameraDegreeMin = 45;
    
    float fOpacityMax = 255;
    float fOpacityMin = 255;
    
    CCSize contentSize = getContentSize();
    CCPoint myCenterInWorld = convertToWorldSpace(ccp(contentSize.width*0.5,contentSize.height*0.5));
    
    for(int i=0;i<m_pButtonArray->count();i++)
    {
        CCButton* pButton = (CCButton*)m_pButtonArray->objectAtIndex(i);
        pButton->setColor(ccWHITE);
        
        int nIndex = pButton->getTag();
        float fDataPosX = m_fOriginSpaceX + m_itemSize.width*0.5 + (m_fSpaceX + m_itemSize.width)*nIndex;
        
        CCSize buttonSize = pButton->getContentSize();
        CCPoint buttonCenterInWorld = pButton->getParent()->convertToWorldSpace(ccp(fDataPosX,getPosition().y));
        
        float disToCenter = fabs(buttonCenterInWorld.x - myCenterInWorld.x);
        float toCenterPercent = disToCenter/(contentSize.width*0.5);
        toCenterPercent = toCenterPercent < 0 ? 0 : toCenterPercent;
        toCenterPercent = toCenterPercent > 1 ? 1 : toCenterPercent;
        float percent  = 1 - toCenterPercent;
        //=============================================================
        //scale order opacity
        //=============================================================
        float scale = itemScaleMin + (itemScaleMax - itemScaleMin)*toCenterPercent;
        pButton->setScale(scale);
        int nOrder = zOrderMax * percent;
        pButton->setZOrder(nOrder);
        float fButtonOpacity = fOpacityMin + (fOpacityMax - fOpacityMin)*percent;
        pButton->setOpacity(fButtonOpacity);
        
        int nPriority = -nOrder;
        pButton->setTouchPriority(nPriority);
        //=============================================================
        //=============================================================
        //
        //
        //
        //=============================================================
        //position
        //=============================================================
        int flagMove = disToCenter/(buttonCenterInWorld.x - myCenterInWorld.x);
        float fMovePercent = sinf(toCenterPercent*M_PI);
        fMovePercent = fMovePercent * fMovePercent;
        float moveX = fPosXMoveMin + (fPosXMoveMax - fPosXMoveMin) * fMovePercent;
        moveX *= flagMove;
        float buttonPosX = fDataPosX ;
        buttonPosX += moveX;
        CCPoint newPos = ccp(buttonPosX,buttonPosY);
        pButton->setPosition(newPos);
        //=============================================================
        //=============================================================
        //
        //
        //
        //=============================================================
        //camera
        //=============================================================
        float fCameraDegree = fCameraDegreeMin + (fCameraDegreeMax - fCameraDegreeMin)*toCenterPercent;
        fCameraDegree *= -flagMove;
        float radians = (float)CC_DEGREES_TO_RADIANS(fCameraDegree);
        float fSin = sinf(radians) * CCCamera::getZEye();
        CCCamera* pCamera = pButton->getCamera();
        
        pCamera->setCenterXYZ(fSin, 0, 0);
        //=============================================================
        //=============================================================
        //
        //
        //
        //=============================================================
        //selected
        //=============================================================
        float minDis = m_itemSize.width*0.5 + m_fSpaceX*0.5;
        if(disToCenter < minDis)
        {
            if(m_pSelectedButton != pButton)
            {
                m_pSelectedButton = pButton;
                m_pSelectedButton->setColor(ccRED);
                
                float fCameraDegree = 0;
                float radians = (float)CC_DEGREES_TO_RADIANS(fCameraDegree);
                float fSin = sinf(radians) * CCCamera::getZEye();
                CCCamera* pCamera = m_pSelectedButton->getCamera();
                pCamera->setCenterXYZ(fSin, 0, 0);
                m_pSelectedButton->setScale(itemScaleMax);
                m_pSelectedButton->setPosition(ccp(buttonPosX,buttonPosY-m_itemSize.height*0.1));
                
                m_pSelectedButton = pButton;
                if(m_pDelegate)
                {
                    m_pDelegate->onCC3DListLayerSelectIndex(m_pSelectedButton->getTag());
                }
            }
        }
        //=============================================================
        //=============================================================
    }
}
//=============================================================
//=============================================================
void CC3DListLayer::initDataLine(float fSpaceX,float fSpaceY,CCSize itemSize,CCArray* pArray,bool bFlipY,bool bToBoundsMove)
{
    if(!pArray)
        return;
    m_eListType = eListType_Line;

    m_bToBoundsMove = bToBoundsMove;
    m_fSpaceX = fSpaceX;
    m_itemSize = itemSize;
    m_fOriginSpaceX = getContentSize().width*0.5 - m_itemSize.width*0.5;
    
    m_pButtonArray->removeAllObjects();
    for(int i=0;i<pArray->count();i++)
    {
        CCSprite* pSprite = (CCSprite*)pArray->objectAtIndex(i);
        
        CCScale9Sprite* pScaleSprite = CCScale9Sprite::createWithSpriteFrame(pSprite->displayFrame());
        pSprite->setOpacityModifyRGB(true);
        CCButton* pButton = CCButton::create( pScaleSprite);
        pButton->setTag(i);
        pButton->setZoomOnTouchDown(false);
        float X = m_fOriginSpaceX + m_itemSize.width*0.5 + (m_fSpaceX + m_itemSize.width)*i;
        float Y = fSpaceY + getContentSize().height*0.5;
        CCPoint point = ccp(X,Y);
        pButton->setPosition(point);
        pButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CC3DListLayer::clickButton), CCControlEventTouchUpInside);
        m_pScrollLayer->addSubChild(pButton);
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
    }
    float scrollWidth = m_fOriginSpaceX*2 + (m_fSpaceX + m_itemSize.width)*pArray->count() - m_fSpaceX;
    CCSize scrollSize = CCSizeMake(scrollWidth, getContentSize().height);
    m_pScrollLayer->setScrollContentSize(scrollSize);
}//=============================================================
//=============================================================
void CC3DListLayer::refreshItemLine()
{
    CCSize contentSize = getContentSize();

    float itemScaleMax = m_fItemScaleMax;
    float itemScaleMin = m_fItemScaleMin;
    
    int   zOrderMax = 1000;
    float posXMoveMax = m_itemSize.width*0.5;
    float fToCenterDisMax = contentSize.width*0.5;
    
    CCPoint myCenterInWorld = convertToWorldSpace(ccp(contentSize.width*0.5,contentSize.height*0.5));
    
    for(int i=0;i<m_pButtonArray->count();i++)
    {
        CCButton* pButton = (CCButton*)m_pButtonArray->objectAtIndex(i);
        
        int     nIndex      = pButton->getTag();
        float   fDataPosX   = m_fOriginSpaceX + m_itemSize.width*0.5 + (m_fSpaceX + m_itemSize.width)*nIndex;
        
        CCSize buttonSize = pButton->getContentSize();
        CCPoint buttonCenterInWorld = pButton->getParent()->convertToWorldSpace(ccp(fDataPosX,getPosition().y));
        float disToCenter = fabs(buttonCenterInWorld.x - myCenterInWorld.x);
        float toCenterPercent = disToCenter/fToCenterDisMax;
        toCenterPercent = toCenterPercent < 0 ? 0 : toCenterPercent;
        toCenterPercent = toCenterPercent > 1 ? 1 : toCenterPercent;
        float percent  = 1 - toCenterPercent;
        
        //=============================================================
        //scale order
        //=============================================================
        float scale = itemScaleMin + (itemScaleMax - itemScaleMin)*percent;
        pButton->setScale(scale);
        int nOrder = zOrderMax * percent;
        pButton->setZOrder(nOrder);
        //=============================================================
        //=============================================================
        //
        //
        //
        //=============================================================
        //position
        //=============================================================
        int flagMove = disToCenter/(buttonCenterInWorld.x - myCenterInWorld.x);
        float fMovePercent = sinf(toCenterPercent*M_PI);
        fMovePercent = fMovePercent * fMovePercent;
        float moveX = posXMoveMax * fMovePercent * flagMove;
        float buttonPosX = fDataPosX ;
        if(m_bToBoundsMove)
        {
            buttonPosX += moveX;
        }
        
        
        float buttonPosY = pButton->getPosition().y;
        CCPoint newPos = ccp(buttonPosX,buttonPosY);
        pButton->setPosition(newPos);
        //=============================================================
        //=============================================================
        //
        //
        //
        //=============================================================
        //selected
        //=============================================================
        float minDis = m_itemSize.width*0.5 + m_fSpaceX*0.5;
        if(disToCenter < minDis)
        {
            if(m_pSelectedButton != pButton)
            {
                m_pSelectedButton = pButton;
                if(m_pDelegate)
                {
                    m_pDelegate->onCC3DListLayerSelectIndex(m_pSelectedButton->getTag());
                }
            }
        }
        //=============================================================
        //=============================================================
    }
}

//=============================================================
//=============================================================
void CC3DListLayer::initDataHalfEllipse(float fSpaceX,float fEllipseA,float fEllipseB,CCSize itemSize,CCArray* pArray,bool bFlipY)
{
    m_eListType = eListType_HalfEllipse;
    if(!pArray)
        return;
    m_fSpaceX = fSpaceX;
    m_fEllipseA = fEllipseA;
    m_fEllipseB = fEllipseB;
    m_itemSize = itemSize;
    m_fOriginSpaceX = getContentSize().width*0.5 - m_itemSize.width*0.5;
    
    m_pButtonArray->removeAllObjects();
    for(int i=0;i<pArray->count();i++)
    {
        CCSprite* pSprite = (CCSprite*)pArray->objectAtIndex(i);
        
        CCScale9Sprite* pScaleSprite = CCScale9Sprite::createWithSpriteFrame(pSprite->displayFrame());
        pSprite->setOpacityModifyRGB(true);
        CCButton* pButton = CCButton::create( pScaleSprite);
        pButton->setTag(i);
        pButton->setZoomOnTouchDown(false);
        float X = m_fOriginSpaceX + m_itemSize.width*0.5 + (m_fSpaceX + m_itemSize.width)*i;
        float Y = getContentSize().height*0.5;
        CCPoint point = ccp(X,Y);
        pButton->setPosition(point);
        pButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CC3DListLayer::clickButton), CCControlEventTouchUpInside);
        m_pScrollLayer->addSubChild(pButton);
        m_pButtonArray->addObject(pButton);
        
       // pButton->setAnchorPoint(ccp(0.5,0));
        
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
        CCSize buttonSize = pButton->getContentSize();
        char testChr[32];
        sprintf(testChr, "%d",i+1);
        CCLabelTTF* pTestLabel = CCLabelTTF::create(testChr, "Arial", 52);
        pTestLabel->setColor(ccRED);
        pTestLabel->setPosition(ccp(buttonSize.width*0.5,buttonSize.height*0.5));
        pButton->addChild(pTestLabel);
        
    }
    float scrollWidth = m_fOriginSpaceX*2 + (m_fSpaceX + m_itemSize.width)*pArray->count() - m_fSpaceX;
    CCSize scrollSize = CCSizeMake(scrollWidth, getContentSize().height);
    m_pScrollLayer->setScrollContentSize(scrollSize);
}
//=============================================================
//=============================================================
void CC3DListLayer::refreshItemHalfEllipse()
{
//    float itemScaleMax = 1;
//    float itemScaleMin = 0.2;
    
    float itemScaleMax = m_fItemScaleMax;
    float itemScaleMin = m_fItemScaleMin;
    
    int   zOrderMax = 1000;
    
    float posXMoveMax = m_itemSize.width*0.5;
    
    CCSize contentSize = getContentSize();
    CCPoint myCenterInWorld = convertToWorldSpace(ccp(contentSize.width*0.5,contentSize.height*0.5));
    
    float fToCenterDisMaX = m_fEllipseA*2;
    
    for(int i=0;i<m_pButtonArray->count();i++)
    {
        CCButton* pButton = (CCButton*)m_pButtonArray->objectAtIndex(i);
        
        int nIndex = pButton->getTag();
        float fDataPosX = m_fOriginSpaceX + m_itemSize.width*0.5 + (m_fSpaceX + m_itemSize.width)*nIndex;
        
        CCSize buttonSize = pButton->getContentSize();
        CCPoint buttonCenterInWorld = pButton->getParent()->convertToWorldSpace(ccp(fDataPosX,pButton->getPosition().y));
        
        float disToCenter = fabs(buttonCenterInWorld.x - myCenterInWorld.x);
        int nRightFlag = disToCenter/(buttonCenterInWorld.x - myCenterInWorld.x);
        
        float toCenterPercent = disToCenter/fToCenterDisMaX;
        
        toCenterPercent = toCenterPercent < 0 ? 0 : toCenterPercent;
        toCenterPercent = toCenterPercent > 1 ? 1 : toCenterPercent;

        float fToBoundsPercent  = 1 - toCenterPercent;
      
        float fDegree = 0;
        if(nRightFlag)
        {
           fDegree  = -M_PI_2 + M_PI * toCenterPercent;
        }
        else
        {
            fDegree  = -M_PI_2 + (-M_PI) * toCenterPercent;
        }
        
        float fEllipseX = m_fEllipseA * cosf(fDegree);
        float fEllipseY = m_fEllipseB * sinf(fDegree); 
        
        float buttonPosX =  fEllipseX;
        float buttonPosY =  fEllipseY;
        
        buttonPosY = buttonPosY + m_fEllipseB*2;
        
        CCPoint pointInWorld = convertToWorldSpace(ccp(buttonPosX,buttonPosY));
        CCPoint pointInScroll = m_pScrollLayer->convertToNodeSpace(pointInWorld);
       
        //=============================================================
        //=============================================================
        int flagMove = disToCenter/(buttonCenterInWorld.x - myCenterInWorld.x);
        float fMovePercent = sinf(toCenterPercent*M_PI);
        fMovePercent = powf(fMovePercent, 2);
        float moveX = posXMoveMax * fMovePercent * flagMove;
        pointInScroll.x = fDataPosX + moveX;
        //=============================================================
        //=============================================================
        
        pButton->setPosition(pointInScroll);
        
        float scale = itemScaleMin + (itemScaleMax - itemScaleMin)*fToBoundsPercent;
        pButton->setScale(scale);
        int nOrder = zOrderMax * fToBoundsPercent;
        pButton->setZOrder(nOrder);
        
        //=============================================================
        //selected
        //=============================================================
        float minDis = m_itemSize.width*0.5 + m_fSpaceX*0.5;
        if(disToCenter < minDis)
        {            
            if(m_pSelectedButton != pButton)
            {
                m_pSelectedButton = pButton;
                if(m_pDelegate)
                {
                    m_pDelegate->onCC3DListLayerSelectIndex(m_pSelectedButton->getTag());
                }
            }
        }
        //=============================================================
        //=============================================================
    }
}
//=============================================================
//=============================================================
void CC3DListLayer::initDataEllipse(float fSpaceX,CCSize itemSize,CCArray* pArray,bool bFlipY)
{
    if(!pArray)
        return;
    m_eListType = eListType_Ellipse;

    m_fSpaceX = fSpaceX;
    m_itemSize = itemSize;
    m_fOriginSpaceX = getContentSize().width*0.5 - m_itemSize.width*0.5;
    
    m_pButtonArray->removeAllObjects();
    for(int i=0;i<pArray->count();i++)
    {
        CCSprite* pSprite = (CCSprite*)pArray->objectAtIndex(i);
        
        CCScale9Sprite* pScaleSprite = CCScale9Sprite::createWithSpriteFrame(pSprite->displayFrame());
        pSprite->setOpacityModifyRGB(true);
        CCButton* pButton = CCButton::create( pScaleSprite);
        pButton->setTag(i);
        pButton->setZoomOnTouchDown(false);
        float X = m_fOriginSpaceX + m_itemSize.width*0.5 + (m_fSpaceX + m_itemSize.width)*i;
        float Y = getContentSize().height*0.5;
        CCPoint point = ccp(X,Y);
        pButton->setPosition(point);
        pButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CC3DListLayer::clickButton), CCControlEventTouchUpInside);
        m_pScrollLayer->addSubChild(pButton);
        m_pButtonArray->addObject(pButton);
        //pButton->setAnchorPoint(ccp(0.5,0));
        
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
    }
    float scrollWidth = m_fOriginSpaceX*2 + (m_fSpaceX + m_itemSize.width)*pArray->count() - m_fSpaceX;
    CCSize scrollSize = CCSizeMake(scrollWidth, getContentSize().height);
    m_pScrollLayer->setScrollContentSize(scrollSize);
}
//=============================================================
//=============================================================
void CC3DListLayer::refreshItemEllipse()
{
    float itemScaleMax = 1;
    float itemScaleMin = 0.2;
    int   zOrderMax = 1000;
    
    float posXMoveMax = m_itemSize.width*0.5;
    
    CCSize contentSize = getContentSize();
    CCPoint myCenterInWorld = convertToWorldSpace(ccp(contentSize.width*0.5,contentSize.height*0.5));
    
    float fToCenterDisMaX = m_fEllipseA*2;
    
    for(int i=0;i<m_pButtonArray->count();i++)
    {
        CCButton* pButton = (CCButton*)m_pButtonArray->objectAtIndex(i);
        
        int nIndex = pButton->getTag();
        float fDataPosX = m_fOriginSpaceX + m_itemSize.width*0.5 + (m_fSpaceX + m_itemSize.width)*nIndex;
        
        CCSize buttonSize = pButton->getContentSize();
        CCPoint buttonCenterInWorld = pButton->getParent()->convertToWorldSpace(ccp(fDataPosX,pButton->getPosition().y));
        
        float disToCenter = fabs(buttonCenterInWorld.x - myCenterInWorld.x);
        int nRightFlag = disToCenter/(buttonCenterInWorld.x - myCenterInWorld.x);
        
        float toCenterPercent = disToCenter/fToCenterDisMaX;
        
        toCenterPercent = toCenterPercent < 0 ? 0 : toCenterPercent;
        toCenterPercent = toCenterPercent > 1 ? 1 : toCenterPercent;
        
        float fToBoundsPercent  = 1 - toCenterPercent;
        
        float fDegree = 0;
        if(nRightFlag)
        {
            fDegree  = -M_PI_2 + M_PI * toCenterPercent;
        }
        else
        {
            fDegree  = -M_PI_2 + (-M_PI) * toCenterPercent;
        }
        
        float fEllipseX = m_fEllipseA * cosf(fDegree);
        float fEllipseY = m_fEllipseB * sinf(fDegree);
        
        float buttonPosX =  fEllipseX;
        float buttonPosY =  fEllipseY;
        
        buttonPosY = buttonPosY + m_fEllipseB*2;
        
        CCPoint pointInWorld = convertToWorldSpace(ccp(buttonPosX,buttonPosY));
        CCPoint pointInScroll = m_pScrollLayer->convertToNodeSpace(pointInWorld);
        
        //=============================================================
        //=============================================================
        int flagMove = disToCenter/(buttonCenterInWorld.x - myCenterInWorld.x);
        float fMovePercent = sinf(toCenterPercent*M_PI);
        fMovePercent = powf(fMovePercent, 2);
        float moveX = posXMoveMax * fMovePercent * flagMove;
        pointInScroll.x = fDataPosX + moveX;
        //=============================================================
        //=============================================================
        
        pButton->setPosition(pointInScroll);
        
        float scale = itemScaleMin + (itemScaleMax - itemScaleMin)*fToBoundsPercent;
        pButton->setScale(scale);
        int nOrder = zOrderMax * fToBoundsPercent;
        pButton->setZOrder(nOrder);
        
        //=============================================================
        //selected
        //=============================================================
        float minDis = m_itemSize.width*0.5 + m_fSpaceX*0.5;
        if(disToCenter < minDis)
        {
            m_pSelectedButton = pButton;
        }
        //=============================================================
        //=============================================================
    }
    
    
    //    float itemScaleMax = 1;
    //    float itemScaleMin = 0;
    //    int   zOrderMax = 1000;
    //
    //   // m_fSpaceX = -m_itemSize.width*0.8;
    //
    //    //float posXMoveMax = m_itemSize.width*0.5;
    //
    //    float fEllipseA = getContentSize().width*0.5;
    //    float fEllipseB = getContentSize().height*0.5;
    //
    //    CCSize contentSize = getContentSize();
    //    CCPoint myCenterInWorld = convertToWorldSpace(ccp(contentSize.width*0.5,contentSize.height*0.5));
    //
    //    for(int i=0;i<m_pButtonArray->count();i++)
    //    {
    //        CCButton* pButton = (CCButton*)m_pButtonArray->objectAtIndex(i);
    //
    //        int nIndex      = pButton->getTag();
    //        float fDataPosX = m_fOriginSpaceX  + m_itemSize.width*0.5 + (m_fSpaceX + m_itemSize.width)*nIndex;
    //
    //        CCSize buttonSize = pButton->getContentSize();
    //        CCPoint buttonCenterInWorld = pButton->getParent()->convertToWorldSpace(ccp(fDataPosX,getPosition().y));
    //
    //        float disToCenter = fabs(buttonCenterInWorld.x - myCenterInWorld.x);
    //        float toCenterPercent = disToCenter/(contentSize.width);
    //        toCenterPercent = toCenterPercent < 0 ? 0 : toCenterPercent;
    //        toCenterPercent = toCenterPercent > 1 ? 1 : toCenterPercent;
    //        float percent  = 1 - toCenterPercent;
    //        percent = percent < 0 ? 0 : percent;
    //        percent = percent > 1 ? 1 : percent;
    //
    //        float fScalePercent = sinf(percent*M_PI_2);
    //        float scale = itemScaleMin + (itemScaleMax - itemScaleMin)*fScalePercent;
    //        pButton->setScale(scale);
    //        int nOrder = zOrderMax * fScalePercent;
    //        pButton->setZOrder(nOrder);
    //
    //
    //        float buttonPosX = fDataPosX ;
    //
    //        float fDegree = M_PI_2;
    //        if(toCenterPercent > 0.5)
    //        {
    //            toCenterPercent = 1 - toCenterPercent;
    //        }
    //        float fEllipseX = toCenterPercent*fEllipseA;
    //
    //        float fEllipseY = fEllipseB*(sin(acos(toCenterPercent*fDegree)));
    //
    //        if(toCenterPercent > 0.5)
    //        {
    //            fEllipseY -= fEllipseY;
    //        }
    //
    //        float posY = getContentSize().height - fEllipseY;
    //        //float posY =  fEllipseY;
    //        CCPoint yInWorld = convertToWorldSpace(ccp(fEllipseX,posY));
    //        CCPoint yInScroll = m_pScrollLayer->convertToNodeSpace(yInWorld);
    //        CCPoint newPos = ccp(buttonPosX,yInScroll.y);
    //        pButton->setPosition(newPos);
    //        
    //    }
}
//=============================================================
//=============================================================
void CC3DListLayer::refreshItem()
{
    switch (m_eListType)
    {
        case eListType_HalfEllipse:
        {
            refreshItemHalfEllipse();
        }
            break;
        case eListType_CoverFlow:
        {
            refreshItemCoverFlow();
        }
            break;
        case eListType_Ellipse:
        {
            refreshItemEllipse();
        }
            break;
        case eListType_Line:
        {
            refreshItemLine();
        }
            break;
            
        default:
            break;
    }
}
//=============================================================
//=============================================================
void CC3DListLayer::clickButton(CCButton* pButton)
{
    int index = pButton->getTag();
    CCLog("%d",index);
    
    if(m_pSelectedButton == pButton)
    {
        if(m_pDelegate)
        {
            m_pDelegate->onCC3DListLayerClickSelect(index);
        }
    }
    else
    {
        moveToCenter(pButton,true);
    }
}
//=============================================================
//=============================================================
void CC3DListLayer::moveToCenter(CCButton* pButton,bool bAction)
{
    if(!pButton)
        return;
    CCSize contentSize = getContentSize();
    CCPoint buttonPos = pButton->getPosition();
    int nIndex = pButton->getTag();
    float fDataPosX = m_fOriginSpaceX + m_itemSize.width*0.5 + (m_fSpaceX + m_itemSize.width)*nIndex;

    CCPoint offset = ccp(fDataPosX-(m_fOriginSpaceX+pButton->getContentSize().width*0.5),0);
    m_pScrollLayer->setContentOffset(offset, bAction);
}
//=============================================================
//=============================================================
void CC3DListLayer::decelerateMove()
{
}
//=============================================================
//=============================================================
void CC3DListLayer::decelerateEnd()
{
    switch (m_eListType)
    {
        case eListType_Line:
        {
            moveToCenter(m_pSelectedButton,false);
            refreshItem();
        }
            break;
        case eListType_Ellipse:
        {
        }
            break;
        case eListType_CoverFlow:
        {
            moveToCenter(m_pSelectedButton,false);
            refreshItem();
        }
            break;
        case eListType_HalfEllipse:
        {
            moveToCenter(m_pSelectedButton,false);
            refreshItem();
        }
            break;
            
        default:
            break;
    }
}
//=============================================================
//=============================================================
void CC3DListLayer::actionMove()
{
}
//=============================================================
//=============================================================
void CC3DListLayer::actionEnd()
{
}
//=============================================================
//=============================================================




