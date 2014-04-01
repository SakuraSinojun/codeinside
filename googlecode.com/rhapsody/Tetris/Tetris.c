

#include "Tetris.h"
#include "debug.h"
#include "bool.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>


/******************************************************************************/



static const int        BOX_STRUCT_SIZE = 16 * sizeof(BOXTYPE);
static const int        BOX_COUNT_BASE  = 7;
static const int        BOX_COUNT_TOTAL = 17;
static int              board_width     = 0;
static int              board_height    = 0;
static int              box_count       = 0;
static int              current_x;
static int              current_y;
static bool             newbox;
static bool             first_run;

static BOXTYPE *        tetris_board    = NULL;
static BOXTYPE          box_next[4][4];
static BOXTYPE          box_current[4][4];
static BOXTYPE          box_list[][4][4]=
{
        {       /* 方块 */
                {0, 0, 0, 0},
                {0, 1, 1, 0},
                {0, 1, 1, 0},
                {0, 0, 0, 0}
        },                                        
        {       /* 长条 */
                {0, 0, 0, 0},
                {2, 2, 2, 2},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
        },
        {       /* 土 */
                {0, 3, 0, 0},
                {3, 3, 3, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
        },
        {       /* Z */
                {0, 4, 4, 0},
                {4, 4, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
        },
        {       /* 反Z */
                {0, 0, 0, 0},
                {0, 0, 5, 0},
                {0, 5, 5, 0},
                {0, 5, 0, 0}
        },
        {       /* 7 */
                {0, 0, 0, 0},
                {0, 6, 6, 0},
                {0, 0, 6, 0},
                {0, 0, 6, 0}
        },
        {       /* 反7 */
                {0, 0, 0, 0},
                {0, 7, 0, 0},
                {0, 7, 7, 7},
                {0, 0, 0, 0}
        },
        {       /* 十 */
                {0, 0, 0, 0},
                {0, 8, 0, 0},
                {8, 8, 8, 0},
                {0, 8, 0, 0}
        },
        {       /* 大Z */
                {0, 0, 0, 0},
                {0, 9, 9, 0},
                {0, 0, 9, 0},
                {0, 0, 9, 9}
        },
        {       /* 反大Z */
                {0, 0, 0, 0},
                {0, 0, 10, 10},
                {0, 0, 10, 0},
                {0, 10, 10, 0}
        },
        {       /* 凹 */
                {0, 0, 0, 0},
                {0, 11, 11, 0},
                {0, 0, 11, 0},
                {0, 11, 11, 0}
        },
        {       /* T */
                {0, 0, 0, 0},
                {0, 12, 12, 12},
                {0, 0, 12, 0},
                {0, 0, 12, 0}
        },
        {       /* .. */
                {0, 0, 0, 0},
                {0, 0, 13, 13},
                {0, 13, 13, 0},
                {0, 0, 13, 0}
        },
        {       /* b */
                {0, 0, 0, 0},
                {0, 0, 14, 0},
                {0, 14, 14, 0},
                {0, 14, 14, 0}
        },
        {       /* L */
                {0, 0, 0, 0},
                {0, 0, 15, 0},
                {0, 0, 15, 0},
                {15, 15, 15, 0}
        },
        {       /* .. */
                {0, 0, 0, 0},
                {0, 0, 16, 0},
                {0, 16, 16, 0},
                {0, 0, 16, 16}
        },
        {       /* d */
                {0, 0, 0, 0},
                {0, 17, 0, 0},
                {0, 17, 17, 0},
                {0, 17, 17, 0}
        },
};


/******************************************************************************/

static void _pick_box(___out int box[4][4]);
static int  _check_board(void);

/******************************************************************************/

bool tetris_use_extended_boxes(bool use)
{
        bool    b;
        if(box_count == BOX_COUNT_BASE)
        {
                b = false;
        }else{
                b = true;
        }
        
        if(use)
        {
                box_count = BOX_COUNT_TOTAL;
        }else{
                box_count = BOX_COUNT_BASE;
        }
        
        return b;
        
}

bool tetris_init(int width, int height)
{
        assert(tetris_board == NULL);
        if(tetris_board != NULL)
        {
                return false;
        }
        
        srand((unsigned int)time(NULL));


        tetris_board = (BOXTYPE *)malloc(width * height * sizeof(BOXTYPE));
        if(tetris_board == NULL)
        {
                return false;
        }
        
        memset(tetris_board, 0, width * height * sizeof(BOXTYPE));

        if(box_count == 0)
        {
                box_count = BOX_COUNT_BASE;
        }
        
        board_width     = width;
        board_height    = height;
        
        _pick_box(box_next);
        memset(box_current, 0, BOX_STRUCT_SIZE);
        current_y = -1;
        current_x = 0;
        newbox = true;
        first_run = true;

        return true;
}

bool tetris_quit(void)
{
        assert(tetris_board != NULL);
        
        if(tetris_board == NULL)
        {
                return false;
        }
        
        free(tetris_board);
        tetris_board = NULL;
        return true;
}

bool tetris_get_board(___out BOXTYPE * board)
{
        int     i, j;

        assert(board != NULL);
        
        if(board == NULL)
        {
                return false;
        }
        
        memcpy(board, tetris_board, board_width * board_height * sizeof(BOXTYPE));

        for(j=0; j<4; j++)
        {
                for(i=0; i<4; i++)
                {
                        if(box_current[j][i] != 0)
                        {
                                if( (current_y + j) >= 0 )
                                {
                                        board[ (current_y + j) * board_width 
                                                + current_x + i] =
                                        box_current[j][i];
                                }
                        }
                }
        }

        return true;
}

bool tetris_get_next_box(___out BOXTYPE box[4][4])
{
        memcpy(box, box_next, BOX_STRUCT_SIZE);
        return true;
}

bool tetris_rotate(bool clockwise)
{
        return tetris_rotate_box(box_current, clockwise, true);
}

int tetris_game_loop(void)
{
        int     count;
        count = _check_board();
        if(count < 0)
        {
                return -2;
        }
        
        if(newbox)
        {
                memcpy(box_current, box_next, BOX_STRUCT_SIZE);
                _pick_box(box_next);
                current_x = board_width / 2 - 1;
                current_y = -1;
                newbox = false;
                if(!first_run && count == 0)
                {
                        count = -1;
                }
        }else{
                current_y ++;
        }
        

        first_run = false;
        return count;
        
}

bool tetris_move_left(void)
{
        if(!tetris_check_box(box_current, current_x-1, current_y))
        {
                return false;
        }
        current_x --;
        return true;
}

bool tetris_move_right(void)
{
        if(!tetris_check_box(box_current, current_x+1, current_y))
        {
                return false;
        }
        current_x ++;
        return true;
}

bool tetris_move_down(void)
{
        if(!tetris_check_box(box_current, current_x, current_y+1))
        {
                return false;
        }
        current_y ++;
        return true;
}

int tetris_instant_drop(void)
{
        while(tetris_check_box(box_current, current_x, current_y+1))
        {
                current_y ++;
        }
        return tetris_game_loop();
}

/******************************************************************************/

static void _pick_box(___out BOXTYPE box[4][4])
{
        int     index;
        int     t;
        int     i;
        
        index = rand() % box_count;
        t = rand() % 4;
        if(t==0)
        {
                t = 4;
        }
        
        memcpy(box, box_list[index], BOX_STRUCT_SIZE);
        for(i=0; i<t; i++)
        {
                tetris_rotate_box(box, true, false);
        }
        
}


// 消行、结束的判断。
static int _check_board(void)
{
        int     i;
        int     j;
        int     t;
        int     x;
        int     y;
        int     count;

        if(newbox)
        {
                return 0;
        }

        if(tetris_check_box(box_current, current_x, current_y+1))
        {
                return 0;
        }
        
        if(current_y <= 0)
        {
                //游戏结束 
                return -1;
        }
        
        // 落到地面。 
        for(j=0; j<4; j++)
        {
                for(i=0; i<4; i++)
                {
                        if(box_current[j][i] != 0)
                        {
                                tetris_board[(current_y+j)*board_width+current_x+i] =
                                        box_current[j][i];
                        }
                }
        }


        // 消行
        count = 0;
        for(j=board_height-1; j>0; j--)
        {
                t = 1;
                for(i=0; i<board_width; i++)
                {
                        t = t * tetris_board[j*board_width+i];
                }
                
                if(t != 0)
                {
                        count ++;
                        for(y=j; y>0; y--)
                        {
                                for(x=0; x<board_width; x++)
                                {
                                        tetris_board[y*board_width+x] =
                                        tetris_board[(y-1)*board_width+x];
                                }
                        }
                        j++;
                }
        }
        
        newbox = true;

        return count;

}


/******************************************************************************
 *
 * AI导出函数。
 *
 */

bool tetris_rotate_box(___inout BOXTYPE box[4][4], bool clockwise, bool checkboard)
{

        int     temp[4][4];
        int     t[4][4];
        int     i, j;
        int     minx, miny;
        
        assert(box != NULL);
        if(box == NULL)
        {
                return false;
        }

        if(clockwise)
        {
                minx = 4;
                miny = 4;

                for(j=0; j<4; j++)
                {
                        for(i=0; i<4; i++)
                        {
                                temp[j][i] = box[3-i][j];
                                if(temp[j][i] != 0)
                                {
                                        if(j < miny)
                                        {
                                                miny = j;
                                        }
                                        if(i < minx)
                                        {
                                                minx = i;
                                        }
                                }
                        }
                }
                
                memset(t, 0, BOX_STRUCT_SIZE);
                for(j=miny; j<4; j++)
                {
                        for(i=minx; i<4; i++)
                        {
                                t[j-miny][i-minx] = temp[j][i];
                        }
                }
                
                if(checkboard)
                {
                        if(!tetris_check_box(t, current_x, current_y))
                        {
                                return false;
                        }
                }
                
                memcpy(box, t, BOX_STRUCT_SIZE);
        }else{    
        }
        
        return true;
}

bool tetris_check_box(BOXTYPE box[4][4], int x, int y)
{
        int     width;
        int     height;
        int     i;
        int     j;
        
        if(x < 0)
        {
                return false;
        }
        if(y < 0)
        {
                return false;
        }
        width = 0;
        height = 0;
        
        for(j=0; j<4; j++)
        {
                for(i=0; i<4; i++)
                {
                        if(box[j][i] != 0)
                        {
                                if(j>height)
                                {
                                        height = j;
                                }
                                if(i > width)
                                {
                                        width = i;
                                }
                        }
                }
        }
        if(x + width >= board_width)
        {
                return false;
        }
        if(y + height >= board_height)
        {
                return false;
        }
        for(j=0; j<=height; j++)
        {
                for(i=0; i<=width; i++)
                {
                        if(box[j][i] != 0)
                        {
                                if(tetris_board[(y+j)*board_width+(i+x)] != 0)
                                {
                                        return false;
                                }
                        }
                }
        }
        
        
        return true;
}

/******************************************************************************/













