

#ifndef __BOX2D_MANAGER_H__
#define __BOX2D_MANAGER_H__

#include "cocos2d.h"
#include "Box2D.h"
#include "GLES-Render.h"
#include "CommonDef.h"
#include "CommonStruct.h"
#include "CCBodyRope.h"

USING_NS_CC;


//class DestructionListener : public b2DestructionListener
//{
//public:
//    void SayGoodbye(b2Fixture* fixture) { B2_NOT_USED(fixture); }
//    void SayGoodbye(b2Joint* joint);
//    
//    Box2dManager* test;
//};

class  Box2dManager : public CCObject ,public b2ContactListener
{
public:
	Box2dManager();
	virtual	~Box2dManager();
    static Box2dManager*                shareBox2dManager();
    void                                init();
    void                                draw();
    b2World*                            getWord(){return m_world;}
    void                                destroyJoint(b2Joint* pJoint);
    b2Body*                             createBody(BodyInfo info);
    void                                destroyBody(b2Body* pbody);
    virtual void                        update(float dt);
    void                                setAbleUpdate(bool bAble){m_bAbleUpdate = bAble;}
    bool                                getAbleUpdate (){return m_bAbleUpdate;}
    virtual void                        BeginContact(b2Contact* contact);
    virtual void                        EndContact(b2Contact* contact);
    virtual void                        PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void                        PostSolve(const b2Contact* contact, const b2ContactImpulse* impulse);
    
    void                                setDebugDraw(bool bDraw){_bDebugDraw = bDraw;}
protected:
    void                                createBodyFixtures(int bodyId,std::vector<int>fixtures);
    bool                                isContactGround(b2Body* pBodyA,b2Body* pBodyB);
private:
    b2World*                            m_world;
    GLESDebugDraw*                      m_debugDraw;
    bool                                m_bAbleUpdate;
    
    bool                                _bDebugDraw;
};


#define gBox2dManager   Box2dManager::GetSingleton()
#define gBox2dManagerPtr   Box2dManager::GetSingletonPtr()

#endif // __BOX2D_MANAGER_H__