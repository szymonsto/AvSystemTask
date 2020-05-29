/// @file avst_utils.h

//AVSystem task utilities

#ifndef AVST_UTILS_H
#define AVST_UTILS_H

#include <stdint.h>

/**
 * Reads amount of free disc memory and assigns this value to given argument
 *
 * @param[in] memory_free  pointer to integer
 * @param[out] returns 0 on success, -1 on faliure
 */
int avst_get_free_memory(int32_t* memory_free);

/**
 * Reads amount of total disc memory and assigns this value to given argument
 *
 * @param[in] memory_total  pointer to integer
 * @param[out] returns 0 on success, -1 on faliure
 */
int avst_get_total_memory(int32_t* memory_total);

/**
 * Reads CPU temperature and assigns this value to given argument
 *
 * @param[in] cpu_temp  pointer to floating point number
 * @param[out] returns 0 on success, -1 on faliure
 */
int avst_get_cpu_temperature(float* cpu_temp);

/**
 * Reads outgoing IP address and assigns this value to given argument
 * demands net-tools installed (sudo apt install net-tools)
 *
 * @param[in] ip_addr  string
 * @param[out] returns 0 on success, -1 on faliure
 */
int avst_get_internet_ip_addr(char* ip_addr);

/**
 * Reboots device
 * demands running application as superuser (sudo program)
 *
 * @param[out] returns 0 on success, -1 on faliure
 */
int avst_reboot_device(void);
#endif