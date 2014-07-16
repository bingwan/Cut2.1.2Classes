






#include "CCSceneManager.h"
#include "HomeScene.h"
#include "EditScene.h"
#include "LevelScene.h"
#include "SelectLevelScene.h"

CCSceneManager* s_pSceneManager = NULL;
CCSceneManager* CCSceneManager::share()
{
    if(NULL == s_pSceneManager)
    {
        s_pSceneManager = new CCSceneManager();
        
    }
    return s_pSceneManager;
}


CCSceneManager::CCSceneManager()
{
}

CCSceneManager::~CCSceneManager()
{
}

void CCSceneManager::replaceScene(SceneIdEnum sceneId)
{
    CCScene* pScene = createScene(sceneId);
    if(pScene)
    CCDirector::sharedDirector()->replaceScene(pScene);
    
}

void CCSceneManager::runWithScene(SceneIdEnum sceneId)
{
    CCScene* pScene = createScene(sceneId);
    CCDirector::sharedDirector()->runWithScene(pScene);
}

CCScene* CCSceneManager::createScene(SceneIdEnum sceneId)
{
    CCScene* pScene = NULL;
    switch (sceneId)
    {
        case SIE_HOME:
        {
           pScene = HomeScene::create();
        }
            break;
        case SIE_LOGO:
        {
        }
            break;
        case SIE_SELECT:
        {
            pScene = SelectLevelScene::create();
        }
            break;
        case SIE_LEVEL:
        {
            pScene = LevelScene::create();
        }
            break;
        case SIE_EDIT:
        {
            pScene = EditScene::create();
        }
            break;
            
        default:
            break;
    }
    return pScene;
}

void CCSceneManager::logoCallCreateNextScene()
{
    CCScene* pScene = createScene(m_nextSceneId);
    CCDirector::sharedDirector()->replaceScene(pScene);
    m_currentSceneId = m_nextSceneId;
}







