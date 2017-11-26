/**************************************************************************
*
* Copyright 2011-2016 by Andrey Butok. FNET Community.
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
/*!
* @brief K66FN2 specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_MK66FN2_CONFIG_H_

#define _FNET_MK66FN2_CONFIG_H_

#define FNET_MK                                 (1)

/* System frequency in Hz. */
#ifndef FNET_CFG_CPU_CLOCK_HZ
    #define FNET_CFG_CPU_CLOCK_HZ               (120000000U)
#endif

/* The platform does not have second Ethernet Module.*/
#define FNET_CFG_CPU_ETH1                       (0)

/* Defines the maximum number of incoming frames that may
 *           be buffered by the Ethernet module.*/
#ifndef FNET_CFG_CPU_ETH_RX_BUFS_MAX
    #define FNET_CFG_CPU_ETH_RX_BUFS_MAX        (4u)
#endif

/* The platform has Kinetis Flash Memory Module (FTFL).*/
#define FNET_CFG_CPU_FLASH                      (1)

/* Smallest logical block which can be erased independently.*/
#define FNET_CFG_CPU_FLASH_PAGE_SIZE            (4U*1024U)        /* 4KB sector.*/

/* On-chip Flash size.*/
#define FNET_CFG_CPU_FLASH_SIZE                 (1024U * 1024U * 2U)    /* 2 MB */

#define FNET_CFG_CPU_FLASH_PROGRAM_SIZE         (8U) /*Bytes.*/

#ifndef FNET_CFG_CPU_SERIAL_PORT_DEFAULT
    #define FNET_CFG_CPU_SERIAL_PORT_DEFAULT    (0U) /* FRDMK66F board uses the default port number 0.*/
#endif

#endif
