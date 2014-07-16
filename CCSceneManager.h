

#ifndef __CCSCENE_MANAGER_H__
#define __CCSCENE_MANAGER_H__

#include "cocos2d.h"
#include "CommonEnum.h"
USING_NS_CC;

class  CCSceneManager : public CCObject
{
public:
    
	CCSceneManager();
	virtual			~CCSceneManager();

    static CCSceneManager* share();
    void replaceScene(SceneIdEnum sceneId);
    void runWithScene(SceneIdEnum sceneId);
    
    CCScene* createScene(SceneIdEnum sceneId);
    
    void logoCallCreateNextScene();
    
    void setCurLevelId(int nId){m_nCurLevelId = nId;}
    
    int getCurLevelId(){return m_nCurLevelId;}
    
protected:
private:
    
    SceneIdEnum  m_nextSceneId;
    SceneIdEnum  m_preSceneId;
    SceneIdEnum  m_currentSceneId;
    
    int m_nCurLevelId;
    
};

#endif // __CCSCENE_MANAGER_H__