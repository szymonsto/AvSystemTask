/// @file device_object.h

#ifndef DEVICE_OBJECT_H
#define DEVICE_OBJECT_H

#include <anjay/dm.h>

typedef enum {
    NO_ERROR = 0,
    LOW_BATTERY_POWER,
    EXTERNAL_POWER_SUPPLY_OFF,
    GPS_MODULE_FAILURE,
    LOW_RECEIVED_SIGNAL_STRENGTH,
    OUT_OF_MEMORY,
    SMS_FAILURE,
    IP_CONNECTIVITY_FAILURE,
    ERR_PERIPHERAL_MALFUNCTION
} error_code_t;   

/**
 * Creates Device object
 *
 * @param[out] Device object with default vaules of attributes
 */
const anjay_dm_object_def_t **device_object_create(void);

/**
 * Releases Device object
 *
 * @param[in] def  pointer to object
 */ 
void device_object_release(const anjay_dm_object_def_t **def);

/**
 * Notifies about changes of Device time
 *
 * @param[in] anjay  pointer to Anjay instance
 * @param[in] def  pointer to object
 */ 
void device_object_notify(anjay_t *anjay, const anjay_dm_object_def_t **def);

/**
 * Adds error code to error code list
 * If given error code is on the list, function doesn't add this error code
 * If NO_ERROR is the only list item function changes error code of this item to given error code 
 *
 * @param[in] def  pointer to Anjay instance
 * @param[in] error_code  error code (range 0-8)
 * @param[out] returns 0 on success, -1 on failure
 */ 
int device_object_add_error(const anjay_dm_object_def_t **def, const error_code_t error_code);

#endif // TIME_OBJECT_H