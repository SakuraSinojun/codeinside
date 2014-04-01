


#include "Animation.h"
#include "Surface.h"
#include "Render.h"
#include <algorithm>
using namespace std;

#include "log.h"
DECLARE_MODULE(render);

CAnimation::CAnimation():
        running(false),
        dwFrameInterval(0),
        dwTrackInterval(0)
{
    SCHECK(false) << "Animation is not supported now.";
        iterCurrentSurface = vecSurface.begin();
}

CAnimation::~CAnimation()
{
        vecSurface.clear();
        vecWP.clear();
}

void CAnimation::AddWayPoint(int x, int y)
{
        vecWP.push_back(std::pair<int, int>(x, y));
        this->iterCurrentWayPoint = vecWP.begin();
}

void CAnimation::DelWay(void)
{
        vecWP.clear();
}

bool CAnimation::IsRunning(void)
{
        return this->running;
}

void CAnimation::Start(void)
{
#if 0
        running = true;
        dfStartTime = CTools::Get()->CurrentTime();
        dfLastWPTime = CTools::Get()->CurrentTime();
        PRINTF("Animation started.\n");
#endif
}

void CAnimation::Pause(void)
{
        running = false;
        // PRINTF("Animation paused.\n");
}

// 停止+重置
void CAnimation::Stop(void)
{
        running = false;
        Reset();
}

// 不改变IsRunning状态。
void CAnimation::Reset(void)
{
#if 0
        this->iterCurrentSurface = vecSurface.begin();
        vector<CSurface *>::iterator    iter;
        CSurface        *               sur;
        for(iter = vecSurface.begin(); iter != vecSurface.end(); iter++)
        {
                sur = *iter;
                sur->Show(false);
                sur->ResetPos();
        }
        dfStartTime = CTools::Get()->CurrentTime();
        dfLastWPTime = CTools::Get()->CurrentTime();
        iterCurrentWayPoint = vecWP.begin();
#endif
}

void CAnimation::FrameInterval(unsigned int   dwMilliseconds)
{
        this->dwFrameInterval = dwMilliseconds;
}

unsigned int CAnimation::FrameInterval(void)
{
        return this->dwFrameInterval;
}

void CAnimation::TrackInterval(unsigned int   dwMilliseconds)
{
        this->dwTrackInterval = dwMilliseconds;
}

unsigned int CAnimation::TrackInterval(void)
{
        return this->dwTrackInterval;
}

void CAnimation::AddFrame(CSurface *     surface)
{
        this->vecSurface.push_back(surface);
        surface->Show(false);
        Reset();
}

void CAnimation::DelFrame(CSurface *     surface)
{
        vector<CSurface *>::iterator    iter;
        iter = find(vecSurface.begin(), vecSurface.end(), surface);
        if(iter == vecSurface.end())
                return;

        vecSurface.erase(iter);
        delete surface;
}

void CAnimation::RenderScene(void)
{
#if 0
        // 暂停时
        double  nowtime = CTools::Get()->CurrentTime();
        if(!running)
        {
                return;
        }

        if(vecSurface.empty())
                return;

        double  frametime       = (double)this->dwFrameInterval / 1000.0;
        double  tracktime       = (double)this->dwTrackInterval / 1000.0;

        vector<CSurface *>::iterator    iter;
        if(this->iterCurrentSurface == vecSurface.end())
                iterCurrentSurface = vecSurface.begin();
        iter = this->iterCurrentSurface;
        iter ++;
        if(iter == vecSurface.end())
                iter = vecSurface.begin();

        CSurface *      surface = *iterCurrentSurface;
        if(vecSurface.size() > 1)
        {
                // 下一帧
                if(nowtime - dfStartTime >= frametime)
                {
                        // surface->ResetPos();
                        surface->Show(false);
                        iterCurrentSurface = iter;
                        iter ++;
                        if(iter == vecSurface.end())
                                iter = vecSurface.begin();
                        surface = *iterCurrentSurface;
                        dfStartTime = nowtime;
                }
                surface->Show(true);
        }

        // 路径点。
        vector<pair<int, int> >::iterator       itwp;
        if(this->iterCurrentWayPoint == vecWP.end())
                iterCurrentWayPoint = vecWP.begin();
        itwp = iterCurrentWayPoint;
        itwp ++;
        if(itwp == vecWP.end())
                itwp = vecWP.begin();

        if(vecWP.size() > 1)
        {
                if(nowtime - dfLastWPTime >= tracktime)
                {
                        iterCurrentWayPoint = itwp;
                        itwp ++;
                        if(itwp == vecWP.end())
                                itwp = vecWP.begin();
                        dfLastWPTime = nowtime;
                }
                int             cx, cy;
                cx = static_cast<int>(iterCurrentWayPoint->first + (itwp->first - iterCurrentWayPoint->first) * (nowtime - dfLastWPTime) / tracktime);
                cy = static_cast<int>(iterCurrentWayPoint->second + (itwp->second - iterCurrentWayPoint->second) * (nowtime - dfLastWPTime) / tracktime);
                surface->SetDrawPos(cx, cy);
        }else{
                surface->ResetPos();
        }
        return;
#endif
}

CSurface *      CAnimation::Surface(void)
{
        if(vecSurface.empty())
                return NULL;
        if(iterCurrentSurface == vecSurface.end())
                iterCurrentSurface = vecSurface.begin();
        return *iterCurrentSurface;
}


