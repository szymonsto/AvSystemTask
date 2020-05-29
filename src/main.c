/// @file main.c
/**
 * Authon: Szymon Stolarski 2020-05-26
 */
#include <anjay/anjay.h>
#include <avsystem/commons/avs_log.h>
#include <anjay/attr_storage.h>
#include <anjay/security.h>
#include <anjay/server.h>

#include <poll.h>

#include "device_object.h"
#include "connectivity_object.h"
#include "temperature_object.h"
#include "avst_utils.h"

int main_loop(anjay_t *anjay, const anjay_dm_object_def_t **device_object, const anjay_dm_object_def_t **temperature_object)
{
    while (true)
    {
        // Obtain all network data sources
        AVS_LIST(avs_net_socket_t *const)
        sockets = anjay_get_sockets(anjay);

        // Prepare to poll() on them
        size_t numsocks = AVS_LIST_SIZE(sockets);
        struct pollfd pollfds[numsocks];
        size_t i = 0;
        AVS_LIST(avs_net_socket_t *const)
        sock;
        AVS_LIST_FOREACH(sock, sockets)
        {
            pollfds[i].fd = *(const int *)avs_net_socket_get_system(*sock);
            pollfds[i].events = POLLIN;
            pollfds[i].revents = 0;
            ++i;
        }

        const int max_wait_time_ms = 1000;
        // Determine the expected time to the next job in milliseconds.
        // If there is no job we will wait till something arrives for
        // at most 1 second (i.e. max_wait_time_ms).
        int wait_ms =
            anjay_sched_calculate_wait_time_ms(anjay, max_wait_time_ms);

        // Wait for the events if necessary, and handle them.
        if (poll(pollfds, numsocks, wait_ms) > 0)
        {
            int socket_id = 0;
            AVS_LIST(avs_net_socket_t *const)
            socket = NULL;
            AVS_LIST_FOREACH(socket, sockets)
            {
                if (pollfds[socket_id].revents)
                {
                    if (anjay_serve(anjay, *socket))
                    {
                        avs_log(tutorial, ERROR, "anjay_serve failed");
                    }
                }
                ++socket_id;
            }
        }

        device_object_notify(anjay, device_object); //notifies about time changes
        temperature_object_notify(anjay, temperature_object);

        // Finally run the scheduler
        anjay_sched_run(anjay);
    }
    return 0;
}

//! Installs Security Object and adds and instance of it.
//! An instance of Security Object provides information needed to connect to
//! LwM2M server.
static int setup_security_object(anjay_t *anjay, const char* psk_identity, const char* server_url, const char* password)
{
    if (anjay_security_object_install(anjay))
    {
        return -1;
    }

    const char* PSK_IDENTITY = psk_identity;
    const char* PSK_KEY = password;
    //+10 if for protocor type (coaps or coap)
    char *server_uri= (char *) avs_malloc(strlen(server_url) + 10);
        strcat(server_uri, "coaps://");
        strcat(server_uri, server_url);


     anjay_security_instance_t security_instance = {
            .ssid = 1,
            .server_uri = server_uri,
            .security_mode = ANJAY_SECURITY_PSK,
            .public_cert_or_psk_identity = (const uint8_t *)PSK_IDENTITY,
            .public_cert_or_psk_identity_size = strlen(PSK_IDENTITY),
            .private_cert_or_psk_key = (const uint8_t *)PSK_KEY,
            .private_cert_or_psk_key_size = strlen(PSK_KEY)};      


    // Anjay will assign Instance ID automatically
    anjay_iid_t security_instance_id = ANJAY_ID_INVALID;
    if (anjay_security_object_add_instance(anjay, &security_instance,
                                           &security_instance_id))
    {
        avs_free(server_uri);
        return -1;
    }
     avs_free(server_uri);
    return 0;
}



//! Installs Server Object and adds and instance of it.
//! An instance of Server Object provides the data related to a LwM2M Server.
static int setup_server_object(anjay_t *anjay)
{
    if (anjay_server_object_install(anjay))
    {
        return -1;
    }

    const anjay_server_instance_t server_instance = {
        // Server Short ID
        .ssid = 1,
        // Client will send Update message often than every 60 seconds
        .lifetime = 60,
        // Disable Default Minimum Period resource
        .default_min_period = -1,
        // Disable Default Maximum Period resource
        .default_max_period = -1,
        // Disable Disable Timeout resource
        .disable_timeout = -1,
        // Sets preferred transport to UDP
        .binding = "U"};

    // Anjay will assign Instance ID automatically
    anjay_iid_t server_instance_id = ANJAY_ID_INVALID;
    if (anjay_server_object_add_instance(anjay, &server_instance,
                                         &server_instance_id))
    {
        return -1;
    }

    return 0;
}




int main(int argc, char *argv[])
{
    if(argc < 3){
        avs_log(tutorial, ERROR, "Too few command line arguments [endpoint_name server_url password]");
        return -1;
    }
    const char* endpoint_name = argv[1];
    const char* server_url = argv[2];
    const char* password = argv[3];

    const anjay_configuration_t CONFIG = {
        .endpoint_name = endpoint_name,
        .in_buffer_size = 4000,
        .out_buffer_size = 4000,
        .msg_cache_size = 4000};

    anjay_t *anjay = anjay_new(&CONFIG);
    if (!anjay)
    {
        avs_log(tutorial, ERROR, "Could not create Anjay object");
        return -1;
    }

    int result = 0;
    // Install Attribute storage and setup necessary objects
    if (anjay_attr_storage_install(anjay) || setup_security_object(anjay, endpoint_name, server_url, password) || setup_server_object(anjay))
    {
        result = -1;
    }

    const anjay_dm_object_def_t **device_object = NULL;
    if (!result)
    {
        device_object = device_object_create();
        if (device_object)
        {
            result = anjay_register_object(anjay, device_object);
        }
        else
        {
            result = -1;
        }
    }

const anjay_dm_object_def_t **temperature_object = NULL;
    if (!result)
    {
        temperature_object = temperature_object_create();
        if (temperature_object)
        {
            result = anjay_register_object(anjay, temperature_object);
        }
        else
        {
            result = -1;
        }
    }

    const anjay_dm_object_def_t **connectivity_object = NULL;
    if (!result)
    {
        connectivity_object = connectivity_monitoring_object_create();
        if (connectivity_object)
        {
            result = anjay_register_object(anjay, connectivity_object);
        }
        else
        {
            result = -1;
        }
    }

    if (!result)
    {
        result = main_loop(anjay, device_object, temperature_object);
    }

    anjay_delete(anjay);
    device_object_release(device_object);
    connectivity_monitoring_object_release(connectivity_object);
    temperature_object_release(temperature_object);

    return 0;
}