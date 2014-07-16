
#ifndef __SELECTLEVEL_LAYER_H__
#define __SELECTLEVEL_LAYER_H__
#include "CC3DListLayer.h"
#include "cocos2d.h"
USING_NS_CC;

class SelectLevelLayer : public CCLayer ,public CC3DListLayerDelegate
{
public:
    virtual ~SelectLevelLayer();
    SelectLevelLayer();
    virtual bool init();
    CREATE_FUNC(SelectLevelLayer);
    
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    
    virtual void  onCC3DListLayerSelectIndex(int nIndex);
    virtual void  onCC3DListLayerClickSelect(int nIndex);
    
protected:
    void clickBack();
private:
};

#endif // __SELECTLEVEL_LAYER_H__
