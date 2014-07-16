

#include "BuffManager.h"
#include "BuffObject.h"
#include "BuffProcess.h"
#include "BuffProcessInherit.h"
#include "Character.h"
#include "tinyxml.h"
#include "CharacterManager.h"
#include "CCAPI.h"
#include "ResManager.h"

static BuffManager* s_pBufferManager = NULL;

BuffManager* BuffManager::share()
{
    if( NULL == s_pBufferManager)
    {
        s_pBufferManager = new BuffManager();
    }
    return s_pBufferManager;
}
BuffManager::BuffManager()
{
}

BuffManager::~BuffManager()
{
}

bool BuffManager::init()
{
    intBuffProcess();
	return true;
}

void BuffManager::intBuffProcess()
{
    m_mapBuffProcess[Buff001::BUFF_ID] = new  Buff001();
    m_mapBuffProcess[Buff002::BUFF_ID] = new  Buff002();
    m_mapBuffProcess[Buff003::BUFF_ID] = new  Buff003();
    m_mapBuffProcess[Buff004::BUFF_ID] = new  Buff004();
    m_mapBuffProcess[Buff005::BUFF_ID] = new  Buff005();
    m_mapBuffProcess[Buff006::BUFF_ID] = new  Buff006();
    m_mapBuffProcess[Buff007::BUFF_ID] = new  Buff007();
    m_mapBuffProcess[Buff008::BUFF_ID] = new  Buff008();
    m_mapBuffProcess[Buff009::BUFF_ID] = new  Buff009();
    m_mapBuffProcess[Buff010::BUFF_ID] = new  Buff010();
    m_mapBuffProcess[Buff011::BUFF_ID] = new  Buff011();
    m_mapBuffProcess[Buff012::BUFF_ID] = new  Buff012();
}


BuffObject* BuffManager::addBuffObject( Character* pCharacter,int buffTemplateId,unsigned int& buffUid, bool bTick)
{
	if(pCharacter == NULL || buffTemplateId ==  eBufferType_UNKNOWN)
		return NULL;
    
//    BuffTemplateInfo* pBuffTemplate = getBuffTemplate(buffTemplateId);
//    assert(pBuffTemplate);
    
//    if(pCharacter->hasBuff(buffTemplateId))
//    {
//            return NULL;
//
//    }
    

    BuffObject* pBuff = createBuffObject(buffTemplateId);
    pBuff->setTick(bTick);
    buffUid = pBuff->getUid();
    //pCharacter->delayAddBuff(pBuff);
    
//    if(buffTemplateId == 21)
//        pCharacter->delayAddBuff(pBuff);
    
    return pBuff;
}

BuffObject* BuffManager::createBuffObject(int buffTemplateId)
{
    BuffTemplateInfo* pBuffTemplate = ResManager::shareResManager()->getBuffTemplate(buffTemplateId);
    if(!pBuffTemplate)
        return NULL;
    BuffObject* pBuff = new  BuffObject();
	pBuff->setBuffTemplate(buffTemplateId);
    return pBuff;    
}


BuffProcess* BuffManager::getBuffProcess( BuffObject* pBuff)
{
	BuffTemplateInfo* pBuffTemplate = ResManager::shareResManager()->getBuffTemplate(pBuff->getBuffTemplate());
	if(pBuffTemplate)
	{
		std::map<int, BuffProcess*>::iterator it = m_mapBuffProcess.find(pBuffTemplate->eBufferType);
		if (it != m_mapBuffProcess.end())
		{
			return it->second;
		}
	}
	return NULL;
}

void  BuffManager::deleteBuff(BuffObject* pBuff)
{
    if(!pBuff)
        return;
    if(pBuff)
    {
        delete pBuff;
        pBuff = NULL;
    }
}

