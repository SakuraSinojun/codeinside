
#include "Static.h"
#include "commctrl.h"

CStatic::CStatic()
{
}

CStatic::~CStatic()
{
}

void CStatic::PreCreateWindow(CREATEWINDOWPARAM& param)
{
        param.dwStyle = WS_VISIBLE | WS_CHILD | SS_CENTER | SS_SIMPLE | SS_ENDELLIPSIS | SS_NOTIFY;
        param.WndClass.lpfnWndProc = NULL;
        param.WndClass.lpszClassName = WC_STATIC;
}



