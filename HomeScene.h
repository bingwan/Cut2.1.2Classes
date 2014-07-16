
#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class HomeScene : public CCScene {
public:
    virtual ~HomeScene();
    HomeScene();
    virtual bool init();
    CREATE_FUNC(HomeScene);
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
protected:
    void play();
    void edit();
    void debug();
private:
};

#endif // __HOME_SCENE_H__
