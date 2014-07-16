
#ifndef __BuffProcess_H__
#define __BuffProcess_H__

#include "CommonEnum.h"
#include "cocos2d.h"

USING_NS_CC;

class BuffObject;
class Character;

class BuffProcess
{
public:
	enum 
	{
		BUFF_ID = eBufferType_UNKNOWN,
	};
	virtual	~BuffProcess();
	//interface
	virtual bool	isContinued();
	virtual bool	isOnced();
	virtual bool	onInit(Character* pCharacter,BuffObject* pBuff);
	virtual	bool	onStartProcessBuff(Character* pCharacter,BuffObject* pBuff);
	virtual	bool	onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt);
	virtual	bool	onEndProcessBuff(Character* pCharacter,BuffObject* pBuff);
    virtual bool    canOverlap(Character* pCharacter,BuffObject* pBuff){ return false; }
    
    
protected:
	BuffProcess();
private:
};

#endif //__BuffProcess_H__