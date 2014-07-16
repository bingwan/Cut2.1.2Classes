
#include "CCLabelStroke.h"

bool CCLabelStroke::m_canStroke = true;


//-------------------------------------------
CCLabelStroke::CCLabelStroke():
m_renderTexture(NULL),
m_lineWidth(0)
{
}

CCLabelStroke::~CCLabelStroke()
{}

void CCLabelStroke::setString(const char *label)
{
    CCLabelTTF::setString(label);
    if(m_renderTexture)
    {
        m_renderTexture->removeFromParentAndCleanup(true);
        m_renderTexture = NULL;
      
        setStroke(m_lineWidth, m_lineColor);
    }
}

CCLabelStroke * CCLabelStroke::labelWithString(const char *label, CCSize dimensions, CCTextAlignment alignment, const char *fontName, float fontSize,float lineWidth,ccColor3B lineColor)
{
    CCLabelStroke* strokeLabel = labelWithString(label,dimensions,alignment,fontName,fontSize);
    
    if(lineWidth!=0)
    {
        strokeLabel->setStroke(lineWidth, lineColor);
    }
    return strokeLabel;
}

CCLabelStroke * CCLabelStroke::labelWithString(const char *label, const char *fontName, float fontSize,float lineWidth, ccColor3B lineColor)
{
    CCLabelStroke* strokeLabel = labelWithString(label,fontName,fontSize);
    if(lineWidth!=0)
    {
         strokeLabel->setStroke(lineWidth, lineColor);
    }
   
    return strokeLabel;
}

CCLabelStroke * CCLabelStroke::labelWithString(const char *label, CCSize dimensions, CCTextAlignment alignment, const char *fontName,float fontSize)
{
    CCLabelStroke* strokeLabel = new CCLabelStroke();
    //strokeLabel->initWithString(label, dimensions,alignment,fontName, fontSize);
    
    strokeLabel->initWithString(label, fontName, fontSize, dimensions, alignment);
    strokeLabel->autorelease();
    return strokeLabel;
}

CCLabelStroke * CCLabelStroke::labelWithString(const char *label, const char *fontName, float fontSize)
{
    CCLabelStroke* strokeLabel = new CCLabelStroke();
    strokeLabel->initWithString(label,fontName, fontSize);
    strokeLabel->autorelease();
    return strokeLabel;
}


void CCLabelStroke::setStroke(float size,ccColor3B color)
{
    if(!m_canStroke)
    {
        return;
    }
    m_lineWidth = size;
    m_lineColor = color;
    
    CCSize labelSize = getTexture()->getContentSize();
    
    CCPoint labelAnchorPoint = getAnchorPoint();
    CCPoint originalPos = getPosition();
    ccColor3B originalColor = getColor();
    ccBlendFunc originalBlend = getBlendFunc();
    float originalScale = getScale();
    bool bOldVisible = isVisible();
    

    float scaleStroke = 2.f; //syq: 占用多少内存？
    
    float w = labelSize.width + m_lineWidth*2;
    float h = labelSize.height + m_lineWidth*2;
    w *= scaleStroke;
    h *= scaleStroke;
    CCRenderTexture* renderRexture = CCRenderTexture::create(w,h, kCCTexture2DPixelFormat_RGBA8888);

    setColor(color);
    setBlendFunc((ccBlendFunc) { GL_SRC_ALPHA, GL_ONE });
    
    float moveX = labelSize.width * (labelAnchorPoint.x - 0.5) * scaleStroke;
    float moveY = labelSize.height * (labelAnchorPoint.y - 0.5) * scaleStroke;

    CCPoint position = ccp(w/2 + moveX, h/2 + moveY);
    
    renderRexture->begin();
    
    setVisible(true);
    for (int i=0; i<360; i+=30)
    {
        setScale(scaleStroke);
        setPosition(ccp(position.x + sin(CC_DEGREES_TO_RADIANS(i)) * m_lineWidth * scaleStroke,
                        position.y + cos(CC_DEGREES_TO_RADIANS(i)) * m_lineWidth * scaleStroke));
        visit();
    }

    renderRexture->end();
    
    setAnchorPoint(labelAnchorPoint);
    setPosition(originalPos);
    setColor(originalColor);
    setBlendFunc(originalBlend);
    setScale(originalScale);
    setVisible(bOldVisible);
    
    CCPoint strokePosition = ccp(labelSize.width/2, labelSize.height/2);
    renderRexture->setPosition(strokePosition);
    renderRexture->setScale(1/scaleStroke);
    addChild(renderRexture,-1);
    
    m_renderTexture = renderRexture;
     
}
