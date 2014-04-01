
#include "View.h"

CView::CView()
{
}

CView::~CView()
{
}

void CView::PreCreateWindow(CREATEWINDOWPARAM& param)
{
        param.dwStyle                   = WS_CHILD | WS_OVERLAPPED;
        param.WndClass.lpszClassName    = TEXT("LINGVIEW");
        param.WndClass.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
}


