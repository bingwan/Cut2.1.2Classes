

/*
 *
 *  Created by wangbin
 *
 */

#include <iostream>
#include "CCButton.h"


CCButton::CCButton():
m_dragDistancePercent(0.25),
m_beginPoint(CCPointZero),
m_bPreventTouchFlag(true)
{
    setAdjustBackgroundImage(false);
}
//=============================================================
//=============================================================
CCButton::~CCButton()
{
} 
//=============================================================
//=============================================================
void CCButton::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, m_bPreventTouchFlag);
}
//=============================================================
//=============================================================
bool CCButton::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
//CCControlButton
//    if (!isTouchInside(pTouch) || !isEnabled() || !isVisible())
//    {
//        return false;
//    }
//    
//    m_isPushed = true;
//    this->setHighlighted(true);
//    sendActionsForControlEvents(CCControlEventTouchDown);
//    return true;
    
    if (!isVisible()) // 父视图改变visible 不影响孩子的变量   //不可见不处理touch
    { 
        return false;
    }
    CCNode* fatherNode = getParent();
    if(!fatherNode)
        return false;
    CCPoint touchPointInWorld = pTouch->getLocation();
    CCRect  selfRect = boundingBox();
    CCPoint selfOriginInWorld = fatherNode->convertToWorldSpace(selfRect.origin);
    CCRect  selfRectInWorld = CCRectMake(selfOriginInWorld.x,selfOriginInWorld.y,selfRect.size.width,selfRect.size.height);
    if(selfRectInWorld.containsPoint(touchPointInWorld) == false)
    {
        return false;
    }
    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent()) 
    {
        if (c->isVisible() == false) //若父亲有不可见的则不处理touch
        {
            return false;
        }
        
//        CCRect  rect = c->boundingBox();
//        CCNode* parentNode = c->getParent();
//        if(parentNode)
//        {
//            CCPoint originInWorld = parentNode->convertToWorldSpace(rect.origin);
//            
//            CCRect rectInWorld = CCRectMake(originInWorld.x,originInWorld.y,rect.size.width,rect.size.height);
//            if(rectInWorld.containsPoint(touchPointInWorld) == false)
//            {
//                return false;  // 不在父视图区域内 按钮不响应
//            }
//        }
    }
    if (!isTouchInside(pTouch) || !isEnabled())
    {
        return false;
    }
    m_isPushed = true;
    setHighlighted(true);
    sendActionsForControlEvents(CCControlEventTouchDown);
    
    m_beginPoint = pTouch->getLocation();
    return true;
}
//=============================================================
//=============================================================
void CCButton::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
//CCControlButton
//    if (!isEnabled() || !isPushed() || isSelected())
//    {
//        if (isHighlighted())
//        {
//            setHighlighted(false);
//        }
//        return;
//    }
//    
//    bool isTouchMoveInside = isTouchInside(pTouch);
//    if (isTouchMoveInside && !isHighlighted())
//    {
//        setHighlighted(true);
//        sendActionsForControlEvents(CCControlEventTouchDragEnter);
//    }
//    else if (isTouchMoveInside && isHighlighted())
//    {
//        sendActionsForControlEvents(CCControlEventTouchDragInside);
//    }
//    else if (!isTouchMoveInside && isHighlighted())
//    {
//        setHighlighted(false);
//        
//        sendActionsForControlEvents(CCControlEventTouchDragExit);
//    }
//    else if (!isTouchMoveInside && !isHighlighted())
//    {
//        sendActionsForControlEvents(CCControlEventTouchDragOutside);
//    }
    
    CCPoint movePoint = pTouch->getLocation();
    float distanceX = fabsf(movePoint.x-m_beginPoint.x);
    float distanceY = fabsf(movePoint.y-m_beginPoint.y);
    CCSize contentSize = getContentSize();
    if(distanceX >= contentSize.width*m_dragDistancePercent || distanceY >= contentSize.height*m_dragDistancePercent)
    {
        if(isHighlighted())
        {
            m_eState = CCControlStateNormal;
            m_isPushed = false;
            setHighlighted(false);
        }
        return;
    }    
    CCControlButton::ccTouchMoved(pTouch, pEvent);
}
//=============================================================
//=============================================================
void CCButton::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
//CCControlButton
//    m_isPushed = false;
//    setHighlighted(false);
//    
//    
//    if (isTouchInside(pTouch))
//    {
//        sendActionsForControlEvents(CCControlEventTouchUpInside);
//    }
//    else
//    {
//        sendActionsForControlEvents(CCControlEventTouchUpOutside);
//    }
    
    m_eState = CCControlStateNormal;
    m_isPushed = false;
    bool oldHigh = isHighlighted();
    setHighlighted(false);
    
    if(oldHigh)
    {
        if (isTouchInside(pTouch))
        {
            sendActionsForControlEvents(CCControlEventTouchUpInside);        
        }
        else
        {
            sendActionsForControlEvents(CCControlEventTouchUpOutside);        
        }
    }
}
//=============================================================
//=============================================================
CCButton* CCButton::create(CCNode* label, CCScale9Sprite* backgroundSprite)
{
    CCButton *pRet = new CCButton();
    pRet->initWithLabelAndBackgroundSprite(label, backgroundSprite);
    pRet->autorelease();
    pRet->setAdjustBackgroundImage(false);
    return pRet;
}
//=============================================================
//=============================================================
CCButton* CCButton::create(std::string title, const char * fontName, float fontSize)
{
    CCButton *pRet = new CCButton();
    pRet->initWithTitleAndFontNameAndFontSize(title, fontName, fontSize);
    pRet->autorelease();
    pRet->setAdjustBackgroundImage(false);
    return pRet;
}
//=============================================================
//=============================================================
CCButton* CCButton::create(CCScale9Sprite* sprite)
{
    CCButton *pRet = new CCButton();
    pRet->initWithBackgroundSprite(sprite);
    pRet->autorelease();
    pRet->setAdjustBackgroundImage(false);
    return pRet;
}
//=============================================================
//=============================================================
void CCButton::setEnabled(bool enabled)
{
    CCControl::setEnabled(enabled);
    if(enabled)
    {
        m_eState = CCControlStateNormal;
    }
    else
    {
        m_eState = CCControlStateDisabled;
    }
    needsLayout();
}
//=============================================================
//=============================================================
void CCButton::setSelected(bool enabled)
{
    CCControl::setSelected(enabled);
    if(enabled)
    {
        m_eState = CCControlStateSelected;
    }
    else
    {
        m_eState = CCControlStateNormal;
    }
    needsLayout();
}
//=============================================================
//=============================================================
void CCButton::setHighlighted(bool enabled)
{
    CCControlButton::setHighlighted(enabled);
    
    if(enabled)
    {
        m_eState = CCControlStateHighlighted;
    }
    else
    {
        m_eState = CCControlStateNormal;
    }
    needsLayout();
}
//=============================================================
//=============================================================
void CCButton::needsLayout()
{
   CCControlButton::needsLayout();
}
//=============================================================
//=============================================================
void CCButton::setBackgroundSpriteForState(CCScale9Sprite* sprite, CCControlState state)
{
//    CCSize oldPreferredSize = m_preferredSize;
//    
//    CCScale9Sprite* previousBackgroundSprite = (CCScale9Sprite*)m_backgroundSpriteDispatchTable->objectForKey(state);
//    if (previousBackgroundSprite)
//    {
//        removeChild(previousBackgroundSprite, true);
//        m_backgroundSpriteDispatchTable->removeObjectForKey(state);
//    }
//    
//    m_backgroundSpriteDispatchTable->setObject(sprite, state);
//    sprite->setVisible(false);
//    sprite->setAnchorPoint(ccp(0.5f, 0.5f));
//    addChild(sprite);
//    
//    if (this->m_preferredSize.width != 0 || this->m_preferredSize.height != 0)
//    {
//        if (oldPreferredSize.equals(m_preferredSize))
//        {
//            // Force update of preferred size
//            sprite->setPreferredSize(CCSizeMake(oldPreferredSize.width+1, oldPreferredSize.height+1));
//        }
//        
//        sprite->setPreferredSize(this->m_preferredSize);
//    }
//    
//    // If the current state if equal to the given state we update the layout
//    if (getState() == state)
//    {
//        needsLayout();
//    }
    
    CCDictionary* Pdic = (CCDictionary*)getBackgroundSpriteDispatchTable();
    CCScale9Sprite* previousSprite = (CCScale9Sprite*)Pdic->objectForKey(state);
    if (previousSprite)
    {
        removeChild(previousSprite, true);
        ((CCDictionary*)getBackgroundSpriteDispatchTable())->removeObjectForKey(state);
    }
    
    ((CCDictionary*)getBackgroundSpriteDispatchTable())->setObject(sprite, state);
    sprite->setVisible(false);
    sprite->setAnchorPoint(ccp(0.5f, 0.5f));
    addChild(sprite);
        
    setBackgroundSprite(getBackgroundSpriteForState(m_eState));  // add
    
    if (this->m_preferredSize.width != 0 || this->m_preferredSize.height != 0)
    {
        sprite->setPreferredSize(this->m_preferredSize);
    }
    
    // If the current state if equal to the given state we update the layout
    if (getState() == state)
    {
        needsLayout();
    }
}
//=============================================================
//=============================================================
