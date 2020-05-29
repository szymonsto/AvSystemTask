/**
 * Authon: Szymon Stolarski 2020-05-26
 */
#define _XOPEN_SOURCE 700
//#include<stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/statvfs.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <stdlib.h>
#include <string.h>

#include"avst_utils.h"
#define BUFFER_SIZE 128


int avst_get_free_memory(int32_t* memory_free){
    struct statvfs buf;

    if (statvfs(".", &buf) == -1){
        perror("statvfs() error");
    }
    else{
        //check if memory free pointer is not NULL
        assert(memory_free); 

        // / 1024 in order to return result in kB
        *memory_free = ((int32_t)buf.f_bavail * buf.f_frsize / 1024);

        return 0;
    }
    return -1;    
}

int avst_get_total_memory(int32_t* memory_total){
    struct statvfs buf;

    if (statvfs(".", &buf) == -1){
        perror("statvfs() error");
    }
    else{
        //check if memory total pointer is not NULL
        assert(memory_total); 

        // / 1024 in order to return result in kB
        *memory_total = ((int32_t)buf.f_blocks * buf.f_frsize / 1024);

        return 0;
    }
    return -1;    
}



int avst_get_cpu_temperature(float* cpu_temp){
    float millideg;
	FILE *thermal;
	int n;

	thermal = fopen("/sys/class/thermal/thermal_zone2/temp","r");
	n = fscanf(thermal,"%f",&millideg);
	fclose(thermal);
	*cpu_temp = millideg / 1000;
    return 0;
}

int32_t avst_reboot_device(void){ 
    sync();
    reboot(RB_AUTOBOOT);
    return 0;
}

int avst_get_internet_ip_addr(char* ip_addr){

    char *cmd = "ifconfig", *p1, *p2;      
    char buf[BUFFER_SIZE];
    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        return -1;
    }

    while (fgets(buf, BUFFER_SIZE, fp) != NULL) {
        
        p1 = strstr(buf, "inet");
        
        if(p1 != NULL && strcmp(p1, "127.0.0.1")){ 
        	p1 += strlen("inet ");
        	p2 = strstr(p1, "  netmask");
        	size_t len = p2 - p1;
        	char *res = (char*)malloc(sizeof(char)*(len+1));
    		strncpy(res, p1, len);
    		res[len] = '\0';
            strcpy(ip_addr, res);
    		free(res);
        	return 0;
        }
       
    }

    if(pclose(fp))  {
        return -1;
    }

    return 0;
}

