
#define __USE_MAIN__
#ifdef __USE_MAIN__

#include "debug.h"
#include "Tetris.h"
#include <windows.h>
#include <TCHAR.h>
#include <time.h>

BOOL file_exists(LPCTSTR file)
{	
        WIN32_FIND_DATA	fd;
        HANDLE		handle;	
                
        handle = FindFirstFile(file, &fd);
        FindClose(handle);
        
        if(handle == INVALID_HANDLE_VALUE)
        {
                return FALSE;
        }
        
        return TRUE;

}

bool parse_input(void)
{
        SHORT   key;
        bool	flag;
		static clock_t  t1 = 0;

		if(clock() - t1 < 140)
	{
			return false;
	}
	t1 = clock();

		flag = false;
        key = GetAsyncKeyState(VK_UP);
        if(key < 0)
        {
                tetris_rotate(true);
                if(file_exists(_T("rotate.wav")))
                {
                        PlaySound(_T("rotate.wav"),NULL,SND_ALIAS|SND_ASYNC);
                }
				flag = true;
        }

        key = GetAsyncKeyState(VK_DOWN);
        if(key < 0)
        {
                tetris_move_down();
				flag = true;
        }

        key = GetAsyncKeyState(VK_LEFT);
        if(key < 0)
        {
                tetris_move_left();
				flag = true;
        }

        key = GetAsyncKeyState(VK_RIGHT);
        if(key < 0)
        {
                tetris_move_right();
				flag = true;
        }
        key = GetAsyncKeyState(VK_SPACE);
        if(key < 0)
        {
                tetris_instant_drop();
                if(file_exists(_T("oned.wav")))
                {
                        PlaySound(_T("oned.wav"),NULL,SND_ALIAS|SND_ASYNC);
                }
				flag = true;
        }
        
        return flag;
        
}
void gotoxy(int x,int y)
{
        COORD C;
        C.X = x;
        C.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), C);
}

int main()
{
        BOXTYPE board[20][12];
        BOXTYPE nextbox[4][4];
        int     i, j;
        int     count;
        int     goal;
        time_t	t1;
		bool	bChanged;
		bool	bLooped;

        tetris_init(12, 20);
        
        goal = 0;
		t1 = 0;
        system("cls");
		bLooped = false;
        while(true)
        {
                bChanged = parse_input();

                tetris_get_board((BOXTYPE*)board);
				if(bChanged || bLooped)
				{
						gotoxy(0, 0);
						for(j=0; j<20; j++)
						{
								for(i=0; i<12; i++)
								{
										if(board[j][i] != 0)
										{
												printf("■");
										}else{
												printf("　");
										}
								}
								printf("\n");
						}
						
						gotoxy(30, 0);
						printf("下一个方块：");
						for(j=0; j<4; j++)
						{
								gotoxy(30, j+2);
								for(i=0; i<4; i++)
								{
										if(nextbox[j][i] != 0)
										{
												printf("■");
										}else{
												printf("　");
										}
								}
						}
						
						gotoxy(0, 21);
						printf("总行数：%d", goal);
						
						tetris_get_next_box(nextbox); 
						

						bChanged = false;
						bLooped = false;
				}
				
				if(time(NULL) - t1 < 1)
                {
                        continue;
                }

				t1 = time(NULL);

                count = tetris_game_loop();
                if(count < -1)
                {
                        break;
                }
                if(count == -1)
                {
                        if(file_exists(_T("oned.wav")))
                        {
                                PlaySound(_T("oned.wav"),NULL,SND_ALIAS|SND_ASYNC);
                        }
                }else{
                        goal += count;
                }
                bLooped = true;
        }
        
        tetris_quit();
        return 0;
}

#endif


