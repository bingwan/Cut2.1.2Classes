

#include "ResManager.h"
#include "CommonDef.h"
#include "CCAPI.h"
#include "tinyxml.h"

ResManager* ResManager::shareResManager()
{
    static ResManager* s_UIInstance = NULL;
    if(NULL == s_UIInstance)
    {
        s_UIInstance = new ResManager();
    }
    return s_UIInstance;
}
//=============================================================
//=============================================================
ResManager::ResManager()
{
}
//=============================================================
//=============================================================
void ResManager::init()
{
    loadCharacterTemplate("character.xml");
    loadBuffTemplate("buffer.xml");
    loadBodyFixtureTemplate("fixture.xml");
    loadJointTemplate("joint.xml");
    loadBodyTemplate("body.xml");
}
//=============================================================
//=============================================================
ResManager::~ResManager()
{}
//=============================================================
//=============================================================
bool ResManager::loadLevelMap(const char* file,std::map<int, CharacterInfo>*  levelmap)
{
    TiXmlDocument tempDoc;
	//Documents-----Resources
	std::string DocumentsPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    DocumentsPath.append("/");
    DocumentsPath.append(file);
	std::string ResourcesPath = CCAPI::share()->getResourcePath();
	ResourcesPath.append("/");
    ResourcesPath.append(file);
    
	if(!tempDoc.LoadFile(DocumentsPath.c_str()))
	{
		if(!tempDoc.LoadFile(ResourcesPath.c_str()))
		{
			CCLog("XML Parse Failed.Erro=%s,(row=%d, col=%d).",tempDoc.ErrorDesc(),
                  tempDoc.ErrorRow(),tempDoc.ErrorCol());
			return false;
		}
	}
	
    TiXmlElement* level = tempDoc.RootElement();
    if(!level)
        return false;
    
    TiXmlElement* p_haracter = level->FirstChildElement("character");
    while(p_haracter)
    {
        CharacterInfo pCharacter ;
        
        p_haracter->Attribute("index", &(pCharacter.index));
        p_haracter->Attribute("id", &(pCharacter.Id));
        const char* x = p_haracter->Attribute("posx");
        const char* y = p_haracter->Attribute("posy");
        pCharacter.position = ccp(atof(x),atof(y));
        
        p_haracter = p_haracter->NextSiblingElement("character");
        
        levelmap->insert(std::make_pair(pCharacter.index, pCharacter));
        
    }
    
    return true;
    
}

bool ResManager::loadCharacterTemplate(const char* file)
{
    TiXmlDocument tempDoc;
	//Documents-----Resources
	std::string DocumentsPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    DocumentsPath.append("/");
    DocumentsPath.append(file);
	std::string ResourcesPath = CCAPI::share()->getResourcePath();
	ResourcesPath.append("/");
    ResourcesPath.append(file);
    
	if(!tempDoc.LoadFile(DocumentsPath.c_str()))
	{
		if(!tempDoc.LoadFile(ResourcesPath.c_str()))
		{
			CCLog("XML Parse Failed.Erro=%s,(row=%d, col=%d).",tempDoc.ErrorDesc(),
                  tempDoc.ErrorRow(),tempDoc.ErrorCol());
			return false;
		}
	}
	
    TiXmlElement* pCharacterTemplates = tempDoc.RootElement();
    if(!pCharacterTemplates)
        return false;
	//fixtures
	TiXmlElement* p_haracter = pCharacterTemplates->FirstChildElement("character");
    while(p_haracter)
    {
        CharacterTemplateInfo* pCharacter = new CharacterTemplateInfo();
        p_haracter->Attribute("id", &(pCharacter->nTabId));
        
        const char* width = p_haracter->Attribute("width");
        const char* height = p_haracter->Attribute("height");
        if(width)
            pCharacter->contentSize.width =  atof(width);
        if(height)
            pCharacter->contentSize.height =  atof(height);
        int type =0;
        p_haracter->Attribute("type",&type);
        pCharacter->eType = (CharacterType)type;
        
        p_haracter->Attribute("init_buffid", &(pCharacter->nInitBuffId));
        
        p_haracter->Attribute("to_begin_buffid", &(pCharacter->nToBeginBuffId));
        p_haracter->Attribute("to_end_buffid", &(pCharacter->nToEndBuffId));
        
        p_haracter->Attribute("self_begin_buffid", &(pCharacter->nSelfBeginBuffId));
        p_haracter->Attribute("self_end_buffid", &(pCharacter->nSelfEndBuffId));
        
        p_haracter->Attribute("self_click_buffid", &(pCharacter->nClickApplySelfBuffId));
        p_haracter->Attribute("to_click_buffid", &(pCharacter->nClickApplyToMeBuffId));
        
        
        TiXmlElement* p_bodys = p_haracter->FirstChildElement("bodys");
        if(p_bodys)
        {
            TiXmlElement* p_body = p_bodys->FirstChildElement("body");
            while(p_body)
            {
                BodyInfo body;
                p_body->Attribute("index", &body.index);
                p_body->Attribute("id", &body.Id);
                int type=0;
                p_body->Attribute("body_type", &type);
                
                body.bodyType = (b2BodyType)type;
                
                const char* x = p_body->Attribute("posx");
                const char* y = p_body->Attribute("posy");
                const char* rotate = p_body->Attribute("rotate");
                if(x && y )
                {
                    body.bodyPosition = ccp( atof(x), atof(y));
                    
                }
                
                if(rotate)
                    body.rotate = atof(rotate);
                pCharacter->vBodyArray.push_back(body);
                p_body = p_body->NextSiblingElement("body");
            }
        }
        
        TiXmlElement* p_joints = p_haracter->FirstChildElement("joints");
        if(p_joints)
        {
            TiXmlElement* p_joint = p_joints->FirstChildElement("joint");
            while(p_joint)
            {
                JointInfo joint;
                p_joint->Attribute("id", &joint.Id);
                
                p_joint->Attribute("bodya_index", &joint.bodyAIndex);
                
                p_joint->Attribute("bodyb_index", &joint.bodyBIndex);
                
                // bodya_posx="0" bodya_posy="0" bodyb_posx="0" bodyb_posy="0"
                
                const char* bodyaPosx = p_joint->Attribute("bodya_posx");
                const char* bodyaPosy = p_joint->Attribute("bodya_posy");
                
                if(bodyaPosx && bodyaPosy)
                {
                    joint.bodyAPos = ccp(atof(bodyaPosx),atof(bodyaPosy));
                }
                
                const char* bodybPosx = p_joint->Attribute("bodyb_posx");
                const char* bodybPosy = p_joint->Attribute("bodyb_posy");
                if(bodybPosx && bodybPosy)
                {
                    joint.bodyAPos = ccp(atof(bodybPosx),atof(bodybPosy));
                }
                
                pCharacter->vJointArray.push_back(joint);
                p_joint = p_joint->NextSiblingElement("joint");
            }
        }
        
        m_characterTemplateMap.insert(std::make_pair(pCharacter->nTabId,pCharacter));
        p_haracter = p_haracter->NextSiblingElement("character");
    }
    return true;
    
}

CharacterTemplateInfo* ResManager::getCharacterTemplateInfo(int Id)
{
    std::map<int, CharacterTemplateInfo*>:: iterator it = m_characterTemplateMap.find(Id);
    if(it != m_characterTemplateMap.end())
    {
        return it->second;
    }
    return NULL;
}

bool ResManager::loadBuffTemplate(const char* pBuffTemplateFileName)
{
	TiXmlDocument tempDoc;
	std::string DocumentsPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    DocumentsPath.append("/");
    DocumentsPath.append(pBuffTemplateFileName);
	std::string ResourcesPath = CCAPI::share()->getResourcePath();
	ResourcesPath.append("/");
    ResourcesPath.append(pBuffTemplateFileName);
    
	if(!tempDoc.LoadFile(DocumentsPath.c_str()))
	{
		if(!tempDoc.LoadFile(ResourcesPath.c_str()))
		{
			CCLog("XML Parse Failed.Erro=%s,(row=%d, col=%d).",tempDoc.ErrorDesc(),
                  tempDoc.ErrorRow(),tempDoc.ErrorCol());
			return false;
		}
	}
	
	clearBuffTemplate();
	
    TiXmlElement* pBufftemplates = tempDoc.RootElement();
    if(!pBufftemplates)
        return false;
    TiXmlElement* pBuff = pBufftemplates->FirstChildElement("buffer");
    while (pBuff)
    {
		BuffTemplateInfo bTI;
        pBuff->Attribute("id",&(bTI.Id));
        int buffer_type = 0;
        pBuff->Attribute("buffer_type", &buffer_type);
        bTI.eBufferType = (BufferType)buffer_type;
        
        TiXmlElement* parm = pBuff->FirstChildElement("parm");
        while (parm)
        {
            const char* szParm = parm->Attribute("value");
            if(szParm)
            {
                bTI.vParm.push_back(szParm);
            }
            parm = parm->NextSiblingElement("parm");
        }
        m_mapBuffTemplateMap.insert(std::make_pair(bTI.Id, bTI));
        pBuff = pBuff->NextSiblingElement("buffer");
    }
    return true;
}

BuffTemplateInfo* ResManager::getBuffTemplate(int buffId)
{
	std::map<int,BuffTemplateInfo>::iterator itBuff = m_mapBuffTemplateMap.find(buffId);
	if (itBuff != m_mapBuffTemplateMap.end())
	{
		return &itBuff->second;
	}
	return NULL;
}

void ResManager::clearBuffTemplate()
{
	m_mapBuffTemplateMap.clear();
}
const std::map<int,BuffTemplateInfo>* ResManager::getAllBuffTemplate()
{
	return &m_mapBuffTemplateMap;
}

bool ResManager::loadBodyFixtureTemplate(const char* file)
{
	TiXmlDocument tempDoc;
	std::string DocumentsPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    DocumentsPath.append("/");
    DocumentsPath.append(file);
	std::string ResourcesPath = CCAPI::share()->getResourcePath();
	ResourcesPath.append("/");
    ResourcesPath.append(file);
    
	if(!tempDoc.LoadFile(DocumentsPath.c_str()))
	{
		if(!tempDoc.LoadFile(ResourcesPath.c_str()))
		{
			CCLog("XML Parse Failed.Erro=%s,(row=%d, col=%d).",tempDoc.ErrorDesc(),
                  tempDoc.ErrorRow(),tempDoc.ErrorCol());
			return false;
		}
	}
	
    TiXmlElement* pBodytemplates = tempDoc.RootElement();
    if(!pBodytemplates)
        return false;
	//fixtures
	TiXmlElement* pFixture = pBodytemplates->FirstChildElement("fixture");
    while(pFixture)
    {
        FixtureTemplateInfo* pFTI = new FixtureTemplateInfo();
        pFixture->Attribute("id", &(pFTI->fixtureid));
        int shape =0;
        pFixture->Attribute("fixture_shape", &shape);
        pFTI->shape = (FixtureShapeEnum)shape;
        
        const char* friction = pFixture->Attribute("friction");
        if(friction)
        {
            pFTI->friction = atof(friction);
        }
        
        const char* restitution = pFixture->Attribute("restitution");
        if(restitution)
        {
            pFTI->restitution = atof(restitution);
        }
        
        const char* density = pFixture->Attribute("density");
        if(density)
        {
            pFTI->density = atof(density);
        }
        pFixture->Attribute("is_sensor", &(pFTI->isSensor));
        
        const char* width = pFixture->Attribute("width");
        if(width)
        {
            pFTI->width = atof(width);
        }
        
        const char* height = pFixture->Attribute("height");
        if(height)
        {
            pFTI->height = atof(height);
        }
        
        const char* radius = pFixture->Attribute("radius");
        if(radius)
        {
            pFTI->radius = atof(radius);
        }
        
        TiXmlElement* pPt = pFixture->FirstChildElement("pt");
        while(pPt)
        {
            const char* x = pPt->Attribute("x");
            const char* y = pPt->Attribute("y");
            b2Vec2 vec2(atof(x)/PTM_RATIO, atof(y)/PTM_RATIO);
            pFTI->addPt(vec2);
            pPt = pPt->NextSiblingElement("pt");
        }
        m_FixtureTemplatesMap.insert(std::make_pair(pFTI->fixtureid,pFTI));
        pFixture = pFixture->NextSiblingElement("fixture");
    }
    return true;
}

FixtureTemplateInfo* ResManager::getFixture(int fixtureId)
{
    std::map<int, FixtureTemplateInfo*>::iterator it = m_FixtureTemplatesMap.find(fixtureId);
    if(it !=  m_FixtureTemplatesMap.end())
    {
        return it->second;
    }
    return NULL;
}

bool ResManager::loadJointTemplate(const char* file)
{
    TiXmlDocument tempDoc;
	std::string DocumentsPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    DocumentsPath.append("/");
    DocumentsPath.append(file);
	std::string ResourcesPath = CCAPI::share()->getResourcePath();
	ResourcesPath.append("/");
    ResourcesPath.append(file);
    
	if(!tempDoc.LoadFile(DocumentsPath.c_str()))
	{
		if(!tempDoc.LoadFile(ResourcesPath.c_str()))
		{
			CCLog("XML Parse Failed.Erro=%s,(row=%d, col=%d).",tempDoc.ErrorDesc(),
                  tempDoc.ErrorRow(),tempDoc.ErrorCol());
			return false;
		}
	}
    TiXmlElement* pRoot = tempDoc.RootElement();
    if(!pRoot)
        return false;
	//joint
	TiXmlElement* pJoint = pRoot->FirstChildElement("joint");
    while(pJoint)
    {
        JointTemplateInfo* pJTI = new JointTemplateInfo();
        
        pJoint->Attribute("id", &(pJTI->Id));
        int type = 0;
        pJoint->Attribute("joint_type", &type);
        pJTI->jointType = (b2JointType)type;
        
        TiXmlElement* pParm = pJoint->FirstChildElement("parm");
        while(pParm)
        {
            const char* value = pParm->Attribute("value");
            pJTI->parmArray.push_back(atof(value));
            pParm = pParm->NextSiblingElement("parm");
        }
        m_jointTemplatesMap.insert(std::make_pair(pJTI->Id,pJTI));
        pJoint = pJoint->NextSiblingElement("joint");
    }
    return true;
}

JointTemplateInfo* ResManager::getJointTemplate(int jointId)
{
    std::map<int, JointTemplateInfo*>::iterator it = m_jointTemplatesMap.find(jointId);
    if(it !=  m_jointTemplatesMap.end())
    {
        return it->second;
    }
    return NULL;
}

bool ResManager::loadBodyTemplate(const char* file)
{
    TiXmlDocument tempDoc;
	std::string DocumentsPath = CCFileUtils::sharedFileUtils()->getWritablePath();
    DocumentsPath.append("/");
    DocumentsPath.append(file);
	std::string ResourcesPath = CCAPI::share()->getResourcePath();
	ResourcesPath.append("/");
    ResourcesPath.append(file);
    
	if(!tempDoc.LoadFile(DocumentsPath.c_str()))
	{
		if(!tempDoc.LoadFile(ResourcesPath.c_str()))
		{
			CCLog("XML Parse Failed.Erro=%s,(row=%d, col=%d).",tempDoc.ErrorDesc(),
                  tempDoc.ErrorRow(),tempDoc.ErrorCol());
			return false;
		}
	}
    TiXmlElement* pSpriteTemplates = tempDoc.RootElement();
    if(!pSpriteTemplates)
        return false;
	//fixtures
	TiXmlElement* p_body = pSpriteTemplates->FirstChildElement("body");
    while(p_body)
    {
        BodyTemplateInfo* pBodyTemplate = new BodyTemplateInfo();
        p_body->Attribute("id", &(pBodyTemplate->Id));
        //        int shape =0;
        //        p_body->Attribute("body_shape", &shape);
        //        pBodyTemplate->fixtureShape = (FixtureShapeEnum)shape;
        pBodyTemplate->png = p_body->Attribute("png");
        
        const char* gravity_scale = p_body->Attribute("gravity_scale");
        if(gravity_scale)
        {
            pBodyTemplate->gravityScale = atof(gravity_scale);
        }
        
        TiXmlElement* p_fixtures = p_body->FirstChildElement("fixtures");
        if(p_fixtures)
        {
            TiXmlElement* p_fixture = p_fixtures->FirstChildElement("fixture");
            while(p_fixture)
            {
                int fixtureId = 0;
                p_fixture->Attribute("id", &fixtureId);
                pBodyTemplate->fixtureIdArray.push_back(fixtureId);
                p_fixture = p_fixture->NextSiblingElement("fixture");
            }
        }
        m_bodyTemplateMap.insert(std::make_pair(pBodyTemplate->Id,pBodyTemplate));
        p_body = p_body->NextSiblingElement("body");
    }
    return true;
}

BodyTemplateInfo* ResManager::getBodyTemplateInfo(int Id)
{
    std::map<int, BodyTemplateInfo*>:: iterator it = m_bodyTemplateMap.find(Id);
    if(it != m_bodyTemplateMap.end())
    {
        return it->second;
    }
    return NULL;
}


