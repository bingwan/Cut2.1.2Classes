
#ifndef CCWater_LevelScene_h
#define CCWater_LevelScene_h

#include "cocos2d.h"
#include "Levellayer.h"
USING_NS_CC;

class LevelScene : public CCScene {
public:
    virtual ~LevelScene();
    LevelScene();
    virtual bool init();
    CREATE_FUNC(LevelScene);
     
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
    LevelLayer* getLevelLayer(){return m_pLevelLayer;}
protected:
private:
    LevelLayer* m_pLevelLayer;
};


#endif
