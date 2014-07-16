

#ifndef __BuffObject_H__
#define __BuffObject_H__

#include "cocos2d.h"
#include "CommonDef.h"
#include "CommonEnum.h"

class Character;
USING_NS_CC;

class BuffObject : public CCObject
{
public:
	BuffObject();
	virtual	~BuffObject();
	//interface
    unsigned int getUid() { return m_uID;}
	bool		setBuffTemplate(int buffId);
	int			getBuffTemplate() { return m_BuffTemplateId; }
	void		setFinished(bool finish) { m_Finished = finish;}
	bool		getFinished() { return m_Finished;}
	bool		setBuffParam(int index,std::string param);
	const char* getBuffParam(int index);
	void		setContinuTime(float time) { m_ContinueTime = time;}
	float		getContinuTime() { return m_ContinueTime;}
	void		setElapsedTime(float time) { m_ElapsedTime = time; }
	float		getElapsedTime() { return m_ElapsedTime;}
    void        setTick(bool bTick){ m_bTick = bTick; }
    bool        getTick(){ return m_bTick; }
    void        setTickCount(int count){ m_tickCount = count; }
    int         getTickCount(){ return m_tickCount; }
    void        setSoundValue(float value){ m_soundValue = value; }
    float       getSoundValue(){return m_soundValue; }
    void        setInterval(float value){ m_interval = value; }
    float       getInterval(){ return m_interval; }
    void        setItemId(int itemId){ m_itemId = itemId; }
    int         getItemId(){ return m_itemId; }
    void        setIndex(int index){ m_index = index; }
    int         getIndex(){ return m_index; }
    void        setUIBuffObject(bool b){ m_bUIBuffObject = b; }
    bool        getUIBuffObject(){ return m_bUIBuffObject; }
    void        setFromCharacter(Character* pCharacter){m_pFromCharacter = pCharacter;}
    Character*  getFromCharacter(){return m_pFromCharacter;}
    void        setBuffStateType(BuffStateType type){m_eBuffStateType = type;}
    BuffStateType getBuffStateType(){return m_eBuffStateType;}
protected:
private:
	int			m_BuffTemplateId;
    std::vector<std::string>          m_vParm;
	bool		m_Finished;			//用于标记删除
	float		m_ContinueTime;		//buff总的时间
	float		m_ElapsedTime;		//buff持续时间
    bool        m_bTick;
    int         m_tickCount;
    float       m_soundValue;
    float       m_interval;
    int         m_itemId;
    int         m_index;
    bool        m_bUIBuffObject;
    Character*  m_pFromCharacter;
    BuffStateType  m_eBuffStateType;
	
};

#endif //__BuffObject_H__