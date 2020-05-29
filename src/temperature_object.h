/// @file temperature_object.h
/**
 * Authon: Szymon Stolarski 2020-05-26
 */
#ifndef TEMPERATURE_OBJECT_H
#define TEMPERATURE_OBJECT_H

#include <anjay/dm.h>

/**
 * Creates Temperature object with one instance
 *
 * @param[out] Temperature object with default vaules of attributes
 */
const anjay_dm_object_def_t **temperature_object_create(void);

/**
 * Releases Temperature object
 *
 * @param[in] def  pointer to object
 */ 
void temperature_object_release(const anjay_dm_object_def_t **def);

/**
 * Notifies about changes of temperature
 *
 * @param[in] anjay  pointer to Anjay instance
 * @param[in] def  pointer to object
 */ 
void temperature_object_notify(anjay_t *anjay, const anjay_dm_object_def_t **def);


int temp_create(anjay_t* anjay);

#endif