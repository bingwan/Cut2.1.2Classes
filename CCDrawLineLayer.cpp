

#include "CCDrawLineLayer.h"

CCDrawLineLayer::CCDrawLineLayer():
m_fingerIndex(-1),
m_deleteCountOfTick(5),
m_pointListKeepCount(300),
m_lineWidthMax(18),
m_lineWidthMin(1),
m_lineAlphaMin(10),
m_lineAlphaMax(200),
m_percentOfMaxPoint(0.8),
m_isRandColor(false),
m_drawLayerDelegate(NULL)
{}

CCDrawLineLayer::~CCDrawLineLayer()
{}

bool CCDrawLineLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
    setTouchEnabled(true);
    
    return true;
}

void CCDrawLineLayer::draw()
{
    drawLine();
}

void CCDrawLineLayer::drawLine()
{
    
    for (int i=0; i<m_deleteCountOfTick ; i++) 
    {
        if (m_pointList.size() >0)
        {
            m_pointList.pop_front();
        }
        else 
        {
            break;
        }
    }
	while (m_pointList.size() > m_pointListKeepCount) 
    {
		m_pointList.pop_front();
	}
    
    
    if(m_isRandColor)
    {
        int  R = arc4random()%255;
        int  G = arc4random()%255;
        int  B = arc4random()%255;
        
        m_lineColor.r = R;
        m_lineColor.g = G;
        m_lineColor.b = B;
    }
    
    int pointListCount = m_pointList.size();
	std::list <CCPoint>::iterator it = m_pointList.begin();
    int pointIndex = 0;
	for(;it!=m_pointList.end();it++)
	{
        int maxPointIndex =  pointListCount * m_percentOfMaxPoint;
        
        int distanceToMiddle = pointIndex-maxPointIndex;
        
        float countPercent = 0;
        if(distanceToMiddle < 0)
        {
             distanceToMiddle = fabs(distanceToMiddle);
             countPercent = distanceToMiddle/(float)maxPointIndex;
        }
        else
        {
             countPercent = distanceToMiddle/(float)(pointListCount - maxPointIndex);
        }
       
        float percent = 1.0 - countPercent;
        float lintWidth = m_lineWidthMin + (m_lineWidthMax-m_lineWidthMin)*percent;
        int alpha = m_lineAlphaMin + (m_lineAlphaMax-m_lineAlphaMin)*percent;
        
        ccDrawColor4B(m_lineColor.r,m_lineColor.g,m_lineColor.b,alpha );
        ccPointSize(lintWidth);
        ccDrawPoint( *it );
        
        pointIndex++;
	}
}
void CCDrawLineLayer::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority,false);  
}

bool CCDrawLineLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    m_deleteCountOfTick = 3;

    
    CCTouch* touch = pTouch;
    CCPoint beginPoint = touch->getLocationInView();
    beginPoint = CCDirector::sharedDirector()->convertToGL(beginPoint);
    m_pointList.push_back(beginPoint);
    
    //CCLog("CCDrawLineLayer::ccTouchesBegan:beginPointX=%f",beginPoint.x);
    
    if(m_drawLayerDelegate)
    {
        m_drawLayerDelegate->ccDrawLineLayerTouchBegin(this,touch);
    }
    
    return true;    
}
void CCDrawLineLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCTouch* touch = pTouch;
	
	CCPoint nextPoint = touch->getLocationInView();
	nextPoint = CCDirector::sharedDirector()->convertToGL(nextPoint);
    
    CCPoint preMovePoint = touch->getPreviousLocationInView();
	preMovePoint = CCDirector::sharedDirector()->convertToGL(preMovePoint);
	
	float distance = ccpDistance(nextPoint, preMovePoint);
	if (distance > 1)
	{
		int d = (int)distance;
		for (int i = 0; i < d; i++ )
		{
			float distanceX = nextPoint.x - preMovePoint.x;
			float distanceY = nextPoint.y - preMovePoint.y;
            
			float percent = i / distance;
			CCPoint newPoint;
			newPoint.x = preMovePoint.x + (distanceX * percent);
			newPoint.y = preMovePoint.y + (distanceY * percent); 
			
			m_pointList.push_back(newPoint);
		}
	}
    
    if(m_drawLayerDelegate)
    {
        m_drawLayerDelegate->ccDrawLineLayerTouchMove(this,touch);
    }   

    
}
void CCDrawLineLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    m_deleteCountOfTick = 30;
    
}
void CCDrawLineLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    m_deleteCountOfTick = 30;
}



