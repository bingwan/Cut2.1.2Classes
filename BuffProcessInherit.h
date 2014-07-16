

#ifndef __BUFFPROCESSINHERIT_H__
#define __BUFFPROCESSINHERIT_H__

#include "BuffProcess.h"
#include "Character.h"
#include "BuffObject.h"


class  Buff001 : public  BuffProcess   
{
public:
	enum 
	{
		BUFF_ID = eBufferType_ScaleGravity,
	};
	enum ParamEnum
	{
		eParamEnum_ScaleGravity  =0,
		
	};
    Buff001();
	virtual	~ Buff001();
	virtual bool	isContinued();
	virtual bool	isOnced();
	virtual	bool	onStartProcessBuff(Character* pCharacter, BuffObject* pBuff);
	virtual	bool	onTickProcessBuff(Character* pCharacter, BuffObject* pBuff,float dt);
	virtual	bool	onEndProcessBuff(Character* pCharacter, BuffObject* pBuff);

protected:
private:
};

// 产生绳子
class  Buff002 : public  BuffProcess
{
public:
	enum
	{
		BUFF_ID = eBufferType_CreateRopeToMe,
	};
	enum ParamEnum
	{
		eParamEnum_Range        =0,
        eParamEnum_RopeLength   =1,
		
	};
    Buff002();
	virtual	~ Buff002();
	virtual bool	isContinued();
	virtual bool	isOnced();
	virtual	bool	onStartProcessBuff(Character* pCharacter, BuffObject* pBuff);
	virtual	bool	onTickProcessBuff(Character* pCharacter, BuffObject* pBuff,float dt);
	virtual	bool	onEndProcessBuff(Character* pCharacter, BuffObject* pBuff);
    
protected:
private:
};


// 加星星
class  Buff003 : public  BuffProcess
{
public:
	enum
	{
		BUFF_ID = eBufferType_AddStar,
	};
	enum ParamEnum
	{
	};
    Buff003();
	virtual	~ Buff003();
	virtual bool	isContinued();
	virtual bool	isOnced();
	virtual	bool	onStartProcessBuff(Character* pCharacter, BuffObject* pBuff);
	virtual	bool	onTickProcessBuff(Character* pCharacter, BuffObject* pBuff,float dt);
	virtual	bool	onEndProcessBuff(Character* pCharacter, BuffObject* pBuff);
    
protected:
private:
};

//删除
class  Buff004 : public  BuffProcess
{
public:
	enum
	{
		BUFF_ID = eBufferType_Delete,
	};
	enum ParamEnum
	{
	};
    Buff004();
	virtual	~ Buff004();
	virtual bool	isContinued();
	virtual bool	isOnced();
	virtual	bool	onStartProcessBuff(Character* pCharacter, BuffObject* pBuff);
	virtual	bool	onTickProcessBuff(Character* pCharacter, BuffObject* pBuff,float dt);
	virtual	bool	onEndProcessBuff(Character* pCharacter, BuffObject* pBuff);
    
protected:
private:
};

//加力
class  Buff005 : public  BuffProcess
{
public:
	enum
	{
		BUFF_ID = eBufferType_AddForce,
	};
	enum ParamEnum
	{
        eParamEnum_Force =0,
	};
    Buff005();
	virtual	~ Buff005();
	virtual bool	isContinued();
	virtual bool	isOnced();
	virtual	bool	onStartProcessBuff(Character* pCharacter, BuffObject* pBuff);
	virtual	bool	onTickProcessBuff(Character* pCharacter, BuffObject* pBuff,float dt);
	virtual	bool	onEndProcessBuff(Character* pCharacter, BuffObject* pBuff);
    
protected:
private:
};
//失败
class  Buff006 : public  BuffProcess
{
public:
	enum
	{
		BUFF_ID = eBufferType_Failed,
	};
	enum ParamEnum
	{
	};
    Buff006();
	virtual	~ Buff006();
	virtual bool	isContinued();
	virtual bool	isOnced();
	virtual	bool	onStartProcessBuff(Character* pCharacter, BuffObject* pBuff);
	virtual	bool	onTickProcessBuff(Character* pCharacter, BuffObject* pBuff,float dt);
	virtual	bool	onEndProcessBuff(Character* pCharacter, BuffObject* pBuff);
    
protected:
private:
};
//成功
class  Buff007 : public  BuffProcess
{
public:
	enum
	{
		BUFF_ID = eBufferType_Success,
	};
	enum ParamEnum
	{
	};
    Buff007();
	virtual	~ Buff007();
	virtual bool	isContinued();
	virtual bool	isOnced();
	virtual	bool	onStartProcessBuff(Character* pCharacter, BuffObject* pBuff);
	virtual	bool	onTickProcessBuff(Character* pCharacter, BuffObject* pBuff,float dt);
	virtual	bool	onEndProcessBuff(Character* pCharacter, BuffObject* pBuff);
    
protected:
private:
};

//定身
class  Buff008 : public  BuffProcess
{
public:
	enum
	{
		BUFF_ID = eBufferType_StopMove,
	};
	enum ParamEnum
	{
	};
    Buff008();
	virtual	~ Buff008();
	virtual bool	isContinued();
	virtual bool	isOnced();
	virtual	bool	onStartProcessBuff(Character* pCharacter, BuffObject* pBuff);
	virtual	bool	onTickProcessBuff(Character* pCharacter, BuffObject* pBuff,float dt);
	virtual	bool	onEndProcessBuff(Character* pCharacter, BuffObject* pBuff);
    
protected:
private:
};

//hat
class  Buff009 : public  BuffProcess
{
public:
	enum
	{
		BUFF_ID = eBufferType_Hat,
	};
	enum ParamEnum
	{
	};
    Buff009();
	virtual	~ Buff009();
	virtual bool	isContinued();
	virtual bool	isOnced();
	virtual	bool	onStartProcessBuff(Character* pCharacter, BuffObject* pBuff);
	virtual	bool	onTickProcessBuff(Character* pCharacter, BuffObject* pBuff,float dt);
	virtual	bool	onEndProcessBuff(Character* pCharacter, BuffObject* pBuff);
    
protected:
private:
};
//==========================================
//==========================================
//scale 
class  Buff010 : public  BuffProcess
{
public:
	enum
	{
		BUFF_ID = eBufferType_ScaleAction,
	};
	enum ParamEnum
	{
        ParamEnum_Scale_X           =0,
        ParamEnum_Scale_Y           =1,
        ParamEnum_Scale_Time        =2,
        ParamEnum_Reset_Time        =3,
        
        //x
        //y
        //scale time
        //reset time
	};
    Buff010();
	virtual	~ Buff010();
	virtual bool	isContinued();
	virtual bool	isOnced();
	virtual	bool	onStartProcessBuff(Character* pCharacter, BuffObject* pBuff);
	virtual	bool	onTickProcessBuff(Character* pCharacter, BuffObject* pBuff,float dt);
	virtual	bool	onEndProcessBuff(Character* pCharacter, BuffObject* pBuff);
    
protected:
private:
};
//==========================================
//==========================================
// 吹风机
class  Buff011 : public  BuffProcess
{
public:
	enum
	{
		BUFF_ID = eBufferType_chuifengji,
	};
	enum ParamEnum
	{
        ParamEnum_Force_X           =0,
        ParamEnum_Force_Y           =1,
    
	};
    Buff011();
	virtual	~ Buff011();
	virtual bool	isContinued();
	virtual bool	isOnced();
	virtual	bool	onStartProcessBuff(Character* pCharacter, BuffObject* pBuff);
	virtual	bool	onTickProcessBuff(Character* pCharacter, BuffObject* pBuff,float dt);
	virtual	bool	onEndProcessBuff(Character* pCharacter, BuffObject* pBuff);
    
protected:
private:
};

//==========================================
//==========================================
// 竹子
class  Buff012 : public  BuffProcess
{
public:
	enum
	{
		BUFF_ID = eBufferType_zhuzi,
	};
	enum ParamEnum
	{
        ParamEnum_Force           =0,    
	};
    Buff012();
	virtual	~ Buff012();
	virtual bool	isContinued();
	virtual bool	isOnced();
	virtual	bool	onStartProcessBuff(Character* pCharacter, BuffObject* pBuff);
	virtual	bool	onTickProcessBuff(Character* pCharacter, BuffObject* pBuff,float dt);
	virtual	bool	onEndProcessBuff(Character* pCharacter, BuffObject* pBuff);
    
protected:
private:
};



#endif //__BUFFPROCESSINHERIT_H__