
#ifndef __SELECTLEVEL_SCENE_H__
#define __SELECTLEVEL_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class SelectLevelScene : public CCScene {
public:
    virtual ~SelectLevelScene();
    SelectLevelScene();
    virtual bool init();
    CREATE_FUNC(SelectLevelScene);
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
protected:
private:
};

#endif // __LOGO_SCENE_H__
