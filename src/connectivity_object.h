/// @file connectiity_object.h
/**
 * Authon: Szymon Stolarski 2020-05-26
 */
#ifndef CONNECTIVITY_OBJECT_H
#define CONNECTIVITY_OBJECT_H

#include <anjay/dm.h>

/**
 * Creates Connectivity Monitoring object
 *
 * @param[out] Connectivity Monitoring object with default vaules of attributes
 */
const anjay_dm_object_def_t **connectivity_monitoring_object_create(void);

/**
 * Releases Connectivity Monitoring object
 *
 * @param[in] def  pointer to object
 */ 
void connectivity_monitoring_object_release(const anjay_dm_object_def_t **def);

#endif