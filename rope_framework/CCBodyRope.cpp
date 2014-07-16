//
//  CCBodyRope.cpp
//  Cut
//
//  Created by wangbin on 13-5-11.
//
//

#include "CCBodyRope.h"
#include "CommonDef.h"
#include "Box2dManager.h"

CCBodyRope::CCBodyRope()
{
    
}
CCBodyRope::~CCBodyRope()
{
}
void CCBodyRope::init(b2Body* pBodyA,b2Body* pBodyB,CCNode* pParentNode,float fLength)
{
    m_pBodyA = pBodyA;
    m_pBodyB = pBodyB;
    m_pParentNode = pParentNode;
    m_fLength = fLength/PTM_RATIO;
}
void CCBodyRope::createBodyRope()
{
    if(!m_pBodyA || !m_pBodyB)
        return;
    float fCircleR = 2* 5.0/PTM_RATIO;
    
    b2Vec2 anchorA =  b2Vec2(0,0);
    b2Vec2 anchorB =  b2Vec2(0,0);
    
    b2Vec2 dirDis = m_pBodyB->GetWorldPoint(anchorB) - m_pBodyA->GetWorldPoint(anchorA)  ;
    float32 ropeLength = dirDis.Length();
    
    if(m_fLength != 0)
    {
        ropeLength = m_fLength;
    }
    
    ropeLength = ropeLength - fCircleR*2;
    
    m_pRopeSpriteSheet = CCSpriteBatchNode::create("bodyrope.png");
    m_pParentNode->addChild(m_pRopeSpriteSheet,ROPE_Z);
    
    b2BodyDef bd;
    bd.type =  b2_dynamicBody;
    
    b2CircleShape circleShpae;
    circleShpae.m_radius = fCircleR;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShpae;
    fixtureDef.density = 0;
    fixtureDef.friction = 0;
    fixtureDef.isSensor = true;
    fixtureDef.restitution = 0;
    
    int nCircleCount = ropeLength/(fCircleR*2);
    for(int i=0;i<nCircleCount;i++)
    {
        float fPercent = ((float)i)/nCircleCount;
        float fX = m_pBodyA->GetWorldPoint(anchorA).x + fPercent* dirDis.x;
        float fY = m_pBodyA->GetWorldPoint(anchorA).y + fPercent* dirDis.y;
        bd.position = b2Vec2(fX,fY);;
        
        b2Body* pBody = Box2dManager::shareBox2dManager()->getWord()->CreateBody(&bd);
        pBody->CreateFixture(&fixtureDef);
        m_vBody.push_back(pBody);
        
        CCSprite* pRopeSprite = CCSprite::createWithTexture(m_pRopeSpriteSheet->getTexture());
        m_pRopeSpriteSheet->addChild(pRopeSprite);
        pBody->SetUserData(pRopeSprite);
        
        pRopeSprite->setUserData(this);
        pRopeSprite->setTag(eSpriteTagType_BodyRope);
    }
    
    b2RopeJointDef jd;
    jd.localAnchorA.Set(anchorA.x,anchorA.y);
    jd.localAnchorB.Set(anchorB.x,anchorB.y);
    jd.maxLength = fCircleR*2;
    
    jd.bodyA = m_pBodyA;
    jd.bodyB = m_vBody[0];
    b2Joint* pJoint = Box2dManager::shareBox2dManager()->getWord()->CreateJoint(&jd);
    m_vJoint.push_back(pJoint);
    
    for(int i=0;i<nCircleCount-1;i++)
    {
        jd.bodyA= m_vBody[i];
        jd.bodyB = m_vBody[i+1];
        pJoint = Box2dManager::shareBox2dManager()->getWord()->CreateJoint(&jd);
        m_vJoint.push_back(pJoint);
    }
    
    jd.bodyA= m_vBody[nCircleCount-1] ;
    jd.bodyB = m_pBodyB;
    pJoint = Box2dManager::shareBox2dManager()->getWord()->CreateJoint(&jd);
    m_vJoint.push_back(pJoint);
}

void CCBodyRope::removeBodyRope()
{
    if(m_pRopeSpriteSheet)
    {
        m_pRopeSpriteSheet->removeFromParentAndCleanup(true);
        m_pRopeSpriteSheet = NULL;
    }
    
    for(int i=0;i<m_vJoint.size();i++)
    {
        b2Joint* pJoint = m_vJoint[i];
        if(!pJoint)
        {
            continue;
        }
        Box2dManager::shareBox2dManager()->getWord()->DestroyJoint(pJoint);
    }
    m_vJoint.clear();
    
    for(int i=0;i<m_vBody.size();i++)
    {
        b2Body* pBody = m_vBody[i];
        if(!pBody)
        {
            continue;
        }
        pBody->SetUserData(NULL);
        Box2dManager::shareBox2dManager()->getWord()->DestroyBody(pBody);
    }
    m_vBody.clear();
}





