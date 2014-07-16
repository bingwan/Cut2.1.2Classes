
/*
 *
 *  Created by wangbin
 *
 */

#ifndef __CCSCROLL_LAYER_H__
#define __CCSCROLL_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;

class  CCScrollLayer : public CCLayerColor
{
public:
    enum DragOrientationEnum
    {
        DOE_UNKNOWN =0,
        DOE_UP      =1,
        DOE_DOWN    =2,
        DOE_LEFT    =3,
        DOE_RIGHT   =4,
    };
    
    enum ScrollSelectorTypeEnum
    {
        SSTE_UNKNOWN = 0,
        SSTE_TOUCH_BEGIN   = 1,
        SSTE_TOUCH_MOVE    = 2,
        SSTE_TOUCH_END     = 3,
        SSTE_TOUCH_CANCEL  = 4,
        
        SSTE_ACTION_BEGIN =5, 
        SSTE_ACTION_MOVE =6,
        SSTE_ACTION_END =7,
        
        SSTE_DECELERATE_BEGIN =8,
        SSTE_DECELERATE_MOVE =9,
        SSTE_DECELERATE_END =10,
        
    };
    
    struct DragOrientation2
    {
        DragOrientation2(){clear();}
        ~DragOrientation2(){}
        void clear()
        {
            HOriention = DOE_UNKNOWN;
            VOriention = DOE_UNKNOWN;
            
        }
        
        DragOrientationEnum HOriention;
        DragOrientationEnum VOriention;
    };
    
	CCScrollLayer();
	virtual			~CCScrollLayer();
    CREATE_FUNC(CCScrollLayer);
    
    static CCScrollLayer*     scrollLayerWidthColor(const ccColor4B color, CCSize size);
        
	virtual bool        ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void        ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void        ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void        ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
   
    virtual void        visit();
    virtual void        registerWithTouchDispatcher(void);
    virtual bool        initWithColorWidthHeight(const ccColor4B color, GLfloat width, GLfloat height);
    
    void                setScrollContentSize(CCSize contentSize);
    CCSize              getScrollContentSize();
    void                moveToAction(CCPoint toPoint, float duration);
    
    
    void                setPagingEnable(bool pagingEnableFlag){m_pagingEnable = pagingEnableFlag;}  
    bool                getPagingEnable(){ return  m_pagingEnable;}
    
    void                setToBoundsDuration(float toBoundsDuration){m_toBoundsDuration = toBoundsDuration;}
    float               getToBoundsDuration(){return m_toBoundsDuration;}
    void                setNextPageDuration(float nextPageDuration){m_nextPageDuration = nextPageDuration;}
    float               getNextPageDurationDuration(){return m_nextPageDuration;}
    void                nextPageAction(CCPoint endPoint);
    void                addSubChild(CCNode* node);
    void                setPageSize(CCSize moveSize);
    CCSize              getPageSize(){return m_pageSize;}
    void                setContentOffset(CCPoint offsetPoint,bool actionFlag);
    CCPoint             getContentOffset();
    
    void                setPageColumnsCount(int columnsCount,bool animateFlag);
    int                 getPageColumnsCount(){return m_pageColumnsIndex+1;}
    void                setPageRowCount(int rowCount,bool animateFlag);
    int                 getPageRowCount(){return m_pageRowIndex+1;}
    
   
    void                setDelegate(CCObject *delegate, SEL_CallFuncN selector,ScrollSelectorTypeEnum selectorType);
    void                setEndFlag(bool flag);
    DragOrientation2    getDragOriation(){return m_boundsDragOriation;}
   
    void                beginDeclerate();
    void                decelerateTick(float dt);
        
    void                setBoundsDragDistance(ccVertex2F dragDistance){m_boundsDragDistance = dragDistance;}
    ccVertex2F          getBoundsDragDistance(){return m_boundsDragDistance;}
        
    void                stopDecelerate();
        
    void                setShowHDot(bool isShow){m_bShowHDot = isShow;}
    void                setShowVDot(bool isShow){m_bShowVDot = isShow;}
    
    void                removeAllContentChildrenWithCleanup(bool cleanup);
    void                addSubChild(CCNode* node,int zOrder);

    void                refreshPosition();
protected:
    void                actionTick(float dt);
    void                moveToEndPosition(bool auctionFlag,CCPoint toEndPoint);
    void                scrollDidEnd();
    void                refreshDotPosition();
    CCPoint             getCorrectPoint(CCPoint point);
    void                refreshDragOrientation(CCPoint toPoint);
    void                sendSelector(ScrollSelectorTypeEnum type);
private: 
    void                afterDraw();
    void                beforeDraw();
private:
	CCPoint             m_beginPoint;
    CCLayer*            m_contentLayer;
    bool                m_containBeginTouchPointFlag;
    bool                m_pagingEnable;       //可翻页标记
    float               m_toBoundsDuration;   //边界归位时间
    float               m_nextPageDuration;   //下一页时间
    int                 m_pageColumnsIndex;   //竖向索引位置
    int                 m_pageRowIndex;       //横向索引位置
    CCSize              m_pageSize;           //页的大小
    float               m_pageMovePercent;    //触发翻页的距离百分比
    
    CCObject*            m_delegate;
    SEL_CallFuncN        m_touchBeginSelector;  //点击开始回调
    SEL_CallFuncN        m_touchMoveSelector;   //移动时回调    
	SEL_CallFuncN        m_touchEndSelector;    //点击结束回调
    SEL_CallFuncN        m_touchCancelSelector; //取消时回调    todo
    
    
    SEL_CallFuncN        m_decelerateBeginSelector;
    SEL_CallFuncN        m_decelerateMoveSelector;
    SEL_CallFuncN        m_decelerateEndSelector;
    
    SEL_CallFuncN        m_actionBeginSelector;
    SEL_CallFuncN        m_actionMoveSelector;
    SEL_CallFuncN        m_actionEndSelector; //翻页 边界归位 后回调
    
    bool                 m_endFlag;   //设置位置后 取消touchend回调
    
    DragOrientation2     m_boundsDragOriation;   //超出边界的方向
    ccVertex2F           m_boundsDragDistance;   //超出边界的距离
    
    bool                 m_HMoveAbleFlag;   //横向移动
    bool                 m_VMoveAbleFlag;   //竖向移动
    bool                 m_decelerateFlag;
    
    CCSprite*            m_scrollDotVSprite;
    CCSprite*            m_scrollDotHSprite;
    
    float                m_percentOfMoveWhenDragOut;
    bool                 m_bShowVDot;  //显示竖 滑动条
    bool                 m_bShowHDot;  //显示横 滑动条
    
    ccVertex2F           m_2fLastMoveDis;
    float                m_fDeceleratePercent;
    
    

};

#endif // __CCSCROLL_LAYER_H__