
/*
 *
 *  Created by wangbin
 *
 */

#include <iostream>
#include "CCCutTouchLayer.h"

CCCutTouchLayer::CCCutTouchLayer():
m_bClipsToBounds(true),
m_bPreventTouch(true)
{
    ignoreAnchorPointForPosition(false);
}
//=============================================================
//=============================================================
CCCutTouchLayer::~CCCutTouchLayer()
{
}
//=============================================================
//=============================================================
CCCutTouchLayer * CCCutTouchLayer::create(const ccColor4B& color, GLfloat width, GLfloat height)
{
    CCCutTouchLayer * pLayer = new CCCutTouchLayer();
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
bool CCCutTouchLayer::initWithColor(const ccColor4B& color, GLfloat w, GLfloat h) 
{
    if(CCLayerColor::initWithColor(color, w, h))
    {
        setTouchEnabled(true);
        return true;
    }
    return false;
}
//=============================================================
//=============================================================
void CCCutTouchLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority,m_bPreventTouch);  
}
//=============================================================
//=============================================================
bool CCCutTouchLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)  //截住touch
{
    if(!isVisible())
    {
        return false;
    }
    
    CCNode* fatherNode = getParent();
    if(!fatherNode)
        return false;
    
    CCPoint touchPointInWorld = touch->getLocation();
    CCRect  selfRect = boundingBox();
    CCPoint selfOriginInWorld = fatherNode->convertToWorldSpace(selfRect.origin);
    CCRect selfRectInWorld = CCRectMake(selfOriginInWorld.x,selfOriginInWorld.y,selfRect.size.width,selfRect.size.height);
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
        CCRect  rect = c->boundingBox();
        CCNode* parentNode = c->getParent();
        if(parentNode)
        {
            CCPoint originInWorld = parentNode->convertToWorldSpace(rect.origin);
            
            CCRect rectInWorld = CCRectMake(originInWorld.x,originInWorld.y,rect.size.width,rect.size.height);
            if(rectInWorld.containsPoint(touchPointInWorld) == false)  // 不在父视图区域内
            {
                return false;
            }
        }
    }
    return true;
}
//=============================================================
//=============================================================
void CCCutTouchLayer::beforeDraw()
{
    if (m_bClipsToBounds)
    {
        CCPoint screenPos = this->convertToWorldSpace(CCPointZero);
        float scaleX = this->getScaleX();
        float scaleY = this->getScaleY();
        for (CCNode *p = m_pParent; p != NULL; p = p->getParent())
        {
            scaleX *= p->getScaleX();
            scaleY *= p->getScaleY();
        }
       CCRect frame = CCRectMake(screenPos.x, screenPos.y, getContentSize().width*scaleX, getContentSize().height*scaleY);
       glEnable(GL_SCISSOR_TEST);
        CCEGLView::sharedOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
    }
}
//=============================================================
//=============================================================
/**
 * retract what's done in beforeDraw so that there's no side effect to
 * other nodes.
 */
void CCCutTouchLayer::afterDraw()
{
    if (m_bClipsToBounds)
    {
        glDisable(GL_SCISSOR_TEST);
    }
}
//=============================================================
//=============================================================
//void CCCutTouchLayer::visit()
//{
//    CCLayerColor::visit();
//    return;
//	// quick return if not visible
//	if (!isVisible())
//    {
//		return;
//    }
//    
//	kmGLPushMatrix();
//	
//    if (m_pGrid && m_pGrid->isActive())
//    {
//        m_pGrid->beforeDraw();
//        this->transformAncestors();
//    }
//    
//	this->transform();
//    this->beforeDraw();
//    
//	if(m_pChildren)
//    {
//		ccArray *arrayData = m_pChildren->data;
//		unsigned int i=0;
//		
//		// draw children zOrder < 0
//		for( ; i < arrayData->num; i++ )
//        {
//			CCNode *child =  (CCNode*)arrayData->arr[i];
//			if ( child->getZOrder() < 0 )
//            {
//				child->visit();
//			}
//            else
//            {
//				break;
//            }
//		}
//		
//		// this draw
//		this->draw();
//		
//		// draw children zOrder >= 0
//		for( ; i < arrayData->num; i++ )
//        {
//			CCNode* child = (CCNode*)arrayData->arr[i];
//			child->visit();
//		}
//        
//	}
//    else
//    {
//		this->draw();
//    }
//    
//    this->afterDraw();
//	if ( m_pGrid && m_pGrid->isActive())
//    {
//		m_pGrid->afterDraw(this);
//    }
//    
//	kmGLPopMatrix();
//}
//=============================================================
//=============================================================
void CCCutTouchLayer::onNotification()
{
}
//=============================================================
//=============================================================

