

#ifndef __BUFF_MANAGER_H__
#define __BUFF_MANAGER_H__

#include "CommonStruct.h"

class Character;
class BuffObject;
class BuffProcess;

class  BuffManager
{
public:
	BuffManager();
	virtual ~BuffManager();
    static BuffManager*     share();
	bool                    init();
	BuffObject*             addBuffObject(Character* pCharacter,int buffTemplateId,unsigned int& buffUid, bool bTick = true);
    BuffObject*             createBuffObject(int buffTemplateId);
	BuffProcess*			getBuffProcess(BuffObject* pBuff);
    void                    deleteBuff(BuffObject* pBuff);
protected:
	void                    intBuffProcess();
private:
	std::map<int,BuffProcess*>           m_mapBuffProcess;
    

	
};


#endif // __BUFF_MANAGER_H__