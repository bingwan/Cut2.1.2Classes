

#include <iostream>
#include "cocos2d.h"
#include "EditLayer.h"
#include "CCScrollLayer.h"
#include "CCButton.h"
#include "CCAPI.h"
#include "CharacterManager.h"
#include "tinyxml.h"
#include "CCMenuItemButton.h"
#include "CCSceneManager.h"
#include "Box2dManager.h"
#include "ResManager.h"
#include "CCTextField.h"
  

USING_NS_CC;
USING_NS_CC_EXT;

EditLayer::EditLayer():
m_characterScrollLayer(NULL),
m_mapLayer(NULL),
m_isTouchCharacter(NULL),
m_currentCharacterIndex(-1),
m_buttonScrollLayer(NULL)
{
}

EditLayer::~EditLayer()
{
}

EditLayer * EditLayer::create(const ccColor4B& color, GLfloat width, GLfloat height)
{
    EditLayer * pLayer = new EditLayer();
    if( pLayer && pLayer->initWithColor(color,width,height))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

bool EditLayer::init()
{
	if ( !CCCutTouchLayer::init() )
	{
		return false;
	}
    
    
    return true;
}

void EditLayer::onEnter()
{
    CCCutTouchLayer::onEnter();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //CCSize appSize = gCCAPIPtr->getAppSize();
    CCSize contentSize = getContentSize();
    
    m_mapLayer = CCCutTouchLayer::create(ccc4(0, 200, 0, 200),winSize.width,winSize.height);
    m_mapLayer->setPreventTouch(false);
    m_mapLayer->setAnchorPoint(ccp(0,0));
    m_mapLayer->setPosition(ccp(0,0));
    addChild(m_mapLayer);
    
    addButtonScrollLayer();
    addCharacterTemplateScrollLayer();
    
    CCMenu* pNormalMenu = CCMenu::create();
    pNormalMenu->setPosition(ccp(0,0));
    addChild(pNormalMenu);
    
    CCButton* pBackButton = CCButton::create("Back", "Arial", 25);
    pBackButton->setPosition(ccp(contentSize.width-30,30));
    pBackButton->addTargetWithActionForControlEvents(this, cccontrol_selector(EditLayer::back), CCControlEventTouchUpInside);
    addChild(pBackButton);
    
    
    CCButton* pSaveButton = CCButton::create("EditHide", "Arial", 25);
    pSaveButton->setPosition(ccp(contentSize.width-200,30));
    pSaveButton->addTargetWithActionForControlEvents(this, cccontrol_selector(EditLayer::EditHide), CCControlEventTouchUpInside);
    addChild(pSaveButton);
   
}

void EditLayer::EditHide()
{
    bool  bVisible= m_characterScrollLayer->isVisible();
    m_characterScrollLayer->setVisible(!bVisible);
    m_buttonScrollLayer->setVisible(!bVisible);
}

void EditLayer::loadOldMap(int nLevelId)
{
    char szLevel[32];
    sprintf(szLevel, "%d.xml",nLevelId);
    
    std::map<int, CharacterInfo> levelMap;
    ResManager::shareResManager()->loadLevelMap(szLevel,&levelMap);
    std::map<int, CharacterInfo>:: iterator it = levelMap.begin();
    int nIndex = 0;
    m_vAllCharacter.clear();
    while (it != levelMap.end()) 
    {
        CharacterInfo character = it->second;
        CharacterTemplateInfo* pCTI = ResManager::shareResManager()->getCharacterTemplateInfo(character.Id);
        if(!pCTI)
            continue;
        
        CharacterInfo* pEditCharacter = new CharacterInfo();
        CCLayerColor* pBgLayer = CCLayerColor::create(ccc4(200, 0, 0, 100),pCTI->contentSize.width,pCTI->contentSize.height);
        pBgLayer->setPosition(character.position);
        m_mapLayer->addChild(pBgLayer);
        pEditCharacter->pBgColorLayer = pBgLayer;
        pEditCharacter->Id = character.Id;
        pEditCharacter->position = character.position;
        pEditCharacter->rotate = character.rotate;
        
        //for(int i=0;i<pCTI->vBodyArray.size();++i)
        {
            BodyInfo* pBody = &pCTI->vBodyArray[0];
           
            BodyTemplateInfo* pBodyTemplate = ResManager::shareResManager()->getBodyTemplateInfo(pBody->Id);
            if(pBodyTemplate)
            {
                CCSprite* pBodySprite = CCSprite::create(pBodyTemplate->png.c_str());
                
                pBodySprite->setPosition(ccp(pCTI->contentSize.width*pBody->bodyPosition.x,pCTI->contentSize.height*pBody->bodyPosition.y));
                pBgLayer->addChild(pBodySprite);
            }
            
        }
        pEditCharacter->index = nIndex;
        m_vAllCharacter.push_back(pEditCharacter);
        it++;
        nIndex++;
    }
}


void EditLayer::addButtonScrollLayer()
{
    //CCSize appSize = gCCAPIPtr->getAppSize();
    
    int nFontSize = 25;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize contentSize = getContentSize();
    
    m_buttonScrollLayer = CCScrollLayer::scrollLayerWidthColor(ccc4(0, 0, 200, 200),CCSizeMake(winSize.width,100));
    m_buttonScrollLayer->setAnchorPoint(ccp(0,0));
    m_buttonScrollLayer->setPosition(ccp(0,300));
    addChild(m_buttonScrollLayer);
    
    float width = 100;
    float height = m_buttonScrollLayer->getContentSize().height;
    int X = width*0.5;
    int Y = height*0.5;
    
    m_pLoadTextField = CCTextField::create(ccc4(255, 0, 0, 200), "1", CCSizeMake(100, 50), kCCTextAlignmentCenter, "", nFontSize);
    m_pLoadTextField->setPosition(ccp(X,Y));
    m_buttonScrollLayer->addSubChild(m_pLoadTextField);
    
     X += width;
    CCButton* pButton = CCButton::create("加载","Arial",nFontSize);
    
    pButton->addTargetWithActionForControlEvents(this,cccontrol_selector(EditLayer::loadLevel),CCControlEventTouchUpInside);
    pButton->setPosition(ccp(X,Y));
    pButton->setPreventTouch(false);
    m_buttonScrollLayer->addSubChild(pButton);
    
    X += width;
    m_pSaveTextField = CCTextField::create(ccc4(255, 0, 0, 200), "1", CCSizeMake(100, 50), kCCTextAlignmentCenter, "", nFontSize);
    m_pSaveTextField->setPosition(ccp(X,Y));
    m_buttonScrollLayer->addSubChild(m_pSaveTextField);
    
    
    X += width;
    pButton = CCButton::create("保存","Arial",nFontSize);
    pButton->addTargetWithActionForControlEvents(this,cccontrol_selector(EditLayer::saveLevel),CCControlEventTouchUpInside);
    pButton->setPosition(ccp(X,Y));
    pButton->setPreventTouch(false);
    m_buttonScrollLayer->addSubChild(pButton);

//    X += width;
//    pButton = CCButton::create("R+","Arial",20);
//    pButton->addTargetWithActionForControlEvent(this,cccontrol_selector(EditLayer::rotateAdd),CCControlEventTouchUpInside);
//    pButton->setPosition(ccp(X,Y));
//    pButton->setPreventTouch(false);
//    m_buttonScrollLayer->addSubChild(pButton);
    
//    CCButton* pButton = CCButton::create("R-","Arial",20);
//    pButton->addTargetWithActionForControlEvent(this,cccontrol_selector(EditLayer::rotateSub),CCControlEventTouchUpInside);
//    pButton->setPosition(ccp(X,Y));
//    pButton->setPreventTouch(false);
//    m_buttonScrollLayer->addSubChild(pButton); 
//    
//    X += width;
//    pButton = CCButton::create("R+","Arial",20);
//    pButton->addTargetWithActionForControlEvent(this,cccontrol_selector(EditLayer::rotateAdd),CCControlEventTouchUpInside);
//    pButton->setPosition(ccp(X,Y));
//    pButton->setPreventTouch(false);
//    m_buttonScrollLayer->addSubChild(pButton); 
    
    
    X += width;
    m_buttonScrollLayer->setScrollContentSize(CCSizeMake(X,height));
}

void EditLayer::loadLevel()
{
    const char* pszLevelId = m_pLoadTextField->getString();
    if(!pszLevelId)
        return;
    int nLevelId = atoi(pszLevelId);
    if(nLevelId == 0)
        return;
    
    loadOldMap(nLevelId);
    
}

void EditLayer::saveLevel()
{
    saveMap();
}

void EditLayer::addCharacterTemplateScrollLayer()
{
    //CCSize appSize = gCCAPIPtr->getAppSize();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCSize contentSize = getContentSize();
    
    m_characterScrollLayer = CCScrollLayer::scrollLayerWidthColor(ccc4(200, 0, 0, 200),CCSizeMake(winSize.width,winSize.height*0.15));
    m_characterScrollLayer->setAnchorPoint(ccp(0,1));
    m_characterScrollLayer->setPosition(ccp(0,contentSize.height));
    addChild(m_characterScrollLayer);
    
    std::map<int,CharacterTemplateInfo*>* pCharacterMap = ResManager::shareResManager()->getCharacterTemplateMap();
    if(!pCharacterMap)
        return;
    
    int index = 0;
//    CCSprite* testSprite = CCSprite::create();
//    CCSize bgSize = testSprite->getContentSize();
    
    float width = 100;
    float height = m_characterScrollLayer->getContentSize().height;
    float originX = 0;
    
    float spaceX = 10;    
    int allCount = pCharacterMap->size();
    
    std::map<int, CharacterTemplateInfo*>:: iterator it = pCharacterMap->begin();
    while (it != pCharacterMap->end())
    {
        CharacterTemplateInfo* pCTI = it->second;
        float X = originX*2 + width/2+ (spaceX + width)*index;
        float Y = height/2;
        
        CCPoint position = ccp(X,Y);
    
        CCScale9Sprite* pItemSprite = CCScale9Sprite::create("Icon-72.png");
        CCButton* pItemButton = CCButton::create(pItemSprite);
        pItemButton->setPosition(position);
        pItemButton->addTargetWithActionForControlEvents(this, cccontrol_selector(EditLayer::clickCharacter), CCControlEventTouchUpInside);

        pItemButton->setTag(pCTI->nTabId);
        m_characterScrollLayer->addSubChild(pItemButton);
        
        CCSize buttonSize = pItemButton->getContentSize();
        
        //for(int i=0; i<pCTI->vBodyArray.size(); ++i)
        {
            BodyInfo bodyInfo = pCTI->vBodyArray[0];
            BodyTemplateInfo* pBodyTemplateInfo = ResManager::shareResManager()->getBodyTemplateInfo(bodyInfo.Id);
            if(pBodyTemplateInfo)
            {
                CCSprite* pBodySprite = CCSprite::create(pBodyTemplateInfo->png.c_str());
                pBodySprite->setPosition(ccp(buttonSize.width*0.5,buttonSize.height*0.5));
                pItemButton->addChild(pBodySprite);                
            }
        }
        it++;
        index++;
    }
    CCSize scrollContentSize = CCSizeMake(originX*2 +(spaceX + width)*allCount-spaceX,m_characterScrollLayer->getContentSize().height);
    m_characterScrollLayer->setScrollContentSize(scrollContentSize);
}

void EditLayer::clickCharacter(CCObject* pObject)
{
    CCControlButton* pButton = (CCControlButton*)pObject;
    int  characterId = pButton->getTag();
    CharacterTemplateInfo* pCTI = ResManager::shareResManager()->getCharacterTemplateInfo(characterId);
    if(!pCTI)
        return;
    CharacterInfo* pEditCharacter = new CharacterInfo();
    pEditCharacter->Id = pCTI->nTabId;
    pEditCharacter->index = m_vAllCharacter.size();
    
    CCLayerColor* pBgLayer = CCLayerColor::create(BG_COLOR_EDIT,pCTI->contentSize.width,pCTI->contentSize.height);
    pBgLayer->setPosition(ccp(m_mapLayer->getContentSize().width*0.5,m_mapLayer->getContentSize().height*0.5));
    pEditCharacter->position = pBgLayer->getPosition();
    pEditCharacter->pBgColorLayer = pBgLayer;
    m_mapLayer->addChild(pBgLayer);
    
    for(int i=0; i<pCTI->vBodyArray.size(); ++i)
    {
        BodyInfo bodyInfo = pCTI->vBodyArray[i];
        BodyTemplateInfo* pBodyTemplateInfo = ResManager::shareResManager()->getBodyTemplateInfo(bodyInfo.Id);
        if(pBodyTemplateInfo)
        {
            CCSprite* pBodySprite = CCSprite::create(pBodyTemplateInfo->png.c_str());
            pBodySprite->setPosition(ccp(pCTI->contentSize.width*bodyInfo.bodyPosition.x, pCTI->contentSize.height*bodyInfo.bodyPosition.y));
            pBgLayer->addChild(pBodySprite);                
        }
    }
    m_vAllCharacter.push_back(pEditCharacter);
}

CharacterInfo* EditLayer::findCharacterInfo(int index)
{
    if(index < 0 || index >= m_vAllCharacter.size())
        return NULL;
    return m_vAllCharacter[index];
}

int EditLayer::getTouchCharacterIndex(CCPoint worldPoint)
{
    for(int i=0;i<m_vAllCharacter.size();i++)
    {
        CharacterInfo* pCharacter = m_vAllCharacter[i];
        if(!pCharacter)
            continue;
        CCRect characterRect = pCharacter->pBgColorLayer->boundingBox();
        CCNode* pCharacterSpriteParent = pCharacter->pBgColorLayer->getParent();
        CCPoint originInWorld = pCharacterSpriteParent->convertToWorldSpace(characterRect.origin);
        CCRect characterRectInWorld = CCRectMake(originInWorld.x, originInWorld.y, characterRect.size.width, characterRect.size.height);
        if(characterRectInWorld.containsPoint(worldPoint))
        {
            return i;
        }
        
    }
    return -1;
}


bool EditLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if(CCCutTouchLayer::ccTouchBegan(pTouch, pEvent))
    {
        CCPoint touchPointInWorld = pTouch->getLocationInView();
        touchPointInWorld = CCDirector::sharedDirector()->convertToGL(touchPointInWorld);
        m_currentCharacterIndex = getTouchCharacterIndex(touchPointInWorld);
        return true;
    }

    return false;
    
}
void EditLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPointInWorld = pTouch->getLocationInView();
    CCPoint prePointInWorld = pTouch->getPreviousLocationInView();
    touchPointInWorld = CCDirector::sharedDirector()->convertToGL(touchPointInWorld);
    prePointInWorld = CCDirector::sharedDirector()->convertToGL(prePointInWorld);
    
    int moveX = touchPointInWorld.x - prePointInWorld.x;
    int moveY = touchPointInWorld.y - prePointInWorld.y;
    
    if(m_currentCharacterIndex == -1)
    {
        CCPoint oldPosition = m_mapLayer->getPosition();
        CCPoint newPosition = ccp(oldPosition.x+moveX,oldPosition.y+moveY);
        m_mapLayer->setPosition(newPosition);
    }
    else 
    {
        CharacterInfo* pECI = findCharacterInfo(m_currentCharacterIndex);
        if(pECI)
        {
            CCLayerColor* pBgNode = pECI->pBgColorLayer;
            CCPoint oldPosition = pBgNode->getPosition();
            CCPoint newPosition = ccp(oldPosition.x+moveX,oldPosition.y+moveY);
            pBgNode->setPosition(newPosition); 
        }
    }
}

void EditLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocation();
    if(m_currentCharacterIndex > -1)
    {
        CharacterInfo* pCI = findCharacterInfo(m_currentCharacterIndex);
        if(!pCI)
            return;
        CCLayerColor* pBgNode = pCI->pBgColorLayer;
        if(!pBgNode)
            return;
        pCI->position = pBgNode->getPosition();
        
        CCPoint charPos = pBgNode->getPosition();
        CCRect bgMapLayerRect = CCRectMake(0, 0, m_mapLayer->getContentSize().width, m_mapLayer->getContentSize().height);
        if(bgMapLayerRect.containsPoint(charPos) == false)
        {
            deleteChar(m_currentCharacterIndex);
        }
    }
    else
    {
       
    }
    
    m_currentCharacterIndex = -1;
}

void EditLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{}

void EditLayer::deleteChar(int nIndex)
{
    CharacterInfo* pCI = findCharacterInfo(nIndex);
    if(!pCI)
        return;
    pCI->pBgColorLayer->removeFromParent();
    delete pCI;
    
    m_vAllCharacter.erase(m_vAllCharacter.begin() + nIndex);
    
}

void EditLayer::rotate(float degree)
{
    CharacterInfo* pCI = findCharacterInfo(m_currentCharacterIndex);
    if(pCI)
    {
//         CCSprite* pCharacterSprite = pCI->pSprite;
//         float oldDegree = pCharacterSprite->getRotation();
//         pCharacterSprite->setRotation(oldDegree+degree);
//        
//        pCI->rotate = oldDegree+degree;
    }
}

void EditLayer::rotateSub()
{
    rotate(-10);
}
void EditLayer::rotateAdd()
{
    rotate(10);
}

void EditLayer::saveMap()
{
    const char* pszSaveId = m_pSaveTextField->getString();
    if(!pszSaveId)
        return;
    
    int nCurLevel = atoi(pszSaveId);
    if(nCurLevel == 0)
        return;
    
    char filename[128] = {0,0};
	sprintf(filename,"/%d.xml",nCurLevel);
	std::string DocumentsPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    DocumentsPath.append(filename); 
    
    TiXmlDocument tempDoc;
    const char* header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!--Copyright 2012 wangbin. All rights reserved.-->";
    tempDoc.Parse(header);
    
    TiXmlElement level_map("level");
    
    TiXmlElement character("character");
    
    for(int i=0;i<m_vAllCharacter.size();i++)
    {
        CharacterInfo* pCharacter = m_vAllCharacter[i];
        character.SetAttribute("index", i);
        character.SetAttribute("id", pCharacter->Id);
        character.SetAttribute("posx", pCharacter->position.x);
        character.SetAttribute("posy", pCharacter->position.y);
        
        level_map.InsertEndChild(character);
    }

    tempDoc.InsertEndChild(level_map);
    tempDoc.SaveFile(DocumentsPath.c_str()); 
}


void EditLayer::back()
{
    CCSceneManager::share()->replaceScene(SIE_HOME);
}













