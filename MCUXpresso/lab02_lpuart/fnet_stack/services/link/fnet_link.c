/**************************************************************************
*
* Copyright 2016 by Andrey Butok. FNET Community.
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
**********************************************************************/
#include "fnet.h"

#if FNET_CFG_LINK

#if FNET_CFG_DEBUG_LINK && FNET_CFG_DEBUG
    #define FNET_DEBUG_LINK   FNET_DEBUG
#else
    #define FNET_DEBUG_LINK(...)    do{}while(0)
#endif

/************************************************************************
*     Definitions
************************************************************************/

#define FNET_LINK_ERR_IS_INITIALIZED    "ERROR: No free service."

static void fnet_link_poll( void *fnet_link_if_p );

/************************************************************************
*    Link-Detection interface structure.
*************************************************************************/
typedef struct fnet_link_if
{
    fnet_netif_desc_t               netif;
    fnet_bool_t                     is_enabled;
    fnet_poll_desc_t                service_descriptor;

    fnet_bool_t                     connection_flag;
    fnet_link_callback_t            callback;           /* Pointer to the handler
                                                        * callback function, that is
                                                        * called when the netif has
                                                        * changed its link-connection status.*/
    void                            *callback_param;    /* Optional user-application specific parameter.*/
} fnet_link_if_t;

/* The Link-Detection service interface */
static struct fnet_link_if fnet_link_if_list[FNET_CFG_LINK_MAX];

/******************************************************************************
 * DESCRIPTION: Initializes the Link-Detection service.
 ******************************************************************************/
fnet_link_desc_t fnet_link_init( struct fnet_link_params *params )
{
    fnet_link_if_t    *link_if = FNET_NULL;
    fnet_index_t      i;

    if ((params == 0) || (params->netif_desc == 0) || (params->callback == 0))
    {
        goto ERROR;
    }

    /* Try to find free Link-Detection Service. */
    for(i = 0u; i < FNET_CFG_LINK_MAX; i++)
    {
        if(fnet_link_if_list[i].is_enabled == FNET_FALSE)
        {
            link_if = &fnet_link_if_list[i];
            break;
        }
    }

    /* Is Link-Detection service already initialized. */
    if(link_if == 0)
    {
        FNET_DEBUG_LINK(FNET_LINK_ERR_IS_INITIALIZED);
        goto ERROR;
    }

    /* Set all the fields to zero */
    fnet_memset_zero(link_if, sizeof(fnet_link_if_t));

    link_if->netif = params->netif_desc;
    link_if->callback = params->callback;
    link_if->callback_param = params->callback_param;

    link_if->service_descriptor = fnet_poll_service_register(fnet_link_poll, (void *) link_if);
    if (link_if->service_descriptor == 0)
    {
        goto ERROR;
    }

    link_if->connection_flag = FNET_FALSE;

    link_if->is_enabled = FNET_TRUE;

    return (fnet_link_desc_t)link_if;

ERROR:
    return 0;
}

/******************************************************************************
 * DESCRIPTION: Link-Detection service state machine.
 ******************************************************************************/
static void fnet_link_poll( void *fnet_link_if_p )
{
    fnet_bool_t     connected;
    fnet_link_if_t  *link_if = (fnet_link_if_t *)fnet_link_if_p;

    /* Check connection state. */
    connected = fnet_netif_is_connected(link_if->netif);
    if(link_if->connection_flag != connected)
    {
        link_if->connection_flag = connected;
        /* User callback */
        link_if->callback(link_if->netif, link_if->connection_flag, link_if->callback_param);
    }
}

/******************************************************************************
 * DESCRIPTION: Releases the Link-Detection service.
 ******************************************************************************/
void fnet_link_release(fnet_link_desc_t desc)
{
    struct fnet_link_if   *link_if = (struct fnet_link_if *) desc;

    if (link_if)
    {
        fnet_poll_service_unregister(link_if->service_descriptor); /* Delete service.*/
        link_if->is_enabled = FNET_FALSE;
    }
}

/******************************************************************************
 * DESCRIPTION: Detects if the Link-Detection service is enabled or disabled.
 ******************************************************************************/
fnet_bool_t fnet_link_is_enabled(fnet_link_desc_t desc)
{
    struct fnet_link_if   *link_if = (struct fnet_link_if *) desc;
    fnet_bool_t             result;

    if(link_if)
    {
        result = link_if->is_enabled;
    }
    else
    {
        result = FNET_FALSE;
    }

    return result;
}

#endif /* FNET_CFG_LINK */
