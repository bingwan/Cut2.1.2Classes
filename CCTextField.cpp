
/*
 *
 *  Created by wangbin
 *
 */


#include "CCTextField.h"
CCTextField::CCTextField():
m_fontName(""),
m_fontSize(20),
m_textFieldTTF(NULL),
m_beginPoint(ccp(0,0)),
m_type(TFT_NORMAL),
m_contentString(),
m_visitFlag(true),
m_renderTexture(NULL), 
m_bClearWhenTouch(false),
m_sprBg(NULL),
m_textDelegate(NULL),
m_flagLabel(NULL),
m_autoAttach(true)
{
    ignoreAnchorPointForPosition (false);
}
//=============================================================
//=============================================================
CCTextField::~CCTextField()
{
}
//=============================================================
//=============================================================
CCTextField* CCTextField::create(const ccColor4B color,const char* placeHolder,CCSize size,CCTextAlignment alignment,const char* fontName,int fontSize)
{
    CCTextField* textField = new CCTextField();
    textField->initWithColorWidthHeight(color, size.width, size.height);
    textField->autorelease();
    textField->addTextField(placeHolder,fontName, fontSize,alignment);
    return  textField;
}
//=============================================================
//=============================================================
bool CCTextField::initWithColorWidthHeight(const ccColor4B color, GLfloat width, GLfloat height)
{
    if(CCCutTouchLayer::initWithColor(color,width,height))
    {
        return true;
    }
    return false;
}
//=============================================================
//=============================================================
void CCTextField::addTextField(const char* placeHolder,const char* fontName, int fontSize,CCTextAlignment alignment)
{
    m_fontName = fontName;
    m_fontSize = fontSize;
    m_textFieldTTF = CCTextFieldTTF::textFieldWithPlaceHolder(placeHolder,fontName,fontSize);
    
    switch (alignment) 
    {
        case kCCTextAlignmentLeft:
        {
            m_textFieldTTF->setAnchorPoint(ccp(0,0.5));
            m_textFieldTTF->setPosition(ccp(0,getContentSize().height*0.5));
        }
            break;
            
        case kCCTextAlignmentCenter:
        {
            m_textFieldTTF->setAnchorPoint(ccp(0.5,0.5));
            m_textFieldTTF->setPosition(ccp(getContentSize().width*0.5,getContentSize().height*0.5));
        }
            break;
        case kCCTextAlignmentRight:
        {
            m_textFieldTTF->setAnchorPoint(ccp(1,0.5));
            m_textFieldTTF->setPosition(ccp(getContentSize().width,getContentSize().height*0.5));
        }
            break;
        default:
            break;
    }
    m_textFieldTTF->setDelegate(this);
    addChild(m_textFieldTTF);    
    
    m_flagLabel = CCLabelTTF::create("I", fontName, fontSize);
    m_flagLabel->setColor(ccBLUE);
    m_flagLabel->setAnchorPoint(ccp(0,0.5));
    m_textFieldTTF->addChild(m_flagLabel);
    
    CCFadeOut* fadeOut = CCFadeOut::create(1.2);
    CCRepeatForever* repeat = CCRepeatForever::create(fadeOut);
    m_flagLabel->runAction(repeat);
    
    refreshFlagLabelInput();
}
//=============================================================
//=============================================================
//void CCTextField::addTextField(CCSize size,const char* placeHolder,CCTextAlignment alignment,const char* fontName, int fontSize)
//{
//    m_fontName = fontName;
//    m_fontSize = fontSize;
//
//    m_textFieldTTF = CCTextFieldTTF::textFieldWithPlaceHolder(placeHolder,size,alignment,fontName,fontSize);
//    m_textFieldTTF->setAnchorPoint(ccp(0,0));
//    m_textFieldTTF->setPosition(ccp(0,0));
//    m_textFieldTTF->setDelegate(this);
//    addChild(m_textFieldTTF);
//}
//=============================================================
//=============================================================
CCSize CCTextField::getTextContentSize()
{
    return m_textFieldTTF->getContentSize();
}
//=============================================================
//=============================================================
void CCTextField::visit()
{
    CCCutTouchLayer::visit();
    return;
    
    if(m_visitFlag)
    {
        CCNode* parent = getParent();
        CCRect rect = boundingBox();
        CCPoint originPoint = parent->convertToWorldSpace(rect.origin);
        glEnable(GL_SCISSOR_TEST);
        int factor = CCDirector::sharedDirector()->getContentScaleFactor();
        glScissor((GLint)originPoint.x*factor,(GLint)originPoint.y*factor,(GLsizei)(rect.size.width*factor),(GLsizei)(rect.size.height*factor));
        CCCutTouchLayer::visit();
        glDisable(GL_SCISSOR_TEST);
        
//        CCRect drawRect = CCRectZero;
        
//        CCNode* parent = getParent();
//        CCRect parentRect = boundingBox();
//        CCPoint originPoint = parent->convertToWorldSpace(parentRect.origin);
//        CCRect selfRectInWorld = CCRectMake(originPoint.x, originPoint.y, parentRect.size.width, parentRect.size.height);
//        
//        CCNode* grand = parent->getParent();
//        if(grand)
//        {
//            CCRect grandRect = parent->boundingBox();
//            CCPoint grandOriginPoint = grand->convertToWorldSpace(grandRect.origin);
//            CCRect parentRectInWorld = CCRectMake(grandOriginPoint.x, grandOriginPoint.y, grandRect.size.width, grandRect.size.height);
//            drawRect =  gLJCCAPIPtr->rectIntersection(parentRectInWorld, selfRectInWorld);
//        }
//        else
//        {
//            drawRect = selfRectInWorld;
//        }
//        
//        int factor = CCDirector::sharedDirector()->getContentScaleFactor();
//        
//        glEnable(GL_SCISSOR_TEST);
//        glScissor((GLint)drawRect.origin.x*factor,(GLint)drawRect.origin.y*factor,(GLsizei)(drawRect.size.width*factor),(GLsizei)(drawRect.size.height*factor));
//        CCLayerColor::visit();
//        glDisable(GL_SCISSOR_TEST);
    }
    else
    {
        CCCutTouchLayer::visit();
    }
}
//=============================================================
//=============================================================
bool CCTextField::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    //CCSetIterator it = pTouches->begin();
    //CCTouch* pTouch = (CCTouch*)(*it);
    if(!isVisible()) //父亲的visible变量改变不影响子节点 visible=false 时也接受touch
    {
        return false;
    }
    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent()) 
    {
        if (c->isVisible() == false) //若父亲有不可见的则不处理touch 
        {
            return false;
        }
    }
    m_beginPoint = pTouch->getLocationInView();
    m_beginPoint = CCDirector::sharedDirector()->convertToGL(m_beginPoint);
    
    CCNode* parent = getParent();
    CCPoint localPoint = parent->convertToNodeSpace(m_beginPoint);
    CCRect  boundRect = boundingBox();
    if (boundRect.containsPoint(localPoint))
    {
        m_textFieldTTF->attachWithIME();
    }
    else
    {
        m_textFieldTTF->detachWithIME();
    }
    return (CCCutTouchLayer::ccTouchBegan(pTouch, pEvent));
}
//=============================================================
//=============================================================
void CCTextField::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
//    CCTouch* touch = pTouch;
//	  CCPoint endPoint = touch->getLocationInView();
//    endPoint = CCDirector::sharedDirector()->convertToGL(endPoint);
//    
//    CCNode* parent = getParent();
//    if(parent == NULL) return;
//    CCPoint localPoint = parent->convertToNodeSpace(endPoint);
//    CCRect boundRect = boundingBox();
}
//=============================================================
//=============================================================
bool CCTextField::onTextFieldAttachWithIME(CCTextFieldTTF * sender)
{ 
    if (m_bClearWhenTouch)
    {
        m_textFieldTTF->setString("");
        refreshFlagLabelInput();
    }

    if(m_textDelegate)
    {
        return   m_textDelegate->onTextFieldAttachWithIME(this);
    }
    return false;
}
//=============================================================
//=============================================================
bool CCTextField::onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
    const char* text = m_textFieldTTF->getString();
    if(text == NULL)
    {
        m_textFieldTTF->setString(m_textFieldTTF->getPlaceHolder());
        refreshFlagLabelInput();
    }
    if(m_textDelegate)
    {
      return  m_textDelegate->onTextFieldDetachWithIME(this);
    }
    return false;
}
//=============================================================
//=============================================================
bool CCTextField::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen)
{
    if(m_textDelegate && m_textDelegate->onTextFieldInsertText(this, text,nLen))
    {
        return true;
    }
    else
    {
        m_contentString.append(text);
    }
    //CCLog("contentString=%s",m_contentString.c_str());
    if(m_type == TFT_SECRET)
    {
        m_secretTextString.append("●");
        schedule(schedule_selector(CCTextField::addSecretText),0.1);
    }
    //refreshFlagLabel(true);
    scheduleOnce(schedule_selector(CCTextField::refreshFlagLabelInput),0.1);
    
    //CCLog("CCTextField::onTextFieldInsertText=%s.",m_contentString.c_str());
    return false;
}
//=============================================================
//=============================================================
void CCTextField::addSecretText()
{
    unschedule(schedule_selector(CCTextField::addSecretText));
    m_textFieldTTF->setString(m_secretTextString.c_str());
    refreshFlagLabelInput();
}
//=============================================================
//=============================================================
bool CCTextField::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    //CCLog("CCTextField::onTextFieldDeleteBackward1=%s.",m_contentString.c_str());
    m_contentString.assign(m_contentString.begin(),m_contentString.end()-1);
    //CCLog("CCTextField::onTextFieldDeleteBackward2=%s.",m_contentString.c_str());
    if(m_type == TFT_SECRET)
    {
        m_secretTextString.assign(m_secretTextString.begin(),m_secretTextString.end()-1);
        //CCLog("afterdelete====%s",m_secretTextString.c_str());
        m_textFieldTTF->setString(m_secretTextString.c_str());
    }
    refreshFlagLabelDelete();
    if(m_textDelegate)
    {
        return  m_textDelegate->onTextFieldDeleteBackward(this,delText,nLen);
    }
    return false;
}
//=============================================================
//=============================================================
const char* CCTextField::getString()
{
    //CCLog("CCTextField::getString=%s.",m_contentString.c_str());
    return  m_contentString.c_str();
}
//=============================================================
//=============================================================
//const char* CCTextField::getLabelString()
//{
//    CCLog("CCTextField::getString=%s.",m_textFieldTTF->getString());
//    return  m_textFieldTTF->getString();
//}
//=============================================================
//=============================================================
void CCTextField::setString(const char* pStr)
{
    if(m_type == TFT_SECRET)
    {
        m_secretTextString.clear();
        for (int i=0; i<strlen(pStr); i++)
        {
            m_secretTextString.append("●");
        }
        m_textFieldTTF->setString(m_secretTextString.c_str());
        
    }
    else
    {
        m_textFieldTTF->setString(pStr);
    }
    
    refreshFlagLabelInput();  //刷新光标
    m_contentString.assign(pStr);
    if(m_renderTexture)
    {
        m_renderTexture->removeFromParentAndCleanup(true);
        m_renderTexture = NULL;
        setStroke(m_lineWidth, m_lineColor);
    }
}
//=============================================================
//=============================================================
void CCTextField::setPlaceHolder(const char * text)
{
    m_textFieldTTF->setPlaceHolder(text);
}
//=============================================================
//=============================================================
const char* CCTextField::getPlaceHolder(void)
{
    return  m_textFieldTTF->getPlaceHolder();
}
//=============================================================
//=============================================================
void CCTextField::setBackGroundColor(ccColor4B color)
{
    setColor(ccc3(color.r, color.g, color.b));
}
//=============================================================
//=============================================================
void CCTextField::setTextPosition(CCPoint point)
{
    m_textFieldTTF->setPosition(point);
}
//=============================================================
//=============================================================
void CCTextField::setStroke(float size,ccColor3B color)
{
    m_lineWidth = size;
    m_lineColor = color;
    
    CCSize labelSize = m_textFieldTTF->getTexture()->getContentSize();
    CCPoint labelAnchorPoint = m_textFieldTTF->getAnchorPoint();
    CCRenderTexture* renderRexture = CCRenderTexture::create(labelSize.width+m_lineWidth*2,labelSize.height+m_lineWidth*2);
    
    CCPoint originalPos = m_textFieldTTF->getPosition();
    ccColor3B originalColor = m_textFieldTTF->getColor();
    m_textFieldTTF-> setColor(color);
    ccBlendFunc originalBlend =m_textFieldTTF->getBlendFunc();
    m_textFieldTTF->setBlendFunc((ccBlendFunc) { GL_SRC_ALPHA, GL_ONE });
    

    float moveX = labelSize.width* (labelAnchorPoint.x-0.5);
    float moveY = labelSize.height* (labelAnchorPoint.y-0.5);
    
    float X = labelSize.width*0.5 + m_lineWidth + moveX;
    float Y = labelSize.height*0.5+ m_lineWidth + moveY;
    
    CCPoint position = ccp(X,Y);
    
    renderRexture->begin();
    for (int i=0; i<360; i+=60)
    {
        m_textFieldTTF->setPosition(ccp(position.x + sin(CC_DEGREES_TO_RADIANS(i))*m_lineWidth,position.y+cos(CC_DEGREES_TO_RADIANS(i))*m_lineWidth));
        m_textFieldTTF->visit();
    }
    renderRexture->end();
    m_textFieldTTF->setAnchorPoint(labelAnchorPoint);
    m_textFieldTTF-> setPosition(originalPos);
    m_textFieldTTF->setColor(originalColor);
    m_textFieldTTF->setBlendFunc(originalBlend);
    
    CCPoint strokePosition = ccp(labelSize.width/2,labelSize.height/2);
    renderRexture->setPosition(strokePosition);
    m_textFieldTTF->addChild(renderRexture,-1);
    m_renderTexture = renderRexture;
}
//=============================================================
//=============================================================
void CCTextField::setTextColor(ccColor3B color)
{
    m_textFieldTTF->setColor(color);
}
//=============================================================
//=============================================================
void CCTextField::attachWithIME()
{
    m_textFieldTTF->attachWithIME();
}
//=============================================================
//=============================================================
void CCTextField::detachWithIME()
{
    m_textFieldTTF->detachWithIME();
}
//=============================================================
//=============================================================
void CCTextField::refreshFlagLabelInput()
{
    if(m_flagLabel)
    {
        m_flagLabel->setAnchorPoint(ccp(0.5,0.5));
        m_flagLabel->setPosition(ccp(m_textFieldTTF->getContentSize().width,m_textFieldTTF->getContentSize().height*0.5)); 
    }
}
//=============================================================
//=============================================================
void CCTextField::refreshFlagLabelDelete()
{
    if(m_flagLabel)
    {
        std::string contentString = m_textFieldTTF->getString();
        if(contentString.length()>1)
        {
            m_flagLabel->setAnchorPoint(ccp(0.5,0.5));
            CCPoint newPos = ccp(m_textFieldTTF->getContentSize().width-m_textFieldTTF->getFontSize()*0.5,m_textFieldTTF->getContentSize().height*0.5);
            m_flagLabel->setPosition(newPos);
        }
        else
        {
            m_flagLabel->setAnchorPoint(ccp(0,0));
            CCPoint newPos = ccp(m_textFieldTTF->getContentSize().width-m_textFieldTTF->getFontSize()*0.5,0);
            m_flagLabel->setPosition(newPos);
        }
    }
}
//=============================================================
//=============================================================
bool CCTextField::canAttachWithIME()
{
    return (m_textDelegate) ? (! m_textDelegate->onTextFieldAttachWithIME(this)) : true;
}
//=============================================================
//=============================================================
bool CCTextField::canDetachWithIME()
{
    return (m_textDelegate) ? (! m_textDelegate->onTextFieldDetachWithIME(this)) : true;
}
//=============================================================
//=============================================================

