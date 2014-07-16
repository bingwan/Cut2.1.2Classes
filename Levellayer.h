

#ifndef CCWater_Levellayer_h
#define CCWater_Levellayer_h

#include "CCCutTouchLayer.h"
#include "Box2D.h"
#include "GLES-Render.h"
#include "Character.h"
#include "CCBodyRope.h"


class  LevelLayer : public CCLayerColor
{
public:
    
    LevelLayer();
	virtual			~LevelLayer();
	CREATE_FUNC(LevelLayer);
    
    virtual void onEnter();
    static  LevelLayer * create(const ccColor4B& color, GLfloat width, GLfloat height);
    virtual bool initWithColor(const ccColor4B& color, GLfloat width, GLfloat height);
    
//    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    //virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void draw();
    void onExit();
    
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
    
    //virtual void update(float dt);
    
    Character*  getCharacterMe(){return m_pCharacterMe;}
    void        ropeTwoCharacter(Character* pCharA ,Character* pCharB,float fLength );
    int         getCurStar();
    void        setCurStar(int nStar);
    void        reStarGame();
    void        gameSuccess();
    void        gameStart();
protected:
    void        clickBack();
    void        initBuff();
    void        deleteBodyRope(CCBodyRope* pRope);
    int         getBodyRopeIndex(CCBodyRope* pRope);
    void        cutRope(CCPoint pTouchPoint);
    void        clickCharacter(CCPoint pTouchPoint);
    void        deleteCurLevelAll();
    void        cleanAllCharacter();
    void        showGameResult();
    void        goHomeScene();
    void        initHat();
    Character*         getSameToBuffTypeCharacter(Character* pCharacter,int nBuffId);
private:
    CCCutTouchLayer*            m_pBgLayer;
    Character*                  m_pCharacterMe ;
    int                         m_nCurStar;
  
};


#endif
