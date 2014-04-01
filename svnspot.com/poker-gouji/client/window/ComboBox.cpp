
#include "ComboBox.h"
#include <commctrl.h>
#include <iostream>
using namespace std;

CComboBox::CComboBox()
{
}

CComboBox::~CComboBox()
{
}

void CComboBox::PreCreateWindow(CREATEWINDOWPARAM& param)
{
        param.WndClass.lpszClassName = WC_COMBOBOX;//TEXT("COMBOBOX");
        param.WndClass.lpfnWndProc = NULL;
        param.dwStyle = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | CBS_HASSTRINGS;

}







