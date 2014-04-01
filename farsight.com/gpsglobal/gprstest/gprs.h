#ifndef GPRS
#define GPRS

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DEVICE "/dev/ttyUSB0"
#define FIRE 0
#define THIEF 1
#define BUF_SIZE 1024


int send_eng_mes(char *phone, int alarm_type);
int gprs_func(char *phone_num, int alarm_type);
int gprs(int alarm_type,char *phone_num);

#endif
