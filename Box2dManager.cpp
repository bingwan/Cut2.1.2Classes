






#include "Box2dManager.h"
#include "tinyxml.h"
#include "CCAPI.h"
#include "CommonStruct.h"
#include "cocos2d.h"
#include "Character.h"
#include "CharacterManager.h"
#include "BuffManager.h"
#include "ResManager.h"

USING_NS_CC;

static Box2dManager* s_pBox2dManager = NULL;
Box2dManager* Box2dManager::shareBox2dManager()
{
    if( NULL == s_pBox2dManager)
    {
        s_pBox2dManager = new Box2dManager();
    }
    return s_pBox2dManager;
}

Box2dManager::Box2dManager():
m_world(NULL),
m_debugDraw(NULL),
m_bAbleUpdate(false)
{
    _bDebugDraw = false;
}

Box2dManager::~Box2dManager()
{
    if(m_world)
    {
        delete m_world;
    }
}

void Box2dManager::init()
{
    b2Vec2 gravity;
	gravity.Set(0.0f, GRAVITY);
	
	bool doSleep = true;
    
    m_world = new b2World(gravity);
    m_world->SetAllowSleeping(doSleep);    
	m_world->SetContinuousPhysics(true);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    float fSpaceX = 100;
    float fSpaceY = 100;
    
    CCSize boundsSize = CCSizeMake(winSize.width+fSpaceX*2, winSize.height+fSpaceY*2);
    
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_kinematicBody;
    groundBodyDef.position.Set(winSize.width/2/PTM_RATIO,
                               winSize.height/2/PTM_RATIO); // bottom-left corner
    
    
    b2Body* groundBody = m_world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    
    // bottom
    CCPoint bottomCenterPoint = ccp(0, -(boundsSize.height*0.5));
    b2Vec2 bottomCenter = b2Vec2(bottomCenterPoint.x/PTM_RATIO,bottomCenterPoint.y/PTM_RATIO);
    groundBox.SetAsBox(boundsSize.width*0.5/PTM_RATIO, 0, bottomCenter, 0);
    groundBody->CreateFixture(&groundBox, 0);
    
    // top
    CCPoint topCenterPoint = ccp(0, (boundsSize.height*0.5));
    b2Vec2 topCenter = b2Vec2(topCenterPoint.x/PTM_RATIO,topCenterPoint.y/PTM_RATIO);
    groundBox.SetAsBox(boundsSize.width*0.5/PTM_RATIO, 0, topCenter, 0);
    groundBody->CreateFixture(&groundBox, 0);
    
    // left
    CCPoint leftCenterPoint = ccp(-(boundsSize.width*0.5),0);
    b2Vec2 leftCenter = b2Vec2(leftCenterPoint.x/PTM_RATIO,leftCenterPoint.y/PTM_RATIO);
    groundBox.SetAsBox(0, boundsSize.height*0.5/PTM_RATIO, leftCenter, 0);
    groundBody->CreateFixture(&groundBox, 0);
    
    // right
    CCPoint rightCenterPoint = ccp(boundsSize.width*0.5,0);
    b2Vec2 rightCenter = b2Vec2(rightCenterPoint.x/PTM_RATIO,rightCenterPoint.y/PTM_RATIO);
    groundBox.SetAsBox(0, boundsSize.height*0.5/PTM_RATIO,rightCenter, 0);
    groundBody->CreateFixture(&groundBox, 0);
    
    m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    m_world->SetDebugDraw(m_debugDraw);
    
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    //flags += b2Draw::e_aabbBit;
    //flags += b2Draw::e_pairBit;
    //flags += b2Draw::e_centerOfMassBit;
    m_debugDraw->SetFlags(flags);
    
    
   // m_destructionListener.test = this;
    //m_world->SetDestructionListener(&m_destructionListener);
    m_world->SetContactListener(this);
    
   // m_world->SetContinuousPhysics(false);
}

bool Box2dManager::isContactGround(b2Body* pBodyA,b2Body* pBodyB)
{
    if(pBodyA == NULL || pBodyA == NULL)
        return false;
    
    b2Body* pBodyMe = NULL;
    if(pBodyA->GetType() == b2_kinematicBody && pBodyB->GetType() == b2_dynamicBody)
    {
        pBodyMe = pBodyB;
    }
    else if(pBodyA->GetType() == b2_dynamicBody && pBodyB->GetType() == b2_kinematicBody)
    {
        pBodyMe = pBodyA;
    }
    if(!pBodyMe)
        return false;
    
    CCSprite* pContactSprite = (CCSprite*)pBodyMe->GetUserData();
    if(!pContactSprite)
    {
        CCLog("!pContactSprite");
        return  false;
    }
        
    Character* pCharacter = (Character*)pContactSprite->getUserData();
    if(!pCharacter)
    {
        CCLog("!pCharacter");
        return false;
    }
        
    if(eCharacterType_Me != pCharacter->getCharacterType())
    {
        CCLog("eCharacterType_Me != pCharacter->getCharacterType()");
        return false;
    }
    pCharacter->addBuff(61, NULL,eBuffStateType_Unknown);  //重新开始 61= 失败
    return true;
}
// Callbacks for derived classes.
void Box2dManager::BeginContact(b2Contact* pContact)
{
    if(!pContact)
    {
        return;
    }
    CCLog("Box2dManager::BeginContact");
    
    b2Fixture* pFixtureA = pContact->GetFixtureA();
    b2Fixture* pFixtureB = pContact->GetFixtureB();
    if(pFixtureA == NULL || pFixtureB == NULL)
    {
        CCLog("pFixtureA == NULL || pFixtureB == NULL");
        return;
    }
    
    b2Body* pBodyA = pFixtureA->GetBody();
    b2Body* pBodyB = pFixtureB->GetBody();
    if(pBodyA == NULL || pBodyA == NULL)
        return;
    
    if(isContactGround(pBodyA,pBodyB))
        return;
    
    CCSprite* pContactSpriteA = (CCSprite*)pBodyA->GetUserData();
    CCSprite* pContactSpriteB = (CCSprite*)pBodyB->GetUserData();
    if(pContactSpriteA == NULL || pContactSpriteB == NULL)
        return;
    
    if(pContactSpriteA->getTag() != eSpriteTagType_Character || pContactSpriteB->getTag() != eSpriteTagType_Character)
        return;
    
    Character* pCharacterA = (Character*)pContactSpriteA->getUserData();
    Character* pCharacterB = (Character*)pContactSpriteB->getUserData();
    if(pCharacterA == NULL || pCharacterB == NULL)
        return;
    
    if(eCharacterType_Me != pCharacterA->getCharacterType() &&
       eCharacterType_Me != pCharacterB->getCharacterType())
    {
        return;
    }
    
    Character* pCharacterMe = NULL;
    Character* pCharacterNpc = NULL;
    if(eCharacterType_Me == pCharacterA->getCharacterType()
       && eCharacterType_Npc == pCharacterB->getCharacterType())
    {
        pCharacterMe = pCharacterA;
        pCharacterNpc = pCharacterB;
    }
    else if(eCharacterType_Npc  == pCharacterA->getCharacterType()
            && eCharacterType_Me == pCharacterB->getCharacterType())
    {
        pCharacterMe = pCharacterB;
        pCharacterNpc = pCharacterA;
    }
    if(!pCharacterMe || !pCharacterNpc)
    {
        return;
    }
    
    pCharacterMe->setContactStateType(eContactStateType_Begin);
    pCharacterNpc->setContactStateType(eContactStateType_Begin);

    
    int nBuffId = pCharacterNpc->getApplyToMeContectBeginBuffId();
    pCharacterMe->addBuff(nBuffId,pCharacterNpc,eBuffStateType_Begin);
    pCharacterNpc->addBuff(pCharacterNpc->getSelfContectBeginBuffId(), NULL,eBuffStateType_Begin);
    
    
}
void Box2dManager::EndContact(b2Contact* pContact)
{
    if(!pContact)
    {
        CCLog("!pContact");
        return;
    }
    
    b2Fixture* pFixtureA = pContact->GetFixtureA();
    b2Fixture* pFixtureB = pContact->GetFixtureB();
    if(pFixtureA == NULL || pFixtureB == NULL)
    {
        CCLog("pFixtureA == NULL || pFixtureB == NULL");
        return;
    }
    
    //b2Manifold* mainfold = contact->GetManifold();
    b2Body* pBodyA = pFixtureA->GetBody();
    b2Body* pBodyB = pFixtureB->GetBody();
    if(pBodyA == NULL || pBodyA == NULL)
    {
        CCLog("pBodyA == NULL || pBodyA == NULL");

        return;
    }
    
    CCSprite* pContactSpriteA = (CCSprite*)pBodyA->GetUserData();
    CCSprite* pContactSpriteB = (CCSprite*)pBodyB->GetUserData();
    if(pContactSpriteA == NULL || pContactSpriteB == NULL)
    {
        CCLog("pContactSpriteA == NULL || pContactSpriteB == NULL");
        return;
    }
        
    if(pContactSpriteA->getTag() != eSpriteTagType_Character || pContactSpriteB->getTag() != eSpriteTagType_Character)
    {
        CCLog("pContactSpriteA->getTag() != eSpriteTagType_Character || pContactSpriteB->getTag() != eSpriteTagType_Character");
        return;
    }
    
    Character* pCharacterA = (Character*)pContactSpriteA->getUserData();
    Character* pCharacterB = (Character*)pContactSpriteB->getUserData();
    if(pCharacterA == NULL || pCharacterB == NULL)
    {
        CCLog("pCharacterA == NULL || pCharacterB == NULL");
        return;
    }
    
    if(eCharacterType_Me != pCharacterA->getCharacterType() &&
       eCharacterType_Me != pCharacterB->getCharacterType())
    {
        CCLog("eCharacterType_Me != pCharacterA->getCharacterType() && eCharacterType_Me != pCharacterB->getCharacterType()");
        return;
    }
    
    Character* pCharacterMe = NULL;
    Character* pCharacterNpc = NULL;
    if(eCharacterType_Me == pCharacterA->getCharacterType()
       && eCharacterType_Npc == pCharacterB->getCharacterType())
    {
        pCharacterMe = pCharacterA;
        pCharacterNpc = pCharacterB;
    }
    else if(eCharacterType_Npc  == pCharacterA->getCharacterType()
            && eCharacterType_Me == pCharacterB->getCharacterType()) 
    {
        pCharacterMe = pCharacterB;
        pCharacterNpc = pCharacterA;
    }
    if(!pCharacterMe || !pCharacterNpc)
    {
        CCLog("!pCharacterMe || !pCharacterNpc");
        return;
    }
    pCharacterMe->setContactStateType(eContactStateType_End);
    pCharacterNpc->setContactStateType(eContactStateType_End);
    
    int nBuffId = pCharacterNpc->getApplyToMeContentEndBuffId();
    pCharacterMe->addBuff(nBuffId,pCharacterNpc,eBuffStateType_End);
    pCharacterNpc->addBuff(pCharacterNpc->getSelfContectBeginBuffId(), NULL,eBuffStateType_End);
    pCharacterNpc->setFromOtherHat(false);

}
void Box2dManager::PreSolve(b2Contact* pContact, const b2Manifold* oldManifold)
{
    CCLog("Box2dManager::PreSolve");
    if(!pContact)
    {
        return;
    }
    
    b2Fixture* pFixtureA = pContact->GetFixtureA();
    b2Fixture* pFixtureB = pContact->GetFixtureB();
    if(pFixtureA == NULL || pFixtureB == NULL)
    {
        CCLog("Box2dManager::PreSolve=pFixtureA == NULL || pFixtureB == NULL");
        return;
    }
    
    b2Body* pBodyA = pFixtureA->GetBody();
    b2Body* pBodyB = pFixtureB->GetBody();
    if(pBodyA == NULL || pBodyA == NULL)
    {
        CCLog("Box2dManager::PreSolve=pBodyA == NULL || pBodyA == NULL");
        return;
    }
       
    
    if(isContactGround(pBodyA,pBodyB))
    {
        CCLog("Box2dManager::PreSolve=isContactGround(pBodyA,pBodyB)");

        return;
    }
    
    CCSprite* pContactSpriteA = (CCSprite*)pBodyA->GetUserData();
    CCSprite* pContactSpriteB = (CCSprite*)pBodyB->GetUserData();
    if(pContactSpriteA == NULL || pContactSpriteB == NULL)
    {
        CCLog("Box2dManager::PreSolve=pContactSpriteA == NULL || pContactSpriteB == NULL");

        return;
    }
    if(pContactSpriteA->getTag() != eSpriteTagType_Character || pContactSpriteB->getTag() != eSpriteTagType_Character)
    {
        CCLog("Box2dManager::PreSolve=pContactSpriteA->getTag() != eSpriteTagType_Character || pContactSpriteB->getTag() != eSpriteTagType_Character");

        return;
    }
    
    Character* pCharacterA = (Character*)pContactSpriteA->getUserData();
    Character* pCharacterB = (Character*)pContactSpriteB->getUserData();
    if(pCharacterA == NULL || pCharacterB == NULL)
    {
        CCLog("Box2dManager::PreSolve=pCharacterA == NULL || pCharacterB == NULL");
        return;
    }
    
    if(eCharacterType_Me != pCharacterA->getCharacterType() &&
       eCharacterType_Me != pCharacterB->getCharacterType())
    {
        CCLog("Box2dManager::PreSolve=eCharacterType_Me != pCharacterA->getCharacterType() && eCharacterType_Me != pCharacterB->getCharacterType()");

        return;
    }
    
    Character* pCharacterMe = NULL;
    Character* pCharacterNpc = NULL;
    if(eCharacterType_Me == pCharacterA->getCharacterType()
       && eCharacterType_Npc == pCharacterB->getCharacterType())
    {
        pCharacterMe = pCharacterA;
        pCharacterNpc = pCharacterB;
    }
    else if(eCharacterType_Npc  == pCharacterA->getCharacterType()
            && eCharacterType_Me == pCharacterB->getCharacterType())
    {
        pCharacterMe = pCharacterB;
        pCharacterNpc = pCharacterA;
    }
    if(!pCharacterMe || !pCharacterNpc)
    {
        CCLog("!pCharacterMe || !pCharacterNpc");
        return;
    }
    pCharacterMe->setContactStateType(eContactStateType_Begin);
    pCharacterNpc->setContactStateType(eContactStateType_Begin);
    
    int nBuffId = pCharacterNpc->getApplyToMeContectBeginBuffId();
    pCharacterMe->addBuff(nBuffId,pCharacterNpc,eBuffStateType_Begin);
    pCharacterNpc->addBuff(pCharacterNpc->getSelfContectBeginBuffId(), NULL,eBuffStateType_Begin);
    
    CCLog("Box2dManager::PreSolve:end");

}
void Box2dManager::PostSolve(const b2Contact* contact, const b2ContactImpulse* impulse)
{
    CCLog("Box2dManager::PostSolve");

}

void Box2dManager::destroyJoint(b2Joint* pJoint)
{
    if(!pJoint)
        return;
    m_world->DestroyJoint(pJoint);
}

void Box2dManager::destroyBody(b2Body* pbody)
{
    if(!pbody)
        return;
    m_world->DestroyBody(pbody);
}

b2Body* Box2dManager::createBody(BodyInfo aBody)
{
    BodyTemplateInfo* pBTI = ResManager::shareResManager()->getBodyTemplateInfo(aBody.Id);
    if(!pBTI)
        return NULL;
    
    b2BodyDef bodyDef;
    bodyDef.type = aBody.bodyType;
    b2Body* pBody = m_world->CreateBody(&bodyDef);
    if(!pBody)
        return NULL;
    
    pBody->SetGravityScale(pBTI->gravityScale);
    
    for(int indexF=0;indexF<pBTI->fixtureIdArray.size();++indexF)
    {
        int fixtureId = pBTI->fixtureIdArray[indexF];
        FixtureTemplateInfo* pFTI = ResManager::shareResManager()->getFixture(fixtureId);
        assert(pFTI);
        
        //                PSE_UNKOWN = 0,
        //                FSE_CIRCLE = 1,
        //                FSE_EDGE = 2,
        //                FSE_POLOYJON = 3,
        //                FSE_CHAIN = 4,
        //                FSE_TYPECOUNT = 5,
        //                PSE_BOX = 6,
        b2FixtureDef fixtureDef;
        switch (pFTI->shape)
        {
            case FSE_CIRCLE:
            {
                b2CircleShape circleShpae;
                circleShpae.m_radius = pFTI->radius/PTM_RATIO;
                fixtureDef.shape = &circleShpae;
                
            }
                break;
            case FSE_EDGE:
            {}
                break;
            case FSE_POLOYJON:
            {
                b2PolygonShape polygonShape;
                polygonShape.Set(pFTI->pts, pFTI->pts_count);
                fixtureDef.shape = &polygonShape;
            }
                break;
            case FSE_CHAIN:
            {}
                break;
            case FSE_TYPECOUNT:
            {}
                break;
            case PSE_BOX:
            {
                b2PolygonShape boxShape;
                boxShape.SetAsBox(pFTI->width/PTM_RATIO, pFTI->height/PTM_RATIO);
                fixtureDef.shape = &boxShape;
            }
                break;
                
            default:
                break;
        }
        fixtureDef.density = pFTI->density;
        fixtureDef.friction = pFTI->friction;
        fixtureDef.restitution = pFTI->restitution;
        fixtureDef.isSensor = pFTI->isSensor;
        pBody->CreateFixture(&fixtureDef);
    }
    return pBody;
    
}


//Filter是"过滤"的意思，所以FilterData可以简单的理解成用来过滤碰撞刚体(比如接下来哦我么要实现的矩形只与矩形刚体碰撞，圆形只与圆形刚体碰撞)。FilterData有3个属性：groupIndex、categoryBits和maskBits，他们的用法如下：
//
//groupIndex：表示刚体的分组信息。相同groupIndex属性的刚体属性一个组，groupIndex为正数时，刚体只和同组的刚体发生碰撞。groupIndex为负数时，刚体只和同组之外的刚体进行碰撞。


//categoryBits：表示刚体的分组信息，但不决定要碰撞的分组对象。另外，值得注意的，这个值必须是2的N次方。当然设置成其他值，程序不会报错，但是实际的碰撞分类效果，可能会出现意想不到的差错。


//maskBits：表示刚体要碰撞的那个刚体分组对象。这个值通常是另外一个FilterData对象的categoryBits属性，表示只与该类刚体发生碰撞。如果要对多组刚体进行碰撞，可以设置maskBits为多个categoryBits的加合。如要和categoryBits分别为2和4的刚体组都进行碰撞，可以设置maskBits属性为6。


//举个例子，比如，圆形刚体的categoryBits和maskBits分别为2和2，矩形刚体的categoryBits和maskBits分别为4和4。那么圆形与矩形刚体之间不会发生碰撞，只有相同形状刚体之间才会发生碰撞。
//
//FilterData对象可以通过赋值给b2FixtureDef对象filter属性来设置，非常简单吧！


void Box2dManager::update(float dt)
{
    if(!m_bAbleUpdate)
        return;
    
    int velocityIterations = 8;
    int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    m_world->Step(dt, velocityIterations, positionIterations);
    
    //Iterate over the bodies in the physics world
    for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) 
        {
            CCSprite* pBodySprite  = (CCSprite*)b->GetUserData();
            if(!pBodySprite)
            {
                continue;
            }
            CCNode* pCharBgNode = pBodySprite->getParent();
            if(!pCharBgNode)
            {
                continue;
            }
            CCPoint localPos = pCharBgNode->convertToNodeSpace(CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
            pBodySprite->setPosition(localPos);
            pBodySprite->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
        }    
    }
}

void Box2dManager::draw()
{
    if(!m_bAbleUpdate)
        return;
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    if(_bDebugDraw)
    {
        m_world->DrawDebugData();
    }
    
//#if BOX2D_DEBUG_DRAW
//    m_world->DrawDebugData();
//#endif
    
    kmGLPopMatrix();
}

void Box2dManager::createBodyFixtures(int bodyId,std::vector<int>fixtures)
{}


//    for(int i=0;i<m_vNeedToDeleteBodyArray.size();++i)
//    {
//        b2Body* pBody = m_vNeedToDeleteBodyArray[i];
//        if(!pBody)
//            continue;
//        m_world->DestroyBody(pBody);
//    }
//    m_vNeedToDeleteBodyArray.clear();
//
//
//    for(int i=0;i<m_vNeedToDeleteJointArray.size();++i)
//    {
//        b2Joint* pJoint = m_vNeedToDeleteJointArray[i];
//        if(!pJoint)
//            continue;
//        m_world->DestroyJoint(pJoint);
//    }
//    m_vNeedToDeleteJointArray.clear();