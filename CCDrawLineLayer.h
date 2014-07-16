

#ifndef CCWater_CCDrawLineLayer_h
#define CCWater_CCDrawLineLayer_h

#include "cocos2d.h"
#include  <queue>
#include <list>
USING_NS_CC;

class CCDrawLineLayer;

class CC_DLL CCDrawLineLayerDelegate
{
public:
    virtual void ccDrawLineLayerTouchBegin(CCDrawLineLayer* drawLineLayer,CCTouch* touch)
    {
        CC_UNUSED_PARAM(drawLineLayer);
        CC_UNUSED_PARAM(touch);
    }
    
    virtual void ccDrawLineLayerTouchMove(CCDrawLineLayer* drawLineLayer,CCTouch* touch)
    {
        CC_UNUSED_PARAM(drawLineLayer);
        CC_UNUSED_PARAM(touch);
    }
};

class CCDrawLineLayer : public CCLayer
{
public:
    CCDrawLineLayer();
    virtual ~CCDrawLineLayer();
	virtual bool init();  
	CREATE_FUNC(CCDrawLineLayer);
    
    virtual void draw();
//    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
//	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
//	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    //virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual bool        ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void        ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void        ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void        ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void drawLine();
        
    void setFingerIndex(int finger){m_fingerIndex = finger;}
    int  getFingerIndex(){return m_fingerIndex;}
    void setLineColor(ccColor3B lineColor){m_lineColor = lineColor;}
    void setIsRandColor(bool isRandColor){m_isRandColor = isRandColor;}
    
    virtual void registerWithTouchDispatcher(void);

    
private:
    
    int   m_fingerIndex;
    int   m_deleteCountOfTick;
    int   m_pointListKeepCount;
    float m_lineWidthMin;
    float m_lineWidthMax;
    int   m_lineAlphaMin;
    int   m_lineAlphaMax;
    float m_percentOfMaxPoint;
    ccColor3B m_lineColor;
    bool m_isRandColor;
    
    
    CCDrawLineLayerDelegate* m_drawLayerDelegate;
    std::list<CCPoint> m_pointList; 
};


#endif
