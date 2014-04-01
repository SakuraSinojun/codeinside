



#pragma once

#ifndef __TETRIS_H__
#define __TETRIS_H__

#include "bool.h"

#ifndef ___in
#define ___in
#define ___out
#define ___inout
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef int     BOXTYPE;

bool tetris_use_extended_boxes(bool use);
bool tetris_init(int width, int height);
bool tetris_quit(void);
bool tetris_get_board(___out BOXTYPE * board);
bool tetris_rotate(bool clockwise);
int tetris_game_loop(void);
bool tetris_move_left(void);
bool tetris_move_right(void);
bool tetris_move_down(void);
int tetris_instant_drop(void);
bool tetris_get_next_box(___out BOXTYPE box[4][4]);

/* AIµ¼³öº¯Êý */
bool tetris_rotate_box(___inout BOXTYPE box[4][4], bool clockwise, bool checkboard);
bool tetris_check_box(BOXTYPE box[4][4], int x, int y);

#ifdef __cplusplus
}
#endif

#endif

