

#ifndef CCWater_CommonStruct_h
#define CCWater_CommonStruct_h

#include "cocos2d.h"
#include "Box2D.h"
#include "CommonEnum.h"
#include "CommonDef.h"

class Character;
USING_NS_CC;

struct BodyTemplateInfo
{
    BodyTemplateInfo(){clear();}
    ~BodyTemplateInfo(){}
    void clear()
    {
        Id = -1;
        png.clear();
        animateId = -1;
        fixtureIdArray.clear();
        gravityScale = 1;
        
    }
    int Id;
    std::string png;
    int animateId;
    std::vector<int> fixtureIdArray;
    float gravityScale ;
};


struct BodyInfo
{
    BodyInfo(){clear();}
    ~BodyInfo(){}
    void clear()
    {
        index =-1;
        Id = -1;
        bodyPosition = CCPointZero;
        rotate=0;
        bodyType = b2_staticBody;
    }
    int index;
    int Id;
    CCPoint bodyPosition;
    float rotate;
    b2BodyType  bodyType;
    
};

struct CharacterInfo
{
    CharacterInfo(){clear();}
    ~CharacterInfo(){}
    void clear()
    {
        index = -1;
        position = CCPointZero;
        rotate = 0;
        scale = 0;
        pBgColorLayer = NULL;
        Id = -1;
    }
    int             index;
    CCPoint         position;
    float           rotate;
    float           scale;
    CCLayerColor*   pBgColorLayer;
    int             Id;
};

struct JointInfo
{
    JointInfo(){clear();}
    ~JointInfo(){}
    void clear()
    {
        Id = -1;
        bodyAIndex =-1;
        bodyAPos = CCPointZero;
        bodyBIndex = -1;
        bodyBPos = CCPointZero;
        
    }
    int Id;
    int bodyAIndex;
    CCPoint bodyAPos;
    int bodyBIndex;
    CCPoint bodyBPos;
};

struct CharacterTemplateInfo
{
    CharacterTemplateInfo()
    {
        nTabId = 0;
        contentSize = CCSize(0,0);
        eType = eCharacterType_Unknown;
        vBodyArray.clear();
        vJointArray.clear();
        
        nInitBuffId = 0; //初始化buffID
        nToBeginBuffId =0; 
        nToEndBuffId =0;
        
        nSelfBeginBuffId =0;
        nSelfEndBuffId=0;
        
        nClickApplySelfBuffId=0; //点击时候buffId
        nClickApplyToMeBuffId=0; //点击时候buffId应用到me
    }
    int                     nTabId;
    CCSize                  contentSize;
    CharacterType           eType;
    std::vector<BodyInfo>   vBodyArray;
    std::vector<JointInfo>  vJointArray;
    
    int                     nInitBuffId; //初始化buffID
    int                     nToBeginBuffId; //
    int                     nToEndBuffId;
    
    int                     nSelfBeginBuffId;
    int                     nSelfEndBuffId;
    
    int                     nClickApplySelfBuffId; //点击时候buffId
    int                     nClickApplyToMeBuffId; //点击时候buffId应用到me

};

struct JointTemplateInfo
{
    JointTemplateInfo(){clear();}
    ~JointTemplateInfo(){}
    void clear()
    {
        Id = -1;
        jointType = e_unknownJoint;
        parmArray.clear();
        
    }
    int Id;
    b2JointType jointType;
    std::vector<float> parmArray;
    
};


//shape = NULL;
//userData = NULL;
//friction = 0.2f;
//restitution = 0.0f;
//density = 0.0f;
//isSensor = false;

struct FixtureTemplateInfo
{
    FixtureTemplateInfo():pts_count(0){}
    
    void addPt(b2Vec2& pt)
    {
        if(pts_count == MAX_FIXTURE_PTS)
        {
            assert(false);
        }
        pts[pts_count++] = pt;     
    }
    int fixtureid;
    FixtureShapeEnum shape;
    float friction;
    float restitution;
    float density;
    int isSensor;
    
    b2Vec2 pts[MAX_FIXTURE_PTS];
    int pts_count;
    float width;
    float height;
    float radius;    
};

struct BuffTemplateInfo
{
    int Id;
    BufferType                  eBufferType;
    std::vector<std::string>    vParm;
};


struct NeedToCharacter
{
    NeedToCharacterType eType;
    Character*          pChar;
    CCPoint             pos;
    
};

#endif
