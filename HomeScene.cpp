
#include "HomeScene.h"
#include "SimpleAudioEngine.h"
#include "CCMenuItemButton.h"
#include "CCSceneManager.h"
#include "Box2dManager.h"
USING_NS_CC;

HomeScene::HomeScene()
{
}

HomeScene::~HomeScene()
{
}
bool HomeScene::init()
{
    if (!CCScene::init())
    {
        return false;
    }
    return true;
}

void HomeScene::onEnter()
{
    CCScene::onEnter();
    
    CCSize contentSize  = getContentSize();
    
    CCMenu* menu = CCMenu::create();
    menu->setPosition(ccp(0,0));
    addChild(menu);
    
    CCMenuItemImage* pEditButton = CCMenuItemImage::create("CloseNormal.png", NULL, NULL, this,menu_selector(HomeScene::edit));
    pEditButton->setPosition(ccp(contentSize.width*0.5,contentSize.height*0.2));
    menu->addChild(pEditButton,0);
    
    CCMenuItemImage* pPlayButton = CCMenuItemImage::create("CloseNormal.png", NULL, NULL, this,menu_selector(HomeScene::play));
    pPlayButton->setPosition(ccp(contentSize.width*0.5,contentSize.height*0.5));
    menu->addChild(pPlayButton,0);
    
    CCMenuItemImage* pButtonDebug = CCMenuItemImage::create("CloseNormal.png", NULL, NULL, this,menu_selector(HomeScene::debug));
    pButtonDebug->setPosition(ccp(contentSize.width*0.8,contentSize.height*0.2));
    menu->addChild(pButtonDebug,0);
}

void HomeScene::play()
{
    CCSceneManager::share()->replaceScene(SIE_SELECT);
}

void HomeScene::debug()
{
    Box2dManager::shareBox2dManager()->setDebugDraw(true);
}

void HomeScene::edit()
{
     CCSceneManager::share()->replaceScene(SIE_EDIT);
}

void HomeScene::onEnterTransitionDidFinish()
{
    CCScene::onEnterTransitionDidFinish();
    CCLog("HomeScene::onEnterTransitionDidFinish");
}

void HomeScene::onExit()
{
    CCScene::onExit();
    CCLog("HomeScene::onExit");
}