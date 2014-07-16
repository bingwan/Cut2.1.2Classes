

#ifndef CCLabelStroke_CCLabelStroke_h
#define CCLabelStroke_CCLabelStroke_h

#include "cocos2d.h"
USING_NS_CC;

class CCLabelStroke : public CCLabelTTF
{
    public:
    CCLabelStroke();
    virtual  ~CCLabelStroke();
    
    virtual void setString(const char *label);
    
    static CCLabelStroke * labelWithString(const char *label, CCSize dimensions, CCTextAlignment alignment, const char *fontName, float fontSize,float lineWidth, ccColor3B lineColor);
    static CCLabelStroke * labelWithString(const char *label, const char *fontName, float fontSize,float lineWidth, ccColor3B lineColor);
    
    static CCLabelStroke * labelWithString(const char *label, CCSize dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
    static CCLabelStroke * labelWithString(const char *label, const char *fontName, float fontSize);
    
    void   setStroke(float size,ccColor3B color);
    
    void   setDimensions(CCSize size){m_tDimensions = size;}
    
private:
    CCRenderTexture* m_renderTexture;
    float m_lineWidth;
    ccColor3B   m_lineColor;
    
    static bool   m_canStroke;
};



#endif
