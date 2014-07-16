
#include "SelectLevelScene.h"
#include "SimpleAudioEngine.h"
#include "CCSceneManager.h"

#include "SelectLevelLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;

SelectLevelScene::SelectLevelScene()
{
}

SelectLevelScene::~SelectLevelScene()
{
}
bool SelectLevelScene::init()
{
    if (!CCScene::init())
    {
        return false;
    }
    
    return true;
}

void SelectLevelScene::onEnter()
{
    CCScene::onEnter();
    
    CCSize contentSize = getContentSize();
    
    
    SelectLevelLayer* pLayer = SelectLevelLayer::create();
    addChild(pLayer);
    
}

void SelectLevelScene::onEnterTransitionDidFinish()
{
    CCScene::onEnterTransitionDidFinish();
}

void SelectLevelScene::onExit()
{
    CCScene::onExit();
}