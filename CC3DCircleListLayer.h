//
//  CCTextLayer.h
//  CCTextLayer
//
//  Created by wangbin on 13-1-9.
//
//

#ifndef __CC3DCircleListLayer__CCTextLayer__
#define __CC3DCircleListLayer__CCTextLayer__

#include <iostream>
#include "CCCutTouchLayer.h"
#include "cocos2d.h"
#include "CCScrollLayer.h"
#include "CCButton.h"
USING_NS_CC;

class CC3DCircleListLayerDelegate
{
public:
    virtual void  onCC3DCircleListLayerSelectIndex(int nIndex){}
};

class  CC3DCircleListLayer : public CCCutTouchLayer
{
public:

    CC3DCircleListLayer();
	virtual ~CC3DCircleListLayer();
	CREATE_FUNC(CC3DCircleListLayer);
    
    static CC3DCircleListLayer*     create(const ccColor4B& color, GLfloat width, GLfloat height);
    virtual bool                    initWithColor(const ccColor4B& color, GLfloat w, GLfloat h);
    void                            initDataCircle(float fEllipseA,float fEllipseB,CCSize itemSize,CCArray* pArray,bool bFlipY);

    void                            refreshItem(float fMoveDis);

    virtual void                    registerWithTouchDispatcher(void);
    
    virtual bool                    ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void                    ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void                    ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void                    ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void                            setDelegate(CC3DCircleListLayerDelegate* pDelegate){m_pDelegate = pDelegate; }
protected:
    void                            touchEnd();
    void                            updateItemPos(float dt);
    void                            clickButton(CCButton* pButton);
    void                            moveToCenter(CCButton* pButton,bool bActionFlag);
    void                            refreshItemWithMoveDegree(float fMoveDegree);
    void                            updateItemDegree(float dt);
    void                            stopUpdateItemDegree();

    void                            endUpdateToCenter();
    CCButton*                       getSelectedButton();

private:
    CCArray*            m_pButtonArray;
    float               m_fEllipseA;
    float               m_fEllipseB;
    CCSize              m_itemSize;
    bool                m_bDraging;
    float               m_fLastMoveDis;
    float               m_fDeacceleratePercent;
    
    float               m_fLastMoveDegree;
    float               m_fCurMoveDegree;
    float               m_fToEndActionTime;
    
    CC3DCircleListLayerDelegate* m_pDelegate;
    CCButton*           m_pSelectedButton;
};

#endif /* defined(__CC3DCircleListLayer__CCTextLayer__) */
