#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "gprs.h"

char *message2 = "67094EBA95EF5165"; //内容“有人闯入”
char *message3 = "53D1751F706B8B66"; //内容“发生火警”

int fd;

int send_eng_mes(char *phone, int alarm_type)
{
    int n_read;
	char *message ;
	char AT[] = "AT\r\n";
	char CSMP[]= "AT+CSMP=17,167,0,8\r\n";        //设置短消息文本模式参数
	char CMGF[] = "AT+CMGF=1\r\n";                  //选择短消息信息格式：0-PDU;1-文本
	char CMGS[] = "AT+CMGS=";         //发送短消息
	char END[] = "\n";
	char end_mes[] = {0x1A, 0x0D, 0x0A};
	char read_buf[BUF_SIZE];
	
	memset(read_buf, 0, BUF_SIZE);
	
    printf("%s\n",phone);
    write(fd, "AT\r\n", 3);
    printf("after write!\n");
    sleep(1);
    n_read = read(fd, read_buf, sizeof(read_buf));
    printf("n_read= %d is %s \n", n_read, read_buf);
	write(fd, CSMP, strlen(CSMP));
	sleep(1);
	write(fd, CMGF, strlen(CMGF));
	sleep(1);
	write(fd, CMGS, strlen(CMGS));
	sleep(1);
	write(fd, phone, strlen(phone));
	sleep(1);
	
	write(fd, END, strlen(END));
	sleep(2);
	if(alarm_type == 0)
		message = message3;
	else if(alarm_type == 1)
		message = message2;
	write(fd, message, strlen(message));
	sleep(2);
	write(fd, end_mes, strlen(end_mes));

	sleep(4);
	printf("send message successful!\n");

	memset(read_buf, 0, BUF_SIZE);
    n_read = read(fd, read_buf, sizeof(read_buf));
    printf("n_read= %d is %s \n", n_read, read_buf);

	return 0;
}

int gprs_func(char *phone_num, int alarm_type)
{
	char *mes;
	struct termios option;
	
	printf("before gprs open\n");
	fd = open(DEVICE, O_RDWR|O_NOCTTY);//|O_NDELAY);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
    printf("after gprs open\n");
	tcgetattr(fd, &option);    //保存原串口的配置
	cfmakeraw(&option);             //将终端设置为原始模式
	cfsetispeed(&option, B115200);          
	cfsetospeed(&option, B115200);
	tcflush(fd, TCIOFLUSH);        //用于清空输入与输出缓冲区
	tcsetattr(fd, TCSANOW, &option);                  //激活刚才的配置并配置生效//

	if(alarm_type == 0)
		mes = message3;
	else if(alarm_type == 1)
		mes = message2;
	else
	{
		printf("wrong alarm type!!!\n");
		return -1;
	}
    printf("before send gprs\n");
	send_eng_mes(phone_num , alarm_type);
    printf("after sentmsg\n");
	return 0;	
}

int gprs(int alarm_type,char *phone_num)
{
	//int alarm_type = 0;
	//char phone_num[] = "18801284768";
	printf("after  define\n");
    gprs_func(phone_num, alarm_type);
    printf("after the main  gprs_func\n");
	return 0;
}

int main()
{
        gprs(0, "13591991768");
        return 0;
}



