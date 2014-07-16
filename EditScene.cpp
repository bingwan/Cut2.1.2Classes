

#include "EditScene.h"

#include "CCMenuItemButton.h"
#include "CCSceneManager.h"
#include "EditLayer.h"
#include "CCAPI.h"

USING_NS_CC;

EditScene::EditScene()
{
}

EditScene::~EditScene()
{
}
bool EditScene::init()
{
    if (!CCScene::init())
    {
        return false;
    }
    return true;
}
void EditScene::onEnter()
{
    CCScene::onEnter();    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //CCSize appSize = gCCAPIPtr->getAppSize();
    
    CCSize contentSize = getContentSize();
    
    EditLayer* pLayer = EditLayer::create(ccc4(100, 100, 0,255),winSize.width,winSize.height);
    pLayer->setAnchorPoint(ccp(0,0));
    pLayer->setPosition(ccp(0,0));
    addChild(pLayer);
}