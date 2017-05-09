//============================================================================
// Name        : Ground_transceiver.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <string.h>

#include "shm/redis.h"
#include "structs.h"

#include <stdio.h>   /* Стандартные объявления ввода/вывода */
#include <string.h>  /* Объявления строковых функций */
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>

using namespace std;

int checkDataParsing(char* devName, redisDataService* dserv);

int configurePort(int fd) {
	struct termios options; /*структура для установки порта*/
	tcgetattr(fd, &options); /*читает пораметры порта*/

	cfsetispeed(&options, B115200); /*установка скорости порта*/
	cfsetospeed(&options, B115200); /*установка скорости порта*/

	tcsetattr(fd, TCSANOW, &options); /*сохронения параметров порта*/

	return 1;
}

void data_parse(uint8_t* pack_data, int pack_size, resp_s* resp) {
	resp->type = *(uint8_t* )pack_data;
	resp->sec  = *(int32_t*)(pack_data + 2);
	resp->ms   = *(uint16_t*)(pack_data + 7);
	resp->data =  (char*)(pack_data + 9);
	resp->dataSize = pack_size - 9;
}

void data_set(resp_s* resp, uint8_t* pack_data, int* pack_size) {
	pack_data[0] = resp->type;
	pack_data[1] = ':';

	*( (int*)(pack_data+2) ) = resp->sec;

	pack_data[6] = ':';

	*((uint16_t*)(pack_data+7)) = resp->ms;

	resp->data = (char*)(pack_data+9);

	*pack_size = 9 + resp->dataSize;
}

int main(int argc, char* argv[]) {
	char* devName;
	if (argc > 1) {
		devName = argv[1];
	} else {
		printf("Need device name as input parameter\n");
	}

	redisDataService* dserv = new redisDataService();
	dserv->connect();

	int transceiver = open(devName, O_RDWR | O_NOCTTY);
	if (transceiver == -1) {
		perror("open file");
		return 0;
	}

	configurePort(transceiver);

	int count = 0;
	char data[255];
	memset(data, '\0', sizeof(data));

	int pack_count = 0;

	resp_s resp;
	while ( 1 ) {
		int i = 0;
		while (i < 66) {
			int c = read(transceiver, data+i, 1);
			if ( c == 1) {
				i++;
				continue;
			}

			if ( c == -1) {
				perror("read transceiver");
				exit(0);
			}
		}

		//if (count == 0) continue;

		count = 64;

		data_parse((uint8_t*)data, count, &resp);

		req_s req;
		req.type = resp.type;
		req.sec  = resp.sec;
		req.ms   = resp.ms;

		string str(resp.data, resp.dataSize);
		dserv->add(&req, &str);

		printf("type=%d, sec=%d\n", count, req.type, req.sec);
	}

	return 0;
}

int checkDataParsing(char* devName, redisDataService* dserv) {
	char buff[10];
	memset(buff, 's', sizeof(buff));

	resp_s resp;
	resp.type = 5;
	resp.sec  = 123231;
	resp.ms   = 0xFFFF;
	resp.data = buff;
	resp.dataSize = sizeof(buff);

	uint8_t data_ex[100];
	int data_ex_size;
	data_set(&resp, data_ex, &data_ex_size);

	resp_s resp_out;
	data_parse(data_ex, data_ex_size, &resp_out);

	if (resp.type == resp_out.type
	 && resp.sec  == resp_out.sec
	 && resp.ms   == resp_out.ms
	 && resp.dataSize == resp_out.dataSize
	 && !memcmp(resp.data, resp_out.data, resp.dataSize)) {
		printf("Data parse true\n");
	} else {
		printf("Data parse false\n");
		return 0;
	}

	FILE* f = fopen(devName, "w");
	if (f == NULL) {
		perror("open file");
		return 0;
	}

	if (data_ex_size != fwrite(data_ex, 1, data_ex_size, f) ) {
		perror("write file");
		return 0;
	}

	fclose(f);

	f = fopen(devName, "r");
	if (f == NULL) {
		perror("open file");
		return 0;
	}

	if ( data_ex_size != fread(data_ex, 1, data_ex_size, f) ) {
		return 0;
	}

	data_parse((uint8_t*)data_ex, data_ex_size, &resp_out);

	req_s req;
	req.type = resp.type;
	req.sec  = resp.sec;
	req.ms   = resp.ms;

	string str(resp.data, resp.dataSize);
	dserv->add(&req, &str);

	return 1;
}
