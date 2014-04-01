
#include <windows.h>
#include "App.h"
#include <stdio.h>


int main(void)
{
        CApp *   pApp = GetApp();
        if(pApp == NULL)
        {
                printf("APP NULL!\n");
                return -1;
        }
        pApp->loop();
        return 0;
}

int WINAPI WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow
        )
{
        return main();
}

