

#include <iostream>

#include "LevelScene.h"
#include "SimpleAudioEngine.h"
#include "CCMenuItemButton.h"
#include "CCSceneManager.h"
#include "Levellayer.h"
#include "CCAPI.h"
#include "CCDrawLineLayer.h"
#include "Box2dManager.h"
#include "CharacterManager.h"

USING_NS_CC;

LevelScene::LevelScene()
{
   
}

LevelScene::~LevelScene()
{
}
bool LevelScene::init()
{
    if (!CCScene::init())
    {
        return false;
    }
    return true;
}
void LevelScene::onEnterTransitionDidFinish()
{
    CCScene::onEnterTransitionDidFinish();
}

void LevelScene::onExit()
{
    CCScene::onExit();
   
}

void LevelScene::onEnter()
{
    CCScene::onEnter();

    
    m_pLevelLayer = LevelLayer::create(ccc4(200, 0, 0, 0), getContentSize().width, getContentSize().height);
    m_pLevelLayer->setAnchorPoint(ccp(0,0));
    m_pLevelLayer->setPosition(ccp(0,0));
    addChild(m_pLevelLayer);
    
    CCDrawLineLayer* drawLineLayer = CCDrawLineLayer::create();
    drawLineLayer->setFingerIndex(0);
    drawLineLayer->setIsRandColor(true);
    addChild(drawLineLayer);   
    
}





