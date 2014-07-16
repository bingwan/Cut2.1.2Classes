

#ifndef CCWater_CommonEnum_h
#define CCWater_CommonEnum_h

enum SceneIdEnum
{
    SIE_UNKNOWN = 0,
    SIE_HOME = 1,
    SIE_LOGO = 2,
    SIE_SELECT = 3,
    SIE_LEVEL = 4,
    SIE_EDIT = 5,
};


enum FixtureShapeEnum
{
    PSE_UNKOWN = 0,
    FSE_CIRCLE = 1,
    FSE_EDGE = 2,
    FSE_POLOYJON = 3,
    FSE_CHAIN = 4,
    FSE_TYPECOUNT = 5,
    PSE_BOX = 6,
};

enum BufferType
{
    eBufferType_UNKNOWN = 0,
    eBufferType_ScaleGravity   = 1, //  重力的倍数  －1 相反方向
    eBufferType_CreateRopeToMe = 2, //  创建绳子
    eBufferType_AddStar        = 3, //  加星星
    eBufferType_Delete         = 4, //  删除
    eBufferType_AddForce       = 5, //  加力
    eBufferType_Failed         = 6, //  失败
    eBufferType_Success        = 7, //  成功
    eBufferType_StopMove       = 8, // 定身
    eBufferType_Hat            = 9, //hat
    eBufferType_ScaleAction    = 10, //放大缩小动画
    eBufferType_chuifengji     = 11, //吹风机
    eBufferType_zhuzi = 12,
};

//enum BufferReplaceType
//{
//    eBufferType_UNKNOWN = 0,
//    eBufferType_ScaleGravity   = 1, //  重力的倍数  －1 相反方向
//    eBufferType_CreateRopeToMe = 2, //  创建绳子
//    eBufferType_AddStar        = 3, //  加星星
//    eBufferType_Delete         = 4, //  删除
//};

enum CharacterType
{
    eCharacterType_Unknown = 0,
    eCharacterType_Me      = 1,
    eCharacterType_Npc     = 2,
    //eCharacterType_Home    = 3,
};

enum SpriteTagType
{
    eSpriteTagType_Unknown = 0,
    eSpriteTagType_Character = 1,
    eSpriteTagType_BodyRope = 2,
    
};

enum NeedToCharacterType
{
    eNeedToCharacterType_Unknown = 0,
    eNeedToCharacterType_Setpos  = 1,
};

enum BuffStateType
{
    eBuffStateType_Unknown = 0,
    eBuffStateType_Begin   = 1,
    eBuffStateType_End     = 2,
};


enum ContactStateType
{
    eContactStateType_Unknown = 0,
    eContactStateType_Begin   = 1,
    eContactStateType_Move    = 2,
    eContactStateType_End     = 3,
};

#endif
