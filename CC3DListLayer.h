//
//  CCTextLayer.h
//  CCTextLayer
//
//  Created by wangbin on 13-1-9.
//
//

#ifndef __CC3DListLayer__CCTextLayer__
#define __CC3DListLayer__CCTextLayer__

#include <iostream>
#include "CCCutTouchLayer.h"
#include "cocos2d.h"
#include "CCScrollLayer.h"
#include "CCButton.h"
USING_NS_CC;

class CC3DListLayerDelegate
{
public:
    virtual void  onCC3DListLayerSelectIndex(int nIndex){}
    virtual void  onCC3DListLayerClickSelect(int nIndex){}
};

class  CC3DListLayer : public CCCutTouchLayer
{
public:
    
    enum ListType
    {
        eListType_Unknown =0,
        eListType_HalfEllipse =1,
        eListType_CoverFlow   =2,
        eListType_Ellipse     =3,
        eListType_Line        =4,
        eListType_LineNormal  =5,
        
    };
    CC3DListLayer();
	virtual ~CC3DListLayer();
	CREATE_FUNC(CC3DListLayer);
    
    static CC3DListLayer*   create(const ccColor4B& color, GLfloat width, GLfloat height);
    virtual bool            initWithColor(const ccColor4B& color, GLfloat w, GLfloat h);
    
    
    void                    initDataCoverFlow(float fSpaceX,CCSize itemSize,CCArray* pArray,bool bFlipY);
    void                    initDataLine(float fSpaceX,float fSpaceY,CCSize itemSize,CCArray* pArray,bool bFlipY,bool bToBoundsMove);
    void                    initDataHalfEllipse(float fSpaceX,float fEllipseA,float fEllipseB,CCSize itemSize,CCArray* pArray,bool bFlipY);
    void                    initDataEllipse(float fSpaceX,CCSize itemSize,CCArray* pArray,bool bFlipY);
    void                    setItemScaleRange(float fMin,float fMax){m_fItemScaleMin= fMin;m_fItemScaleMax= fMax;}
    void                    setDelegate(CC3DListLayerDelegate* pDelegate){m_pDelegate = pDelegate;}


protected:
    void                    refreshItem();
    void                    actionMove();
    void                    moveToCenter(CCButton* pButton,bool bAction);
    void                    refreshItemCoverFlow();
    void                    refreshItemHalfEllipse();
    void                    refreshItemEllipse();
    void                    refreshItemLine();

    void                    clickButton(CCButton* pButton);
    
    void                    decelerateEnd();
    void                    actionEnd();
    void                    decelerateMove();
private:
    CCArray*                m_pButtonArray;
    CCScrollLayer*          m_pScrollLayer;
    float                   m_fSpaceX;
    float                   m_fOriginSpaceX;
    CCSize                  m_itemSize;
    ListType                m_eListType;
    
    CCButton*               m_pSelectedButton;
    
    float                   m_fEllipseA;
    float                   m_fEllipseB;
    
    CC3DListLayerDelegate*  m_pDelegate;
    float                   m_fItemScaleMax;
    float                   m_fItemScaleMin;
    
    bool                    m_bToBoundsMove;
};

#endif /* defined(__CC3DListLayer__CCTextLayer__) */
