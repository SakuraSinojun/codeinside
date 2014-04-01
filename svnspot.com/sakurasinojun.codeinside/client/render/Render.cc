
#include "Render.h"
#include "Surface.h"
#include "Animation.h"
#include <algorithm>
#include <cstring>
#include <assert.h>
#include "log.h"
using namespace std;
DECLARE_MODULE(render);

CRender::CRender(int width, int height):
        nWidth(width),
        nHeight(height)
{
        this->mainSurface = new CSurface(nWidth, nHeight);
        this->mainSurface->CreateEmptyBitmap();
}

CRender::CRender():
        nWidth(640),
        nHeight(480)
{
        this->mainSurface = new CSurface(nWidth, nHeight);
        this->mainSurface->CreateEmptyBitmap();
}

CRender::~CRender()
{
        list<CSurface *>::iterator      iter;
        CSurface        *               surface;
        for(iter = lsSurface.begin(); iter != lsSurface.end(); iter++)
        {
                surface = *iter;
                delete surface;
        }
        lsSurface.clear();
        delete mainSurface;
}

CSurface * CRender::CreateSurface(void)
{
        CSurface *      surface = new CSurface(nWidth, nHeight);
        surface->AttachTo(this);
        return surface;
}

CAnimation * CRender::CreateAnimation(void)
{
        CAnimation *    ani = new CAnimation();
        Attach(ani);
        return ani;
}
        
void    CRender::Attach(CSurface * surface)
{
        assert(surface != NULL);
        if(surface == NULL)
                return;
        lsSurface.push_back(surface);
}

void    CRender::Detach(CSurface * surface)
{
        list<CSurface *>::iterator      iter;
        iter = find(lsSurface.begin(), lsSurface.end(), surface);
        if(iter == lsSurface.end())
        {
                Debug() << "No Such Surface!";
                //throw("No Such Surface!");
                return;
        }
        lsSurface.erase(iter);
}

void    CRender::Attach(CAnimation *     ani)
{
        assert(ani != NULL);
        if(ani == NULL)
                return;
        lsAnimation.push_back(ani);
}

void    CRender::Detach(CAnimation *     ani)
{
        list<CAnimation *>::iterator      iter;
        iter = find(lsAnimation.begin(), lsAnimation.end(), ani);
        if(iter == lsAnimation.end())
        {
                Debug() << "No Such Animation!";
                //throw("No Such Surface!");
                return;
        }
        lsAnimation.erase(iter);
}

const char *    CRender::RenderScene(BMPHEADER & bmpHeader, BMPINFOHEADER & biHeader)
{
        Lock();
        list<CSurface *>::iterator      iter;
        CSurface *                      surface;

        list<CAnimation *>::iterator    ita;
        CAnimation *                    ani;
        for(ita = lsAnimation.begin(); ita != lsAnimation.end(); ita++)
        {
                ani = *ita;
                ani->RenderScene();
        }

        mainSurface->Clear();
        for(iter = lsSurface.begin(); iter != lsSurface.end(); iter++)
        {
                surface = *iter;
                mainSurface->Blt(surface);
        }
        memcpy(&bmpHeader, &mainSurface->bmpHeader, sizeof(BMPHEADER));
        memcpy(&biHeader, &mainSurface->biHeader, sizeof(BMPINFOHEADER));

        Unlock();
        return (const char *)mainSurface->imgbuffer;
}

void CRender::SwapSurface(CSurface * surface1, CSurface * surface2)
{
        if(surface1 == surface2)
                return;
        if(surface1 == NULL || surface2 == NULL)
                return;
        list<CSurface *>::iterator      iter1, iter2;
        iter1 = find(lsSurface.begin(), lsSurface.end(), surface1);
        iter2 = find(lsSurface.begin(), lsSurface.end(), surface2);
        if(iter1 == lsSurface.end() || iter2 == lsSurface.end())
                return;
        swap(*iter1, *iter2);
}

void CRender::PushSurfaceToBottom(CSurface * surface)
{
        if(surface == NULL)
                return;
        list<CSurface *>::iterator      iter;
        iter = find(lsSurface.begin(), lsSurface.end(), surface);
        if(iter == lsSurface.end())
                return;
        lsSurface.erase(iter);
        lsSurface.push_front(surface);
}

void CRender::BringSurfaceToTop(CSurface * surface)
{
        if(surface == NULL)
                return;
        list<CSurface *>::iterator      iter;
        iter = find(lsSurface.begin(), lsSurface.end(), surface);
        if(iter == lsSurface.end())
                return;
        lsSurface.erase(iter);
        lsSurface.push_back(surface);
}

void CRender::Lock(void)
{
        m_lock.Lock();
}

void CRender::Unlock(void)
{
        m_lock.Unlock();
}











