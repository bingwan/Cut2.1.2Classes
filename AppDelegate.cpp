//
//  HappyFishAppDelegate.cpp
//  HappyFish
//
//  Created by  on 12-8-12.
//  Copyright __MyCompanyName__ 2012年. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "CCSceneManager.h"
#include "HomeScene.h"
#include "ResManager.h"
#include "BuffManager.h"
USING_NS_CC;

AppDelegate::AppDelegate():
m_ccAPI(NULL),
m_box2dManager(NULL),
m_sceneManager(NULL),
m_characterManager(NULL)
{
    if(m_ccAPI == NULL)
    {
        m_ccAPI = new CCAPI();
    }
    
    if(m_sceneManager==NULL)
    {
        m_sceneManager = new CCSceneManager();
    }
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    pDirector->setDisplayStats(true);
    pDirector->setAnimationInterval(1.0 / 60);
    pDirector->getOpenGLView()->setDesignResolutionSize(1024, 683, kResolutionShowAll);
    
    
    ResManager::shareResManager()->init();
    BuffManager::share()->init();
    
    
    CharacterManager::shareCharacterManager()->init();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(CharacterManager::shareCharacterManager(), -1024, false);
    
    Box2dManager::shareBox2dManager()->init();
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(Box2dManager::shareBox2dManager(), -1023, false);
        
    HomeScene* pHome = HomeScene::create();
    CCDirector::sharedDirector()->runWithScene(pHome);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
