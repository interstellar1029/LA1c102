#ifndef __ESP8266_H
#define __ESP8266_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ls1c102.h"

//extern char restart[];
//extern char cwmode[];
//extern char cwlap[];
//extern char cwjap[];
//extern char cifsr[];
//extern char cipmux[];
//extern char cipstart[];
//extern char cipsend[];
//extern char cipserver[];
//extern char cwlif[];
//extern char cipstatus[];
//extern char cipsto[];
//extern char cipmode[];
//extern char test[];

char* esp8266_check_cmd(char *str);
char esp8266_send_cmd(char *cmd,char *ack,uint16_t waittime);
void esp8266_start_trans(void);
void esp8266_send_data(char *cmd);

#ifdef __cplusplus
}
#endif

#endif

