
#ifndef FlyFlyRookie_CCMenuItemImage_h
#define FlyFlyRookie_CCMenuItemImage_h

#include "cocos2d.h"
#include "CCLabelStroke.h"
USING_NS_CC;


class  CCMenuItemButton : public CCMenuItemImage
{
public:
    
    enum itemSelectorStateEnum
    {
        ISSE_UNKNOWN = 0,
        ISSE_BEGIN   = 1,
        ISSE_MOVE    = 2,
        ISSE_END     = 3
        
    };
    
    enum ItemStateEnum
    {
        ISE_UNKNOWN   = 0,
        ISE_NORMAL    = 1,
        ISE_SELECT    = 2,
        ISE_DISABLE    = 3
    };
    
    enum ChildItemZ
    {
        CIZ_UNKNOWN         = 0,
        CIZ_BG_IMAGE        = 10,
        CIZ_TITLE_IMAGE     = 20,
        CIZ_TITLE_LABEL     = 30,
        CIZ_OTHER           = 40,
    };

    CCMenuItemButton();
    virtual ~CCMenuItemButton();
    
    static CCMenuItemButton* create(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector);
    
    static CCMenuItemButton* create(CCLabelStroke *titleLabelNor,CCLabelStroke *titleLabelSel, CCLabelStroke *titleLabelDis,CCObject* target, SEL_MenuHandler selector);

    virtual void setNormalImage(CCNode* var);
    virtual void setSelectedImage(CCNode* var);
    virtual void setDisabledImage(CCNode* var);
    
    void setTitleNormalImage(CCSprite* var);
    void setTitleSelectedImage(CCSprite* var);
    void setTitleDisabledImage(CCSprite* var);
    CCSprite* getTitleNormalImage(){ return m_titleImageNor; }
    CCSprite* getTitleSelectImage(){ return m_titleImageSel; }
    CCSprite* getTitleDisableImage(){ return m_titleImageDis; }

    virtual void selected();
    virtual void unselected();
    virtual void setIsEnabled(bool bEnabled);
    
    void    setImageAnchorPoint(CCPoint anchorPoint);
    
    void    setTarget(CCObject *target, SEL_MenuHandler selector, itemSelectorStateEnum selectorState);
    void    setTitleImage(const char* titleImageNor,const char* titleImageSel, const char* titleImageDis);
    void    setTitleSprite(CCSprite* titleSpriteNor,CCSprite* titleSpriteSel,CCSprite* titleSpriteDis);

    void    setTitleString(const char* titleStringNor,const char* fontName,float fontSize, ItemStateEnum itemState, ccColor3B textColor = ccWHITE,int lineWidth = 0, ccColor3B lineColor = ccBLACK);
    void    setTitleDisabledLabel(CCLabelStroke* var);
    void    setTitleSelectedLabel(CCLabelStroke* var);
    void    setTitleNormalLabel(CCLabelStroke* var);
    CCPoint getImageAnchorPoint();
    
    void    setSelectedAble(bool able){m_selectedAble = able;}
    bool    getSelectedAble(){return  m_selectedAble;}
    
    void    setUnSelectedAble(bool able){m_unselectedAble = able;}
    bool    getUnSelectedAble(){return  m_unselectedAble;}
    
    
private:
    
    CCPoint             m_imageAnchorPoint;
    CCObject*           m_target;
    SEL_MenuHandler     m_beginSelector;
    SEL_MenuHandler     m_moveSelector;
    
    itemSelectorStateEnum m_currentSelectorState;
    
    CCSprite*       m_titleImageNor;
    CCSprite*       m_titleImageSel;
    CCSprite*       m_titleImageDis;
    
    CCLabelStroke*  m_titleLabelNor;
    CCLabelStroke*  m_titleLabelSel;
    CCLabelStroke*  m_titleLabelDis;
    
    bool            m_selectedAble;
    bool            m_unselectedAble;

};

#endif
