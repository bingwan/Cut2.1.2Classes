

#include "BuffProcessInherit.h"
#include "Character.h"
#include "BuffObject.h"
#include "BuffManager.h"
#include "LevelScene.h"
#include "CCSceneManager.h"
#include "CharacterManager.h"
#include "Levellayer.h"

//bubble 
//================================================================
//================================================================
Buff001::Buff001(){}
Buff001::~Buff001(){}

bool Buff001::isContinued()
{
	return true;
}
bool Buff001::isOnced() 
{
	return true;
}
bool Buff001::onStartProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
    const char* szScale = pBuff->getBuffParam(eParamEnum_ScaleGravity);
    if(!szScale)
    {
        return false;
    }
//    std::string strScale = szScale;
//    float fScale = atof(strScale.c_str());
    
    float fScale = atof(szScale);
    pCharacter->setScaleGravity(fScale);
    
    if(fScale != 1)
    {
        pCharacter->showBall();
    }
    else 
    {
        pCharacter->removeBall();
    }
        
	return true;
}
bool Buff001::onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	return true;
}
bool Buff001::onEndProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	
	return true;
}
//================================================================
//================================================================
Buff002::Buff002(){}
Buff002::~Buff002(){}

bool Buff002::isContinued()
{
	return true;
}
bool Buff002::isOnced()
{
	return true;
}
bool Buff002::onStartProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
    Character* pFromCharacter = pBuff->getFromCharacter();
    if(!pFromCharacter)
    {
        return false;
    }
    LevelScene* pLevelScene = (LevelScene*)CCDirector::sharedDirector()->getRunningScene();
    if(!pLevelScene)
    {
        return false;
    }
    LevelLayer* pLevelLayer = pLevelScene->getLevelLayer();
    if(!pLevelLayer)
    {
        return false;
    }
    std::string strLength = pBuff->getBuffParam(eParamEnum_RopeLength);
    float fLength = atof(strLength.c_str());
    
//    std::string strRange = pBuff->getBuffParam(eParamEnum_RopeLength);
//    float fLength = atof(strLength.c_str());
    if(pFromCharacter->getLinkRopeTome())
    {
        return false;
    }
    pLevelLayer->ropeTwoCharacter(pLevelLayer->getCharacterMe(), pBuff->getFromCharacter(),fLength);
    pFromCharacter->setLinkRopeToMe(true);

	return true;
}
bool Buff002::onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	return true;
}
bool Buff002::onEndProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	return true;
}
//================================================================
//================================================================

Buff003::Buff003(){}
Buff003::~Buff003(){}

bool Buff003::isContinued()
{
	return true;
}
bool Buff003::isOnced()
{
	return true;
}
bool Buff003::onStartProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}

    LevelScene* pLevelScene = (LevelScene*)CCDirector::sharedDirector()->getRunningScene();
    if(!pLevelScene)
    {
        return false;
    }
    LevelLayer* pLevelLayer = pLevelScene->getLevelLayer();
    if(!pLevelLayer)
    {
        return false;
    }
    pLevelLayer->setCurStar(pLevelLayer->getCurStar() + 1);
    
	return true;
}
bool Buff003::onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	return true;
}
bool Buff003::onEndProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	
	return true;
}
//================================================================
//================================================================
Buff004::Buff004(){}
Buff004::~Buff004(){}

bool Buff004::isContinued()
{
	return true;
}
bool Buff004::isOnced()
{
	return true;
}
bool Buff004::onStartProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
    CharacterManager::shareCharacterManager()->deleteCharacterDelay(pCharacter);

	return true;
}
bool Buff004::onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	return true;
}
bool Buff004::onEndProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	
	return true;
}
//================================================================
//================================================================
//airball  向右的力量
Buff005::Buff005(){}
Buff005::~Buff005(){}

bool Buff005::isContinued()
{
	return true;
}
bool Buff005::isOnced()
{
	return true;
}
bool Buff005::onStartProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
    Character* pFromCharacter = pBuff->getFromCharacter();
    if(!pFromCharacter)
    {
        CCLog("Buff005::onStartProcessBuff=!pFromCharacter");
        return false;
    }

    CCPoint fromPos  = pFromCharacter->getPositionInWorld();
    CCPoint mePos  = pCharacter->getPositionInWorld();
    
    if(fromPos.x > mePos.x)
    {
        return false;
    }
    
    if(fabs( fromPos.x - mePos.x)  > 200)
    {
        return false;
    }
    
    if(ccpDistance(fromPos, mePos) > 250)
    {
        return false;
    }
    
    
    const char* szForce = pBuff->getBuffParam(eParamEnum_Force);
    if(!szForce)
    {
        CCLog("Buff005::onStartProcessBuff=!szForce");
        return false;
    }
    float fForce = atof(szForce);

    b2Vec2  force = b2Vec2(fForce,0);
    pCharacter->applyForceToCenter(force);
    
//    float fRadian = CC_DEGREES_TO_RADIANS(fRotation);
//    float fDirX = cosf(fRadian);
//    float fDirY = sinf(fRadian);
//    CCPoint dirPoint = ccp(fDirX,fDirY);
//    b2Vec2  force = b2Vec2(fForce*dirPoint.x,fForce*dirPoint.y);
//    pCharacter->applyForceToCenter(force);
    
	return true;
}
bool Buff005::onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	return true;
}
bool Buff005::onEndProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	
	return true;
}
//================================================================
//================================================================
//restar
Buff006::Buff006(){}
Buff006::~Buff006(){}

bool Buff006::isContinued()
{
	return true;
}
bool Buff006::isOnced()
{
	return true;
}
bool Buff006::onStartProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
    
    LevelScene* pLevelScene = (LevelScene*)CCDirector::sharedDirector()->getRunningScene();
    if(!pLevelScene)
    {
        return false;
    }
    LevelLayer* pLevelLayer = pLevelScene->getLevelLayer();
    if(!pLevelLayer)
    {
        return false;
    }
    pLevelLayer->reStarGame();
    
	return true;
}
bool Buff006::onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	return true;
}
bool Buff006::onEndProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	
	return true;
}
//================================================================
//================================================================
//success
Buff007::Buff007(){}
Buff007::~Buff007(){}

bool Buff007::isContinued()
{
	return true;
}
bool Buff007::isOnced()
{
	return true;
}
bool Buff007::onStartProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
    
    LevelScene* pLevelScene = (LevelScene*)CCDirector::sharedDirector()->getRunningScene();
    if(!pLevelScene)
    {
        return false;
    }
    LevelLayer* pLevelLayer = pLevelScene->getLevelLayer();
    if(!pLevelLayer)
    {
        return false;
    }
    
    pLevelLayer->gameSuccess();
    
	return true;
}
bool Buff007::onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	return true;
}
bool Buff007::onEndProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	
	return true;
}
//================================================================
//================================================================
//定身
Buff008::Buff008(){}
Buff008::~Buff008(){}

bool Buff008::isContinued()
{
	return true;
}
bool Buff008::isOnced()
{
	return true;
}
bool Buff008::onStartProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
    CharacterManager::shareCharacterManager()->stopMoveDelay(pCharacter);
	return true;
}
bool Buff008::onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	return true;
}
bool Buff008::onEndProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	
	return true;
}
//================================================================
//================================================================
//hat
Buff009::Buff009(){}
Buff009::~Buff009(){}

bool Buff009::isContinued()
{
	return true;
}
bool Buff009::isOnced()
{
	return true;
}
bool Buff009::onStartProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
        CCLog("NULL == pBuff || NULL == pCharacter ");
		return false;
	}
    
    Character* pFromChar = pBuff->getFromCharacter();
    if(!pFromChar)
    {
        CCLog("Buff009::onStartProcessBuff=!pFromChar");

        return false;
    }
        
    Character* pToHatChar = pFromChar->getToHatCharacter();
    if(!pToHatChar)
    {
        CCLog("Buff009::onStartProcessBuff=!pToHatChar");
        return false;
    }
       
    
    if(pFromChar->getFromOtherHat())
    {
        return false;
    }
    if( eBuffStateType_Begin == pBuff->getBuffStateType() )
    {
        CCPoint pos = pToHatChar->getWorldPos();
        CharacterManager::shareCharacterManager()->setTransformDelay(pCharacter,pos,0);
        pToHatChar->setFromOtherHat(true);
        pFromChar->setFromOtherHat(false);
    }
    else if(eBuffStateType_End == pBuff->getBuffStateType())
    {
        pFromChar->setFromOtherHat(false);
    }
	return true;
}
bool Buff009::onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	return true;
}
bool Buff009::onEndProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	
	return true;
}
//================================================================
//================================================================
//Scale 放大缩小
Buff010::Buff010(){}
Buff010::~Buff010(){}

bool Buff010::isContinued()
{
	return true;
}
bool Buff010::isOnced()
{
	return true;
}
bool Buff010::onStartProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
        CCLog("NULL == pBuff || NULL == pCharacter ");
		return false;
	}
    const char* szScaleX = pBuff->getBuffParam(ParamEnum_Scale_X);
    if(!szScaleX)
    {
        return false;
    }
    const char* szScaleY = pBuff->getBuffParam(ParamEnum_Scale_Y);
    if(!szScaleY)
    {
        return false;
    }
    const char* szScaleTime = pBuff->getBuffParam(ParamEnum_Scale_Time);
    if(!szScaleTime)
    {
        return false;
    }
    const char* szResetTime = pBuff->getBuffParam(ParamEnum_Reset_Time);
    if(!szResetTime)
    {
        return false;
    }
    
    float fScaleX = atof(szScaleX);
    float fScaleY = atof(szScaleY);
    float fScaleTime = atof(szScaleTime);
    float fResetTime = atof(szResetTime);
    pCharacter->showScaleAction(fScaleX,fScaleY,fScaleTime,fResetTime);
	return true;
}
bool Buff010::onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	return true;
}
bool Buff010::onEndProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	
	return true;
}
//================================================================
//================================================================
//向右加力
Buff011::Buff011(){}
Buff011::~Buff011(){}

bool Buff011::isContinued()
{
	return true;
}
bool Buff011::isOnced()
{
	return true;
}
bool Buff011::onStartProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
        CCLog("NULL == pBuff || NULL == pCharacter ");
		return false;
	}
    const char* szForce = pBuff->getBuffParam(ParamEnum_Force_X);
    if(!szForce)
    {
        CCLog("Buff005::onStartProcessBuff=!szForce");
        return false;
    }
    
    const char* szForceY = pBuff->getBuffParam(ParamEnum_Force_Y);
    if(!szForceY)
    {
        CCLog("Buff005::onStartProcessBuff=!szForce");
        return false;
    }
    float fForceX = atof(szForce);
    float fForceY = atof(szForceY);
    
    b2Vec2  force = b2Vec2(fForceX,fForceY);
    pCharacter->applyForceToCenter(force);
    CCLog("Buff011::onStartProcessBuff");
	return true;
}
bool Buff011::onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	return true;
}
bool Buff011::onEndProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	
	return true;
}
//================================================================
//================================================================


//================================================================
//================================================================
//zhuzi
Buff012::Buff012(){}
Buff012::~Buff012(){}

bool Buff012::isContinued()
{
	return true;
}
bool Buff012::isOnced()
{
	return true;
}
bool Buff012::onStartProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
        CCLog("NULL == pBuff || NULL == pCharacter ");
		return false;
	}
//    const char* szForce = pBuff->getBuffParam(ParamEnum_Force_X);
//    if(!szForce)
//    {
//        CCLog("Buff005::onStartProcessBuff=!szForce");
//        return false;
//    }
//    
//    const char* szForceY = pBuff->getBuffParam(ParamEnum_Force_Y);
//    if(!szForceY)
//    {
//        CCLog("Buff005::onStartProcessBuff=!szForce");
//        return false;
//    }
    float fForceX = 500;//atof(szForce); //bingwan todo
    float fForceY = 100;//atof(szForceY);
    
    b2Vec2  force = b2Vec2(fForceX,fForceY);
    pCharacter->applyForceToCenter(force);
    CCLog("Buff011::onStartProcessBuff");
	return true;
}
bool Buff012::onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	return true;
}
bool Buff012::onEndProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	if ( NULL == pBuff || NULL == pCharacter )
	{
		return false;
	}
	
	return true;
}
//================================================================
//================================================================



























