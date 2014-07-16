

#ifndef __ResManager__
#define __ResManager__

#include "CommonDef.h"
#include "CommonStruct.h"


class ResManager
{
public:
    ResManager();
    virtual ~ResManager();
    static ResManager*                          shareResManager();
    void                                        init();
    bool                                        loadLevelMap(const char* file,std::map<int, CharacterInfo>*  levelmap);
    std::map<int, CharacterTemplateInfo*>*      getCharacterTemplateMap(){return &m_characterTemplateMap;}
    CharacterTemplateInfo*                      getCharacterTemplateInfo(int Id);
    bool                                        loadCharacterTemplate(const char* file);
    
    bool                                        loadBuffTemplate(const char* pBuffTemplateFileName);
	BuffTemplateInfo*                           getBuffTemplate(int buffId);
	const std::map<int,BuffTemplateInfo>*       getAllBuffTemplate();
	void                                        clearBuffTemplate();
    
    FixtureTemplateInfo*                        getFixture(int fixtureId);
    JointTemplateInfo*                          getJointTemplate(int jointId);
    BodyTemplateInfo*                           getBodyTemplateInfo(int Id);

protected:
    
    bool                                        loadBodyFixtureTemplate(const char* pBodyTemplateFileName);
    bool                                        loadJointTemplate(const char* file);
    bool                                        loadBodyTemplate(const char* file);

private:
    std::map<int, CharacterTemplateInfo*>       m_characterTemplateMap;
    std::map<int, BodyTemplateInfo*>            m_bodyTemplateMap;
    std::map<int, BuffTemplateInfo>             m_mapBuffTemplateMap;
    std::map<int, FixtureTemplateInfo*>         m_FixtureTemplatesMap;
    std::map<int, JointTemplateInfo*>           m_jointTemplatesMap;


};

#endif /* defined(__ResManager__) */
