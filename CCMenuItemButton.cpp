


#include "CCMenuItemButton.h"


CCMenuItemButton::CCMenuItemButton():
m_imageAnchorPoint(ccp(0.5,0.5)),
m_target(NULL),
m_currentSelectorState(ISSE_UNKNOWN),
m_beginSelector(NULL),
m_moveSelector(NULL),
m_titleImageNor(NULL),
m_titleImageSel(NULL),
m_titleImageDis(NULL),
m_titleLabelNor(NULL),
m_titleLabelSel(NULL),
m_titleLabelDis(NULL),
m_selectedAble(true),
m_unselectedAble(true)
{
}

CCMenuItemButton::~CCMenuItemButton()
{
}

CCMenuItemButton* CCMenuItemButton::create(CCLabelStroke *titleLabelNor,CCLabelStroke *titleLabelSel, CCLabelStroke *titleLabelDis,CCObject* target, SEL_MenuHandler selector)
{
    CCMenuItemButton* item =  new CCMenuItemButton();
    item->autorelease();
    item->initWithNormalSprite(titleLabelNor,titleLabelSel,titleLabelDis,target,selector);
    return item;
}

CCMenuItemButton* CCMenuItemButton::create(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector)
{
    CCMenuItemButton* item =  new CCMenuItemButton();
    item->autorelease();
    item->initWithNormalImage(normalImage, selectedImage, disabledImage, target, selector);
    return  item;
}

// title label
void CCMenuItemButton::setTitleNormalLabel(CCLabelStroke* var)
{
    if(var)
    {
        addChild(var, CIZ_TITLE_LABEL);
        var->setVisible(true);
    }
    if(m_titleLabelNor)
    {
        m_titleLabelNor->removeFromParentAndCleanup(true);
    }
    m_titleLabelNor = var;
}

void CCMenuItemButton::setTitleSelectedLabel(CCLabelStroke* var)
{
    if(var)
    {
        addChild(var,CIZ_TITLE_LABEL);
        var->setVisible(false);
    }
    if(m_titleLabelSel)
    {
        m_titleLabelSel->removeFromParentAndCleanup(true);
    }
    m_titleLabelSel = var;
}

void CCMenuItemButton::setTitleDisabledLabel(CCLabelStroke* var)
{
    if(var)
    {
        addChild(var,CIZ_TITLE_LABEL);
        var->setVisible(false);
    }
    
    if(m_titleLabelDis)
    {
        m_titleLabelDis->removeFromParentAndCleanup(true);
    }
    m_titleLabelDis = var;
}

// title image
void CCMenuItemButton::setTitleNormalImage(CCSprite* var)
{
    if(var)
    {
        addChild(var,CIZ_TITLE_IMAGE);
        var->setVisible(true);
    }
    if(m_titleImageNor)
    {
        m_titleImageNor->removeFromParentAndCleanup(true);
    }
    m_titleImageNor = var;
}

void CCMenuItemButton::setTitleSelectedImage(CCSprite* var)
{
    if(var)
    {
        addChild(var,CIZ_TITLE_IMAGE);
        var->setVisible(false);
    }
    if(m_titleImageSel)
    {
        m_titleImageSel->removeFromParentAndCleanup(true);
    }
    m_titleImageSel = var;
}

void CCMenuItemButton::setTitleDisabledImage(CCSprite* var)
{
    if(var)
    {
        addChild(var,CIZ_TITLE_IMAGE);
        var->setVisible(false);
    }

    if(m_titleImageDis)
    {
        m_titleImageDis->removeFromParentAndCleanup(true);
    }
    m_titleImageDis = var;
}

void CCMenuItemButton::setTitleSprite(CCSprite* titleSpriteNor,CCSprite* titleSpriteSel,CCSprite* titleSpriteDis)
{
    if(titleSpriteNor)
    {
        titleSpriteNor->setAnchorPoint(ccp(0.5,0.5));
        titleSpriteNor->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
        setTitleNormalImage(titleSpriteNor);
    }
    if(titleSpriteSel)
    {
        titleSpriteSel->setAnchorPoint(ccp(0.5,0.5));
        titleSpriteSel->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
        setTitleSelectedImage(titleSpriteSel);
    }
       
    if(titleSpriteDis)
    {
        titleSpriteDis->setAnchorPoint(ccp(0.5,0.5));
        titleSpriteDis->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
        setTitleDisabledImage(titleSpriteDis);
    }
}


void CCMenuItemButton::setTitleImage(const char* titleImageNor,const char* titleImageSel, const char* titleImageDis)
{
    CCSprite* titleSpriteNor = NULL;
    CCSprite* titleSpriteSel = NULL;
    CCSprite* titleSpriteDis = NULL;
    
    if(titleImageNor)
        titleSpriteNor = CCSprite::create(titleImageNor);
    
    if(titleImageSel)
        titleSpriteSel = CCSprite::create(titleImageSel);
    
    if(titleImageDis)
        titleSpriteDis = CCSprite::create(titleImageDis);
    
    setTitleSprite(titleSpriteNor, titleSpriteSel, titleSpriteDis);
}

// bg
void CCMenuItemButton::setNormalImage(CCNode* var)
{
    if (var)
    {
        CCSprite* norSprite = (CCSprite*)var;
        CCSize size = norSprite->getContentSize();
        setContentSize(size);
        addChild(var,CIZ_BG_IMAGE);
        var->setAnchorPoint(m_imageAnchorPoint);
        var->setPosition(ccp(getContentSize().width*m_imageAnchorPoint.x,getContentSize().height*m_imageAnchorPoint.y));
        var->setVisible(true);
    }
    if (m_pNormalImage)
    {
        removeChild(m_pNormalImage, true);
    }
    m_pNormalImage = var;
}

void CCMenuItemButton::setSelectedImage(CCNode* var)
{
    if (var)
    {
        addChild(var,CIZ_BG_IMAGE);
        var->setAnchorPoint(m_imageAnchorPoint);
        var->setPosition(ccp(getContentSize().width*m_imageAnchorPoint.x,getContentSize().height*m_imageAnchorPoint.y));
        var->setVisible(false);
    }
    if (m_pSelectedImage)
    {
        removeChild(m_pSelectedImage, true);
    }
    m_pSelectedImage = var;
}

void CCMenuItemButton::setDisabledImage(CCNode* var)
{
    if (var)
    {
        addChild(var,CIZ_BG_IMAGE);
        var->setAnchorPoint(m_imageAnchorPoint);
        var->setPosition(ccp(getContentSize().width*m_imageAnchorPoint.x,getContentSize().height*m_imageAnchorPoint.y));
        var->setVisible(false);
    }
    if (m_pDisabledImage)
    {
        removeChild(m_pDisabledImage, true);
    }
    m_pDisabledImage = var;
}



void CCMenuItemButton::setImageAnchorPoint(CCPoint anchorPoint)
{
    m_imageAnchorPoint = anchorPoint;
}

CCPoint CCMenuItemButton::getImageAnchorPoint()
{
    return m_imageAnchorPoint;
}


void CCMenuItemButton::setIsEnabled(bool bEnabled)
{
    CCMenuItemImage::setEnabled(bEnabled);
    
    if (m_titleImageSel)
    {
        m_titleImageSel->setVisible(false);
    }
    if (bEnabled)
    {
        if(m_titleImageNor)
        m_titleImageNor->setVisible(true);
        
        if (m_titleImageDis)
        {
            m_titleImageDis->setVisible(false);
        }
    }
    else
    {
        if (m_titleImageDis)
        {
            m_titleImageDis->setVisible(true);
            if(m_titleImageNor)
            m_titleImageNor->setVisible(false);
        }
        else
        {
            if(m_titleImageNor)
            m_titleImageNor->setVisible(true);
        }
    }
    
    //label 
    if (m_titleLabelSel)
    {
        m_titleLabelSel->setVisible(false);
    }
    if (bEnabled)
    {
        if(m_titleLabelNor)
            m_titleLabelNor->setVisible(true);
        
        if (m_titleLabelDis)
        {
            m_titleLabelDis->setVisible(false);
        }
    }
    else
    {
        if (m_titleLabelDis)
        {
            m_titleLabelDis->setVisible(true);
            if(m_titleLabelNor)
                m_titleLabelNor->setVisible(false);
        }
        else
        {
            if(m_titleLabelNor)
                m_titleLabelNor->setVisible(true);
        }
    }
}

void CCMenuItemButton::selected()
{
    if(m_selectedAble == false) return;
    CCMenuItemImage::selected();
    
    if (m_titleImageDis)
    {
        m_titleImageDis->setVisible(false);
    }
    if (m_titleImageSel)
    {
        if(m_titleImageNor)
        m_titleImageNor->setVisible(false);
        m_titleImageSel->setVisible(true);
    }
    else
    {
        if(m_titleImageNor)
            m_titleImageNor->setVisible(true);
    }
    
    //label
    if (m_titleLabelDis)
    {
        m_titleLabelDis->setVisible(false);
    }
    if (m_titleLabelSel)
    {
        if(m_titleLabelNor)
            m_titleLabelNor->setVisible(false);
        m_titleLabelSel->setVisible(true);
    }
    else
    {
        if(m_titleLabelNor)
            m_titleLabelNor->setVisible(true);
    }
    
   
    if(m_currentSelectorState == ISSE_UNKNOWN)
    {
        m_currentSelectorState = ISSE_MOVE;
        if (m_target)
        {
            (m_target->*m_beginSelector)(this);
        }
    }
    else  //todo
    {
        if (m_target&&m_moveSelector)
        {
            (m_target->*m_moveSelector)(this);
        }
    }
}

void CCMenuItemButton::unselected()
{
    if(!m_unselectedAble) return;
    CCMenuItemImage::unselected();
    
    if(m_titleImageNor)
    m_titleImageNor->setVisible(true);
    
    if (m_titleImageSel)
    {
        m_titleImageSel->setVisible(false);
    }
    
    if (m_titleImageDis)
    {
        m_titleImageDis->setVisible(false);
    }
    
    //label
    if(m_titleLabelNor)
        m_titleLabelNor->setVisible(true);
    
    if (m_titleLabelSel)
    {
        m_titleLabelSel->setVisible(false);
    }
    
    if (m_titleLabelDis)
    {
        m_titleLabelDis->setVisible(false);
    }
    
    m_currentSelectorState = ISSE_UNKNOWN;
}


void CCMenuItemButton::setTitleString(const char* titleString,const char* fontName,float fontSize, ItemStateEnum itemState,
                                       ccColor3B textColor,
                                       int lineWidth, ccColor3B lineColor )
{
    CCLabelStroke* titleLabel = NULL;
    if(lineWidth > 0)
    {
        titleLabel = CCLabelStroke::labelWithString(titleString, fontName, fontSize, lineWidth, lineColor);
    }
    else
    {
        titleLabel = CCLabelStroke::labelWithString(titleString, fontName, fontSize);
    }
    titleLabel->setColor(textColor);
    titleLabel->setAnchorPoint(ccp(0.5,0.5));
    titleLabel->setPosition(ccp(getContentSize().width/2,getContentSize().height/2));
    switch (itemState)
    {
        case ISE_NORMAL:
        {
            setTitleNormalLabel(titleLabel);
        }
            break;
    case ISE_SELECT:
        {
            setTitleSelectedLabel(titleLabel);
        }
        break;
    case ISE_DISABLE:
        {
            setTitleDisabledLabel(titleLabel);
        }
        break;
        default:
            break;
    }
}


void CCMenuItemButton::setTarget(CCObject *target, SEL_MenuHandler selector, itemSelectorStateEnum selectorState)
{
    m_target = target;
    switch (selectorState) 
    {
        case ISSE_BEGIN:
        {
            m_beginSelector = selector;
        }
            break;
        case ISSE_MOVE:
        {
            m_moveSelector = selector;
        }
            break;
        case ISSE_END:
        {
        }
            break;
        default:
            break;
    }
}


