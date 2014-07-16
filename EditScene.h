

#ifndef CCWater_EditScene_h
#define CCWater_EditScene_h

#include "cocos2d.h"
USING_NS_CC;

class EditScene : public CCScene {
public:
    EditScene();
    virtual ~EditScene();
    
    virtual bool init();
    CREATE_FUNC(EditScene);
    
    virtual void onEnter();
    //virtual void onEnterTransitionDidFinish();
    //virtual void onExit();
    
protected:
private:
};

#endif
