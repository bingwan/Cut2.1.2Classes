
#include "SelectLevelLayer.h"
#include "SimpleAudioEngine.h"
#include "CCSceneManager.h"
#include "CC3DListLayer.h"
using namespace cocos2d;
using namespace CocosDenshion;

SelectLevelLayer::SelectLevelLayer()
{
}

SelectLevelLayer::~SelectLevelLayer()
{
}
bool SelectLevelLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    return true;
}

void SelectLevelLayer::onEnter()
{
    CCLayer::onEnter();
    
    CCSize contentSize = getContentSize();
    CCSprite* pBgSprite = CCSprite::create("sel_level_bg.png");
    pBgSprite->setPosition(ccp(contentSize.width*0.5,contentSize.height*0.5));
    addChild(pBgSprite);
    
    CCArray* pArray = CCArray::create();
    for(int i=0;i<100;i++)
    {
        int nPng = arc4random()%3+1;
        char szPng[32];
        sprintf(szPng, "%d.png",nPng);
        CCSprite* pSprite = CCSprite::create(szPng);
        pArray->addObject(pSprite);
    }
    const char*  testPng = "1.png";
//    CCSize oneSize = CCSprite::create(testPng)->getContentSize();
//    CC3DListLayer* pLayer = CC3DListLayer::create(ccc4(0, 200, 0, 255*0), getContentSize().width, getContentSize().height);
//    pLayer->initDataCoverFlow(-oneSize.width*0.2, oneSize, pArray,true);
//    pLayer->setAnchorPoint(ccp(0,0));
//    pLayer->setPosition(ccp(0,0));
//    addChild(pLayer);
    
    
    CCSize oneSize = CCSprite::create(testPng)->getContentSize();
    CC3DListLayer* pLayer = CC3DListLayer::create(ccc4(0, 200, 0, 255*0), getContentSize().width,getContentSize().height);
    pLayer->initDataHalfEllipse(-60,contentSize.width*0.5, contentSize.height*0.5, oneSize, pArray, true);
    pLayer->setAnchorPoint(ccp(0,0));
    pLayer->setPosition(ccp(0,0));
    addChild(pLayer);
    
    pLayer->setDelegate(this);
    
    
    CCButton* pButton= CCButton::create("Back", "", 43);
    pButton->setPosition(ccp(contentSize.width*0.5,contentSize.height*0.8));
    pButton->addTargetWithActionForControlEvents(this, cccontrol_selector(SelectLevelLayer::clickBack),CCControlEventTouchUpInside);
    addChild(pButton);
}

void SelectLevelLayer::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
}

void SelectLevelLayer::onExit()
{
    CCLayer::onExit();
}

void  SelectLevelLayer::onCC3DListLayerSelectIndex(int nIndex)
{}
void  SelectLevelLayer::onCC3DListLayerClickSelect(int nIndex)
{
    int nLevel = nIndex+1;

    CCSceneManager::share()->setCurLevelId(nLevel);
    
    CCSceneManager::share()->replaceScene(SIE_LEVEL);
}

void SelectLevelLayer::clickBack()
{
    CCSceneManager::share()->replaceScene(SIE_HOME);
}




