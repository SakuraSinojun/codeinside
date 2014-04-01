


#include "Button.h"

#include <iostream>
using namespace std;

CButton::CButton()
{
}

CButton::~CButton()
{
}

void CButton::PreCreateWindow(CREATEWINDOWPARAM& param)
{
        param.WndClass.lpszClassName = TEXT("BUTTON");
        param.WndClass.lpfnWndProc = NULL;
        param.dwStyle = BS_CENTER | BS_PUSHBUTTON | WS_TABSTOP | WS_CHILD;
}

void CButton::OnClick()
{
}

void CButton::OnCommand(WPARAM wParam, LPARAM lParam)
{
}


