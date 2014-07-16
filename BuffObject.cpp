

#include "BuffObject.h"
#include "BuffManager.h"
#include "ResManager.h"

#define MAX_BUFF_PARAM_NUM 10


BuffObject::BuffObject():
m_BuffTemplateId(-1),
m_Finished(false),
m_ContinueTime(0),
m_ElapsedTime(0),
m_tickCount(0),
m_soundValue(0.f),
m_itemId(0),
m_bTick(true),
m_index(0)
{
}
BuffObject::~BuffObject()
{	
}

bool BuffObject::setBuffTemplate(int buffId)
{
	BuffTemplateInfo* pBuffTemplate = ResManager::shareResManager()->getBuffTemplate(buffId);
    if(!pBuffTemplate)
        return false;
    m_BuffTemplateId = pBuffTemplate->Id;
	m_vParm = pBuffTemplate->vParm;
	return false;
}
const char* BuffObject::getBuffParam(int index)
{
    if(index>= m_vParm.size() || index<0)
    {
        return NULL;
    }
	return m_vParm[index].c_str();
}
bool BuffObject::setBuffParam(int index,std::string param)
{
    return true;
}







