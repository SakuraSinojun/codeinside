
#pragma once

#include <list>
#include "type.h"
#include "tools/lock.h"

class CSurface;
class CAnimation;

class CRender
{
public:
        ~CRender();
        CRender(int width, int height);

        CSurface *      CreateSurface(void);
        CAnimation *    CreateAnimation(void);

        void            Attach(CSurface * surface);
        void            Detach(CSurface * surface);
        void            Attach(CAnimation *     ani);
        void            Detach(CAnimation *     ani);
        const char *    RenderScene(BMPHEADER & bmpHeader, BMPINFOHEADER & biHeader);

        void            SwapSurface(CSurface * surface1, CSurface * surface2);
        void            PushSurfaceToBottom(CSurface * surface);
        void            BringSurfaceToTop(CSurface * surface);
        
        int             Width(void){return nWidth;}
        int             Height(void){return nHeight;}

        void            Lock(void);
        void            Unlock(void);

private:
        CRender();
        int                     nWidth, nHeight;
        CSurface *              mainSurface;
        std::list<CSurface *>   lsSurface;
        std::list<CAnimation *> lsAnimation;

        static CRender *        m_instance;
        tools::CLock            m_lock;

};



