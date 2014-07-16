//
//  HappyFishAppDelegate.h
//  HappyFish
//
//  Created by  on 12-8-12.
//  Copyright __MyCompanyName__ 2012年. All rights reserved.
//

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "CCApplication.h"
#include "CCSceneManager.h"
#include "Box2dManager.h"
#include "CCAPI.h"
#include "CharacterManager.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by CCDirector.
*/
class  AppDelegate : private cocos2d::CCApplication
{
public:
    AppDelegate();
    virtual ~AppDelegate();


    /**
    @brief    Implement CCDirector and CCScene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
    private:
    CCSceneManager* m_sceneManager;
    Box2dManager*  m_box2dManager;
    CCAPI*       m_ccAPI;
    CharacterManager* m_characterManager;
};

#endif // _APP_DELEGATE_H_

