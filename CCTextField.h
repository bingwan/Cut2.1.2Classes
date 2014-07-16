
/*
 *
 *  Created by wangbin 
 *
 */

#ifndef __TEXTFIELD_H__
#define __TEXTFIELD_H__

#include "CCCutTouchLayer.h"
#include "cocos2d.h"
USING_NS_CC;
class CCTextField;

class  CCTextDelegate
{
public:
    /**
     @brief	If the sender doesn't want to attach with IME, return true;
     */
    virtual bool onTextFieldAttachWithIME(CCTextField * sender)
    { 
        CC_UNUSED_PARAM(sender);
        return false;
    }
    
    /**
     @brief	If the sender doesn't want to detach with IME, return true;
     */
    virtual bool onTextFieldDetachWithIME(CCTextField * sender)
    {
        CC_UNUSED_PARAM(sender);
        return false;
    }
    
    /**
     @brief	If the sender doesn't want to insert the text, return true;
     */
    virtual bool  onTextFieldInsertText(CCTextField * sender, const char * text, int nLen)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(text);
        CC_UNUSED_PARAM(nLen);
        return false;
    }
    
    /**
     @brief	If the sender doesn't want to delete the delText, return true;
     */
    virtual bool onTextFieldDeleteBackward(CCTextField * sender, const char * delText, int nLen)
    {
        CC_UNUSED_PARAM(sender);
        CC_UNUSED_PARAM(delText);
        CC_UNUSED_PARAM(nLen);
        return false;
    }
    
    /**
     @brief	If doesn't want draw sender as default, return true.
     */
    virtual bool onDraw(CCTextField * sender)
    {
        CC_UNUSED_PARAM(sender);
        return false;
    }
};

class CCTextField : public CCCutTouchLayer, public CCTextFieldDelegate
{
public:
    
    enum TextFieldType
    {
        TFT_UNKNOWN    =0,
        TFT_NORMAL     =1,
        TFT_SECRET     =2,
        
    };
    
    CCTextField();
    virtual ~CCTextField();

    static  CCTextField* create(const ccColor4B color,const char* placeHolder,CCSize size,CCTextAlignment alignment,const char* fontName,int fontSize);
    
    virtual void        visit();
    virtual bool        initWithColorWidthHeight(const ccColor4B color, GLfloat width, GLfloat height);
    
    virtual bool        onTextFieldAttachWithIME(CCTextFieldTTF * sender);
    virtual bool        onTextFieldDetachWithIME(CCTextFieldTTF * sender);
    virtual bool        onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool        onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    
    virtual bool        ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void        ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual bool        canAttachWithIME();
    virtual bool        canDetachWithIME();
    const char*         getString();
	void                setString(const char* pStr);
    virtual void        setPlaceHolder(const char * text);
    virtual const char * getPlaceHolder(void);
    void                setType(TextFieldType aType){m_type = aType;}
    TextFieldType       getType(){return m_type;}
    void                setBackGroundColor(ccColor4B color);
    void                setTextPosition(CCPoint point);
    void                setTextColor(ccColor3B color);
    CCSize              getTextContentSize();
    void                setDelegate(CCTextDelegate* delegate){m_textDelegate = delegate;};
    void                setVisitFlag(bool visitFlag){m_visitFlag = visitFlag;}
    void                clearTextWhenTouch(bool clear){m_bClearWhenTouch = clear;}
    
protected:
    void                addTextField(const char* placeHolder,const char* fontName, int fontSize,CCTextAlignment alignment);
    void                refreshFlagLabelInput();
    void                refreshFlagLabelDelete();
    void                addSecretText();
    void                setStroke(float size,ccColor3B color);
    void                attachWithIME();
    void                detachWithIME();

private:
    const char*         m_fontName;
    int                 m_fontSize;
    CCTextFieldTTF*     m_textFieldTTF;
    CCPoint             m_beginPoint;
    SEL_CallFuncN       m_insertSelector;
    TextFieldType       m_type;
    std::string         m_contentString;
    std::string         m_secretTextString;
    CCTextAlignment     m_textAlignment;
    bool                m_bClearWhenTouch;
    CCSprite*           m_sprBg;
    float               m_lineWidth;
    ccColor3B           m_lineColor;      
    CCRenderTexture*    m_renderTexture;
    bool                m_visitFlag;
    CCTextDelegate*     m_textDelegate;
    CCLabelTTF*         m_flagLabel;
    bool                m_autoAttach;
};



#endif //__TEXTFIELD_H__