

#ifndef _EditLayer_h
#define _EditLayer_h

#include "CCCutTouchLayer.h"
#include "CommonStruct.h"
#include "CCTextField.h"
#include "cocos-ext.h"

class CCScrollLayer;

USING_NS_CC;
USING_NS_CC_EXT;


class  EditLayer : public CCCutTouchLayer 
{
public:
    
    EditLayer();
	virtual			~EditLayer();
	virtual bool	init(); 
	CREATE_FUNC(EditLayer);
    
    virtual void onEnter();
    static  EditLayer * create(const ccColor4B& color, GLfloat width, GLfloat height);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    CharacterInfo* findCharacterInfo(int index);
    
protected:
    void clickCharacter(CCObject* pObject);
    int  getTouchCharacterIndex(CCPoint worldPoint);
    
    void addButtonScrollLayer();
    void addCharacterTemplateScrollLayer();
    void rotateSub();
    void rotateAdd();
    void rotate(float degree);
    
    void saveMap();
    void back();
    
    void addOldMap();
    void deleteChar(int nIndex);
    
    
    void loadLevel();
    void saveLevel();
    void loadOldMap(int nLevelId);
    
    void EditHide();
    
    
private:
    CCScrollLayer* m_characterScrollLayer;
    
    CCCutTouchLayer* m_mapLayer;
    CCScrollLayer* m_buttonScrollLayer;
        
    std::vector<CharacterInfo*> m_vAllCharacter;
    bool m_isTouchCharacter;
    int  m_currentCharacterIndex;
    
    CCTextField* m_pLoadTextField;
    CCTextField* m_pSaveTextField;
    
};



#endif
