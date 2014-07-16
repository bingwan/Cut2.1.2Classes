
/*
 *
 *  Created by wangbin
 *
 */

#include "CCScrollLayer.h"
USING_NS_CC;

CCScrollLayer::CCScrollLayer():
m_beginPoint(CCPointZero),
m_containBeginTouchPointFlag(false),
m_contentLayer(NULL),
m_pagingEnable(false),
m_toBoundsDuration(0.3),
m_nextPageDuration(0.3),
m_pageColumnsIndex(0), //列
m_pageRowIndex(0), //行
m_pageMovePercent(0.25),
m_delegate(NULL),
m_touchBeginSelector(NULL),
m_touchMoveSelector(NULL),
m_touchEndSelector(NULL),
m_actionEndSelector(NULL),
m_touchCancelSelector(NULL),
m_endFlag(false),
m_boundsDragDistance(vertex2(30,30)),           //超出边界的距离
m_HMoveAbleFlag(false),
m_VMoveAbleFlag(false),
m_decelerateBeginSelector(NULL),
m_decelerateMoveSelector(NULL),
m_decelerateEndSelector(NULL),
m_actionBeginSelector(NULL),
m_actionMoveSelector(NULL),
m_decelerateFlag(false),
m_scrollDotVSprite(NULL),
m_scrollDotHSprite(NULL),
m_percentOfMoveWhenDragOut(0.25),
m_bShowVDot(false),
m_bShowHDot(false),
m_2fLastMoveDis(vertex2(0, 0)),
m_fDeceleratePercent(0.95)
{
   ignoreAnchorPointForPosition(false);
}
//=============================================================
//=============================================================
CCScrollLayer::~CCScrollLayer()
{
}
//=============================================================
//=============================================================
CCScrollLayer* CCScrollLayer::scrollLayerWidthColor(const ccColor4B color, CCSize size)
{
    CCScrollLayer* scrollLayer = CCScrollLayer::create();
    scrollLayer->initWithColorWidthHeight(color, size.width, size.height);
    return  scrollLayer;
}
//=============================================================
//=============================================================
bool CCScrollLayer::initWithColorWidthHeight(const ccColor4B color, GLfloat width, GLfloat height)
{
    CCLayerColor::initWithColor(color, width, height);
    setTouchEnabled(true);
    
    m_contentLayer = CCLayer::create();
    m_contentLayer->setAnchorPoint(ccp(0,0));
    m_contentLayer->setPosition(ccp(0,0));
    addChild(m_contentLayer);
    
    setScrollContentSize(getContentSize());
    setPageSize(getContentSize());
    
    
    CCPoint position = ccp(0,m_contentLayer->getContentSize().height - getContentSize().height);
    setContentOffset(position,false);
    
    m_pageColumnsIndex = (m_contentLayer->getContentSize().height-1)/m_pageSize.height;
    
    //滑动条
    m_scrollDotVSprite = CCSprite::create("scroll_dot.png");
    //m_scrollDotVSprite->setAnchorPoint(ccp(0.5,0));
    CCSize size = getContentSize();
    CCSize dotSize = m_scrollDotVSprite->getContentSize();
    m_scrollDotVSprite->setPosition(ccp(size.width-dotSize.width,size.height-dotSize.height));
    addChild(m_scrollDotVSprite);

    m_scrollDotHSprite = CCSprite::create("scroll_dot.png");
    //m_scrollDotHSprite->setAnchorPoint(ccp(0.5,0));
    m_scrollDotHSprite->setPosition(ccp(0,0));
    m_scrollDotHSprite->setPosition(ccp(dotSize.width,dotSize.height));
    addChild(m_scrollDotHSprite);
    
    m_scrollDotVSprite->setVisible(false);
    m_scrollDotHSprite->setVisible(false);
    
    m_scrollDotVSprite->setOpacity(255*0.5);
    m_scrollDotHSprite->setOpacity(255*0.5);
    
	return true;
}
//=============================================================
//=============================================================
void CCScrollLayer::registerWithTouchDispatcher(void)
{    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority-1, false);
}
//=============================================================
//=============================================================
void CCScrollLayer::setContentOffset(CCPoint offsetPoint,bool actionFlag)
{
    CCPoint position = ccp(-offsetPoint.x,-offsetPoint.y);
    if(actionFlag == true)
    {
        moveToAction(position, m_toBoundsDuration);
    }
    else
    {
        m_contentLayer->setPosition(position);
    }
}
//=============================================================
//=============================================================
void CCScrollLayer::setEndFlag(bool flag)
{
}
//=============================================================
//=============================================================
CCPoint CCScrollLayer::getContentOffset()
{
    CCPoint position = m_contentLayer->getPosition();
    CCPoint offset = ccp(-position.x,-position.y);
    return offset;
}
//=============================================================
//=============================================================
void CCScrollLayer::refreshDotPosition()
{
    if(m_bShowHDot)
    {
        m_scrollDotHSprite->setVisible(true);
        float dotWidthPercent = getContentSize().width/m_contentLayer->getContentSize().width;
        float dotWidth = dotWidthPercent* getContentSize().width;
        m_scrollDotHSprite->setScaleX(dotWidth/m_scrollDotVSprite->getContentSize().width);
        
        
        CCPoint offSetPoint =  getContentOffset();
        float maxSpace = m_contentLayer->getContentSize().width - getContentSize().width;
        
        if(offSetPoint.x!=0)
        {
            float positionPercent = offSetPoint.x/maxSpace;
            float percentwidth = positionPercent* (getContentSize().width - dotWidth);
            m_scrollDotHSprite->setPosition(ccp(dotWidth/2+percentwidth,m_scrollDotHSprite->getPosition().y));
        }
    }
    if(m_bShowVDot)
    {
        m_scrollDotVSprite->setVisible(true);
        float dotHeightPercent = getContentSize().height/m_contentLayer->getContentSize().height;
        float dotHeight = dotHeightPercent* getContentSize().height;
        m_scrollDotVSprite->setScaleY(dotHeight/m_scrollDotVSprite->getContentSize().height);
        
        float maxSpace = m_contentLayer->getContentSize().height - getContentSize().height;
        CCPoint offSetPoint =  getContentOffset();
        
        if(offSetPoint.y!=0)
        {
            float positionPercent = offSetPoint.y/maxSpace;
            float percentHeight = positionPercent*(getContentSize().height-dotHeight);
            m_scrollDotVSprite->setPosition(ccp(m_scrollDotVSprite->getPosition().x,dotHeight/2+percentHeight));
        }
    }
}
//=============================================================
//=============================================================
void CCScrollLayer::setScrollContentSize(CCSize contentSize)
{
    m_contentLayer->setContentSize(contentSize);
    CCPoint position = ccp(0,m_contentLayer->getContentSize().height - getContentSize().height);
    setContentOffset(position,false);
    
    CCSize size = getContentSize();
    if(contentSize.width < size.width)      contentSize.width = size.width;
    if(contentSize.height < size.height)    contentSize.height = size.height;
    m_contentLayer->setContentSize(contentSize);
    
    if(m_contentLayer->getContentSize().width > getContentSize().width)    //横竖是否可移动
    {
        m_HMoveAbleFlag = true;
        m_bShowHDot = true;
    }
    if(m_contentLayer->getContentSize().height > getContentSize().height)
    {
        m_VMoveAbleFlag = true;
        m_bShowVDot = true;
    }
    //设置边条长度
    refreshDotPosition();    
}
//=============================================================
//=============================================================
void CCScrollLayer::setPageSize(CCSize pageSize)
{
    m_pageSize = pageSize;
    m_pageColumnsIndex = (m_contentLayer->getContentSize().height-1)/m_pageSize.height;
}
//=============================================================
//=============================================================
CCSize CCScrollLayer::getScrollContentSize()
{
    return  m_contentLayer->getContentSize();
}
//=============================================================
//=============================================================
void CCScrollLayer::beforeDraw()
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
//=============================================================
//=============================================================
void CCScrollLayer::afterDraw()
{
    glDisable(GL_SCISSOR_TEST);
}
//=============================================================
//=============================================================
void CCScrollLayer::visit()
{
	// quick return if not visible
	if (!isVisible())
    {
		return;
    }

	kmGLPushMatrix();

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        this->transformAncestors();
    }

	this->transform();
    this->beforeDraw();

	if(m_pChildren)
    {
		ccArray *arrayData = m_pChildren->data;
		unsigned int i=0;

		// draw children zOrder < 0
		for( ; i < arrayData->num; i++ )
        {
			CCNode *child =  (CCNode*)arrayData->arr[i];
			if ( child->getZOrder() < 0 )
            {
				child->visit();
			}
            else
            {
				break;
            }
		}

		// this draw
		this->draw();

		// draw children zOrder >= 0
		for( ; i < arrayData->num; i++ )
        {
			CCNode* child = (CCNode*)arrayData->arr[i];
			child->visit();
		}

	}
    else
    {
		this->draw();
    }

    this->afterDraw();
	if ( m_pGrid && m_pGrid->isActive())
    {
		m_pGrid->afterDraw(this);
    }

	kmGLPopMatrix();
}
//=============================================================
//=============================================================
void CCScrollLayer::addSubChild(CCNode* node)
{
     m_contentLayer->addChild(node);
}
//=============================================================
//=============================================================
void CCScrollLayer::addSubChild(CCNode* node,int zOrder)
{
    m_contentLayer->addChild(node,zOrder);
}
//=============================================================
//=============================================================
void CCScrollLayer::moveToAction(CCPoint toPoint, float duration)
{
    sendSelector(SSTE_ACTION_BEGIN);

    
    CCActionInterval* moveTo = CCMoveTo::create(duration, toPoint);
    CCFiniteTimeAction* seq = CCSequence::create(moveTo,
                                          CCCallFuncN::create( this, callfuncN_selector(CCScrollLayer::scrollDidEnd)),
                                          NULL);
	m_contentLayer->runAction(seq);
    schedule(schedule_selector(CCScrollLayer::actionTick));
}
//=============================================================
//=============================================================
void CCScrollLayer::actionTick(float dt)
{
    sendSelector(SSTE_ACTION_MOVE);
    refreshDotPosition();
}
//=============================================================
//=============================================================
#pragma mark 翻页 边界归位 时停止滑动
void CCScrollLayer::scrollDidEnd()
{
    unschedule(schedule_selector(CCScrollLayer::actionTick));
    sendSelector(SSTE_ACTION_END);
}
void CCScrollLayer::refreshPosition()
{
    moveToEndPosition(false, m_contentLayer->getPosition());
}
//=============================================================
//=============================================================
void CCScrollLayer::refreshDragOrientation(CCPoint toPoint)
{
    m_boundsDragOriation.HOriention = DOE_UNKNOWN;
    m_boundsDragOriation.VOriention = DOE_UNKNOWN;
    
    CCSize contentlayerSize = m_contentLayer->getContentSize();
    if(toPoint.x >= 0) //向右拖出边界
    {
        if(toPoint.x >= m_boundsDragDistance.x)
        {
            m_boundsDragOriation.HOriention = DOE_RIGHT;
        }
    }
    else if(toPoint.x <= -(contentlayerSize.width - getContentSize().width))  //向左拖出边界
    {
        float dragDistance = fabsf(-(contentlayerSize.width - getContentSize().width)-toPoint.x);
        if(dragDistance >= m_boundsDragDistance.x)
        {
            m_boundsDragOriation.HOriention = DOE_LEFT;
        }
    }
    
    if(toPoint.y >= 0) //向上拖出边界
    {
        if(toPoint.y >= m_boundsDragDistance.y)
        {
            m_boundsDragOriation.VOriention = DOE_UP;
        }
    }
    else if(toPoint.y <= -(contentlayerSize.height - getContentSize().height)) //向下拖出边界
    {
        float dragDistance = fabsf(-(contentlayerSize.height - getContentSize().height) - toPoint.y);
        if(dragDistance >= m_boundsDragDistance.y)
        {
            m_boundsDragOriation.VOriention = DOE_DOWN;
        }
    }
}
//=============================================================
//=============================================================
bool CCScrollLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!isVisible()) // 父视图改变visible 不影响孩子的变量   //不可见依然接收touch
    { 
        return false;
    }
    
    CCNode* fatherNode = getParent();
    if(!fatherNode)
        return false;
    
    CCPoint touchPointInWorld = pTouch->getLocation();
    
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
            if(rectInWorld.containsPoint(touchPointInWorld) == false)
            {
                return false;
            }
        }
        
    }
    
   // CCSetIterator it = pTouches->begin();
    CCTouch* touch = pTouch;
	m_beginPoint = touch->getLocation();
    
    CCNode* parent = getParent();
    CCPoint localPoint = parent->convertToNodeSpace(m_beginPoint);
    CCRect boundRect = boundingBox();
    
    bool unmoveAbleFlag = getContentSize().equals(m_contentLayer->getContentSize());// CCSize::CCSizeEqualToSize(getContentSize(),m_contentLayer->getContentSize());
    if(boundRect.containsPoint(localPoint) && unmoveAbleFlag==false) //点击开始在区域内
    {
        m_containBeginTouchPointFlag = true;
        m_2fLastMoveDis = vertex2(0, 0);
        
        sendSelector(SSTE_TOUCH_BEGIN);
       
        if(m_decelerateFlag)
        {
            m_decelerateFlag = false;
            unschedule(schedule_selector(CCScrollLayer::decelerateTick));
            
        }
        moveToEndPosition(false,m_contentLayer->getPosition());
        return true;
    }
    else
    {
        m_containBeginTouchPointFlag = false;
        return false;
    }
}
//=============================================================
//=============================================================
void CCScrollLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	//CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//CCSetIterator it = pTouches->begin();
    CCTouch* touch = pTouch;
	
	CCPoint movePoint = touch->getLocation();
	CCPoint prePoint = touch->getPreviousLocation();
    
    if(!m_containBeginTouchPointFlag)
        return;
    
    float fMoveX = movePoint.x - prePoint.x;
    float fMoveY = movePoint.y - prePoint.y;
    m_2fLastMoveDis = vertex2(fMoveX, fMoveY);
    
    CCNode* parent = getParent();
    CCPoint localPoint = parent->convertToNodeSpace(movePoint);
    CCRect boundRect = boundingBox();
    
    if(boundRect.containsPoint(localPoint))
    {
        float moveX = movePoint.x - prePoint.x;
        float moveY = movePoint.y - prePoint.y;
        
        CCPoint movePoint = CCPointZero;
        
        if(m_HMoveAbleFlag)    //横竖是否可移动
        {
            if(m_boundsDragOriation.HOriention != DOE_UNKNOWN) //没超出边界时
            {
                moveX *= m_percentOfMoveWhenDragOut;
            }
            movePoint = ccp(m_contentLayer->getPosition().x + moveX,m_contentLayer->getPosition().y);
            
        }
        if(m_VMoveAbleFlag)
        {
            if(m_boundsDragOriation.VOriention != DOE_UNKNOWN) //没超出边界时
            {
                moveY *= m_percentOfMoveWhenDragOut;
            }
            movePoint = ccp(movePoint.x,m_contentLayer->getPosition().y + moveY);
        }
        
        refreshDragOrientation(movePoint);     //移动时刷新数据
        m_contentLayer->setPosition(movePoint);  
        refreshDotPosition();
        sendSelector(SSTE_TOUCH_MOVE);
                  
    }
    else //手势移动到边界之外  可能不进入
    {
        m_containBeginTouchPointFlag = false;
        if(m_pagingEnable == true)
        {
            nextPageAction(localPoint);
        }
        else
        {
            moveToEndPosition(false,m_contentLayer->getPosition());
            sendSelector(SSTE_TOUCH_END);
           // beginDeclerate();  //开始减速
        }
    }
    
}

//=============================================================
//=============================================================
void CCScrollLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCTouch* touch = pTouch;
	CCPoint endPoint = touch->getLocation();
    
    CCNode* parent = getParent();
    if(!parent)
    {
        return;
    }
    CCPoint localPoint = parent->convertToNodeSpace(endPoint);
    CCRect boundRect = boundingBox();
    
    if(boundRect.containsPoint(localPoint) && m_containBeginTouchPointFlag)  //结束点是否在区域内
    {
        if(m_pagingEnable == true)
        {
            nextPageAction(localPoint);
        }
        else
        {
            beginDeclerate();  //开始减速
        }
        sendSelector(SSTE_TOUCH_CANCEL);
    }
    else
    {
        if(m_pagingEnable == true)
        {
            //nextPageAction(localPoint);
            setPageRowCount(m_pageRowIndex+1,true);
            setPageColumnsCount(m_pageColumnsIndex+1,true);
        }
        else
        {
            moveToEndPosition(true, m_contentLayer->getPosition());
        }
    }
}
//=============================================================
//=============================================================
void CCScrollLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}
//=============================================================
//=============================================================
#pragma  翻页
void CCScrollLayer::nextPageAction(CCPoint endPoint)
{
    int allPageRow = (m_contentLayer->getContentSize().width-1)/m_pageSize.width + 1;
    int allPageColumns = (m_contentLayer->getContentSize().height-1)/m_pageSize.height + 1;
    
    CCPoint toPoint = ccp(-m_pageRowIndex*m_pageSize.width,-m_pageColumnsIndex*m_pageSize.height);
    
    if(endPoint.x - m_beginPoint.x > m_pageSize.width*m_pageMovePercent )
    {
        m_pageRowIndex--;
        m_pageRowIndex = m_pageRowIndex < 0 ? 0 : m_pageRowIndex;
        toPoint = ccp(-m_pageRowIndex*m_pageSize.width,toPoint.y);
    }
    else if(endPoint.x - m_beginPoint.x < -m_pageSize.width*m_pageMovePercent)
    {
        m_pageRowIndex++;
        int lastPageRowIndex = allPageRow-1;
        m_pageRowIndex = m_pageRowIndex > lastPageRowIndex ? lastPageRowIndex : m_pageRowIndex;
        toPoint = ccp(-m_pageRowIndex*m_pageSize.width,toPoint.y);
    }
    if(endPoint.y - m_beginPoint.y > m_pageSize.height* m_pageMovePercent )
    {
        m_pageColumnsIndex--;
        m_pageColumnsIndex = m_pageColumnsIndex < 0 ? 0 : m_pageColumnsIndex;
        toPoint = ccp(toPoint.x,-m_pageColumnsIndex*m_pageSize.height);
    }
    else if(endPoint.y - m_beginPoint.y < -m_pageSize.height * m_pageMovePercent)
    {
        m_pageColumnsIndex++;
        int lastPageColumnsIndex = allPageColumns-1;
        m_pageColumnsIndex = m_pageColumnsIndex > lastPageColumnsIndex ? lastPageColumnsIndex : m_pageColumnsIndex;
        toPoint = ccp(toPoint.x,-m_pageColumnsIndex*m_pageSize.height);
    }
    
    moveToEndPosition(true,toPoint);
}
//=============================================================
//=============================================================
#pragma mark 若超出边界 获得正确的位置 和 超出方向
CCPoint CCScrollLayer::getCorrectPoint(CCPoint toPoint)
{
    m_boundsDragOriation.HOriention = DOE_UNKNOWN;
    m_boundsDragOriation.VOriention = DOE_UNKNOWN;
    
    CCSize contentlayerSize = m_contentLayer->getContentSize();
    CCPoint toCorrectPosition = toPoint;
    if(toPoint.x >= 0) //向右拖出边界
    {
        if(toPoint.x >= m_boundsDragDistance.x)
        {
            m_boundsDragOriation.HOriention = DOE_RIGHT;
        }
        toCorrectPosition = ccp(0,toPoint.y);
    }
    else if(toPoint.x <= -(contentlayerSize.width - getContentSize().width))  //向左拖出边界
    {
        float dragDistance = fabsf(-(contentlayerSize.width - getContentSize().width)-toPoint.x);
        if(dragDistance >= m_boundsDragDistance.x)
        {
            m_boundsDragOriation.HOriention = DOE_LEFT;
        }
        toCorrectPosition = ccp(-(contentlayerSize.width - getContentSize().width),toPoint.y);
    }
    
    if(toPoint.y >= 0) //向上拖出边界
    {
        if(toPoint.y >= m_boundsDragDistance.y)
        {
            m_boundsDragOriation.VOriention = DOE_UP;
        }
        toCorrectPosition = ccp(toCorrectPosition.x,0);
    }
    else if(toPoint.y <= -(contentlayerSize.height - getContentSize().height)) //向下拖出边界
    {
        float dragDistance = fabsf(-(contentlayerSize.height - getContentSize().height) - toPoint.y);
        if(dragDistance >= m_boundsDragDistance.y)
        {
            m_boundsDragOriation.VOriention = DOE_DOWN;
        }
        
        toCorrectPosition = ccp(toCorrectPosition.x,-(contentlayerSize.height - getContentSize().height));
    }
    return toCorrectPosition;
}
//=============================================================
//=============================================================
#pragma  边界归位
void CCScrollLayer::moveToEndPosition(bool auctionFlag,CCPoint toEndPoint)  //边界位置恢复
{
    CCPoint toPosition = getCorrectPoint(toEndPoint);
    
    if(auctionFlag == true)
    {
        moveToAction(toPosition,m_toBoundsDuration);
    }
    else
    {
        m_contentLayer->setPosition(toPosition);
    }
}
//=============================================================
//=============================================================
#pragma mark 设置竖直方向 页数
void  CCScrollLayer::setPageColumnsCount(int columnsCount,bool animateFlag) //竖直方向
{
    int allPageColumns = (m_contentLayer->getContentSize().height-1)/m_pageSize.height + 1;
    CCPoint toPoint = ccp(-m_pageRowIndex*m_pageSize.width,-m_pageColumnsIndex*m_pageSize.height);
    
    m_pageColumnsIndex =  columnsCount-1;
    int lastPageColumnsIndex = allPageColumns-1;
    m_pageColumnsIndex = m_pageColumnsIndex > lastPageColumnsIndex ? lastPageColumnsIndex : m_pageColumnsIndex;
    m_pageColumnsIndex = m_pageColumnsIndex < 0 ? 0 : m_pageColumnsIndex;
    toPoint = ccp(toPoint.x,-m_pageColumnsIndex*m_pageSize.height);
    
    moveToEndPosition(animateFlag,toPoint);
}
//=============================================================
//=============================================================
#pragma mark 设置水平方向 页数
void  CCScrollLayer::setPageRowCount(int rowCount,bool animateFlag) //水平方向
{
    int allPageRow = (m_contentLayer->getContentSize().width-1)/m_pageSize.width + 1;
    CCPoint toPoint = ccp(-m_pageRowIndex*m_pageSize.width,-m_pageColumnsIndex*m_pageSize.height);
    
    m_pageRowIndex =  rowCount-1;
    int lastPageRowIndex = allPageRow-1;
    m_pageRowIndex = m_pageRowIndex > lastPageRowIndex ? lastPageRowIndex : m_pageRowIndex;
    m_pageRowIndex = m_pageRowIndex < 0 ? 0 : m_pageRowIndex;
    toPoint = ccp(-m_pageRowIndex*m_pageSize.width,toPoint.y);
    
    moveToEndPosition(animateFlag,toPoint);
}

//=============================================================
//=============================================================
#pragma mark 开始减速
void CCScrollLayer::beginDeclerate()
{
    sendSelector(SSTE_DECELERATE_BEGIN);
    
    if(!m_HMoveAbleFlag)    //横竖是否可移动
    {
        m_2fLastMoveDis.x = 0;
    }
    if(!m_VMoveAbleFlag)
    {
        m_2fLastMoveDis.y = 0;
    }
    
    if(m_decelerateFlag == false  )
    {
        if( fabs(m_2fLastMoveDis.x) > 0.1 || fabs(m_2fLastMoveDis.y) > 0.1)
        {
            m_decelerateFlag = true;
            schedule(schedule_selector(CCScrollLayer::decelerateTick));
        }
    }
}
//=============================================================
//=============================================================
void CCScrollLayer::stopDecelerate()
{
    m_contentLayer->stopAllActions();
    if(m_decelerateFlag)
    {
        m_decelerateFlag = false;
        unschedule(schedule_selector(CCScrollLayer::decelerateTick));
    }
    
    //getCorrectPoint(m_contentLayer->getPosition());
   // moveToEndPosition(false,m_contentLayer->getPosition());
    setContentOffset(ccp(0,0),false);
}
//=============================================================
//=============================================================
#pragma mark 减速tick
void CCScrollLayer::decelerateTick(float dt)  
{
    CCPoint oldPosition = m_contentLayer->getPosition();
    refreshDragOrientation(oldPosition);
    if(m_boundsDragOriation.HOriention != DOE_UNKNOWN)
    {
        m_2fLastMoveDis.x = 0;
    }
    if(m_boundsDragOriation.VOriention != DOE_UNKNOWN)
    {
        m_2fLastMoveDis.y = 0;
    }
    
    if(m_2fLastMoveDis.x ==0 && m_2fLastMoveDis.y ==0)
    {
        m_decelerateFlag = false;
        unschedule(schedule_selector(CCScrollLayer::decelerateTick));
        moveToEndPosition(true,oldPosition);
        sendSelector(SSTE_DECELERATE_END);
        return;
    }
    
    m_2fLastMoveDis.x = fabs(m_2fLastMoveDis.x) < 0.1 ? 0 : m_2fLastMoveDis.x;
    m_2fLastMoveDis.y = fabs(m_2fLastMoveDis.y) < 0.1 ? 0 : m_2fLastMoveDis.y;
    
    if(m_2fLastMoveDis.x ==0 && m_2fLastMoveDis.y ==0)
    {
        m_decelerateFlag = false;
        unschedule(schedule_selector(CCScrollLayer::decelerateTick));
        moveToEndPosition(false,oldPosition);
        sendSelector(SSTE_DECELERATE_END);
        return;
    }
    m_2fLastMoveDis.x = m_2fLastMoveDis.x*m_fDeceleratePercent;
    m_2fLastMoveDis.y = m_2fLastMoveDis.y*m_fDeceleratePercent;
    float fNewX = m_contentLayer->getPosition().x + m_2fLastMoveDis.x;
    float fNewY = m_contentLayer->getPosition().y + m_2fLastMoveDis.y;
    CCPoint newPoint = ccp(fNewX,fNewY);
    moveToEndPosition(false,newPoint);
    sendSelector(SSTE_DECELERATE_MOVE);
    refreshDotPosition();
}
//=============================================================
//=============================================================
void CCScrollLayer::removeAllContentChildrenWithCleanup(bool cleanup)
{
    m_contentLayer->removeAllChildrenWithCleanup(cleanup);
}
//=============================================================
//=============================================================
# pragma mark 设置回调函数
void CCScrollLayer::setDelegate(CCObject *delegate, SEL_CallFuncN selector, ScrollSelectorTypeEnum selectorType)
{
    m_delegate = delegate;
    switch (selectorType)
    {
        case SSTE_TOUCH_BEGIN:
        {
            m_touchBeginSelector = selector;
        }
            break;
        case SSTE_TOUCH_MOVE:
        {
            m_touchMoveSelector = selector;
        }
            break;
        case SSTE_TOUCH_END:
        {
            m_touchEndSelector = selector;
        }
            break;
        case SSTE_TOUCH_CANCEL:
        {
            m_touchCancelSelector = selector;
        }
            break;
        case SSTE_ACTION_BEGIN:
        {
            m_actionBeginSelector = selector;
        }
            break;
        case SSTE_ACTION_MOVE:
        {
            m_actionMoveSelector = selector;
        }
            break;
        case SSTE_ACTION_END:
        {
            m_actionEndSelector = selector;
        }
            break;
        case SSTE_DECELERATE_BEGIN:
        {
            m_decelerateBeginSelector = selector;
        }
            break;
        case SSTE_DECELERATE_MOVE:
        {
            m_decelerateMoveSelector = selector;
        }
            break;
        case SSTE_DECELERATE_END:
        {
            m_decelerateEndSelector = selector;
        }
            break;
        default:
            break;
    }
}
//=============================================================
//=============================================================
void CCScrollLayer::sendSelector(ScrollSelectorTypeEnum type)
{
    switch (type)
    {
        case SSTE_TOUCH_BEGIN:
        {
            if(m_delegate!=NULL&&m_touchBeginSelector!=NULL)
            {
                (m_delegate->*m_touchBeginSelector)(this);
            }
        }
            break;
        case SSTE_TOUCH_MOVE:
        {
            if(m_delegate&&m_touchMoveSelector) //回调移动函数
            {
                (m_delegate->*m_touchMoveSelector)(this);
            }
        }
            break;
        case SSTE_TOUCH_END:
        {
            if(m_delegate!=NULL&&m_touchEndSelector!=NULL) //回调end函数
            {
                (m_delegate->*m_touchEndSelector)(this);
            }
        }
            break;
        case SSTE_TOUCH_CANCEL:
        {
            if(m_delegate!=NULL&&m_touchCancelSelector!=NULL) //回调end函数
            {
                (m_delegate->*m_touchEndSelector)(this);
            }
        }
            break;
        case SSTE_ACTION_BEGIN:
        {
            
            if(m_delegate!=NULL&&m_actionBeginSelector!=NULL)
            {
                (m_delegate->*m_actionBeginSelector)(this);
            }
        }
            break;
        case SSTE_ACTION_MOVE:
        {
            
            if(m_delegate!=NULL&&m_actionMoveSelector!=NULL)
            {
                (m_delegate->*m_actionMoveSelector)(this);
            }
        }
            break;
        case SSTE_ACTION_END:
        {
            if(m_delegate!=NULL&&m_actionEndSelector!=NULL)
            {
                (m_delegate->*m_actionEndSelector)(this);
            }
        }
            break;
        case SSTE_DECELERATE_BEGIN:
        {
            if(m_delegate!=NULL&&m_decelerateBeginSelector!=NULL)
            {
                (m_delegate->*m_decelerateBeginSelector)(this);
            }
        }
            break;
        case SSTE_DECELERATE_MOVE:
        {
            if(m_delegate!=NULL&&m_decelerateMoveSelector!=NULL)
            {
                (m_delegate->*m_decelerateMoveSelector)(this);
            }
        }
            break;
        case SSTE_DECELERATE_END:
        {
            if(m_delegate!=NULL&&m_decelerateEndSelector!=NULL)
            {
                (m_delegate->*m_decelerateEndSelector)(this);
            }
        }
            break;
            
        default:
            break;
    }
}
//=============================================================
//=============================================================
