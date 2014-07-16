
#include "BuffProcess.h"

BuffProcess::BuffProcess()
{
}

BuffProcess::~BuffProcess()
{	
}
bool BuffProcess::isContinued()
{
	return false;
}
bool BuffProcess::isOnced()
{
	return true;
}
bool BuffProcess::onInit(Character* pCharacter,BuffObject* pBuff)
{
	return true;
}
bool BuffProcess::onStartProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	return true;
}

bool BuffProcess::onTickProcessBuff(Character* pCharacter,BuffObject* pBuff,float dt)
{
	return true;
}
bool BuffProcess::onEndProcessBuff(Character* pCharacter,BuffObject* pBuff)
{
	return true;
}