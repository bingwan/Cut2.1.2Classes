
/*
 *
 *  Created by wangbin
 *
 */

#ifndef MT_B_LJCCControlButton_h
#define MT_B_LJCCControlButton_h

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
    
class CCButton : public CCControlButton
{        
public:
    CCButton();
    virtual ~CCButton();
    static CCButton* create(CCNode* label, CCScale9Sprite* backgroundSprite);
    static CCButton* create(std::string title, const char * fontName, float fontSize);
    static CCButton* create(CCScale9Sprite* sprite);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();

    virtual void needsLayout();
    virtual void setBackgroundSpriteForState(CCScale9Sprite* sprite, CCControlState state);

    virtual void setEnabled(bool enabled);
    virtual void setSelected(bool enabled);
    virtual void setHighlighted(bool enabled);
    void         setPreventTouch(bool preventFlag){m_bPreventTouchFlag = preventFlag;}
private:
    float           m_dragDistancePercent;
    CCPoint         m_beginPoint;
    bool            m_bPreventTouchFlag;
};

#endif
