//
//  CCBodyRope.h
//  Cut
//
//  Created by wangbin on 13-5-11.
//
//

#ifndef __Cut__CCBodyRope__
#define __Cut__CCBodyRope__

#include "cocos2d.h"
#include "Box2D.h"
#include <iostream>
USING_NS_CC;

class CCBodyRope
{
public:
    CCBodyRope();
    virtual ~CCBodyRope();
    void    init(b2Body* bodyA,b2Body* bodyB,CCNode* pParentNode,float fLength);
    void    removeBodyRope();
    void    createBodyRope();
    
protected:
    std::vector<b2Body*>    m_vBody;
    std::vector<b2Joint*>   m_vJoint;
    CCSpriteBatchNode*      m_pRopeSpriteSheet;

    b2Body* m_pBodyA;
    b2Body* m_pBodyB;
    CCNode* m_pParentNode;
    float   m_fLength;
};


#endif /* defined(__Cut__CCBodyRope__) */
