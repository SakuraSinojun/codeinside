
#include "Edit.h"

#include <iostream>
using namespace std;

CEdit::CEdit() :
        m_multiline(true)
{
}

CEdit::~CEdit()
{
}

void CEdit::PreCreateWindow(CREATEWINDOWPARAM& param)
{
        param.WndClass.lpszClassName = TEXT("EDIT");
        param.WndClass.lpfnWndProc = NULL;
        param.dwStyle = WS_VISIBLE | ES_LEFT | WS_CHILD;
        if(m_multiline)
                param.dwStyle = param.dwStyle | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL; 

}

LRESULT CEdit::GetTextLength(void)
{
        return SendMessage(WM_GETTEXTLENGTH, 0, 0);
}

LRESULT CEdit::GetText(LPTSTR buf, int len)
{
        return SendMessage(WM_GETTEXT, len, (LPARAM)buf);
}

void CEdit::SetText(LPCTSTR text)
{
        SendMessage(WM_SETTEXT, 0, (LPARAM)text);
}

void CEdit::SetMultiLine(bool multi)
{
        m_multiline = multi;
}















